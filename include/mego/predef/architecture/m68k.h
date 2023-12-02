
#include "mego/predef/helper_macros.h"

#if !defined(MEGO_ARCH__STRING)

#if defined(__m68k__) || defined(M68000)

#   if !defined(MEGO_ARCH__M68K) && (defined(__mc68060__) || defined(mc68060) || defined(__mc68060))
#       define MEGO_ARCH__M68K MEGO__MAKE_VERSION_NUMBER(6, 0, 0)
#   endif
#   if !defined(MEGO_ARCH__M68K) && (defined(__mc68040__) || defined(mc68040) || defined(__mc68040))
#       define MEGO_ARCH__M68K MEGO__MAKE_VERSION_NUMBER(4, 0, 0)
#   endif
#   if !defined(MEGO_ARCH__M68K) && (defined(__mc68030__) || defined(mc68030) || defined(__mc68030))
#       define MEGO_ARCH__M68K MEGO__MAKE_VERSION_NUMBER(3, 0, 0)
#   endif
#   if !defined(MEGO_ARCH__M68K) && (defined(__mc68020__) || defined(mc68020) || defined(__mc68020))
#       define MEGO_ARCH__M68K MEGO__MAKE_VERSION_NUMBER(2, 0, 0)
#   endif
#   if !defined(MEGO_ARCH__M68K) && (defined(__mc68010__) || defined(mc68010) || defined(__mc68010))
#       define MEGO_ARCH__M68K MEGO__MAKE_VERSION_NUMBER(1, 0, 0)
#   endif

#   if !defined(MEGO_ARCH__M68K)
#       define MEGO_ARCH__M68K (1)
#   endif

#define MEGO_ARCH__STRING "Motorola 68k"

#endif

#endif

#ifndef MEGO_ARCH__M68K
#define MEGO_ARCH__M68K (0)
#endif
