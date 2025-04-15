
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>


TEST_CASE("memepp::string - 45", "string from_hexadecimals")
{
    REQUIRE(memepp::from_hexadecimals(NULL,  0) == "");
    REQUIRE(memepp::from_hexadecimals(NULL,  1) == "");
    REQUIRE(memepp::from_hexadecimals(NULL, -1) == "");
    REQUIRE(memepp::from_hexadecimals("", NULL,  0) == "");
    REQUIRE(memepp::from_hexadecimals("", NULL,  1) == "");
    REQUIRE(memepp::from_hexadecimals("", NULL, -1) == "");

    uint8_t buf01_01[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    auto str01_01 = memepp::from_hexadecimals(buf01_01, sizeof(buf01_01));
    REQUIRE(str01_01.size() == sizeof(buf01_01) * 2);
    REQUIRE(str01_01 == "000102030405060708090A0B0C0D0E0F");

    auto str01_02 = memepp::from_hexadecimals(" ", buf01_01, sizeof(buf01_01));
    REQUIRE(str01_02.size() == sizeof(buf01_01) * 3 - 1);
    REQUIRE(str01_02 == "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");

    auto str01_03 = memepp::from_hexadecimals(buf01_01, -1);
    REQUIRE(str01_03 == "");

    auto str01_04 = memepp::from_hexadecimals(" ", buf01_01, -1);
    REQUIRE(str01_04 == "");
}
