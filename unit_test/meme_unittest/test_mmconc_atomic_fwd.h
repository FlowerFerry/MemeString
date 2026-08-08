#ifndef TEST_MEME_CONC_ATOMIC_FWD_H_INCLUDED
#define TEST_MEME_CONC_ATOMIC_FWD_H_INCLUDED

#include <minunit.h>
#include <stddef.h>
#include <meme/conc/atomic_fwd.h>

MU_TEST(test_mmconc_atomic_size_and_align)
{
    /* Size assertions — must match internal representation */
    mu_assert_int_eq(sizeof(mmconc_atomic_bool_t),  4);
    mu_assert_int_eq(sizeof(mmconc_atomic_flag_t),  4);
    mu_assert_int_eq(sizeof(mmconc_atomic_int8_t),  4);
    mu_assert_int_eq(sizeof(mmconc_atomic_uint8_t), 4);
    mu_assert_int_eq(sizeof(mmconc_atomic_int16_t), 6);
    mu_assert_int_eq(sizeof(mmconc_atomic_uint16_t),6);
    mu_assert_int_eq(sizeof(mmconc_atomic_int32_t), 8);
    mu_assert_int_eq(sizeof(mmconc_atomic_uint32_t),8);
    mu_assert_int_eq(sizeof(mmconc_atomic_int64_t), 16);
    mu_assert_int_eq(sizeof(mmconc_atomic_uint64_t),16);

    /* Alignment assertions — critical for ARMv7 atomic ldrex/strex */
    mu_assert_int_eq(_Alignof(mmconc_atomic_bool_t),  sizeof(int));
    mu_assert_int_eq(_Alignof(mmconc_atomic_flag_t),  sizeof(int));
    mu_assert_int_eq(_Alignof(mmconc_atomic_int16_t), sizeof(int16_t));
    mu_assert_int_eq(_Alignof(mmconc_atomic_uint16_t),sizeof(uint16_t));
    mu_assert_int_eq(_Alignof(mmconc_atomic_int32_t), sizeof(int32_t));
    mu_assert_int_eq(_Alignof(mmconc_atomic_uint32_t),sizeof(uint32_t));
    mu_assert_int_eq(_Alignof(mmconc_atomic_int64_t), sizeof(int64_t));
    mu_assert_int_eq(_Alignof(mmconc_atomic_uint64_t),sizeof(uint64_t));

    /* int8/uint8 alignment is always 1 — no assertion needed */
    mu_assert_int_eq(_Alignof(mmconc_atomic_int8_t),  1);
    mu_assert_int_eq(_Alignof(mmconc_atomic_uint8_t), 1);
}

MU_TEST_SUITE(test_mmconc_atomic_fwd)
{
    MU_RUN_TEST(test_mmconc_atomic_size_and_align);
}

#endif // TEST_MEME_CONC_ATOMIC_FWD_H_INCLUDED
