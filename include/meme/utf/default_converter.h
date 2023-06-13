
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
    MMUTF_DEFAULT_CONVERT_X_TO_Y(u8, u16, _buf, _len, _out);
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
