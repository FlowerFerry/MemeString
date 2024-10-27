
#ifndef MEME_IMPL_CONC_NATIVE_ATOMIC_FWD_H_INCLUDED
#define MEME_IMPL_CONC_NATIVE_ATOMIC_FWD_H_INCLUDED

#include <meme/conc/atomic_fwd.h>

#include <stdint.h>
#include <stdbool.h>

MEME_EXTERN_C_SCOPE_START

typedef struct mmconc_native_atmc_bl {
    mmconc_atomic_none_t base;
    uint8_t reserved[MMCONC_ATOMIC_BOOL_OBJ_SIZE - sizeof(mmconc_atomic_none_t) - sizeof(bool)];
    bool data;
} mmconc_native_atmc_bl_t;

static_assert(
    sizeof(mmconc_native_atmc_bl_t) == sizeof(mmconc_atomic_bool_t),
    "Native 'bool' size mismatch");

typedef struct mmconc_native_atmc_i8 {
    mmconc_atomic_none_t base;
    uint8_t reserved[MMCONC_ATOMIC_INT8_OBJ_SIZE - sizeof(mmconc_atomic_none_t) - sizeof(int8_t)];
    int8_t data;
} mmconc_native_atmc_i8_t;

static_assert(
    sizeof(mmconc_native_atmc_i8_t) == sizeof(mmconc_atomic_int8_t),
    "Native 'int8_t' size mismatch");

typedef struct mmconc_native_atmc_u8 {
    mmconc_atomic_none_t base;
    uint8_t reserved[MMCONC_ATOMIC_UINT8_OBJ_SIZE - sizeof(mmconc_atomic_none_t) - sizeof(uint8_t)];
    uint8_t data;
} mmconc_native_atmc_u8_t;

static_assert(
    sizeof(mmconc_native_atmc_u8_t) == sizeof(mmconc_atomic_uint8_t),
    "Native 'uint8_t' size mismatch");

typedef struct mmconc_native_atmc_i16 {
    mmconc_atomic_none_t base;
    uint8_t reserved[MMCONC_ATOMIC_INT16_OBJ_SIZE - sizeof(mmconc_atomic_none_t) - sizeof(int16_t)];
    int16_t data;
} mmconc_native_atmc_i16_t;

static_assert(
    sizeof(mmconc_native_atmc_i16_t) == sizeof(mmconc_atomic_int16_t),
    "Native 'int16_t' size mismatch");

typedef struct mmconc_native_atmc_u16 {
    mmconc_atomic_none_t base;
    uint8_t reserved[MMCONC_ATOMIC_UINT16_OBJ_SIZE - sizeof(mmconc_atomic_none_t) - sizeof(uint16_t)];
    uint16_t data;
} mmconc_native_atmc_u16_t;

static_assert(
    sizeof(mmconc_native_atmc_u16_t) == sizeof(mmconc_atomic_uint16_t),
    "Native 'uin16_t' size mismatch");

typedef struct mmconc_native_atmc_i32 {
    mmconc_atomic_none_t base;
    uint8_t reserved[MMCONC_ATOMIC_INT32_OBJ_SIZE - sizeof(mmconc_atomic_none_t) - sizeof(int32_t)];
    int32_t data;
} mmconc_native_atmc_i32_t;

static_assert(
    sizeof(mmconc_native_atmc_i32_t) == sizeof(mmconc_atomic_int32_t),
    "Native 'int32_t' size mismatch");

typedef struct mmconc_native_atmc_u32 {
    mmconc_atomic_none_t base;
    uint8_t reserved[MMCONC_ATOMIC_UINT32_OBJ_SIZE - sizeof(mmconc_atomic_none_t) - sizeof(uint32_t)];
    uint32_t data;
} mmconc_native_atmc_u32_t;

static_assert(
    sizeof(mmconc_native_atmc_u32_t) == sizeof(mmconc_atomic_uint32_t),
    "Native 'uint32_t' size mismatch");

typedef struct mmconc_native_atmc_i64 {
    mmconc_atomic_none_t base;
    uint8_t reserved[MMCONC_ATOMIC_INT64_OBJ_SIZE - sizeof(mmconc_atomic_none_t) - sizeof(int64_t)];
    int64_t data;
} mmconc_native_atmc_i64_t;

typedef struct mmconc_native_atmc_u64 {
    mmconc_atomic_none_t base;
    uint8_t reserved[MMCONC_ATOMIC_UINT64_OBJ_SIZE - sizeof(mmconc_atomic_none_t) - sizeof(uint64_t)];
    uint64_t data;
} mmconc_native_atmc_u64_t;

static_assert(
    sizeof(mmconc_native_atmc_i64_t) == sizeof(mmconc_atomic_int64_t),
    "Native 'int64_t' size mismatch");

static_assert(
    sizeof(mmconc_native_atmc_u64_t) == sizeof(mmconc_atomic_uint64_t),
    "Native 'uint64_t' size mismatch");


MEME_EXTERN_C_SCOPE_ENDED

#endif // !MEME_IMPL_CONC_NATIVE_ATOMIC_FWD_H_INCLUDED
