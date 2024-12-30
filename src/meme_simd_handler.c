
#include <mego/predef/architecture/x86.h>
#include <mego/hardware/cpu/instruction.h>
#include <mego/thrd/call_once.h>
#include <meme/simd/simd.h>
#include <meme/impl/simd/simd.h>

#include "meme_simd_default.c.inl"
#if MEGO_ARCH__X86 || MEGO_ARCH__X64 
#include "meme_simd_avx2.c.inl"
#endif

#include <stdlib.h>

MEME_EXTERN_C_SCOPE_START

static mmsimd_arith8_hdlr_t* mmsimd_get_default_arith8_handler()
{
    static mmsimd_arith8_hdlr_t hdlr = {
        .i8_add_fn = mmsimd_default_i8_add,
        .i8_sub_fn = mmsimd_default_i8_sub,
        .i8_mul_fn = mmsimd_default_i8_mul,
        .i8_div_fn = mmsimd_default_i8_div,
        .u8_add_fn = mmsimd_default_u8_add,
        .u8_sub_fn = mmsimd_default_u8_sub,
        .u8_mul_fn = mmsimd_default_u8_mul,
        .u8_div_fn = mmsimd_default_u8_div,
        .i8_add_scalar_fn = mmsimd_default_i8_add_scalar,
        .i8_sub_scalar_fn = mmsimd_default_i8_sub_scalar,
        .i8_mul_scalar_fn = mmsimd_default_i8_mul_scalar,
        .i8_div_scalar_fn = mmsimd_default_i8_div_scalar,
        .u8_add_scalar_fn = mmsimd_default_u8_add_scalar,
        .u8_sub_scalar_fn = mmsimd_default_u8_sub_scalar,
        .u8_mul_scalar_fn = mmsimd_default_u8_mul_scalar,
        .u8_div_scalar_fn = mmsimd_default_u8_div_scalar
    };
    return &hdlr;
}

static mmsimd_arith16_hdlr_t* mmsimd_get_default_arith16_handler()
{
    static mmsimd_arith16_hdlr_t hdlr = {
        .i16_add_fn = mmsimd_default_i16_add,
        .i16_sub_fn = mmsimd_default_i16_sub,
        .i16_mul_fn = mmsimd_default_i16_mul,
        .i16_div_fn = mmsimd_default_i16_div,
        .u16_add_fn = mmsimd_default_u16_add,
        .u16_sub_fn = mmsimd_default_u16_sub,
        .u16_mul_fn = mmsimd_default_u16_mul,
        .u16_div_fn = mmsimd_default_u16_div,
        .i16_add_scalar_fn = mmsimd_default_i16_add_scalar,
        .i16_sub_scalar_fn = mmsimd_default_i16_sub_scalar,
        .i16_mul_scalar_fn = mmsimd_default_i16_mul_scalar,
        .i16_div_scalar_fn = mmsimd_default_i16_div_scalar,
        .u16_add_scalar_fn = mmsimd_default_u16_add_scalar,
        .u16_sub_scalar_fn = mmsimd_default_u16_sub_scalar,
        .u16_mul_scalar_fn = mmsimd_default_u16_mul_scalar,
        .u16_div_scalar_fn = mmsimd_default_u16_div_scalar
    };
    return &hdlr;
}

static mmsimd_arith32_hdlr_t* mmsimd_get_default_arith32_handler()
{
    static mmsimd_arith32_hdlr_t hdlr = {
        .i32_add_fn = mmsimd_default_i32_add,
        .i32_sub_fn = mmsimd_default_i32_sub,
        .i32_mul_fn = mmsimd_default_i32_mul,
        .i32_div_fn = mmsimd_default_i32_div,
        .u32_add_fn = mmsimd_default_u32_add,
        .u32_sub_fn = mmsimd_default_u32_sub,
        .u32_mul_fn = mmsimd_default_u32_mul,
        .u32_div_fn = mmsimd_default_u32_div,
        .i32_add_scalar_fn = mmsimd_default_i32_add_scalar,
        .i32_sub_scalar_fn = mmsimd_default_i32_sub_scalar,
        .i32_mul_scalar_fn = mmsimd_default_i32_mul_scalar,
        .i32_div_scalar_fn = mmsimd_default_i32_div_scalar,
        .u32_add_scalar_fn = mmsimd_default_u32_add_scalar,
        .u32_sub_scalar_fn = mmsimd_default_u32_sub_scalar,
        .u32_mul_scalar_fn = mmsimd_default_u32_mul_scalar,
        .u32_div_scalar_fn = mmsimd_default_u32_div_scalar,
        .f32_add_fn = mmsimd_default_f32_add,
        .f32_sub_fn = mmsimd_default_f32_sub,
        .f32_mul_fn = mmsimd_default_f32_mul,
        .f32_div_fn = mmsimd_default_f32_div,
        .f32_add_scalar_fn = mmsimd_default_f32_add_scalar,
        .f32_sub_scalar_fn = mmsimd_default_f32_sub_scalar,
        .f32_mul_scalar_fn = mmsimd_default_f32_mul_scalar,
        .f32_div_scalar_fn = mmsimd_default_f32_div_scalar
    };
    return &hdlr;
}

static mmsimd_arith64_hdlr_t* mmsimd_get_default_arith64_handler()
{
    static mmsimd_arith64_hdlr_t hdlr = {
        .i64_add_fn = mmsimd_default_i64_add,
        .i64_sub_fn = mmsimd_default_i64_sub,
        .i64_mul_fn = mmsimd_default_i64_mul,
        .i64_div_fn = mmsimd_default_i64_div,
        .u64_add_fn = mmsimd_default_u64_add,
        .u64_sub_fn = mmsimd_default_u64_sub,
        .u64_mul_fn = mmsimd_default_u64_mul,
        .u64_div_fn = mmsimd_default_u64_div,
        .i64_add_scalar_fn = mmsimd_default_i64_add_scalar,
        .i64_sub_scalar_fn = mmsimd_default_i64_sub_scalar,
        .i64_mul_scalar_fn = mmsimd_default_i64_mul_scalar,
        .i64_div_scalar_fn = mmsimd_default_i64_div_scalar,
        .u64_add_scalar_fn = mmsimd_default_u64_add_scalar,
        .u64_sub_scalar_fn = mmsimd_default_u64_sub_scalar,
        .u64_mul_scalar_fn = mmsimd_default_u64_mul_scalar,
        .u64_div_scalar_fn = mmsimd_default_u64_div_scalar,
        .f64_add_fn = mmsimd_default_f64_add,
        .f64_sub_fn = mmsimd_default_f64_sub,
        .f64_mul_fn = mmsimd_default_f64_mul,
        .f64_div_fn = mmsimd_default_f64_div,
        .f64_add_scalar_fn = mmsimd_default_f64_add_scalar,
        .f64_sub_scalar_fn = mmsimd_default_f64_sub_scalar,
        .f64_mul_scalar_fn = mmsimd_default_f64_mul_scalar,
        .f64_div_scalar_fn = mmsimd_default_f64_div_scalar
    };
    return &hdlr;
}

static mmsimd_arith8_hdlr_t* mmsimd_get_avx2_arith8_handler()
{
#if MEGO_ARCH__X86 || MEGO_ARCH__X64 
    static mmsimd_arith8_hdlr_t hdlr = {
        .i8_add_fn = mmsimd_avx2_i8_add,
        .i8_sub_fn = mmsimd_avx2_i8_sub,
        .i8_mul_fn = mmsimd_default_i8_mul,
#if MG_COMP__MSVC_AVAIL
        .i8_div_fn = mmsimd_avx2_i8_div,
#else
        .i8_div_fn = mmsimd_default_i8_div,
#endif
        .u8_add_fn = mmsimd_avx2_u8_add,
        .u8_sub_fn = mmsimd_avx2_u8_sub,
        .u8_mul_fn = mmsimd_default_u8_mul,
#if MG_COMP__MSVC_AVAIL
        .u8_div_fn = mmsimd_avx2_u8_div,
#else
        .u8_div_fn = mmsimd_default_u8_div,
#endif
        .i8_add_scalar_fn = mmsimd_avx2_i8_add_scalar,
        .i8_sub_scalar_fn = mmsimd_avx2_i8_sub_scalar,
        .i8_mul_scalar_fn = mmsimd_default_i8_mul_scalar,
#if MG_COMP__MSVC_AVAIL
        .i8_div_scalar_fn = mmsimd_avx2_i8_div_scalar,
#else
        .i8_div_scalar_fn = mmsimd_default_i8_div_scalar,
#endif
        .u8_add_scalar_fn = mmsimd_avx2_u8_add_scalar,
        .u8_sub_scalar_fn = mmsimd_avx2_u8_sub_scalar,
        .u8_mul_scalar_fn = mmsimd_default_u8_mul_scalar,
#if MG_COMP__MSVC_AVAIL
        .u8_div_scalar_fn = mmsimd_avx2_u8_div_scalar
#else
        .u8_div_scalar_fn = mmsimd_default_u8_div_scalar
#endif
    };
    return &hdlr;
    
#else
    return mmsimd_get_default_arith8_handler();
#endif
}

static mgthrd_once_flag* __mmsimd_get_arith8_startup_once_flag()
{
    static mgthrd_once_flag f = MGTHRD_ONCE_FLAG_INIT;
    return &f;
}

static mgthrd_once_flag* __mmsimd_get_arith16_startup_once_flag()
{
    static mgthrd_once_flag f = MGTHRD_ONCE_FLAG_INIT;
    return &f;
}

static mgthrd_once_flag* __mmsimd_get_arith32_startup_once_flag()
{
    static mgthrd_once_flag f = MGTHRD_ONCE_FLAG_INIT;
    return &f;
}

static mgthrd_once_flag* __mmsimd_get_arith64_startup_once_flag()
{
    static mgthrd_once_flag f = MGTHRD_ONCE_FLAG_INIT;
    return &f;
}

static volatile uintptr_t* __mmsimd_get_arith8_handler_pointer()
{
    static volatile mmint_t pointer = 0;
    return &pointer;
}

static volatile uintptr_t* __mmsimd_get_arith16_handler_pointer()
{
    static volatile mmint_t pointer = 0;
    return &pointer;
}

static volatile uintptr_t* __mmsimd_get_arith32_handler_pointer()
{
    static volatile mmint_t pointer = 0;
    return &pointer;
}

static volatile uintptr_t* __mmsimd_get_arith64_handler_pointer()
{
    static volatile mmint_t pointer = 0;
    return &pointer;
}

static void __mmsimd_arith8_startup(void)
{
    mghw_simd_instruction_t instructions = mghw_detect_supported_simd_instructions();

    if (instructions & MGHW_SIMD_INSTRUCTION__AVX2)
    {
        *__mmsimd_get_arith8_handler_pointer() = (uintptr_t)mmsimd_get_avx2_arith8_handler();
        return;
    }

    *__mmsimd_get_arith8_handler_pointer() = (uintptr_t)mmsimd_get_default_arith8_handler();
}

static void __mmsimd_arith16_startup(void)
{
    mghw_simd_instruction_t instructions = mghw_detect_supported_simd_instructions();

    //if (instructions & MGHW_SIMD_INSTRUCTION__AVX2)
    //{
    //    *__mmsimd_get_arith16_handler_pointer() = (uintptr_t)mmsimd_get_avx2_arith16_handler();
    //    return;
    //}

    *__mmsimd_get_arith16_handler_pointer() = (uintptr_t)mmsimd_get_default_arith16_handler();
}

static void __mmsimd_arith32_startup(void)
{
    mghw_simd_instruction_t instructions = mghw_detect_supported_simd_instructions();

    //if (instructions & MGHW_SIMD_INSTRUCTION__AVX2)
    //{
    //    *__mmsimd_get_arith32_handler_pointer() = (uintptr_t)mmsimd_get_avx2_arith32_handler();
    //    return;
    //}

    *__mmsimd_get_arith32_handler_pointer() = (uintptr_t)mmsimd_get_default_arith32_handler();
}

static void __mmsimd_arith64_startup(void)
{
    mghw_simd_instruction_t instructions = mghw_detect_supported_simd_instructions();

    //if (instructions & MGHW_SIMD_INSTRUCTION__AVX2)
    //{
    //    *__mmsimd_get_arith64_handler_pointer() = (uintptr_t)mmsimd_get_avx2_arith64_handler();
    //    return;
    //}

    *__mmsimd_get_arith64_handler_pointer() = (uintptr_t)mmsimd_get_default_arith64_handler();
}

static mmsimd_arith8_hdlr_t* mmsimd_get_best_arith8_handler()
{
    mgthrd_call_once(__mmsimd_get_arith8_startup_once_flag(), __mmsimd_arith8_startup);
    return (mmsimd_arith8_hdlr_t*)*__mmsimd_get_arith8_handler_pointer();
}

static mmsimd_arith16_hdlr_t* mmsimd_get_best_arith16_handler()
{
    mgthrd_call_once(__mmsimd_get_arith16_startup_once_flag(), __mmsimd_arith16_startup);
    return (mmsimd_arith16_hdlr_t*)*__mmsimd_get_arith16_handler_pointer();
}

static mmsimd_arith32_hdlr_t* mmsimd_get_best_arith32_handler()
{
    mgthrd_call_once(__mmsimd_get_arith32_startup_once_flag(), __mmsimd_arith32_startup);
    return (mmsimd_arith32_hdlr_t*)*__mmsimd_get_arith32_handler_pointer();
}

static mmsimd_arith64_hdlr_t* mmsimd_get_best_arith64_handler()
{
    mgthrd_call_once(__mmsimd_get_arith64_startup_once_flag(), __mmsimd_arith64_startup);
    return (mmsimd_arith64_hdlr_t*)*__mmsimd_get_arith64_handler_pointer();
}

static mmsimd_conv8_hdlr_t* mmsimd_get_default_conv8_handler()
{
    static mmsimd_conv8_hdlr_t hdlr = {
        .i8_to_i16_fn = mmsimd_default_i8_to_i16,
        .i8_to_u16_fn = mmsimd_default_i8_to_u16,
        .i8_to_i32_fn = mmsimd_default_i8_to_i32,
        .i8_to_u32_fn = mmsimd_default_i8_to_u32,
        .i8_to_i64_fn = mmsimd_default_i8_to_i64,
        .i8_to_u64_fn = mmsimd_default_i8_to_u64,
        .i8_to_f32_fn = mmsimd_default_i8_to_f32,
        .i8_to_f64_fn = mmsimd_default_i8_to_f64,
        .u8_to_i16_fn = mmsimd_default_u8_to_i16,
        .u8_to_u16_fn = mmsimd_default_u8_to_u16,
        .u8_to_i32_fn = mmsimd_default_u8_to_i32,
        .u8_to_u32_fn = mmsimd_default_u8_to_u32,
        .u8_to_i64_fn = mmsimd_default_u8_to_i64,
        .u8_to_u64_fn = mmsimd_default_u8_to_u64,
        .u8_to_f32_fn = mmsimd_default_u8_to_f32,
        .u8_to_f64_fn = mmsimd_default_u8_to_f64
    };
    return &hdlr;
}

static mmsimd_conv16_hdlr_t* mmsimd_get_default_conv16_handler()
{
    static mmsimd_conv16_hdlr_t hdlr = {
        .i16_to_i8_fn  = mmsimd_default_i16_to_i8,
        .i16_to_u8_fn  = mmsimd_default_i16_to_u8,
        .i16_to_i32_fn = mmsimd_default_i16_to_i32,
        .i16_to_u32_fn = mmsimd_default_i16_to_u32,
        .i16_to_i64_fn = mmsimd_default_i16_to_i64,
        .i16_to_u64_fn = mmsimd_default_i16_to_u64,
        .i16_to_f32_fn = mmsimd_default_i16_to_f32,
        .i16_to_f64_fn = mmsimd_default_i16_to_f64,
        .u16_to_i8_fn  = mmsimd_default_u16_to_i8,
        .u16_to_u8_fn  = mmsimd_default_u16_to_u8,
        .u16_to_i32_fn = mmsimd_default_u16_to_i32,
        .u16_to_u32_fn = mmsimd_default_u16_to_u32,
        .u16_to_i64_fn = mmsimd_default_u16_to_i64,
        .u16_to_u64_fn = mmsimd_default_u16_to_u64,
        .u16_to_f32_fn = mmsimd_default_u16_to_f32,
        .u16_to_f64_fn = mmsimd_default_u16_to_f64
    };
    return &hdlr;
}

static mmsimd_conv32_hdlr_t* mmsimd_get_default_conv32_handler()
{
    static mmsimd_conv32_hdlr_t hdlr = {
        .i32_to_i8_fn  = mmsimd_default_i32_to_i8,
        .i32_to_u8_fn  = mmsimd_default_i32_to_u8,
        .i32_to_i16_fn = mmsimd_default_i32_to_i16,
        .i32_to_u16_fn = mmsimd_default_i32_to_u16,
        .i32_to_i64_fn = mmsimd_default_i32_to_i64,
        .i32_to_u64_fn = mmsimd_default_i32_to_u64,
        .i32_to_f32_fn = mmsimd_default_i32_to_f32,
        .i32_to_f64_fn = mmsimd_default_i32_to_f64,
        .u32_to_i8_fn  = mmsimd_default_u32_to_i8,
        .u32_to_u8_fn  = mmsimd_default_u32_to_u8,
        .u32_to_i16_fn = mmsimd_default_u32_to_i16,
        .u32_to_u16_fn = mmsimd_default_u32_to_u16,
        .u32_to_i64_fn = mmsimd_default_u32_to_i64,
        .u32_to_u64_fn = mmsimd_default_u32_to_u64,
        .u32_to_f32_fn = mmsimd_default_u32_to_f32,
        .u32_to_f64_fn = mmsimd_default_u32_to_f64
    };
    return &hdlr;
}

static mmsimd_conv64_hdlr_t* mmsimd_get_default_conv64_handler()
{
    static mmsimd_conv64_hdlr_t hdlr = {
        .i64_to_i8_fn  = mmsimd_default_i64_to_i8,
        .i64_to_u8_fn  = mmsimd_default_i64_to_u8,
        .i64_to_i16_fn = mmsimd_default_i64_to_i16,
        .i64_to_u16_fn = mmsimd_default_i64_to_u16,
        .i64_to_i32_fn = mmsimd_default_i64_to_i32,
        .i64_to_u32_fn = mmsimd_default_i64_to_u32,
        .i64_to_f32_fn = mmsimd_default_i64_to_f32,
        .i64_to_f64_fn = mmsimd_default_i64_to_f64,
        .u64_to_i8_fn  = mmsimd_default_u64_to_i8,
        .u64_to_u8_fn  = mmsimd_default_u64_to_u8,
        .u64_to_i16_fn = mmsimd_default_u64_to_i16,
        .u64_to_u16_fn = mmsimd_default_u64_to_u16,
        .u64_to_i32_fn = mmsimd_default_u64_to_i32,
        .u64_to_u32_fn = mmsimd_default_u64_to_u32,
        .u64_to_f32_fn = mmsimd_default_u64_to_f32,
        .u64_to_f64_fn = mmsimd_default_u64_to_f64
    };
    return &hdlr;
}

static mmsimd_conv8_hdlr_t* mmsimd_get_avx2_conv8_handler()
{
#if MEGO_ARCH__X86 || MEGO_ARCH__X64 
    static mmsimd_conv8_hdlr_t hdlr = {
        .i8_to_i16_fn = mmsimd_avx2_i8_to_i16,
        .i8_to_u16_fn = mmsimd_default_i8_to_u16,
        .i8_to_i32_fn = mmsimd_default_i8_to_i32,
        .i8_to_u32_fn = mmsimd_default_i8_to_u32,
        .i8_to_i64_fn = mmsimd_default_i8_to_i64,
        .i8_to_u64_fn = mmsimd_default_i8_to_u64,
        .i8_to_f32_fn = mmsimd_default_i8_to_f32,
        .i8_to_f64_fn = mmsimd_default_i8_to_f64,
        .u8_to_i16_fn = mmsimd_default_u8_to_i16,
        .u8_to_u16_fn = mmsimd_default_u8_to_u16,
        .u8_to_i32_fn = mmsimd_default_u8_to_i32,
        .u8_to_u32_fn = mmsimd_default_u8_to_u32,
        .u8_to_i64_fn = mmsimd_default_u8_to_i64,
        .u8_to_u64_fn = mmsimd_default_u8_to_u64,
        .u8_to_f32_fn = mmsimd_default_u8_to_f32,
        .u8_to_f64_fn = mmsimd_default_u8_to_f64
    };
    return &hdlr;
#else
    return mmsimd_get_default_conv8_handler();
#endif
}

static mgthrd_once_flag* __mmsimd_get_conv8_startup_once_flag()
{
    static mgthrd_once_flag f = MGTHRD_ONCE_FLAG_INIT;
    return &f;
}

static mgthrd_once_flag* __mmsimd_get_conv16_startup_once_flag()
{
    static mgthrd_once_flag f = MGTHRD_ONCE_FLAG_INIT;
    return &f;
}

static mgthrd_once_flag* __mmsimd_get_conv32_startup_once_flag()
{
    static mgthrd_once_flag f = MGTHRD_ONCE_FLAG_INIT;
    return &f;
}

static mgthrd_once_flag* __mmsimd_get_conv64_startup_once_flag()
{
    static mgthrd_once_flag f = MGTHRD_ONCE_FLAG_INIT;
    return &f;
}

static volatile uintptr_t* __mmsimd_get_conv8_handler_pointer()
{
    static volatile mmint_t pointer = 0;
    return &pointer;
}

static volatile uintptr_t* __mmsimd_get_conv16_handler_pointer()
{
    static volatile mmint_t pointer = 0;
    return &pointer;
}

static volatile uintptr_t* __mmsimd_get_conv32_handler_pointer()
{
    static volatile mmint_t pointer = 0;
    return &pointer;
}

static volatile uintptr_t* __mmsimd_get_conv64_handler_pointer()
{
    static volatile mmint_t pointer = 0;
    return &pointer;
}

static void __mmsimd_conv8_startup(void)
{
    mghw_simd_instruction_t instructions = mghw_detect_supported_simd_instructions();

    if (instructions & MGHW_SIMD_INSTRUCTION__AVX2)
    {
        *__mmsimd_get_conv8_handler_pointer() = (uintptr_t)mmsimd_get_avx2_conv8_handler();
        return;
    }

    *__mmsimd_get_conv8_handler_pointer() = (uintptr_t)mmsimd_get_default_conv8_handler();
}

static void __mmsimd_conv16_startup(void)
{
    mghw_simd_instruction_t instructions = mghw_detect_supported_simd_instructions();

    //if (instructions & MGHW_SIMD_INSTRUCTION__AVX2)
    //{
    //    *__mmsimd_get_conv16_handler_pointer() = (uintptr_t)mmsimd_get_avx2_conv16_handler();
    //    return;
    //}

    *__mmsimd_get_conv16_handler_pointer() = (uintptr_t)mmsimd_get_default_conv16_handler();
}

static void __mmsimd_conv32_startup(void)
{
    mghw_simd_instruction_t instructions = mghw_detect_supported_simd_instructions();

    //if (instructions & MGHW_SIMD_INSTRUCTION__AVX2)
    //{
    //    *__mmsimd_get_conv32_handler_pointer() = (uintptr_t)mmsimd_get_avx2_conv32_handler();
    //    return;
    //}

    *__mmsimd_get_conv32_handler_pointer() = (uintptr_t)mmsimd_get_default_conv32_handler();
}

static void __mmsimd_conv64_startup(void)
{
    mghw_simd_instruction_t instructions = mghw_detect_supported_simd_instructions();

    //if (instructions & MGHW_SIMD_INSTRUCTION__AVX2)
    //{
    //    *__mmsimd_get_conv64_handler_pointer() = (uintptr_t)mmsimd_get_avx2_conv64_handler();
    //    return;
    //}

    *__mmsimd_get_conv64_handler_pointer() = (uintptr_t)mmsimd_get_default_conv64_handler();
}

static mmsimd_conv8_hdlr_t* mmsimd_get_best_conv8_handler()
{
    mgthrd_call_once(__mmsimd_get_conv8_startup_once_flag(), __mmsimd_conv8_startup);
    return (mmsimd_conv8_hdlr_t*)*__mmsimd_get_conv8_handler_pointer();
}

static mmsimd_conv16_hdlr_t* mmsimd_get_best_conv16_handler()
{
    mgthrd_call_once(__mmsimd_get_conv16_startup_once_flag(), __mmsimd_conv16_startup);
    return (mmsimd_conv16_hdlr_t*)*__mmsimd_get_conv16_handler_pointer();
}

static mmsimd_conv32_hdlr_t* mmsimd_get_best_conv32_handler()
{
    mgthrd_call_once(__mmsimd_get_conv32_startup_once_flag(), __mmsimd_conv32_startup);
    return (mmsimd_conv32_hdlr_t*)*__mmsimd_get_conv32_handler_pointer();
}

static mmsimd_conv64_hdlr_t* mmsimd_get_best_conv64_handler()
{
    mgthrd_call_once(__mmsimd_get_conv64_startup_once_flag(), __mmsimd_conv64_startup);
    return (mmsimd_conv64_hdlr_t*)*__mmsimd_get_conv64_handler_pointer();
}

#define MMSIMD_ADD_FUNC(BIT, NAME, TYPE) \
    MEME_API void MEME_STDCALL mmsimd_##NAME##_add(const TYPE* _a, const TYPE* _b, TYPE* _out, mmint_t _len) \
    { \
        mmsimd_arith##BIT##_hdlr_t* hdlr = mmsimd_get_best_arith##BIT##_handler(); \
        hdlr->NAME##_add_fn(_a, _b, _out, _len); \
    } \
    MEME_API void MEME_STDCALL mmsimd_##NAME##_add_scalar(const TYPE* _a, TYPE _b, TYPE* _out, mmint_t _len) \
    { \
        mmsimd_arith##BIT##_hdlr_t* hdlr = mmsimd_get_best_arith##BIT##_handler(); \
        hdlr->NAME##_add_scalar_fn(_a, _b, _out, _len); \
    }

MMSIMD_ADD_FUNC(8, i8, int8_t)
MMSIMD_ADD_FUNC(8, u8, uint8_t)
MMSIMD_ADD_FUNC(16, i16, int16_t)
MMSIMD_ADD_FUNC(16, u16, uint16_t)
MMSIMD_ADD_FUNC(32, i32, int32_t)
MMSIMD_ADD_FUNC(32, u32, uint32_t)
MMSIMD_ADD_FUNC(64, i64, int64_t)
MMSIMD_ADD_FUNC(64, u64, uint64_t)
MMSIMD_ADD_FUNC(32, f32, float)
MMSIMD_ADD_FUNC(64, f64, double)

#undef MMSIMD_ADD_FUNC

#define MMSIMD_SUB_FUNC(BIT, NAME, TYPE) \
    MEME_API void MEME_STDCALL mmsimd_##NAME##_sub(const TYPE* _a, const TYPE* _b, TYPE* _out, mmint_t _len) \
    { \
        mmsimd_arith##BIT##_hdlr_t* hdlr = mmsimd_get_best_arith##BIT##_handler(); \
        hdlr->NAME##_sub_fn(_a, _b, _out, _len); \
    } \
    MEME_API void MEME_STDCALL mmsimd_##NAME##_sub_scalar(const TYPE* _a, TYPE _b, TYPE* _out, mmint_t _len) \
    { \
        mmsimd_arith##BIT##_hdlr_t* hdlr = mmsimd_get_best_arith##BIT##_handler(); \
        hdlr->NAME##_sub_scalar_fn(_a, _b, _out, _len); \
    }

MMSIMD_SUB_FUNC(8, i8, int8_t)
MMSIMD_SUB_FUNC(8, u8, uint8_t)
MMSIMD_SUB_FUNC(16, i16, int16_t)
MMSIMD_SUB_FUNC(16, u16, uint16_t)
MMSIMD_SUB_FUNC(32, i32, int32_t)
MMSIMD_SUB_FUNC(32, u32, uint32_t)
MMSIMD_SUB_FUNC(64, i64, int64_t)
MMSIMD_SUB_FUNC(64, u64, uint64_t)
MMSIMD_SUB_FUNC(32, f32, float)
MMSIMD_SUB_FUNC(64, f64, double)

#undef MMSIMD_SUB_FUNC

#define MMSIMD_MUL_FUNC(BIT, NAME, TYPE) \
    MEME_API void MEME_STDCALL mmsimd_##NAME##_mul(const TYPE* _a, const TYPE* _b, TYPE* _out, mmint_t _len) \
    { \
        mmsimd_arith##BIT##_hdlr_t* hdlr = mmsimd_get_best_arith##BIT##_handler(); \
        hdlr->NAME##_mul_fn(_a, _b, _out, _len); \
    } \
    MEME_API void MEME_STDCALL mmsimd_##NAME##_mul_scalar(const TYPE* _a, TYPE _b, TYPE* _out, mmint_t _len) \
    { \
        mmsimd_arith##BIT##_hdlr_t* hdlr = mmsimd_get_best_arith##BIT##_handler(); \
        hdlr->NAME##_mul_scalar_fn(_a, _b, _out, _len); \
    }

MMSIMD_MUL_FUNC(8, i8, int8_t)
MMSIMD_MUL_FUNC(8, u8, uint8_t)
MMSIMD_MUL_FUNC(16, i16, int16_t)
MMSIMD_MUL_FUNC(16, u16, uint16_t)
MMSIMD_MUL_FUNC(32, i32, int32_t)
MMSIMD_MUL_FUNC(32, u32, uint32_t)
MMSIMD_MUL_FUNC(64, i64, int64_t)
MMSIMD_MUL_FUNC(64, u64, uint64_t)
MMSIMD_MUL_FUNC(32, f32, float)
MMSIMD_MUL_FUNC(64, f64, double)

#undef MMSIMD_MUL_FUNC

#define MMSIMD_DIV_FUNC(BIT, NAME, TYPE) \
    MEME_API void MEME_STDCALL mmsimd_##NAME##_div(const TYPE* _a, const TYPE* _b, TYPE* _out, mmint_t _len) \
    { \
        mmsimd_arith##BIT##_hdlr_t* hdlr = mmsimd_get_best_arith##BIT##_handler(); \
        hdlr->NAME##_div_fn(_a, _b, _out, _len); \
    } \
    MEME_API void MEME_STDCALL mmsimd_##NAME##_div_scalar(const TYPE* _a, TYPE _b, TYPE* _out, mmint_t _len) \
    { \
        mmsimd_arith##BIT##_hdlr_t* hdlr = mmsimd_get_best_arith##BIT##_handler(); \
        hdlr->NAME##_div_scalar_fn(_a, _b, _out, _len); \
    }

MMSIMD_DIV_FUNC(8, i8, int8_t)
MMSIMD_DIV_FUNC(8, u8, uint8_t)
MMSIMD_DIV_FUNC(16, i16, int16_t)
MMSIMD_DIV_FUNC(16, u16, uint16_t)
MMSIMD_DIV_FUNC(32, i32, int32_t)
MMSIMD_DIV_FUNC(32, u32, uint32_t)
MMSIMD_DIV_FUNC(64, i64, int64_t)
MMSIMD_DIV_FUNC(64, u64, uint64_t)
MMSIMD_DIV_FUNC(32, f32, float)
MMSIMD_DIV_FUNC(64, f64, double)

#undef MMSIMD_DIV_FUNC

#define MMSIMD_CONV_FUNC(BIT, SRC_NAME, DST_NAME, SRC_TYPE, DST_TYPE) \
    MEME_API void MEME_STDCALL mmsimd_##SRC_NAME##_to_##DST_NAME(const SRC_TYPE* _in, DST_TYPE* _out, mmint_t _len) \
    { \
        mmsimd_conv##BIT##_hdlr_t* hdlr = mmsimd_get_best_conv##BIT##_handler(); \
        hdlr->SRC_NAME##_to_##DST_NAME##_fn(_in, _out, _len); \
    }

MMSIMD_CONV_FUNC(8, i8, i16, int8_t, int16_t)
MMSIMD_CONV_FUNC(8, i8, u16, int8_t, uint16_t)
MMSIMD_CONV_FUNC(8, i8, i32, int8_t, int32_t)
MMSIMD_CONV_FUNC(8, i8, u32, int8_t, uint32_t)
MMSIMD_CONV_FUNC(8, i8, i64, int8_t, int64_t)
MMSIMD_CONV_FUNC(8, i8, u64, int8_t, uint64_t)
MMSIMD_CONV_FUNC(8, i8, f32, int8_t, float)
MMSIMD_CONV_FUNC(8, i8, f64, int8_t, double)
MMSIMD_CONV_FUNC(8, u8, i16, uint8_t, int16_t)
MMSIMD_CONV_FUNC(8, u8, u16, uint8_t, uint16_t)
MMSIMD_CONV_FUNC(8, u8, i32, uint8_t, int32_t)
MMSIMD_CONV_FUNC(8, u8, u32, uint8_t, uint32_t)
MMSIMD_CONV_FUNC(8, u8, i64, uint8_t, int64_t)
MMSIMD_CONV_FUNC(8, u8, u64, uint8_t, uint64_t)
MMSIMD_CONV_FUNC(8, u8, f32, uint8_t, float)
MMSIMD_CONV_FUNC(8, u8, f64, uint8_t, double)
MMSIMD_CONV_FUNC(16, i16, i8, int16_t, int8_t)
MMSIMD_CONV_FUNC(16, i16, u8, int16_t, uint8_t)
MMSIMD_CONV_FUNC(16, i16, i32, int16_t, int32_t)
MMSIMD_CONV_FUNC(16, i16, u32, int16_t, uint32_t)
MMSIMD_CONV_FUNC(16, i16, i64, int16_t, int64_t)
MMSIMD_CONV_FUNC(16, i16, u64, int16_t, uint64_t)
MMSIMD_CONV_FUNC(16, i16, f32, int16_t, float)
MMSIMD_CONV_FUNC(16, i16, f64, int16_t, double)
MMSIMD_CONV_FUNC(16, u16, i8, uint16_t, int8_t)
MMSIMD_CONV_FUNC(16, u16, u8, uint16_t, uint8_t)
MMSIMD_CONV_FUNC(16, u16, i32, uint16_t, int32_t)
MMSIMD_CONV_FUNC(16, u16, u32, uint16_t, uint32_t)
MMSIMD_CONV_FUNC(16, u16, i64, uint16_t, int64_t)
MMSIMD_CONV_FUNC(16, u16, u64, uint16_t, uint64_t)
MMSIMD_CONV_FUNC(16, u16, f32, uint16_t, float)
MMSIMD_CONV_FUNC(16, u16, f64, uint16_t, double)
MMSIMD_CONV_FUNC(32, i32, i8, int32_t, int8_t)
MMSIMD_CONV_FUNC(32, i32, u8, int32_t, uint8_t)
MMSIMD_CONV_FUNC(32, i32, i16, int32_t, int16_t)
MMSIMD_CONV_FUNC(32, i32, u16, int32_t, uint16_t)
MMSIMD_CONV_FUNC(32, i32, i64, int32_t, int64_t)
MMSIMD_CONV_FUNC(32, i32, u64, int32_t, uint64_t)
MMSIMD_CONV_FUNC(32, i32, f32, int32_t, float)
MMSIMD_CONV_FUNC(32, i32, f64, int32_t, double)
MMSIMD_CONV_FUNC(32, u32, i8, uint32_t, int8_t)
MMSIMD_CONV_FUNC(32, u32, u8, uint32_t, uint8_t)
MMSIMD_CONV_FUNC(32, u32, i16, uint32_t, int16_t)
MMSIMD_CONV_FUNC(32, u32, u16, uint32_t, uint16_t)
MMSIMD_CONV_FUNC(32, u32, i64, uint32_t, int64_t)
MMSIMD_CONV_FUNC(32, u32, u64, uint32_t, uint64_t)
MMSIMD_CONV_FUNC(32, u32, f32, uint32_t, float)
MMSIMD_CONV_FUNC(32, u32, f64, uint32_t, double)
MMSIMD_CONV_FUNC(64, i64, i8, int64_t, int8_t)
MMSIMD_CONV_FUNC(64, i64, u8, int64_t, uint8_t)
MMSIMD_CONV_FUNC(64, i64, i16, int64_t, int16_t)
MMSIMD_CONV_FUNC(64, i64, u16, int64_t, uint16_t)
MMSIMD_CONV_FUNC(64, i64, i32, int64_t, int32_t)
MMSIMD_CONV_FUNC(64, i64, u32, int64_t, uint32_t)
MMSIMD_CONV_FUNC(64, i64, f32, int64_t, float)
MMSIMD_CONV_FUNC(64, i64, f64, int64_t, double)
MMSIMD_CONV_FUNC(64, u64, i8, uint64_t, int8_t)
MMSIMD_CONV_FUNC(64, u64, u8, uint64_t, uint8_t)
MMSIMD_CONV_FUNC(64, u64, i16, uint64_t, int16_t)
MMSIMD_CONV_FUNC(64, u64, u16, uint64_t, uint16_t)
MMSIMD_CONV_FUNC(64, u64, i32, uint64_t, int32_t)
MMSIMD_CONV_FUNC(64, u64, u32, uint64_t, uint32_t)
MMSIMD_CONV_FUNC(64, u64, f32, uint64_t, float)
MMSIMD_CONV_FUNC(64, u64, f64, uint64_t, double)
MMSIMD_CONV_FUNC(32, f32, i8, float, int8_t)
MMSIMD_CONV_FUNC(32, f32, u8, float, uint8_t)
MMSIMD_CONV_FUNC(32, f32, i16, float, int16_t)
MMSIMD_CONV_FUNC(32, f32, u16, float, uint16_t)
MMSIMD_CONV_FUNC(32, f32, i32, float, int32_t)
MMSIMD_CONV_FUNC(32, f32, u32, float, uint32_t)
MMSIMD_CONV_FUNC(32, f32, i64, float, int64_t)
MMSIMD_CONV_FUNC(32, f32, u64, float, uint64_t)
MMSIMD_CONV_FUNC(32, f32, f64, float, double)
MMSIMD_CONV_FUNC(64, f64, i8, double, int8_t)
MMSIMD_CONV_FUNC(64, f64, u8, double, uint8_t)
MMSIMD_CONV_FUNC(64, f64, i16, double, int16_t)
MMSIMD_CONV_FUNC(64, f64, u16, double, uint16_t)
MMSIMD_CONV_FUNC(64, f64, i32, double, int32_t)
MMSIMD_CONV_FUNC(64, f64, u32, double, uint32_t)
MMSIMD_CONV_FUNC(64, f64, i64, double, int64_t)
MMSIMD_CONV_FUNC(64, f64, u64, double, uint64_t)
MMSIMD_CONV_FUNC(64, f64, f32, double, float)


#undef MMSIMD_CONV_FUNC

MEME_EXTERN_C_SCOPE_ENDED
