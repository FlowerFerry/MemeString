
#ifndef MEME_UTF_DEFAULT_CONVERT_H_INCLUDED
#define MEME_UTF_DEFAULT_CONVERT_H_INCLUDED

#include <meme/string_fwd.h>
#include <mego/hardware/cpu/instruction.h>
#include <meme/utf/u8rune.h>
#include <meme/utf/u16rune.h>
#include <meme/utf/u32rune.h>

#include <assert.h>

MEME_EXTERN_C_SCOPE_START

mghw_simd_instruction_t
mmutf_default_required_simd_instruction_sets(void*);

MemeInteger_t
mmutf_default_u8valid(
    const MemeByte_t* _buf, MemeInteger_t _len, void*);
MemeInteger_t
mmutf_default_u16valid(
    const uint16_t* _buf, MemeInteger_t _len, void*);

MemeInteger_t 
mmutf_default_u8rune_size(
    const MemeByte_t* _buf, MemeInteger_t _len, void*);

MemeInteger_t 
mmutf_default_u16rune_size(
    const uint16_t* _buf, MemeInteger_t _len, void*);

MemeInteger_t 
mmutf_default_char_size_u8from16(
    const uint16_t* _buf, MemeInteger_t _len, void*);

MemeInteger_t 
mmutf_default_char_size_u16from8(
    const MemeByte_t* _buf, MemeInteger_t _len, void*);

MemeInteger_t 
mmutf_default_char_size_u8from32(
    const uint32_t* _buf, MemeInteger_t _len, void*);

MemeInteger_t 
mmutf_default_char_size_u16from32(
    const uint32_t* _buf, MemeInteger_t _len, void*);

MemeInteger_t
mmutf_default_char_size_u32from16(
    const uint16_t* _buf, MemeInteger_t _len, void*);

MemeInteger_t
mmutf_default_char_size_u32from8(
    const MemeByte_t* _buf, MemeInteger_t _len, void*);

MemeInteger_t
mmutf_default_convert_u8to16(
    const MemeByte_t* _buf, MemeInteger_t _len, uint16_t* _out, void*);

MemeInteger_t
mmutf_default_convert_u8to32(
    const MemeByte_t* _buf, MemeInteger_t _len, uint32_t* _out, void*);

MemeInteger_t 
mmutf_default_convert_u16to8(
    const uint16_t* _buf, MemeInteger_t _len, MemeByte_t* _out, void*);

MemeInteger_t 
mmutf_default_convert_u16to32(
    const uint16_t* _buf, MemeInteger_t _len, uint32_t* _out, void*);

MemeInteger_t
mmutf_default_convert_u32to8(
    const uint32_t* _buf, MemeInteger_t _len, MemeByte_t* _out, void*);

MemeInteger_t
mmutf_default_convert_u32to16(
    const uint32_t* _buf, MemeInteger_t _len, uint16_t* _out, void*);



inline mghw_simd_instruction_t
mmutf_default_required_simd_instruction_sets(void* _data)
{
    return MGHW_SIMD_INSTRUCTION__DEFAULT;
}

inline MemeInteger_t
mmutf_default_u8valid(
    const MemeByte_t* _buf, MemeInteger_t _len, void* _data)
{
    int byteSize = 0;
    MemeInteger_t pos = 0;

    if (!_buf) {
        return pos;
    }

    while (_len > 0) {
        byteSize = mmutf_u8rune_valid(_buf, _len);
        if (byteSize == -1) {
            break;
        }
        _buf += byteSize;
        _len -= byteSize;
        pos  += byteSize;
    }

    return pos;
}

inline MemeInteger_t
mmutf_default_u16valid(
    const uint16_t* _buf, MemeInteger_t _len, void* _data)
{
    int charSize = 0;
    MemeInteger_t pos = 0;

    if (!_buf) {
        return pos;
    }

    while (_len > 0) {
        charSize = mmutf_u16rune_valid(_buf, _len);
        if (charSize == -1) {
            break;
        }
        _buf += charSize;
        _len -= charSize;
        pos  += charSize;
    }

    return pos;
}

inline MemeInteger_t
mmutf_default_u8rune_size(
    const MemeByte_t* _buf, MemeInteger_t _len, void* _data)
{
    int byteSize = 0;
    MemeInteger_t runeSize = 0;

    if (!_buf) {
        return runeSize;
    }
    
    while (_len > 0) {
        byteSize = mmutf_u8rune_char_size(*_buf);
        if (byteSize == -1) {
            break;
        }
        _buf += byteSize;
        _len -= byteSize;

        if (_len >= 0)
            ++runeSize;
    }
    
    return runeSize;
}

inline MemeInteger_t
mmutf_default_u16rune_size(
    const uint16_t* _buf, MemeInteger_t _len, void* _data)
{
    int charSize = 0;
    MemeInteger_t runeSize = 0;

    if (!_buf) {
        return runeSize;
    }

    while (_len > 0) {
        charSize = mmutf_u16rune_char_size(*_buf);
        if (charSize == -1) {
            break;
        }
        
        _buf += charSize;
        _len -= charSize;

        if (_len >= 0)
            ++runeSize;
    }

    return runeSize;
}


#define MMUTF_DEFAULT_CHAR_SIZE_X_FROM_Y(UTFX, UTFY, BUF, LEN) \
do {\
    int srcCharSize = 0; \
    int dstCharSize = 0; \
    int sum = 0; \
    if (!BUF) \
        return 0; \
    \
    while (LEN > 0) {\
        dstCharSize = mmutf_##UTFX##char_size_from##UTFY(BUF, LEN, &srcCharSize); \
        if (dstCharSize == -1) \
            break; \
        BUF += srcCharSize; \
        LEN -= srcCharSize; \
        sum += dstCharSize; \
    } \
\
    return sum; \
} while (0)

inline MemeInteger_t
mmutf_default_char_size_u8from16(
    const uint16_t* _buf, MemeInteger_t _len, void* _data)
{
    MMUTF_DEFAULT_CHAR_SIZE_X_FROM_Y(u8, 16, _buf, _len);
}

inline MemeInteger_t
mmutf_default_char_size_u8from32(
    const uint32_t* _buf, MemeInteger_t _len, void* _data)
{
    MMUTF_DEFAULT_CHAR_SIZE_X_FROM_Y(u8, 32, _buf, _len);
}

inline MemeInteger_t
mmutf_default_char_size_u16from8(
    const MemeByte_t* _buf, MemeInteger_t _len, void* _data)
{
    MMUTF_DEFAULT_CHAR_SIZE_X_FROM_Y(u16, 8, _buf, _len);
}

inline MemeInteger_t
mmutf_default_char_size_u16from32(
    const uint32_t* _buf, MemeInteger_t _len, void* _data)
{
    MMUTF_DEFAULT_CHAR_SIZE_X_FROM_Y(u16, 32, _buf, _len);
}

inline MemeInteger_t
mmutf_default_char_size_u32from8(
    const MemeByte_t* _buf, MemeInteger_t _len, void* _data)
{
    MMUTF_DEFAULT_CHAR_SIZE_X_FROM_Y(u32, 8, _buf, _len);
}

inline MemeInteger_t
mmutf_default_char_size_u32from16(
    const uint16_t* _buf, MemeInteger_t _len, void* _data)
{
    MMUTF_DEFAULT_CHAR_SIZE_X_FROM_Y(u32, 16, _buf, _len);
}

#define MMUTF_DEFAULT_CONVERT_X_TO_Y(UTFX, UTFY, BUF, LEN, OUT) \
do{\
    MemeInteger_t pos = 0; \
    int srcCharSize   = 0; \
    int dstCharSize   = 0; \
    uint32_t u32rune  = 0; \
    \
    if (!BUF) { \
        return pos; \
    } \
    \
    while (LEN > 0) { \
        srcCharSize = mmutf_##UTFX##rune_get_u32(BUF, LEN, &u32rune); \
        if (srcCharSize == -1) { \
            break; \
        } \
        \
        dstCharSize = mmutf_##UTFY##rune_set_u32(OUT + pos, INTPTR_MAX, u32rune); \
        if (dstCharSize == -1) { \
            break; \
        } \
        \
        BUF += srcCharSize; \
        LEN -= srcCharSize; \
        pos += dstCharSize; \
    } \
    return pos; \
} while(0)

inline MemeInteger_t
mmutf_default_convert_u8to16(
    const MemeByte_t* _buf, MemeInteger_t _len, uint16_t* _out, void* _data)
{
    /* Hand-written rather than MMUTF_DEFAULT_CONVERT_X_TO_Y so that an overlong
       encoding of U+0000 can be replaced before it reaches the UTF-16 output.

       A multi-byte sequence that decodes to U+0000 (C0 80, E0 80 80,
       F0 80 80 80, F8 80 80 80 80, FC 80 80 80 80 80) would otherwise emit a
       literal NUL code unit, which silently truncates any consumer that treats
       the result as a C string.  Such sequences become U+FFFD instead.  A
       single 0x00 byte still decodes to U+0000 unchanged, and no other overlong
       form (C1 BF, E0 9F BF, ...) is affected.

       The matching size computation in mmutf_u16char_size_from8 already reports
       1 unit for both U+0000 and U+FFFD, so the two passes stay in step. */
    MemeInteger_t pos = 0;
    int srcCharSize   = 0;
    int dstCharSize   = 0;
    uint32_t u32rune  = 0;

    if (!_buf) {
        return pos;
    }

    while (_len > 0) {
        srcCharSize = mmutf_u8rune_get_u32(_buf, _len, &u32rune);
        if (srcCharSize == -1) {
            break;
        }

        /* Overlong encoding of U+0000: emit REPLACEMENT CHARACTER instead. */
        if (u32rune == 0 && srcCharSize > 1) {
            u32rune = 0xFFFD;
        }

        dstCharSize = mmutf_u16rune_set_u32(_out + pos, INTPTR_MAX, u32rune);
        if (dstCharSize == -1) {
            break;
        }

        _buf += srcCharSize;
        _len -= srcCharSize;
        pos += dstCharSize;
    }
    return pos;
}

inline MemeInteger_t
mmutf_default_convert_u8to32(
    const MemeByte_t* _buf, MemeInteger_t _len, uint32_t* _out, void* _data)
{
    MMUTF_DEFAULT_CONVERT_X_TO_Y(u8, u32, _buf, _len, _out);
}

inline MemeInteger_t
mmutf_default_convert_u16to8(
    const uint16_t* _buf, MemeInteger_t _len, MemeByte_t* _out, void* _data)
{
    MMUTF_DEFAULT_CONVERT_X_TO_Y(u16, u8, _buf, _len, _out);
}

inline MemeInteger_t
mmutf_default_convert_u16to32(
    const uint16_t* _buf, MemeInteger_t _len, uint32_t* _out, void* _data)
{
    MMUTF_DEFAULT_CONVERT_X_TO_Y(u16, u32, _buf, _len, _out);
}

inline MemeInteger_t
mmutf_default_convert_u32to8(
    const uint32_t* _buf, MemeInteger_t _len, MemeByte_t* _out, void* _data)
{
    MMUTF_DEFAULT_CONVERT_X_TO_Y(u32, u8, _buf, _len, _out);
}

inline MemeInteger_t
mmutf_default_convert_u32to16(
    const uint32_t* _buf, MemeInteger_t _len, uint16_t* _out, void* _data)
{
    MMUTF_DEFAULT_CONVERT_X_TO_Y(u32, u16, _buf, _len, _out);
}

#undef MMUTF_DEFAULT_CONVERT_X_TO_Y
#undef MMUTF_DEFAULT_CHAR_SIZE_X_FROM_Y

MEME_EXTERN_C_SCOPE_ENDED

#endif // !MEME_UTF_DEFAULT_CONVERT_H_INCLUDED
