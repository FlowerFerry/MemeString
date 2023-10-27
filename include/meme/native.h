
#ifndef MEME_NATIVE_H_INCLUDED
#define MEME_NATIVE_H_INCLUDED

#include <mego/predef/os/windows.h>

#if MG_OS__WIN_AVAIL
#   define MMN_TEXT(_s) L##_s
#else
#   define MMN_TEXT(_s) _s
#endif

#if MG_OS__WIN_AVAIL
    using mmn_char_t = wchar_t;
#else
    using mmn_char_t = char;
#endif

#endif // !MEME_NATIVE_H_INCLUDED
