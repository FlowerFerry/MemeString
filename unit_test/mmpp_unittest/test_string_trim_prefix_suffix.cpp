
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

// ---------------------------------------------------------------------------
// memepp::string_view — trim_prefix
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view trim_prefix — found", "[string_view_trim_prefix]")
{
    memepp::string_view sv = "Hello World";
    REQUIRE(sv.trim_prefix("Hello ") == memepp::string_view{ "World" });
}

TEST_CASE("memepp::string_view trim_prefix — not found", "[string_view_trim_prefix]")
{
    memepp::string_view sv = "Hello World";
    REQUIRE(sv.trim_prefix("World") == sv);
}

TEST_CASE("memepp::string_view trim_prefix — empty prefix", "[string_view_trim_prefix]")
{
    memepp::string_view sv = "Hello";
    REQUIRE(sv.trim_prefix("") == sv);
}

TEST_CASE("memepp::string_view trim_prefix — empty string", "[string_view_trim_prefix]")
{
    memepp::string_view sv = "";
    REQUIRE(sv.trim_prefix("x") == memepp::string_view{ "" });
}

TEST_CASE("memepp::string_view trim_prefix — prefix equals whole string", "[string_view_trim_prefix]")
{
    memepp::string_view sv = "Hello";
    REQUIRE(sv.trim_prefix("Hello") == memepp::string_view{ "" });
}

// ---------------------------------------------------------------------------
// memepp::string_view — trim_suffix
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view trim_suffix — found", "[string_view_trim_suffix]")
{
    memepp::string_view sv = "Hello World";
    REQUIRE(sv.trim_suffix(" World") == memepp::string_view{ "Hello" });
}

TEST_CASE("memepp::string_view trim_suffix — not found", "[string_view_trim_suffix]")
{
    memepp::string_view sv = "Hello World";
    REQUIRE(sv.trim_suffix("Hello") == sv);
}

TEST_CASE("memepp::string_view trim_suffix — empty suffix", "[string_view_trim_suffix]")
{
    memepp::string_view sv = "Hello";
    REQUIRE(sv.trim_suffix("") == sv);
}

TEST_CASE("memepp::string_view trim_suffix — empty string", "[string_view_trim_suffix]")
{
    memepp::string_view sv = "";
    REQUIRE(sv.trim_suffix("x") == memepp::string_view{ "" });
}

TEST_CASE("memepp::string_view trim_suffix — suffix equals whole string", "[string_view_trim_suffix]")
{
    memepp::string_view sv = "Hello";
    REQUIRE(sv.trim_suffix("Hello") == memepp::string_view{ "" });
}

// ---------------------------------------------------------------------------
// memepp::string — trim_prefix
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string trim_prefix — found", "[string_trim_prefix]")
{
    memepp::string s = "Hello World";
    REQUIRE(s.trim_prefix("Hello ") == memepp::string_view{ "World" });
}

TEST_CASE("memepp::string trim_prefix — not found", "[string_trim_prefix]")
{
    memepp::string s = "Hello World";
    REQUIRE(s.trim_prefix("World") == memepp::string_view{ "Hello World" });
}

TEST_CASE("memepp::string trim_prefix — empty prefix", "[string_trim_prefix]")
{
    memepp::string s = "Hello";
    REQUIRE(s.trim_prefix("") == memepp::string_view{ "Hello" });
}

TEST_CASE("memepp::string trim_prefix — prefix equals whole string", "[string_trim_prefix]")
{
    memepp::string s = "Hello";
    REQUIRE(s.trim_prefix("Hello") == memepp::string_view{ "" });
}

// ---------------------------------------------------------------------------
// memepp::string — trim_suffix
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string trim_suffix — found", "[string_trim_suffix]")
{
    memepp::string s = "Hello World";
    REQUIRE(s.trim_suffix(" World") == memepp::string_view{ "Hello" });
}

TEST_CASE("memepp::string trim_suffix — not found", "[string_trim_suffix]")
{
    memepp::string s = "Hello World";
    REQUIRE(s.trim_suffix("Hello") == memepp::string_view{ "Hello World" });
}

TEST_CASE("memepp::string trim_suffix — empty suffix", "[string_trim_suffix]")
{
    memepp::string s = "Hello";
    REQUIRE(s.trim_suffix("") == memepp::string_view{ "Hello" });
}

TEST_CASE("memepp::string trim_suffix — suffix equals whole string", "[string_trim_suffix]")
{
    memepp::string s = "Hello";
    REQUIRE(s.trim_suffix("Hello") == memepp::string_view{ "" });
}
