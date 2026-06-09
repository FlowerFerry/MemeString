#include <catch2/catch.hpp>
#include <megopp/numeric/types.h>

TEST_CASE("int_neg_mode_t - enum values", "[numeric_types]")
{
    REQUIRE(static_cast<int>(mgpp::int_neg_mode_t::standard) == 0);
    REQUIRE(static_cast<int>(mgpp::int_neg_mode_t::sign_bit) == 1);
}

TEST_CASE("int_neg_mode_t - standard and sign_bit are distinct", "[numeric_types]")
{
    REQUIRE(mgpp::int_neg_mode_t::standard != mgpp::int_neg_mode_t::sign_bit);
}

TEST_CASE("int_neg_mode_t - underlying type is uint8_t", "[numeric_types]")
{
    REQUIRE(sizeof(mgpp::int_neg_mode_t) == 1);
}
