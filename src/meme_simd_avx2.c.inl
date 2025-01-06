
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
    if (_n >= (mmint_t)sizeof(__m256i))
    {
        __m256i vb = _mm256_set1_epi8(_b);
        for (; i + (mmint_t)sizeof(__m256i) <= _n; i += (mmint_t)sizeof(__m256i))
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[i]);
            __m256i vr = _mm256_add_epi8(va, vb);

            _mm256_storeu_si256((__m256i*) & _c[i], vr);
        }
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
    if (_n >= (mmint_t)sizeof(__m256i))
    {
        __m256i vb = _mm256_set1_epi8(_b);
        for (; i + (mmint_t)sizeof(__m256i) <= _n; i += (mmint_t)sizeof(__m256i))
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[i]);
            __m256i vr = _mm256_sub_epi8(va, vb);

            _mm256_storeu_si256((__m256i*) & _c[i], vr);
        }
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
    if (_n >= (mmint_t)sizeof(__m256i))
    {
        __m256i vb = _mm256_set1_epi8(_b);
        for (; i + (mmint_t)sizeof(__m256i) <= _n; i += (mmint_t)sizeof(__m256i))
        {
            __m256i va = _mm256_loadu_si256((__m256i*) & _a[i]);
            __m256i vr = _mm256_div_epi8(va, vb);

            _mm256_storeu_si256((__m256i*) & _c[i], vr);
        }
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

#if MG_COMP__MSVC_AVAIL
void mmsimd_avx2_u8_div(const uint8_t* _a, const uint8_t* _b, uint8_t* _c, mmint_t _n)
{
    mmsimd_avx2_i8_div((const int8_t*)_a, (const int8_t*)_b, (int8_t*)_c, _n);
}
#endif

#if MG_COMP__MSVC_AVAIL
void mmsimd_avx2_u8_div_scalar(const uint8_t* _a, uint8_t _b, uint8_t* _c, mmint_t _n)
{
    mmsimd_avx2_i8_div_scalar((const int8_t*)_a, (int8_t)_b, (int8_t*)_c, _n);
}
#endif

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

void mmsimd_avx2_i16_fill(int16_t* _out, mmint_t _len, int16_t _val)
{
    mmint_t idx = 0;
    mmint_t offset = (mmint_t)(sizeof(__m256i) / sizeof(int16_t));
    if (_len >= offset)
    {
        __m256i vval = _mm256_set1_epi16(_val);
        for (; idx + offset <= _len; idx += offset)
            _mm256_storeu_si256((__m256i*)&_out[idx], vval);
    }

    for (; idx < _len; ++idx)
        _out[idx] = _val;
}

void mmsimd_avx2_u16_fill(uint16_t* _out, mmint_t _len, uint16_t _val)
{
    mmsimd_avx2_i16_fill((int16_t*)_out, _len, (int16_t)_val);
}

void mmsimd_avx2_i32_fill(int32_t* _out, mmint_t _len, int32_t _val)
{
    mmint_t idx = 0;
    mmint_t offset = (mmint_t)(sizeof(__m256i) / sizeof(int32_t));
    if (_len >= offset)
    {
        __m256i vval = _mm256_set1_epi32(_val);
        for (; idx + offset <= _len; idx += offset)
            _mm256_storeu_si256((__m256i*)&_out[idx], vval);
    }

    for (; idx < _len; ++idx)
        _out[idx] = _val;
}

void mmsimd_avx2_u32_fill(uint32_t* _out, mmint_t _len, uint32_t _val)
{
    mmsimd_avx2_i32_fill((int32_t*)_out, _len, (int32_t)_val);
}

void mmsimd_avx2_i64_fill(int64_t* _out, mmint_t _len, int64_t _val)
{
    mmint_t idx = 0;
    mmint_t offset = (mmint_t)(sizeof(__m256i) / sizeof(int64_t));
    if (_len >= offset)
    {
        __m256i vval = _mm256_set1_epi64x(_val);
        for (; idx + offset <= _len; idx += offset)
            _mm256_storeu_si256((__m256i*)&_out[idx], vval);
    }

    for (; idx < _len; ++idx)
        _out[idx] = _val;
}

void mmsimd_avx2_u64_fill(uint64_t* _out, mmint_t _len, uint64_t _val)
{
    mmsimd_avx2_i64_fill((int64_t*)_out, _len, (int64_t)_val);
}

void mmsimd_avx2_f32_fill(float* _out, mmint_t _len, float _val)
{
    mmint_t idx = 0;
    mmint_t offset = (mmint_t)(sizeof(__m256) / sizeof(float));
    if (_len >= offset)
    {
        __m256 vval = _mm256_set1_ps(_val);
        for (; idx + offset <= _len; idx += offset)
            _mm256_storeu_ps(&_out[idx], vval);
    }

    for (; idx < _len; ++idx)
        _out[idx] = _val;
}

void mmsimd_avx2_f64_fill(double* _out, mmint_t _len, double _val)
{
    mmint_t idx = 0;
    mmint_t offset = (mmint_t)(sizeof(__m256d) / sizeof(double));
    if (_len >= offset)
    {
        __m256d vval = _mm256_set1_pd(_val);
        for (; idx + offset <= _len; idx += offset)
            _mm256_storeu_pd(&_out[idx], vval);
    }

    for (; idx < _len; ++idx)
        _out[idx] = _val;
}

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

mmint_t mmsimd_avx2_i16_find(const int16_t* _buf, mmint_t _len, int16_t _val)
{
    mmint_t idx = 0;
    mmint_t offset = (mmint_t)(sizeof(__m256i) / sizeof(int16_t));
    if (_len >= offset)
    {
        __m256i vval = _mm256_set1_epi16(_val);
        for (; idx + offset <= _len; idx += offset)
        {
            __m256i vi = _mm256_loadu_si256((__m256i*) & _buf[idx]);
            __m256i vm = _mm256_cmpeq_epi16(vi, vval);
            int mask = _mm256_movemask_epi8(vm);
            if (mask) {
                for (int i = 0; i < offset; ++i)
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

mmint_t mmsimd_avx2_i32_find(const int32_t* _buf, mmint_t _len, int32_t _val)
{
    mmint_t idx = 0;
    mmint_t offset = (mmint_t)(sizeof(__m256i) / sizeof(int32_t));
    if (_len >= offset)
    {
        __m256i vval = _mm256_set1_epi32(_val);
        for (; idx + offset <= _len; idx += offset)
        {
            __m256i vi = _mm256_loadu_si256((__m256i*) & _buf[idx]);
            __m256i vm = _mm256_cmpeq_epi32(vi, vval);
            int mask = _mm256_movemask_epi8(vm);
            if (mask) {
                for (int i = 0; i < offset; ++i)
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

mmint_t mmsimd_avx2_i64_find(const int64_t* _buf, mmint_t _len, int64_t _val)
{
    mmint_t idx = 0;
    mmint_t offset = (mmint_t)(sizeof(__m256i) / sizeof(int64_t));
    if (_len >= offset)
    {
        __m256i vval = _mm256_set1_epi64x(_val);
        for (; idx + offset <= _len; idx += offset)
        {
            __m256i vi = _mm256_loadu_si256((__m256i*) & _buf[idx]);
            __m256i vm = _mm256_cmpeq_epi64(vi, vval);
            int mask = _mm256_movemask_epi8(vm);
            if (mask) {
                for (int i = 0; i < offset; ++i)
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
//    mmint_t offset = (mmint_t)(sizeof(__m256) / sizeof(float));
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
//    mmint_t offset = (mmint_t)(sizeof(__m256d) / sizeof(double));
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
