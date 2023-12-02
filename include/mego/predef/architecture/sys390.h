
#include "mego/predef/helper_macros.h"

#if !defined(MEGO_ARCH__STRING)

#if #if defined(__s390__) || defined(__s390x__)
#   define MEGO_ARCH__SYS390 MEGO__MAKE_VERSION_NUMBER(0, 0, 1)
#   define MEGO_ARCH__STRING "System/390"
#endif

#ifndef MEGO_ARCH__SYS390
#define MEGO_ARCH__SYS390 (0)
#endif

#endif 
