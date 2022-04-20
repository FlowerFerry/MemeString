

#ifndef MEGO_PREDEF_COMPILER_GCC_H_INCLUDED
#define MEGO_PREDEF_COMPILER_GCC_H_INCLUDED

#if defined(__GNUC__) && !defined(__ibmxl__)
#   define MEGO_COMP__GCC__AVAILABLE (1)
#   include "details/comp_detected.h"
#endif

#ifndef MEGO_COMP__GCC__AVAILABLE
#define MEGO_COMP__GCC__AVAILABLE (0)
#endif

#endif
