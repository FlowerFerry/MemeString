
#ifndef MEGO_PREDEF_SYMBOL_STATIC_ASSERT_H_INCLUDED
#define MEGO_PREDEF_SYMBOL_STATIC_ASSERT_H_INCLUDED

#include <mego/predef/compiler/visualc.h>
#include <mego/predef/lang/version.h>

#ifndef MEGO__STATIC_ASSERT
#if MG_COMP__MSVC_AVAIL && _MSC_VER >= 1600
#  define MEGO__STATIC_ASSERT(expr, msg) static_assert(expr, msg)
#endif 
#endif

#ifndef MEGO__STATIC_ASSERT
#if MG_LANG__CXX11_AVAIL
#  define MEGO__STATIC_ASSERT(expr, msg) static_assert(expr, msg)
#endif
#endif

#ifndef MEGO__STATIC_ASSERT
#if MG_LANG__C11_AVAIL
#  define MEGO__STATIC_ASSERT(expr, msg) _Static_assert(expr, msg)
#endif
#endif

#endif // !MEGO_PREDEF_SYMBOL_STATIC_ASSERT_H_INCLUDED
