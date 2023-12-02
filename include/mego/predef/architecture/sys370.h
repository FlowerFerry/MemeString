
#include "mego/predef/helper_macros.h"

#if !defined(MEGO_ARCH__STRING)

#if defined(__370__) || defined(__THW_370__)
#   define MEGO_ARCH__SYS370 MEGO__MAKE_VERSION_NUMBER(0, 0, 1)
#   define MEGO_ARCH__STRING "System/370"
#endif

#ifndef MEGO_ARCH__SYS370
#define MEGO_ARCH__SYS370 (0)
#endif

#endif 
