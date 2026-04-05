#include <catch2/catch.hpp>
#include <megopp/endian/net.h>
#include <megopp/endian/types.h>
#include <megopp/endian/byte_swap.h>

#include <cstdint>

using namespace mgpp::endian;
using mgpp::endian_t;

// =========================================================================
// endian_t enum values
// =========================================================================

TEST_CASE("endian_t enum values are stable", "[net][endian_t]")
{
    REQUIRE(static_cast<int>(endian_t::little_byte) == 0);
    REQUIRE(static_cast<int>(endian_t::big_byte)    == 1);
    REQUIRE(static_cast<int>(endian_t::little_word) == 2);
    REQUIRE(static_cast<int>(endian_t::big_word)    == 3);
}

TEST_CASE("endian_t::native equals the compile-time platform endianness", "[net][endian_t]")
{
#if MEGO_ENDIAN__LITTLE_BYTE
    REQUIRE(endian_t::native == endian_t::little_byte);
#elif MEGO_ENDIAN__BIG_BYTE
    REQUIRE(endian_t::native == endian_t::big_byte);
#elif MEGO_ENDIAN__LITTLE_WORD
    REQUIRE(endian_t::native == endian_t::little_word);
#elif MEGO_ENDIAN__BIG_WORD
    REQUIRE(endian_t::native == endian_t::big_word);
#endif
}

// =========================================================================
// hton / ntoh — roundtrip
// =========================================================================

TEST_CASE("ntoh(hton(v)) roundtrip for uint8_t", "[net][hton][ntoh]")
{
    REQUIRE(ntoh(hton(uint8_t(0x00))) == uint8_t(0x00));
    REQUIRE(ntoh(hton(uint8_t(0xAB))) == uint8_t(0xAB));
    REQUIRE(ntoh(hton(uint8_t(0xFF))) == uint8_t(0xFF));
}

TEST_CASE("ntoh(hton(v)) roundtrip for uint16_t", "[net][hton][ntoh]")
{
    REQUIRE(ntoh(hton(uint16_t(0x0000))) == uint16_t(0x0000));
    REQUIRE(ntoh(hton(uint16_t(0x1234))) == uint16_t(0x1234));
    REQUIRE(ntoh(hton(uint16_t(0xFFFF))) == uint16_t(0xFFFF));
}

TEST_CASE("ntoh(hton(v)) roundtrip for uint32_t", "[net][hton][ntoh]")
{
    REQUIRE(ntoh(hton(uint32_t(0x00000000))) == uint32_t(0x00000000));
    REQUIRE(ntoh(hton(uint32_t(0x01020304))) == uint32_t(0x01020304));
    REQUIRE(ntoh(hton(uint32_t(0xDEADBEEF))) == uint32_t(0xDEADBEEF));
    REQUIRE(ntoh(hton(uint32_t(0xFFFFFFFF))) == uint32_t(0xFFFFFFFF));
}

TEST_CASE("ntoh(hton(v)) roundtrip for uint64_t", "[net][hton][ntoh]")
{
    REQUIRE(ntoh(hton(uint64_t(0x0102030405060708ULL))) == uint64_t(0x0102030405060708ULL));
    REQUIRE(ntoh(hton(uint64_t(0xCAFEBABEDEADBEEFULL))) == uint64_t(0xCAFEBABEDEADBEEFULL));
}

// =========================================================================
// hton on little-byte platform converts to network (big) byte order
// =========================================================================

#if MEGO_ENDIAN__LITTLE_BYTE
TEST_CASE("hton on little-byte platform reverses bytes (uint16_t)", "[net][hton]")
{
    REQUIRE(hton(uint16_t(0x0102)) == uint16_t(0x0201));
    REQUIRE(hton(uint16_t(0x1234)) == uint16_t(0x3412));
}

TEST_CASE("hton on little-byte platform reverses bytes (uint32_t)", "[net][hton]")
{
    REQUIRE(hton(uint32_t(0x01020304)) == uint32_t(0x04030201));
    REQUIRE(hton(uint32_t(0x12345678)) == uint32_t(0x78563412));
}

TEST_CASE("hton equals byte_swap on little-byte platform", "[net][hton]")
{
    const uint32_t v = 0xAABBCCDD;
    REQUIRE(hton(v) == byte_swap(v));
}
#endif

#if MEGO_ENDIAN__BIG_BYTE
TEST_CASE("hton is identity on big-byte platform", "[net][hton]")
{
    REQUIRE(hton(uint32_t(0x12345678)) == uint32_t(0x12345678));
    REQUIRE(hton(uint16_t(0x1234))     == uint16_t(0x1234));
}
#endif

// =========================================================================
// htot — convert host to target endianness
// =========================================================================

TEST_CASE("htot to native endian is identity", "[net][htot]")
{
    const uint32_t v = 0xDEADBEEF;
#if MEGO_ENDIAN__LITTLE_BYTE
    REQUIRE(htot(v, endian_t::little_byte) == v);
#elif MEGO_ENDIAN__BIG_BYTE
    REQUIRE(htot(v, endian_t::big_byte) == v);
#endif
}

#if MEGO_ENDIAN__LITTLE_BYTE
TEST_CASE("htot little->big is byte_swap on little-byte platform", "[net][htot]")
{
    const uint32_t v = 0x01020304;
    REQUIRE(htot(v, endian_t::big_byte) == byte_swap(v));
}

TEST_CASE("htot big->little roundtrip on little-byte platform", "[net][htot]")
{
    const uint32_t original = 0x01020304;
    const uint32_t big_form = htot(original, endian_t::big_byte);
    // Re-interpret big_form as if it's big-endian and convert back to host (little)
    // htot(big_form, big_byte) converts big_form from little-host to big — applying
    // byte_swap again gives us back original
    REQUIRE(byte_swap(big_form) == original);
}

TEST_CASE("htot uint16_t to big_byte on little-byte platform", "[net][htot]")
{
    REQUIRE(htot(uint16_t(0x0102), endian_t::big_byte) == uint16_t(0x0201));
}
#endif

#if MEGO_ENDIAN__BIG_BYTE
TEST_CASE("htot big->little is byte_swap on big-byte platform", "[net][htot]")
{
    const uint32_t v = 0x01020304;
    REQUIRE(htot(v, endian_t::little_byte) == byte_swap(v));
}

TEST_CASE("htot big->big is identity on big-byte platform", "[net][htot]")
{
    const uint32_t v = 0x01020304;
    REQUIRE(htot(v, endian_t::big_byte) == v);
}
#endif

TEST_CASE("htot for 1-byte type is always identity", "[net][htot]")
{
    // byte_swap of single byte is identity; word_swap is also identity for 1-byte
    REQUIRE(htot(uint8_t(0xAB), endian_t::big_byte)    == uint8_t(0xAB));
    REQUIRE(htot(uint8_t(0xAB), endian_t::little_byte) == uint8_t(0xAB));
}
