
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>

TEST_CASE("memepp::string_view trim_if byte predicate", "[string_view]")
{
    // trim leading/trailing spaces
    memepp::string_view s01 = "  Hello, World!  ";
    auto r01 = s01.trim_if([](mmbyte_t ch) { return ch == ' '; });
    REQUIRE(r01 == "Hello, World!");

    // trim leading/trailing custom chars
    memepp::string_view s02 = "~~~Hello~~~";
    auto r02 = s02.trim_if([](mmbyte_t ch) { return ch == '~'; });
    REQUIRE(r02 == "Hello");

    // no trimmable chars -> unchanged
    memepp::string_view s03 = "Hello";
    auto r03 = s03.trim_if([](mmbyte_t ch) { return ch == '~'; });
    REQUIRE(r03 == "Hello");

    // all chars trimmable -> empty
    memepp::string_view s04 = "!!!";
    auto r04 = s04.trim_if([](mmbyte_t ch) { return ch == '!'; });
    REQUIRE(r04 == "");

    // empty string_view
    memepp::string_view s05 = "";
    auto r05 = s05.trim_if([](mmbyte_t ch) { return ch == ' '; });
    REQUIRE(r05 == "");

    // asymmetric leading/trailing
    memepp::string_view s06 = "!!Hello!";
    auto r06 = s06.trim_if([](mmbyte_t ch) { return ch == '!'; });
    REQUIRE(r06 == "Hello");
}

TEST_CASE("memepp::string_view trim_if_rune rune predicate", "[string_view]")
{
    // trim ASCII spaces via rune
    memepp::string_view s01 = "   abc   ";
    auto r01 = s01.trim_if_rune([](const memepp::rune& r) {
        return r == memepp::rune(' ');
    });
    REQUIRE(r01 == "abc");

    // trim multibyte Unicode ideographic space (U+3000, UTF-8: \xE3\x80\x80)
    memepp::string_view s02("\xE3\x80\x80" "abc" "\xE3\x80\x80", 9);
    auto r02 = s02.trim_if_rune([](const memepp::rune& r) {
        const char u3000[4] = { '\xE3', '\x80', '\x80', '\0' };
        return r == memepp::rune(reinterpret_cast<const uint8_t*>(u3000), 3);
    });
    REQUIRE(r02 == "abc");

    // no trimming needed
    memepp::string_view s03 = "Hello";
    auto r03 = s03.trim_if_rune([](const memepp::rune& r) {
        return r == memepp::rune(' ');
    });
    REQUIRE(r03 == "Hello");

    // all chars trimmable -> empty
    memepp::string_view s04 = "   ";
    auto r04 = s04.trim_if_rune([](const memepp::rune& r) {
        return r == memepp::rune(' ');
    });
    REQUIRE(r04 == "");
}
