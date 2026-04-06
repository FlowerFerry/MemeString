#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

// ---------------------------------------------------------------------------
// string_view::substr  (returns string_view — non-owning slice)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view substr - normal ranges", "[string_view]")
{
    memepp::string src = "Hello, World!"; // keep alive
    memepp::string_view s = src;

    REQUIRE(s.substr(0, 5) == "Hello");
    REQUIRE(s.substr(7, 5) == "World");
    REQUIRE(s.substr(0, 1) == "H");
    REQUIRE(s.substr(12, 1) == "!");

    // rest of string
    REQUIRE(s.substr(7) == "World!");
    REQUIRE(s.substr(0) == "Hello, World!");
}

TEST_CASE("memepp::string_view substr - result is still a view (not a copy)", "[string_view]")
{
    memepp::string src = "Hello, World!";
    memepp::string_view s = src;

    auto sub = s.substr(0, 5);
    // substr of a string_view should return a view-type storage
    REQUIRE(static_cast<int>(sub.storage_type()) ==
            static_cast<int>(MemeString_UnsafeStorageType_view));
}

TEST_CASE("memepp::string_view substr - count larger than remaining", "[string_view]")
{
    memepp::string src = "abcde";
    memepp::string_view s = src;

    REQUIRE(s.substr(3, 100) == "de");
    REQUIRE(s.substr(0, 999) == "abcde");
}

TEST_CASE("memepp::string_view substr - pos equals size returns empty", "[string_view]")
{
    memepp::string src = "abc";
    memepp::string_view s = src;

    REQUIRE(s.substr(3)    == "");
    REQUIRE(s.substr(3, 5) == "");
}

TEST_CASE("memepp::string_view substr - empty view", "[string_view]")
{
    memepp::string_view empty;

    REQUIRE(empty.substr()     == "");
    REQUIRE(empty.substr(0)    == "");
    REQUIRE(empty.substr(0, 0) == "");
}

TEST_CASE("memepp::string_view substr - count zero returns empty", "[string_view]")
{
    memepp::string src = "Hello";
    memepp::string_view s = src;

    REQUIRE(s.substr(0, 0) == "");
    REQUIRE(s.substr(2, 0) == "");
}

TEST_CASE("memepp::string_view substr - entire view via npos", "[string_view]")
{
    memepp::string src = "Hello";
    memepp::string_view s = src;

    REQUIRE(s.substr(0, memepp::string_view::npos) == "Hello");
}

TEST_CASE("memepp::string_view substr - constructed from char*", "[string_view]")
{
    // string_view over a literal
    memepp::string_view s = "ABCDEFGH";

    REQUIRE(s.substr(2, 4) == "CDEF");
    REQUIRE(s.substr(6)    == "GH");
}
