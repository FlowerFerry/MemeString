
#ifndef MEME_IMPL_UTF_CONVERTER_H_INCLUDED
#define MEME_IMPL_UTF_CONVERTER_H_INCLUDED

#include <meme/utf/converter.h>

MEME_EXTERN_C_SCOPE_START

struct mmutf_converter
{
    void* data;
    char  name[32];
    
    mmutf_required_simd_instruction_sets_func_t* required_instruction_sets_fn;
    
    mmutf_u8valid_func_t*  u8valid_fn;
    mmutf_u16valid_func_t* u16valid_fn;

    mmutf_u8runesize_func_t*  u8runesize_fn;
    mmutf_u16runesize_func_t* u16runesize_fn;

    mmutf_charsize_u8from16_func_t* u8charsize_from16_fn;
    mmutf_charsize_u8from32_func_t* u8charsize_from32_fn;
    mmutf_charsize_u16from8_func_t* u16charsize_from8_fn;
    mmutf_charsize_u16from32_func_t* u16charsize_from32_fn;
    mmutf_charsize_u32from8_func_t* u32charsize_from8_fn;
    mmutf_charsize_u32from16_func_t* u32charsize_from16_fn;

    mmutf_convert_u8to16_func_t*  u8to16_fn;
    mmutf_convert_u8to32_func_t*  u8to32_fn;
    mmutf_convert_u16to8_func_t*  u16to8_fn;
    mmutf_convert_u16to32_func_t* u16to32_fn;
    mmutf_convert_u32to8_func_t*  u32to8_fn;
    mmutf_convert_u32to16_func_t* u32to16_fn;
};

MEME_EXTERN_C_SCOPE_ENDED

#endif // !MEME_IMPL_UTF_CONVERTER_H_INCLUDED
