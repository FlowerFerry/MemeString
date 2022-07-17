
#ifndef MEGO_PREDEF_SYMBOL_EXPORT_H_INCLUDED
#define MEGO_PREDEF_SYMBOL_EXPORT_H_INCLUDED

#include "mego/predef/os/windows.h"
#include "mego/predef/compiler/gcc.h"


#if MEGO_COMP__GCC__AVAILABLE
//
// Dynamic shared object (DSO) and dynamic-link library (DLL) support
//
#if __GNUC__ >= 4
#  if (defined(_WIN32) || defined(__WIN32__) || defined(WIN32)) && !defined(__CYGWIN__)
// All Win32 development environments, including 64-bit Windows and MinGW, define
// _WIN32 or one of its variant spellings. Note that Cygwin is a POSIX environment,
// so does not define _WIN32 or its variants.
#    define MEGO_SYMBOL__EXPORT __attribute__((__dllexport__))
#  else
#    define MEGO_SYMBOL__EXPORT __attribute__((__visibility__("default")))
#  endif

#endif

#endif

#if MEGO_OS__WINDOWS__AVAILABLE && !defined(MEGO_SYMBOL__EXPORT)
#  define MEGO_SYMBOL__EXPORT __declspec(dllexport)
#endif

#ifndef MEGO_SYMBOL__EXPORT
#define MEGO_SYMBOL__EXPORT
#endif

#endif // !MEGO_PREDEF_SYMBOL_EXPORT_H_INCLUDED