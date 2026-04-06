
#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>

#include <vector>
#include <list>
#include <deque>

TEST_CASE("memepp::string_view join member method", "[string_view]")
{
    // basic join with string items
    std::vector<memepp::string> v01 = { "a", "b", "c" };
    auto r01 = memepp::string_view(",").join(v01);
    REQUIRE(r01 == "a,b,c");

    // multi-char separator
    std::vector<memepp::string> v02 = { "foo", "bar", "baz" };
    auto r02 = memepp::string_view(", ").join(v02);
    REQUIRE(r02 == "foo, bar, baz");

    // empty separator -> concatenation
    std::vector<memepp::string> v03 = { "Hello", " ", "World" };
    auto r03 = memepp::string_view("").join(v03);
    REQUIRE(r03 == "Hello World");

    // single element
    std::vector<memepp::string> v04 = { "only" };
    auto r04 = memepp::string_view(",").join(v04);
    REQUIRE(r04 == "only");

    // empty container
    std::vector<memepp::string> v05;
    auto r05 = memepp::string_view(",").join(v05);
    REQUIRE(r05 == "");

    // string_view items
    std::vector<memepp::string_view> v06 = { "x", "y", "z" };
    auto r06 = memepp::string_view("-").join(v06);
    REQUIRE(r06 == "x-y-z");

    // string_view items, empty container
    std::vector<memepp::string_view> v07;
    auto r07 = memepp::string_view("-").join(v07);
    REQUIRE(r07 == "");

    // string_view items, single element
    std::vector<memepp::string_view> v08 = { "hello" };
    auto r08 = memepp::string_view(",").join(v08);
    REQUIRE(r08 == "hello");

    // std::list with string items
    std::list<memepp::string> l09 = { "1", "2", "3" };
    auto r09 = memepp::string_view(":").join(l09);
    REQUIRE(r09 == "1:2:3");

    // std::deque with string items
    std::deque<memepp::string> d10 = { "a", "bb", "ccc" };
    auto r10 = memepp::string_view("|").join(d10);
    REQUIRE(r10 == "a|bb|ccc");

    // std::list with string_view items
    std::list<memepp::string_view> l11 = { "alpha", "beta", "gamma" };
    auto r11 = memepp::string_view("/").join(l11);
    REQUIRE(r11 == "alpha/beta/gamma");

    // std::deque with string_view items
    std::deque<memepp::string_view> d12 = { "x1", "x2", "x3" };
    auto r12 = memepp::string_view(",").join(d12);
    REQUIRE(r12 == "x1,x2,x3");
}
