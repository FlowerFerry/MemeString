#include <catch2/catch.hpp>

#include <megopp/memory/cow_ptr.h>

#include <atomic>
#include <memory>
#include <thread>
#include <vector>

namespace {

struct Data {
    int value{0};
};

} // namespace

using Cow = mgpp::mem::cow_ptr<Data>;

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

TEST_CASE("cow_ptr - default construction is null", "[cow_ptr]")
{
    Cow p;
    REQUIRE(p.read() == nullptr);
}

TEST_CASE("cow_ptr - nullptr construction is null", "[cow_ptr]")
{
    Cow p(nullptr);
    REQUIRE(p.read() == nullptr);
}

TEST_CASE("cow_ptr - construct from raw pointer", "[cow_ptr]")
{
    Cow p(new Data{42});
    REQUIRE(p.read() != nullptr);
    REQUIRE(p.read()->value == 42);
}

TEST_CASE("cow_ptr - construct from shared_ptr", "[cow_ptr]")
{
    auto sp = std::make_shared<Data>();
    sp->value = 7;
    Cow p(sp);
    REQUIRE(p.read()->value == 7);
}

TEST_CASE("cow_ptr - construct from unique_ptr", "[cow_ptr]")
{
    auto up = std::make_unique<Data>();
    up->value = 55;
    Cow p(std::move(up));
    REQUIRE(p.read() != nullptr);
    REQUIRE(p.read()->value == 55);
}

TEST_CASE("cow_ptr - copy construction shares the pointed-to object", "[cow_ptr]")
{
    Cow p1(std::make_shared<Data>());
    Cow p2(p1);
    REQUIRE(p2.read() == p1.read());
}

TEST_CASE("cow_ptr - move construction transfers ownership and leaves source empty", "[cow_ptr]")
{
    Cow p1(std::make_shared<Data>());
    auto original = p1.read();
    Cow p2(std::move(p1));
    REQUIRE(p2.read() == original);
    REQUIRE(p1.read() == nullptr);
}

// ---------------------------------------------------------------------------
// read
// ---------------------------------------------------------------------------

TEST_CASE("cow_ptr - read returns shared_ptr<const T>", "[cow_ptr]")
{
    Cow p(std::make_shared<Data>());
    std::shared_ptr<const Data> r = p.read();
    REQUIRE(r != nullptr);
}

// ---------------------------------------------------------------------------
// write
// ---------------------------------------------------------------------------

TEST_CASE("cow_ptr - write creates a COW copy and applies the mutation", "[cow_ptr]")
{
    auto sp = std::make_shared<Data>();
    Cow p(sp);

    auto result = p.write([](std::shared_ptr<Data>& ptr) {
        ptr->value = 99;
    });

    REQUIRE(result != nullptr);
    REQUIRE(result->value == 99);
    REQUIRE(sp->value == 0);          // original object unchanged
    REQUIRE(p.read()->value == 99);
}

TEST_CASE("cow_ptr - write with bool fn returning true applies update", "[cow_ptr]")
{
    Cow p(std::make_shared<Data>());

    p.write([](std::shared_ptr<Data>& ptr) -> bool {
        ptr->value = 42;
        return true;
    });

    REQUIRE(p.read()->value == 42);
}

TEST_CASE("cow_ptr - write with bool fn returning false cancels the update", "[cow_ptr]")
{
    Cow p(std::make_shared<Data>());
    auto before = p.read();

    auto result = p.write([](std::shared_ptr<Data>& ptr) -> bool {
        ptr->value = 99;
        return false;
    });

    REQUIRE(result == before);
    REQUIRE(p.read()->value == 0);
}

// ---------------------------------------------------------------------------
// write_if
// ---------------------------------------------------------------------------

TEST_CASE("cow_ptr - write_if skips mutation when condition returns false", "[cow_ptr]")
{
    Cow p(std::make_shared<Data>());

    p.write_if(
        [](const auto&) -> bool { return false; },
        [](std::shared_ptr<Data>& ptr) { ptr->value = 77; });

    REQUIRE(p.read()->value == 0);
}

TEST_CASE("cow_ptr - write_if applies mutation when condition returns true", "[cow_ptr]")
{
    Cow p(std::make_shared<Data>());

    p.write_if(
        [](const auto&) -> bool { return true; },
        [](std::shared_ptr<Data>& ptr) { ptr->value = 88; });

    REQUIRE(p.read()->value == 88);
}

TEST_CASE("cow_ptr - write_if condition receives current value", "[cow_ptr]")
{
    Cow p(std::make_shared<Data>());
    p.write([](std::shared_ptr<Data>& ptr) { ptr->value = 5; });

    // Condition: only update if current value is 5
    p.write_if(
        [](const auto& cur) -> bool { return cur && cur->value == 5; },
        [](std::shared_ptr<Data>& ptr) { ptr->value = 10; });

    REQUIRE(p.read()->value == 10);
}

// ---------------------------------------------------------------------------
// release
// ---------------------------------------------------------------------------

TEST_CASE("cow_ptr - release returns the shared_ptr and clears the cow_ptr", "[cow_ptr]")
{
    auto sp = std::make_shared<Data>();
    sp->value = 5;
    Cow p(sp);

    auto released = p.release();

    REQUIRE(released == sp);
    REQUIRE(p.read() == nullptr);
}

// ---------------------------------------------------------------------------
// Assignment
// ---------------------------------------------------------------------------

TEST_CASE("cow_ptr - copy assignment shares the pointed-to object", "[cow_ptr]")
{
    Cow p1(std::make_shared<Data>());
    Cow p2;
    p2 = p1;
    REQUIRE(p2.read() == p1.read());
}

TEST_CASE("cow_ptr - move assignment transfers ownership and leaves source empty", "[cow_ptr]")
{
    Cow p1(std::make_shared<Data>());
    auto original = p1.read();
    Cow p2;
    p2 = std::move(p1);
    REQUIRE(p2.read() == original);
    REQUIRE(p1.read() == nullptr);
}

TEST_CASE("cow_ptr - operator= from shared_ptr", "[cow_ptr]")
{
    Cow p;
    auto sp = std::make_shared<Data>();
    sp->value = 33;
    p = sp;
    REQUIRE(p.read()->value == 33);
}

TEST_CASE("cow_ptr - operator= from unique_ptr", "[cow_ptr]")
{
    Cow p;
    auto up = std::make_unique<Data>();
    up->value = 44;
    p = std::move(up);
    REQUIRE(p.read()->value == 44);
}

// ---------------------------------------------------------------------------
// Concurrency smoke test
// ---------------------------------------------------------------------------

TEST_CASE("cow_ptr - concurrent reads and writes smoke test", "[cow_ptr]")
{
    Cow p(std::make_shared<Data>());

    constexpr int kThreads = 4;
    constexpr int kIters   = 200;
    std::atomic<bool> all_ok{true};

    std::vector<std::thread> threads;
    threads.reserve(kThreads * 2);

    // Writers
    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&p, t, kIters]() {
            for (int i = 0; i < kIters; ++i) {
                p.write([t, kIters, i](std::shared_ptr<Data>& ptr) {
                    ptr->value = t * kIters + i;
                });
            }
        });
    }

    // Readers
    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&p, &all_ok, kIters]() {
            for (int i = 0; i < kIters; ++i) {
                if (p.read() == nullptr)
                    all_ok.store(false, std::memory_order_relaxed);
            }
        });
    }

    for (auto& th : threads)
        th.join();

    REQUIRE(all_ok.load());
}
