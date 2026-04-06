#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

// ---------------------------------------------------------------------------
// string_view::index_of
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view index_of(string_view) - found", "[string_view]")
{
    memepp::string_view s = "Hello, World! Hello.";

    REQUIRE(s.index_of(memepp::string_view("Hello")) == 0);
    REQUIRE(s.index_of(memepp::string_view("World")) == 7);
}

TEST_CASE("memepp::string_view index_of(string_view) - not found returns npos", "[string_view]")
{
    memepp::string_view s = "Hello, World!";

    REQUIRE(s.index_of(memepp::string_view("xyz"))   == memepp::string_view::npos);
    REQUIRE(s.index_of(memepp::string_view("hello")) == memepp::string_view::npos); // case sensitive
}

TEST_CASE("memepp::string_view index_of - basic", "[string_view]")
{
    memepp::string_view s = "abcABCabcABC";

    REQUIRE(s.index_of(memepp::string_view("abc")) == 0);
    REQUIRE(s.index_of(memepp::string_view("ABC")) == 3);
}

TEST_CASE("memepp::string_view index_of - full_match false vs true", "[string_view]")
{
    // full_match=false: substring search
    // full_match=true:  whole-word match (bounded by punctuation/space/boundary)
    memepp::string_view sub  = "hello world";
    memepp::string_view word = "say hello world!";

    // full_match=false: finds "hello" as substring
    REQUIRE(sub.index_of(memepp::string_view("hello"), false) == 0);

    // full_match=true: "hello" bounded by space -> match
    REQUIRE(word.index_of(memepp::string_view("hello"), true) == 4);

    // full_match=true: embedded "ell" has no word boundaries -> not found
    REQUIRE(sub.index_of(memepp::string_view("ell"), true) == memepp::string_view::npos);

    // full_match=false: embedded "ell" found as substring
    REQUIRE(sub.index_of(memepp::string_view("ell"), false) == 1);
}

TEST_CASE("memepp::string_view index_of - with offset and limit", "[string_view]")
{
    memepp::string_view s = "abcXabcXabc"; // size=11

    // offset=4, limit=7 (bytes from offset) covers "abcXabc" -> finds "abc" at byte 4
    REQUIRE(s.index_of(memepp::string_view("abc"), 4, 7, false) == 4);

    // offset=4, limit=3 covers only "abc" at byte 4 -> found
    REQUIRE(s.index_of(memepp::string_view("abc"), 4, 3, false) == 4);

    // offset=4, limit=2 not enough bytes -> not found
    REQUIRE(s.index_of(memepp::string_view("abc"), 4, 2, false) == memepp::string_view::npos);

    // offset=8, limit=3 -> finds "abc" at byte 8
    REQUIRE(s.index_of(memepp::string_view("abc"), 8, 3, false) == 8);
}

TEST_CASE("memepp::string_view index_of(char*) - overloads", "[string_view]")
{
    memepp::string_view s = "abcXabcXabc";

    REQUIRE(s.index_of("abc") == 0);
    // full_match=false: substring
    REQUIRE(s.index_of("abc", false) == 0);
    // u8len=3, offset=4, full_match=false -> finds at byte 4
    REQUIRE(s.index_of("abc", 3, 4, false) == 4);

    REQUIRE(s.index_of("xyz")        == memepp::string_view::npos);
    REQUIRE(s.index_of("xyz", false) == memepp::string_view::npos);
}

TEST_CASE("memepp::string_view index_of - empty string / empty pattern", "[string_view]")
{
    memepp::string_view empty;

    REQUIRE(empty.index_of(memepp::string_view("a")) == memepp::string_view::npos);
    REQUIRE(empty.index_of("a")                      == memepp::string_view::npos);
}

// ---------------------------------------------------------------------------
// string_view::last_index_of
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view last_index_of(string_view) - found", "[string_view]")
{
    memepp::string_view s = "Hello, World! Hello.";

    REQUIRE(s.last_index_of(memepp::string_view("Hello")) == 14);
    REQUIRE(s.last_index_of(memepp::string_view("World")) == 7);
}

TEST_CASE("memepp::string_view last_index_of(string_view) - not found returns npos", "[string_view]")
{
    memepp::string_view s = "Hello, World!";

    REQUIRE(s.last_index_of(memepp::string_view("xyz"))   == memepp::string_view::npos);
    REQUIRE(s.last_index_of(memepp::string_view("hello")) == memepp::string_view::npos);
}

TEST_CASE("memepp::string_view last_index_of(string_view) - full_match false vs true", "[string_view]")
{
    memepp::string_view sub  = "hello world";
    memepp::string_view word = "say hello world!";

    // full_match=false: finds last "hello" as substring
    REQUIRE(sub.last_index_of(memepp::string_view("hello"), false) == 0);

    // full_match=true: "hello" bounded by space -> found
    REQUIRE(word.last_index_of(memepp::string_view("hello"), true) == 4);

    // full_match=true: embedded "ell" has no word boundaries -> not found
    REQUIRE(sub.last_index_of(memepp::string_view("ell"), true) == memepp::string_view::npos);

    // full_match=false: embedded "ell" found
    REQUIRE(sub.last_index_of(memepp::string_view("ell"), false) == 1);
}

TEST_CASE("memepp::string_view last_index_of - with offset/limit", "[string_view]")
{
    memepp::string_view s = "abcXabcXabc"; // size=11

    // last occurrence overall
    REQUIRE(s.last_index_of(memepp::string_view("abc")) == 8);

    // offset=0, limit=7 (bytes from offset) covers "abcXabc" -> last "abc" is at byte 4
    REQUIRE(s.last_index_of(memepp::string_view("abc"), 0, 7, false) == 4);

    // limit too small to contain pattern -> not found
    REQUIRE(s.last_index_of(memepp::string_view("abc"), 0, 2, false) == memepp::string_view::npos);
}

TEST_CASE("memepp::string_view last_index_of(char*) - overloads", "[string_view]")
{
    memepp::string_view s = "abcXabcXabc";

    REQUIRE(s.last_index_of("abc")        == 8);
    // full_match=false: substring
    REQUIRE(s.last_index_of("abc", false) == 8);
    // u8len=3, offset=0, limit=7 (bytes), full_match=false -> last "abc" in first 7 bytes = byte 4
    REQUIRE(s.last_index_of("abc", 3, 0, 7, false) == 4);

    REQUIRE(s.last_index_of("xyz")        == memepp::string_view::npos);
    REQUIRE(s.last_index_of("xyz", false) == memepp::string_view::npos);
}

TEST_CASE("memepp::string_view last_index_of - empty view", "[string_view]")
{
    memepp::string_view empty;

    REQUIRE(empty.last_index_of(memepp::string_view("a")) == memepp::string_view::npos);
    REQUIRE(empty.last_index_of("a")                      == memepp::string_view::npos);
}
