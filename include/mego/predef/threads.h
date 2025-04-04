
#ifndef MEGO_PREDEF_THREADS_H_INCLUDED
#define MEGO_PREDEF_THREADS_H_INCLUDED

#include "mego/predef/compiler.h"
#include "mego/predef/os/windows.h"
#include "mego/predef/hdr/unistd.h"
#include <mego/predef/helper_macros.h>

#include <stddef.h>

#if MEGO_HDR__UNISTD_AVAILABLE
#  include <unistd.h>

#  if !defined(MEGO_THR__PTHREADS_AVAILABLE) && defined(_POSIX_THREADS) && (_POSIX_THREADS+0 >= 0) \
		&& !defined(MEGO_THR__WINTHREADS_AVAILABLE)
#    define MEGO_THR__PTHREADS_AVAILABLE (1)

#    if !defined(__GLIBCXX__) && defined(__GLIBC__) && defined(__GLIBC_MINOR__) \
        && MEGO__MAKE_VERSION_NUMBER(__GLIBC__+0, __GLIBC_MINOR__+0, 0) >= MEGO__MAKE_VERSION_NUMBER(2, 3, 0)
#      define MEGO_THR__THREADS_AVAILABLE (1)
#    endif

#    if defined(__UCLIBC__) \
        && defined(__UCLIBC_MAJOR__) && defined(__UCLIBC_MINOR__) && defined(__UCLIBC_SUBLEVEL__) \
        && MEGO__MAKE_VERSION_NUMBER(__UCLIBC_MAJOR__+0, __UCLIBC_MINOR__+0, __UCLIBC_SUBLEVEL__+0) >= MEGO__MAKE_VERSION_NUMBER(0, 9, 33)
#      define MEGO_THR__THREADS_AVAILABLE (1)
#    endif

#  endif

#endif

#if !defined(MEGO_THR__THREADS_AVAILABLE) && MEGO_COMP__GCC__AVAILABLE \
	&& !defined(__MINGW32__) && !defined(linux) && !defined(__linux) && !defined(__linux__)
#  define MEGO_THR__THREADS_AVAILABLE (1)
#endif

#if !defined(MEGO_THR__THREADS_AVAILABLE) && defined(__GLIBCXX__) // gcc 3.4 and greater:
#  if defined(_GLIBCXX_HAVE_GTHR_DEFAULT) \
        || defined(_GLIBCXX__PTHREADS) \
        || defined(_GLIBCXX_HAS_GTHREADS) \
        || defined(_WIN32) \
        || defined(_AIX) \
        || defined(__HAIKU__)

#    define MEGO_THR__THREADS_AVAILABLE (1)
#  else
#    define MEGO_THR__THREADS_ENABLE (0)
#  endif
#elif !defined(MEGO_THR__THREADS_AVAILABLE) && defined(__GLIBCPP__) \
        && !defined(_GLIBCPP_HAVE_GTHR_DEFAULT) \
        && !defined(_GLIBCPP__PTHREADS)
#  define MEGO_THR__THREADS_ENABLE (0)
#endif

#if !defined(MEGO_THR__THREADS_AVAILABLE) && (defined(linux) || defined(__linux) || defined(__linux__)) \
    && defined(__arm__) && defined(_GLIBCPP_HAVE_GTHR_DEFAULT)
#  define MEGO_THR__THREADS_AVAILABLE (1)
#endif


#if !defined(MEGO_THR__WINTHREADS_AVAILABLE) \
	&& MEGO_OS__WINDOWS__AVAILABLE && !defined(MEGO_THR__PTHREADS_AVAILABLE)
#  define MEGO_THR__WINTHREADS_AVAILABLE (1)
#endif

#if !defined(MEGO_THR__THREADS_AVAILABLE) && (defined(__MT__) || defined(_MT) || defined(_REENTRANT) \
    || defined(_PTHREADS) || defined(__APPLE__) || defined(__DragonFly__))
#  define MEGO_THR__THREADS_AVAILABLE (1)
#endif

#ifndef MEGO_THR__THREADS_ENABLE
#define MEGO_THR__THREADS_ENABLE (1)
#endif

#if !MEGO_THR__THREADS_ENABLE && defined(MEGO_THR__THREADS_AVAILABLE)
#  undef MEGO_THR__THREADS_AVAILABLE
#  define MEGO_THR__THREADS_AVAILABLE (0)
#endif


#if defined(MEGO_THR__THREADS_AVAILABLE) \
	&& !defined(MEGO_THR__PTHREADS_AVAILABLE) \
    && !defined(MEGO_THR__WINTHREADS_AVAILABLE)
#  undef MEGO_THR__THREADS_AVAILABLE
#  define MEGO_THR__THREADS_AVAILABLE (0)
#endif

#if !MEGO_THR__THREADS_AVAILABLE
#  undef MEGO_THR__PTHREADS_AVAILABLE
#  undef MEGO_THR__WINTHREADS_AVAILABLE

#define MEGO_THR__PTHREADS_AVAILABLE   (0)
#define MEGO_THR__WINTHREADS_AVAILABLE (0)
#endif


#ifndef MEGO_THR__THREADS_AVAILABLE
#define MEGO_THR__THREADS_AVAILABLE (0)
#endif
#ifndef MEGO_THR__PTHREADS_AVAILABLE
#define MEGO_THR__PTHREADS_AVAILABLE (0)
#endif
#ifndef MEGO_THR__WINTHREADS_AVAILABLE
#define MEGO_THR__WINTHREADS_AVAILABLE (0)
#endif

#ifndef MG_THR__THREADS_AVAIL
#define MG_THR__THREADS_AVAIL MEGO_THR__THREADS_AVAILABLE
#endif

#ifndef MG_THR__PTHREADS_AVAIL
#define MG_THR__PTHREADS_AVAIL MEGO_THR__PTHREADS_AVAILABLE
#endif

#ifndef MG_THR__WINTHREADS_AVAIL
#define MG_THR__WINTHREADS_AVAIL MEGO_THR__WINTHREADS_AVAILABLE
#endif

#endif // !MEGO_PREDEF_THREADS_H_INCLUDED
