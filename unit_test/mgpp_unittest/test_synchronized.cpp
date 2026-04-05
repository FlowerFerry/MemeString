#include <catch2/catch.hpp>
#include <megopp/thrd/synchronized.h>

#include <string>
#include <thread>
#include <vector>

using mgpp::thrd::synchronized_value;
using mgpp::thrd::shared_synchronized_value;

// ============================================================
// synchronized_value (std::mutex, exclusive locking)
// ============================================================

TEST_CASE("synchronized_value - rlock provides read access", "[synchronized]")
{
    synchronized_value<int> sv{42};
    auto r = sv.rlock();
    REQUIRE(*r == 42);
}

TEST_CASE("synchronized_value - wlock provides write access", "[synchronized]")
{
    synchronized_value<int> sv{0};
    {
        auto w = sv.wlock();
        *w = 99;
    }
    auto r = sv.rlock();
    REQUIRE(*r == 99);
}

TEST_CASE("synchronized_value - wlock arrow operator works", "[synchronized]")
{
    synchronized_value<std::string> sv{"hello"};
    {
        auto w = sv.wlock();
        w->append(" world");
    }
    auto r = sv.rlock();
    REQUIRE(*r == "hello world");
}

TEST_CASE("synchronized_value - with_rlock functor", "[synchronized]")
{
    synchronized_value<int> sv{7};
    int result = sv.with_rlock([](const int* v) { return *v; });
    REQUIRE(result == 7);
}

TEST_CASE("synchronized_value - with_wlock functor", "[synchronized]")
{
    synchronized_value<int> sv{0};
    sv.with_wlock([](int* v) { *v = 55; });
    int result = sv.with_rlock([](const int* v) { return *v; });
    REQUIRE(result == 55);
}

TEST_CASE("synchronized_value - constructor forwarding", "[synchronized]")
{
    synchronized_value<std::string> sv{"hello"};
    auto r = sv.rlock();
    REQUIRE(*r == "hello");
}

TEST_CASE("synchronized_value - mutex() returns non-null pointer", "[synchronized]")
{
    synchronized_value<int> sv;
    REQUIRE(sv.mutex() != nullptr);
}

TEST_CASE("synchronized_value - rlock arrow operator works", "[synchronized]")
{
    synchronized_value<std::string> sv{"test"};
    auto r = sv.rlock();
    REQUIRE(r->size() == 4);
}

// ============================================================
// shared_synchronized_value (std::shared_mutex, shared/exclusive)
// ============================================================

TEST_CASE("shared_synchronized_value - rlock allows shared read access", "[synchronized]")
{
    shared_synchronized_value<int> sv{10};
    auto r = sv.rlock();
    REQUIRE(*r == 10);
}

TEST_CASE("shared_synchronized_value - wlock allows exclusive write access", "[synchronized]")
{
    shared_synchronized_value<int> sv{0};
    {
        auto w = sv.wlock();
        *w = 100;
    }
    auto r = sv.rlock();
    REQUIRE(*r == 100);
}

TEST_CASE("shared_synchronized_value - concurrent writers produce correct result", "[synchronized]")
{
    shared_synchronized_value<int> sv{0};
    constexpr int N_THREADS = 4;
    constexpr int N_INC = 100;

    std::vector<std::thread> threads;
    threads.reserve(N_THREADS);
    for (int i = 0; i < N_THREADS; ++i) {
        threads.emplace_back([&] {
            for (int j = 0; j < N_INC; ++j) {
                auto w = sv.wlock();
                ++(*w);
            }
        });
    }
    for (auto& t : threads)
        t.join();

    auto r = sv.rlock();
    REQUIRE(*r == N_THREADS * N_INC);
}

TEST_CASE("shared_synchronized_value - ulock upgrade to write", "[synchronized]")
{
    shared_synchronized_value<int> sv{10};
    {
        auto u = sv.ulock();
        REQUIRE(*u == 10);
        auto w = u.upgrade();
        *w = 20;
    }
    auto r = sv.rlock();
    REQUIRE(*r == 20);
}

TEST_CASE("shared_synchronized_value - multiple rlocks can coexist", "[synchronized]")
{
    shared_synchronized_value<int> sv{5};
    // Two shared read locks on the same value
    auto r1 = sv.rlock();
    auto r2 = sv.rlock();
    REQUIRE(*r1 == 5);
    REQUIRE(*r2 == 5);
}
