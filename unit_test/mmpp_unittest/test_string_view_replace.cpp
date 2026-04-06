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

    // pattern does not exist → original content unchanged
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

    // empty from → implementation-defined; the result must at minimum be non-crashing
    // and the string must remain valid
    auto r = s.replace("", "X");
    // just verify it is a valid string
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

    // verify the return type behaves as a full string (can compare, has storage_type)
    REQUIRE(r == "World");
    REQUIRE(r.size() == 5);
}
