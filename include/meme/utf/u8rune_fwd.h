
#ifndef MEME_UTF_U8RUNE_FWD_H_INCLUDED
#define MEME_UTF_U8RUNE_FWD_H_INCLUDED

#include <meme/string_fwd.h>

MEME_EXTERN_C_SCOPE_START

//int
//mmutf_u8rune_char_size(uint8_t _ch);
int
mmutf_u8rune_valid(const MemeByte_t* _buf, MemeInteger_t _len);
int
mmutf_u8char_size_from16(const uint16_t* _ch, MemeInteger_t _len, int* _ch_size);
int
mmutf_u8char_size_from32(const uint32_t* _ch, MemeInteger_t _len, int* _ch_size);
int
mmutf_u8rune_get_u32(
    const MemeByte_t* _buf, MemeInteger_t _len, uint32_t* _value);
int
mmutf_u8rune_set_u32(
    MemeByte_t* _buf, MemeInteger_t _len, uint32_t _value);

MEME_EXTERN_C_SCOPE_ENDED

#endif // !MEME_UTF_U8RUNE_FWD_H_INCLUDED
