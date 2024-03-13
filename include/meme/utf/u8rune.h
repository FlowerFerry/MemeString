
#ifndef MEGO_UTF_U8RUNE_H_INCLUDED
#define MEGO_UTF_U8RUNE_H_INCLUDED

#include <meme/string_fwd.h>
#include <meme/utf/u8rune_fwd.h>

MEME_EXTERN_C_SCOPE_START

MEME_API int MEME_STDCALL
mmutf_u8rune_char_size(uint8_t _ch);
MEME_API int MEME_STDCALL
mmutf_u8rune_valid(const MemeByte_t* _buf, MemeInteger_t _len);
MEME_API int MEME_STDCALL
mmutf_u8char_size_from16(const uint16_t* _ch, MemeInteger_t _len, int* _ch_size);
MEME_API int MEME_STDCALL
mmutf_u8char_size_from32(const uint32_t* _ch, MemeInteger_t _len, int* _ch_size);
MEME_API int MEME_STDCALL
mmutf_u8rune_get_u32(
    const MemeByte_t* _buf, MemeInteger_t _len, uint32_t* _value);
MEME_API int MEME_STDCALL
mmutf_u8rune_set_u32(
    MemeByte_t* _buf, MemeInteger_t _len, uint32_t _value);

MEME_API int MEME_STDCALL
mmutf_u8rune_prev_char_size(const mmbyte_t* _first, const mmbyte_t* _last);

MEME_EXTERN_C_SCOPE_ENDED

#endif // !MEGO_UTF_U8RUNE_H_INCLUDED
