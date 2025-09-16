
#ifndef MEGO_UTIL_UTOA_H_INCLUDED
#define MEGO_UTIL_UTOA_H_INCLUDED

#include <mego/predef/symbol/likely.h>
#include <mego/predef/symbol/inline.h>

#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include <limits.h>

#define MGU_RADIX_LOG10_BASE(RADIX) ( \
    (RADIX) == 2  ? 30103  : \
    (RADIX) == 3  ? 47712  : \
    (RADIX) == 4  ? 60206  : \
    (RADIX) == 5  ? 69897  : \
    (RADIX) == 6  ? 77815  : \
    (RADIX) == 7  ? 84510  : \
    (RADIX) == 8  ? 90309  : \
    (RADIX) == 9  ? 95424  : \
    (RADIX) == 10 ? 100000 : \
    (RADIX) == 11 ? 104137 : \
    (RADIX) == 12 ? 107957 : \
    (RADIX) == 13 ? 111469 : \
    (RADIX) == 14 ? 114735 : \
    (RADIX) == 15 ? 117783 : \
    (RADIX) == 16 ? 120679 : \
    (RADIX) == 17 ? 123384 : \
    (RADIX) == 18 ? 125892 : \
    (RADIX) == 19 ? 128303 : \
    (RADIX) == 20 ? 130624 : \
    (RADIX) == 21 ? 132877 : \
    (RADIX) == 22 ? 135055 : \
    (RADIX) == 23 ? 137170 : \
    (RADIX) == 24 ? 139228 : \
    (RADIX) == 25 ? 141238 : \
    (RADIX) == 26 ? 143205 : \
    (RADIX) == 27 ? 145130 : \
    (RADIX) == 28 ? 147016 : \
    (RADIX) == 29 ? 148866 : \
    (RADIX) == 30 ? 150681 : \
    (RADIX) == 31 ? 152463 : \
    (RADIX) == 32 ? 154203 : \
    (RADIX) == 33 ? 155912 : \
    (RADIX) == 34 ? 157581 : \
    (RADIX) == 35 ? 159215 : \
    (RADIX) == 36 ? 160819 : 0)

#define MGU_MAX_UNSIGNED_BASESTRLEN(BITS, RADIX) \
    (MGU_RADIX_LOG10_BASE(RADIX) == 0 ? (BITS) : (((BITS) * 30103) / MGU_RADIX_LOG10_BASE(RADIX) + 1))

#define MGU_UTOA_HELPER(TYPE, VALUE, RADIX, BUF, BUF_SIZE) \
    do { \
        static const char hex_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; \
        char* index = BUF; \
        if (MG_SYM__UNLIKELY(RADIX < 2 || RADIX > 36)) \
            return NULL; \
        if (MG_SYM__UNLIKELY(BUF_SIZE <= MGU_MAX_UNSIGNED_BASESTRLEN(sizeof(TYPE) * CHAR_BIT, RADIX))) \
            return NULL; \
        do { \
            TYPE rem = (VALUE) % (RADIX); \
            *index++ = hex_digits[rem]; \
            (VALUE) /= (RADIX); \
        } while ((VALUE) != 0); \
        *index-- = '\0'; \
        \
        for (char* left = BUF; left < index; ++left, --index) \
        { \
            char temp = *left; \
            *left = *index; \
            *index = temp; \
        } \
        return BUF; \
    } while (0)

MG_CAPI_INLINE char* mgu_utoa8(uint8_t _value, int _radix, char* _buf, size_t _buf_size)
{
    MGU_UTOA_HELPER(uint8_t, _value, _radix, _buf, _buf_size);
}

MG_CAPI_INLINE char* mgu_utoa16(uint16_t _value, int _radix, char* _buf, size_t _buf_size)
{
    MGU_UTOA_HELPER(uint16_t, _value, _radix, _buf, _buf_size);
}

MG_CAPI_INLINE char* mgu_utoa32(uint32_t _value, int _radix, char* _buf, size_t _buf_size)
{
    MGU_UTOA_HELPER(uint32_t, _value, _radix, _buf, _buf_size);
}

MG_CAPI_INLINE char* mgu_utoa64(uint64_t _value, int _radix, char* _buf, size_t _buf_size)
{
    MGU_UTOA_HELPER(uint64_t, _value, _radix, _buf, _buf_size);
}

#endif // !MEGO_UTIL_UTOA_H_INCLUDED
