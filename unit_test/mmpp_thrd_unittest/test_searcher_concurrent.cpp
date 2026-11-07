#include <catch2/catch.hpp>

#include <memepp/searcher.hpp>

#include <atomic>
#include <cstring>
#include <string>
#include <thread>
#include <vector>

// ============================================================================
// Thread safety of memepp::searcher.
//
// find() normally bumps the hit/miss counters at the end of every call:
//
//     if (result >= 0) hit_count++;
//     call_count++;
//     if (call_count >= 4) density = ...;
//
// Those are plain (non-atomic) read-modify-writes, so two threads calling
// find() on the SAME searcher race on them.  The match position itself is
// still computed correctly -- the search path only reads the needle and the
// prebuilt tables.
//
// searcher_flag_t::disable_density turns the bookkeeping off, after which
// find() writes nothing to the object and concurrent use is safe.
// ============================================================================

namespace {

// The search results are always compared against an expected value inside the
// worker loops, so the compiler cannot elide the calls.

} // namespace

TEST_CASE("memepp::searcher: concurrent find with disable_density",
          "[searcher][thread]")
{
    // One searcher shared by several threads, each searching its own text.
    memepp::searcher s("needle", memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(s.valid());
    s.set_flags(memepp::searcher_flag_t::disable_density);
    REQUIRE(s.flags() == memepp::searcher_flag_t::disable_density);

    const std::string hay_a = "aaaa needles aaaa";     // hit at 5
    const std::string hay_b = "bbbb NEEDLE  bbbb";     // hit at 5
    const std::string hay_c = "ccccc nothing ccccc";   // miss

    const int kThreads = 8;
    const int kIters   = 2000;

    std::atomic<int> bad{0};
    std::vector<std::thread> workers;
    workers.reserve(kThreads);

    for (int t = 0; t < kThreads; ++t)
    {
        workers.emplace_back([&, t]() {
            const std::string* hay = (t % 3 == 0) ? &hay_a
                                  : (t % 3 == 1) ? &hay_b
                                                 : &hay_c;
            const bool expect_hit = (t % 3 != 2);
            for (int i = 0; i < kIters; ++i)
            {
                const auto pos = s.find(
                    reinterpret_cast<const MemeByte_t*>(hay->data()),
                    static_cast<mmint_t>(hay->size()));
                if (expect_hit ? (pos != 5) : (pos != memepp::searcher::npos))
                    ++bad;
            }
        });
    }
    for (auto& w : workers) w.join();

    REQUIRE(bad.load() == 0);

    // Nothing was counted, so density stayed at its initial value.
    REQUIRE(s.density() == memepp::search_density_t::unknown);
}

TEST_CASE("memepp::searcher: concurrent find on the Boyer-Moore path",
          "[searcher][thread]")
{
    // Long, case-insensitive needle -> Boyer-Moore with heap tables.
    std::string needle(200, 'a');
    std::string big = "xx" + needle + "yy";

    memepp::searcher s(
        reinterpret_cast<const MemeByte_t*>(needle.data()),
        static_cast<mmint_t>(needle.size()),
        memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(s.valid());
    s.set_flags(memepp::searcher_flag_t::disable_density);

    const int kThreads = 8;
    const int kIters   = 500;

    std::atomic<int> bad{0};
    std::vector<std::thread> workers;
    for (int t = 0; t < kThreads; ++t)
    {
        workers.emplace_back([&]() {
            for (int i = 0; i < kIters; ++i)
            {
                if (s.find(reinterpret_cast<const MemeByte_t*>(big.data()),
                           static_cast<mmint_t>(big.size())) != 2)
                    ++bad;
            }
        });
    }
    for (auto& w : workers) w.join();

    REQUIRE(bad.load() == 0);
}

TEST_CASE("memepp::searcher: concurrent find without the flag counts racy",
          "[searcher][thread]")
{
    // Documents the default behaviour: find() is still correct, but the
    // density counters are shared mutable state, so this test only asserts
    // the match positions -- never the counter values.
    memepp::searcher s("needle");
    REQUIRE(s.valid());
    REQUIRE(s.flags() == memepp::searcher_flag_t::none);

    const std::string hay = "aaaa needles aaaa";
    const int kThreads = 8;
    const int kIters   = 1000;

    std::atomic<int> bad{0};
    std::vector<std::thread> workers;
    for (int t = 0; t < kThreads; ++t)
    {
        workers.emplace_back([&]() {
            for (int i = 0; i < kIters; ++i)
                if (s.find(reinterpret_cast<const MemeByte_t*>(hay.data()),
                           static_cast<mmint_t>(hay.size())) != 5)
                    ++bad;
        });
    }
    for (auto& w : workers) w.join();

    REQUIRE(bad.load() == 0);
}

TEST_CASE("memepp::searcher: copied searcher is independent across threads",
          "[searcher][thread]")
{
    // A copy owns its own tables, so two threads can each use their own copy
    // of the same original without any flag at all -- including the default
    // counter updates, because the counters live in separate objects.
    memepp::searcher original("brown");
    REQUIRE(original.valid());

    const std::string hay = "the quick brown fox";
    const int kIters = 2000;

    std::atomic<int> bad{0};

    std::thread t1([&]() {
        memepp::searcher mine(original);
        for (int i = 0; i < kIters; ++i)
            if (mine.find(reinterpret_cast<const MemeByte_t*>(hay.data()),
                          static_cast<mmint_t>(hay.size())) != 10)
                ++bad;
        // Each copy counts independently, so density is meaningful here.
        if (mine.density() != memepp::search_density_t::frequent)
            ++bad;
    });

    std::thread t2([&]() {
        memepp::searcher mine(original);
        for (int i = 0; i < kIters; ++i)
            if (mine.find(reinterpret_cast<const MemeByte_t*>(hay.data()),
                          static_cast<mmint_t>(hay.size())) != 10)
                ++bad;
        if (mine.density() != memepp::search_density_t::frequent)
            ++bad;
    });

    t1.join();
    t2.join();

    REQUIRE(bad.load() == 0);

    // The original was never searched, so its counters are untouched.
    REQUIRE(original.density() == memepp::search_density_t::unknown);
}
