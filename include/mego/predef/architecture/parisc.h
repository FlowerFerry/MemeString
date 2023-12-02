
#include "mego/predef/helper_macros.h"

#if !defined(MEGO_ARCH__STRING)

#if defined(__hppa__) || defined(__hppa) || defined(__HPPA__)

#   if !defined(MEGO_ARCH__PARISC) && (defined(_PA_RISC1_0))
#       define MEGO_ARCH__PARISC MEGO__MAKE_VERSION_NUMBER(1, 0, 0)
#   endif
#   if !defined(MEGO_ARCH__PARISC) && (defined(_PA_RISC1_1) || defined(__HPPA11__) || defined(__PA7100__))
#       define MEGO_ARCH__PARISC MEGO__MAKE_VERSION_NUMBER(1, 1, 0)
#   endif
#   if !defined(MEGO_ARCH__PARISC) && (defined(_PA_RISC2_0) || defined(__RISC2_0__) || defined(__HPPA20__) || defined(__PA8000__))
#       define MEGO_ARCH__PARISC MEGO__MAKE_VERSION_NUMBER(2, 0, 0)
#   endif
#   if !defined(MEGO_ARCH__PARISC)
#       define MEGO_ARCH__PARISC (1)
#   endif

#endif

#define MEGO_ARCH__STRING "HP/PA RISC"

#endif

#ifndef MEGO_ARCH__PARISC
#define MEGO_ARCH__PARISC (0)
#endif
