
#include "mego/predef/helper_macros.h"

#if !defined(MEGO_ARCH__STRING)

#  if defined(i386) || defined(__i386) || defined(__i386__) || defined(__IA32__) || defined(_M_IX86)    \
      || defined(__X86__) || defined(_X86_) || defined(__THW_INTEL__) || defined(__I86__)               \
      || defined(__386) || defined(__INTEL__)

#     if   defined(__I86__)
#        define MEGO_ARCH__I386 MEGO__MAKE_VERSION_NUMBER(__I86__, 0, 0)
#     elif defined(_M_IX86)
#        define MEGO_ARCH__I386 MEGO__MAKE_VERSION_NUMBER(((_M_IX86) / 100) % 100, 0, 0)
#     elif defined(__i686__)
#        define MEGO_ARCH__I386 MEGO__MAKE_VERSION_NUMBER(6, 0, 0)
#     elif defined(__i586__)
#        define MEGO_ARCH__I386 MEGO__MAKE_VERSION_NUMBER(5, 0, 0)
#     elif defined(__i486__)
#        define MEGO_ARCH__I386 MEGO__MAKE_VERSION_NUMBER(4, 0, 0)
#     elif defined(__i386__)
#        define MEGO_ARCH__I386 MEGO__MAKE_VERSION_NUMBER(3, 0, 0)
#     endif

//! @def MEGO_ARCH__I386
//! @brief
#     if !defined(MEGO_ARCH__I386)
#        define MEGO_ARCH__I386 MEGO__MAKE_VERSION_NUMBER(0,0,1)
#     endif

#     define MEGO_ARCH__STRING "i386"

#  elif defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) 

#     if defined(_M_X64)
#        define MEGO_ARCH__AMD64 MEGO__MAKE_VERSION_NUMBER(((_M_X64) / 100) % 100, 0, 0)
#     endif 

//! @def MEGO_ARCH__AMD64
//! @brief
#     if !defined(MEGO_ARCH__AMD64)
#        define MEGO_ARCH__AMD64 MEGO__MAKE_VERSION_NUMBER(0, 0, 1)
#     endif

#     define MEGO_ARCH__STRING "amd64"

#  endif


#ifndef MEGO_ARCH__I386
#define MEGO_ARCH__I386 (0)
#endif

#ifndef MEGO_ARCH__AMD64
#define MEGO_ARCH__AMD64 (0)
#endif 

#define MEGO_ARCH__X86 MEGO_ARCH__I386
#define MEGO_ARCH__X64 MEGO_ARCH__AMD64

#endif
