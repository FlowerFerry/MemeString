
#ifndef MEGO_PREDEF_SYMBOL_THREAD_LOCAL_H_INCLUDED
#define MEGO_PREDEF_SYMBOL_THREAD_LOCAL_H_INCLUDED

#include <mego/predef/compiler/visualc.h>
#include <mego/predef/compiler/gcc.h>
#include <mego/predef/compiler/clang.h>
#include <mego/predef/lang/version.h>

#if defined(__cplusplus)
#  if MG_LANG__CXX11_AVAIL
#    define MEGO__THREAD_LOCAL thread_local
#  endif
#elif MG_COMP__MSVC_AVAIL
#  define MEGO__THREAD_LOCAL __declspec(thread)
#elif MG_LANG__C23_AVAIL
#  define MEGO__THREAD_LOCAL thread_local
#elif MG_LANG__C11_AVAIL
#  include <threads.h>
#  define MEGO__THREAD_LOCAL thread_local
#elif (MG_COMP__GCC_AVAIL || MG_COMP__CLANG_AVAIL)
#  define MEGO__THREAD_LOCAL __thread
#endif

#ifndef MEGO__THREAD_LOCAL
#error "MEGO__THREAD_LOCAL is not defined"
#endif

#endif // !MEGO_PREDEF_SYMBOL_THREAD_LOCAL_H_INCLUDED
