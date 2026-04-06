
#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/join/self.hpp>
#include <megopp/util/scope_cleanup.h>

#include <vector>
#include <list>
#include <deque>

TEST_CASE("memepp::string join member method", "[string]")
{
    // basic join with separator
    std::vector<memepp::string> v01 = { "a", "b", "c" };
    auto r01 = memepp::string(",").join(v01);
    REQUIRE(r01 == "a,b,c");

    // multi-char separator
    std::vector<memepp::string> v02 = { "foo", "bar", "baz" };
    auto r02 = memepp::string(", ").join(v02);
    REQUIRE(r02 == "foo, bar, baz");

    // empty separator → concatenation
    std::vector<memepp::string> v03 = { "Hello", " ", "World" };
    auto r03 = memepp::string("").join(v03);
    REQUIRE(r03 == "Hello World");

    // single element
    std::vector<memepp::string> v04 = { "only" };
    auto r04 = memepp::string(",").join(v04);
    REQUIRE(r04 == "only");

    // empty container
    std::vector<memepp::string> v05;
    auto r05 = memepp::string(",").join(v05);
    REQUIRE(r05 == "");

    // string_view items
    std::vector<memepp::string_view> v06 = { "x", "y", "z" };
    auto r06 = memepp::string("-").join(v06);
    REQUIRE(r06 == "x-y-z");

    // std::list
    std::list<memepp::string> l07 = { "1", "2", "3" };
    auto r07 = memepp::string(":").join(l07);
    REQUIRE(r07 == "1:2:3");

    // std::deque
    std::deque<memepp::string> d08 = { "a", "bb", "ccc" };
    auto r08 = memepp::string("|").join(d08);
    REQUIRE(r08 == "a|bb|ccc");
}

TEST_CASE("memepp::join free function", "[string]")
{
    // basic
    std::vector<memepp::string> v01 = { "one", "two", "three" };
    auto r01 = memepp::join(v01, ",");
    REQUIRE(r01 == "one,two,three");

    // multi-char separator
    std::vector<memepp::string> v02 = { "a", "b" };
    auto r02 = memepp::join(v02, " -> ");
    REQUIRE(r02 == "a -> b");

    // empty container
    std::vector<memepp::string> v03;
    auto r03 = memepp::join(v03, ",");
    REQUIRE(r03 == "");

    // single element
    std::vector<memepp::string> v04 = { "hello" };
    auto r04 = memepp::join(v04, ",");
    REQUIRE(r04 == "hello");

    // string_view items
    std::vector<memepp::string_view> v05 = { "p", "q", "r" };
    auto r05 = memepp::join(v05, ".");
    REQUIRE(r05 == "p.q.r");

    // std::list with string items
    std::list<memepp::string> l06 = { "alpha", "beta" };
    auto r06 = memepp::join(l06, "/");
    REQUIRE(r06 == "alpha/beta");

    // std::deque with string_view items
    std::deque<memepp::string_view> d07 = { "x1", "x2", "x3" };
    auto r07 = memepp::join(d07, ",");
    REQUIRE(r07 == "x1,x2,x3");

    // empty separator → concatenation
    std::vector<memepp::string> v08 = { "AB", "CD", "EF" };
    auto r08 = memepp::join(v08, "");
    REQUIRE(r08 == "ABCDEF");
}
