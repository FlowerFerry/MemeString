
#ifndef MEME_IMPL_CONC_STD_ATOMIC_FWD_H_INCLUDED
#define MEME_IMPL_CONC_STD_ATOMIC_FWD_H_INCLUDED

#include <meme/conc/atomic_fwd.h>
#include <mego/predef/symbol/static_assert.h>

#include <assert.h>
#include <stdatomic.h>

MEME_EXTERN_C_SCOPE_START

typedef struct mmconc_std_atmc_bl {
    mmconc_atomic_none_t base;
    uint8_t reserved[MMCONC_ATOMIC_BOOL_OBJ_SIZE - sizeof(mmconc_atomic_none_t) - sizeof(atomic_bool)];
    atomic_bool data;
} mmconc_std_atmc_bl_t;

MEGO__STATIC_ASSERT(
    sizeof(mmconc_std_atmc_bl_t) == sizeof(mmconc_atomic_bool_t),
    mmconc_std_atmc_bl_size_mismatch,
    "'atomic_bool' size mismatch");

typedef struct mmconc_std_atmc_i8 {
    mmconc_atomic_none_t base;
    uint8_t reserved[MMCONC_ATOMIC_INT8_OBJ_SIZE - sizeof(mmconc_atomic_none_t) - sizeof(atomic_schar)];
    atomic_schar data;
} mmconc_std_atmc_i8_t;

MEGO__STATIC_ASSERT(
    sizeof(mmconc_std_atmc_i8_t) == sizeof(mmconc_atomic_int8_t),
    mmconc_std_atmc_i8_size_mismatch,
    "'atomic_schar' size mismatch");

typedef struct mmconc_std_atmc_u8 {
    mmconc_atomic_none_t base;
    uint8_t reserved[MMCONC_ATOMIC_UINT8_OBJ_SIZE - sizeof(mmconc_atomic_none_t) - sizeof(atomic_uchar)];
    atomic_uchar data;
} mmconc_std_atmc_u8_t;

MEGO__STATIC_ASSERT(
    sizeof(mmconc_std_atmc_u8_t) == sizeof(mmconc_atomic_uint8_t),
    mmconc_std_atmc_u8_size_mismatch,
    "'atomic_uchar' size mismatch");

typedef struct mmconc_std_atmc_i16 {
    mmconc_atomic_none_t base;
    uint8_t reserved[MMCONC_ATOMIC_INT16_OBJ_SIZE - sizeof(mmconc_atomic_none_t) - sizeof(atomic_short)];
    atomic_short data;
} mmconc_std_atmc_i16_t;

MEGO__STATIC_ASSERT(
    sizeof(mmconc_std_atmc_i16_t) == sizeof(mmconc_atomic_int16_t),
    mmconc_std_atmc_i16_size_mismatch,
    "'atomic_short' size mismatch");

typedef struct mmconc_std_atmc_u16 {
    mmconc_atomic_none_t base;
    uint8_t reserved[MMCONC_ATOMIC_UINT16_OBJ_SIZE - sizeof(mmconc_atomic_none_t) - sizeof(atomic_ushort)];
    atomic_ushort data;
} mmconc_std_atmc_u16_t;

MEGO__STATIC_ASSERT(
    sizeof(mmconc_std_atmc_u16_t) == sizeof(mmconc_atomic_uint16_t),
    mmconc_std_atmc_u16_size_mismatch,
    "'atomic_ushort' size mismatch");

typedef struct mmconc_std_atmc_i32 {
    mmconc_atomic_none_t base;
    uint8_t reserved[MMCONC_ATOMIC_INT32_OBJ_SIZE - sizeof(mmconc_atomic_none_t) - sizeof(atomic_int)];
    atomic_int data;
} mmconc_std_atmc_i32_t;

MEGO__STATIC_ASSERT(
    sizeof(mmconc_std_atmc_i32_t) == sizeof(mmconc_atomic_int32_t),
    mmconc_std_atmc_i32_size_mismatch,
    "'atomic_int' size mismatch");

typedef struct mmconc_std_atmc_u32 {
    mmconc_atomic_none_t base;
    uint8_t reserved[MMCONC_ATOMIC_UINT32_OBJ_SIZE - sizeof(mmconc_atomic_none_t) - sizeof(atomic_uint)];
    atomic_uint data;
} mmconc_std_atmc_u32_t;

MEGO__STATIC_ASSERT(
    sizeof(mmconc_std_atmc_u32_t) == sizeof(mmconc_atomic_uint32_t),
    mmconc_std_atmc_u32_size_mismatch,
    "'atomic_uint' size mismatch");


typedef struct mmconc_std_atmc_i64 {
    mmconc_atomic_none_t base;
    uint8_t reserved[MMCONC_ATOMIC_INT64_OBJ_SIZE - sizeof(mmconc_atomic_none_t) - sizeof(atomic_llong)];
    atomic_llong data;
} mmconc_std_atmc_i64_t;

typedef struct mmconc_std_atmc_u64 {
    mmconc_atomic_none_t base;
    uint8_t reserved[MMCONC_ATOMIC_UINT64_OBJ_SIZE - sizeof(mmconc_atomic_none_t) - sizeof(atomic_ullong)];
    atomic_ullong data;
} mmconc_std_atmc_u64_t;

MEGO__STATIC_ASSERT(
    sizeof(mmconc_std_atmc_i64_t) == sizeof(mmconc_atomic_int64_t),
    mmconc_std_atmc_i64_size_mismatch,
    "'atomic_llong' size mismatch");

    MEGO__STATIC_ASSERT(
    sizeof(mmconc_std_atmc_u64_t) == sizeof(mmconc_atomic_uint64_t),
    mmconc_std_atmc_u64_size_mismatch,
    "'atomic_ullong' size mismatch");

typedef struct mmconc_std_atmc_flag {
    mmconc_atomic_none_t base;
    uint8_t reserved[MMCONC_ATOMIC_BOOL_OBJ_SIZE - sizeof(mmconc_atomic_none_t) - sizeof(atomic_flag)];
    atomic_flag data;
} mmconc_std_atmc_flag_t;

MEGO__STATIC_ASSERT(
    sizeof(mmconc_std_atmc_flag_t) == sizeof(mmconc_atomic_flag_t),
    mmconc_std_atmc_flag_size_mismatch,
    "'atomic_flag' size mismatch");

MEME_EXTERN_C_SCOPE_ENDED

#endif // !MEME_IMPL_CONC_STD_ATOMIC_FWD_H_INCLUDED
