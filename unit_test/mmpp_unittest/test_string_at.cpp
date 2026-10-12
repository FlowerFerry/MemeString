
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>
#include <memepp/errc.hpp>

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
    REQUIRE_THROWS_AS(str.at(10), memepp::errc_except);
    REQUIRE_THROWS_AS(str.at(-1), memepp::errc_except);
#else
    // exception disabled: at() returns sentinel '\0' and sets errc
    memepp::set_errc(0);  // reset errc
    REQUIRE(str.at(10) == '\0');
    REQUIRE(memepp::get_errc() != 0);
    
    memepp::set_errc(0);  // reset errc
    REQUIRE(str.at(-1) == '\0');
    REQUIRE(memepp::get_errc() != 0);
#endif

    memepp::string_view sv = str;
    for (int i = 0; i < 10; ++i) {
        CAPTURE(i);
        REQUIRE(sv.at(i) == '0' + i);
    }

    // out of range
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(sv.at(10), memepp::errc_except);
    REQUIRE_THROWS_AS(sv.at(-1), memepp::errc_except);
#else
    // exception disabled: at() returns sentinel '\0' and sets errc
    memepp::set_errc(0);  // reset errc
    REQUIRE(sv.at(10) == '\0');
    REQUIRE(memepp::get_errc() != 0);
    
    memepp::set_errc(0);  // reset errc
    REQUIRE(sv.at(-1) == '\0');
    REQUIRE(memepp::get_errc() != 0);
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
