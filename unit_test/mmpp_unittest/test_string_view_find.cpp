#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

// ---------------------------------------------------------------------------
// string_view::find
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view find - basic hits", "[string_view]")
{
    memepp::string_view s = "Hello, World! Hello.";

    // find(string_view, pos)
    REQUIRE(s.find(memepp::string_view("Hello")) == 0);
    REQUIRE(s.find(memepp::string_view("World")) == 7);
    REQUIRE(s.find(memepp::string_view("Hello"), 1) == 14);  // skip first
    REQUIRE(s.find(memepp::string_view("Hello"), 14) == 14); // exact start

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

TEST_CASE("memepp::string_view find - not found returns npos", "[string_view]")
{
    memepp::string_view s = "Hello, World!";

    REQUIRE(s.find("xyz")                       == memepp::string_view::npos);
    REQUIRE(s.find("Hello", 5)                  == memepp::string_view::npos);
    REQUIRE(s.find('z')                         == memepp::string_view::npos);
    REQUIRE(s.find(memepp::rune('z'))           == memepp::string_view::npos);
    REQUIRE(s.find("Hello!", 0, 6)              == memepp::string_view::npos);
    REQUIRE(s.find(memepp::string_view("xyz"))  == memepp::string_view::npos);
}

TEST_CASE("memepp::string_view find - empty / edge cases", "[string_view]")
{
    memepp::string_view empty;
    memepp::string_view s = "abc";

    REQUIRE(empty.find("a")               == memepp::string_view::npos);
    REQUIRE(empty.find('a')               == memepp::string_view::npos);
    REQUIRE(empty.find(memepp::rune('a')) == memepp::string_view::npos);

    // empty pattern
    REQUIRE(s.find("") == memepp::string_view::npos);

    // pos beyond length
    REQUIRE(s.find("a", 10) == memepp::string_view::npos);

    // pattern longer than view
    REQUIRE(s.find("abcdefgh") == memepp::string_view::npos);
}

TEST_CASE("memepp::string_view find - pointer overload", "[string_view]")
{
    memepp::string_view s = "ABC123ABC";
    const memepp::string_view::const_pointer p =
        reinterpret_cast<const memepp::string_view::const_pointer>("ABC");

    REQUIRE(s.find(p)       == 0);
    REQUIRE(s.find(p, 1)    == 6);
    REQUIRE(s.find(p, 0, 3) == 0);
    REQUIRE(s.find(p, 1, 3) == 6);
}

// ---------------------------------------------------------------------------
// string_view::rfind
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view rfind - basic hits", "[string_view]")
{
    memepp::string_view s = "Hello, World! Hello.";

    REQUIRE(s.rfind(memepp::string_view("Hello"))     == 14);
    REQUIRE(s.rfind("Hello")                          == 14);
    REQUIRE(s.rfind("Hello", 13)                      == 0);  // search up to pos 13
    REQUIRE(s.rfind('o')                              == 18);
    REQUIRE(s.rfind(memepp::rune('l'))                == 17);
}

TEST_CASE("memepp::string_view rfind - not found returns npos", "[string_view]")
{
    memepp::string_view s = "Hello, World!";

    REQUIRE(s.rfind("xyz")              == memepp::string_view::npos);
    REQUIRE(s.rfind('z')               == memepp::string_view::npos);
    REQUIRE(s.rfind(memepp::rune('z')) == memepp::string_view::npos);
}

TEST_CASE("memepp::string_view rfind - empty / edge cases", "[string_view]")
{
    memepp::string_view empty;
    memepp::string_view s = "abc";

    REQUIRE(empty.rfind("a")            == memepp::string_view::npos);
    REQUIRE(empty.rfind('a')            == memepp::string_view::npos);
    REQUIRE(s.rfind("abcdefgh")         == memepp::string_view::npos);
}

TEST_CASE("memepp::string_view rfind - pointer overload", "[string_view]")
{
    memepp::string_view s = "ABC123ABC";
    const memepp::string_view::const_pointer p =
        reinterpret_cast<const memepp::string_view::const_pointer>("ABC");

    REQUIRE(s.rfind(p)       == 6);
    REQUIRE(s.rfind(p, 5)    == 0);
    REQUIRE(s.rfind(p, 9, 3) == 6);
}

TEST_CASE("memepp::string_view rfind - const char* with substr_count", "[string_view]")
{
    memepp::string_view s = "Hello, Hello!";

    // find full word
    REQUIRE(s.rfind("Hello", memepp::string_view::npos, 5) == 7);
    REQUIRE(s.rfind("Hello", 6, 5) == 0);
    // partial prefix match
    REQUIRE(s.rfind("HelloXYZ", memepp::string_view::npos, 4) == 7);
    // zero count → should return npos
    REQUIRE(s.rfind("Hello", memepp::string_view::npos, 0) == memepp::string_view::npos);
    // not found
    REQUIRE(s.rfind("Hello", memepp::string_view::npos, 6) == memepp::string_view::npos);
}
