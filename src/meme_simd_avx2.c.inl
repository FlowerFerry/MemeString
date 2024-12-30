
#include <immintrin.h>
#include <meme/string_fwd.h>
#include <mego/predef/compiler/visualc.h>

void mmsimd_avx2_i8_add(const int8_t* _a, const int8_t* _b, int8_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    for (; i + (mmint_t)sizeof(__m256i) <= _n; i += (mmint_t)sizeof(__m256i))
    {
        __m256i va = _mm256_loadu_si256((__m256i*)&_a[i]);
        __m256i vb = _mm256_loadu_si256((__m256i*)&_b[i]);

        __m256i vr = _mm256_add_epi8(va, vb);

        _mm256_storeu_si256((__m256i*)&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b[i];
}

void mmsimd_avx2_i8_add_scalar(const int8_t* _a, int8_t _b, int8_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    __m256i vb = _mm256_set1_epi8(_b);
    for (; i + (mmint_t)sizeof(__m256i) <= _n; i += (mmint_t)sizeof(__m256i))
    {
        __m256i va = _mm256_loadu_si256((__m256i*) & _a[i]);

        __m256i vr = _mm256_add_epi8(va, vb);

        _mm256_storeu_si256((__m256i*) & _c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b;
}

void mmsimd_avx2_i8_sub(const int8_t* _a, const int8_t* _b, int8_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    for (; i + (mmint_t)sizeof(__m256i) <= _n; i += (mmint_t)sizeof(__m256i))
    {
        __m256i va = _mm256_loadu_si256((__m256i*)&_a[i]);
        __m256i vb = _mm256_loadu_si256((__m256i*)&_b[i]);

        __m256i vr = _mm256_sub_epi8(va, vb);

        _mm256_storeu_si256((__m256i*)&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b[i];
}

void mmsimd_avx2_i8_sub_scalar(const int8_t* _a, int8_t _b, int8_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    __m256i vb = _mm256_set1_epi8(_b);
    for (; i + (mmint_t)sizeof(__m256i) <= _n; i += (mmint_t)sizeof(__m256i))
    {
        __m256i va = _mm256_loadu_si256((__m256i*) & _a[i]);

        __m256i vr = _mm256_sub_epi8(va, vb);

        _mm256_storeu_si256((__m256i*) & _c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b;
}

#if MG_COMP__MSVC_AVAIL
void mmsimd_avx2_i8_div(const int8_t* _a, const int8_t* _b, int8_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    for (; i + (mmint_t)sizeof(__m256i) <= _n; i += (mmint_t)sizeof(__m256i))
    {
        __m256i va = _mm256_loadu_si256((__m256i*) &_a[i]);
        __m256i vb = _mm256_loadu_si256((__m256i*) &_b[i]);

        __m256i vr = _mm256_div_epi8(va, vb);

        _mm256_storeu_si256((__m256i*) &_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] / _b[i];
}
#endif

#if MG_COMP__MSVC_AVAIL
void mmsimd_avx2_i8_div_scalar(const int8_t* _a, int8_t _b, int8_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    __m256i vb = _mm256_set1_epi8(_b);
    for (; i + (mmint_t)sizeof(__m256i) <= _n; i += (mmint_t)sizeof(__m256i))
    {
        __m256i va = _mm256_loadu_si256((__m256i*) &_a[i]);

        __m256i vr = _mm256_div_epi8(va, vb);

        _mm256_storeu_si256((__m256i*) &_c[i], vr);
    }

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

void mmsimd_avx2_u8_div(const uint8_t* _a, const uint8_t* _b, uint8_t* _c, mmint_t _n)
{
    mmsimd_avx2_i8_div((const int8_t*)_a, (const int8_t*)_b, (int8_t*)_c, _n);
}

void mmsimd_avx2_u8_div_scalar(const uint8_t* _a, uint8_t _b, uint8_t* _c, mmint_t _n)
{
    mmsimd_avx2_i8_div_scalar((const int8_t*)_a, (int8_t)_b, (int8_t*)_c, _n);
}

void mmsimd_avx2_i8_to_i16(const int8_t* _in, int16_t* _out, mmint_t _n)
{
    mmint_t idx = 0;
    for (; idx + (mmint_t)sizeof(__m256i) <= _n; idx += (mmint_t)sizeof(__m256i))
    {
        __m256i vi   = _mm256_loadu_si256((__m256i*)&_in[idx]);
        __m128i vi_l = _mm256_extracti128_si256(vi, 0);
        __m128i vi_h = _mm256_extracti128_si256(vi, 1);
        __m256i vo;

        vo = _mm256_cvtepi8_epi16(vi_l);
        _mm256_storeu_si256((__m256i*)&_out[idx], vo);
        vo = _mm256_cvtepi8_epi16(vi_h);
        _mm256_storeu_si256((__m256i*)&_out[idx + 16], vo);
    }

    for (; idx < _n; ++idx)
        _out[idx] = _in[idx];
}

void mmsimd_avx2_i8_to_u16(const int8_t* _in, uint16_t* _out, mmint_t _n)
{
    mmsimd_avx2_i8_to_i16(_in, (int16_t*)_out, _n);
}

//! _mm_loadu_si128 and _mm_srli_si128 is SSE2 instruction
void mmsimd_sse2_avx2_i8_to_i32(const int8_t* _in, int32_t* _out, mmint_t _n)
{
    mmint_t idx = 0;
    for (; idx + (mmint_t)sizeof(__m128i) <= _n; idx += (mmint_t)sizeof(__m128i))
    {
        __m128i vi_l = _mm_loadu_si128((__m128i*)&_in[idx]);
        __m128i vi_h = _mm_srli_si128(vi_l, 8);
        __m256i vo;

        vo = _mm256_cvtepi8_epi32(vi_l);
        _mm256_storeu_si256((__m256i*)&_out[idx], vo);

        vo = _mm256_cvtepi8_epi32(vi_h);
        _mm256_storeu_si256((__m256i*)&_out[idx + 8], vo);
    }

    for (; idx < _n; ++idx)
        _out[idx] = _in[idx];
}

void mmsimd_sse2_avx2_i8_to_u32(const int8_t* _in, uint32_t* _out, mmint_t _n)
{
    mmsimd_sse2_avx2_i8_to_i32(_in, (int32_t*)_out, _n);
}

//! _mm_loadu_si128 and _mm_srli_si128 is SSE2 instruction
void mmsimd_sse2_avx2_i8_to_i64(const int8_t* _in, int64_t* _out, mmint_t _n)
{
    mmint_t idx = 0;
    for (; idx + (mmint_t)sizeof(__m128i) <= _n; idx += (mmint_t)sizeof(__m128i))
    {
        __m128i vi_1 = _mm_loadu_si128((__m128i*)&_in[idx]);
        __m128i vi_2 = _mm_srli_si128(vi_1, 4);
        __m128i vi_3 = _mm_srli_si128(vi_1, 8);
        __m128i vi_4 = _mm_srli_si128(vi_1, 12);
        __m256i vo;

        vo = _mm256_cvtepi8_epi64(vi_1);
        _mm256_storeu_si256((__m256i*)&_out[idx], vo);

        vo = _mm256_cvtepi8_epi64(vi_2);
        _mm256_storeu_si256((__m256i*)&_out[idx + 4], vo);

        vo = _mm256_cvtepi8_epi64(vi_3);
        _mm256_storeu_si256((__m256i*)&_out[idx + 8], vo);

        vo = _mm256_cvtepi8_epi64(vi_4);
        _mm256_storeu_si256((__m256i*)&_out[idx + 12], vo);
    }

    for (; idx < _n; ++idx)
        _out[idx] = _in[idx];
}

void mmsimd_sse2_avx2_i8_to_u64(const int8_t* _in, uint64_t* _out, mmint_t _n)
{
    mmsimd_sse2_avx2_i8_to_i64(_in, (int64_t*)_out, _n);
}

//! _mm_loadu_si128 and _mm_srli_si128 is SSE2 instruction
void mmsimd_sse2_avx2_i8_to_f32(const int8_t* _in, float* _out, mmint_t _n)
{
    mmint_t idx = 0;
    for (; idx + (mmint_t)sizeof(__m256) <= _n; idx += (mmint_t)sizeof(__m256))
    {
        __m128i vi_l = _mm_loadu_si128((__m128i*) & _in[idx]);
        __m128i vi_h = _mm_srli_si128(vi_l, 8);
        __m256 vo;

        vo = _mm256_cvtepi32_ps(_mm256_cvtepi8_epi32(vi_l));
        _mm256_storeu_ps(&_out[idx], vo);

        vo = _mm256_cvtepi32_ps(_mm256_cvtepi8_epi32(vi_h));
        _mm256_storeu_ps(&_out[idx + 8], vo);
    }

    for (; idx < _n; ++idx)
        _out[idx] = (float)_in[idx];
}
