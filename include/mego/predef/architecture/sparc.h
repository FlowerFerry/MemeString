
#include "mego/predef/helper_macros.h"

#if !defined(MEGO_ARCH__STRING)

#if defined(__sparc__) || defined(__sparc)
#   if !defined(MEGO_ARCH__SPARC) && (defined(__sparcv9) || defined(__sparc_v9__)
#       define MEGO_ARCH__SPARC MEGO__MAKE_VERSION_NUMBER(9,0,0)
#   endif
#   if !defined(MEGO_ARCH__SPARC) && (defined(__sparcv8) || defined(__sparc_v8__)
#       define MEGO_ARCH__SPARC MEGO__MAKE_VERSION_NUMBER(8,0,0)
#   endif
#   if !defined(MEGO_ARCH__SPARC)
#       define MEGO_ARCH__SPARC MEGO__MAKE_VERSION_NUMBER
#   endif

#define MEGO_ARCH__STRING "sparc"
#endif

#endif

#ifndef MEGO_ARCH__SPARC
#define MEGO_ARCH__SPARC (0)
#endif