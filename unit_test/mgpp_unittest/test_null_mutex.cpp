#include <catch2/catch.hpp>
#include <megopp/help/null_mutex.h>
#include <mutex>
#include <shared_mutex>

TEST_CASE("null_mutex - lock/unlock are no-ops", "[null_mutex]")
{
    mgpp::help::null_mutex mtx;
    REQUIRE_NOTHROW(mtx.lock());
    REQUIRE_NOTHROW(mtx.unlock());
}

TEST_CASE("null_mutex - lock_shared/unlock_shared are no-ops", "[null_mutex]")
{
    mgpp::help::null_mutex mtx;
    REQUIRE_NOTHROW(mtx.lock_shared());
    REQUIRE_NOTHROW(mtx.unlock_shared());
}

TEST_CASE("null_mutex - compatible with std::lock_guard", "[null_mutex]")
{
    mgpp::help::null_mutex mtx;
    {
        std::lock_guard<mgpp::help::null_mutex> lock(mtx);
        SUCCEED("std::lock_guard compiles with null_mutex");
    }
}

TEST_CASE("null_mutex - compatible with std::unique_lock", "[null_mutex]")
{
    mgpp::help::null_mutex mtx;
    {
        std::unique_lock<mgpp::help::null_mutex> lock(mtx);
        REQUIRE(lock.owns_lock());
    }
}

TEST_CASE("null_mutex - compatible with std::shared_lock", "[null_mutex]")
{
    mgpp::help::null_mutex mtx;
    {
        std::shared_lock<mgpp::help::null_mutex> lock(mtx);
        REQUIRE(lock.owns_lock());
    }
}
