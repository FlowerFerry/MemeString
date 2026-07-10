#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>

// ---------------------------------------------------------------------------
// string::find
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string find - basic hits", "[string]")
{
    memepp::string s = "Hello, World! Hello.";

    // find(string, pos)
    REQUIRE(s.find(memepp::string("Hello")) == 0);
    REQUIRE(s.find(memepp::string("World")) == 7);
    REQUIRE(s.find(memepp::string("Hello"), 1) == 14);   // skip first occurrence
    REQUIRE(s.find(memepp::string("Hello"), 14) == 14);  // exact start

    // find(const char*, pos)
    REQUIRE(s.find("Hello") == 0);
    REQUIRE(s.find("World") == 7);
    REQUIRE(s.find("Hello", 1) == 14);

    // find(const char*, pos, substr_count)
    REQUIRE(s.find("Hello!", 0, 5) == 0);   // "Hello" (5 chars)
    REQUIRE(s.find("Hello!", 1, 5) == 14);  // second "Hello"

    // find(char, pos)
    REQUIRE(s.find('H') == 0);
    REQUIRE(s.find('H', 1) == 14);
    REQUIRE(s.find(',') == 5);

    // find(rune, pos)
    memepp::rune r_W = 'W';
    REQUIRE(s.find(r_W) == 7);
    memepp::rune r_H = 'H';
    REQUIRE(s.find(r_H, 1) == 14);
}

TEST_CASE("memepp::string find - not found returns npos", "[string]")
{
    memepp::string s = "Hello, World!";

    REQUIRE(s.find("xyz")           == memepp::string::npos);
    REQUIRE(s.find("Hello", 5)      == memepp::string::npos);  // past first occurrence
    REQUIRE(s.find('z')             == memepp::string::npos);
    REQUIRE(s.find(memepp::rune('z')) == memepp::string::npos);
    REQUIRE(s.find("Hello!", 0, 6)  == memepp::string::npos);  // "Hello!" not in s
}

TEST_CASE("memepp::string find - empty / edge cases", "[string]")
{
    memepp::string empty;
    memepp::string s = "abc";

    // find on empty string
    REQUIRE(empty.find("a")           == memepp::string::npos);
    REQUIRE(empty.find('a')           == memepp::string::npos);
    REQUIRE(empty.find(memepp::rune('a')) == memepp::string::npos);

    // find empty pattern in non-empty string
    REQUIRE(s.find("") == memepp::string::npos);

    // pos beyond length
    REQUIRE(s.find("a", 10) == memepp::string::npos);

    // pattern longer than string
    REQUIRE(s.find("abcdefgh") == memepp::string::npos);
}

TEST_CASE("memepp::string find - pointer overload", "[string]")
{
    memepp::string s = "ABC123ABC";
    const memepp::string::const_pointer p =
        reinterpret_cast<const memepp::string::const_pointer>("ABC");

    REQUIRE(s.find(p)    == 0);
    REQUIRE(s.find(p, 1) == 6);

    // with explicit length
    REQUIRE(s.find(p, 0, 3) == 0);
    REQUIRE(s.find(p, 1, 3) == 6);
}

// ---------------------------------------------------------------------------
// string::rfind
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string rfind - basic hits", "[string]")
{
    memepp::string s = "Hello, World! Hello.";

    REQUIRE(s.rfind(memepp::string("Hello"))       == 14);
    REQUIRE(s.rfind("Hello")                       == 14);
    REQUIRE(s.rfind("Hello", 13)                   == 0);   // search up to pos 13
    REQUIRE(s.rfind('o')                           == 18);
    REQUIRE(s.rfind(memepp::rune('l'))             == 17);
}

TEST_CASE("memepp::string rfind - not found returns npos", "[string]")
{
    memepp::string s = "Hello, World!";

    REQUIRE(s.rfind("xyz")              == memepp::string::npos);
    REQUIRE(s.rfind('z')               == memepp::string::npos);
    REQUIRE(s.rfind(memepp::rune('z')) == memepp::string::npos);
}

TEST_CASE("memepp::string rfind - empty / edge cases", "[string]")
{
    memepp::string empty;
    memepp::string s = "abc";

    REQUIRE(empty.rfind("a")            == memepp::string::npos);
    REQUIRE(empty.rfind('a')            == memepp::string::npos);
    REQUIRE(s.rfind("abcdefgh")         == memepp::string::npos);
}

TEST_CASE("memepp::string rfind - pointer overload", "[string]")
{
    memepp::string s = "ABC123ABC";
    const memepp::string::const_pointer p =
        reinterpret_cast<const memepp::string::const_pointer>("ABC");

    REQUIRE(s.rfind(p)       == 6);
    REQUIRE(s.rfind(p, 5)    == 0);  // search up to pos 5 — only first occurrence visible
    REQUIRE(s.rfind(p, 9, 3) == 6);
}

TEST_CASE("memepp::string rfind - const char* with substr_count", "[string]")
{
    memepp::string s = "Hello, Hello!";

    // find full word
    REQUIRE(s.rfind("Hello", memepp::string::npos, 5) == 7);
    REQUIRE(s.rfind("Hello", 6, 5) == 0);
    // zero count → should return npos
    REQUIRE(s.rfind("Hello", memepp::string::npos, 0) == memepp::string::npos);
    // partial prefix match
    REQUIRE(s.rfind("HelloXYZ", memepp::string::npos, 4) == 7);

    // not found with count
    REQUIRE(s.rfind("Hello", memepp::string::npos, 6) == memepp::string::npos);
}
