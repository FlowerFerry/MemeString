
#ifndef MEGO_UTIL_CONV_TO_NATIVE_C_STR_H_INCLUDED
#define MEGO_UTIL_CONV_TO_NATIVE_C_STR_H_INCLUDED

#include <mego/err/ec.h>
#include <mego/predef/symbol/likely.h>
#include <mego/predef/symbol/inline.h>
#include <mego/predef/os/windows.h>
#include <mego/mem/cstr_alloc_if_no_end_zero.h>

#include <meme/utf/converter.h>
#include <meme/native.h>

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#if MG_OS__WIN_AVAIL

MG_CAPI_INLINE mgec_t mgu__to_converted_native_string(
    const char *_src, mmint_t _slen, 
    mmn_char_cptr_t *_out, mmint_t *_out_slen, int)
{
    mmint_t u16len = 0;
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
    const wchar_t *_src, mmint_t _slen, 
    mmn_char_cptr_t *_out, mmint_t *_out_slen, int _must_alloc)
{
    return mgmem__wcstr_alloc_if_no_end_zero(_src, _slen, _out, _out_slen, _must_alloc);
}

MG_CAPI_INLINE void mgu_w__free_converted_native_string(const wchar_t *_src, mmn_char_cptr_t _new)
{
    mgmem__free_if_ptr_not_equal(_src, (void*)_new);
}

#else

MG_CAPI_INLINE mgec_t mgu__to_converted_native_string(
    const char *_src, mmint_t _slen, mmn_char_cptr_t *_out, mmint_t *_out_slen, int _must_alloc)
{
    return mgmem__cstr_alloc_if_no_end_zero(_src, _slen, _out, _out_slen, _must_alloc);
}

MG_CAPI_INLINE void mgu__free_converted_native_string(const char *_src, mmn_char_cptr_t _new)
{
    mgmem__free_if_ptr_not_equal(_src, (void*)_new);
}

#endif

#if MG_OS__WIN_AVAIL

MG_CAPI_INLINE mgec_t mgu_w__to_cns(
    const wchar_t *_src, mmint_t _slen, 
    mmn_char_cptr_t *_out, mmint_t *_out_slen, int _must_alloc)
{
    return mgu_w__to_converted_native_string(_src, _slen, _out, _out_slen, _must_alloc);
}

MG_CAPI_INLINE void mgu_w__free_cns(const wchar_t *_src, mmn_char_cptr_t _new)
{
    mgu_w__free_converted_native_string(_src, _new);
}
#endif

MG_CAPI_INLINE mgec_t mgu__to_cns(
    const char *_src, mmint_t _slen, 
    mmn_char_cptr_t *_out, mmint_t *_out_slen, int _must_alloc)
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
