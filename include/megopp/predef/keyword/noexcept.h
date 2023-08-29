
#ifndef MEGOPP_PREDEF_KEYWORD_NOEXCEPT_H_INCLUDED
#define MEGOPP_PREDEF_KEYWORD_NOEXCEPT_H_INCLUDED

#include "mego/predef/compiler.h"
#include "mego/predef/helper_macros.h"

#include "megopp/predef/has_cxx11.h"

#if MEGO_COMP__MSVC__AVAILABLE

#if !defined(MEGOPP_CXX11__NOEXCEPT__AVAILABLE) \
    && (_MSC_FULL_VER < 190023026)
#   define MEGOPP_CXX11__NOEXCEPT__AVAILABLE (0)
#endif

#endif

#if MEGO_COMP__GCC__AVAILABLE

#if (MEGO_COMP__GNUC_VERSION < MEGO__MAKE_VERSION_NUMBER(4, 6, 0)) || !MEGO__GCC_CXX11__AVAILABLE
#   define MEGOPP_CXX11__NOEXCEPT__AVAILABLE (0)
#endif

#endif 


#if MEGO_COMP__CLANG__AVAILABLE

#if !__has_feature(cxx_noexcept)
#  undef  MEGOPP_CXX11__NOEXCEPT__AVAILABLE
#  define MEGOPP_CXX11__NOEXCEPT__AVAILABLE (0)
#else
#  undef  MEGOPP_CXX11__NOEXCEPT__AVAILABLE
#endif

#endif 

#ifndef MEGOPP_CXX11__NOEXCEPT__AVAILABLE
#define MEGOPP_CXX11__NOEXCEPT__AVAILABLE (1)
#endif

#if MEGOPP_CXX11__NOEXCEPT__AVAILABLE
#  define MEGOPP__NOEXCEPT noexcept
#  define MEGOPP__NOEXCEPT_OR_NOTHROW noexcept
#  define MEGOPP__NOEXCEPT_IF(Predicate) noexcept((Predicate))
#  define MEGOPP__NOEXCEPT_EXPR(Expression) noexcept((Expression))
#else
#  define MEGOPP__NOEXCEPT
#  define MEGOPP__NOEXCEPT_OR_NOTHROW throw()
#  define MEGOPP__NOEXCEPT_IF(Predicate)
#  define MEGOPP__NOEXCEPT_EXPR(Expression) false
#endif


#endif // !MEGOPP_PREDEF_KEYWORD_NOEXCEPT_H_INCLUDED
