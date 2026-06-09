#include <catch2/catch.hpp>
#include <megopp/endian/types.h>

TEST_CASE("endian_t - enum values are correct", "[endian_types]")
{
    REQUIRE(static_cast<int>(mgpp::endian_t::little_byte) == 0);
    REQUIRE(static_cast<int>(mgpp::endian_t::big_byte) == 1);
    REQUIRE(static_cast<int>(mgpp::endian_t::little_word) == 2);
    REQUIRE(static_cast<int>(mgpp::endian_t::big_word) == 3);
}

TEST_CASE("endian_t - native is set to platform endianness", "[endian_types]")
{
    // On x86/x64 Windows, native is always little_byte
#if MEGO_ENDIAN__LITTLE_BYTE
    REQUIRE(mgpp::endian_t::native == mgpp::endian_t::little_byte);
#elif MEGO_ENDIAN__BIG_BYTE
    REQUIRE(mgpp::endian_t::native == mgpp::endian_t::big_byte);
#endif
}

TEST_CASE("endian_t - little_byte and big_byte are distinct", "[endian_types]")
{
    REQUIRE(mgpp::endian_t::little_byte != mgpp::endian_t::big_byte);
}

TEST_CASE("endian_t - word variants are distinct", "[endian_types]")
{
    REQUIRE(mgpp::endian_t::little_word != mgpp::endian_t::big_word);
    REQUIRE(mgpp::endian_t::little_word != mgpp::endian_t::little_byte);
    REQUIRE(mgpp::endian_t::big_word != mgpp::endian_t::big_byte);
}
