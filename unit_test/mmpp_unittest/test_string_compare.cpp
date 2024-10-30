
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

TEST_CASE("memepp::string - 33", "string compare")
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

    memepp::string s04_01 = "Hello World...";
    const char* sz04_01 = "Hello World...";
    memepp::string_view sv04_01 = sz04_01;
    
    REQUIRE(s04_01 > s01_01);
    REQUIRE(s04_01 >= s01_01);
    REQUIRE(s01_01 <  s04_01);
    REQUIRE(s01_01 <= s04_01);

    REQUIRE(s04_01 > sz01_01);
    REQUIRE(s04_01 >= sz01_01);
    REQUIRE(sz01_01 <  s04_01);
    REQUIRE(sz01_01 <= s04_01);

    REQUIRE(s04_01 >  sv01_01);
    REQUIRE(s04_01 >= sv01_01);
    REQUIRE(sv01_01 <  s04_01);
    REQUIRE(sv01_01 <= s04_01);

    memepp::string s01_02;
    memepp::string s02_02;
    const char* sz01_02 = "";
    memepp::string_view sv01_02 = sz01_02;
    
    REQUIRE(s01_02 <= s02_02);
    REQUIRE(s01_02 >= s02_02);
    REQUIRE(s02_02 <= s01_02);
    REQUIRE(s02_02 >= s01_02);

    REQUIRE(s01_02 <= sz01_02);
    REQUIRE(s01_02 >= sz01_02);
    REQUIRE(sz01_02 <= s01_02);
    REQUIRE(sz01_02 >= s01_02);
    
    REQUIRE(s01_02 <= sv01_02);
    REQUIRE(s01_02 >= sv01_02);
    REQUIRE(sv01_02 <= s01_02);
    REQUIRE(sv01_02 >= s01_02);
}
