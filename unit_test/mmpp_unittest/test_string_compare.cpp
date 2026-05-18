
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

TEST_CASE("memepp::string compare", "[string]")
{
    memepp::string s01_01 = "Hello World.";
    memepp::string s02_01 = "Hello World!";
    const char* sz01_01 = "Hello World.";
    const char* sz02_01 = "Hello World!";
    memepp::string_view sv01_01 = sz01_01;
    memepp::string_view sv02_01 = sz02_01;
    
    REQUIRE(s01_01 >  s02_01);
    REQUIRE(s01_01 >= s02_01);
    REQUIRE(s02_01 <  s01_01);
    REQUIRE(s02_01 <= s01_01);
    
    REQUIRE(s01_01 >  sz02_01);
    REQUIRE(s01_01 >= sz02_01);
    REQUIRE(sz02_01 <  s01_01);
    REQUIRE(sz02_01 <= s01_01);

    REQUIRE(s01_01 >  sv02_01);
    REQUIRE(s01_01 >= sv02_01);
    REQUIRE(sv02_01 <  s01_01);
    REQUIRE(sv02_01 <= s01_01);
    
    REQUIRE(s01_01 <= sz01_01);
    REQUIRE(s01_01 >= sz01_01);
    REQUIRE(sz01_01 <= s01_01);
    REQUIRE(sz01_01 >= s01_01);

    REQUIRE(s01_01 <= sv01_01);
    REQUIRE(s01_01 >= sv01_01);
    REQUIRE(sv01_01 <= s01_01);
    REQUIRE(sv01_01 >= s01_01);

    memepp::string s03_01 = "Hello";
    const char* sz03_01 = "Hello";
    memepp::string_view sv03_01 = sz03_01;

    REQUIRE(s03_01 <  s01_01);
    REQUIRE(s03_01 <= s01_01);
    REQUIRE(s01_01 >  s03_01);
    REQUIRE(s01_01 >= s03_01);
    
    REQUIRE(s03_01 <  sz01_01);
    REQUIRE(s03_01 <= sz01_01);
    REQUIRE(sz01_01 >  s03_01);
    REQUIRE(sz01_01 >= s03_01);
    
    REQUIRE(s03_01 <  sv01_01);
    REQUIRE(s03_01 <= sv01_01);
    REQUIRE(sv01_01 >  s03_01);
    REQUIRE(sv01_01 >= s03_01);

    REQUIRE(s03_01 <= sz03_01);
    REQUIRE(sz03_01 >= s03_01);

    REQUIRE(s03_01 <= sv03_01);
    REQUIRE(sv03_01 >= s03_01);

    memepp::string s04_01 = "Hello World...";
    const char* sz04_01 = "Hello World...";
    memepp::string_view sv04_01 = sz04_01;
    
    REQUIRE(s04_01 >  s01_01);
    REQUIRE(s04_01 >= s01_01);
    REQUIRE(s01_01 <  s04_01);
    REQUIRE(s01_01 <= s04_01);

    REQUIRE(s04_01 >  sz01_01);
    REQUIRE(s04_01 >= sz01_01);
    REQUIRE(sz01_01 <  s04_01);
    REQUIRE(sz01_01 <= s04_01);

    REQUIRE(s04_01 >  sv01_01);
    REQUIRE(s04_01 >= sv01_01);
    REQUIRE(sv01_01 <  s04_01);
    REQUIRE(sv01_01 <= s04_01);

    REQUIRE(s04_01 >= sz04_01);
    REQUIRE(sz04_01 <= s04_01);

    REQUIRE(s04_01 >= sv04_01);
    REQUIRE(sv04_01 <= s04_01);

    memepp::string s05_01;
    memepp::string s05_02;
    const char* sz05_01 = "";
    memepp::string_view sv05_01 = sz05_01;
    memepp::rune r05_01;
    
    REQUIRE(s05_01 <= s05_02);
    REQUIRE(s05_01 >= s05_02);
    REQUIRE(s05_02 <= s05_01);
    REQUIRE(s05_02 >= s05_01);

    REQUIRE(s05_01 <= sz05_01);
    REQUIRE(s05_01 >= sz05_01);
    REQUIRE(sz05_01 <= s05_01);
    REQUIRE(sz05_01 >= s05_01);
    
    REQUIRE(s05_01 <= sv05_01);
    REQUIRE(s05_01 >= sv05_01);
    REQUIRE(sv05_01 <= s05_01);
    REQUIRE(sv05_01 >= s05_01);

    REQUIRE(s05_01 <= r05_01);
    REQUIRE(s05_01 >= r05_01);
    REQUIRE(r05_01 <= s05_01);
    REQUIRE(r05_01 >= s05_01);
    
    memepp::rune r06_01 = 'H';

    REQUIRE(s01_01 >  r06_01);
    REQUIRE(s01_01 >= r06_01);
    REQUIRE(r06_01 <  s01_01);
    REQUIRE(r06_01 <= s01_01);
    REQUIRE(s01_01 >  s03_01);
    REQUIRE(s01_01 >= s03_01);
    REQUIRE(s03_01 <  s01_01);
    REQUIRE(s03_01 <= s01_01);
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
