
#ifndef MEGO_UTIL_CONV_TO_NATIVE_C_STR_H_INCLUDED
#define MEGO_UTIL_CONV_TO_NATIVE_C_STR_H_INCLUDED

#include <mego/err/ec.h>
#include <mego/predef/symbol/likely.h>
#include <mego/predef/symbol/inline.h>
#include <mego/predef/os/windows.h>

#include <meme/utf/converter.h>
#include <meme/native.h>

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// #if MG_OS__WIN_AVAIL

// mgec_t mgu__to_converted_native_string(
//     const char *_src, size_t _slen, 
//     mmn_char_cptr_t *_out, size_t *_out_slen, int _must_alloc);

// void mgu__free_converted_native_string(const char *_src, mmn_char_cptr_t _new);

// mgec_t mgu_w__to_converted_native_string(
//     const wchar_t *_src, size_t _slen, 
//     mmn_char_cptr_t *_out, size_t *_out_slen, int _must_alloc);

// void mgu_w__free_converted_native_string(const wchar_t *_src, mmn_char_cptr_t _new);

// #else

// mgec_t mgu__to_converted_native_string(
//     const char *_src, size_t _slen, 
//     mmn_char_cptr_t *_out, size_t *_out_slen, int _must_alloc);

// void mgu__free_converted_native_string(const char *_src, mmn_char_cptr_t _new);

// #endif


#if MG_OS__WIN_AVAIL

MG_CAPI_INLINE mgec_t mgu__to_converted_native_string(
    const char *_src, size_t _slen, 
    mmn_char_cptr_t *_out, size_t *_out_slen, int)
{
    size_t u16len = 0;
    wchar_t * path = NULL;

    if (MEGO_SYMBOL__UNLIKELY(_src == NULL))
        return MGEC__INVAL;
    if (MEGO_SYMBOL__UNLIKELY(_out == NULL))
        return MGEC__INVAL;

    if (_slen < 0)
        _slen = strlen(_src);

    u16len = mmutf_char_size_u16from8((const uint8_t*)_src, _slen);
    if (u16len < 1)
        return MGEC__INVAL;

    path = (wchar_t*)malloc(sizeof(wchar_t) * (u16len + 1));
    if (path == NULL)
        return MGEC__NOMEM;

    if (mmutf_convert_u8to16((const uint8_t*)_src, _slen, (uint16_t*)path) == 0)
    {
        free(path);
        return MGEC__INVAL;
    }

    path[u16len] = L'\0';

    if (_out_slen != NULL)
        *_out_slen = u16len;
    *_out = path;
    return 0;
}

MG_CAPI_INLINE void mgu__free_converted_native_string(const char *_src, mmn_char_cptr_t _new)
{
    (void*)_src;

    if (_new != NULL)
        free((void*)_new);
}

MG_CAPI_INLINE mgec_t mgu_w__to_converted_native_string(
    const wchar_t *_src, size_t _slen, 
    mmn_char_cptr_t *_out, size_t *_out_slen, int _must_alloc)
{
    wchar_t * path = NULL;

    if (MEGO_SYMBOL__UNLIKELY(_src == NULL))
        return MGEC__INVAL;
    if (MEGO_SYMBOL__UNLIKELY(_out == NULL))
        return MGEC__INVAL;

    if (_slen < 0) {
        if (_must_alloc) {
            _slen = wcslen(_src);
        }
        else {         
            if (_out_slen != NULL)
                *_out_slen = wcslen(_src);
            *_out = _src;
            return 0;
        }
    }

    if (_src[_slen] == '\0' && !_must_alloc) 
    {
        if (_out_slen != NULL)
            *_out_slen = _slen;
        *_out = _src;
        return 0;
    } 

    path = (wchar_t*)malloc(sizeof(wchar_t) * (_slen + 1));
    if (path == NULL)
        return MGEC__NOMEM;
    
    memcpy(path, _src, sizeof(wchar_t) * _slen);
    path[_slen] = L'\0';

    if (_out_slen != NULL)
        *_out_slen = _slen;
    *_out = path;
    return 0;
}

MG_CAPI_INLINE void mgu_w__free_converted_native_string(const wchar_t *_src, mmn_char_cptr_t _new)
{
    if (_new != NULL && _new != _src)
        free((void*)_new);
}

#else

MG_CAPI_INLINE mgec_t mgu__to_converted_native_string(
    const char *_src, size_t _slen, mmn_char_cptr_t *_out, size_t *_out_slen, int _must_alloc)
{
    char * path = NULL;

    if (MEGO_SYMBOL__UNLIKELY(_src == NULL))
        return MGEC__INVAL;
    if (MEGO_SYMBOL__UNLIKELY(_out == NULL))
        return MGEC__INVAL;

    if (_slen < 0) {
        if (_must_alloc) {
            _slen = strlen(_src);
        }
        else {
            if (_out_slen != NULL)
                *_out_slen = strlen(_src);
            *_out = _src;
            return 0;
        }
    }

    if (_src[_slen] == '\0' && !_must_alloc) 
    {
        if (_out_slen != NULL)
            *_out_slen = _slen;
        *_out = _src;
        return 0;
    }

    path = (char*)malloc(sizeof(char) * (_slen + 1));
    if (path == NULL)
        return MGEC__NOMEM;

    memcpy(path, _src, _slen);
    path[_slen] = '\0';

    if (_out_slen != NULL)
        *_out_slen = _slen;
    *_out = path;
    return 0;
}

MG_CAPI_INLINE void mgu__free_converted_native_string(const char *_src, mmn_char_cptr_t _new)
{
    if (_new != NULL && _new != _src)
        free((void*)_new);
}

#endif

#if MG_OS__WIN_AVAIL

MG_CAPI_INLINE mgec_t mgu_w__to_cns(
    const wchar_t *_src, size_t _slen, 
    mmn_char_cptr_t *_out, size_t *_out_slen, int _must_alloc)
{
    return mgu_w__to_converted_native_string(_src, _slen, _out, _out_slen, _must_alloc);
}

MG_CAPI_INLINE void mgu_w__free_cns(const wchar_t *_src, mmn_char_cptr_t _new)
{
    mgu_w__free_converted_native_string(_src, _new);
}
#endif

MG_CAPI_INLINE mgec_t mgu__to_cns(
    const char *_src, size_t _slen, 
    mmn_char_cptr_t *_out, size_t *_out_slen, int _must_alloc)
{
    return mgu__to_converted_native_string(_src, _slen, _out, _out_slen, _must_alloc);
}

MG_CAPI_INLINE void mgu__free_cns(const char *_src, mmn_char_cptr_t _new)
{
    mgu__free_converted_native_string(_src, _new);
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !MEGO_UTIL_CONV_TO_NATIVE_C_STR_H_INCLUDED
