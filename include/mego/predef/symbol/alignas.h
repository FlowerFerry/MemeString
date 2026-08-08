
#ifndef MEGO_PREDEF_SYMBOL_ALIGNAS_H_INCLUDED
#define MEGO_PREDEF_SYMBOL_ALIGNAS_H_INCLUDED

#include <mego/predef/compiler/visualc.h>
#include <mego/predef/compiler/gcc.h>
#include <mego/predef/compiler/clang.h>
#include <mego/predef/lang/version.h>

#ifndef MEGO__ALIGNAS
#  if defined(__cplusplus)
#    if MG_LANG__CXX11_AVAIL
#      define MEGO__ALIGNAS(n) alignas(n)
#    endif
#  elif MG_LANG__C11_AVAIL
#    define MEGO__ALIGNAS(n) _Alignas(n)
#  elif MG_COMP__MSVC_AVAIL
#    define MEGO__ALIGNAS(n) __declspec(align(n))
#  elif (MG_COMP__GCC_AVAIL || MG_COMP__CLANG_AVAIL)
#    define MEGO__ALIGNAS(n) __attribute__((aligned(n)))
#  endif
#endif

#ifndef MEGO__ALIGNAS
#  define MEGO__ALIGNAS(n)
#endif

#endif // !MEGO_PREDEF_SYMBOL_ALIGNAS_H_INCLUDED
