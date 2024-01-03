
#ifndef MEGO_UTF_U16RUNE_H_INCLUDED
#define MEGO_UTF_U16RUNE_H_INCLUDED

#include <meme/string_fwd.h>
#include <meme/utf/u16rune_fwd.h>

MEME_EXTERN_C_SCOPE_START

MEME_API int MEME_STDCALL
mmutf_u16rune_char_size(uint16_t _ch);
MEME_API int MEME_STDCALL
mmutf_u16rune_valid(const uint16_t* _buf, MemeInteger_t _len);
MEME_API int MEME_STDCALL
mmutf_u16char_size_from8(const MemeByte_t* _ch, MemeInteger_t _len, int* _ch_size);
MEME_API int MEME_STDCALL
mmutf_u16char_size_from32(const uint32_t* _ch, MemeInteger_t _len, int* _ch_size);
MEME_API int MEME_STDCALL
mmutf_u16rune_get_u32(
    const uint16_t* _buf, MemeInteger_t _len, uint32_t* _value);
MEME_API int MEME_STDCALL
mmutf_u16rune_set_u32(
    uint16_t* _buf, MemeInteger_t _len, uint32_t _value);

MEME_EXTERN_C_SCOPE_ENDED

#endif // !MEGO_UTF_U16RUNE_H_INCLUDED

