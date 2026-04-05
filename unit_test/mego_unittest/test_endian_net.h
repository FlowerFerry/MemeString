#ifndef MEGO_TEST_ENDIAN_NET_H_INCLUDED
#define MEGO_TEST_ENDIAN_NET_H_INCLUDED

#include <mego/util/endian/net.h>
#include <minunit.h>

/* ---- hton / ntoh 16 ---- */

MU_TEST(ut_net16_roundtrip)
{
    uint16_t v = 0x1234;
    mu_assert(MegoEndian_ntoh16(MegoEndian_hton16(v)) == v,
              "hton16->ntoh16 roundtrip failed");
}

MU_TEST(ut_net16_symmetry)
{
    /* hton and ntoh are identical operations (both swap on LE, both pass on BE) */
    uint16_t v = 0xABCD;
    mu_assert(MegoEndian_hton16(v) == MegoEndian_ntoh16(v),
              "hton16 != ntoh16 (should be symmetric)");
}

MU_TEST(ut_net16_boundary)
{
    mu_assert(MegoEndian_ntoh16(MegoEndian_hton16(0x0000)) == 0x0000,
              "net16 zero roundtrip failed");
    mu_assert(MegoEndian_ntoh16(MegoEndian_hton16(0xFFFF)) == 0xFFFF,
              "net16 max roundtrip failed");
}

/* ---- hton / ntoh 32 ---- */

MU_TEST(ut_net32_roundtrip)
{
    uint32_t v = 0x12345678u;
    mu_assert(MegoEndian_ntoh32(MegoEndian_hton32(v)) == v,
              "hton32->ntoh32 roundtrip failed");
}

MU_TEST(ut_net32_symmetry)
{
    uint32_t v = 0xDEADBEEFu;
    mu_assert(MegoEndian_hton32(v) == MegoEndian_ntoh32(v),
              "hton32 != ntoh32 (should be symmetric)");
}

MU_TEST(ut_net32_boundary)
{
    mu_assert(MegoEndian_ntoh32(MegoEndian_hton32(0x00000000u)) == 0x00000000u,
              "net32 zero roundtrip failed");
    mu_assert(MegoEndian_ntoh32(MegoEndian_hton32(0xFFFFFFFFu)) == 0xFFFFFFFFu,
              "net32 max roundtrip failed");
    mu_assert(MegoEndian_ntoh32(MegoEndian_hton32(0x00000001u)) == 0x00000001u,
              "net32 one roundtrip failed");
}

/* ---- hton / ntoh 64 ---- */

MU_TEST(ut_net64_roundtrip)
{
    uint64_t v = 0x0102030405060708ULL;
    mu_assert(MegoEndian_ntoh64(MegoEndian_hton64(v)) == v,
              "hton64->ntoh64 roundtrip failed");
}

MU_TEST(ut_net64_symmetry)
{
    uint64_t v = 0xCAFEBABEDEADBEEFULL;
    mu_assert(MegoEndian_hton64(v) == MegoEndian_ntoh64(v),
              "hton64 != ntoh64 (should be symmetric)");
}

MU_TEST(ut_net64_boundary)
{
    mu_assert(MegoEndian_ntoh64(MegoEndian_hton64(0x0000000000000000ULL)) == 0x0000000000000000ULL,
              "net64 zero roundtrip failed");
    mu_assert(MegoEndian_ntoh64(MegoEndian_hton64(0xFFFFFFFFFFFFFFFFULL)) == 0xFFFFFFFFFFFFFFFFULL,
              "net64 max roundtrip failed");
}

/* ---- suite ---- */

MU_TEST_SUITE(test_endian_net)
{
    MU_RUN_TEST(ut_net16_roundtrip);
    MU_RUN_TEST(ut_net16_symmetry);
    MU_RUN_TEST(ut_net16_boundary);
    MU_RUN_TEST(ut_net32_roundtrip);
    MU_RUN_TEST(ut_net32_symmetry);
    MU_RUN_TEST(ut_net32_boundary);
    MU_RUN_TEST(ut_net64_roundtrip);
    MU_RUN_TEST(ut_net64_symmetry);
    MU_RUN_TEST(ut_net64_boundary);
}

#endif /* MEGO_TEST_ENDIAN_NET_H_INCLUDED */
