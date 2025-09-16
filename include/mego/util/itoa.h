
#include <mego/util/utoa.h>

#define MGU_MAX_SIGNED_BASESTRLEN(BITS, RADIX) \
    (MGU_RADIX_LOG10_BASE(RADIX) == 0 ? (BITS + 1) : (((BITS) * 30103) / MGU_RADIX_LOG10_BASE(RADIX) + 2))

#define MGU_ITOA_HELPER(UTYPE, VAL, RADIX, BUF, BUF_SIZE) \
    do { \
        static const char hex_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; \
        char* index = BUF; \
        UTYPE tmp; \
        int negative = 0; \
        if (MG_SYM__UNLIKELY(RADIX < 2 || RADIX > 36)) \
            return NULL; \
        if (MG_SYM__UNLIKELY(BUF_SIZE <= MGU_MAX_SIGNED_BASESTRLEN(sizeof(UTYPE) * CHAR_BIT, RADIX))) \
            return NULL; \
        if (VAL < 0) { \
            negative = 1; \
            tmp = (UTYPE)(-VAL); \
        } else { \
            tmp = (UTYPE)VAL; \
        } \
        do { \
            UTYPE rem = (tmp) % (RADIX); \
            *index++ = hex_digits[rem]; \
            (tmp) /= (RADIX); \
        } while ((tmp) != 0); \
        if (negative) { \
            *index++ = '-'; \
        } \
        *index-- = '\0'; \
        for (char* left = BUF; left < index; ++left, --index) \
        { \
            char temp = *left; \
            *left = *index; \
            *index = temp; \
        } \
        return BUF; \
    } while (0)

MG_CAPI_INLINE char* mgu_itoa8(int8_t _value, int _radix, char* _buf, size_t _buf_size)
{
    MGU_ITOA_HELPER(uint8_t, _value, _radix, _buf, _buf_size);
}

MG_CAPI_INLINE char* mgu_itoa16(int16_t _value, int _radix, char* _buf, size_t _buf_size)
{
    MGU_ITOA_HELPER(uint16_t, _value, _radix, _buf, _buf_size);
}

MG_CAPI_INLINE char* mgu_itoa32(int32_t _value, int _radix, char* _buf, size_t _buf_size)
{
    MGU_ITOA_HELPER(uint32_t, _value, _radix, _buf, _buf_size);
}

MG_CAPI_INLINE char* mgu_itoa64(int64_t _value, int _radix, char* _buf, size_t _buf_size)
{
    MGU_ITOA_HELPER(uint64_t, _value, _radix, _buf, _buf_size);
}
