
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

TEST_CASE("memepp::string at", "[string]")
{
    memepp::string str = "0123456789";
    for (int i = 0; i < 10; ++i) {
        CAPTURE(i);
        REQUIRE(str.at(i) == '0' + i);
    }

    // out of range
#if !MMOPT__EXCEPTION_DISABLED
    try {
        str.at(10);
        FAIL("Expected std::out_of_range exception");
    } catch (const std::out_of_range&) {
        // expected
    } catch (...) {
        FAIL("Expected std::out_of_range exception");
    }
    try {
        str.at(-1);
        FAIL("Expected std::out_of_range exception");
    } catch (const std::out_of_range&) {
        // expected
    } catch (...) {
        FAIL("Expected std::out_of_range exception");
    }
#endif

    memepp::string_view sv = str;
    for (int i = 0; i < 10; ++i) {
        CAPTURE(i);
        REQUIRE(sv.at(i) == '0' + i);
    }

    // out of range
#if !MMOPT__EXCEPTION_DISABLED
    try {
        sv.at(10);
        FAIL("Expected std::out_of_range exception");
    } catch (const std::out_of_range&) {
        // expected
    } catch (...) {
        FAIL("Expected std::out_of_range exception");
    }
    try {
        sv.at(-1);
        FAIL("Expected std::out_of_range exception");
    } catch (const std::out_of_range&) {
        // expected
    } catch (...) {
        FAIL("Expected std::out_of_range exception");
    }
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
