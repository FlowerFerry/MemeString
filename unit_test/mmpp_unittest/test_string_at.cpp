
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

#include <stdexcept>

TEST_CASE("memepp::string at", "[string]")
{
    memepp::string str = "0123456789";
    for (int i = 0; i < 10; ++i) {
        CAPTURE(i);
        REQUIRE(str.at(i) == '0' + i);
    }

    // out of range
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(str.at(10), std::out_of_range);
    REQUIRE_THROWS_AS(str.at(-1), std::out_of_range);
#endif

    memepp::string_view sv = str;
    for (int i = 0; i < 10; ++i) {
        CAPTURE(i);
        REQUIRE(sv.at(i) == '0' + i);
    }

    // out of range
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(sv.at(10), std::out_of_range);
    REQUIRE_THROWS_AS(sv.at(-1), std::out_of_range);
#endif
}

TEST_CASE("memepp::string operator[]", "[string]")
{
    memepp::string str = "0123456789";
    REQUIRE(str[0] == '0');
    REQUIRE(str[1] == '1');
    REQUIRE(str[2] == '2');
    REQUIRE(str[3] == '3');
    REQUIRE(str[9] == '9');

    memepp::string_view sv = str;
    REQUIRE(sv[0] == '0');
    REQUIRE(sv[1] == '1');
    REQUIRE(sv[9] == '9');
}
