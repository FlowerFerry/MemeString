
#include <meme/common.h>


MEME_EXTERN_C_SCOPE_START

#define MMSIMD_DEFAULT_ADD_FUNC(NAME, TYPE) \
    static void mmsimd_default_##NAME##_add(const TYPE* _a, const TYPE* _b, TYPE* _o, mmint_t _n) \
    { \
        for (mmint_t i = 0; i < _n; ++i) \
        { \
            _o[i] = _a[i] + _b[i]; \
        } \
    } \
    static void mmsimd_default_##NAME##_add_scalar(const TYPE* _a, TYPE _b, TYPE* _o, mmint_t _n) \
    { \
        for (mmint_t i = 0; i < _n; ++i) \
        { \
            _o[i] = _a[i] + _b; \
        } \
    }

MMSIMD_DEFAULT_ADD_FUNC(i8, int8_t)
MMSIMD_DEFAULT_ADD_FUNC(u8, uint8_t)
MMSIMD_DEFAULT_ADD_FUNC(i16, int16_t)
MMSIMD_DEFAULT_ADD_FUNC(u16, uint16_t)
MMSIMD_DEFAULT_ADD_FUNC(i32, int32_t)
MMSIMD_DEFAULT_ADD_FUNC(u32, uint32_t)
MMSIMD_DEFAULT_ADD_FUNC(i64, int64_t)
MMSIMD_DEFAULT_ADD_FUNC(u64, uint64_t)
MMSIMD_DEFAULT_ADD_FUNC(f32, float)
MMSIMD_DEFAULT_ADD_FUNC(f64, double)

#undef MMSIMD_DEFAULT_ADD_FUNC

#define MMSIMD_DEFAULT_SUB_FUNC(NAME, TYPE) \
    static void mmsimd_default_##NAME##_sub(const TYPE* _a, const TYPE* _b, TYPE* _o, mmint_t _n) \
    { \
        for (mmint_t i = 0; i < _n; ++i) \
        { \
            _o[i] = _a[i] - _b[i]; \
        } \
    } \
    static void mmsimd_default_##NAME##_sub_scalar(const TYPE* _a, TYPE _b, TYPE* _o, mmint_t _n) \
    { \
        for (mmint_t i = 0; i < _n; ++i) \
        { \
            _o[i] = _a[i] - _b; \
        } \
    }

MMSIMD_DEFAULT_SUB_FUNC(i8, int8_t)
MMSIMD_DEFAULT_SUB_FUNC(u8, uint8_t)
MMSIMD_DEFAULT_SUB_FUNC(i16, int16_t)
MMSIMD_DEFAULT_SUB_FUNC(u16, uint16_t)
MMSIMD_DEFAULT_SUB_FUNC(i32, int32_t)
MMSIMD_DEFAULT_SUB_FUNC(u32, uint32_t)
MMSIMD_DEFAULT_SUB_FUNC(i64, int64_t)
MMSIMD_DEFAULT_SUB_FUNC(u64, uint64_t)
MMSIMD_DEFAULT_SUB_FUNC(f32, float)
MMSIMD_DEFAULT_SUB_FUNC(f64, double)

#undef MMSIMD_DEFAULT_SUB_FUNC

#define MMSIMD_DEFAULT_MUL_FUNC(NAME, TYPE) \
    static void mmsimd_default_##NAME##_mul(const TYPE* _a, const TYPE* _b, TYPE* _o, mmint_t _n) \
    { \
        for (mmint_t i = 0; i < _n; ++i) \
        { \
            _o[i] = _a[i] * _b[i]; \
        } \
    } \
    static void mmsimd_default_##NAME##_mul_scalar(const TYPE* _a, TYPE _b, TYPE* _o, mmint_t _n) \
    { \
        for (mmint_t i = 0; i < _n; ++i) \
        { \
            _o[i] = _a[i] * _b; \
        } \
    }

MMSIMD_DEFAULT_MUL_FUNC(i8, int8_t)
MMSIMD_DEFAULT_MUL_FUNC(u8, uint8_t)
MMSIMD_DEFAULT_MUL_FUNC(i16, int16_t)
MMSIMD_DEFAULT_MUL_FUNC(u16, uint16_t)
MMSIMD_DEFAULT_MUL_FUNC(i32, int32_t)
MMSIMD_DEFAULT_MUL_FUNC(u32, uint32_t)
MMSIMD_DEFAULT_MUL_FUNC(i64, int64_t)
MMSIMD_DEFAULT_MUL_FUNC(u64, uint64_t)
MMSIMD_DEFAULT_MUL_FUNC(f32, float)
MMSIMD_DEFAULT_MUL_FUNC(f64, double)

#undef MMSIMD_DEFAULT_MUL_FUNC

#define MMSIMD_DEFAULT_DIV_FUNC(NAME, TYPE) \
    static void mmsimd_default_##NAME##_div(const TYPE* _a, const TYPE* _b, TYPE* _o, mmint_t _n) \
    { \
        for (mmint_t i = 0; i < _n; ++i) \
        { \
            _o[i] = _a[i] / _b[i]; \
        } \
    } \
    static void mmsimd_default_##NAME##_div_scalar(const TYPE* _a, TYPE _b, TYPE* _o, mmint_t _n) \
    { \
        for (mmint_t i = 0; i < _n; ++i) \
        { \
            _o[i] = _a[i] / _b; \
        } \
    }

MMSIMD_DEFAULT_DIV_FUNC(i8, int8_t)
MMSIMD_DEFAULT_DIV_FUNC(u8, uint8_t)
MMSIMD_DEFAULT_DIV_FUNC(i16, int16_t)
MMSIMD_DEFAULT_DIV_FUNC(u16, uint16_t)
MMSIMD_DEFAULT_DIV_FUNC(i32, int32_t)
MMSIMD_DEFAULT_DIV_FUNC(u32, uint32_t)
MMSIMD_DEFAULT_DIV_FUNC(i64, int64_t)
MMSIMD_DEFAULT_DIV_FUNC(u64, uint64_t)
MMSIMD_DEFAULT_DIV_FUNC(f32, float)
MMSIMD_DEFAULT_DIV_FUNC(f64, double)

#undef MMSIMD_DEFAULT_DIV_FUNC

#define MMSIMD_DEFAULT_FILL_FUNC(NAME, TYPE) \
    static void mmsimd_default_##NAME##_fill(TYPE* _o, mmint_t _n, TYPE _v) \
    { \
        for (mmint_t i = 0; i < _n; ++i) \
        { \
            _o[i] = _v; \
        } \
    }

MMSIMD_DEFAULT_FILL_FUNC(i16, int16_t)
MMSIMD_DEFAULT_FILL_FUNC(u16, uint16_t)
MMSIMD_DEFAULT_FILL_FUNC(i32, int32_t)
MMSIMD_DEFAULT_FILL_FUNC(u32, uint32_t)
MMSIMD_DEFAULT_FILL_FUNC(i64, int64_t)
MMSIMD_DEFAULT_FILL_FUNC(u64, uint64_t)
MMSIMD_DEFAULT_FILL_FUNC(f32, float)
MMSIMD_DEFAULT_FILL_FUNC(f64, double)

#undef MMSIMD_DEFAULT_FILL_FUNC

#define MMSIMD_DEFAULT_CONV_FUNC(SRC_NAME, DST_NAME, SRC_TYPE, DST_TYPE) \
static void mmsimd_default_##SRC_NAME##_to_##DST_NAME(const SRC_TYPE* _in, DST_TYPE* _out, mmint_t _n) \
{ \
    for (mmint_t i = 0; i < _n; ++i) \
    { \
        _out[i] = (DST_TYPE)_in[i]; \
    } \
} 

MMSIMD_DEFAULT_CONV_FUNC(i8, i16, int8_t, int16_t)
MMSIMD_DEFAULT_CONV_FUNC(i8, u16, int8_t, uint16_t)
MMSIMD_DEFAULT_CONV_FUNC(i8, i32, int8_t, int32_t)
MMSIMD_DEFAULT_CONV_FUNC(i8, u32, int8_t, uint32_t)
MMSIMD_DEFAULT_CONV_FUNC(i8, i64, int8_t, int64_t)
MMSIMD_DEFAULT_CONV_FUNC(i8, u64, int8_t, uint64_t)
MMSIMD_DEFAULT_CONV_FUNC(i8, f32, int8_t, float)
MMSIMD_DEFAULT_CONV_FUNC(i8, f64, int8_t, double)
MMSIMD_DEFAULT_CONV_FUNC(u8, i16, uint8_t, int16_t)
MMSIMD_DEFAULT_CONV_FUNC(u8, u16, uint8_t, uint16_t)
MMSIMD_DEFAULT_CONV_FUNC(u8, i32, uint8_t, int32_t)
MMSIMD_DEFAULT_CONV_FUNC(u8, u32, uint8_t, uint32_t)
MMSIMD_DEFAULT_CONV_FUNC(u8, i64, uint8_t, int64_t)
MMSIMD_DEFAULT_CONV_FUNC(u8, u64, uint8_t, uint64_t)
MMSIMD_DEFAULT_CONV_FUNC(u8, f32, uint8_t, float)
MMSIMD_DEFAULT_CONV_FUNC(u8, f64, uint8_t, double)
MMSIMD_DEFAULT_CONV_FUNC(i16, i8, int16_t, int8_t)
MMSIMD_DEFAULT_CONV_FUNC(i16, u8, int16_t, uint8_t)
MMSIMD_DEFAULT_CONV_FUNC(i16, i32, int16_t, int32_t)
MMSIMD_DEFAULT_CONV_FUNC(i16, u32, int16_t, uint32_t)
MMSIMD_DEFAULT_CONV_FUNC(i16, i64, int16_t, int64_t)
MMSIMD_DEFAULT_CONV_FUNC(i16, u64, int16_t, uint64_t)
MMSIMD_DEFAULT_CONV_FUNC(i16, f32, int16_t, float)
MMSIMD_DEFAULT_CONV_FUNC(i16, f64, int16_t, double)
MMSIMD_DEFAULT_CONV_FUNC(u16, i8, uint16_t, int8_t)
MMSIMD_DEFAULT_CONV_FUNC(u16, u8, uint16_t, uint8_t)
MMSIMD_DEFAULT_CONV_FUNC(u16, i32, uint16_t, int32_t)
MMSIMD_DEFAULT_CONV_FUNC(u16, u32, uint16_t, uint32_t)
MMSIMD_DEFAULT_CONV_FUNC(u16, i64, uint16_t, int64_t)
MMSIMD_DEFAULT_CONV_FUNC(u16, u64, uint16_t, uint64_t)
MMSIMD_DEFAULT_CONV_FUNC(u16, f32, uint16_t, float)
MMSIMD_DEFAULT_CONV_FUNC(u16, f64, uint16_t, double)
MMSIMD_DEFAULT_CONV_FUNC(i32, i8, int32_t, int8_t)
MMSIMD_DEFAULT_CONV_FUNC(i32, u8, int32_t, uint8_t)
MMSIMD_DEFAULT_CONV_FUNC(i32, i16, int32_t, int16_t)
MMSIMD_DEFAULT_CONV_FUNC(i32, u16, int32_t, uint16_t)
MMSIMD_DEFAULT_CONV_FUNC(i32, i64, int32_t, int64_t)
MMSIMD_DEFAULT_CONV_FUNC(i32, u64, int32_t, uint64_t)
MMSIMD_DEFAULT_CONV_FUNC(i32, f32, int32_t, float)
MMSIMD_DEFAULT_CONV_FUNC(i32, f64, int32_t, double)
MMSIMD_DEFAULT_CONV_FUNC(u32, i8, uint32_t, int8_t)
MMSIMD_DEFAULT_CONV_FUNC(u32, u8, uint32_t, uint8_t)
MMSIMD_DEFAULT_CONV_FUNC(u32, i16, uint32_t, int16_t)
MMSIMD_DEFAULT_CONV_FUNC(u32, u16, uint32_t, uint16_t)
MMSIMD_DEFAULT_CONV_FUNC(u32, i64, uint32_t, int64_t)
MMSIMD_DEFAULT_CONV_FUNC(u32, u64, uint32_t, uint64_t)
MMSIMD_DEFAULT_CONV_FUNC(u32, f32, uint32_t, float)
MMSIMD_DEFAULT_CONV_FUNC(u32, f64, uint32_t, double)
MMSIMD_DEFAULT_CONV_FUNC(i64, i8, int64_t, int8_t)
MMSIMD_DEFAULT_CONV_FUNC(i64, u8, int64_t, uint8_t)
MMSIMD_DEFAULT_CONV_FUNC(i64, i16, int64_t, int16_t)
MMSIMD_DEFAULT_CONV_FUNC(i64, u16, int64_t, uint16_t)
MMSIMD_DEFAULT_CONV_FUNC(i64, i32, int64_t, int32_t)
MMSIMD_DEFAULT_CONV_FUNC(i64, u32, int64_t, uint32_t)
MMSIMD_DEFAULT_CONV_FUNC(i64, u64, int64_t, uint64_t)
MMSIMD_DEFAULT_CONV_FUNC(i64, f32, int64_t, float)
MMSIMD_DEFAULT_CONV_FUNC(i64, f64, int64_t, double)
MMSIMD_DEFAULT_CONV_FUNC(u64, i8, uint64_t, int8_t)
MMSIMD_DEFAULT_CONV_FUNC(u64, u8, uint64_t, uint8_t)
MMSIMD_DEFAULT_CONV_FUNC(u64, i16, uint64_t, int16_t)
MMSIMD_DEFAULT_CONV_FUNC(u64, u16, uint64_t, uint16_t)
MMSIMD_DEFAULT_CONV_FUNC(u64, i32, uint64_t, int32_t)
MMSIMD_DEFAULT_CONV_FUNC(u64, u32, uint64_t, uint32_t)
MMSIMD_DEFAULT_CONV_FUNC(u64, i64, uint64_t, int64_t)
MMSIMD_DEFAULT_CONV_FUNC(u64, f32, uint64_t, float)
MMSIMD_DEFAULT_CONV_FUNC(u64, f64, uint64_t, double)
MMSIMD_DEFAULT_CONV_FUNC(f32, i8, float, int8_t)
MMSIMD_DEFAULT_CONV_FUNC(f32, u8, float, uint8_t)
MMSIMD_DEFAULT_CONV_FUNC(f32, i16, float, int16_t)
MMSIMD_DEFAULT_CONV_FUNC(f32, u16, float, uint16_t)
MMSIMD_DEFAULT_CONV_FUNC(f32, i32, float, int32_t)
MMSIMD_DEFAULT_CONV_FUNC(f32, u32, float, uint32_t)
MMSIMD_DEFAULT_CONV_FUNC(f32, i64, float, int64_t)
MMSIMD_DEFAULT_CONV_FUNC(f32, u64, float, uint64_t)
MMSIMD_DEFAULT_CONV_FUNC(f32, f64, float, double)
MMSIMD_DEFAULT_CONV_FUNC(f64, i8, double, int8_t)
MMSIMD_DEFAULT_CONV_FUNC(f64, u8, double, uint8_t)
MMSIMD_DEFAULT_CONV_FUNC(f64, i16, double, int16_t)
MMSIMD_DEFAULT_CONV_FUNC(f64, u16, double, uint16_t)
MMSIMD_DEFAULT_CONV_FUNC(f64, i32, double, int32_t)
MMSIMD_DEFAULT_CONV_FUNC(f64, u32, double, uint32_t)
MMSIMD_DEFAULT_CONV_FUNC(f64, i64, double, int64_t)
MMSIMD_DEFAULT_CONV_FUNC(f64, u64, double, uint64_t)
MMSIMD_DEFAULT_CONV_FUNC(f64, f32, double, float)

#undef MMSIMD_DEFAULT_CONV_FUNC

#define MMSIMD_DEFAULT_FIND_FUNC(NAME, TYPE) \
    static mmint_t mmsimd_default_##NAME##_find(const TYPE* _buf, mmint_t _len, TYPE _val) \
    { \
        for (mmint_t i = 0; i < _len; ++i) \
        { \
            if (_buf[i] == _val) \
            { \
                return i; \
            } \
        } \
        return -1; \
    }

MMSIMD_DEFAULT_FIND_FUNC(i8, int8_t)
MMSIMD_DEFAULT_FIND_FUNC(u8, uint8_t)
MMSIMD_DEFAULT_FIND_FUNC(i16, int16_t)
MMSIMD_DEFAULT_FIND_FUNC(u16, uint16_t)
MMSIMD_DEFAULT_FIND_FUNC(i32, int32_t)
MMSIMD_DEFAULT_FIND_FUNC(u32, uint32_t)
MMSIMD_DEFAULT_FIND_FUNC(i64, int64_t)
MMSIMD_DEFAULT_FIND_FUNC(u64, uint64_t)
//MMSIMD_DEFAULT_FIND_FUNC(f32, float)
//MMSIMD_DEFAULT_FIND_FUNC(f64, double)

#undef MMSIMD_DEFAULT_FIND_FUNC

#define MMSIMD_DEFAULT_RFIND_FUNC(NAME, TYPE) \
    static mmint_t mmsimd_default_##NAME##_rfind(const TYPE* _buf, mmint_t _len, TYPE _val) \
    { \
        for (mmint_t i = _len - 1; i >= 0; --i) \
        { \
            if (_buf[i] == _val) \
            { \
                return i; \
            } \
        } \
        return -1; \
    }

MMSIMD_DEFAULT_RFIND_FUNC(i8, int8_t)
MMSIMD_DEFAULT_RFIND_FUNC(u8, uint8_t)
MMSIMD_DEFAULT_RFIND_FUNC(i16, int16_t)
MMSIMD_DEFAULT_RFIND_FUNC(u16, uint16_t)
MMSIMD_DEFAULT_RFIND_FUNC(i32, int32_t)
MMSIMD_DEFAULT_RFIND_FUNC(u32, uint32_t)
MMSIMD_DEFAULT_RFIND_FUNC(i64, int64_t)
MMSIMD_DEFAULT_RFIND_FUNC(u64, uint64_t)
//MMSIMD_DEFAULT_RFIND_FUNC(f32, float)
//MMSIMD_DEFAULT_RFIND_FUNC(f64, double)

#undef MMSIMD_DEFAULT_RFIND_FUNC

MEME_EXTERN_C_SCOPE_ENDED
