
#ifndef MEME_UTF_CONVERTER_FWD_H_INCLUDED
#define MEME_UTF_CONVERTER_FWD_H_INCLUDED

#include <meme/string_fwd.h>
#include <mego/hardware/cpu/instruction.h>

MEME_EXTERN_C_SCOPE_START

typedef mghw_simd_instruction_t(mmutf_required_simd_instruction_sets_func_t)(void* _data);

typedef MemeInteger_t(mmutf_u8valid_func_t) (
    const MemeByte_t* _buf, MemeInteger_t _len, void* _data);
typedef MemeInteger_t(mmutf_u16valid_func_t)(
    const uint16_t* _buf, MemeInteger_t _len, void* _data);
//typedef MemeInteger_t(mmutf_u32valid_func_t)(
//    const uint32_t* _buf, MemeInteger_t _len, void* _data);

typedef MemeInteger_t(mmutf_u8runesize_func_t)(
    const MemeByte_t* _buf, MemeInteger_t _len, void* _data);
typedef MemeInteger_t(mmutf_u16runesize_func_t)(
    const uint16_t* _buf, MemeInteger_t _len, void* _data);

typedef MemeInteger_t(mmutf_charsize_u8from16_func_t)(
    const uint16_t* _buf, MemeInteger_t _len, void* _data);
typedef MemeInteger_t(mmutf_charsize_u32from16_func_t)(
    const uint16_t* _buf, MemeInteger_t _len, void* _data);
typedef MemeInteger_t(mmutf_charsize_u8from32_func_t)(
    const uint32_t* _buf, MemeInteger_t _len, void* _data);
typedef MemeInteger_t(mmutf_charsize_u16from8_func_t)(
    const MemeByte_t* _buf, MemeInteger_t _len, void* _data);
typedef MemeInteger_t(mmutf_charsize_u32from8_func_t)(
    const MemeByte_t* _buf, MemeInteger_t _len, void* _data);
typedef MemeInteger_t(mmutf_charsize_u16from32_func_t)(
    const uint32_t* _buf, MemeInteger_t _len, void* _data);

typedef MemeInteger_t(mmutf_convert_u8to16_func_t)(
    const MemeByte_t* _buf, MemeInteger_t _len, uint16_t* _out, void* _data);
typedef MemeInteger_t(mmutf_convert_u8to32_func_t)(
    const MemeByte_t* _buf, MemeInteger_t _len, uint32_t* _out, void* _data);
typedef MemeInteger_t(mmutf_convert_u16to8_func_t)(
    const uint16_t* _buf, MemeInteger_t _len, MemeByte_t* _out, void* _data);
typedef MemeInteger_t(mmutf_convert_u16to32_func_t)(
    const uint16_t* _buf, MemeInteger_t _len, uint32_t* _out, void* _data);
typedef MemeInteger_t(mmutf_convert_u32to8_func_t)(
    const uint32_t* _buf, MemeInteger_t _len, MemeByte_t* _out, void* _data);
typedef MemeInteger_t(mmutf_convert_u32to16_func_t)(
    const uint32_t* _buf, MemeInteger_t _len, uint16_t* _out, void* _data);

struct mmutf_converter;
typedef struct mmutf_converter mmutf_converter_t;

MEME_EXTERN_C_SCOPE_ENDED

#endif // !MEME_UTF_CONVERTER_FWD_H_INCLUDED
