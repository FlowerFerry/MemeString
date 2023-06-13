
#ifndef MEME_UTF_CONVERTER_H_INCLUDED
#define MEME_UTF_CONVERTER_H_INCLUDED

#include <meme/utf/converter_fwd.h>

MEME_EXTERN_C_SCOPE_START

MEME_API volatile mmutf_converter_t* 
MEME_STDCALL mmutf_get_best_converter();

MEME_API int
MEME_STDCALL mmutf_set_converter(volatile mmutf_converter_t* _converter);


MEME_API MemeInteger_t
MEME_STDCALL mmutf_u8valid(
    const MemeByte_t* _buf, MemeInteger_t _len);

MEME_API MemeInteger_t
MEME_STDCALL mmutf_u16valid(
    const uint16_t* _buf, MemeInteger_t _len);

MEME_API MemeInteger_t
MEME_STDCALL mmutf_u8rune_size(
    const MemeByte_t* _buf, MemeInteger_t _len);

MEME_API MemeInteger_t
MEME_STDCALL mmutf_u16rune_size(
    const uint16_t* _buf, MemeInteger_t _len);

MEME_API MemeInteger_t
MEME_STDCALL mmutf_char_size_u8from16(
    const uint16_t* _buf, MemeInteger_t _len);

MEME_API MemeInteger_t
MEME_STDCALL mmutf_char_size_u16from8(
    const MemeByte_t* _buf, MemeInteger_t _len);

MEME_API MemeInteger_t
MEME_STDCALL mmutf_convert_u8to16(
    const MemeByte_t* _buf, MemeInteger_t _len, uint16_t* _out);

MEME_API MemeInteger_t
MEME_STDCALL mmutf_convert_u16to8(
    const uint16_t* _buf, MemeInteger_t _len, MemeByte_t* _out);

MEME_EXTERN_C_SCOPE_ENDED

#endif // !MEME_UTF_CONVERTER_H_INCLUDED
