
#ifndef MEGO_UTF_U16RUNE_H_INCLUDED
#define MEGO_UTF_U16RUNE_H_INCLUDED

#include <meme/string_fwd.h>
#include <meme/utf/u8rune_fwd.h>
#include <meme/utf/u16rune_fwd.h>

MEME_EXTERN_C_SCOPE_START
    
inline int
mmutf_u16rune_char_size(uint16_t _ch)
{
    if (_ch < 0xD800) // [0x0000¨E0xD7FF]
        return 1;
    
    if (_ch < 0xDC00) // [0xD800¨E0xDBFF] [0xDC00¨E0xDFFF]
        return 2;

    if (_ch < 0xE000)
        return -1;

    // [0xE000¨E0xFFFF]
    return 1;
}
        
inline int
mmutf_u16rune_valid(const uint16_t* _buf, MemeInteger_t _len)
{
    uint16_t ch0;

    if (_len < 1)
        return -1;

    ch0 = _buf[0];

    if (ch0 < 0xD800) // [0x0000¨E0xD7FF]
        return 1;

    if (ch0 < 0xDC00) { // [0xD800¨E0xDBFF] [0xDC00¨E0xDFFF]
        if (_len < 2)
            return -1;
        if (_buf[1] >> 10 != 0x37)
            return -1;
        return 2;
    }

    if (ch0 < 0xE000)
        return -1;

    // [0xE000¨E0xFFFF]
    return 1;
}

inline int
mmutf_u16char_size_from8(const MemeByte_t* _ch, MemeInteger_t _len, int* _ch_size)
{
    uint32_t ch = 0;
    int dstCharSize = 0;
    int srcCharSize = mmutf_u8rune_get_u32(_ch, _len, &ch);
    if (srcCharSize < 0)
        return -1;

    dstCharSize = mmutf_u16char_size_from32(&ch, 1, &dstCharSize);
    if (dstCharSize < 0)
        return -1;

    if (_ch_size)
        *_ch_size = srcCharSize;
    return dstCharSize;
}

inline int
mmutf_u16char_size_from32(const uint32_t* _ch, MemeInteger_t _len, int* _ch_size)
{
    if (*_ch < 0xD800) {  // [0x0000¨E0xD7FF]
        if (_ch_size)
            *_ch_size = 1;
        return 1;
    }

    if (*_ch < 0x10000) {
        if (*_ch < 0xE000)
            return -1;
        
        if (_ch_size)
            *_ch_size = 1;
        return 1;
    }

    if (*_ch < 0x110000) {
        if (_ch_size)
            *_ch_size = 1;
        return 2;
    }
    
    return -1;
}

inline int
mmutf_u16rune_get_u32(
    const uint16_t* _buf, MemeInteger_t _len, uint32_t* _value)
{
    uint16_t ch0;

    if (_len < 1)
        return -1;

    ch0 = _buf[0];

    if (ch0 < 0xD800) {  // [0x0000¨E0xD7FF]
        *_value = ch0;
        return 1;
    }

    if (ch0 < 0xDC00) {  // [0xD800¨E0xDBFF] [0xDC00¨E0xDFFF]
        if (_len < 2)
            return -1;
        if (_buf[1] >> 10 != 0x37)
            return -1;

        *_value = (ch0 << 10) + _buf[1] - 0x35FDC00;
        return 2;
    }

    if (ch0 < 0xE000)
        return -1;

    *_value = ch0;
    return 1;
}

inline int
mmutf_u16rune_set_u32(
    uint16_t* _buf, MemeInteger_t _len, uint32_t _value)
{
    if (_value < 0xD800) {  // [0x0000¨E0xD7FF]
        if (_len < 1)
            return -1;

        _buf[0] = (uint16_t)_value;
        return 1;
    }

    else if (_value < 0x10000) {
        if (_len < 1)
            return -1;
        
        if (_value < 0xE000)
            return -1;

        _buf[0] = (uint16_t)_value;
        return 1;
    }

    else if (_value < 0x110000) {
        if (_len < 2)
            return -1;

        _buf[0] = (uint16_t)(0xD7C0 + (_value >> 10));
        _buf[1] = (uint16_t)(0xDC00 + (_value & 0x3FF));
        return 2;
    }
    
    return -1;
}

MEME_EXTERN_C_SCOPE_ENDED

#endif // !MEGO_UTF_U16RUNE_H_INCLUDED

