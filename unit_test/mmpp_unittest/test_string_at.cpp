
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

TEST_CASE("memepp::string - 11", "string at")
{
    memepp::string str = "0123456789";
    REQUIRE(str.at(0) == '0');
    REQUIRE(str.at(1) == '1');
    REQUIRE(str.at(2) == '2');
    REQUIRE(str.at(3) == '3');
    REQUIRE(str.at(4) == '4');
    REQUIRE(str.at(5) == '5');
    REQUIRE(str.at(6) == '6');
    REQUIRE(str.at(7) == '7');
    REQUIRE(str.at(8) == '8');
    REQUIRE(str.at(9) == '9');

    // out of range
#if MMOPT__EXCEPTION_DISABLED
    REQUIRE(str.at(10) == 0);
    REQUIRE(str.at(-1) == 0);
#else
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
    REQUIRE(sv.at(0) == '0');
    REQUIRE(sv.at(1) == '1');
    REQUIRE(sv.at(2) == '2');
    REQUIRE(sv.at(3) == '3');
    REQUIRE(sv.at(4) == '4');
    REQUIRE(sv.at(5) == '5');
    REQUIRE(sv.at(6) == '6');
    REQUIRE(sv.at(7) == '7');
    REQUIRE(sv.at(8) == '8');
    REQUIRE(sv.at(9) == '9');

    // out of range
#if MMOPT__EXCEPTION_DISABLED
    REQUIRE(sv.at(10) == 0);
    REQUIRE(sv.at(-1) == 0);
#else
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
