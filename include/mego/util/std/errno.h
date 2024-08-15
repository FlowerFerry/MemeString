
#ifndef MEGO_UTIL_STD_ERRNO_H_INCLUDED
#define MEGO_UTIL_STD_ERRNO_H_INCLUDED

#include <mego/predef/os/windows.h>

#ifndef __STDC_WANT_LIB_EXT1__
#define __STDC_WANT_LIB_EXT1__ 1
// #define __MEGO_UNDEF__STDC_WANT_LIB_EXT1__ 1
#endif
#include <errno.h>

#if !MG_OS__WIN_AVAIL && !defined(__STDC_LIB_EXT1__)

typedef int errno_t;

#endif 

// #ifdef __MEGO_UNDEF__STDC_WANT_LIB_EXT1__
// #undef __STDC_WANT_LIB_EXT1__
// #undef __MEGO_UNDEF__STDC_WANT_LIB_EXT1__
// #endif

#endif // !MEGO_UTIL_STD_ERRNO_H_INCLUDED
