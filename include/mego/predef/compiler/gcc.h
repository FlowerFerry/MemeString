

#ifndef MEGO_PREDEF_COMPILER_GCC_H_INCLUDED
#define MEGO_PREDEF_COMPILER_GCC_H_INCLUDED

#include "mego/predef/helper_macros.h"

#if defined(__GNUC__) && !defined(__ibmxl__)
#   define MEGO_COMP__GCC__AVAILABLE (1)
#   include "details/comp_detected.h"

#   if !defined(MEGO_COMP__GNUC_VERSION) && defined(__GNUC_PATCHLEVEL__)
#       define MEGO_COMP__GNUC_VERSION \
            MEGO__MAKE_VERSION_NUMBER(__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__)
#   endif
#   if !defined(MEGO_COMP__GNUC_VERSION)
#       define MEGO_COMP__GNUC_VERSION \
            MEGO__MAKE_VERSION_NUMBER(__GNUC__,__GNUC_MINOR__,0)
#   endif

#endif

#ifndef MEGO_COMP__GCC__AVAILABLE
#define MEGO_COMP__GCC__AVAILABLE (0)
#endif

#endif
