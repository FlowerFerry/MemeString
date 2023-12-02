
#include "mego/predef/helper_macros.h"

#if !defined(MEGO_ARCH__STRING)

#if defined(__bfin__) || defined(__BFIN__) || \
    defined(bfin) || defined(BFIN)
    
#define MEGO_ARCH__BLACKFIN (1)

#define MEGO_ARCH__STRING "Blackfin"

#endif

#endif

#ifndef MEGO_ARCH__BLACKFIN
#define MEGO_ARCH__BLACKFIN (0)
#endif
