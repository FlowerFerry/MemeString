
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>

TEST_CASE("memepp::string_view to_valid_utf8", "[string_view]")
{
    // empty string_view
    memepp::string_view s01 = "";
    auto r01 = s01.to_valid_utf8();
    REQUIRE(r01 == "");
    REQUIRE(r01.size() == 0);

    // already valid ASCII
    memepp::string_view s02 = "Hello, World!";
    auto r02 = s02.to_valid_utf8();
    REQUIRE(r02 == "Hello, World!");

    // already valid UTF-8 with multibyte (emoji U+1F60A = \xF0\x9F\x98\x8A)
    memepp::string_view s03("Hello, \xF0\x9F\x98\x8A", 11);
    auto r03 = s03.to_valid_utf8();
    REQUIRE(r03.size() == 11);
    REQUIRE(r03 == s03);

    // invalid UTF-8: truncated multibyte sequence (\xF0\x9F\x98 only, missing last byte)
    memepp::string_view s04("Hello, \xF0\x9F\x98", 10);
    auto r04 = s04.to_valid_utf8();
    // invalid tail bytes stripped -> only "Hello, " remains
    REQUIRE(r04 == "Hello, ");
}
