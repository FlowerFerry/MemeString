
#ifndef MGU_STD_FILE_H_INCLUDED
#define MGU_STD_FILE_H_INCLUDED

#include <meme/utf/converter.h>
#include <mego/predef/os/windows.h>
#include <mego/util/converted_native_string.h>

#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#if MG_OS__WIN_AVAIL
inline FILE* mgu_w_fopen(
    const wchar_t* _path, mmint_t _slen, const wchar_t* _mode, mmint_t _mlen)
{
    FILE* fp = NULL;
    mmn_char_cptr_t path = NULL;
    mmn_char_cptr_t mode = NULL;
    mgec_t ec = 0;
    
    ec = mgu_w__to_cns(_path, _slen, &path, NULL, 0);
    if (MEGO_SYMBOL__UNLIKELY(ec != 0))
        return NULL;

    ec = mgu_w__to_cns(_mode, _mlen, &mode, NULL, 0);
    if (MEGO_SYMBOL__UNLIKELY(ec != 0)) {
        mgu_w__free_cns(_path, path);
        return NULL;
    }

    fp = _wfopen(path, mode);
    mgu_w__free_cns(_path, path);
    mgu_w__free_cns(_mode, mode);
    return fp;
}
#endif

inline FILE* mgu_fopen(
    const char* _path, mmint_t _slen, const char* _mode, mmint_t _mlen)
{
    FILE* fp = NULL;
    mmn_char_cptr_t path = NULL;
    mmn_char_cptr_t mode = NULL;
    mgec_t ec = 0;
    
    ec = mgu__to_cns(_path, _slen, &path, NULL, 0);
    if (MEGO_SYMBOL__UNLIKELY(ec != 0))
        return NULL;

    ec = mgu__to_cns(_mode, _mlen, &mode, NULL, 0);
    if (MEGO_SYMBOL__UNLIKELY(ec != 0)) {
        mgu__free_cns(_path, path);
        return NULL;
    }

#if MG_OS__WIN_AVAIL
    fp = _wfopen(path, mode);
#else
    fp = fopen(path, mode);
#endif
    mgu__free_cns(_path, path);
    mgu__free_cns(_mode, mode);
    return fp;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !MGU_STD_FILE_H_INCLUDED
