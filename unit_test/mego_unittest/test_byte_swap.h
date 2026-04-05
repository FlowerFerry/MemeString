#ifndef MEGO_TEST_BYTE_SWAP_H_INCLUDED
#define MEGO_TEST_BYTE_SWAP_H_INCLUDED

#include <mego/util/endian/byte_swap.h>
#include <minunit.h>

/* ---- byteSwap16 ---- */

MU_TEST(ut_byteswap16_known)
{
    mu_assert(MegoEndian_byteSwap16(0x1234) == 0x3412, "byteSwap16(0x1234) != 0x3412");
    mu_assert(MegoEndian_byteSwap16(0x0001) == 0x0100, "byteSwap16(0x0001) != 0x0100");
    mu_assert(MegoEndian_byteSwap16(0xFF00) == 0x00FF, "byteSwap16(0xFF00) != 0x00FF");
    mu_assert(MegoEndian_byteSwap16(0x0000) == 0x0000, "byteSwap16(0) != 0");
    mu_assert(MegoEndian_byteSwap16(0xFFFF) == 0xFFFF, "byteSwap16(0xFFFF) != 0xFFFF");
}

MU_TEST(ut_byteswap16_roundtrip)
{
    uint16_t v = 0xABCD;
    mu_assert(MegoEndian_byteSwap16(MegoEndian_byteSwap16(v)) == v,
              "byteSwap16 roundtrip failed");
}

/* ---- byteSwap32 ---- */

MU_TEST(ut_byteswap32_known)
{
    mu_assert(MegoEndian_byteSwap32(0x12345678u) == 0x78563412u,
              "byteSwap32(0x12345678) != 0x78563412");
    mu_assert(MegoEndian_byteSwap32(0x00000001u) == 0x01000000u,
              "byteSwap32(0x00000001) != 0x01000000");
    mu_assert(MegoEndian_byteSwap32(0xFF000000u) == 0x000000FFu,
              "byteSwap32(0xFF000000) != 0x000000FF");
    mu_assert(MegoEndian_byteSwap32(0x00000000u) == 0x00000000u,
              "byteSwap32(0) != 0");
    mu_assert(MegoEndian_byteSwap32(0xFFFFFFFFu) == 0xFFFFFFFFu,
              "byteSwap32(0xFFFFFFFF) != 0xFFFFFFFF");
}

MU_TEST(ut_byteswap32_roundtrip)
{
    uint32_t v = 0xDEADBEEFu;
    mu_assert(MegoEndian_byteSwap32(MegoEndian_byteSwap32(v)) == v,
              "byteSwap32 roundtrip failed");
}

/* ---- byteSwap64 ---- */

MU_TEST(ut_byteswap64_known)
{
    mu_assert(
        MegoEndian_byteSwap64(0x0102030405060708ULL) == 0x0807060504030201ULL,
        "byteSwap64(0x0102030405060708) != 0x0807060504030201");
    mu_assert(MegoEndian_byteSwap64(0x0000000000000000ULL) == 0x0000000000000000ULL,
              "byteSwap64(0) != 0");
    mu_assert(MegoEndian_byteSwap64(0xFFFFFFFFFFFFFFFFULL) == 0xFFFFFFFFFFFFFFFFULL,
              "byteSwap64(max) != max");
    mu_assert(MegoEndian_byteSwap64(0xFF00000000000000ULL) == 0x00000000000000FFULL,
              "byteSwap64 high byte failed");
}

MU_TEST(ut_byteswap64_roundtrip)
{
    uint64_t v = 0xCAFEBABEDEADBEEFULL;
    mu_assert(MegoEndian_byteSwap64(MegoEndian_byteSwap64(v)) == v,
              "byteSwap64 roundtrip failed");
}

/* ---- wordSwap32 ---- */

MU_TEST(ut_wordswap32_known)
{
    mu_assert(MegoEndian_wordSwap32(0x12345678u) == 0x56781234u,
              "wordSwap32(0x12345678) != 0x56781234");
    mu_assert(MegoEndian_wordSwap32(0x00010000u) == 0x00000001u,
              "wordSwap32(0x00010000) != 0x00000001");
    mu_assert(MegoEndian_wordSwap32(0x00000000u) == 0x00000000u,
              "wordSwap32(0) != 0");
    mu_assert(MegoEndian_wordSwap32(0xFFFFFFFFu) == 0xFFFFFFFFu,
              "wordSwap32(max) != max");
}

MU_TEST(ut_wordswap32_roundtrip)
{
    uint32_t v = 0xABCD1234u;
    mu_assert(MegoEndian_wordSwap32(MegoEndian_wordSwap32(v)) == v,
              "wordSwap32 roundtrip failed");
}

/* ---- wordSwap64 ---- */

MU_TEST(ut_wordswap64_known)
{
    mu_assert(
        MegoEndian_wordSwap64(0x0001000200030004ULL) == 0x0004000300020001ULL,
        "wordSwap64 known value failed");
    mu_assert(MegoEndian_wordSwap64(0x0000000000000000ULL) == 0x0000000000000000ULL,
              "wordSwap64(0) != 0");
    mu_assert(MegoEndian_wordSwap64(0xFFFFFFFFFFFFFFFFULL) == 0xFFFFFFFFFFFFFFFFULL,
              "wordSwap64(max) != max");
}

MU_TEST(ut_wordswap64_roundtrip)
{
    uint64_t v = 0x1111222233334444ULL;
    mu_assert(MegoEndian_wordSwap64(MegoEndian_wordSwap64(v)) == v,
              "wordSwap64 roundtrip failed");
}

/* ---- suite ---- */

MU_TEST_SUITE(test_byte_swap)
{
    MU_RUN_TEST(ut_byteswap16_known);
    MU_RUN_TEST(ut_byteswap16_roundtrip);
    MU_RUN_TEST(ut_byteswap32_known);
    MU_RUN_TEST(ut_byteswap32_roundtrip);
    MU_RUN_TEST(ut_byteswap64_known);
    MU_RUN_TEST(ut_byteswap64_roundtrip);
    MU_RUN_TEST(ut_wordswap32_known);
    MU_RUN_TEST(ut_wordswap32_roundtrip);
    MU_RUN_TEST(ut_wordswap64_known);
    MU_RUN_TEST(ut_wordswap64_roundtrip);
}

#endif /* MEGO_TEST_BYTE_SWAP_H_INCLUDED */
