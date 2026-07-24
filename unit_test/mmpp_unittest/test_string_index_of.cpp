#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>

// ---------------------------------------------------------------------------
// string::index_of
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string index_of(string) - found", "[string]")
{
    memepp::string s = "Hello, World! Hello.";

    // default: case-sensitive, first occurrence
    REQUIRE(s.index_of(memepp::string("Hello")) == 0);
    REQUIRE(s.index_of(memepp::string("World")) == 7);
}

TEST_CASE("memepp::string index_of(string) - not found returns npos", "[string]")
{
    memepp::string s = "Hello, World!";

    REQUIRE(s.index_of(memepp::string("xyz"))   == memepp::string::npos);
    REQUIRE(s.index_of(memepp::string("hello")) == memepp::string::npos); // case sensitive
}

TEST_CASE("memepp::string index_of(string_view) - basic", "[string]")
{
    memepp::string s = "abcABCabcABC";

    REQUIRE(s.index_of(memepp::string_view("abc")) == 0);
    REQUIRE(s.index_of(memepp::string_view("ABC")) == 3);
}

TEST_CASE("memepp::string index_of(string_view) - full_match false vs true", "[string]")
{
    // full_match=false: substring search anywhere in string
    // full_match=true:  whole-word match (pattern bounded by punctuation/space/boundary)
    memepp::string sub  = "hello world";
    memepp::string word = "say hello world!";

    // full_match=false: finds "hello" as substring
    REQUIRE(sub.index_of(memepp::string_view("hello"), false) == 0);

    // full_match=true: "hello" is at start boundary and followed by space -> match
    REQUIRE(word.index_of(memepp::string_view("hello"), true) == 4);

    // full_match=true: embedded "ell" has no word boundaries -> not found
    REQUIRE(sub.index_of(memepp::string_view("ell"), true) == memepp::string::npos);

    // full_match=false: embedded "ell" found as substring
    REQUIRE(sub.index_of(memepp::string_view("ell"), false) == 1);
}

TEST_CASE("memepp::string index_of(string_view) - with offset and limit", "[string]")
{
    memepp::string s = "abcXabcXabc"; // size=11

    // offset=4, limit=7 (bytes from offset) covers "abcXabc" -> finds "abc" at byte 4
    REQUIRE(s.index_of(memepp::string_view("abc"), 4, 7, false) == 4);

    // offset=4, limit=3 covers only "abc" at byte 4 -> found
    REQUIRE(s.index_of(memepp::string_view("abc"), 4, 3, false) == 4);

    // offset=4, limit=2 not enough bytes -> not found
    REQUIRE(s.index_of(memepp::string_view("abc"), 4, 2, false) == memepp::string::npos);

    // offset=8, limit=3 -> finds "abc" at byte 8
    REQUIRE(s.index_of(memepp::string_view("abc"), 8, 3, false) == 8);
}

TEST_CASE("memepp::string index_of(char*) - overloads", "[string]")
{
    memepp::string s = "abcXabcXabc";

    REQUIRE(s.index_of("abc") == 0);
    // full_match=false: substring
    REQUIRE(s.index_of("abc", false) == 0);
    // u8len=3, offset=4, full_match=false -> finds at byte 4
    REQUIRE(s.index_of("abc", 3, 4, false) == 4);

    REQUIRE(s.index_of("xyz") == memepp::string::npos);
    REQUIRE(s.index_of("xyz", false) == memepp::string::npos);
}

TEST_CASE("memepp::string index_of - empty string / empty pattern", "[string]")
{
    memepp::string empty;

    REQUIRE(empty.index_of(memepp::string_view("a")) == memepp::string::npos);
    REQUIRE(empty.index_of("a")                      == memepp::string::npos);
}

// ---------------------------------------------------------------------------
// string::last_index_of
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string last_index_of(string) - found", "[string]")
{
    memepp::string s = "Hello, World! Hello.";

    REQUIRE(s.last_index_of(memepp::string("Hello")) == 14);
    REQUIRE(s.last_index_of(memepp::string("World")) == 7);
}

TEST_CASE("memepp::string last_index_of(string) - not found returns npos", "[string]")
{
    memepp::string s = "Hello, World!";

    REQUIRE(s.last_index_of(memepp::string("xyz"))   == memepp::string::npos);
    REQUIRE(s.last_index_of(memepp::string("hello")) == memepp::string::npos);
}

TEST_CASE("memepp::string last_index_of(string_view) - full_match false vs true", "[string]")
{
    memepp::string sub  = "hello world";
    memepp::string word = "say hello world!";

    // full_match=false: finds last "hello" as substring
    REQUIRE(sub.last_index_of(memepp::string_view("hello"), false) == 0);

    // full_match=true: "hello" bounded by space -> found
    REQUIRE(word.last_index_of(memepp::string_view("hello"), true) == 4);

    // full_match=true: embedded "ell" has no word boundaries -> not found
    REQUIRE(sub.last_index_of(memepp::string_view("ell"), true) == memepp::string::npos);

    // full_match=false: embedded "ell" found
    REQUIRE(sub.last_index_of(memepp::string_view("ell"), false) == 1);
}

TEST_CASE("memepp::string last_index_of(string_view) - with offset/limit", "[string]")
{
    memepp::string s = "abcXabcXabc"; // size=11

    // last occurrence overall
    REQUIRE(s.last_index_of(memepp::string_view("abc")) == 8);

    // offset=0, limit=7 (bytes from offset) covers "abcXabc" -> last "abc" is at byte 4
    REQUIRE(s.last_index_of(memepp::string_view("abc"), 0, 7, false) == 4);

    // limit too small to contain pattern -> not found
    REQUIRE(s.last_index_of(memepp::string_view("abc"), 0, 2, false) == memepp::string::npos);
}

TEST_CASE("memepp::string last_index_of(char*) - overloads", "[string]")
{
    memepp::string s = "abcXabcXabc";

    REQUIRE(s.last_index_of("abc")        == 8);
    // full_match=false: substring
    REQUIRE(s.last_index_of("abc", false) == 8);
    // u8len=3, offset=0, limit=7 (bytes), full_match=false -> last "abc" in first 7 bytes = byte 4
    REQUIRE(s.last_index_of("abc", 3, 0, 7, false) == 4);

    REQUIRE(s.last_index_of("xyz")        == memepp::string::npos);
    REQUIRE(s.last_index_of("xyz", false) == memepp::string::npos);
}

TEST_CASE("memepp::string last_index_of - empty string", "[string]")
{
    memepp::string empty;

    REQUIRE(empty.last_index_of(memepp::string_view("a")) == memepp::string::npos);
    REQUIRE(empty.last_index_of("a")                      == memepp::string::npos);
}

// ---------------------------------------------------------------------------
// string::index_of - case insensitive
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string index_of - case insensitive basic", "[string]")
{
    memepp::string s = "Hello, World!";

    REQUIRE(s.index_of(memepp::string_view("hello"),
        memepp::case_sensitivity_t::all_insensitive) == 0);
    REQUIRE(s.index_of(memepp::string_view("world"),
        memepp::case_sensitivity_t::all_insensitive) == 7);
    REQUIRE(s.index_of(memepp::string_view("HELLO"),
        memepp::case_sensitivity_t::all_insensitive) == 0);
}

TEST_CASE("memepp::string index_of - case insensitive not found", "[string]")
{
    memepp::string s = "Hello, World!";

    REQUIRE(s.index_of(memepp::string_view("xyz"),
        memepp::case_sensitivity_t::all_insensitive) == memepp::string::npos);
}

TEST_CASE("memepp::string index_of - case sensitive flag works", "[string]")
{
    memepp::string s = "Hello, World!";

    // case_sensitive flag: 'hello' != 'Hello'
    REQUIRE(s.index_of(memepp::string_view("hello"),
        memepp::case_sensitivity_t::case_sensitive) == memepp::string::npos);
    // all_sensitive (default) also case sensitive
    REQUIRE(s.index_of(memepp::string_view("hello"),
        memepp::case_sensitivity_t::all_sensitive) == memepp::string::npos);
}

TEST_CASE("memepp::string index_of - case insensitive with full_match", "[string]")
{
    memepp::string s = "say Hello world!";

    // full_match=true: "hello" at word boundary -> found
    REQUIRE(s.index_of(memepp::string_view("hello"), true,
        memepp::case_sensitivity_t::all_insensitive) == 4);
    // full_match=true: embedded "ell" not at word boundary -> not found
    REQUIRE(s.index_of(memepp::string_view("ell"), true,
        memepp::case_sensitivity_t::all_insensitive) == memepp::string::npos);
    // full_match=false: embedded "ell" found (in "say Hello world!", 'e' at position 5)
    REQUIRE(s.index_of(memepp::string_view("ell"), false,
        memepp::case_sensitivity_t::all_insensitive) == 5);
}

// ---------------------------------------------------------------------------
// string::last_index_of - case insensitive
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string last_index_of - case insensitive basic", "[string]")
{
    memepp::string s = "Hello, World! Hello.";

    REQUIRE(s.last_index_of(memepp::string_view("hello"),
        memepp::case_sensitivity_t::all_insensitive) == 14);
    REQUIRE(s.last_index_of(memepp::string_view("WORLD"),
        memepp::case_sensitivity_t::all_insensitive) == 7);
}

TEST_CASE("memepp::string last_index_of - case insensitive not found", "[string]")
{
    memepp::string s = "Hello, World!";

    REQUIRE(s.last_index_of(memepp::string_view("xyz"),
        memepp::case_sensitivity_t::all_insensitive) == memepp::string::npos);
}

TEST_CASE("memepp::string last_index_of - case sensitive flag works", "[string]")
{
    memepp::string s = "Hello, World!";

    REQUIRE(s.last_index_of(memepp::string_view("hello"),
        memepp::case_sensitivity_t::case_sensitive) == memepp::string::npos);
}
