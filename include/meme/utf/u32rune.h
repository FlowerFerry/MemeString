
#ifndef MEME_UTF_U32RUNE_H_INCLUDED
#define MEME_UTF_U32RUNE_H_INCLUDED

#include <meme/string_fwd.h>
#include <meme/utf/u8rune_fwd.h>
#include <meme/utf/u16rune.h>

MEME_EXTERN_C_SCOPE_START

inline int
mmutf_u32rune_char_size(uint32_t _ch)
{
    return 1;
}

inline int
mmutf_u32rune_valid(const uint32_t* _buf, MemeInteger_t _len)
{
    return 1;
}

inline int
mmutf_u32char_size_from8(const MemeByte_t* _ch, MemeInteger_t _len, int* _ch_size)
{
    int size = mmutf_u8rune_char_size(*_ch);
    if (size < 0)
        return -1;

    if (_ch_size)
        *_ch_size = size;
    return 1;
}

inline int
mmutf_u32char_size_from16(const uint16_t* _ch, MemeInteger_t _len, int* _ch_size)
{
    int size = mmutf_u16rune_char_size(*_ch);
    if (size < 0)
        return -1;
    
    if (_ch_size)
        *_ch_size = size;
    return 1;
}

inline int
mmutf_u32rune_get_u32(
    const uint32_t* _buf, MemeInteger_t _len, uint32_t* _value)
{
    if (_len < 1)
        return -1;
    
    *_value = (uint32_t)*_buf;
    return 1;
}

inline int
mmutf_u32rune_set_u32(
    uint32_t* _buf, MemeInteger_t _len, uint32_t _value)
{
    if (_len < 1)
        return -1;

    *_buf = _value;
    return 1;
}

MEME_EXTERN_C_SCOPE_ENDED

#endif // !MEME_UTF_U32RUNE_H_INCLUDED
