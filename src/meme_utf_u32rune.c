
#include <meme/utf/u8rune.h>
#include <meme/utf/u16rune.h>
#include <meme/utf/u32rune.h>


MEME_EXTERN_C MEME_API int MEME_STDCALL
mmutf_u32rune_char_size(uint32_t _ch)
{
    return 1;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL
mmutf_u32rune_valid(const uint32_t* _buf, MemeInteger_t _len)
{
    /* Validate a single UTF-32 code unit.
       A valid code unit must be within 0x0..0x7FFFFFFF (the project supports
       long encodings beyond the standard Unicode range U+0..U+10FFFF).
       Also reject 0xFFFE and 0xFFFF which are non-characters reserved for
       internal use. */

    uint32_t ch;

    if (_len < 1)
        return -1;

    ch = _buf[0];

#if 0
    /* Disabled: reject standard Unicode surrogates (U+D800..U+DFFF).
       The project intentionally permits surrogate code points so that they
       round-trip through the internal UTF-8 encoding.  */
    if (ch >= 0xD800 && ch <= 0xDFFF)
        return -1;
#endif
    if (ch == 0xFFFE || ch == 0xFFFF)
        return -1;

    return 1;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL
mmutf_u32char_size_from8(const MemeByte_t* _ch, MemeInteger_t _len, int* _ch_size)
{
    int size = mmutf_u8rune_char_size(*_ch);
    if (size < 0)
        return -1;

    if (_ch_size)
        *_ch_size = size;
    return 1;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL
mmutf_u32char_size_from16(const uint16_t* _ch, MemeInteger_t _len, int* _ch_size)
{
    int size = mmutf_u16rune_char_size(*_ch);
    if (size < 0)
        return -1;

    if (_ch_size)
        *_ch_size = size;
    return 1;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL
mmutf_u32rune_get_u32(
    const uint32_t* _buf, MemeInteger_t _len, uint32_t* _value)
{
    if (_len < 1)
        return -1;

    *_value = (uint32_t)*_buf;
    return 1;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL
mmutf_u32rune_set_u32(
    uint32_t* _buf, MemeInteger_t _len, uint32_t _value)
{
    if (_len < 1)
        return -1;

    *_buf = _value;
    return 1;
}
