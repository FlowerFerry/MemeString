
#ifndef MEME_SIMD_SIMD_H_INCLUDED
#define MEME_SIMD_SIMD_H_INCLUDED

#include <meme/string_fwd.h>
#include <mego/err/ec.h>

MEME_EXTERN_C_SCOPE_START

MEME_API void
    MEME_STDCALL mmsimd_i8_add(
        const int8_t* _a, const int8_t* _b, int8_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i8_add_scalar(
        const int8_t* _a, int8_t _b, int8_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u8_add(
        const uint8_t* _a, const uint8_t* _b, uint8_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u8_add_scalar(
        const uint8_t* _a, uint8_t _b, uint8_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i8_sub(
        const int8_t* _a, const int8_t* _b, int8_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i8_sub_scalar(
        const int8_t* _a, int8_t _b, int8_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u8_sub(
        const uint8_t* _a, const uint8_t* _b, uint8_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u8_sub_scalar(
        const uint8_t* _a, uint8_t _b, uint8_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i8_mul(
        const int8_t* _a, const int8_t* _b, int8_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i8_mul_scalar(
        const int8_t* _a, int8_t _b, int8_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u8_mul(
        const uint8_t* _a, const uint8_t* _b, uint8_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u8_mul_scalar(
        const uint8_t* _a, uint8_t _b, uint8_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i8_div(
        const int8_t* _a, const int8_t* _b, int8_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i8_div_scalar(
        const int8_t* _a, int8_t _b, int8_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u8_div(
        const uint8_t* _a, const uint8_t* _b, uint8_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u8_div_scalar(
        const uint8_t* _a, uint8_t _b, uint8_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i16_add(
        const int16_t* _a, const int16_t* _b, int16_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i16_add_scalar(
        const int16_t* _a, int16_t _b, int16_t* _out, mmint_t _len);
    
MEME_API void
    MEME_STDCALL mmsimd_u16_add(
        const uint16_t* _a, const uint16_t* _b, uint16_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u16_add_scalar(
        const uint16_t* _a, uint16_t _b, uint16_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i16_sub(
        const int16_t* _a, const int16_t* _b, int16_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i16_sub_scalar(
        const int16_t* _a, int16_t _b, int16_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u16_sub(
        const uint16_t* _a, const uint16_t* _b, uint16_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u16_sub_scalar(
        const uint16_t* _a, uint16_t _b, uint16_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i16_mul(
        const int16_t* _a, const int16_t* _b, int16_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i16_mul_scalar(
        const int16_t* _a, int16_t _b, int16_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u16_mul(
        const uint16_t* _a, const uint16_t* _b, uint16_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u16_mul_scalar(
        const uint16_t* _a, uint16_t _b, uint16_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i16_div(
        const int16_t* _a, const int16_t* _b, int16_t* _out, mmint_t _len);
    
MEME_API void  
    MEME_STDCALL mmsimd_i16_div_scalar(
        const int16_t* _a, int16_t _b, int16_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u16_div(
        const uint16_t* _a, const uint16_t* _b, uint16_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u16_div_scalar(
        const uint16_t* _a, uint16_t _b, uint16_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i32_add(
        const int32_t* _a, const int32_t* _b, int32_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i32_add_scalar(
        const int32_t* _a, int32_t _b, int32_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u32_add(
        const uint32_t* _a, const uint32_t* _b, uint32_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u32_add_scalar(
        const uint32_t* _a, uint32_t _b, uint32_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i32_sub(
        const int32_t* _a, const int32_t* _b, int32_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i32_sub_scalar(
        const int32_t* _a, int32_t _b, int32_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u32_sub(
        const uint32_t* _a, const uint32_t* _b, uint32_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u32_sub_scalar(
        const uint32_t* _a, uint32_t _b, uint32_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i32_mul(
        const int32_t* _a, const int32_t* _b, int32_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i32_mul_scalar(
        const int32_t* _a, int32_t _b, int32_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u32_mul(
        const uint32_t* _a, const uint32_t* _b, uint32_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u32_mul_scalar(
        const uint32_t* _a, uint32_t _b, uint32_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i32_div(
        const int32_t* _a, const int32_t* _b, int32_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i32_div_scalar(
        const int32_t* _a, int32_t _b, int32_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u32_div(
        const uint32_t* _a, const uint32_t* _b, uint32_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u32_div_scalar(
        const uint32_t* _a, uint32_t _b, uint32_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i64_add(
        const int64_t* _a, const int64_t* _b, int64_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i64_add_scalar(
        const int64_t* _a, int64_t _b, int64_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u64_add(
        const uint64_t* _a, const uint64_t* _b, uint64_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u64_add_scalar(
        const uint64_t* _a, uint64_t _b, uint64_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i64_sub(
        const int64_t* _a, const int64_t* _b, int64_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i64_sub_scalar(
        const int64_t* _a, int64_t _b, int64_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u64_sub(
        const uint64_t* _a, const uint64_t* _b, uint64_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u64_sub_scalar(
        const uint64_t* _a, uint64_t _b, uint64_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i64_mul(
        const int64_t* _a, const int64_t* _b, int64_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i64_mul_scalar(
        const int64_t* _a, int64_t _b, int64_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u64_mul(
        const uint64_t* _a, const uint64_t* _b, uint64_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u64_mul_scalar(
        const uint64_t* _a, uint64_t _b, uint64_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i64_div(
        const int64_t* _a, const int64_t* _b, int64_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_i64_div_scalar(
        const int64_t* _a, int64_t _b, int64_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u64_div(
        const uint64_t* _a, const uint64_t* _b, uint64_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_u64_div_scalar(
        const uint64_t* _a, uint64_t _b, uint64_t* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_f32_add(
        const float* _a, const float* _b, float* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_f32_add_scalar(
        const float* _a, float _b, float* _out, mmint_t _len);  

MEME_API void
    MEME_STDCALL mmsimd_f32_sub(
        const float* _a, const float* _b, float* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_f32_sub_scalar(
        const float* _a, float _b, float* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_f32_mul(
        const float* _a, const float* _b, float* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_f32_mul_scalar(
        const float* _a, float _b, float* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_f32_div(
        const float* _a, const float* _b, float* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_f32_div_scalar(
        const float* _a, float _b, float* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_f64_add(
        const double* _a, const double* _b, double* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_f64_add_scalar(
        const double* _a, double _b, double* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_f64_sub(
        const double* _a, const double* _b, double* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_f64_sub_scalar(
        const double* _a, double _b, double* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_f64_mul(
        const double* _a, const double* _b, double* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_f64_mul_scalar(
        const double* _a, double _b, double* _out, mmint_t _len);   

MEME_API void
    MEME_STDCALL mmsimd_f64_div(
        const double* _a, const double* _b, double* _out, mmint_t _len);

MEME_API void
    MEME_STDCALL mmsimd_f64_div_scalar(
        const double* _a, double _b, double* _out, mmint_t _len);

#if INTPTR_MAX == INT64_MAX
#define mmsimd_iptr_add             mmsimd_i64_add
#define mmsimd_iptr_add_scalar      mmsimd_i64_add_scalar
#define mmsimd_iptr_sub             mmsimd_i64_sub
#define mmsimd_iptr_sub_scalar      mmsimd_i64_sub_scalar
#define mmsimd_iptr_mul             mmsimd_i64_mul
#define mmsimd_iptr_mul_scalar      mmsimd_i64_mul_scalar
#define mmsimd_iptr_div             mmsimd_i64_div
#define mmsimd_iptr_div_scalar      mmsimd_i64_div_scalar
#define mmsimd_uptr_add             mmsimd_u64_add
#define mmsimd_uptr_add_scalar      mmsimd_u64_add_scalar
#define mmsimd_uptr_sub             mmsimd_u64_sub
#define mmsimd_uptr_sub_scalar      mmsimd_u64_sub_scalar
#define mmsimd_uptr_mul             mmsimd_u64_mul
#define mmsimd_uptr_mul_scalar      mmsimd_u64_mul_scalar
#define mmsimd_uptr_div             mmsimd_u64_div
#define mmsimd_uptr_div_scalar      mmsimd_u64_div_scalar
#define mmsimd_size_add             mmsimd_u64_add
#define mmsimd_size_add_scalar      mmsimd_u64_add_scalar
#define mmsimd_size_sub             mmsimd_u64_sub
#define mmsimd_size_sub_scalar      mmsimd_u64_sub_scalar
#define mmsimd_size_mul             mmsimd_u64_mul
#define mmsimd_size_mul_scalar      mmsimd_u64_mul_scalar
#define mmsimd_size_div             mmsimd_u64_div
#define mmsimd_size_div_scalar      mmsimd_u64_div_scalar

#endif

#if INTPTR_MAX == INT32_MAX
#define mmsimd_iptr_add             mmsimd_i32_add
#define mmsimd_iptr_add_scalar      mmsimd_i32_add_scalar
#define mmsimd_iptr_sub             mmsimd_i32_sub
#define mmsimd_iptr_sub_scalar      mmsimd_i32_sub_scalar
#define mmsimd_iptr_mul             mmsimd_i32_mul
#define mmsimd_iptr_mul_scalar      mmsimd_i32_mul_scalar
#define mmsimd_iptr_div             mmsimd_i32_div
#define mmsimd_iptr_div_scalar      mmsimd_i32_div_scalar
#define mmsimd_uptr_add             mmsimd_u32_add
#define mmsimd_uptr_add_scalar      mmsimd_u32_add_scalar
#define mmsimd_uptr_sub             mmsimd_u32_sub
#define mmsimd_uptr_sub_scalar      mmsimd_u32_sub_scalar
#define mmsimd_uptr_mul             mmsimd_u32_mul
#define mmsimd_uptr_mul_scalar      mmsimd_u32_mul_scalar
#define mmsimd_uptr_div             mmsimd_u32_div
#define mmsimd_uptr_div_scalar      mmsimd_u32_div_scalar
#define mmsimd_size_add             mmsimd_u32_add
#define mmsimd_size_add_scalar      mmsimd_u32_add_scalar
#define mmsimd_size_sub             mmsimd_u32_sub
#define mmsimd_size_sub_scalar      mmsimd_u32_sub_scalar
#define mmsimd_size_mul             mmsimd_u32_mul
#define mmsimd_size_mul_scalar      mmsimd_u32_mul_scalar
#define mmsimd_size_div             mmsimd_u32_div
#define mmsimd_size_div_scalar      mmsimd_u32_div_scalar

#endif

MEME_API void MEME_STDCALL mmsimd_i8_to_i16(const int8_t* _in, int16_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i8_to_u16(const int8_t* _in, uint16_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i8_to_i32(const int8_t* _in, int32_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i8_to_u32(const int8_t* _in, uint32_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i8_to_i64(const int8_t* _in, int64_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i8_to_u64(const int8_t* _in, uint64_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i8_to_f32(const int8_t* _in, float* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i8_to_f64(const int8_t* _in, double* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u8_to_i16(const uint8_t* _in, int16_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u8_to_u16(const uint8_t* _in, uint16_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u8_to_i32(const uint8_t* _in, int32_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u8_to_u32(const uint8_t* _in, uint32_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u8_to_i64(const uint8_t* _in, int64_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u8_to_u64(const uint8_t* _in, uint64_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u8_to_f32(const uint8_t* _in, float* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u8_to_f64(const uint8_t* _in, double* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i16_to_i8(const int16_t* _in, int8_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i16_to_u8(const int16_t* _in, uint8_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i16_to_i32(const int16_t* _in, int32_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i16_to_u32(const int16_t* _in, uint32_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i16_to_i64(const int16_t* _in, int64_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i16_to_u64(const int16_t* _in, uint64_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i16_to_f32(const int16_t* _in, float* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i16_to_f64(const int16_t* _in, double* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u16_to_i8(const uint16_t* _in, int8_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u16_to_u8(const uint16_t* _in, uint8_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u16_to_i32(const uint16_t* _in, int32_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u16_to_u32(const uint16_t* _in, uint32_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u16_to_i64(const uint16_t* _in, int64_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u16_to_u64(const uint16_t* _in, uint64_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u16_to_f32(const uint16_t* _in, float* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u16_to_f64(const uint16_t* _in, double* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i32_to_i8(const int32_t* _in, int8_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i32_to_u8(const int32_t* _in, uint8_t* _out, mmint_t _len);   

MEME_API void MEME_STDCALL mmsimd_i32_to_i16(const int32_t* _in, int16_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i32_to_u16(const int32_t* _in, uint16_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i32_to_i64(const int32_t* _in, int64_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i32_to_u64(const int32_t* _in, uint64_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i32_to_f32(const int32_t* _in, float* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i32_to_f64(const int32_t* _in, double* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u32_to_i8(const uint32_t* _in, int8_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u32_to_u8(const uint32_t* _in, uint8_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u32_to_i16(const uint32_t* _in, int16_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u32_to_u16(const uint32_t* _in, uint16_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u32_to_i64(const uint32_t* _in, int64_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u32_to_u64(const uint32_t* _in, uint64_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u32_to_f32(const uint32_t* _in, float* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u32_to_f64(const uint32_t* _in, double* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i64_to_i8(const int64_t* _in, int8_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i64_to_u8(const int64_t* _in, uint8_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i64_to_i16(const int64_t* _in, int16_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i64_to_u16(const int64_t* _in, uint16_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i64_to_i32(const int64_t* _in, int32_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i64_to_u32(const int64_t* _in, uint32_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i64_to_f32(const int64_t* _in, float* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_i64_to_f64(const int64_t* _in, double* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u64_to_i8(const uint64_t* _in, int8_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u64_to_u8(const uint64_t* _in, uint8_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u64_to_i16(const uint64_t* _in, int16_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u64_to_u16(const uint64_t* _in, uint16_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u64_to_i32(const uint64_t* _in, int32_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u64_to_u32(const uint64_t* _in, uint32_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u64_to_f32(const uint64_t* _in, float* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_u64_to_f64(const uint64_t* _in, double* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_f32_to_i8(const float* _in, int8_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_f32_to_u8(const float* _in, uint8_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_f32_to_i16(const float* _in, int16_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_f32_to_u16(const float* _in, uint16_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_f32_to_i32(const float* _in, int32_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_f32_to_u32(const float* _in, uint32_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_f32_to_i64(const float* _in, int64_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_f32_to_u64(const float* _in, uint64_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_f32_to_f64(const float* _in, double* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_f64_to_i8(const double* _in, int8_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_f64_to_u8(const double* _in, uint8_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_f64_to_i16(const double* _in, int16_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_f64_to_u16(const double* _in, uint16_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_f64_to_i32(const double* _in, int32_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_f64_to_u32(const double* _in, uint32_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_f64_to_i64(const double* _in, int64_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_f64_to_u64(const double* _in, uint64_t* _out, mmint_t _len);

MEME_API void MEME_STDCALL mmsimd_f64_to_f32(const double* _in, float* _out, mmint_t _len);


#if INTPTR_MAX == INT64_MAX
#define mmsimd_iptr_to_i8           mmsimd_i64_to_i8
#define mmsimd_iptr_to_u8           mmsimd_i64_to_u8
#define mmsimd_iptr_to_i16          mmsimd_i64_to_i16
#define mmsimd_iptr_to_u16          mmsimd_i64_to_u16
#define mmsimd_iptr_to_i32          mmsimd_i64_to_i32
#define mmsimd_iptr_to_u32          mmsimd_i64_to_u32
#define mmsimd_iptr_to_f32          mmsimd_i64_to_f32
#define mmsimd_iptr_to_f64          mmsimd_i64_to_f64
#define mmsimd_uptr_to_i8           mmsimd_u64_to_i8
#define mmsimd_uptr_to_u8           mmsimd_u64_to_u8
#define mmsimd_uptr_to_i16          mmsimd_u64_to_i16
#define mmsimd_uptr_to_u16          mmsimd_u64_to_u16
#define mmsimd_uptr_to_i32          mmsimd_u64_to_i32
#define mmsimd_uptr_to_u32          mmsimd_u64_to_u32
#define mmsimd_uptr_to_f32          mmsimd_u64_to_f32
#define mmsimd_uptr_to_f64          mmsimd_u64_to_f64
#define mmsimd_size_to_i8           mmsimd_u64_to_i8
#define mmsimd_size_to_u8           mmsimd_u64_to_u8
#define mmsimd_size_to_i16          mmsimd_u64_to_i16
#define mmsimd_size_to_u16          mmsimd_u64_to_u16
#define mmsimd_size_to_i32          mmsimd_u64_to_i32
#define mmsimd_size_to_u32          mmsimd_u64_to_u32
#define mmsimd_size_to_f32          mmsimd_u64_to_f32
#define mmsimd_size_to_f64          mmsimd_u64_to_f64
#endif

#if INTPTR_MAX == INT32_MAX
#define mmsimd_iptr_to_i8           mmsimd_i32_to_i8
#define mmsimd_iptr_to_u8           mmsimd_i32_to_u8
#define mmsimd_iptr_to_i16          mmsimd_i32_to_i16
#define mmsimd_iptr_to_u16          mmsimd_i32_to_u16
#define mmsimd_iptr_to_i64          mmsimd_i32_to_i64
#define mmsimd_iptr_to_u64          mmsimd_i32_to_u64
#define mmsimd_iptr_to_f32          mmsimd_i32_to_f32
#define mmsimd_iptr_to_f64          mmsimd_i32_to_f64
#define mmsimd_uptr_to_i8           mmsimd_u32_to_i8
#define mmsimd_uptr_to_u8           mmsimd_u32_to_u8
#define mmsimd_uptr_to_i16          mmsimd_u32_to_i16
#define mmsimd_uptr_to_u16          mmsimd_u32_to_u16
#define mmsimd_uptr_to_i64          mmsimd_u32_to_i64
#define mmsimd_uptr_to_u64          mmsimd_u32_to_u64
#define mmsimd_uptr_to_f32          mmsimd_u32_to_f32
#define mmsimd_uptr_to_f64          mmsimd_u32_to_f64
#define mmsimd_size_to_i8           mmsimd_u32_to_i8
#define mmsimd_size_to_u8           mmsimd_u32_to_u8
#define mmsimd_size_toi16           mmsimd_u32_to_i16
#define mmsimd_size_to_u16          mmsimd_u32_to_u16
#define mmsimd_size_to_i64          mmsimd_u32_to_i64
#define mmsimd_size_to_u64          mmsimd_u32_to_u64
#define mmsimd_size_to_f32          mmsimd_u32_to_f32
#define mmsimd_size_to_f64          mmsimd_u32_to_f64
#endif

MEME_API void
MEME_STDCALL mmsimd_i16_fill(int16_t* _out, mmint_t _len, int16_t _val);

MEME_API void
MEME_STDCALL mmsimd_u16_fill(uint16_t* _out, mmint_t _len, uint16_t _val);

MEME_API void
MEME_STDCALL mmsimd_i32_fill(int32_t* _out, mmint_t _len, int32_t _val);

MEME_API void
MEME_STDCALL mmsimd_u32_fill(uint32_t* _out, mmint_t _len, uint32_t _val);

MEME_API void
MEME_STDCALL mmsimd_i64_fill(int64_t* _out, mmint_t _len, int64_t _val);

MEME_API void
MEME_STDCALL mmsimd_u64_fill(uint64_t* _out, mmint_t _len, uint64_t _val);

MEME_API void
MEME_STDCALL mmsimd_f32_fill(float* _out, mmint_t _len, float _val);

MEME_API void
MEME_STDCALL mmsimd_f64_fill(double* _out, mmint_t _len, double _val);

#if INTPTR_MAX == INT64_MAX
#define mmsimd_iptr_fill mmsimd_i64_fill
#define mmsimd_uptr_fill mmsimd_u64_fill
#define mmsimd_size_fill mmsimd_u64_fill
#endif

#if INTPTR_MAX == INT32_MAX
#define mmsimd_iptr_fill mmsimd_i32_fill
#define mmsimd_uptr_fill mmsimd_u32_fill
#define mmsimd_size_fill mmsimd_u32_fill
#endif

MEME_API mmint_t
MEME_STDCALL mmsimd_i8_find(const int8_t* _buf, mmint_t _len, int8_t _val);

MEME_API mmint_t
MEME_STDCALL mmsimd_i8_rfind(const int8_t* _buf, mmint_t _len, int8_t _val);

MEME_API mmint_t
MEME_STDCALL mmsimd_u8_find(const uint8_t* _buf, mmint_t _len, uint8_t _val);

MEME_API mmint_t
MEME_STDCALL mmsimd_u8_rfind(const uint8_t* _buf, mmint_t _len, uint8_t _val);

MEME_API mmint_t
MEME_STDCALL mmsimd_i16_find(const int16_t* _buf, mmint_t _len, int16_t _val);

MEME_API mmint_t
MEME_STDCALL mmsimd_i16_rfind(const int16_t* _buf, mmint_t _len, int16_t _val);

MEME_API mmint_t
MEME_STDCALL mmsimd_u16_find(const uint16_t* _buf, mmint_t _len, uint16_t _val);

MEME_API mmint_t
MEME_STDCALL mmsimd_u16_rfind(const uint16_t* _buf, mmint_t _len, uint16_t _val);

MEME_API mmint_t
MEME_STDCALL mmsimd_i32_find(const int32_t* _buf, mmint_t _len, int32_t _val);

MEME_API mmint_t
MEME_STDCALL mmsimd_i32_rfind(const int32_t* _buf, mmint_t _len, int32_t _val);

MEME_API mmint_t
MEME_STDCALL mmsimd_u32_find(const uint32_t* _buf, mmint_t _len, uint32_t _val);

MEME_API mmint_t
MEME_STDCALL mmsimd_u32_rfind(const uint32_t* _buf, mmint_t _len, uint32_t _val);

MEME_API mmint_t
MEME_STDCALL mmsimd_i64_find(const int64_t* _buf, mmint_t _len, int64_t _val);

MEME_API mmint_t
MEME_STDCALL mmsimd_i64_rfind(const int64_t* _buf, mmint_t _len, int64_t _val);

MEME_API mmint_t
MEME_STDCALL mmsimd_u64_find(const uint64_t* _buf, mmint_t _len, uint64_t _val);

MEME_API mmint_t
MEME_STDCALL mmsimd_u64_rfind(const uint64_t* _buf, mmint_t _len, uint64_t _val);

// MEME_API mmint_t
// MEME_STDCALL mmsimd_f32_find(const float* _buf, mmint_t _len, float _val);

// MEME_API mmint_t
// MEME_STDCALL mmsimd_f32_rfind(const float* _buf, mmint_t _len, float _val);

// MEME_API mmint_t
// MEME_STDCALL mmsimd_f64_find(const double* _buf, mmint_t _len, double _val);

// MEME_API mmint_t
// MEME_STDCALL mmsimd_f64_rfind(const double* _buf, mmint_t _len, double _val);

#if INTPTR_MAX == INT64_MAX
#define mmsimd_iptr_find  mmsimd_i64_find
#define mmsimd_iptr_rfind mmsimd_i64_rfind
#define mmsimd_uptr_find  mmsimd_u64_find
#define mmsimd_uptr_rfind mmsimd_u64_rfind
#define mmsimd_size_find  mmsimd_u64_find
#define mmsimd_size_rfind mmsimd_u64_rfind
#endif

#if INTPTR_MAX == INT32_MAX
#define mmsimd_iptr_find  mmsimd_i32_find
#define mmsimd_iptr_rfind mmsimd_i32_rfind
#define mmsimd_uptr_find  mmsimd_u32_find
#define mmsimd_uptr_rfind mmsimd_u32_rfind
#define mmsimd_size_find  mmsimd_u32_find
#define mmsimd_size_rfind mmsimd_u32_rfind
#endif

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_SIMD_SIMD_H_INCLUDED
