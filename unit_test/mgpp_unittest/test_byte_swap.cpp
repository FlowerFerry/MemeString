#include <catch2/catch.hpp>
#include <megopp/endian/byte_swap.h>

#include <cstdint>

using namespace mgpp::endian;

// =========================================================================
// byte_swap_with_size<N>::convert — compile-time static swap
// =========================================================================

TEST_CASE("byte_swap_with_size<1> is identity", "[byte_swap]")
{
    REQUIRE(byte_swap_with_size<1>::convert(0x00) == 0x00);
    REQUIRE(byte_swap_with_size<1>::convert(0xFF) == 0xFF);
    REQUIRE(byte_swap_with_size<1>::convert(0xAB) == 0xAB);
}

TEST_CASE("byte_swap_with_size<2> swaps two bytes", "[byte_swap]")
{
    REQUIRE(byte_swap_with_size<2>::convert(uint16_t(0x1234)) == uint16_t(0x3412));
    REQUIRE(byte_swap_with_size<2>::convert(uint16_t(0x0001)) == uint16_t(0x0100));
    REQUIRE(byte_swap_with_size<2>::convert(uint16_t(0xFF00)) == uint16_t(0x00FF));
    REQUIRE(byte_swap_with_size<2>::convert(uint16_t(0x0000)) == uint16_t(0x0000));
    REQUIRE(byte_swap_with_size<2>::convert(uint16_t(0xFFFF)) == uint16_t(0xFFFF));
}

TEST_CASE("byte_swap_with_size<4> reverses four bytes", "[byte_swap]")
{
    REQUIRE(byte_swap_with_size<4>::convert(uint32_t(0x12345678)) == uint32_t(0x78563412));
    REQUIRE(byte_swap_with_size<4>::convert(uint32_t(0x00000001)) == uint32_t(0x01000000));
    REQUIRE(byte_swap_with_size<4>::convert(uint32_t(0xFF000000)) == uint32_t(0x000000FF));
    REQUIRE(byte_swap_with_size<4>::convert(uint32_t(0x00000000)) == uint32_t(0x00000000));
    REQUIRE(byte_swap_with_size<4>::convert(uint32_t(0xFFFFFFFF)) == uint32_t(0xFFFFFFFF));
}

TEST_CASE("byte_swap_with_size<8> reverses eight bytes", "[byte_swap]")
{
    REQUIRE(byte_swap_with_size<8>::convert(uint64_t(0x0102030405060708ULL))
            == uint64_t(0x0807060504030201ULL));
    REQUIRE(byte_swap_with_size<8>::convert(uint64_t(0x0000000000000001ULL))
            == uint64_t(0x0100000000000000ULL));
    REQUIRE(byte_swap_with_size<8>::convert(uint64_t(0xFF00000000000000ULL))
            == uint64_t(0x00000000000000FFULL));
    REQUIRE(byte_swap_with_size<8>::convert(uint64_t(0xFFFFFFFFFFFFFFFFULL))
            == uint64_t(0xFFFFFFFFFFFFFFFFULL));
}

TEST_CASE("byte_swap_with_size roundtrip (convert twice is identity)", "[byte_swap]")
{
    const uint16_t v16 = 0xABCD;
    REQUIRE(byte_swap_with_size<2>::convert(byte_swap_with_size<2>::convert(v16)) == v16);

    const uint32_t v32 = 0xDEADBEEF;
    REQUIRE(byte_swap_with_size<4>::convert(byte_swap_with_size<4>::convert(v32)) == v32);

    const uint64_t v64 = 0xCAFEBABEDEADBEEFULL;
    REQUIRE(byte_swap_with_size<8>::convert(byte_swap_with_size<8>::convert(v64)) == v64);
}

// =========================================================================
// byte_swap<T>() — runtime helper (uses memcpy, works on any trivially-copyable)
// =========================================================================

TEST_CASE("byte_swap<uint8_t> is identity", "[byte_swap]")
{
    REQUIRE(byte_swap(uint8_t(0x00)) == uint8_t(0x00));
    REQUIRE(byte_swap(uint8_t(0xAB)) == uint8_t(0xAB));
    REQUIRE(byte_swap(uint8_t(0xFF)) == uint8_t(0xFF));
}

TEST_CASE("byte_swap<uint16_t> known values", "[byte_swap]")
{
    REQUIRE(byte_swap(uint16_t(0x1234)) == uint16_t(0x3412));
    REQUIRE(byte_swap(uint16_t(0x0100)) == uint16_t(0x0001));
}

TEST_CASE("byte_swap<uint32_t> known values", "[byte_swap]")
{
    REQUIRE(byte_swap(uint32_t(0x12345678)) == uint32_t(0x78563412));
    REQUIRE(byte_swap(uint32_t(0xAABBCCDD)) == uint32_t(0xDDCCBBAA));
}

TEST_CASE("byte_swap<uint64_t> known values", "[byte_swap]")
{
    REQUIRE(byte_swap(uint64_t(0x0102030405060708ULL)) == uint64_t(0x0807060504030201ULL));
}

TEST_CASE("byte_swap<int32_t> reinterprets bytes correctly", "[byte_swap]")
{
    // 0x01000000 as int32 -> bytes [01 00 00 00] -> swapped [00 00 00 01] -> int32 = 1
    REQUIRE(byte_swap(int32_t(0x01000000)) == int32_t(1));
}

TEST_CASE("byte_swap roundtrip is identity", "[byte_swap]")
{
    REQUIRE(byte_swap(byte_swap(uint16_t(0xABCD))) == uint16_t(0xABCD));
    REQUIRE(byte_swap(byte_swap(uint32_t(0xDEADBEEF))) == uint32_t(0xDEADBEEF));
    REQUIRE(byte_swap(byte_swap(uint64_t(0xCAFEBABEDEADBEEFULL))) == uint64_t(0xCAFEBABEDEADBEEFULL));
}

// =========================================================================
// word_swap — swaps 16-bit words within a 32/64-bit value
// =========================================================================

TEST_CASE("word_swap<uint8_t> is identity (specialization)", "[byte_swap][word_swap]")
{
    REQUIRE(word_swap(uint8_t(0xAB)) == uint8_t(0xAB));
}

TEST_CASE("word_swap<uint16_t> is identity (specialization)", "[byte_swap][word_swap]")
{
    REQUIRE(word_swap(uint16_t(0x1234)) == uint16_t(0x1234));
    REQUIRE(word_swap(uint16_t(0xABCD)) == uint16_t(0xABCD));
}

TEST_CASE("word_swap<int8_t> is identity (specialization)", "[byte_swap][word_swap]")
{
    REQUIRE(word_swap(int8_t(0x7F)) == int8_t(0x7F));
}

TEST_CASE("word_swap<int16_t> is identity (specialization)", "[byte_swap][word_swap]")
{
    REQUIRE(word_swap(int16_t(0x1234)) == int16_t(0x1234));
}

TEST_CASE("word_swap_by_size<4>::convert swaps two 16-bit halves", "[byte_swap][word_swap]")
{
    REQUIRE(word_swap_by_size<4>::convert(uint32_t(0x12345678)) == uint32_t(0x56781234));
    REQUIRE(word_swap_by_size<4>::convert(uint32_t(0x00010002)) == uint32_t(0x00020001));
    REQUIRE(word_swap_by_size<4>::convert(uint32_t(0x00000000)) == uint32_t(0x00000000));
    REQUIRE(word_swap_by_size<4>::convert(uint32_t(0xFFFFFFFF)) == uint32_t(0xFFFFFFFF));
}

TEST_CASE("word_swap<uint32_t> known values", "[byte_swap][word_swap]")
{
    REQUIRE(word_swap(uint32_t(0x12345678)) == uint32_t(0x56781234));
    REQUIRE(word_swap(uint32_t(0xAABBCCDD)) == uint32_t(0xCCDDAABB));
}

TEST_CASE("word_swap roundtrip is identity", "[byte_swap][word_swap]")
{
    REQUIRE(word_swap(word_swap(uint32_t(0xDEADBEEF))) == uint32_t(0xDEADBEEF));
}
