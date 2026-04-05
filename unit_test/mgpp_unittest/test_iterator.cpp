#include <catch2/catch.hpp>

#include <megopp/help/iterator.h>

#include <map>
#include <string>
#include <vector>
#include <iterator>
#include <type_traits>

TEST_CASE("mgpp::help::iter_traits - non-pair iterator mapped_type equals value_type", "[iterator]")
{
    using It = std::vector<int>::iterator;
    REQUIRE((std::is_same<mgpp::help::iter_traits<It>::mapped_type, int>::value));
}

TEST_CASE("mgpp::help::iter_traits - map iterator mapped_type equals mapped_type", "[iterator]")
{
    using It = std::map<int, std::string>::iterator;
    REQUIRE((std::is_same<mgpp::help::iter_traits<It>::mapped_type, std::string>::value));
}

TEST_CASE("mgpp::help::iter_value - pair returns mutable second", "[iterator]")
{
    std::pair<int, std::string> p{ 1, "hello" };
    REQUIRE(mgpp::help::iter_value(p) == "hello");

    mgpp::help::iter_value(p) = "world";
    REQUIRE(p.second == "world");
}

TEST_CASE("mgpp::help::iter_value - const pair returns second", "[iterator]")
{
    const std::pair<int, std::string> p{ 2, "foo" };
    REQUIRE(mgpp::help::iter_value(p) == "foo");
}

TEST_CASE("mgpp::help::iter_value - non-pair returns mutable self", "[iterator]")
{
    int v = 42;
    REQUIRE(mgpp::help::iter_value(v) == 42);

    mgpp::help::iter_value(v) = 99;
    REQUIRE(v == 99);
}

TEST_CASE("mgpp::help::iter_key - pair returns mutable first", "[iterator]")
{
    std::pair<int, std::string> p{ 10, "bar" };
    REQUIRE(mgpp::help::iter_key(p) == 10);
}

TEST_CASE("mgpp::help::iter_key - const pair returns first", "[iterator]")
{
    const std::pair<int, std::string> p{ 20, "baz" };
    REQUIRE(mgpp::help::iter_key(p) == 20);
}

TEST_CASE("mgpp::help::iter_key - non-pair returns mutable self", "[iterator]")
{
    int v = 7;
    REQUIRE(mgpp::help::iter_key(v) == 7);

    mgpp::help::iter_key(v) = 55;
    REQUIRE(v == 55);
}
