#include <catch2/catch.hpp>
#include <megopp/thrd/spin_mutex.h>

#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

using mgpp::thrd::spin_mutex;

TEST_CASE("spin_mutex - lock and unlock round-trip", "[spin_mutex]")
{
    spin_mutex m;
    REQUIRE_NOTHROW(m.lock());
    REQUIRE_NOTHROW(m.unlock());
}

TEST_CASE("spin_mutex - try_lock succeeds when mutex is free", "[spin_mutex]")
{
    spin_mutex m;
    REQUIRE(m.try_lock());
    m.unlock();
}

TEST_CASE("spin_mutex - try_lock fails from another thread while held", "[spin_mutex]")
{
    spin_mutex m;
    m.lock();

    bool result = true;
    std::thread t([&] { result = m.try_lock(); });
    t.join();

    REQUIRE_FALSE(result);
    m.unlock();
}

TEST_CASE("spin_mutex - re-locking from same thread throws logic_error", "[spin_mutex]")
{
    spin_mutex m;
    m.lock();
    REQUIRE_THROWS_AS(m.lock(), std::logic_error);
    m.unlock();
}

TEST_CASE("spin_mutex - re-try_lock from same thread throws logic_error", "[spin_mutex]")
{
    spin_mutex m;
    m.lock();
    REQUIRE_THROWS_AS(m.try_lock(), std::logic_error);
    m.unlock();
}

TEST_CASE("spin_mutex - unlock from non-owning thread throws logic_error", "[spin_mutex]")
{
    spin_mutex m;
    m.lock();

    std::exception_ptr ep;
    std::thread t([&] {
        try {
            m.unlock();
        } catch (...) {
            ep = std::current_exception();
        }
    });
    t.join();

    // Restore state
    m.unlock();

    REQUIRE(ep != nullptr);
    try {
        std::rethrow_exception(ep);
    } catch (const std::logic_error&) {
        SUCCEED();
    } catch (...) {
        FAIL("Expected std::logic_error");
    }
}

TEST_CASE("spin_mutex - concurrent counter increments produce correct result", "[spin_mutex]")
{
    spin_mutex m;
    int counter = 0;
    constexpr int N_THREADS = 4;
    constexpr int N_INC = 500;

    std::vector<std::thread> threads;
    threads.reserve(N_THREADS);
    for (int i = 0; i < N_THREADS; ++i) {
        threads.emplace_back([&] {
            for (int j = 0; j < N_INC; ++j) {
                m.lock();
                ++counter;
                m.unlock();
            }
        });
    }
    for (auto& t : threads)
        t.join();

    REQUIRE(counter == N_THREADS * N_INC);
}

TEST_CASE("spin_mutex - usable with std::lock_guard via std::unique_lock", "[spin_mutex]")
{
    spin_mutex m;
    {
        std::unique_lock<spin_mutex> lk{m};
        REQUIRE(lk.owns_lock());
    }
    // After scope, mutex is unlocked; can lock again
    REQUIRE_NOTHROW(m.lock());
    m.unlock();
}
