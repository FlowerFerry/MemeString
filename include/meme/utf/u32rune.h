
#ifndef MEME_UTF_U32RUNE_H_INCLUDED
#define MEME_UTF_U32RUNE_H_INCLUDED

#include <meme/string_fwd.h>
#include <meme/utf/u32rune_fwd.h>

MEME_EXTERN_C_SCOPE_START

MEME_API int MEME_STDCALL
mmutf_u32rune_char_size(uint32_t _ch);

MEME_API int MEME_STDCALL
mmutf_u32rune_valid(const uint32_t* _buf, MemeInteger_t _len);

MEME_API int MEME_STDCALL
mmutf_u32char_size_from8(const MemeByte_t* _ch, MemeInteger_t _len, int* _ch_size);

MEME_API int MEME_STDCALL
mmutf_u32char_size_from16(const uint16_t* _ch, MemeInteger_t _len, int* _ch_size);

MEME_API int MEME_STDCALL
mmutf_u32rune_get_u32(
    const uint32_t* _buf, MemeInteger_t _len, uint32_t* _value);

MEME_API int MEME_STDCALL
mmutf_u32rune_set_u32(
    uint32_t* _buf, MemeInteger_t _len, uint32_t _value);

MEME_EXTERN_C_SCOPE_ENDED

#endif // !MEME_UTF_U32RUNE_H_INCLUDED
