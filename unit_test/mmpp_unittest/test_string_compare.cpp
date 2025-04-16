
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
