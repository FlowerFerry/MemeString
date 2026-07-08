
#ifndef MEME_VERSION_H_INCLUDED
#define MEME_VERSION_H_INCLUDED

#include <meme/string_fwd.h>
#include <mego/predef/helper_macros.h>

#define MMVER_MAJOR 0
#define MMVER_MINOR 0
#define MMVER_PATCH 6

#define MMVER_NUM \
    MEGO__MAKE_VERSION_NUMBER( MMVER_MAJOR, MMVER_MINOR, MMVER_PATCH )

#define MMVER_STR \
    (MEGO__STRINGIZE( MMVER_MAJOR ) "." \
     MEGO__STRINGIZE( MMVER_MINOR ) "." \
     MEGO__STRINGIZE( MMVER_PATCH ))

MEME_EXTERN_C_SCOPE_START

typedef mmint_t mmver_t;

MEME_API mmver_t     MEME_STDCALL mmver_num();
MEME_API const char* MEME_STDCALL mmver_str();

MEME_EXTERN_C_SCOPE_ENDED

#endif // !MEME_VERSION_H_INCLUDED
