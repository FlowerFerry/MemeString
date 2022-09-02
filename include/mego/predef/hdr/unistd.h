
#ifndef MEGO_PREDEF_HDR_UNISTD_H_INCLUDED
#define MEGO_PREDEF_HDR_UNISTD_H_INCLUDED

#include "mego/predef/os/linux.h"
#include "mego/predef/os/windows.h"

#if !defined(MEGO_HDR__UNISTD_AVAILABLE) && MEGO_OS__LINUX__AVAILABLE
#   define MEGO_HDR__UNISTD_AVAILABLE (1)
#endif

#if !defined(MEGO_HDR__UNISTD_AVAILABLE) && MEGO_OS__WINDOWS__AVAILABLE \
    && defined(__MINGW32__) && ((__MINGW32_MAJOR_VERSION > 2) || ((__MINGW32_MAJOR_VERSION == 2) && (__MINGW32_MINOR_VERSION >= 0)))
#   define MEGO_HDR__UNISTD_AVAILABLE (1)
#endif

#if !defined(MEGO_HDR__UNISTD_AVAILABLE) && defined(__MSL__) && (__MSL__ >= 0x5000)
#   define MEGO_HDR__UNISTD_AVAILABLE (1)
#endif

#if defined(unix) \
      || defined(__unix) \
      || defined(_XOPEN_SOURCE) \
      || defined(_POSIX_SOURCE)

// generic unix platform:

#   ifndef MEGO_HDR__UNISTD_AVAILABLE
#   define MEGO_HDR__UNISTD_AVAILABLE (1)
#   endif
#endif


#ifndef MEGO_HDR__UNISTD_AVAILABLE
#define MEGO_HDR__UNISTD_AVAILABLE (0)
#endif

#endif // !MEGO_PREDEF_HDR_UNISTD_H_INCLUDED
