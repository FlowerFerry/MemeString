
#ifndef MEGO_PREDEF_LIKELY_H_INCLUDED
#define MEGO_PREDEF_LIKELY_H_INCLUDED

#include "mego/predef/compiler.h"

#ifndef MEGO_SYMBOL__LIKELY
//
//#if MEGO_COMP__MSVC__AVAILABLE
//
//#define MEGO_SYMBOL__LIKELY(x) x
//#define MEGO_SYMBOL__UNLIKELY(x) x
//
//#endif

#if MEGO_COMP__GCC__AVAILABLE

#define MEGO_SYMBOL__LIKELY(x) __builtin_expect(x, 1)
#define MEGO_SYMBOL__UNLIKELY(x) __builtin_expect(x, 0)

#endif

#if MEGO_COMP__CLANG__AVAILABLE

// Branch prediction hints
#if !defined (__c2__) && defined(__has_builtin)
#if __has_builtin(__builtin_expect)
#define MEGO_SYMBOL__LIKELY(x) __builtin_expect(x, 1)
#define MEGO_SYMBOL__UNLIKELY(x) __builtin_expect(x, 0)
#endif
#endif

#endif

#endif

#ifndef MEGO_SYMBOL__LIKELY
#define MEGO_SYMBOL__LIKELY(x) (x)
#define MEGO_SYMBOL__UNLIKELY(x) (x)
#endif

#endif // !MEGO_PREDEF_LIKELY_H_INCLUDED
