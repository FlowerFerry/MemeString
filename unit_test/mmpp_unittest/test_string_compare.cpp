
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

TEST_CASE("memepp::string compare", "[string]")
{
    // --- shared data ---
    memepp::string s_world  = "Hello World.";
    memepp::string s_world_ = "Hello World!";
    const char* sz_world  = "Hello World.";
    const char* sz_world_ = "Hello World!";
    memepp::string_view sv_world  = sz_world;
    memepp::string_view sv_world_ = sz_world_;

    memepp::string s_hello = "Hello";
    const char* sz_hello = "Hello";
    memepp::string_view sv_hello = sz_hello;

    memepp::string s_long = "Hello World...";
    const char* sz_long = "Hello World...";
    memepp::string_view sv_long = sz_long;

    SECTION("differ at last char: 'Hello World.' vs 'Hello World!'")
    {
        // string vs string
        REQUIRE(s_world >  s_world_);
        REQUIRE(s_world >= s_world_);
        REQUIRE(s_world_ <  s_world);
        REQUIRE(s_world_ <= s_world);

        // string vs const char*
        REQUIRE(s_world >  sz_world_);
        REQUIRE(s_world >= sz_world_);
        REQUIRE(sz_world_ <  s_world);
        REQUIRE(sz_world_ <= s_world);

        // string vs string_view
        REQUIRE(s_world >  sv_world_);
        REQUIRE(s_world >= sv_world_);
        REQUIRE(sv_world_ <  s_world);
        REQUIRE(sv_world_ <= s_world);

        // equal case (same content, different types)
        REQUIRE(s_world <= sz_world);
        REQUIRE(s_world >= sz_world);
        REQUIRE(sz_world <= s_world);
        REQUIRE(sz_world >= s_world);

        REQUIRE(s_world <= sv_world);
        REQUIRE(s_world >= sv_world);
        REQUIRE(sv_world <= s_world);
        REQUIRE(sv_world >= s_world);
    }

    SECTION("prefix shorter: 'Hello World.' vs 'Hello'")
    {
        // string vs string
        REQUIRE(s_hello <  s_world);
        REQUIRE(s_hello <= s_world);
        REQUIRE(s_world >  s_hello);
        REQUIRE(s_world >= s_hello);

        // string vs const char*
        REQUIRE(s_hello <  sz_world);
        REQUIRE(s_hello <= sz_world);
        REQUIRE(sz_world >  s_hello);
        REQUIRE(sz_world >= s_hello);

        // string vs string_view
        REQUIRE(s_hello <  sv_world);
        REQUIRE(s_hello <= sv_world);
        REQUIRE(sv_world >  s_hello);
        REQUIRE(sv_world >= s_hello);

        // equal case
        REQUIRE(s_hello <= sz_hello);
        REQUIRE(sz_hello >= s_hello);
        REQUIRE(s_hello <= sv_hello);
        REQUIRE(sv_hello >= s_hello);
    }

    SECTION("prefix longer: 'Hello World.' vs 'Hello World...'")
    {
        // string vs string
        REQUIRE(s_long >  s_world);
        REQUIRE(s_long >= s_world);
        REQUIRE(s_world <  s_long);
        REQUIRE(s_world <= s_long);

        // string vs const char*
        REQUIRE(s_long >  sz_world);
        REQUIRE(s_long >= sz_world);
        REQUIRE(sz_world <  s_long);
        REQUIRE(sz_world <= s_long);

        // string vs string_view
        REQUIRE(s_long >  sv_world);
        REQUIRE(s_long >= sv_world);
        REQUIRE(sv_world <  s_long);
        REQUIRE(sv_world <= s_long);

        // equal case
        REQUIRE(s_long >= sz_long);
        REQUIRE(sz_long <= s_long);
        REQUIRE(s_long >= sv_long);
        REQUIRE(sv_long <= s_long);
    }

    SECTION("empty strings and default rune")
    {
        memepp::string s_empty1;
        memepp::string s_empty2;
        const char* sz_empty = "";
        memepp::string_view sv_empty = sz_empty;
        memepp::rune r_default;

        // string vs string (both empty)
        REQUIRE(s_empty1 <= s_empty2);
        REQUIRE(s_empty1 >= s_empty2);
        REQUIRE(s_empty2 <= s_empty1);
        REQUIRE(s_empty2 >= s_empty1);

        // string vs const char*
        REQUIRE(s_empty1 <= sz_empty);
        REQUIRE(s_empty1 >= sz_empty);
        REQUIRE(sz_empty <= s_empty1);
        REQUIRE(sz_empty >= s_empty1);

        // string vs string_view
        REQUIRE(s_empty1 <= sv_empty);
        REQUIRE(s_empty1 >= sv_empty);
        REQUIRE(sv_empty <= s_empty1);
        REQUIRE(sv_empty >= s_empty1);

        // string vs rune (default)
        REQUIRE(s_empty1 <= r_default);
        REQUIRE(s_empty1 >= r_default);
        REQUIRE(r_default <= s_empty1);
        REQUIRE(r_default >= s_empty1);
    }

    SECTION("string vs rune")
    {
        memepp::rune r_H = 'H';

        // string vs rune
        REQUIRE(s_world >  r_H);
        REQUIRE(s_world >= r_H);
        REQUIRE(r_H <  s_world);
        REQUIRE(r_H <= s_world);
    }
}

// ---------------------------------------------------------------------------
// operator== / operator!= (equality)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string operator== (string, string)", "[string]")
{
    memepp::string a("Hello");
    memepp::string b("Hello");
    memepp::string c("World");

    REQUIRE(a == b);
    REQUIRE_FALSE(a == c);
}

TEST_CASE("memepp::string operator!= (string, string)", "[string]")
{
    memepp::string a("Hello");
    memepp::string b("World");
    memepp::string c("Hello");

    REQUIRE(a != b);
    REQUIRE_FALSE(a != c);
}

TEST_CASE("memepp::string operator== (const char* lhs, string rhs)", "[string]")
{
    memepp::string s("Hello");

    REQUIRE("Hello" == s);
    REQUIRE_FALSE("World" == s);
}

TEST_CASE("memepp::string operator!= (const char* lhs, string rhs)", "[string]")
{
    memepp::string s("Hello");

    REQUIRE("World" != s);
    REQUIRE_FALSE("Hello" != s);
}

TEST_CASE("memepp::string operator== (string lhs, const char* rhs)", "[string]")
{
    memepp::string s("Hello");

    REQUIRE(s == "Hello");
    REQUIRE_FALSE(s == "World");
}

TEST_CASE("memepp::string operator!= (string lhs, const char* rhs)", "[string]")
{
    memepp::string s("Hello");

    REQUIRE(s != "World");
    REQUIRE_FALSE(s != "Hello");
}

TEST_CASE("memepp::string operator== (string::const_pointer lhs, string rhs)", "[string]")
{
    memepp::string s("Hello");
    memepp::string s_same("Hello");
    memepp::string s_diff("World");
    memepp::string::const_pointer p = s.bytes();

    REQUIRE(p == s_same);
    REQUIRE_FALSE(p == s_diff);
}

TEST_CASE("memepp::string operator!= (string::const_pointer lhs, string rhs)", "[string]")
{
    memepp::string s("Hello");
    memepp::string s_same("Hello");
    memepp::string s_diff("World");
    memepp::string::const_pointer p = s.bytes();

    REQUIRE(p != s_diff);
    REQUIRE_FALSE(p != s_same);
}

TEST_CASE("memepp::string operator== (string lhs, string::const_pointer rhs)", "[string]")
{
    memepp::string s("Hello");
    memepp::string s_same("Hello");
    memepp::string s_diff("World");
    memepp::string::const_pointer p = s.bytes();

    REQUIRE(s_same == p);
    REQUIRE_FALSE(s_diff == p);
}

TEST_CASE("memepp::string operator!= (string lhs, string::const_pointer rhs)", "[string]")
{
    memepp::string s("Hello");
    memepp::string s_same("Hello");
    memepp::string s_diff("World");
    memepp::string::const_pointer p = s.bytes();

    REQUIRE(s_diff != p);
    REQUIRE_FALSE(s_same != p);
}

// ---------------------------------------------------------------------------
// operator<  operator>  operator<=  operator>=  with string::const_pointer
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string compare with string::const_pointer", "[string]")
{
    memepp::string s_apple("Apple");
    memepp::string s_banana("Banana");
    memepp::string s_apple2("Apple");

    memepp::string::const_pointer p_apple  = s_apple.bytes();
    memepp::string::const_pointer p_banana = s_banana.bytes();

    // string op const_pointer
    REQUIRE(s_apple  <  p_banana);
    REQUIRE(s_banana >  p_apple);
    REQUIRE(s_apple  <= p_banana);
    REQUIRE(s_banana >= p_apple);

    // const_pointer op string
    REQUIRE(p_apple  <  s_banana);
    REQUIRE(p_banana >  s_apple);
    REQUIRE(p_apple  <= s_banana);
    REQUIRE(p_banana >= s_apple);

    // equal cases (<=, >= true; <, > false)
    REQUIRE(s_apple  <= p_apple);
    REQUIRE(s_apple  >= p_apple);
    REQUIRE(p_apple  <= s_apple2);
    REQUIRE(p_apple  >= s_apple2);
    REQUIRE_FALSE(s_apple  <  p_apple);
    REQUIRE_FALSE(s_apple  >  p_apple);
    REQUIRE_FALSE(p_apple  <  s_apple2);
    REQUIRE_FALSE(p_apple  >  s_apple2);
}
