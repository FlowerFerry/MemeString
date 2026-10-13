
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

#include <string>

TEST_CASE("memepp::string left", "[string]")
{
    memepp::string str = "0123456789";
    const std::string digits = "0123456789";

    // count in [0, size()]: the first count bytes
    for (int count = 0; count <= 10; ++count) {
        CAPTURE(count);
        const std::string expected = digits.substr(0, static_cast<size_t>(count));
        REQUIRE(str.left(count) == expected.c_str());
    }

    // count >= size(): the whole string
    REQUIRE(str.left(11) == "0123456789");

    // negative count: documented-by-test behavior - returns the whole string
    REQUIRE(str.left(-1) == "0123456789");
}

TEST_CASE("memepp::string right", "[string]")
{
    memepp::string str = "0123456789";
    const std::string digits = "0123456789";

    // count in [0, size()]: the last count bytes
    for (int count = 0; count <= 10; ++count) {
        CAPTURE(count);
        const std::string expected = digits.substr(digits.size() - static_cast<size_t>(count));
        REQUIRE(str.right(count) == expected.c_str());
    }

    // count >= size(): the whole string
    REQUIRE(str.right(11) == "0123456789");

    // negative count: documented-by-test behavior - returns the whole string
    REQUIRE(str.right(-1) == "0123456789");
}
