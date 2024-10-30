
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

TEST_CASE("memepp::string - 34", "string front/back")
{
    //memepp::string s01_01 = "Hello World.";
    //memepp::string s02_01 = "Hello World!";
    //const char* sz01_01 = "Hello World.";
    //const char* sz02_01 = "Hello World!";
    //memepp::string_view sv01_01 = sz01_01;
    //memepp::string_view sv02_01 = sz02_01;

    //REQUIRE(s01_01.front() == 'H');
    //REQUIRE(s01_01.back() == '.');
    //REQUIRE(s02_01.front() == 'H');
    //REQUIRE(s02_01.back() == '!');

    //REQUIRE(sz01_01[0] == 'H');
    //REQUIRE(sz01_01[11] == '.');
    //REQUIRE(sz02_01[0] == 'H');
    //REQUIRE(sz02_01[11] == '!');

    //REQUIRE(sv01_01.front() == 'H');
    //REQUIRE(sv01_01.back() == '.');
    //REQUIRE(sv02_01.front() == 'H');
    //REQUIRE(sv02_01.back() == '!');

    //memepp::string s03_01 = "Hello";
    //const char* sz03_01 = "Hello";
    //memepp::string_view sv03_01 = sz03_01;

    //REQUIRE(s03_01.front() == 'H');
    //REQUIRE(s03_01.back() == 'o');

    //REQUIRE(sz03_01[0] == 'H');
    //REQUIRE(sz03_01[4] == 'o');

    //REQUIRE(sv03_01.front() == 'H');
    //REQUIRE(sv03_01.back() == 'o');

    //memepp::string s04_01 = "Hello World...";
    //const char* sz04_01 = "Hello World...";
    //memepp::string_view sv04_01 = sz04_01;

    //REQUIRE(s04_01.front() == 'H');
    //REQUIRE(s04_01.back() == '.');

    //REQUIRE(sz04_01[0] == 'H');
    //REQUIRE(sz04_01[13] == '.');

    //REQUIRE(sv04_01.front() == 'H');
    //REQUIRE(sv04_01.back() == '.');

    memepp::rune r01_02;
    memepp::rune r02_02;
    memepp::string s01_02 = "Hello World!";
    
    r01_02 = s01_02.rune_front();
    r02_02 = s01_02.rune_back ();

    REQUIRE(r01_02 == 'H');
    REQUIRE(r02_02 == '!');

    memepp::string s02_02 = "";
    r01_02 = s02_02.rune_front();
    r02_02 = s02_02.rune_back();

    REQUIRE(r01_02.valid() == false);
    REQUIRE(r02_02.valid() == false);

    uint8_t u8_01_02[] = { 0xE6, 0x82, 0x00 };
    memepp::string_view sv01_02 = u8_01_02;
    
    r01_02 = sv01_02.rune_front();
    r02_02 = sv01_02.rune_back();
    
    REQUIRE(r01_02.valid() == false);
    REQUIRE(r02_02.valid() == false);

    uint8_t u8_02_02[] = { 0xe6, 0x82, 0xa8, 0xe5, 0xa5, 0x00 };
    memepp::string_view sv02_02 = u8_02_02;

    r01_02 = sv02_02.rune_front();
    r02_02 = sv02_02.rune_back();

    REQUIRE(r01_02 == u8"您");
    REQUIRE(r02_02.valid() == false);
}