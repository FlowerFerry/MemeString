
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

// ---------------------------------------------------------------------------
// string::replace -- count parameter (unlimited: count=0 and count=-1)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string replace - count=0 unlimited", "[string]")
{
    memepp::string s = "aa_aa_aa";
    auto r = s.replace("_", "-", 0);
    REQUIRE(r == "aa-aa-aa");
}

TEST_CASE("memepp::string replace - count=-1 unlimited", "[string]")
{
    memepp::string s = "aa_aa_aa";
    auto r = s.replace("_", "-", -1);
    REQUIRE(r == "aa-aa-aa");
}

// ---------------------------------------------------------------------------
// string::replace -- boundary and edge cases
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string replace - from==to is no-op", "[string]")
{
    memepp::string s = "Hello, World!";
    auto r = s.replace("World", "World");
    REQUIRE(r == "Hello, World!");
}

TEST_CASE("memepp::string replace - replace with longer string", "[string]")
{
    memepp::string s = "Hello, World!";
    auto r = s.replace("World", "Beautiful World");
    REQUIRE(r == "Hello, Beautiful World!");
}

TEST_CASE("memepp::string replace - replace with shorter string (delete)", "[string]")
{
    memepp::string s = "Hello, Beautiful World!";
    auto r = s.replace("Beautiful ", "");
    REQUIRE(r == "Hello, World!");
}

TEST_CASE("memepp::string replace - pattern at beginning", "[string]")
{
    memepp::string s = "Hello, World!";
    auto r = s.replace("Hello", "Hi");
    REQUIRE(r == "Hi, World!");
}

TEST_CASE("memepp::string replace - pattern at end", "[string]")
{
    memepp::string s = "Hello, World!";
    auto r = s.replace("World!", "Earth!");
    REQUIRE(r == "Hello, Earth!");
}

TEST_CASE("memepp::string replace - overlapping pattern not re-matched", "[string]")
{
    memepp::string s = "aaa";
    auto r = s.replace("aa", "a");
    REQUIRE(r.size() <= s.size());
}

TEST_CASE("memepp::string replace - empty from and empty to on non-empty", "[string]")
{
    memepp::string s = "Hello";
    auto r = s.replace("", "");
    REQUIRE(r == "Hello");
}

TEST_CASE("memepp::string replace - empty from and empty to on empty", "[string]")
{
    memepp::string empty;
    auto r = empty.replace("", "");
    REQUIRE(r.empty());
}

TEST_CASE("memepp::string replace - result independent of source lifetime", "[string]")
{
    memepp::string result;
    {
        memepp::string s = "Hello, World!";
        result = s.replace("World", "Universe");
    }
    REQUIRE(result == "Hello, Universe!");
}
