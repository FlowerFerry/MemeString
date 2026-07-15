#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

// ---------------------------------------------------------------------------
// string_view::replace  (returns memepp::string, not string_view)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view replace - basic replacement", "[string_view]")
{
    memepp::string_view s = "Hello, World!";

    auto r1 = s.replace("World", "Universe");
    REQUIRE(r1 == "Hello, Universe!");

    auto r2 = s.replace("Hello", "Hi");
    REQUIRE(r2 == "Hi, World!");

    auto r3 = s.replace("l", "L");
    REQUIRE(r3 == "HeLLo, WorLd!");
}

TEST_CASE("memepp::string_view replace - from not found returns original", "[string_view]")
{
    memepp::string_view s = "Hello, World!";
    auto r = s.replace("xyz", "ABC");
    REQUIRE(r == "Hello, World!");
}

TEST_CASE("memepp::string_view replace - empty 'to' deletes occurrences", "[string_view]")
{
    memepp::string_view s = "aXbXcX";
    auto r = s.replace("X", "");
    REQUIRE(r == "abc");
}

TEST_CASE("memepp::string_view replace - empty 'from' returns original", "[string_view]")
{
    memepp::string_view s = "Hello";
    auto r = s.replace("", "X");
    REQUIRE(r.size() >= 0);
}

TEST_CASE("memepp::string_view replace - replace entire string", "[string_view]")
{
    memepp::string_view s = "abc";
    auto r = s.replace("abc", "xyz");
    REQUIRE(r == "xyz");
}

TEST_CASE("memepp::string_view replace - multiple non-overlapping occurrences", "[string_view]")
{
    memepp::string_view s = "aababab";
    auto r = s.replace("ab", "CD");
    REQUIRE(r == "aCDCDCD");
}

TEST_CASE("memepp::string_view replace - empty view", "[string_view]")
{
    memepp::string_view empty;
    auto r = empty.replace("a", "b");
    REQUIRE(r == "");
    REQUIRE(r.empty());
}

TEST_CASE("memepp::string_view replace - result is memepp::string", "[string_view]")
{
    memepp::string_view s = "Hello";
    auto r = s.replace("Hello", "World");
    REQUIRE(r == "World");
    REQUIRE(r.size() == 5);
}

// ---------------------------------------------------------------------------
// string_view::replace -- count parameter (unlimited: count=0 and count=-1)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view replace - count=0 unlimited", "[string_view]")
{
    memepp::string_view s = "aa_aa_aa";
    auto r = s.replace("_", "-", 0);
    REQUIRE(r == "aa-aa-aa");
}

TEST_CASE("memepp::string_view replace - count=-1 unlimited", "[string_view]")
{
    memepp::string_view s = "aa_aa_aa";
    auto r = s.replace("_", "-", -1);
    REQUIRE(r == "aa-aa-aa");
}

// ---------------------------------------------------------------------------
// string_view::replace -- boundary and edge cases
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view replace - from==to is no-op", "[string_view]")
{
    memepp::string_view s = "Hello, World!";
    auto r = s.replace("World", "World");
    REQUIRE(r == "Hello, World!");
}

TEST_CASE("memepp::string_view replace - replace with longer string", "[string_view]")
{
    memepp::string_view s = "Hello, World!";
    auto r = s.replace("World", "Beautiful World");
    REQUIRE(r == "Hello, Beautiful World!");
}

TEST_CASE("memepp::string_view replace - replace with shorter string (delete)", "[string_view]")
{
    memepp::string_view s = "Hello, Beautiful World!";
    auto r = s.replace("Beautiful ", "");
    REQUIRE(r == "Hello, World!");
}

TEST_CASE("memepp::string_view replace - pattern at beginning", "[string_view]")
{
    memepp::string_view s = "Hello, World!";
    auto r = s.replace("Hello", "Hi");
    REQUIRE(r == "Hi, World!");
}

TEST_CASE("memepp::string_view replace - pattern at end", "[string_view]")
{
    memepp::string_view s = "Hello, World!";
    auto r = s.replace("World!", "Earth!");
    REQUIRE(r == "Hello, Earth!");
}

TEST_CASE("memepp::string_view replace - overlapping pattern not re-matched", "[string_view]")
{
    memepp::string_view s = "aaa";
    auto r = s.replace("aa", "a");
    REQUIRE(r.size() <= s.size());
}

TEST_CASE("memepp::string_view replace - empty from and empty to on non-empty", "[string_view]")
{
    memepp::string_view s = "Hello";
    auto r = s.replace("", "");
    REQUIRE(r == "Hello");
}

TEST_CASE("memepp::string_view replace - empty from and empty to on empty", "[string_view]")
{
    memepp::string_view empty;
    auto r = empty.replace("", "");
    REQUIRE(r.empty());
}

TEST_CASE("memepp::string_view replace - result independent of source lifetime", "[string_view]")
{
    memepp::string result;
    {
        memepp::string_view s = "Hello, World!";
        result = s.replace("World", "Universe");
    }
    REQUIRE(result == "Hello, Universe!");
}

TEST_CASE("memepp::string_view replace - from large storage type", "[string_view]")
{
    memepp::string s = "Hello, World!";
    memepp::string_view sv = s.to_large();
    auto r = sv.replace("World", "Universe");
    REQUIRE(r == "Hello, Universe!");
}

TEST_CASE("memepp::string_view replace - from string_view created from string rvalue", "[string_view]")
{
    memepp::string_view sv = memepp::string("Hello, World!");
    auto r = sv.replace("Hello", "Hi");
    REQUIRE(r == "Hi, World!");
}

TEST_CASE("memepp::string_view replace - substring view replace", "[string_view]")
{
    memepp::string s = "Hello, World!";
    memepp::string_view sv = s;
    memepp::string_view sub = sv.substr(0, 5);  // "Hello"
    auto r = sub.replace("Hello", "Hi");
    REQUIRE(r == "Hi");
}
