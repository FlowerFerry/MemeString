
#ifndef MEME_NATIVE_H_INCLUDED
#define MEME_NATIVE_H_INCLUDED

#include <mego/predef/os/windows.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#if MG_OS__WIN_AVAIL
#   define MMN_TEXT(_s) L##_s
#else
#   define MMN_TEXT(_s) _s
#endif

#if MG_OS__WIN_AVAIL
    typedef wchar_t mmn_char_t;
#else
    typedef char    mmn_char_t;
#endif

typedef const mmn_char_t* mmn_char_cptr_t;

#if MG_OS__WIN_AVAIL
#   define MMN_PATH_SEP_CH L'\\'
#else
#   define MMN_PATH_SEP_CH '/'
#endif

#if MG_OS__WIN_AVAIL
#   define MMN_PATH_SEP_STR L"\\"
#else
#   define MMN_PATH_SEP_STR "/"
#endif

#if MG_OS__WIN_AVAIL
#   define MMPATH_SEP_CH '\\'
#else
#   define MMPATH_SEP_CH '/'
#endif

#if MG_OS__WIN_AVAIL
#   define MMPATH_SEP_STR "\\"
#else
#   define MMPATH_SEP_STR "/"
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !MEME_NATIVE_H_INCLUDED
