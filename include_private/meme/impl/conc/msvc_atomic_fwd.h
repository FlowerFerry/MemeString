
#ifndef MEME_IMPL_CONC_MSVC_ATOMIC_FWD_H_INCLUDED
#define MEME_IMPL_CONC_MSVC_ATOMIC_FWD_H_INCLUDED

#include <meme/conc/atomic_fwd.h>
#include <mego/predef/symbol/static_assert.h>

#include <stdint.h>
#include <stdbool.h>

MEME_EXTERN_C_SCOPE_START

typedef bool        mmconc_msvc_atmc_bl_t;
typedef int8_t      mmconc_msvc_atmc_i8_t;
typedef uint8_t     mmconc_msvc_atmc_u8_t;
typedef int16_t     mmconc_msvc_atmc_i16_t;
typedef uint16_t    mmconc_msvc_atmc_u16_t;
typedef int32_t     mmconc_msvc_atmc_i32_t;
typedef uint32_t    mmconc_msvc_atmc_u32_t;
typedef int64_t     mmconc_msvc_atmc_i64_t;
typedef uint64_t    mmconc_msvc_atmc_u64_t;

MEGO__STATIC_ASSERT(
    sizeof(bool) == sizeof(int8_t), mmconc_bool_size_mismatch, "mmconc 'bool' size mismatch");

MEME_EXTERN_C_SCOPE_ENDED

#endif // !MEME_IMPL_CONC_MSVC_ATOMIC_FWD_H_INCLUDED
