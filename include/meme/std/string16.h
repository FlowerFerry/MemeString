
#ifndef MEME_STD_STRING_H_INCLUDED
#define MEME_STD_STRING_H_INCLUDED

#include <uchar.h>
#include <meme/string_fwd.h>
#include <mego/predef/symbol/inline.h>

MEME_EXTERN_C_SCOPE_START

MG_CAPI_INLINE int strlen16(const char16_t* _s)
{
    if (!_s)
        return -1; //strarg is NULL pointer
    const char16_t* str = _s;
    for (; *str; ++str)
        ; // empty body
    return (int)(str - _s);
}

MEME_EXTERN_C_SCOPE_ENDED

#endif // !MEME_STD_STRING_H_INCLUDED
