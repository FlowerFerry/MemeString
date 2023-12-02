
#include "mego/predef/helper_macros.h"

#if !defined(MEGO_ARCH__STRING)

#if defined(__SYSC_ZARCH__)
#   undef MEGO_ARCH__Z
#   define MEGO_ARCH__Z (1)
#endif

#define MEGO_ARCH__STRING "z/Architecture"

#endif

#ifndef MEGO_ARCH__Z
#define MEGO_ARCH__Z (0)
#endif
