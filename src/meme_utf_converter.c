
#include <meme/impl/utf/converter.h>
#include <meme/impl/atomic.h>

#include <meme/utf/default_converter.h>
#include <mego/thrd/call_once.h>

MEME_EXTERN_C_SCOPE_START

static mgthrd_once_flag* __mmutf_get_startup_once_flag()
{
    static mgthrd_once_flag f = MGTHRD_ONCE_FLAG_INIT;
    return &f;
}

static volatile mmutf_converter_t* __mmutf_get_default_converter()
{
    static volatile mmutf_converter_t c = { 
        0,
        "default",
        
        .required_instruction_sets_fn = 
            mmutf_default_required_simd_instruction_sets,
        
        .u8valid_fn            = mmutf_default_u8valid,
        .u16valid_fn           = mmutf_default_u16valid,
        .u8runesize_fn         = mmutf_default_u8rune_size,
        .u16runesize_fn        = mmutf_default_u16rune_size,
        .u8charsize_from16_fn  = mmutf_default_char_size_u8from16,
        .u16charsize_from8_fn  = mmutf_default_char_size_u16from8,
        .u8charsize_from32_fn  = mmutf_default_char_size_u8from32,
        .u32charsize_from8_fn  = mmutf_default_char_size_u32from8,
        .u16charsize_from32_fn = mmutf_default_char_size_u16from32,
        .u32charsize_from16_fn = mmutf_default_char_size_u32from16,
        .u8to16_fn             = mmutf_default_convert_u8to16,
        .u16to8_fn             = mmutf_default_convert_u16to8,
        .u8to32_fn             = mmutf_default_convert_u8to32,
        .u32to8_fn             = mmutf_default_convert_u32to8,
        .u16to32_fn            = mmutf_default_convert_u16to32,
        .u32to16_fn            = mmutf_default_convert_u32to16,
        
    };
    return &c;
}

static volatile MemeAtomicInteger_t* __mmutf_get_converter_pointer()
{
    static volatile MemeAtomicInteger_t pointer = 0;
    return &pointer;
}

static void __mmutf_startup(void)
{
    MemeAtomicInteger_init(
        __mmutf_get_converter_pointer(), 
        (MemeInteger_t)__mmutf_get_default_converter());
}

MEME_API volatile mmutf_converter_t*
MEME_STDCALL mmutf_get_best_converter()
{
    MemeInteger_t p;
    mgthrd_call_once(__mmutf_get_startup_once_flag(), __mmutf_startup);
    
    p = MemeAtomicInteger_load(__mmutf_get_converter_pointer());
    if (p == 0) {
        return __mmutf_get_default_converter();
    }
    else {
        return (volatile mmutf_converter_t*)p;
    }
}

MEME_API int
MEME_STDCALL mmutf_set_converter(volatile mmutf_converter_t* _converter)
{
    MemeInteger_t p;
    if (_converter == NULL) {
        return MMENO__POSIX_OFFSET(EINVAL);
    }
    p = (MemeInteger_t)_converter;
    MemeAtomicInteger_store(__mmutf_get_converter_pointer(), p);
    return 0;
}


MEME_API MemeInteger_t
MEME_STDCALL mmutf_u8valid(
    const MemeByte_t* _buf, MemeInteger_t _len)
{
    volatile mmutf_converter_t* conv = mmutf_get_best_converter();
    if (conv->u8valid_fn)
        return conv->u8valid_fn(_buf, _len, conv->data);
    else
        return 0;
}

MEME_API MemeInteger_t
MEME_STDCALL mmutf_u16valid(
    const uint16_t* _buf, MemeInteger_t _len)
{
    volatile mmutf_converter_t* conv = mmutf_get_best_converter();
    if (conv->u16valid_fn)
        return conv->u16valid_fn(_buf, _len, conv->data);
    else
        return 0;
}

MEME_API MemeInteger_t
MEME_STDCALL mmutf_u8rune_size(
    const MemeByte_t* _buf, MemeInteger_t _len)
{
    volatile mmutf_converter_t* conv = mmutf_get_best_converter();
    if (conv->u8runesize_fn)
        return conv->u8runesize_fn(_buf, _len, conv->data);
    else
        return 0;
}

MEME_API MemeInteger_t
MEME_STDCALL mmutf_u16rune_size(
    const uint16_t* _buf, MemeInteger_t _len)
{
    volatile mmutf_converter_t* conv = mmutf_get_best_converter();
    if (conv->u16runesize_fn)
        return conv->u16runesize_fn(_buf, _len, conv->data);
    else
        return 0;
}

MEME_API MemeInteger_t
MEME_STDCALL mmutf_char_size_u8from16(
    const uint16_t* _buf, MemeInteger_t _len)
{
    volatile mmutf_converter_t* conv = mmutf_get_best_converter();
    if (conv->u8charsize_from16_fn)
        return conv->u8charsize_from16_fn(_buf, _len, conv->data);
    else
        return 0;
}

MEME_API MemeInteger_t
MEME_STDCALL mmutf_char_size_u16from8(
    const MemeByte_t* _buf, MemeInteger_t _len)
{
    volatile mmutf_converter_t* conv = mmutf_get_best_converter();
    if (conv->u16charsize_from8_fn)
        return conv->u16charsize_from8_fn(_buf, _len, conv->data);
    else
        return 0;
}

MEME_API MemeInteger_t
MEME_STDCALL mmutf_convert_u8to16(
    const MemeByte_t* _buf, MemeInteger_t _len, uint16_t* _out)
{
    volatile mmutf_converter_t* conv = mmutf_get_best_converter();
    if (conv->u8to16_fn)
        return conv->u8to16_fn(_buf, _len, _out, conv->data);
    else
        return 0;
}

MEME_API MemeInteger_t
MEME_STDCALL mmutf_convert_u16to8(
    const uint16_t* _buf, MemeInteger_t _len, MemeByte_t* _out)
{
    volatile mmutf_converter_t* conv = mmutf_get_best_converter();
    if (conv->u16to8_fn)
        return conv->u16to8_fn(_buf, _len, _out, conv->data);
    else
        return 0;
}

MEME_EXTERN_C_SCOPE_ENDED