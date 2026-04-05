#include <catch2/catch.hpp>

#include <megopp/memory/atomic_shared_ptr.hpp>

#include <atomic>
#include <memory>
#include <thread>
#include <vector>

using mgpp::mem::atomic_shared_ptr;

namespace {

struct Widget {
    int id;
    explicit Widget(int i) : id(i) {}
};

} // namespace

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

TEST_CASE("atomic_shared_ptr - default construction yields null", "[atomic_shared_ptr]")
{
    atomic_shared_ptr<Widget> ap;
    REQUIRE(ap.load() == nullptr);
}

TEST_CASE("atomic_shared_ptr - construct from shared_ptr", "[atomic_shared_ptr]")
{
    auto sp = std::make_shared<Widget>(42);
    atomic_shared_ptr<Widget> ap(sp);
    REQUIRE(ap.load() == sp);
    REQUIRE(ap.load()->id == 42);
}

// ---------------------------------------------------------------------------
// store / load
// ---------------------------------------------------------------------------

TEST_CASE("atomic_shared_ptr - store and load round-trips the pointer", "[atomic_shared_ptr]")
{
    atomic_shared_ptr<Widget> ap;
    auto sp = std::make_shared<Widget>(7);
    ap.store(sp);
    REQUIRE(ap.load() == sp);
}

TEST_CASE("atomic_shared_ptr - store replaces the previous value", "[atomic_shared_ptr]")
{
    auto sp1 = std::make_shared<Widget>(1);
    auto sp2 = std::make_shared<Widget>(2);
    atomic_shared_ptr<Widget> ap(sp1);
    ap.store(sp2);
    REQUIRE(ap.load() == sp2);
}

TEST_CASE("atomic_shared_ptr - store and load with explicit memory orders", "[atomic_shared_ptr]")
{
    atomic_shared_ptr<Widget> ap;
    auto sp = std::make_shared<Widget>(77);
    ap.store(sp, std::memory_order_release);
    REQUIRE(ap.load(std::memory_order_acquire) == sp);
}

// ---------------------------------------------------------------------------
// operator= / implicit conversion
// ---------------------------------------------------------------------------

TEST_CASE("atomic_shared_ptr - operator= from shared_ptr stores value", "[atomic_shared_ptr]")
{
    atomic_shared_ptr<Widget> ap;
    auto sp = std::make_shared<Widget>(99);
    ap = sp;
    REQUIRE(ap.load() == sp);
}

TEST_CASE("atomic_shared_ptr - implicit conversion to shared_ptr", "[atomic_shared_ptr]")
{
    auto sp = std::make_shared<Widget>(3);
    atomic_shared_ptr<Widget> ap(sp);
    std::shared_ptr<Widget> sp2 = ap;
    REQUIRE(sp2 == sp);
    REQUIRE(sp2->id == 3);
}

// ---------------------------------------------------------------------------
// exchange
// ---------------------------------------------------------------------------

TEST_CASE("atomic_shared_ptr - exchange returns old value and stores new", "[atomic_shared_ptr]")
{
    auto sp1 = std::make_shared<Widget>(10);
    auto sp2 = std::make_shared<Widget>(20);
    atomic_shared_ptr<Widget> ap(sp1);

    auto old = ap.exchange(sp2);

    REQUIRE(old == sp1);
    REQUIRE(ap.load() == sp2);
}

// ---------------------------------------------------------------------------
// compare_exchange_strong
// ---------------------------------------------------------------------------

TEST_CASE("atomic_shared_ptr - compare_exchange_strong succeeds when expected matches", "[atomic_shared_ptr]")
{
    auto sp1 = std::make_shared<Widget>(1);
    auto sp2 = std::make_shared<Widget>(2);
    atomic_shared_ptr<Widget> ap(sp1);

    auto expected = sp1;
    bool ok = ap.compare_exchange_strong(expected, sp2);

    REQUIRE(ok == true);
    REQUIRE(ap.load() == sp2);
}

TEST_CASE("atomic_shared_ptr - compare_exchange_strong fails and updates expected", "[atomic_shared_ptr]")
{
    auto sp1 = std::make_shared<Widget>(1);
    auto sp2 = std::make_shared<Widget>(2);
    auto sp3 = std::make_shared<Widget>(3);
    atomic_shared_ptr<Widget> ap(sp1);

    auto expected = sp2; // wrong — doesn't match stored sp1
    bool ok = ap.compare_exchange_strong(expected, sp3);

    REQUIRE(ok == false);
    REQUIRE(expected == sp1);  // expected updated to current value
    REQUIRE(ap.load() == sp1); // stored value unchanged
}

TEST_CASE("atomic_shared_ptr - compare_exchange_strong with explicit memory orders succeeds", "[atomic_shared_ptr]")
{
    auto sp1 = std::make_shared<Widget>(5);
    auto sp2 = std::make_shared<Widget>(6);
    atomic_shared_ptr<Widget> ap(sp1);

    auto expected = sp1;
    bool ok = ap.compare_exchange_strong(expected, sp2,
        std::memory_order_acq_rel, std::memory_order_acquire);

    REQUIRE(ok == true);
    REQUIRE(ap.load() == sp2);
}

// ---------------------------------------------------------------------------
// compare_exchange_weak
// ---------------------------------------------------------------------------

TEST_CASE("atomic_shared_ptr - compare_exchange_weak eventually succeeds", "[atomic_shared_ptr]")
{
    auto sp1 = std::make_shared<Widget>(10);
    auto sp2 = std::make_shared<Widget>(20);
    atomic_shared_ptr<Widget> ap(sp1);

    auto expected = sp1;
    while (!ap.compare_exchange_weak(expected, sp2))
        expected = sp1; // reset after spurious failure

    REQUIRE(ap.load() == sp2);
}

TEST_CASE("atomic_shared_ptr - compare_exchange_weak with explicit memory orders", "[atomic_shared_ptr]")
{
    auto sp1 = std::make_shared<Widget>(11);
    auto sp2 = std::make_shared<Widget>(22);
    atomic_shared_ptr<Widget> ap(sp1);

    auto expected = sp1;
    while (!ap.compare_exchange_weak(expected, sp2,
            std::memory_order_release, std::memory_order_relaxed))
        expected = sp1;

    REQUIRE(ap.load() == sp2);
}

// ---------------------------------------------------------------------------
// is_lock_free
// ---------------------------------------------------------------------------

TEST_CASE("atomic_shared_ptr - is_lock_free is callable", "[atomic_shared_ptr]")
{
    atomic_shared_ptr<Widget> ap;
    bool lf = ap.is_lock_free();
    (void)lf; // value is platform-defined; we only verify it compiles and runs
}

// ---------------------------------------------------------------------------
// Concurrency smoke test
// ---------------------------------------------------------------------------

TEST_CASE("atomic_shared_ptr - concurrent store/load smoke test", "[atomic_shared_ptr]")
{
    atomic_shared_ptr<Widget> ap(std::make_shared<Widget>(0));

    constexpr int kThreads = 4;
    constexpr int kIters   = 500;
    std::atomic<bool> all_non_null{true};

    std::vector<std::thread> threads;
    threads.reserve(kThreads);

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&ap, &all_non_null, t, kIters]() {
            for (int i = 0; i < kIters; ++i) {
                ap.store(std::make_shared<Widget>(t * kIters + i));
                if (ap.load() == nullptr)
                    all_non_null.store(false, std::memory_order_relaxed);
            }
        });
    }

    for (auto& th : threads)
        th.join();

    REQUIRE(all_non_null.load());
}
