
#include <immintrin.h>
#include <meme/string_fwd.h>
#include <mego/predef/compiler/visualc.h>
#include <mego/predef/symbol/inline.h>
#include <mego/predef/compiler/gcc.h>
#include <mego/predef/compiler/clang.h>

#if MG_COMP__GCC_AVAIL || MG_COMP__CLANG_AVAIL
#define MMSIMD_AVX2_TARGET_ATTRIBUTES        \
    __attribute__((target("avx2")))
#else
#define MMSIMD_AVX2_TARGET_ATTRIBUTES
#endif

#define MMSIMD_AVX2_I16_OFFSET (mmint_t)(sizeof(__m256i) / sizeof(int16_t))
#define MMSIMD_AVX2_I32_OFFSET (mmint_t)(sizeof(__m256i) / sizeof(int32_t))
#define MMSIMD_AVX2_I64_OFFSET (mmint_t)(sizeof(__m256i) / sizeof(int64_t))
#define MMSIMD_AVX2_U16_OFFSET (mmint_t)(sizeof(__m256i) / sizeof(uint16_t))
#define MMSIMD_AVX2_U32_OFFSET (mmint_t)(sizeof(__m256i) / sizeof(uint32_t))
#define MMSIMD_AVX2_U64_OFFSET (mmint_t)(sizeof(__m256i) / sizeof(uint64_t))
#define MMSIMD_AVX2_F32_OFFSET (mmint_t)(sizeof(__m256)  / sizeof(float))
#define MMSIMD_AVX2_F64_OFFSET (mmint_t)(sizeof(__m256d) / sizeof(double))

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_i8_add(
    const int8_t* _a, const int8_t* _b, int8_t* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + (mmint_t)sizeof(__m256i) <= _n; _i += (mmint_t)sizeof(__m256i))
    {
        __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
        __m256i vb = _mm256_loadu_si256((__m256i*) & _b[_i]);

        __m256i vr = _mm256_add_epi8(va, vb);

        _mm256_storeu_si256((__m256i*) & _c[_i], vr);
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_i8_add_scalar(
    const int8_t* _a, int8_t _b, int8_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= (mmint_t)sizeof(__m256i) + _i)
    {
        __m256i vb = _mm256_set1_epi8(_b);
        for (; _i + (mmint_t)sizeof(__m256i) <= _n; _i += (mmint_t)sizeof(__m256i))
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_add_epi8(va, vb);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_i8_sub(
    const int8_t* _a, const int8_t* _b, int8_t* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + (mmint_t)sizeof(__m256i) <= _n; _i += (mmint_t)sizeof(__m256i))
    {
        __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
        __m256i vb = _mm256_loadu_si256((__m256i*) & _b[_i]);

        __m256i vr = _mm256_sub_epi8(va, vb);

        _mm256_storeu_si256((__m256i*) & _c[_i], vr);
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_i8_sub_scalar(
    const int8_t* _a, int8_t _b, int8_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= (mmint_t)sizeof(__m256i) + _i)
    {
        __m256i vb = _mm256_set1_epi8(_b);
        for (; _i + (mmint_t)sizeof(__m256i) <= _n; _i += (mmint_t)sizeof(__m256i))
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_sub_epi8(va, vb);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}

#if MG_COMP__MSVC_AVAIL
MG_CAPI_INLINE mmint_t __mmsimd_avx2_i8_div(
    const int8_t* _a, const int8_t* _b, int8_t* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + (mmint_t)sizeof(__m256i) <= _n; _i += (mmint_t)sizeof(__m256i))
    {
        __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
        __m256i vb = _mm256_loadu_si256((__m256i*) & _b[_i]);

        __m256i vr = _mm256_div_epi8(va, vb);

        _mm256_storeu_si256((__m256i*) & _c[_i], vr);
    }
    return _i;
}
#endif

#if MG_COMP__MSVC_AVAIL
MG_CAPI_INLINE mmint_t __mmsimd_avx2_i8_div_scalar(
    const int8_t* _a, int8_t _b, int8_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= (mmint_t)sizeof(__m256i) + _i)
    {
        __m256i vb = _mm256_set1_epi8(_b);
        for (; _i + (mmint_t)sizeof(__m256i) <= _n; _i += (mmint_t)sizeof(__m256i))
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_div_epi8(va, vb);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}
#endif

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_i16_add(
    const int16_t* _a, const int16_t* _b, int16_t* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + MMSIMD_AVX2_I16_OFFSET <= _n; _i += MMSIMD_AVX2_I16_OFFSET)
    {
        __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
        __m256i vb = _mm256_loadu_si256((__m256i*) & _b[_i]);

        __m256i vr = _mm256_add_epi16(va, vb);

        _mm256_storeu_si256((__m256i*) & _c[_i], vr);
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_i16_add_scalar(
    const int16_t* _a, int16_t _b, int16_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_I16_OFFSET + _i)
    {
        __m256i vb = _mm256_set1_epi16(_b);
        for (; _i + MMSIMD_AVX2_I16_OFFSET <= _n; _i += MMSIMD_AVX2_I16_OFFSET)
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_add_epi16(va, vb);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_i16_sub(
    const int16_t* _a, const int16_t* _b, int16_t* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + MMSIMD_AVX2_I16_OFFSET <= _n; _i += MMSIMD_AVX2_I16_OFFSET)
    {
        __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
        __m256i vb = _mm256_loadu_si256((__m256i*) & _b[_i]);

        __m256i vr = _mm256_sub_epi16(va, vb);

        _mm256_storeu_si256((__m256i*) & _c[_i], vr);
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_i16_sub_scalar(
    const int16_t* _a, int16_t _b, int16_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_I16_OFFSET + _i)
    {
        __m256i vb = _mm256_set1_epi16(_b);
        for (; _i + MMSIMD_AVX2_I16_OFFSET <= _n; _i += MMSIMD_AVX2_I16_OFFSET)
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_sub_epi16(va, vb);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}

#if MG_COMP__MSVC_AVAIL
MG_CAPI_INLINE mmint_t __mmsimd_avx2_i16_div(
    const int16_t* _a, const int16_t* _b, int16_t* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + MMSIMD_AVX2_I16_OFFSET <= _n; _i += MMSIMD_AVX2_I16_OFFSET)
    {
        __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
        __m256i vb = _mm256_loadu_si256((__m256i*) & _b[_i]);

        __m256i vr = _mm256_div_epi16(va, vb);

        _mm256_storeu_si256((__m256i*) & _c[_i], vr);
    }
    return _i;
}
#endif

#if MG_COMP__MSVC_AVAIL
MG_CAPI_INLINE mmint_t __mmsimd_avx2_i16_div_scalar(
    const int16_t* _a, int16_t _b, int16_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_I16_OFFSET + _i)
    {
        __m256i vb = _mm256_set1_epi16(_b);
        for (; _i + MMSIMD_AVX2_I16_OFFSET <= _n; _i += MMSIMD_AVX2_I16_OFFSET)
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_div_epi16(va, vb);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}
#endif

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_i32_add(
    const int32_t* _a, const int32_t* _b, int32_t* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + MMSIMD_AVX2_I32_OFFSET <= _n; _i += MMSIMD_AVX2_I32_OFFSET)
    {
        __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
        __m256i vb = _mm256_loadu_si256((__m256i*) & _b[_i]);

        __m256i vr = _mm256_add_epi32(va, vb);

        _mm256_storeu_si256((__m256i*) & _c[_i], vr);
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_i32_add_scalar(
    const int32_t* _a, int32_t _b, int32_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_I32_OFFSET + _i)
    {
        __m256i vb = _mm256_set1_epi32(_b);
        for (; _i + MMSIMD_AVX2_I32_OFFSET <= _n; _i += MMSIMD_AVX2_I32_OFFSET)
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_add_epi32(va, vb);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_i32_sub(
    const int32_t* _a, const int32_t* _b, int32_t* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + MMSIMD_AVX2_I32_OFFSET <= _n; _i += MMSIMD_AVX2_I32_OFFSET)
    {
        __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
        __m256i vb = _mm256_loadu_si256((__m256i*) & _b[_i]);

        __m256i vr = _mm256_sub_epi32(va, vb);

        _mm256_storeu_si256((__m256i*) & _c[_i], vr);
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_i32_sub_scalar(
    const int32_t* _a, int32_t _b, int32_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_I32_OFFSET + _i)
    {
        __m256i vb = _mm256_set1_epi32(_b);
        for (; _i + MMSIMD_AVX2_I32_OFFSET <= _n; _i += MMSIMD_AVX2_I32_OFFSET)
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_sub_epi32(va, vb);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}

//MG_CAPI_INLINE mmint_t __mmsimd_avx2_i32_mul(
//    const int32_t* _a, const int32_t* _b, int32_t* _c, mmint_t _n)
//{
//    mmint_t i = 0;
//    for (; i + MMSIMD_AVX2_I32_OFFSET <= _n; i += MMSIMD_AVX2_I32_OFFSET)
//    {
//        __m256i va = _mm256_loadu_si256((__m256i*) & _a[i]);
//        __m256i vb = _mm256_loadu_si256((__m256i*) & _b[i]);
//
//        __m256i vr = _mm256_mul_epi32(va, vb);
//
//        _mm256_storeu_si256((__m256i*) & _c[i], vr);
//    }
//    return i;
//}
//
//MG_CAPI_INLINE mmint_t __mmsimd_avx2_i32_mul_scalar(
//    const int32_t* _a, int32_t _b, int32_t* _c, mmint_t _n)
//{
//    mmint_t i = 0;
//    if (_n >= MMSIMD_AVX2_I32_OFFSET)
//    {
//        __m256i vb = _mm256_set1_epi32(_b);
//        for (; i + MMSIMD_AVX2_I32_OFFSET <= _n; i += MMSIMD_AVX2_I32_OFFSET)
//        {
//            __m256i va = _mm256_loadu_si256((__m256i*) & _a[i]);
//            __m256i vr = _mm256_mul_epi32(va, vb);
//
//            _mm256_storeu_si256((__m256i*) & _c[i], vr);
//        }
//    }
//    return i;
//}

#if MG_COMP__MSVC_AVAIL
MG_CAPI_INLINE mmint_t __mmsimd_avx2_i32_div(
    const int32_t* _a, const int32_t* _b, int32_t* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + MMSIMD_AVX2_I32_OFFSET <= _n; _i += MMSIMD_AVX2_I32_OFFSET)
    {
        __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
        __m256i vb = _mm256_loadu_si256((__m256i*) & _b[_i]);

        __m256i vr = _mm256_div_epi32(va, vb);

        _mm256_storeu_si256((__m256i*) & _c[_i], vr);
    }
    return _i;
}
#endif

#if MG_COMP__MSVC_AVAIL
MG_CAPI_INLINE mmint_t __mmsimd_avx2_i32_div_scalar(
    const int32_t* _a, int32_t _b, int32_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_I32_OFFSET + _i)
    {
        __m256i vb = _mm256_set1_epi32(_b);
        for (; _i + MMSIMD_AVX2_I32_OFFSET <= _n; _i += MMSIMD_AVX2_I32_OFFSET)
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_div_epi32(va, vb);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}
#endif

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_i64_add(
    const int64_t* _a, const int64_t* _b, int64_t* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + MMSIMD_AVX2_I64_OFFSET <= _n; _i += MMSIMD_AVX2_I64_OFFSET)
    {
        __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
        __m256i vb = _mm256_loadu_si256((__m256i*) & _b[_i]);

        __m256i vr = _mm256_add_epi64(va, vb);

        _mm256_storeu_si256((__m256i*) & _c[_i], vr);
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_i64_add_scalar(
    const int64_t* _a, int64_t _b, int64_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_I64_OFFSET + _i)
    {
        __m256i vb = _mm256_set1_epi64x(_b);
        for (; _i + MMSIMD_AVX2_I64_OFFSET <= _n; _i += MMSIMD_AVX2_I64_OFFSET)
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_add_epi64(va, vb);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_i64_sub(
    const int64_t* _a, const int64_t* _b, int64_t* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + MMSIMD_AVX2_I64_OFFSET <= _n; _i += MMSIMD_AVX2_I64_OFFSET)
    {
        __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
        __m256i vb = _mm256_loadu_si256((__m256i*) & _b[_i]);

        __m256i vr = _mm256_sub_epi64(va, vb);

        _mm256_storeu_si256((__m256i*) & _c[_i], vr);
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_i64_sub_scalar(
    const int64_t* _a, int64_t _b, int64_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_I64_OFFSET + _i)
    {
        __m256i vb = _mm256_set1_epi64x(_b);
        for (; _i + MMSIMD_AVX2_I64_OFFSET <= _n; _i += MMSIMD_AVX2_I64_OFFSET)
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_sub_epi64(va, vb);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}

#if MG_COMP__MSVC_AVAIL
MG_CAPI_INLINE mmint_t __mmsimd_avx2_i64_div(
    const int64_t* _a, const int64_t* _b, int64_t* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + MMSIMD_AVX2_I64_OFFSET <= _n; _i += MMSIMD_AVX2_I64_OFFSET)
    {
        __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
        __m256i vb = _mm256_loadu_si256((__m256i*) & _b[_i]);

        __m256i vr = _mm256_div_epi64(va, vb);

        _mm256_storeu_si256((__m256i*) & _c[_i], vr);
    }
    return _i;
}
#endif

#if MG_COMP__MSVC_AVAIL
MG_CAPI_INLINE mmint_t __mmsimd_avx2_i64_div_scalar(
    const int64_t* _a, int64_t _b, int64_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_I64_OFFSET + _i)
    {
        __m256i vb = _mm256_set1_epi64x(_b);
        for (; _i + MMSIMD_AVX2_I64_OFFSET <= _n; _i += MMSIMD_AVX2_I64_OFFSET)
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_div_epi64(va, vb);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}
#endif

#if MG_COMP__MSVC_AVAIL 
MG_CAPI_INLINE mmint_t __mmsimd_avx2_u8_div(
    const uint8_t* _a, const uint8_t* _b, uint8_t* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + (mmint_t)sizeof(__m256i) <= _n; _i += (mmint_t)sizeof(__m256i))
    {
        __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
        __m256i vb = _mm256_loadu_si256((__m256i*) & _b[_i]);

        __m256i vr = _mm256_div_epu8(va, vb);

        _mm256_storeu_si256((__m256i*) & _c[_i], vr);
    }
    return _i;
}
#endif

#if MG_COMP__MSVC_AVAIL
MG_CAPI_INLINE mmint_t __mmsimd_avx2_u8_div_scalar(
    const uint8_t* _a, uint8_t _b, uint8_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= (mmint_t)sizeof(__m256i) + _i)
    {
        __m256i vb = _mm256_set1_epi8(_b);
        for (; _i + (mmint_t)sizeof(__m256i) <= _n; _i += (mmint_t)sizeof(__m256i))
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_div_epu8(va, vb);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}
#endif

#if MG_COMP__MSVC_AVAIL
MG_CAPI_INLINE mmint_t __mmsimd_avx2_u16_div(
    const uint16_t* _a, const uint16_t* _b, uint16_t* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + MMSIMD_AVX2_U16_OFFSET <= _n; _i += MMSIMD_AVX2_U16_OFFSET)
    {
        __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
        __m256i vb = _mm256_loadu_si256((__m256i*) & _b[_i]);

        __m256i vr = _mm256_div_epu16(va, vb);

        _mm256_storeu_si256((__m256i*) & _c[_i], vr);
    }
    return _i;
}
#endif

#if MG_COMP__MSVC_AVAIL
MG_CAPI_INLINE mmint_t __mmsimd_avx2_u16_div_scalar(
    const uint16_t* _a, uint16_t _b, uint16_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_U16_OFFSET + _i)
    {
        __m256i vb = _mm256_set1_epi16(_b);
        for (; _i + MMSIMD_AVX2_U16_OFFSET <= _n; _i += MMSIMD_AVX2_U16_OFFSET)
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_div_epu16(va, vb);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}
#endif

//MG_CAPI_INLINE mmint_t __mmsimd_avx2_u32_mul(
//    const uint32_t* _a, const uint32_t* _b, uint32_t* _c, mmint_t _n)
//{
//    mmint_t i = 0;
//    for (; i + MMSIMD_AVX2_U32_OFFSET <= _n; i += MMSIMD_AVX2_U32_OFFSET)
//    {
//        __m256i va = _mm256_loadu_si256((__m256i*) & _a[i]);
//        __m256i vb = _mm256_loadu_si256((__m256i*) & _b[i]);
//
//        __m256i vr = _mm256_mul_epu32(va, vb);
//
//        _mm256_storeu_si256((__m256i*) & _c[i], vr);
//    }
//    return i;
//}
//
//MG_CAPI_INLINE mmint_t __mmsimd_avx2_u32_mul_scalar(
//    const uint32_t* _a, uint32_t _b, uint32_t* _c, mmint_t _n)
//{
//    mmint_t i = 0;
//    if (_n >= MMSIMD_AVX2_U32_OFFSET)
//    {
//        __m256i vb = _mm256_set1_epi32(_b);
//        for (; i + MMSIMD_AVX2_U32_OFFSET <= _n; i += MMSIMD_AVX2_U32_OFFSET)
//        {
//            __m256i va = _mm256_loadu_si256((__m256i*) & _a[i]);
//            __m256i vr = _mm256_mul_epu32(va, vb);
//
//            _mm256_storeu_si256((__m256i*) & _c[i], vr);
//        }
//    }
//    return i;
//}

#if MG_COMP__MSVC_AVAIL
MG_CAPI_INLINE mmint_t __mmsimd_avx2_u32_div(
    const uint32_t* _a, const uint32_t* _b, uint32_t* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + MMSIMD_AVX2_U32_OFFSET <= _n; _i += MMSIMD_AVX2_U32_OFFSET)
    {
        __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
        __m256i vb = _mm256_loadu_si256((__m256i*) & _b[_i]);

        __m256i vr = _mm256_div_epu32(va, vb);

        _mm256_storeu_si256((__m256i*) & _c[_i], vr);
    }
    return _i;
}
#endif

#if MG_COMP__MSVC_AVAIL
MG_CAPI_INLINE mmint_t __mmsimd_avx2_u32_div_scalar(
    const uint32_t* _a, uint32_t _b, uint32_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_U32_OFFSET + _i)
    {
        __m256i vb = _mm256_set1_epi32(_b);
        for (; _i + MMSIMD_AVX2_U32_OFFSET <= _n; _i += MMSIMD_AVX2_U32_OFFSET)
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_div_epu32(va, vb);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}
#endif

#if MG_COMP__MSVC_AVAIL
MG_CAPI_INLINE mmint_t __mmsimd_avx2_u64_div(
    const uint64_t* _a, const uint64_t* _b, uint64_t* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + MMSIMD_AVX2_U64_OFFSET <= _n; _i += MMSIMD_AVX2_U64_OFFSET)
    {
        __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
        __m256i vb = _mm256_loadu_si256((__m256i*) & _b[_i]);

        __m256i vr = _mm256_div_epu64(va, vb);

        _mm256_storeu_si256((__m256i*) & _c[_i], vr);
    }
    return _i;
}
#endif

#if MG_COMP__MSVC_AVAIL
MG_CAPI_INLINE mmint_t __mmsimd_avx2_u64_div_scalar(
    const uint64_t* _a, uint64_t _b, uint64_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_U64_OFFSET + _i)
    {
        __m256i vb = _mm256_set1_epi64x(_b);
        for (; _i + MMSIMD_AVX2_U64_OFFSET <= _n; _i += MMSIMD_AVX2_U64_OFFSET)
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_div_epu64(va, vb);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}
#endif

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_f32_add(
    const float* _a, const float* _b, float* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + MMSIMD_AVX2_F32_OFFSET <= _n; _i += MMSIMD_AVX2_F32_OFFSET)
    {
        __m256 va = _mm256_loadu_ps(& _a[_i]);
        __m256 vb = _mm256_loadu_ps(& _b[_i]);

        __m256 vr = _mm256_add_ps(va, vb);

        _mm256_storeu_ps(& _c[_i], vr);
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_f32_add_scalar(
    const float* _a, float _b, float* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_F32_OFFSET + _i)
    {
        __m256 vb = _mm256_set1_ps(_b);
        for (; _i + MMSIMD_AVX2_F32_OFFSET <= _n; _i += MMSIMD_AVX2_F32_OFFSET)
        {
            __m256 va = _mm256_loadu_ps(& _a[_i]);
            __m256 vr = _mm256_add_ps(va, vb);

            _mm256_storeu_ps(& _c[_i], vr);
        }
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_f32_sub(
    const float* _a, const float* _b, float* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + MMSIMD_AVX2_F32_OFFSET <= _n; _i += MMSIMD_AVX2_F32_OFFSET)
    {
        __m256 va = _mm256_loadu_ps(& _a[_i]);
        __m256 vb = _mm256_loadu_ps(& _b[_i]);

        __m256 vr = _mm256_sub_ps(va, vb);

        _mm256_storeu_ps(& _c[_i], vr);
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_f32_sub_scalar(
    const float* _a, float _b, float* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_F32_OFFSET + _i)
    {
        __m256 vb = _mm256_set1_ps(_b);
        for (; _i + MMSIMD_AVX2_F32_OFFSET <= _n; _i += MMSIMD_AVX2_F32_OFFSET)
        {
            __m256 va = _mm256_loadu_ps(& _a[_i]);
            __m256 vr = _mm256_sub_ps(va, vb);

            _mm256_storeu_ps(& _c[_i], vr);
        }
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_f32_mul(
    const float* _a, const float* _b, float* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + MMSIMD_AVX2_F32_OFFSET <= _n; _i += MMSIMD_AVX2_F32_OFFSET)
    {
        __m256 va = _mm256_loadu_ps(& _a[_i]);
        __m256 vb = _mm256_loadu_ps(& _b[_i]);

        __m256 vr = _mm256_mul_ps(va, vb);

        _mm256_storeu_ps(& _c[_i], vr);
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_f32_mul_scalar(
    const float* _a, float _b, float* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_F32_OFFSET + _i)
    {
        __m256 vb = _mm256_set1_ps(_b);
        for (; _i + MMSIMD_AVX2_F32_OFFSET <= _n; _i += MMSIMD_AVX2_F32_OFFSET)
        {
            __m256 va = _mm256_loadu_ps(& _a[_i]);
            __m256 vr = _mm256_mul_ps(va, vb);

            _mm256_storeu_ps(& _c[_i], vr);
        }
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_f32_div(
    const float* _a, const float* _b, float* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + MMSIMD_AVX2_F32_OFFSET <= _n; _i += MMSIMD_AVX2_F32_OFFSET)
    {
        __m256 va = _mm256_loadu_ps(& _a[_i]);
        __m256 vb = _mm256_loadu_ps(& _b[_i]);

        __m256 vr = _mm256_div_ps(va, vb);

        _mm256_storeu_ps(& _c[_i], vr);
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_f32_div_scalar(
    const float* _a, float _b, float* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_F32_OFFSET + _i)
    {
        __m256 vb = _mm256_set1_ps(_b);
        for (; _i + MMSIMD_AVX2_F32_OFFSET <= _n; _i += MMSIMD_AVX2_F32_OFFSET)
        {
            __m256 va = _mm256_loadu_ps(& _a[_i]);
            __m256 vr = _mm256_div_ps(va, vb);

            _mm256_storeu_ps(& _c[_i], vr);
        }
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_f64_add(
    const double* _a, const double* _b, double* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + MMSIMD_AVX2_F64_OFFSET <= _n; _i += MMSIMD_AVX2_F64_OFFSET)
    {
        __m256d va = _mm256_loadu_pd(& _a[_i]);
        __m256d vb = _mm256_loadu_pd(& _b[_i]);

        __m256d vr = _mm256_add_pd(va, vb);

        _mm256_storeu_pd(& _c[_i], vr);
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_f64_add_scalar(
    const double* _a, double _b, double* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_F64_OFFSET + _i)
    {
        __m256d vb = _mm256_set1_pd(_b);
        for (; _i + MMSIMD_AVX2_F64_OFFSET <= _n; _i += MMSIMD_AVX2_F64_OFFSET)
        {
            __m256d va = _mm256_loadu_pd(& _a[_i]);
            __m256d vr = _mm256_add_pd(va, vb);

            _mm256_storeu_pd(& _c[_i], vr);
        }
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_f64_sub(
    const double* _a, const double* _b, double* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + MMSIMD_AVX2_F64_OFFSET <= _n; _i += MMSIMD_AVX2_F64_OFFSET)
    {
        __m256d va = _mm256_loadu_pd(& _a[_i]);
        __m256d vb = _mm256_loadu_pd(& _b[_i]);

        __m256d vr = _mm256_sub_pd(va, vb);

        _mm256_storeu_pd(& _c[_i], vr);
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_f64_sub_scalar(
    const double* _a, double _b, double* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_F64_OFFSET + _i)
    {
        __m256d vb = _mm256_set1_pd(_b);
        for (; _i + MMSIMD_AVX2_F64_OFFSET <= _n; _i += MMSIMD_AVX2_F64_OFFSET)
        {
            __m256d va = _mm256_loadu_pd(& _a[_i]);
            __m256d vr = _mm256_sub_pd(va, vb);

            _mm256_storeu_pd(& _c[_i], vr);
        }
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_f64_mul(
    const double* _a, const double* _b, double* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + MMSIMD_AVX2_F64_OFFSET <= _n; _i += MMSIMD_AVX2_F64_OFFSET)
    {
        __m256d va = _mm256_loadu_pd(& _a[_i]);
        __m256d vb = _mm256_loadu_pd(& _b[_i]);

        __m256d vr = _mm256_mul_pd(va, vb);

        _mm256_storeu_pd(& _c[_i], vr);
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_f64_mul_scalar(
    const double* _a, double _b, double* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_F64_OFFSET + _i)
    {
        __m256d vb = _mm256_set1_pd(_b);
        for (; _i + MMSIMD_AVX2_F64_OFFSET <= _n; _i += MMSIMD_AVX2_F64_OFFSET)
        {
            __m256d va = _mm256_loadu_pd(& _a[_i]);
            __m256d vr = _mm256_mul_pd(va, vb);

            _mm256_storeu_pd(& _c[_i], vr);
        }
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_f64_div(
    const double* _a, const double* _b, double* _c, mmint_t _i, mmint_t _n)
{
    for (; _i + MMSIMD_AVX2_F64_OFFSET <= _n; _i += MMSIMD_AVX2_F64_OFFSET)
    {
        __m256d va = _mm256_loadu_pd(& _a[_i]);
        __m256d vb = _mm256_loadu_pd(& _b[_i]);

        __m256d vr = _mm256_div_pd(va, vb);

        _mm256_storeu_pd(& _c[_i], vr);
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_f64_div_scalar(
    const double* _a, double _b, double* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_F64_OFFSET + _i)
    {
        __m256d vb = _mm256_set1_pd(_b);
        for (; _i + MMSIMD_AVX2_F64_OFFSET <= _n; _i += MMSIMD_AVX2_F64_OFFSET)
        {
            __m256d va = _mm256_loadu_pd(& _a[_i]);
            __m256d vr = _mm256_div_pd(va, vb);

            _mm256_storeu_pd(& _c[_i], vr);
        }
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_i8_clamp(
    const int8_t* _a, int8_t _min, int8_t _max, int8_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= (mmint_t)sizeof(__m256i) + _i)
    {
        __m256i vmin = _mm256_set1_epi8(_min);
        __m256i vmax = _mm256_set1_epi8(_max);
        for (; _i + (mmint_t)sizeof(__m256i) <= _n; _i += (mmint_t)sizeof(__m256i))
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_min_epi8(_mm256_max_epi8(va, vmin), vmax);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_u8_clamp(
    const uint8_t* _a, uint8_t _min, uint8_t _max, uint8_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= (mmint_t)sizeof(__m256i) + _i)
    {
        __m256i vmin = _mm256_set1_epi8(_min);
        __m256i vmax = _mm256_set1_epi8(_max);
        for (; _i + (mmint_t)sizeof(__m256i) <= _n; _i += (mmint_t)sizeof(__m256i))
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_min_epu8(_mm256_max_epu8(va, vmin), vmax);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_i16_clamp(
    const int16_t* _a, int16_t _min, int16_t _max, int16_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_I16_OFFSET + _i)
    {
        __m256i vmin = _mm256_set1_epi16(_min);
        __m256i vmax = _mm256_set1_epi16(_max);
        for (; _i + MMSIMD_AVX2_I16_OFFSET <= _n; _i += MMSIMD_AVX2_I16_OFFSET)
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_min_epi16(_mm256_max_epi16(va, vmin), vmax);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_u16_clamp(
    const uint16_t* _a, uint16_t _min, uint16_t _max, uint16_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_U16_OFFSET + _i)
    {
        __m256i vmin = _mm256_set1_epi16(_min);
        __m256i vmax = _mm256_set1_epi16(_max);
        for (; _i + MMSIMD_AVX2_U16_OFFSET <= _n; _i += MMSIMD_AVX2_U16_OFFSET)
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_min_epu16(_mm256_max_epu16(va, vmin), vmax);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_i32_clamp(
    const int32_t* _a, int32_t _min, int32_t _max, int32_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_I32_OFFSET + _i)
    {
        __m256i vmin = _mm256_set1_epi32(_min);
        __m256i vmax = _mm256_set1_epi32(_max);
        for (; _i + MMSIMD_AVX2_I32_OFFSET <= _n; _i += MMSIMD_AVX2_I32_OFFSET)
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_min_epi32(_mm256_max_epi32(va, vmin), vmax);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_u32_clamp(
    const uint32_t* _a, uint32_t _min, uint32_t _max, uint32_t* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_U32_OFFSET + _i)
    {
        __m256i vmin = _mm256_set1_epi32(_min);
        __m256i vmax = _mm256_set1_epi32(_max);
        for (; _i + MMSIMD_AVX2_U32_OFFSET <= _n; _i += MMSIMD_AVX2_U32_OFFSET)
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[_i]);
            __m256i vr = _mm256_min_epu32(_mm256_max_epu32(va, vmin), vmax);

            _mm256_storeu_si256((__m256i*) & _c[_i], vr);
        }
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_f32_clamp(
    const float* _a, float _min, float _max, float* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_F32_OFFSET + _i)
    {
        __m256 vmin = _mm256_set1_ps(_min);
        __m256 vmax = _mm256_set1_ps(_max);
        for (; _i + MMSIMD_AVX2_F32_OFFSET <= _n; _i += MMSIMD_AVX2_F32_OFFSET)
        {
            __m256 va = _mm256_loadu_ps(& _a[_i]);
            __m256 vr = _mm256_min_ps(_mm256_max_ps(va, vmin), vmax);

            _mm256_storeu_ps(& _c[_i], vr);
        }
    }
    return _i;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_f64_clamp(
    const double* _a, double _min, double _max, double* _c, mmint_t _i, mmint_t _n)
{
    if (_n >= MMSIMD_AVX2_F64_OFFSET + _i)
    {
        __m256d vmin = _mm256_set1_pd(_min);
        __m256d vmax = _mm256_set1_pd(_max);
        for (; _i + MMSIMD_AVX2_F64_OFFSET <= _n; _i += MMSIMD_AVX2_F64_OFFSET)
        {
            __m256d va = _mm256_loadu_pd(& _a[_i]);
            __m256d vr = _mm256_min_pd(_mm256_max_pd(va, vmin), vmax);

            _mm256_storeu_pd(& _c[_i], vr);
        }
    }
    return _i;
}

void mmsimd_avx2_i8_add(const int8_t* _a, const int8_t* _b, int8_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i8_add(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b[i];
}

void mmsimd_avx2_i8_add_scalar(const int8_t* _a, int8_t _b, int8_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i8_add_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b;
}

void mmsimd_avx2_i8_sub(const int8_t* _a, const int8_t* _b, int8_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i8_sub(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b[i];
}

void mmsimd_avx2_i8_sub_scalar(const int8_t* _a, int8_t _b, int8_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i8_sub_scalar(_a, _b, _c, 0, _n);
    
    for (; i < _n; ++i)
        _c[i] = _a[i] - _b;
}

#if MG_COMP__MSVC_AVAIL
void mmsimd_avx2_i8_div(const int8_t* _a, const int8_t* _b, int8_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i8_div(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] / _b[i];
}
#endif

#if MG_COMP__MSVC_AVAIL
void mmsimd_avx2_i8_div_scalar(const int8_t* _a, int8_t _b, int8_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i8_sub_scalar(_a, _b, _c, 0, _n);
    
    for (; i < _n; ++i)
        _c[i] = _a[i] / _b;
}
#endif

void mmsimd_avx2_u8_add(const uint8_t* _a, const uint8_t* _b, uint8_t* _c, mmint_t _n)
{
    mmsimd_avx2_i8_add((const int8_t*)_a, (const int8_t*)_b, (int8_t*)_c, _n);
}

void mmsimd_avx2_u8_add_scalar(const uint8_t* _a, uint8_t _b, uint8_t* _c, mmint_t _n)
{
    mmsimd_avx2_i8_add_scalar((const int8_t*)_a, (int8_t)_b, (int8_t*)_c, _n);
}

void mmsimd_avx2_u8_sub(const uint8_t* _a, const uint8_t* _b, uint8_t* _c, mmint_t _n)
{
    mmsimd_avx2_i8_sub((const int8_t*)_a, (const int8_t*)_b, (int8_t*)_c, _n);
}

void mmsimd_avx2_u8_sub_scalar(const uint8_t* _a, uint8_t _b, uint8_t* _c, mmint_t _n)
{
    mmsimd_avx2_i8_sub_scalar((const int8_t*)_a, (int8_t)_b, (int8_t*)_c, _n);
}

#if MG_COMP__MSVC_AVAIL
void mmsimd_avx2_u8_div(const uint8_t* _a, const uint8_t* _b, uint8_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_u8_div(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] / _b[i];
}
#endif

#if MG_COMP__MSVC_AVAIL
void mmsimd_avx2_u8_div_scalar(const uint8_t* _a, uint8_t _b, uint8_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_u8_div_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] / _b;
}
#endif

void mmsimd_avx2_i16_add(const int16_t* _a, const int16_t* _b, int16_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i16_add(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b[i];

}

void mmsimd_avx2_i16_add_scalar(const int16_t* _a, int16_t _b, int16_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i16_add_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b;
}

void mmsimd_avx2_i16_sub(const int16_t* _a, const int16_t* _b, int16_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i16_sub(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b[i];
}

void mmsimd_avx2_i16_sub_scalar(const int16_t* _a, int16_t _b, int16_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i16_sub_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b;
}

#if MG_COMP__MSVC_AVAIL
void mmsimd_avx2_i16_div(const int16_t* _a, const int16_t* _b, int16_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i16_div(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] / _b[i];
}
#endif

#if MG_COMP__MSVC_AVAIL
void mmsimd_avx2_i16_div_scalar(const int16_t* _a, int16_t _b, int16_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i16_div_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] / _b;
}
#endif

void mmsimd_avx2_u16_add(const uint16_t* _a, const uint16_t* _b, uint16_t* _c, mmint_t _n)
{
    mmsimd_avx2_i16_add((const int16_t*)_a, (const int16_t*)_b, (int16_t*)_c, _n);
}

void mmsimd_avx2_u16_add_scalar(const uint16_t* _a, uint16_t _b, uint16_t* _c, mmint_t _n)
{
    mmsimd_avx2_i16_add_scalar((const int16_t*)_a, (int16_t)_b, (int16_t*)_c, _n);
}

void mmsimd_avx2_u16_sub(const uint16_t* _a, const uint16_t* _b, uint16_t* _c, mmint_t _n)
{
    mmsimd_avx2_i16_sub((const int16_t*)_a, (const int16_t*)_b, (int16_t*)_c, _n);
}

void mmsimd_avx2_u16_sub_scalar(const uint16_t* _a, uint16_t _b, uint16_t* _c, mmint_t _n)
{
    mmsimd_avx2_i16_sub_scalar((const int16_t*)_a, (int16_t)_b, (int16_t*)_c, _n);
}

#if MG_COMP__MSVC_AVAIL
void mmsimd_avx2_u16_div(const uint16_t* _a, const uint16_t* _b, uint16_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_u16_div(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] / _b[i];
}
#endif

#if MG_COMP__MSVC_AVAIL
void mmsimd_avx2_u16_div_scalar(const uint16_t* _a, uint16_t _b, uint16_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_u16_div_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] / _b;
}
#endif

void mmsimd_avx2_i32_add(const int32_t* _a, const int32_t* _b, int32_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i32_add(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b[i];
}

void mmsimd_avx2_i32_add_scalar(const int32_t* _a, int32_t _b, int32_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i32_add_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b;
}

void mmsimd_avx2_i32_sub(const int32_t* _a, const int32_t* _b, int32_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i32_sub(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b[i];
}

void mmsimd_avx2_i32_sub_scalar(const int32_t* _a, int32_t _b, int32_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i32_sub_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b;
}

//void mmsimd_avx2_i32_mul(const int32_t* _a, const int32_t* _b, int32_t* _c, mmint_t _n)
//{
//    mmint_t i = __mmsimd_avx2_i32_mul(_a, _b, _c, _n);
//
//    for (; i < _n; ++i)
//        _c[i] = _a[i] * _b[i];
//}
//
//void mmsimd_avx2_i32_mul_scalar(const int32_t* _a, int32_t _b, int32_t* _c, mmint_t _n)
//{
//    mmint_t i = __mmsimd_avx2_i32_mul_scalar(_a, _b, _c, _n);
//
//    for (; i < _n; ++i)
//        _c[i] = _a[i] * _b;
//}

#if MG_COMP__MSVC_AVAIL
void mmsimd_avx2_i32_div(const int32_t* _a, const int32_t* _b, int32_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i32_div(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] / _b[i];
}
#endif

#if MG_COMP__MSVC_AVAIL
void mmsimd_avx2_i32_div_scalar(const int32_t* _a, int32_t _b, int32_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i32_div_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] / _b;
}
#endif

void mmsimd_avx2_u32_add(const uint32_t* _a, const uint32_t* _b, uint32_t* _c, mmint_t _n)
{
    mmsimd_avx2_i32_add((const int32_t*)_a, (const int32_t*)_b, (int32_t*)_c, _n);
}

void mmsimd_avx2_u32_add_scalar(const uint32_t* _a, uint32_t _b, uint32_t* _c, mmint_t _n)
{
    mmsimd_avx2_i32_add_scalar((const int32_t*)_a, (int32_t)_b, (int32_t*)_c, _n);
}

void mmsimd_avx2_u32_sub(const uint32_t* _a, const uint32_t* _b, uint32_t* _c, mmint_t _n)
{
    mmsimd_avx2_i32_sub((const int32_t*)_a, (const int32_t*)_b, (int32_t*)_c, _n);
}

void mmsimd_avx2_u32_sub_scalar(const uint32_t* _a, uint32_t _b, uint32_t* _c, mmint_t _n)
{
    mmsimd_avx2_i32_sub_scalar((const int32_t*)_a, (int32_t)_b, (int32_t*)_c, _n);
}

//void mmsimd_avx2_u32_mul(const uint32_t* _a, const uint32_t* _b, uint32_t* _c, mmint_t _n)
//{
//    mmint_t i = __mmsimd_avx2_u32_mul(_a, _b, _c, _n);
//
//    for (; i < _n; ++i)
//        _c[i] = _a[i] * _b[i];
//}
//
//void mmsimd_avx2_u32_mul_scalar(const uint32_t* _a, uint32_t _b, uint32_t* _c, mmint_t _n)
//{
//    mmint_t i = __mmsimd_avx2_u32_mul_scalar(_a, _b, _c, _n);
//
//    for (; i < _n; ++i)
//        _c[i] = _a[i] * _b;
//}

#if MG_COMP__MSVC_AVAIL
void mmsimd_avx2_u32_div(const uint32_t* _a, const uint32_t* _b, uint32_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_u32_div(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] / _b[i];
}
#endif

#if MG_COMP__MSVC_AVAIL
void mmsimd_avx2_u32_div_scalar(const uint32_t* _a, uint32_t _b, uint32_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_u32_div_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] / _b;
}
#endif

void mmsimd_avx2_i64_add(const int64_t* _a, const int64_t* _b, int64_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i64_add(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b[i];
}

void mmsimd_avx2_i64_add_scalar(const int64_t* _a, int64_t _b, int64_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i64_add_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b;
}

void mmsimd_avx2_i64_sub(const int64_t* _a, const int64_t* _b, int64_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i64_sub(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b[i];
}

void mmsimd_avx2_i64_sub_scalar(const int64_t* _a, int64_t _b, int64_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i64_sub_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b;
}

#if MG_COMP__MSVC_AVAIL
void mmsimd_avx2_i64_div(const int64_t* _a, const int64_t* _b, int64_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i64_div(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] / _b[i];
}
#endif

#if MG_COMP__MSVC_AVAIL
void mmsimd_avx2_i64_div_scalar(const int64_t* _a, int64_t _b, int64_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i64_div_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] / _b;
}
#endif

void mmsimd_avx2_u64_add(const uint64_t* _a, const uint64_t* _b, uint64_t* _c, mmint_t _n)
{
    mmsimd_avx2_i64_add((const int64_t*)_a, (const int64_t*)_b, (int64_t*)_c, _n);
}

void mmsimd_avx2_u64_add_scalar(const uint64_t* _a, uint64_t _b, uint64_t* _c, mmint_t _n)
{
    mmsimd_avx2_i64_add_scalar((const int64_t*)_a, (int64_t)_b, (int64_t*)_c, _n);
}

void mmsimd_avx2_u64_sub(const uint64_t* _a, const uint64_t* _b, uint64_t* _c, mmint_t _n)
{
    mmsimd_avx2_i64_sub((const int64_t*)_a, (const int64_t*)_b, (int64_t*)_c, _n);
}

void mmsimd_avx2_u64_sub_scalar(const uint64_t* _a, uint64_t _b, uint64_t* _c, mmint_t _n)
{
    mmsimd_avx2_i64_sub_scalar((const int64_t*)_a, (int64_t)_b, (int64_t*)_c, _n);
}

#if MG_COMP__MSVC_AVAIL
void mmsimd_avx2_u64_div(const uint64_t* _a, const uint64_t* _b, uint64_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_u64_div(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] / _b[i];
}
#endif

#if MG_COMP__MSVC_AVAIL
void mmsimd_avx2_u64_div_scalar(const uint64_t* _a, uint64_t _b, uint64_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_u64_div_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] / _b;
}
#endif

void mmsimd_avx2_f32_add(const float* _a, const float* _b, float* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_f32_add(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b[i];
}

void mmsimd_avx2_f32_add_scalar(const float* _a, float _b, float* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_f32_add_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b;
}

void mmsimd_avx2_f32_sub(const float* _a, const float* _b, float* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_f32_sub(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b[i];
}

void mmsimd_avx2_f32_sub_scalar(const float* _a, float _b, float* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_f32_sub_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b;
}

void mmsimd_avx2_f32_mul(const float* _a, const float* _b, float* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_f32_mul(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] * _b[i];
}

void mmsimd_avx2_f32_mul_scalar(const float* _a, float _b, float* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_f32_mul_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] * _b;
}

void mmsimd_avx2_f32_div(const float* _a, const float* _b, float* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_f32_div(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] / _b[i];
}

void mmsimd_avx2_f32_div_scalar(const float* _a, float _b, float* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_f32_div_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] / _b;
}

void mmsimd_avx2_f64_add(const double* _a, const double* _b, double* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_f64_add(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b[i];
}

void mmsimd_avx2_f64_add_scalar(const double* _a, double _b, double* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_f64_add_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b;
}

void mmsimd_avx2_f64_sub(const double* _a, const double* _b, double* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_f64_sub(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b[i];
}

void mmsimd_avx2_f64_sub_scalar(const double* _a, double _b, double* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_f64_sub_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b;
}

void mmsimd_avx2_f64_mul(const double* _a, const double* _b, double* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_f64_mul(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] * _b[i];
}

void mmsimd_avx2_f64_mul_scalar(const double* _a, double _b, double* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_f64_mul_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] * _b;
}

void mmsimd_avx2_f64_div(const double* _a, const double* _b, double* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_f64_div(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] / _b[i];
}

void mmsimd_avx2_f64_div_scalar(const double* _a, double _b, double* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_f64_div_scalar(_a, _b, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = _a[i] / _b;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t __mmsimd_avx2_i8_to_i16(const int8_t* _in, int16_t* _out, mmint_t _n)
{
    mmint_t idx = 0;
    for (; idx + (mmint_t)sizeof(__m256i) <= _n; idx += (mmint_t)sizeof(__m256i))
    {
        __m256i vi = _mm256_loadu_si256((__m256i*) & _in[idx]);
        __m128i vi_l = _mm256_extracti128_si256(vi, 0);
        __m128i vi_h = _mm256_extracti128_si256(vi, 1);
        __m256i vo;

        vo = _mm256_cvtepi8_epi16(vi_l);
        _mm256_storeu_si256((__m256i*) & _out[idx], vo);
        vo = _mm256_cvtepi8_epi16(vi_h);
        _mm256_storeu_si256((__m256i*) & _out[idx + 16], vo);
    }

    return idx;
}

void mmsimd_avx2_i8_to_i16(const int8_t* _in, int16_t* _out, mmint_t _n)
{
    mmint_t idx = __mmsimd_avx2_i8_to_i16(_in, _out, _n);

    for (; idx < _n; ++idx)
        _out[idx] = _in[idx];
}

void mmsimd_avx2_i8_to_u16(const int8_t* _in, uint16_t* _out, mmint_t _n)
{
    mmsimd_avx2_i8_to_i16(_in, (int16_t*)_out, _n);
}

//! _mm_loadu_si128 and _mm_srli_si128 is SSE2 instruction
//void mmsimd_sse2_avx2_i8_to_i32(const int8_t* _in, int32_t* _out, mmint_t _n)
//{
//    mmint_t idx = 0;
//    for (; idx + (mmint_t)sizeof(__m128i) <= _n; idx += (mmint_t)sizeof(__m128i))
//    {
//        __m128i vi_l = _mm_loadu_si128((__m128i*)&_in[idx]);
//        __m128i vi_h = _mm_srli_si128(vi_l, 8);
//        __m256i vo;
//
//        vo = _mm256_cvtepi8_epi32(vi_l);
//        _mm256_storeu_si256((__m256i*)&_out[idx], vo);
//
//        vo = _mm256_cvtepi8_epi32(vi_h);
//        _mm256_storeu_si256((__m256i*)&_out[idx + 8], vo);
//    }
//
//    for (; idx < _n; ++idx)
//        _out[idx] = _in[idx];
//}

//void mmsimd_sse2_avx2_i8_to_u32(const int8_t* _in, uint32_t* _out, mmint_t _n)
//{
//    mmsimd_sse2_avx2_i8_to_i32(_in, (int32_t*)_out, _n);
//}

//! _mm_loadu_si128 and _mm_srli_si128 is SSE2 instruction
//void mmsimd_sse2_avx2_i8_to_i64(const int8_t* _in, int64_t* _out, mmint_t _n)
//{
//    mmint_t idx = 0;
//    for (; idx + (mmint_t)sizeof(__m128i) <= _n; idx += (mmint_t)sizeof(__m128i))
//    {
//        __m128i vi_1 = _mm_loadu_si128((__m128i*)&_in[idx]);
//        __m128i vi_2 = _mm_srli_si128(vi_1, 4);
//        __m128i vi_3 = _mm_srli_si128(vi_1, 8);
//        __m128i vi_4 = _mm_srli_si128(vi_1, 12);
//        __m256i vo;
//
//        vo = _mm256_cvtepi8_epi64(vi_1);
//        _mm256_storeu_si256((__m256i*)&_out[idx], vo);
//
//        vo = _mm256_cvtepi8_epi64(vi_2);
//        _mm256_storeu_si256((__m256i*)&_out[idx + 4], vo);
//
//        vo = _mm256_cvtepi8_epi64(vi_3);
//        _mm256_storeu_si256((__m256i*)&_out[idx + 8], vo);
//
//        vo = _mm256_cvtepi8_epi64(vi_4);
//        _mm256_storeu_si256((__m256i*)&_out[idx + 12], vo);
//    }
//
//    for (; idx < _n; ++idx)
//        _out[idx] = _in[idx];
//}
//
//void mmsimd_sse2_avx2_i8_to_u64(const int8_t* _in, uint64_t* _out, mmint_t _n)
//{
//    mmsimd_sse2_avx2_i8_to_i64(_in, (int64_t*)_out, _n);
//}

//! _mm_loadu_si128 and _mm_srli_si128 is SSE2 instruction
//void mmsimd_sse2_avx2_i8_to_f32(const int8_t* _in, float* _out, mmint_t _n)
//{
//    mmint_t idx = 0;
//    for (; idx + (mmint_t)sizeof(__m256) <= _n; idx += (mmint_t)sizeof(__m256))
//    {
//        __m128i vi_l = _mm_loadu_si128((__m128i*) & _in[idx]);
//        __m128i vi_h = _mm_srli_si128(vi_l, 8);
//        __m256 vo;
//
//        vo = _mm256_cvtepi32_ps(_mm256_cvtepi8_epi32(vi_l));
//        _mm256_storeu_ps(&_out[idx], vo);
//
//        vo = _mm256_cvtepi32_ps(_mm256_cvtepi8_epi32(vi_h));
//        _mm256_storeu_ps(&_out[idx + 8], vo);
//    }
//
//    for (; idx < _n; ++idx)
//        _out[idx] = (float)_in[idx];
//}

MMSIMD_AVX2_TARGET_ATTRIBUTES
void mmsimd_avx2_i16_fill(int16_t* _out, mmint_t _len, int16_t _val)
{
    mmint_t idx = 0;
    if (_len >= MMSIMD_AVX2_I16_OFFSET)
    {
        __m256i vval = _mm256_set1_epi16(_val);
        for (; idx + MMSIMD_AVX2_I16_OFFSET <= _len; idx += MMSIMD_AVX2_I16_OFFSET)
            _mm256_storeu_si256((__m256i*)&_out[idx], vval);
    }

    for (; idx < _len; ++idx)
        _out[idx] = _val;
}

void mmsimd_avx2_u16_fill(uint16_t* _out, mmint_t _len, uint16_t _val)
{
    mmsimd_avx2_i16_fill((int16_t*)_out, _len, (int16_t)_val);
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
void mmsimd_avx2_i32_fill(int32_t* _out, mmint_t _len, int32_t _val)
{
    mmint_t idx = 0;
    if (_len >= MMSIMD_AVX2_I32_OFFSET)
    {
        __m256i vval = _mm256_set1_epi32(_val);
        for (; idx + MMSIMD_AVX2_I32_OFFSET <= _len; idx += MMSIMD_AVX2_I32_OFFSET)
            _mm256_storeu_si256((__m256i*)&_out[idx], vval);
    }

    for (; idx < _len; ++idx)
        _out[idx] = _val;
}

void mmsimd_avx2_u32_fill(uint32_t* _out, mmint_t _len, uint32_t _val)
{
    mmsimd_avx2_i32_fill((int32_t*)_out, _len, (int32_t)_val);
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
void mmsimd_avx2_i64_fill(int64_t* _out, mmint_t _len, int64_t _val)
{
    mmint_t idx = 0;
    if (_len >= MMSIMD_AVX2_I64_OFFSET)
    {
        __m256i vval = _mm256_set1_epi64x(_val);
        for (; idx + MMSIMD_AVX2_I64_OFFSET <= _len; idx += MMSIMD_AVX2_I64_OFFSET)
            _mm256_storeu_si256((__m256i*)&_out[idx], vval);
    }

    for (; idx < _len; ++idx)
        _out[idx] = _val;
}

void mmsimd_avx2_u64_fill(uint64_t* _out, mmint_t _len, uint64_t _val)
{
    mmsimd_avx2_i64_fill((int64_t*)_out, _len, (int64_t)_val);
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
void mmsimd_avx2_f32_fill(float* _out, mmint_t _len, float _val)
{
    mmint_t idx = 0;
    if (_len >= MMSIMD_AVX2_F32_OFFSET)
    {
        __m256 vval = _mm256_set1_ps(_val);
        for (; idx + MMSIMD_AVX2_F32_OFFSET <= _len; idx += MMSIMD_AVX2_F32_OFFSET)
            _mm256_storeu_ps(&_out[idx], vval);
    }

    for (; idx < _len; ++idx)
        _out[idx] = _val;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
void mmsimd_avx2_f64_fill(double* _out, mmint_t _len, double _val)
{
    mmint_t idx = 0;
    if (_len >= MMSIMD_AVX2_F64_OFFSET)
    {
        __m256d vval = _mm256_set1_pd(_val);
        for (; idx + MMSIMD_AVX2_F64_OFFSET <= _len; idx += MMSIMD_AVX2_F64_OFFSET)
            _mm256_storeu_pd(&_out[idx], vval);
    }

    for (; idx < _len; ++idx)
        _out[idx] = _val;
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t mmsimd_avx2_i8_find(const int8_t* _buf, mmint_t _len, int8_t _val)
{
    mmint_t idx = 0;
    if (_len >= (mmint_t)sizeof(__m256i))
    {
        __m256i vval = _mm256_set1_epi8(_val);
        for (; idx + (mmint_t)sizeof(__m256i) <= _len; idx += (mmint_t)sizeof(__m256i))
        {
            __m256i vi = _mm256_loadu_si256((__m256i*) & _buf[idx]);
            __m256i vm = _mm256_cmpeq_epi8(vi, vval);
            int mask = _mm256_movemask_epi8(vm);
            if (mask) {
                for (int i = 0; i < sizeof(__m256i); ++i)
                {
                    if (_buf[idx + i] == _val)
                        return idx + i;
                }
            }
        }
    }

    for (; idx < _len; ++idx) {
        if (_buf[idx] == _val)
            return idx;
    }

    return -1;
}

mmint_t mmsimd_avx2_u8_find(const uint8_t* _buf, mmint_t _len, uint8_t _val)
{
    return mmsimd_avx2_i8_find((const int8_t*)_buf, _len, (int8_t)_val);
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t mmsimd_avx2_i16_find(const int16_t* _buf, mmint_t _len, int16_t _val)
{
    mmint_t idx = 0;
    if (_len >= MMSIMD_AVX2_I16_OFFSET)
    {
        __m256i vval = _mm256_set1_epi16(_val);
        for (; idx + MMSIMD_AVX2_I16_OFFSET <= _len; idx += MMSIMD_AVX2_I16_OFFSET)
        {
            __m256i vi = _mm256_loadu_si256((__m256i*) & _buf[idx]);
            __m256i vm = _mm256_cmpeq_epi16(vi, vval);
            int mask = _mm256_movemask_epi8(vm);
            if (mask) {
                for (int i = 0; i < MMSIMD_AVX2_I16_OFFSET; ++i)
                {
                    if (_buf[idx + i] == _val)
                        return idx + i;
                }
            }
        }
    }

    for (; idx < _len; ++idx) {
        if (_buf[idx] == _val)
            return idx;
    }

    return -1;
}

mmint_t mmsimd_avx2_u16_find(const uint16_t* _buf, mmint_t _len, uint16_t _val)
{
    return mmsimd_avx2_i16_find((const int16_t*)_buf, _len, (int16_t)_val);
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t mmsimd_avx2_i32_find(const int32_t* _buf, mmint_t _len, int32_t _val)
{
    mmint_t idx = 0;
    if (_len >= MMSIMD_AVX2_I32_OFFSET)
    {
        __m256i vval = _mm256_set1_epi32(_val);
        for (; idx + MMSIMD_AVX2_I32_OFFSET <= _len; idx += MMSIMD_AVX2_I32_OFFSET)
        {
            __m256i vi = _mm256_loadu_si256((__m256i*) & _buf[idx]);
            __m256i vm = _mm256_cmpeq_epi32(vi, vval);
            int mask = _mm256_movemask_epi8(vm);
            if (mask) {
                for (int i = 0; i < MMSIMD_AVX2_I32_OFFSET; ++i)
                {
                    if (_buf[idx + i] == _val)
                        return idx + i;
                }
            }
        }
    }

    for (; idx < _len; ++idx) {
        if (_buf[idx] == _val)
            return idx;
    }

    return -1;
}

mmint_t mmsimd_avx2_u32_find(const uint32_t* _buf, mmint_t _len, uint32_t _val)
{
    return mmsimd_avx2_i32_find((const int32_t*)_buf, _len, (int32_t)_val);
}

MMSIMD_AVX2_TARGET_ATTRIBUTES
mmint_t mmsimd_avx2_i64_find(const int64_t* _buf, mmint_t _len, int64_t _val)
{
    mmint_t idx = 0;
    if (_len >= MMSIMD_AVX2_I64_OFFSET)
    {
        __m256i vval = _mm256_set1_epi64x(_val);
        for (; idx + MMSIMD_AVX2_I64_OFFSET <= _len; idx += MMSIMD_AVX2_I64_OFFSET)
        {
            __m256i vi = _mm256_loadu_si256((__m256i*) & _buf[idx]);
            __m256i vm = _mm256_cmpeq_epi64(vi, vval);
            int mask = _mm256_movemask_epi8(vm);
            if (mask) {
                for (int i = 0; i < MMSIMD_AVX2_I64_OFFSET; ++i)
                {
                    if (_buf[idx + i] == _val)
                        return idx + i;
                }
            }
        }
    }

    for (; idx < _len; ++idx) {
        if (_buf[idx] == _val)
            return idx;
    }

    return -1;
}

mmint_t mmsimd_avx2_u64_find(const uint64_t* _buf, mmint_t _len, uint64_t _val)
{
    return mmsimd_avx2_i64_find((const int64_t*)_buf, _len, (int64_t)_val);
}

//mmint_t mmsimd_avx2_f32_find(const float* _buf, mmint_t _len, float _val)
//{
//    mmint_t idx = 0;
//    mmint_t offset = MMSIMD_AVX2_F32_OFFSET;
//    if (_len >= offset)
//    {
//        __m256 vval = _mm256_set1_ps(_val);
//        for (; idx + offset <= _len; idx += offset)
//        {
//            __m256 vi = _mm256_loadu_ps(&_buf[idx]);
//            __m256 vm = _mm256_cmp_ps(vi, vval, _CMP_EQ_OQ);
//            int mask = _mm256_movemask_ps(vm);
//            if (mask) {
//                for (int i = 0; i < offset; ++i)
//                {
//                    if (_buf[idx + i] == _val)
//                        return idx + i;
//                }
//            }
//        }
//    }
//
//    for (; idx < _len; ++idx)
//    {
//        if (_buf[idx] == _val)
//            return idx;
//    }
//
//    return -1;
//}
//
//mmint_t mmsimd_avx2_f64_find(const double* _buf, mmint_t _len, double _val)
//{
//    mmint_t idx = 0;
//    mmint_t offset = MMSIMD_AVX2_F64_OFFSET;
//    if (_len >= offset)
//    {
//        __m256d vval = _mm256_set1_pd(_val);
//        for (; idx + offset <= _len; idx += offset)
//        {
//            __m256d vi = _mm256_loadu_pd(&_buf[idx]);
//            __m256d vm = _mm256_cmp_pd(vi, vval, _CMP_EQ_OQ);
//            int mask = _mm256_movemask_pd(vm);
//            if (mask) {
//                for (int i = 0; i < offset; ++i)
//                {
//                    if (_buf[idx + i] == _val)
//                        return idx + i;
//                }
//            }
//        }
//    }
//
//    for (; idx < _len; ++idx)
//    {
//        if (_buf[idx] == _val)
//            return idx;
//    }
//
//    return -1;
//}

void mmsimd_avx2_i8_clamp(
    const int8_t* _a, int8_t _min, int8_t _max, int8_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i8_clamp(_a, _min, _max, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = (_a[i] < _min) ? _min : (_a[i] > _max) ? _max : _a[i];
}

void mmsimd_avx2_u8_clamp(
    const uint8_t* _a, uint8_t _min, uint8_t _max, uint8_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_u8_clamp(_a, _min, _max, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = (_a[i] < _min) ? _min : (_a[i] > _max) ? _max : _a[i];
}

void mmsimd_avx2_i16_clamp(
    const int16_t* _a, int16_t _min, int16_t _max, int16_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i16_clamp(_a, _min, _max, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = (_a[i] < _min) ? _min : (_a[i] > _max) ? _max : _a[i];
}

void mmsimd_avx2_u16_clamp(
    const uint16_t* _a, uint16_t _min, uint16_t _max, uint16_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_u16_clamp(_a, _min, _max, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = (_a[i] < _min) ? _min : (_a[i] > _max) ? _max : _a[i];
}

void mmsimd_avx2_i32_clamp(
    const int32_t* _a, int32_t _min, int32_t _max, int32_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_i32_clamp(_a, _min, _max, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = (_a[i] < _min) ? _min : (_a[i] > _max) ? _max : _a[i];
}

void mmsimd_avx2_u32_clamp(
    const uint32_t* _a, uint32_t _min, uint32_t _max, uint32_t* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_u32_clamp(_a, _min, _max, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = (_a[i] < _min) ? _min : (_a[i] > _max) ? _max : _a[i];
}

void mmsimd_avx2_f32_clamp(
    const float* _a, float _min, float _max, float* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_f32_clamp(_a, _min, _max, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = (_a[i] < _min) ? _min : (_a[i] > _max) ? _max : _a[i];
}

void mmsimd_avx2_f64_clamp(
    const double* _a, double _min, double _max, double* _c, mmint_t _n)
{
    mmint_t i = __mmsimd_avx2_f64_clamp(_a, _min, _max, _c, 0, _n);

    for (; i < _n; ++i)
        _c[i] = (_a[i] < _min) ? _min : (_a[i] > _max) ? _max : _a[i];
}

#undef MMSIMD_AVX2_I16_OFFSET
#undef MMSIMD_AVX2_I32_OFFSET
#undef MMSIMD_AVX2_I64_OFFSET
#undef MMSIMD_AVX2_U16_OFFSET
#undef MMSIMD_AVX2_U32_OFFSET
#undef MMSIMD_AVX2_U64_OFFSET
#undef MMSIMD_AVX2_F32_OFFSET
#undef MMSIMD_AVX2_F64_OFFSET
#undef MMSIMD_AVX2_TARGET_ATTRIBUTES
