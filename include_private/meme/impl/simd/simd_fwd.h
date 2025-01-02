
#ifndef MEME_IMPL_SIMD_SIMD_FWD_H_INCLUDED
#define MEME_IMPL_SIMD_SIMD_FWD_H_INCLUDED

#include <meme/string_fwd.h>
#include <mego/err/ec.h>

#include <stdint.h>

MEME_EXTERN_C_SCOPE_START


struct mmsimd_arith8_hdlr;
typedef struct mmsimd_arith8_hdlr mmsimd_arith8_hdlr_t;

struct mmsimd_arith16_hdlr;
typedef struct mmsimd_arith16_hdlr mmsimd_arith16_hdlr_t;

struct mmsimd_arith32_hdlr;
typedef struct mmsimd_arith32_hdlr mmsimd_arith32_hdlr_t;

struct mmsimd_arith64_hdlr;
typedef struct mmsimd_arith64_hdlr mmsimd_arith64_hdlr_t;

struct mmsimd_conv8_hdlr;
typedef struct mmsimd_conv8_hdlr mmsimd_conv8_hdlr_t;

struct mmsimd_conv16_hdlr;
typedef struct mmsimd_conv16_hdlr mmsimd_conv16_hdlr_t;

struct mmsimd_conv32_hdlr;
typedef struct mmsimd_conv32_hdlr mmsimd_conv32_hdlr_t;

struct mmsimd_conv64_hdlr;
typedef struct mmsimd_conv64_hdlr mmsimd_conv64_hdlr_t;

struct mmsimd_other8_hdlr;
typedef struct mmsimd_other8_hdlr mmsimd_other8_hdlr_t;

struct mmsimd_other16_hdlr;
typedef struct mmsimd_other16_hdlr mmsimd_other16_hdlr_t;

struct mmsimd_other32_hdlr;
typedef struct mmsimd_other32_hdlr mmsimd_other32_hdlr_t;

struct mmsimd_other64_hdlr;
typedef struct mmsimd_other64_hdlr mmsimd_other64_hdlr_t;

typedef mmint_t(mmsimd_block_size_func_t)();

typedef void(mmsimd_i8_func_t)(
    const  int8_t* _a, const  int8_t* _b,  int8_t* _out, mmint_t _len);
typedef void(mmsimd_u8_func_t)(
    const uint8_t* _a, const uint8_t* _b, uint8_t* _out, mmint_t _len);
typedef void(mmsimd_i8_scalar_func_t)(
    const  int8_t* _a,  int8_t _b,  int8_t* _out, mmint_t _len);
typedef void(mmsimd_u8_scalar_func_t)(
    const uint8_t* _a, uint8_t _b, uint8_t* _out, mmint_t _len);

typedef void(mmsimd_i16_func_t)(
    const  int16_t* _a, const  int16_t* _b,  int16_t* _out, mmint_t _len);
typedef void(mmsimd_u16_func_t)(
    const uint16_t* _a, const uint16_t* _b, uint16_t* _out, mmint_t _len);
typedef void(mmsimd_i32_func_t)(
    const  int32_t* _a, const  int32_t* _b,  int32_t* _out, mmint_t _len);
typedef void(mmsimd_u32_func_t)(
    const uint32_t* _a, const uint32_t* _b, uint32_t* _out, mmint_t _len);
typedef void(mmsimd_i64_func_t)(
    const  int64_t* _a, const  int64_t* _b,  int64_t* _out, mmint_t _len);
typedef void(mmsimd_u64_func_t)(
    const uint64_t* _a, const uint64_t* _b, uint64_t* _out, mmint_t _len);
typedef void(mmsimd_i16_scalar_func_t)(
    const  int16_t* _a,  int16_t _b,  int16_t* _out, mmint_t _len);
typedef void(mmsimd_u16_scalar_func_t)(
    const uint16_t* _a, uint16_t _b, uint16_t* _out, mmint_t _len);
typedef void(mmsimd_i32_scalar_func_t)(
    const  int32_t* _a,  int32_t _b,  int32_t* _out, mmint_t _len);
typedef void(mmsimd_u32_scalar_func_t)(
    const uint32_t* _a, uint32_t _b, uint32_t* _out, mmint_t _len);
typedef void(mmsimd_i64_scalar_func_t)(
    const  int64_t* _a,  int64_t _b,  int64_t* _out, mmint_t _len);
typedef void(mmsimd_u64_scalar_func_t)(
    const uint64_t* _a, uint64_t _b, uint64_t* _out, mmint_t _len);
    
typedef void(mmsimd_f32_func_t)(
    const  float* _a, const  float* _b,  float* _out, mmint_t _len);
typedef void(mmsimd_f64_func_t)(
    const double* _a, const double* _b, double* _out, mmint_t _len);
typedef void(mmsimd_f32_scalar_func_t)(
    const  float* _a,  float _b,  float* _out, mmint_t _len);
typedef void(mmsimd_f64_scalar_func_t)(
    const double* _a, double _b, double* _out, mmint_t _len);

typedef void(mmsimd_i8_fill_func_t )(int8_t*   _out, mmint_t _len, int8_t   _v);
typedef void(mmsimd_u8_fill_func_t )(uint8_t*  _out, mmint_t _len, uint8_t  _v);
typedef void(mmsimd_i16_fill_func_t)(int16_t*  _out, mmint_t _len, int16_t  _v);
typedef void(mmsimd_u16_fill_func_t)(uint16_t* _out, mmint_t _len, uint16_t _v);
typedef void(mmsimd_i32_fill_func_t)(int32_t*  _out, mmint_t _len, int32_t  _v);
typedef void(mmsimd_u32_fill_func_t)(uint32_t* _out, mmint_t _len, uint32_t _v);
typedef void(mmsimd_i64_fill_func_t)(int64_t*  _out, mmint_t _len, int64_t  _v);
typedef void(mmsimd_u64_fill_func_t)(uint64_t* _out, mmint_t _len, uint64_t _v);
typedef void(mmsimd_f32_fill_func_t)(float*    _out, mmint_t _len, float    _v);
typedef void(mmsimd_f64_fill_func_t)(double*   _out, mmint_t _len, double   _v);

typedef mmint_t(mmsimd_i8_find_func_t )(const int8_t*   _a, mmint_t _len, int8_t   _v);
typedef mmint_t(mmsimd_u8_find_func_t )(const uint8_t*  _a, mmint_t _len, uint8_t  _v);
typedef mmint_t(mmsimd_i16_find_func_t)(const int16_t*  _a, mmint_t _len, int16_t  _v);
typedef mmint_t(mmsimd_u16_find_func_t)(const uint16_t* _a, mmint_t _len, uint16_t _v);
typedef mmint_t(mmsimd_i32_find_func_t)(const int32_t*  _a, mmint_t _len, int32_t  _v);
typedef mmint_t(mmsimd_u32_find_func_t)(const uint32_t* _a, mmint_t _len, uint32_t _v);
typedef mmint_t(mmsimd_i64_find_func_t)(const int64_t*  _a, mmint_t _len, int64_t  _v);
typedef mmint_t(mmsimd_u64_find_func_t)(const uint64_t* _a, mmint_t _len, uint64_t _v);
typedef mmint_t(mmsimd_f32_find_func_t)(const float*    _a, mmint_t _len, float    _v);
typedef mmint_t(mmsimd_f64_find_func_t)(const double*   _a, mmint_t _len, double   _v);

typedef void(mmsimd_i8_to_i16_func_t)(const int8_t* _in, int16_t*  _out, mmint_t _len);
typedef void(mmsimd_i8_to_u16_func_t)(const int8_t* _in, uint16_t* _out, mmint_t _len);
typedef void(mmsimd_i8_to_i32_func_t)(const int8_t* _in, int32_t*  _out, mmint_t _len);
typedef void(mmsimd_i8_to_u32_func_t)(const int8_t* _in, uint32_t* _out, mmint_t _len);
typedef void(mmsimd_i8_to_i64_func_t)(const int8_t* _in, int64_t*  _out, mmint_t _len);
typedef void(mmsimd_i8_to_u64_func_t)(const int8_t* _in, uint64_t* _out, mmint_t _len);
typedef void(mmsimd_i8_to_f32_func_t)(const int8_t* _in, float*    _out, mmint_t _len);
typedef void(mmsimd_i8_to_f64_func_t)(const int8_t* _in, double*   _out, mmint_t _len);

typedef void(mmsimd_u8_to_i16_func_t)(const uint8_t* _in, int16_t*  _out, mmint_t _len);
typedef void(mmsimd_u8_to_u16_func_t)(const uint8_t* _in, uint16_t* _out, mmint_t _len);
typedef void(mmsimd_u8_to_i32_func_t)(const uint8_t* _in, int32_t*  _out, mmint_t _len);
typedef void(mmsimd_u8_to_u32_func_t)(const uint8_t* _in, uint32_t* _out, mmint_t _len);
typedef void(mmsimd_u8_to_i64_func_t)(const uint8_t* _in, int64_t*  _out, mmint_t _len);
typedef void(mmsimd_u8_to_u64_func_t)(const uint8_t* _in, uint64_t* _out, mmint_t _len);
typedef void(mmsimd_u8_to_f32_func_t)(const uint8_t* _in, float*    _out, mmint_t _len);
typedef void(mmsimd_u8_to_f64_func_t)(const uint8_t* _in, double*   _out, mmint_t _len);

typedef void(mmsimd_i16_to_i8_func_t )(const int16_t* _in, int8_t*   _out, mmint_t _len);
typedef void(mmsimd_i16_to_u8_func_t )(const int16_t* _in, uint8_t*  _out, mmint_t _len);
typedef void(mmsimd_i16_to_i32_func_t)(const int16_t* _in, int32_t*  _out, mmint_t _len);
typedef void(mmsimd_i16_to_u32_func_t)(const int16_t* _in, uint32_t* _out, mmint_t _len);
typedef void(mmsimd_i16_to_i64_func_t)(const int16_t* _in, int64_t*  _out, mmint_t _len);
typedef void(mmsimd_i16_to_u64_func_t)(const int16_t* _in, uint64_t* _out, mmint_t _len);
typedef void(mmsimd_i16_to_f32_func_t)(const int16_t* _in, float*    _out, mmint_t _len);
typedef void(mmsimd_i16_to_f64_func_t)(const int16_t* _in, double*   _out, mmint_t _len);

typedef void(mmsimd_u16_to_i8_func_t )(const uint16_t* _in, int8_t*   _out, mmint_t _len);
typedef void(mmsimd_u16_to_u8_func_t )(const uint16_t* _in, uint8_t*  _out, mmint_t _len);
typedef void(mmsimd_u16_to_i32_func_t)(const uint16_t* _in, int32_t*  _out, mmint_t _len);
typedef void(mmsimd_u16_to_u32_func_t)(const uint16_t* _in, uint32_t* _out, mmint_t _len);
typedef void(mmsimd_u16_to_i64_func_t)(const uint16_t* _in, int64_t*  _out, mmint_t _len);
typedef void(mmsimd_u16_to_u64_func_t)(const uint16_t* _in, uint64_t* _out, mmint_t _len);
typedef void(mmsimd_u16_to_f32_func_t)(const uint16_t* _in, float*    _out, mmint_t _len);
typedef void(mmsimd_u16_to_f64_func_t)(const uint16_t* _in, double*   _out, mmint_t _len);

typedef void(mmsimd_i32_to_i8_func_t )(const int32_t* _in, int8_t*   _out, mmint_t _len);
typedef void(mmsimd_i32_to_u8_func_t )(const int32_t* _in, uint8_t*  _out, mmint_t _len);
typedef void(mmsimd_i32_to_i16_func_t)(const int32_t* _in, int16_t*  _out, mmint_t _len);
typedef void(mmsimd_i32_to_u16_func_t)(const int32_t* _in, uint16_t* _out, mmint_t _len);
typedef void(mmsimd_i32_to_i64_func_t)(const int32_t* _in, int64_t*  _out, mmint_t _len);
typedef void(mmsimd_i32_to_u64_func_t)(const int32_t* _in, uint64_t* _out, mmint_t _len);
typedef void(mmsimd_i32_to_f32_func_t)(const int32_t* _in, float*    _out, mmint_t _len);
typedef void(mmsimd_i32_to_f64_func_t)(const int32_t* _in, double*   _out, mmint_t _len);

typedef void(mmsimd_u32_to_i8_func_t )(const uint32_t* _in, int8_t*   _out, mmint_t _len);
typedef void(mmsimd_u32_to_u8_func_t )(const uint32_t* _in, uint8_t*  _out, mmint_t _len);
typedef void(mmsimd_u32_to_i16_func_t)(const uint32_t* _in, int16_t*  _out, mmint_t _len);
typedef void(mmsimd_u32_to_u16_func_t)(const uint32_t* _in, uint16_t* _out, mmint_t _len);
typedef void(mmsimd_u32_to_i64_func_t)(const uint32_t* _in, int64_t*  _out, mmint_t _len);
typedef void(mmsimd_u32_to_u64_func_t)(const uint32_t* _in, uint64_t* _out, mmint_t _len);
typedef void(mmsimd_u32_to_f32_func_t)(const uint32_t* _in, float*    _out, mmint_t _len);
typedef void(mmsimd_u32_to_f64_func_t)(const uint32_t* _in, double*   _out, mmint_t _len);

typedef void(mmsimd_i64_to_i8_func_t )(const int64_t* _in, int8_t*   _out, mmint_t _len);
typedef void(mmsimd_i64_to_u8_func_t )(const int64_t* _in, uint8_t*  _out, mmint_t _len);
typedef void(mmsimd_i64_to_i16_func_t)(const int64_t* _in, int16_t*  _out, mmint_t _len);
typedef void(mmsimd_i64_to_u16_func_t)(const int64_t* _in, uint16_t* _out, mmint_t _len);
typedef void(mmsimd_i64_to_i32_func_t)(const int64_t* _in, int32_t*  _out, mmint_t _len);
typedef void(mmsimd_i64_to_u32_func_t)(const int64_t* _in, uint32_t* _out, mmint_t _len);
typedef void(mmsimd_i64_to_f32_func_t)(const int64_t* _in, float*    _out, mmint_t _len);
typedef void(mmsimd_i64_to_f64_func_t)(const int64_t* _in, double*   _out, mmint_t _len);

typedef void(mmsimd_u64_to_i8_func_t )(const uint64_t* _in, int8_t*   _out, mmint_t _len);
typedef void(mmsimd_u64_to_u8_func_t )(const uint64_t* _in, uint8_t*  _out, mmint_t _len);
typedef void(mmsimd_u64_to_i16_func_t)(const uint64_t* _in, int16_t*  _out, mmint_t _len);
typedef void(mmsimd_u64_to_u16_func_t)(const uint64_t* _in, uint16_t* _out, mmint_t _len);
typedef void(mmsimd_u64_to_i32_func_t)(const uint64_t* _in, int32_t*  _out, mmint_t _len);
typedef void(mmsimd_u64_to_u32_func_t)(const uint64_t* _in, uint32_t* _out, mmint_t _len);
typedef void(mmsimd_u64_to_f32_func_t)(const uint64_t* _in, float*    _out, mmint_t _len);
typedef void(mmsimd_u64_to_f64_func_t)(const uint64_t* _in, double*   _out, mmint_t _len);

typedef void(mmsimd_f32_to_i8_func_t )(const float* _in, int8_t*   _out, mmint_t _len);
typedef void(mmsimd_f32_to_u8_func_t )(const float* _in, uint8_t*  _out, mmint_t _len);
typedef void(mmsimd_f32_to_i16_func_t)(const float* _in, int16_t*  _out, mmint_t _len);
typedef void(mmsimd_f32_to_u16_func_t)(const float* _in, uint16_t* _out, mmint_t _len);
typedef void(mmsimd_f32_to_i32_func_t)(const float* _in, int32_t*  _out, mmint_t _len);
typedef void(mmsimd_f32_to_u32_func_t)(const float* _in, uint32_t* _out, mmint_t _len);
typedef void(mmsimd_f32_to_i64_func_t)(const float* _in, int64_t*  _out, mmint_t _len);
typedef void(mmsimd_f32_to_u64_func_t)(const float* _in, uint64_t* _out, mmint_t _len);
typedef void(mmsimd_f32_to_f64_func_t)(const float* _in, double*   _out, mmint_t _len);

typedef void(mmsimd_f64_to_i8_func_t )(const double* _in, int8_t*   _out, mmint_t _len);
typedef void(mmsimd_f64_to_u8_func_t )(const double* _in, uint8_t*  _out, mmint_t _len);
typedef void(mmsimd_f64_to_i16_func_t)(const double* _in, int16_t*  _out, mmint_t _len);
typedef void(mmsimd_f64_to_u16_func_t)(const double* _in, uint16_t* _out, mmint_t _len);
typedef void(mmsimd_f64_to_i32_func_t)(const double* _in, int32_t*  _out, mmint_t _len);
typedef void(mmsimd_f64_to_u32_func_t)(const double* _in, uint32_t* _out, mmint_t _len);
typedef void(mmsimd_f64_to_i64_func_t)(const double* _in, int64_t*  _out, mmint_t _len);
typedef void(mmsimd_f64_to_u64_func_t)(const double* _in, uint64_t* _out, mmint_t _len);
typedef void(mmsimd_f64_to_f32_func_t)(const double* _in, float*    _out, mmint_t _len);

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_IMPL_SIMD_SIMD_FWD_H_INCLUDED
