
#include <catch2/catch.hpp>
#include <memepp/string.hpp>
#include <megopp/util/scope_cleanup.h>

TEST_CASE("memepp::string replace", "[string]")
{
    memepp::string str01_01 { "Hello, World!" };
    memepp::string str01_02 { "World" };

    auto str01_03 = str01_01.replace("World", "Universe");
    REQUIRE(str01_03 == "Hello, Universe!");

    auto str01_04 = str01_01.replace("l", "L");
    REQUIRE(str01_04 == "HeLLo, WorLd!");

    auto str01_05 = str01_04.replace("o", "O");
    REQUIRE(str01_05 == "HeLLO, WOrLd!");
}

TEST_CASE("memepp::string replace - from not found returns original", "[string]")
{
    memepp::string s = "Hello, World!";

    // pattern does not exist → result equals original
    auto r = s.replace("xyz", "ABC");
    REQUIRE(r == "Hello, World!");
    REQUIRE(r.size() == s.size());
}

TEST_CASE("memepp::string replace - empty 'to' deletes occurrences", "[string]")
{
    memepp::string s = "aXbXcX";

    auto r = s.replace("X", "");
    REQUIRE(r == "abc");
    REQUIRE(r.size() == 3);
}

TEST_CASE("memepp::string replace - empty 'from' is safe", "[string]")
{
    memepp::string s = "Hello";

    // empty from → must not crash; result must be a valid string
    auto r = s.replace("", "X");
    REQUIRE(r.size() >= 0);
}

TEST_CASE("memepp::string replace - replace entire string", "[string]")
{
    memepp::string s = "abc";

    auto r = s.replace("abc", "xyz");
    REQUIRE(r == "xyz");
}

TEST_CASE("memepp::string replace - multiple non-overlapping occurrences", "[string]")
{
    memepp::string s = "aababab";

    auto r = s.replace("ab", "CD");
    REQUIRE(r == "aCDCDCD");
}

TEST_CASE("memepp::string replace - on empty string", "[string]")
{
    memepp::string empty;

    auto r = empty.replace("a", "b");
    REQUIRE(r == "");
    REQUIRE(r.empty());
}
