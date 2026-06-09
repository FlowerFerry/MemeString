#include <catch2/catch.hpp>
#include <megopp/util/scope_cleanup.h>

TEST_CASE("scope_cleanup - runs at scope exit", "[scope_cleanup]")
{
    bool cleanup_ran = false;
    {
        auto cleanup = mgpp::util::scope_cleanup__create([&] { cleanup_ran = true; });
        REQUIRE_FALSE(cleanup_ran);
    }
    REQUIRE(cleanup_ran);
}

TEST_CASE("scope_cleanup - cancelled does not run", "[scope_cleanup]")
{
    bool cleanup_ran = false;
    {
        auto cleanup = mgpp::util::scope_cleanup__create([&] { cleanup_ran = true; });
        cleanup.cancel();
        REQUIRE(cleanup.is_cancel());
    }
    REQUIRE_FALSE(cleanup_ran);
}

TEST_CASE("scope_cleanup - early_exec runs immediately", "[scope_cleanup]")
{
    bool cleanup_ran = false;
    {
        auto cleanup = mgpp::util::scope_cleanup__create([&] { cleanup_ran = true; });
        cleanup.early_exec();
        REQUIRE(cleanup_ran);
        cleanup_ran = false; // reset
    }
    REQUIRE_FALSE(cleanup_ran); // should not run again
}

TEST_CASE("scope_cleanup - multiple cleanups run in reverse order", "[scope_cleanup]")
{
    std::vector<int> order;
    {
        auto c1 = mgpp::util::scope_cleanup__create([&] { order.push_back(1); });
        auto c2 = mgpp::util::scope_cleanup__create([&] { order.push_back(2); });
        auto c3 = mgpp::util::scope_cleanup__create([&] { order.push_back(3); });
    }
    // Destroyed in reverse order of construction
    REQUIRE(order.size() == 3);
    REQUIRE(order[0] == 3);
    REQUIRE(order[1] == 2);
    REQUIRE(order[2] == 1);
}

TEST_CASE("scope_cleanup - early_exec and cancel interaction", "[scope_cleanup]")
{
    bool ran = false;
    {
        auto cleanup = mgpp::util::scope_cleanup__create([&] { ran = true; });
        cleanup.early_exec();
        REQUIRE(ran);
        REQUIRE(cleanup.is_cancel());
    }
    REQUIRE(ran);
}

TEST_CASE("scope_cleanup - MEGOPP_UTIL__ON_SCOPE_CLEANUP macro", "[scope_cleanup]")
{
    bool cleanup_ran = false;
    {
        MEGOPP_UTIL__ON_SCOPE_CLEANUP([&] { cleanup_ran = true; });
        REQUIRE_FALSE(cleanup_ran);
    }
    REQUIRE(cleanup_ran);
}

TEST_CASE("scope_cleanup - move construction", "[scope_cleanup]")
{
    bool cleanup_ran = false;
    {
        auto cleanup1 = mgpp::util::scope_cleanup__create([&] { cleanup_ran = true; });
        auto cleanup2 = std::move(cleanup1);
        REQUIRE_FALSE(cleanup_ran);
    }
    REQUIRE(cleanup_ran);
}
