
#include "mego/predef/helper_macros.h"

#if !defined(MEGO_ARCH__STRING)

#if defined(__ia64__) || defined(_IA64) || \
    defined(__IA64__) || defined(__ia64) || \
    defined(_M_IA64) || defined(__itanium__)
#define MEGO_ARCH__IA64 (1)

#define MEGO_ARCH__STRING "Intel Itanium 64"

#endif 

#endif

#ifndef MEGO_ARCH__IA64
#define MEGO_ARCH__IA64 (0)
#endif
