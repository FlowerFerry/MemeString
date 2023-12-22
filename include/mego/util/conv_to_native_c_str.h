
#ifndef MEGO_UTIL_CONV_TO_NATIVE_C_STR_H_INCLUDED
#define MEGO_UTIL_CONV_TO_NATIVE_C_STR_H_INCLUDED

#include <mego/err/ec.h>
#include <mego/predef/symbol/likely.h>
#include <mego/predef/os/windows.h>

#include <meme/utf/converter.h>
#include <meme/native.h>

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#if MG_OS__WIN_AVAIL

mgec_t mgu__conv_to_native_c_str(
    const char *_src_path, size_t _slen, 
    mmn_char_cptr_t *_out_path, size_t *_out_slen, int _must_alloc);

void mgu__free_native_c_str(const char *_src_path, mmn_char_cptr_t _new_path);

mgec_t mgu_w__conv_to_native_c_str(
    const wchar_t *_src_path, size_t _slen, 
    mmn_char_cptr_t *_out_path, size_t *_out_slen, int _must_alloc);

void mgu_w__free_native_c_str(const wchar_t *_src_path, mmn_char_cptr_t _new_path);

#else

mgec_t mgu__conv_to_native_c_str(
    const char *_src_path, size_t _slen, 
    mmn_char_cptr_t *_out_path, size_t *_out_slen, int _must_alloc);

void mgu__free_native_c_str(const char *_src_path, mmn_char_cptr_t _new_path);

#endif


#if MG_OS__WIN_AVAIL

inline mgec_t mgu__conv_to_native_c_str(
    const char *_src_path, size_t _slen, 
    mmn_char_cptr_t *_out_path, size_t *_out_slen, int)
{
    size_t u16len = 0;
    wchar_t * path = NULL;

    if (MEGO_SYMBOL__UNLIKELY(_src_path == NULL))
        return MGEC__INVAL;
    if (MEGO_SYMBOL__UNLIKELY(_out_path == NULL))
        return MGEC__INVAL;

    if (_slen < 0)
        _slen = strlen(_src_path);

    u16len = mmutf_char_size_u16from8((const uint8_t*)_src_path, _slen);
    if (u16len < 1)
        return MGEC__INVAL;

    path = (wchar_t*)malloc(sizeof(wchar_t) * (u16len + 1));
    if (path == NULL)
        return MGEC__NOMEM;

    if (mmutf_convert_u8to16((const uint8_t*)_src_path, _slen, (uint16_t*)path) == 0)
    {
        free(path);
        return MGEC__INVAL;
    }

    path[u16len] = L'\0';

    if (_out_slen != NULL)
        *_out_slen = u16len;
    *_out_path = path;
    return 0;
}

inline void mgu__free_native_c_str(const char *_src_path, mmn_char_cptr_t _new_path)
{
    (void*)_src_path;

    if (_new_path != NULL)
        free((void*)_new_path);
}

inline mgec_t mgu_w__conv_to_native_c_str(
    const wchar_t *_src_path, size_t _slen, 
    mmn_char_cptr_t *_out_path, size_t *_out_slen, int _must_alloc)
{
    wchar_t * path = NULL;

    if (MEGO_SYMBOL__UNLIKELY(_src_path == NULL))
        return MGEC__INVAL;
    if (MEGO_SYMBOL__UNLIKELY(_out_path == NULL))
        return MGEC__INVAL;

    if (_slen < 0) {
        if (_must_alloc) {
            _slen = wcslen(_src_path);
        }
        else {         
            if (_out_slen != NULL)
                *_out_slen = wcslen(_src_path);
            *_out_path = _src_path;
            return 0;
        }
    }

    if (_src_path[_slen] == '\0' && !_must_alloc) 
    {
        if (_out_slen != NULL)
            *_out_slen = _slen;
        *_out_path = _src_path;
        return 0;
    } 

    path = (wchar_t*)malloc(sizeof(wchar_t) * (_slen + 1));
    if (path == NULL)
        return MGEC__NOMEM;
    
    memcpy(path, _src_path, sizeof(wchar_t) * _slen);
    path[_slen] = L'\0';

    if (_out_slen != NULL)
        *_out_slen = _slen;
    *_out_path = path;
    return 0;
}

inline void mgu_w__free_native_c_str(const wchar_t *_src_path, mmn_char_cptr_t _new_path)
{
    if (_new_path != NULL && _new_path != _src_path)
        free((void*)_new_path);
}

#else

inline mgec_t mgu__conv_to_native_c_str(
    const char *_src_path, size_t _slen, mmn_char_cptr_t *_out_path, int _must_alloc)
{
    char * path = NULL;

    if (MEGO_SYMBOL__UNLIKELY(_src_path == NULL))
        return MGEC__INVAL;
    if (MEGO_SYMBOL__UNLIKELY(_out_path == NULL))
        return MGEC__INVAL;

    if (_slen < 0) {
        if (_must_alloc) {
            _slen = strlen(_src_path);
        }
        else {
            if (_out_slen != NULL)
                *_out_slen = strlen(_src_path);
            *_out_path = _src_path;
            return 0;
        }
    }

    if (_src_path[_slen] == '\0' && !_must_alloc) 
    {
        if (_out_slen != NULL)
            *_out_slen = _slen;
        *_out_path = _src_path;
        return 0;
    }

    path = (char*)malloc(sizeof(char) * (_slen + 1));
    if (path == NULL)
        return MGEC__NOMEM;

    memcpy(path, _src_path, _slen);
    path[_slen] = '\0';

    if (_out_slen != NULL)
        *_out_slen = _slen;
    *_out_path = path;
    return 0;
}

inline void mgu__free_native_c_str(const char *_src_path, mmn_char_cptr_t _new_path)
{
    if (_new_path != NULL && _new_path != _src_path)
        free((void*)_new_path);
}

#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !MEGO_UTIL_CONV_TO_NATIVE_C_STR_H_INCLUDED
