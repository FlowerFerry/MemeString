
#if defined(__ARM_NEON)

#include <arm_neon.h>

void mmsimd_neon_i8_add(const int8_t* _a, const int8_t* _b, int8_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    for (; i + (mmint_t)sizeof(int8x16_t) <= _n; i += (mmint_t)sizeof(int8x16_t))
    {
        int8x16_t va = vld1q_s8(&_a[i]);
        int8x16_t vb = vld1q_s8(&_b[i]);

        int8x16_t vr = vaddq_s8(va, vb);

        vst1q_s8(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b[i];
}

void mmsimd_neon_i8_add_scalar(const int8_t* _a, int8_t _b, int8_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    int8x16_t vb = vdupq_n_s8(_b);
    for (; i + (mmint_t)sizeof(int8x16_t) <= _n; i += (mmint_t)sizeof(int8x16_t))
    {
        int8x16_t va = vld1q_s8(&_a[i]);

        int8x16_t vr = vaddq_s8(va, vb);

        vst1q_s8(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b;
}

void mmsimd_neon_i8_sub(const int8_t* _a, const int8_t* _b, int8_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    for (; i + (mmint_t)sizeof(int8x16_t) <= _n; i += (mmint_t)sizeof(int8x16_t))
    {
        int8x16_t va = vld1q_s8(&_a[i]);
        int8x16_t vb = vld1q_s8(&_b[i]);

        int8x16_t vr = vsubq_s8(va, vb);

        vst1q_s8(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b[i];
}

void mmsimd_neon_i8_sub_scalar(const int8_t* _a, int8_t _b, int8_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    int8x16_t vb = vdupq_n_s8(_b);
    for (; i + (mmint_t)sizeof(int8x16_t) <= _n; i += (mmint_t)sizeof(int8x16_t))
    {
        int8x16_t va = vld1q_s8(&_a[i]);

        int8x16_t vr = vsubq_s8(va, vb);

        vst1q_s8(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b;
}

void mmsimd_neon_i8_mul(const int8_t* _a, const int8_t* _b, int8_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    for (; i + (mmint_t)sizeof(int8x16_t) <= _n; i += (mmint_t)sizeof(int8x16_t))
    {
        int8x16_t va = vld1q_s8(&_a[i]);
        int8x16_t vb = vld1q_s8(&_b[i]);

        int8x16_t vr = vmulq_s8(va, vb);

        vst1q_s8(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] * _b[i];
}

void mmsimd_neon_i8_mul_scalar(const int8_t* _a, int8_t _b, int8_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    int8x16_t vb = vdupq_n_s8(_b);
    for (; i + (mmint_t)sizeof(int8x16_t) <= _n; i += (mmint_t)sizeof(int8x16_t))
    {
        int8x16_t va = vld1q_s8(&_a[i]);

        int8x16_t vr = vmulq_s8(va, vb);

        vst1q_s8(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] * _b;
}

void mmsimd_neon_u8_add(const uint8_t* _a, const uint8_t* _b, uint8_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    for (; i + (mmint_t)sizeof(uint8x16_t) <= _n; i += (mmint_t)sizeof(uint8x16_t))
    {
        uint8x16_t va = vld1q_u8(&_a[i]);
        uint8x16_t vb = vld1q_u8(&_b[i]);

        uint8x16_t vr = vaddq_u8(va, vb);

        vst1q_u8(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b[i];
}

void mmsimd_neon_u8_add_scalar(const uint8_t* _a, uint8_t _b, uint8_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    uint8x16_t vb = vdupq_n_u8(_b);
    for (; i + (mmint_t)sizeof(uint8x16_t) <= _n; i += (mmint_t)sizeof(uint8x16_t))
    {
        uint8x16_t va = vld1q_u8(&_a[i]);

        uint8x16_t vr = vaddq_u8(va, vb);

        vst1q_u8(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b;
}

void mmsimd_neon_u8_sub(const uint8_t* _a, const uint8_t* _b, uint8_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    for (; i + (mmint_t)sizeof(uint8x16_t) <= _n; i += (mmint_t)sizeof(uint8x16_t))
    {
        uint8x16_t va = vld1q_u8(&_a[i]);
        uint8x16_t vb = vld1q_u8(&_b[i]);

        uint8x16_t vr = vsubq_u8(va, vb);

        vst1q_u8(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b[i];
}

void mmsimd_neon_u8_sub_scalar(const uint8_t* _a, uint8_t _b, uint8_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    uint8x16_t vb = vdupq_n_u8(_b);
    for (; i + (mmint_t)sizeof(uint8x16_t) <= _n; i += (mmint_t)sizeof(uint8x16_t))
    {
        uint8x16_t va = vld1q_u8(&_a[i]);

        uint8x16_t vr = vsubq_u8(va, vb);

        vst1q_u8(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b;
}

void mmsimd_neon_u8_mul(const uint8_t* _a, const uint8_t* _b, uint8_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    for (; i + (mmint_t)sizeof(uint8x16_t) <= _n; i += (mmint_t)sizeof(uint8x16_t))
    {
        uint8x16_t va = vld1q_u8(&_a[i]);
        uint8x16_t vb = vld1q_u8(&_b[i]);

        uint8x16_t vr = vmulq_u8(va, vb);

        vst1q_u8(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] * _b[i];
}

void mmsimd_neon_u8_mul_scalar(const uint8_t* _a, uint8_t _b, uint8_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    uint8x16_t vb = vdupq_n_u8(_b);
    for (; i + (mmint_t)sizeof(uint8x16_t) <= _n; i += (mmint_t)sizeof(uint8x16_t))
    {
        uint8x16_t va = vld1q_u8(&_a[i]);

        uint8x16_t vr = vmulq_u8(va, vb);

        vst1q_u8(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] * _b;
}

void mmsimd_neon_i16_add(const int16_t* _a, const int16_t* _b, int16_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    for (; i + (mmint_t)sizeof(int16x8_t) <= _n; i += (mmint_t)sizeof(int16x8_t))
    {
        int16x8_t va = vld1q_s16(&_a[i]);
        int16x8_t vb = vld1q_s16(&_b[i]);

        int16x8_t vr = vaddq_s16(va, vb);

        vst1q_s16(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b[i];
}

void mmsimd_neon_i16_add_scalar(const int16_t* _a, int16_t _b, int16_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    int16x8_t vb = vdupq_n_s16(_b);
    for (; i + (mmint_t)sizeof(int16x8_t) <= _n; i += (mmint_t)sizeof(int16x8_t))
    {
        int16x8_t va = vld1q_s16(&_a[i]);

        int16x8_t vr = vaddq_s16(va, vb);

        vst1q_s16(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b;
}

void mmsimd_neon_i16_sub(const int16_t* _a, const int16_t* _b, int16_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    for (; i + (mmint_t)sizeof(int16x8_t) <= _n; i += (mmint_t)sizeof(int16x8_t))
    {
        int16x8_t va = vld1q_s16(&_a[i]);
        int16x8_t vb = vld1q_s16(&_b[i]);

        int16x8_t vr = vsubq_s16(va, vb);

        vst1q_s16(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b[i];
}

void mmsimd_neon_i16_sub_scalar(const int16_t* _a, int16_t _b, int16_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    int16x8_t vb = vdupq_n_s16(_b);
    for (; i + (mmint_t)sizeof(int16x8_t) <= _n; i += (mmint_t)sizeof(int16x8_t))
    {
        int16x8_t va = vld1q_s16(&_a[i]);

        int16x8_t vr = vsubq_s16(va, vb);

        vst1q_s16(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b;
}

void mmsimd_neon_i16_mul(const int16_t* _a, const int16_t* _b, int16_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    for (; i + (mmint_t)sizeof(int16x8_t) <= _n; i += (mmint_t)sizeof(int16x8_t))
    {
        int16x8_t va = vld1q_s16(&_a[i]);
        int16x8_t vb = vld1q_s16(&_b[i]);

        int16x8_t vr = vmulq_s16(va, vb);

        vst1q_s16(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] * _b[i];
}

void mmsimd_neon_i16_mul_scalar(const int16_t* _a, int16_t _b, int16_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    int16x8_t vb = vdupq_n_s16(_b);
    for (; i + (mmint_t)sizeof(int16x8_t) <= _n; i += (mmint_t)sizeof(int16x8_t))
    {
        int16x8_t va = vld1q_s16(&_a[i]);

        int16x8_t vr = vmulq_s16(va, vb);

        vst1q_s16(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] * _b;
}

void mmsimd_neon_u16_add(const uint16_t* _a, const uint16_t* _b, uint16_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    for (; i + (mmint_t)sizeof(uint16x8_t) <= _n; i += (mmint_t)sizeof(uint16x8_t))
    {
        uint16x8_t va = vld1q_u16(&_a[i]);
        uint16x8_t vb = vld1q_u16(&_b[i]);

        uint16x8_t vr = vaddq_u16(va, vb);

        vst1q_u16(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b[i];
}

void mmsimd_neon_u16_add_scalar(const uint16_t* _a, uint16_t _b, uint16_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    uint16x8_t vb = vdupq_n_u16(_b);
    for (; i + (mmint_t)sizeof(uint16x8_t) <= _n; i += (mmint_t)sizeof(uint16x8_t))
    {
        uint16x8_t va = vld1q_u16(&_a[i]);

        uint16x8_t vr = vaddq_u16(va, vb);

        vst1q_u16(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] + _b;
}

void mmsimd_neon_u16_sub(const uint16_t* _a, const uint16_t* _b, uint16_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    for (; i + (mmint_t)sizeof(uint16x8_t) <= _n; i += (mmint_t)sizeof(uint16x8_t))
    {
        uint16x8_t va = vld1q_u16(&_a[i]);
        uint16x8_t vb = vld1q_u16(&_b[i]);

        uint16x8_t vr = vsubq_u16(va, vb);

        vst1q_u16(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b[i];
}

void mmsimd_neon_u16_sub_scalar(const uint16_t* _a, uint16_t _b, uint16_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    uint16x8_t vb = vdupq_n_u16(_b);
    for (; i + (mmint_t)sizeof(uint16x8_t) <= _n; i += (mmint_t)sizeof(uint16x8_t))
    {
        uint16x8_t va = vld1q_u16(&_a[i]);

        uint16x8_t vr = vsubq_u16(va, vb);

        vst1q_u16(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] - _b;
}

void mmsimd_neon_u16_mul(const uint16_t* _a, const uint16_t* _b, uint16_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    for (; i + (mmint_t)sizeof(uint16x8_t) <= _n; i += (mmint_t)sizeof(uint16x8_t))
    {
        uint16x8_t va = vld1q_u16(&_a[i]);
        uint16x8_t vb = vld1q_u16(&_b[i]);

        uint16x8_t vr = vmulq_u16(va, vb);

        vst1q_u16(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] * _b[i];
}

void mmsimd_neon_u16_mul_scalar(const uint16_t* _a, uint16_t _b, uint16_t* _c, mmint_t _n)
{
    mmint_t i = 0;
    uint16x8_t vb = vdupq_n_u16(_b);
    for (; i + (mmint_t)sizeof(uint16x8_t) <= _n; i += (mmint_t)sizeof(uint16x8_t))
    {
        uint16x8_t va = vld1q_u16(&_a[i]);

        uint16x8_t vr = vmulq_u16(va, vb);

        vst1q_u16(&_c[i], vr);
    }

    for (; i < _n; ++i)
        _c[i] = _a[i] * _b;
}

#endif // defined(__ARM_PCS_VFP)
