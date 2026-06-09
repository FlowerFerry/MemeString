#include <catch2/catch.hpp>
#include <megopp/help/singleton.h>

struct test_singleton_data {
    int value = 42;
    std::string name = "hello";
};

TEST_CASE("singleton - basic instance access", "[singleton]")
{
    auto& inst = mgpp::help::singleton<int>::instance();
    REQUIRE(inst == 0);  // value-initialized
}

TEST_CASE("singleton - same instance is returned", "[singleton]")
{
    auto& a = mgpp::help::singleton<int>::instance();
    auto& b = mgpp::help::singleton<int>::instance();
    REQUIRE(&a == &b);
}

TEST_CASE("singleton - struct type", "[singleton]")
{
    auto& inst = mgpp::help::singleton<test_singleton_data>::instance();
    REQUIRE(inst.value == 42);
    REQUIRE(inst.name == "hello");
}

TEST_CASE("singleton - state persists across accesses", "[singleton]")
{
    auto& inst = mgpp::help::singleton<int>::instance();
    inst = 100;
    auto& same = mgpp::help::singleton<int>::instance();
    REQUIRE(same == 100);
}
