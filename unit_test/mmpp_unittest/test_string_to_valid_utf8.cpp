
#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <megopp/util/scope_cleanup.h>

TEST_CASE("memepp::string to_valid_utf8", "[string]")
{
    // empty string
    memepp::string s01 = "";
    auto r01 = s01.to_valid_utf8();
    REQUIRE(r01 == "");
    REQUIRE(r01.size() == 0);

    // already valid ASCII
    memepp::string s02 = "Hello, World!";
    auto r02 = s02.to_valid_utf8();
    REQUIRE(r02 == "Hello, World!");

    // already valid UTF-8 with multibyte (emoji)
    memepp::string s03("Hello, \xF0\x9F\x98\x8A", 11);
    auto r03 = s03.to_valid_utf8();
    REQUIRE(r03.size() == 11);
    REQUIRE(r03 == s03);

    // invalid UTF-8: truncated multibyte sequence (\xF0\x9F\x98 only, missing last byte)
    memepp::string s04("Hello, \xF0\x9F\x98", 10);
    auto r04 = s04.to_valid_utf8();
    // invalid tail bytes stripped → only "Hello, " remains
    REQUIRE(r04 == "Hello, ");
}
