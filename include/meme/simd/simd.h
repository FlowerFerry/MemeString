
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

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_SIMD_SIMD_H_INCLUDED
