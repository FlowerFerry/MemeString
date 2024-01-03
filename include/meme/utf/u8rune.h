
#ifndef MEGO_UTF_U8RUNE_H_INCLUDED
#define MEGO_UTF_U8RUNE_H_INCLUDED

#include <meme/string_fwd.h>
#include <meme/utf/u8rune_fwd.h>
#include <meme/utf/u16rune.h>

MEME_EXTERN_C_SCOPE_START

static inline int
mmutf_u8rune_char_size(uint8_t _ch)
{
    if (_ch < 0x80) // 0XXXXXXX
        return 1;
    if (_ch < 0xC0)
        return -1;

    if (_ch < 0xE0) // 110XXXXX 10XXXXXX
        return 2;
    if (_ch < 0xF0) // 1110XXXX 10XXXXXX 10XXXXXX
        return 3;
    if (_ch < 0xF8) // 11110XXX 10XXXXXX 10XXXXXX 10XXXXXX
        return 4;
    if (_ch < 0xFC) // 111110XX 10XXXXXX 10XXXXXX 10XXXXXX 10XXXXXX
        return 5;
    if (_ch < 0xFE) // 1111110X 10XXXXXX 10XXXXXX 10XXXXXX 10XXXXXX 10XXXXXX
        return 6;

    return -1;
}

inline int
mmutf_u8rune_valid(const MemeByte_t* _buf, MemeInteger_t _len)
{
    uint8_t ch0;

    if (_len < 1)
        return -1;

    ch0 = _buf[0];

    if (ch0 < 0x80) {   // 0XXXXXXX
        return 1;
    }

    if (ch0 < 0xC0)
        return -1;

    if (ch0 < 0xE0) {   // 110XXXXX 10XXXXXX
        if (_len < 2)
            return -1;
        if (_buf[1] >> 6 != 2)
            return -1;

        return 2;
    }

    if (ch0 < 0xF0) {   // 1110XXXX 10XXXXXX 10XXXXXX
        if (_len < 3)
            return -1;
        if (_buf[1] >> 6 != 2) return -1;
        if (_buf[2] >> 6 != 2) return -1;

        return 3;
    }

    if (ch0 < 0xF8) {   // 11110XXX 10XXXXXX 10XXXXXX 10XXXXXX
        if (_len < 4)
            return -1;
        if (_buf[1] >> 6 != 2) return -1;
        if (_buf[2] >> 6 != 2) return -1;
        if (_buf[3] >> 6 != 2) return -1;

        return 4;
    }

    if (ch0 < 0xFC) {   // 111110XX 10XXXXXX 10XXXXXX 10XXXXXX 10XXXXXX
        if (_len < 5)
            return -1;
        if (_buf[1] >> 6 != 2) return -1;
        if (_buf[2] >> 6 != 2) return -1;
        if (_buf[3] >> 6 != 2) return -1;
        if (_buf[4] >> 6 != 2) return -1;

        return 5;
    }

    if (ch0 < 0xFE) {   // 1111110X 10XXXXXX 10XXXXXX 10XXXXXX 10XXXXXX 10XXXXXX
        if (_len < 6)
            return -1;
        if (_buf[1] >> 6 != 2) return -1;
        if (_buf[2] >> 6 != 2) return -1;
        if (_buf[3] >> 6 != 2) return -1;
        if (_buf[4] >> 6 != 2) return -1;
        if (_buf[5] >> 6 != 2) return -1;

        return 6;
    }

    return -1;
}

inline int
mmutf_u8char_size_from16(const uint16_t* _ch, MemeInteger_t _len, int* _ch_size)
{
    uint32_t ch = 0;
    int dstCharSize = 0;
    int srcCharSize = mmutf_u16rune_get_u32(_ch, _len, &ch);
    if (srcCharSize < 0)
        return -1;
    
    dstCharSize = mmutf_u8char_size_from32(&ch, 1, &dstCharSize);
    if (dstCharSize < 0)
        return -1;

    if (_ch_size)
        *_ch_size = srcCharSize;
    return dstCharSize;
}

inline int
mmutf_u8char_size_from32(const uint32_t* _ch, MemeInteger_t _len, int* _ch_size)
{
    (void*)_len;

    if (*_ch < 0x80) {  // 0XXXXXXX
        if (_ch_size)
            *_ch_size = 1;
        return 1;
    }

    if (*_ch < 0x800) { // 110XXXXX 10XXXXXX
        if (_ch_size)
            *_ch_size = 1;
        return 2;
    }

    if (*_ch < 0x10000) {   // 1110XXXX 10XXXXXX 10XXXXXX
        if (_ch_size)
            *_ch_size = 1;
        return 3;
    }

    if (*_ch < 0x200000) {  // 11110XXX 10XXXXXX 10XXXXXX 10XXXXXX
        if (_ch_size)
            *_ch_size = 1;
        return 4;
    }

    if (*_ch < 0x4000000) { // 111110XX 10XXXXXX 10XXXXXX 10XXXXXX 10XXXXXX
        if (_ch_size)
            *_ch_size = 1;
        return 5;
    }

    if (*_ch < 0x80000000) {    // 1111110X 10XXXXXX 10XXXXXX 10XXXXXX 10XXXXXX 10XXXXXX
        if (_ch_size)
            *_ch_size = 1;
        return 6;
    }

    return -1;
}

inline int
mmutf_u8rune_get_u32(
    const MemeByte_t* _buf, MemeInteger_t _len, uint32_t* _value)
{
    uint8_t ch0;
    
    if (_len < 1)
        return -1;
    
    ch0 = _buf[0];

    if (ch0 < 0x80) {   // 0XXXXXXX
        *_value = ch0;
        return 1;
    }

    if (ch0 < 0xC0)
        return -1;

    if (ch0 < 0xE0) {   // 110XXXXX 10XXXXXX
        if (_len < 2)
            return -1;
        if (_buf[1] >> 6 != 2)
            return -1;

        *_value = (ch0 << 6) + _buf[1] - 0x3080;
        return 2;
    }

    if (ch0 < 0xF0) {   // 1110XXXX 10XXXXXX 10XXXXXX
        if (_len < 3)
            return -1;
        if (_buf[1] >> 6 != 2) return -1;
        if (_buf[2] >> 6 != 2) return -1;

        *_value = (ch0 << 12) + (_buf[1] << 6) + _buf[2] - 0xE2080;
        return 3;
    }
    
    if (ch0 < 0xF8) {   // 11110XXX 10XXXXXX 10XXXXXX 10XXXXXX
        if (_len < 4)
            return -1;
        if (_buf[1] >> 6 != 2) return -1;
        if (_buf[2] >> 6 != 2) return -1;
        if (_buf[3] >> 6 != 2) return -1;
        
        *_value = (ch0 << 18) + (_buf[1] << 12) + (_buf[2] << 6) + _buf[3] - 0x3C82080;
        return 4;
    }
    
    if (ch0 < 0xFC) {   // 111110XX 10XXXXXX 10XXXXXX 10XXXXXX 10XXXXXX
        if (_len < 5)
            return -1;
        if (_buf[1] >> 6 != 2) return -1;
        if (_buf[2] >> 6 != 2) return -1;
        if (_buf[3] >> 6 != 2) return -1;
        if (_buf[4] >> 6 != 2) return -1;
        
        *_value = (ch0 << 24) + (_buf[1] << 18) + (_buf[2] << 12) + (_buf[3] << 6) + _buf[4] - 0xFA082080;
        return 5;
    }
    if (ch0 < 0xFE) {   // 1111110X 10XXXXXX 10XXXXXX 10XXXXXX 10XXXXXX 10XXXXXX
        if (_len < 6)
            return -1;
        if (_buf[1] >> 6 != 2) return -1;
        if (_buf[2] >> 6 != 2) return -1;
        if (_buf[3] >> 6 != 2) return -1;
        if (_buf[4] >> 6 != 2) return -1;
        if (_buf[5] >> 6 != 2) return -1;
        
        *_value = (ch0 << 30) + (_buf[1] << 24) + (_buf[2] << 18) + (_buf[3] << 12) + (_buf[4] << 6) + _buf[5] - 0x82082080;
        return 6;
    }
    
    return -1;
}

inline int
mmutf_u8rune_set_u32(
    MemeByte_t* _buf, MemeInteger_t _len, uint32_t _value)
{
    int byteSize = -1;

    if (_value < 0x80) { // 0XXXXXXX
        if (_len < 1)
            return -1;

        _buf[0] = (uint8_t)_value;
        return 1;
    }

    else if (_value < 0x800) { // 110XXXXX 10XXXXXX
        if (_len < 2)
            return -1;

        *_buf++ = (uint8_t)(0xC0 + (_value >> 6));
        byteSize = 2;
    }

    else if (_value < 0x10000) { // 1110XXXX 10XXXXXX 10XXXXXX
        if (_len < 3)
            return -1;

        *_buf++ = (uint8_t)(0xE0 + (_value >> 12));
        byteSize = 3;
    }

    else if (_value < 0x200000) { // 11110XXX 10XXXXXX 10XXXXXX 10XXXXXX
        if (_len < 4)
            return -1;

        *_buf++ = (uint8_t)(0xF0 + (_value >> 18));
        byteSize = 4;
    }

    else if (_value < 0x4000000) { // 111110XX 10XXXXXX 10XXXXXX 10XXXXXX 10XXXXXX
        if (_len < 5)
            return -1;

        *_buf++ = 0x80 | ((_value >> 24) & 0x3F);
        byteSize = 5;
    }

    else if (_value < 0x80000000) { // 1111110X 10XXXXXX 10XXXXXX 10XXXXXX 10XXXXXX 10XXXXXX
        if (_len < 6)
            return -1;

        *_buf++ = 0xFC + (_value >> 30);
        byteSize = 6;
    }

    switch (byteSize - 1) {
    case 5: *_buf++ = 0x80 + ((_value >> 24) & 0x3F);
    case 4: *_buf++ = 0x80 + ((_value >> 18) & 0x3F);
    case 3: *_buf++ = 0x80 + ((_value >> 12) & 0x3F);
    case 2: *_buf++ = 0x80 + ((_value >>  6) & 0x3F);
    case 1: *_buf++ = 0x80 + ( _value        & 0x3F);
    default:;
    }

    return byteSize;
}

MEME_EXTERN_C_SCOPE_ENDED

#endif // !MEGO_UTF_U8RUNE_H_INCLUDED
