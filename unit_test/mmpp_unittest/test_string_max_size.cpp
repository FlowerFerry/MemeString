
#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <meme/string.h>

#include <string>

TEST_CASE("memepp::string max_size", "[string]")
{
    memepp::string s;
    REQUIRE(s.max_size() == MemeStringOption_getStorageSmallLimit());

    memepp::string s2 = "hello";
    REQUIRE(s2.max_size() == MemeStringOption_getStorageSmallLimit());

    // max_size() should be non-negative for any string
    REQUIRE(s.max_size() == s2.max_size());
}
