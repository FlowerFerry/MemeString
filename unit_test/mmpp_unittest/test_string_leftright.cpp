
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

TEST_CASE("memepp::string - 41", "string left")
{
    memepp::string str = "0123456789";
    REQUIRE(str.left(0) == "");
    REQUIRE(str.left(1) == "0");
    REQUIRE(str.left(2) == "01");
    REQUIRE(str.left(3) == "012");
    REQUIRE(str.left(4) == "0123");
    REQUIRE(str.left(5) == "01234");
    REQUIRE(str.left(6) == "012345");
    REQUIRE(str.left(7) == "0123456");
    REQUIRE(str.left(8) == "01234567");
    REQUIRE(str.left(9) == "012345678");
    REQUIRE(str.left(10) == "0123456789");
    REQUIRE(str.left(11) == "0123456789");
    REQUIRE(str.left(-1) == "0123456789");
}

TEST_CASE("memepp::string - 42", "string right")
{
    memepp::string str = "0123456789";
    REQUIRE(str.right(0) == "");
    REQUIRE(str.right(1) == "9");
    REQUIRE(str.right(2) == "89");
    REQUIRE(str.right(3) == "789");
    REQUIRE(str.right(4) == "6789");
    REQUIRE(str.right(5) == "56789");
    REQUIRE(str.right(6) == "456789");
    REQUIRE(str.right(7) == "3456789");
    REQUIRE(str.right(8) == "23456789");
    REQUIRE(str.right(9) == "123456789");
    REQUIRE(str.right(10) == "0123456789");
    REQUIRE(str.right(11) == "0123456789");
    REQUIRE(str.right(-1) == "0123456789");
}