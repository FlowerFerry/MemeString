#include <catch2/catch.hpp>
#include <megopp/util/scope_locker.h>

#include <mutex>
#include <shared_mutex>

using mgpp::util::scope_unique_locker;
using mgpp::util::scope_shared_locker;
using mgpp::util::rw_lock;
using mgpp::util::scope_locker;

// ============================================================
// scope_unique_locker
// ============================================================

TEST_CASE("scope_unique_locker - acquires lock when not already held", "[scope_locker]")
{
    std::mutex m;
    std::unique_lock<std::mutex> lk{m, std::defer_lock};
    REQUIRE_FALSE(lk.owns_lock());
    {
        scope_unique_locker<std::mutex> sl{lk};
        REQUIRE(lk.owns_lock());
    }
    // src_owns_lock_ was false, so it gets released on exit
    REQUIRE_FALSE(lk.owns_lock());
}

TEST_CASE("scope_unique_locker - does not re-acquire when already held, restores on exit", "[scope_locker]")
{
    std::mutex m;
    std::unique_lock<std::mutex> lk{m};
    REQUIRE(lk.owns_lock());
    {
        scope_unique_locker<std::mutex> sl{lk};
        REQUIRE(lk.owns_lock());
    }
    // src_owns_lock_ was true, lock is still held after scope
    REQUIRE(lk.owns_lock());
}

TEST_CASE("scope_unique_locker - defer_lock temporarily unlocks held lock", "[scope_locker]")
{
    std::mutex m;
    std::unique_lock<std::mutex> lk{m};
    REQUIRE(lk.owns_lock());
    {
        scope_unique_locker<std::mutex> sl{lk, std::defer_lock};
        REQUIRE_FALSE(lk.owns_lock());
    }
    // src_owns_lock_ was true: restores the lock on exit
    REQUIRE(lk.owns_lock());
}

TEST_CASE("scope_unique_locker - lock/unlock methods work", "[scope_locker]")
{
    std::mutex m;
    std::unique_lock<std::mutex> lk{m, std::defer_lock};
    scope_unique_locker<std::mutex> sl{lk};
    REQUIRE(lk.owns_lock());

    sl.unlock();
    REQUIRE_FALSE(lk.owns_lock());

    sl.lock();
    REQUIRE(lk.owns_lock());
}

// ============================================================
// scope_shared_locker
// ============================================================

TEST_CASE("scope_shared_locker - acquires shared lock when not already held", "[scope_locker]")
{
    std::shared_mutex sm;
    std::shared_lock<std::shared_mutex> lk{sm, std::defer_lock};
    REQUIRE_FALSE(lk.owns_lock());
    {
        scope_shared_locker<std::shared_mutex> sl{lk};
        REQUIRE(lk.owns_lock());
    }
    REQUIRE_FALSE(lk.owns_lock());
}

TEST_CASE("scope_shared_locker - defer_lock temporarily unlocks held shared lock", "[scope_locker]")
{
    std::shared_mutex sm;
    std::shared_lock<std::shared_mutex> lk{sm};
    REQUIRE(lk.owns_lock());
    {
        scope_shared_locker<std::shared_mutex> sl{lk, std::defer_lock};
        REQUIRE_FALSE(lk.owns_lock());
    }
    REQUIRE(lk.owns_lock());
}

// ============================================================
// rw_lock
// ============================================================

TEST_CASE("rw_lock - default constructor acquires exclusive lock", "[scope_locker]")
{
    std::shared_mutex sm;
    {
        rw_lock<std::shared_mutex> rl{sm};
        REQUIRE(rl.owns_lock());
    }
    // after scope, exclusive lock released; can re-lock
    REQUIRE_NOTHROW(sm.lock());
    sm.unlock();
}

TEST_CASE("rw_lock - defer_lock does not acquire on construction", "[scope_locker]")
{
    std::shared_mutex sm;
    rw_lock<std::shared_mutex> rl{sm, std::defer_lock};
    REQUIRE_FALSE(rl.owns_lock());
}

TEST_CASE("rw_lock - lock and unlock exclusive", "[scope_locker]")
{
    std::shared_mutex sm;
    rw_lock<std::shared_mutex> rl{sm, std::defer_lock};
    rl.lock();
    REQUIRE(rl.owns_lock());
    rl.unlock();
    REQUIRE_FALSE(rl.owns_lock());
}

TEST_CASE("rw_lock - try_lock succeeds when mutex is free", "[scope_locker]")
{
    std::shared_mutex sm;
    rw_lock<std::shared_mutex> rl{sm, std::defer_lock};
    REQUIRE(rl.try_lock());
    REQUIRE(rl.owns_lock());
}

TEST_CASE("rw_lock - lock_shared acquires in shared mode", "[scope_locker]")
{
    std::shared_mutex sm;
    rw_lock<std::shared_mutex> rl{sm, std::defer_lock};
    rl.lock_shared();
    REQUIRE(rl.owns_lock());
    rl.unlock(); // calls unlock_shared internally
    REQUIRE_FALSE(rl.owns_lock());
}

TEST_CASE("rw_lock - try_lock_shared succeeds when free", "[scope_locker]")
{
    std::shared_mutex sm;
    rw_lock<std::shared_mutex> rl{sm, std::defer_lock};
    REQUIRE(rl.try_lock_shared());
    REQUIRE(rl.owns_lock());
}

TEST_CASE("rw_lock - adopt_lock takes ownership of already-locked mutex", "[scope_locker]")
{
    std::shared_mutex sm;
    sm.lock();
    rw_lock<std::shared_mutex> rl{sm, std::adopt_lock};
    REQUIRE(rl.owns_lock());
    // destructor will call sm.unlock()
}

// ============================================================
// scope_locker<std::unique_lock<std::mutex>> (generic wrapper)
// ============================================================

TEST_CASE("scope_locker - acquires unique_lock when not held", "[scope_locker]")
{
    std::mutex m;
    std::unique_lock<std::mutex> lk{m, std::defer_lock};
    REQUIRE_FALSE(lk.owns_lock());
    {
        scope_locker<std::unique_lock<std::mutex>> sl{lk};
        REQUIRE(lk.owns_lock());
    }
    REQUIRE_FALSE(lk.owns_lock());
}

TEST_CASE("scope_locker - defer_lock variant temporarily unlocks", "[scope_locker]")
{
    std::mutex m;
    std::unique_lock<std::mutex> lk{m};
    REQUIRE(lk.owns_lock());
    {
        scope_locker<std::unique_lock<std::mutex>> sl{lk, std::defer_lock};
        REQUIRE_FALSE(lk.owns_lock());
    }
    REQUIRE(lk.owns_lock());
}
