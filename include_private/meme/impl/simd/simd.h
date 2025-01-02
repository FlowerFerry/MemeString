
#ifndef MEME_IMPL_SIMD_SIMD_H_INCLUDED
#define MEME_IMPL_SIMD_SIMD_H_INCLUDED

#include <meme/impl/simd/simd_fwd.h>

MEME_EXTERN_C_SCOPE_START

struct mmsimd_arith8_hdlr
{
    mmsimd_i8_func_t* i8_add_fn;
    mmsimd_i8_func_t* i8_sub_fn;
    mmsimd_i8_func_t* i8_mul_fn;
    mmsimd_i8_func_t* i8_div_fn;
    mmsimd_u8_func_t* u8_add_fn;
    mmsimd_u8_func_t* u8_sub_fn;
    mmsimd_u8_func_t* u8_mul_fn;
    mmsimd_u8_func_t* u8_div_fn;
    mmsimd_i8_scalar_func_t* i8_add_scalar_fn;
    mmsimd_i8_scalar_func_t* i8_sub_scalar_fn;
    mmsimd_i8_scalar_func_t* i8_mul_scalar_fn;
    mmsimd_i8_scalar_func_t* i8_div_scalar_fn;
    mmsimd_u8_scalar_func_t* u8_add_scalar_fn;
    mmsimd_u8_scalar_func_t* u8_sub_scalar_fn;
    mmsimd_u8_scalar_func_t* u8_mul_scalar_fn;
    mmsimd_u8_scalar_func_t* u8_div_scalar_fn;
};

struct mmsimd_arith16_hdlr
{
    mmsimd_i16_func_t* i16_add_fn;
    mmsimd_i16_func_t* i16_sub_fn;
    mmsimd_i16_func_t* i16_mul_fn;
    mmsimd_i16_func_t* i16_div_fn;
    mmsimd_u16_func_t* u16_add_fn;
    mmsimd_u16_func_t* u16_sub_fn;
    mmsimd_u16_func_t* u16_mul_fn;
    mmsimd_u16_func_t* u16_div_fn;
    mmsimd_i16_scalar_func_t* i16_add_scalar_fn;
    mmsimd_i16_scalar_func_t* i16_sub_scalar_fn;
    mmsimd_i16_scalar_func_t* i16_mul_scalar_fn;
    mmsimd_i16_scalar_func_t* i16_div_scalar_fn;
    mmsimd_u16_scalar_func_t* u16_add_scalar_fn;
    mmsimd_u16_scalar_func_t* u16_sub_scalar_fn;
    mmsimd_u16_scalar_func_t* u16_mul_scalar_fn;
    mmsimd_u16_scalar_func_t* u16_div_scalar_fn;
};

struct mmsimd_arith32_hdlr
{
    mmsimd_i32_func_t* i32_add_fn;
    mmsimd_i32_func_t* i32_sub_fn;
    mmsimd_i32_func_t* i32_mul_fn;
    mmsimd_i32_func_t* i32_div_fn;
    mmsimd_u32_func_t* u32_add_fn;
    mmsimd_u32_func_t* u32_sub_fn;
    mmsimd_u32_func_t* u32_mul_fn;
    mmsimd_u32_func_t* u32_div_fn;
    mmsimd_i32_scalar_func_t* i32_add_scalar_fn;
    mmsimd_i32_scalar_func_t* i32_sub_scalar_fn;
    mmsimd_i32_scalar_func_t* i32_mul_scalar_fn;
    mmsimd_i32_scalar_func_t* i32_div_scalar_fn;
    mmsimd_u32_scalar_func_t* u32_add_scalar_fn;
    mmsimd_u32_scalar_func_t* u32_sub_scalar_fn;
    mmsimd_u32_scalar_func_t* u32_mul_scalar_fn;
    mmsimd_u32_scalar_func_t* u32_div_scalar_fn;
    mmsimd_f32_func_t* f32_add_fn;
    mmsimd_f32_func_t* f32_sub_fn;
    mmsimd_f32_func_t* f32_mul_fn;
    mmsimd_f32_func_t* f32_div_fn;
    mmsimd_f32_scalar_func_t* f32_add_scalar_fn;
    mmsimd_f32_scalar_func_t* f32_sub_scalar_fn;
    mmsimd_f32_scalar_func_t* f32_mul_scalar_fn;
    mmsimd_f32_scalar_func_t* f32_div_scalar_fn;
};

struct mmsimd_arith64_hdlr
{
    mmsimd_i64_func_t* i64_add_fn;
    mmsimd_i64_func_t* i64_sub_fn;
    mmsimd_i64_func_t* i64_mul_fn;
    mmsimd_i64_func_t* i64_div_fn;
    mmsimd_u64_func_t* u64_add_fn;
    mmsimd_u64_func_t* u64_sub_fn;
    mmsimd_u64_func_t* u64_mul_fn;
    mmsimd_u64_func_t* u64_div_fn;
    mmsimd_i64_scalar_func_t* i64_add_scalar_fn;
    mmsimd_i64_scalar_func_t* i64_sub_scalar_fn;
    mmsimd_i64_scalar_func_t* i64_mul_scalar_fn;
    mmsimd_i64_scalar_func_t* i64_div_scalar_fn;
    mmsimd_u64_scalar_func_t* u64_add_scalar_fn;
    mmsimd_u64_scalar_func_t* u64_sub_scalar_fn;
    mmsimd_u64_scalar_func_t* u64_mul_scalar_fn;
    mmsimd_u64_scalar_func_t* u64_div_scalar_fn;
    mmsimd_f64_func_t* f64_add_fn;
    mmsimd_f64_func_t* f64_sub_fn;
    mmsimd_f64_func_t* f64_mul_fn;
    mmsimd_f64_func_t* f64_div_fn;
    mmsimd_f64_scalar_func_t* f64_add_scalar_fn;
    mmsimd_f64_scalar_func_t* f64_sub_scalar_fn;
    mmsimd_f64_scalar_func_t* f64_mul_scalar_fn;
    mmsimd_f64_scalar_func_t* f64_div_scalar_fn;

};

struct mmsimd_conv8_hdlr
{
    mmsimd_i8_to_i16_func_t* i8_to_i16_fn;
    mmsimd_i8_to_u16_func_t* i8_to_u16_fn;
    mmsimd_i8_to_i32_func_t* i8_to_i32_fn;
    mmsimd_i8_to_u32_func_t* i8_to_u32_fn;
    mmsimd_i8_to_i64_func_t* i8_to_i64_fn;
    mmsimd_i8_to_u64_func_t* i8_to_u64_fn;
    mmsimd_i8_to_f32_func_t* i8_to_f32_fn;
    mmsimd_i8_to_f64_func_t* i8_to_f64_fn;

    mmsimd_u8_to_i16_func_t* u8_to_i16_fn;
    mmsimd_u8_to_u16_func_t* u8_to_u16_fn;
    mmsimd_u8_to_i32_func_t* u8_to_i32_fn;
    mmsimd_u8_to_u32_func_t* u8_to_u32_fn;
    mmsimd_u8_to_i64_func_t* u8_to_i64_fn;
    mmsimd_u8_to_u64_func_t* u8_to_u64_fn;
    mmsimd_u8_to_f32_func_t* u8_to_f32_fn;
    mmsimd_u8_to_f64_func_t* u8_to_f64_fn;
};

struct mmsimd_conv16_hdlr
{
    mmsimd_i16_to_i8_func_t*  i16_to_i8_fn;
    mmsimd_i16_to_u8_func_t*  i16_to_u8_fn;
    mmsimd_i16_to_i32_func_t* i16_to_i32_fn;
    mmsimd_i16_to_u32_func_t* i16_to_u32_fn;
    mmsimd_i16_to_i64_func_t* i16_to_i64_fn;
    mmsimd_i16_to_u64_func_t* i16_to_u64_fn;
    mmsimd_i16_to_f32_func_t* i16_to_f32_fn;
    mmsimd_i16_to_f64_func_t* i16_to_f64_fn;

    mmsimd_u16_to_i8_func_t*  u16_to_i8_fn;
    mmsimd_u16_to_u8_func_t*  u16_to_u8_fn;
    mmsimd_u16_to_i32_func_t* u16_to_i32_fn;
    mmsimd_u16_to_u32_func_t* u16_to_u32_fn;
    mmsimd_u16_to_i64_func_t* u16_to_i64_fn;
    mmsimd_u16_to_u64_func_t* u16_to_u64_fn;
    mmsimd_u16_to_f32_func_t* u16_to_f32_fn;
    mmsimd_u16_to_f64_func_t* u16_to_f64_fn;
};


struct mmsimd_conv32_hdlr
{
    mmsimd_i32_to_i8_func_t*  i32_to_i8_fn;
    mmsimd_i32_to_u8_func_t*  i32_to_u8_fn;
    mmsimd_i32_to_i16_func_t* i32_to_i16_fn;
    mmsimd_i32_to_u16_func_t* i32_to_u16_fn;
    mmsimd_i32_to_i64_func_t* i32_to_i64_fn;
    mmsimd_i32_to_u64_func_t* i32_to_u64_fn;
    mmsimd_i32_to_f32_func_t* i32_to_f32_fn;
    mmsimd_i32_to_f64_func_t* i32_to_f64_fn;

    mmsimd_u32_to_i8_func_t*  u32_to_i8_fn;
    mmsimd_u32_to_u8_func_t*  u32_to_u8_fn;
    mmsimd_u32_to_i16_func_t* u32_to_i16_fn;
    mmsimd_u32_to_u16_func_t* u32_to_u16_fn;
    mmsimd_u32_to_i64_func_t* u32_to_i64_fn;
    mmsimd_u32_to_u64_func_t* u32_to_u64_fn;
    mmsimd_u32_to_f32_func_t* u32_to_f32_fn;
    mmsimd_u32_to_f64_func_t* u32_to_f64_fn;

    mmsimd_f32_to_i8_func_t*  f32_to_i8_fn;
    mmsimd_f32_to_u8_func_t*  f32_to_u8_fn;
    mmsimd_f32_to_i16_func_t* f32_to_i16_fn;
    mmsimd_f32_to_u16_func_t* f32_to_u16_fn;
    mmsimd_f32_to_i32_func_t* f32_to_i32_fn;
    mmsimd_f32_to_u32_func_t* f32_to_u32_fn;
    mmsimd_f32_to_i64_func_t* f32_to_i64_fn;
    mmsimd_f32_to_u64_func_t* f32_to_u64_fn;
    mmsimd_f32_to_f64_func_t* f32_to_f64_fn;

};


struct mmsimd_conv64_hdlr
{
    mmsimd_i64_to_i8_func_t*  i64_to_i8_fn;
    mmsimd_i64_to_u8_func_t*  i64_to_u8_fn;
    mmsimd_i64_to_i16_func_t* i64_to_i16_fn;
    mmsimd_i64_to_u16_func_t* i64_to_u16_fn;
    mmsimd_i64_to_i32_func_t* i64_to_i32_fn;
    mmsimd_i64_to_u32_func_t* i64_to_u32_fn;
    mmsimd_i64_to_f32_func_t* i64_to_f32_fn;
    mmsimd_i64_to_f64_func_t* i64_to_f64_fn;

    mmsimd_u64_to_i8_func_t*  u64_to_i8_fn;
    mmsimd_u64_to_u8_func_t*  u64_to_u8_fn;
    mmsimd_u64_to_i16_func_t* u64_to_i16_fn;
    mmsimd_u64_to_u16_func_t* u64_to_u16_fn;
    mmsimd_u64_to_i32_func_t* u64_to_i32_fn;
    mmsimd_u64_to_u32_func_t* u64_to_u32_fn;
    mmsimd_u64_to_f32_func_t* u64_to_f32_fn;
    mmsimd_u64_to_f64_func_t* u64_to_f64_fn;

    mmsimd_f64_to_i8_func_t*  f64_to_i8_fn;
    mmsimd_f64_to_u8_func_t*  f64_to_u8_fn;
    mmsimd_f64_to_i16_func_t* f64_to_i16_fn;
    mmsimd_f64_to_u16_func_t* f64_to_u16_fn;
    mmsimd_f64_to_i32_func_t* f64_to_i32_fn;
    mmsimd_f64_to_u32_func_t* f64_to_u32_fn;
    mmsimd_f64_to_i64_func_t* f64_to_i64_fn;
    mmsimd_f64_to_u64_func_t* f64_to_u64_fn;
    mmsimd_f64_to_f32_func_t* f64_to_f32_fn;
};

struct mmsimd_other8_hdlr
{
    mmsimd_i8_find_func_t* i8_find_fn;
    mmsimd_u8_find_func_t* u8_find_fn;
    mmsimd_i8_find_func_t* i8_rfind_fn;
    mmsimd_u8_find_func_t* u8_rfind_fn;
};

struct mmsimd_other16_hdlr
{
    mmsimd_i16_fill_func_t* i16_fill_fn;
    mmsimd_u16_fill_func_t* u16_fill_fn;
    mmsimd_i16_find_func_t* i16_find_fn;
    mmsimd_u16_find_func_t* u16_find_fn;
    mmsimd_i16_find_func_t* i16_rfind_fn;
    mmsimd_u16_find_func_t* u16_rfind_fn;
};

struct mmsimd_other32_hdlr
{
    mmsimd_i32_fill_func_t* i32_fill_fn;
    mmsimd_u32_fill_func_t* u32_fill_fn;
    mmsimd_f32_fill_func_t* f32_fill_fn;
    mmsimd_i32_find_func_t* i32_find_fn;
    mmsimd_u32_find_func_t* u32_find_fn;
    mmsimd_f32_find_func_t* f32_find_fn;
    mmsimd_i32_find_func_t* i32_rfind_fn;
    mmsimd_u32_find_func_t* u32_rfind_fn;
    mmsimd_f32_find_func_t* f32_rfind_fn;
};

struct mmsimd_other64_hdlr
{
    mmsimd_i64_fill_func_t* i64_fill_fn;
    mmsimd_u64_fill_func_t* u64_fill_fn;
    mmsimd_f64_fill_func_t* f64_fill_fn;
    mmsimd_i64_find_func_t* i64_find_fn;
    mmsimd_u64_find_func_t* u64_find_fn;
    mmsimd_f64_find_func_t* f64_find_fn;
    mmsimd_i64_find_func_t* i64_rfind_fn;
    mmsimd_u64_find_func_t* u64_rfind_fn;
    mmsimd_f64_find_func_t* f64_rfind_fn;
};

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_IMPL_SIMD_SIMD_H_INCLUDED
