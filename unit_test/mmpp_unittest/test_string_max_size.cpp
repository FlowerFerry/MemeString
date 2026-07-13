
#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <meme/string.h>

TEST_CASE("memepp::string max_size", "[string]")
{
    memepp::string s;
    REQUIRE(s.max_size() >= 0);

    memepp::string s2 = "hello";
    REQUIRE(s2.max_size() >= static_cast<memepp::string::size_type>(s2.size()));

    // max_size() should be non-negative for any string
    REQUIRE(s.max_size() == s2.max_size());
}
