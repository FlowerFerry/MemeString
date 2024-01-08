
#ifndef MEGO_PREDEF_INLINE_H_INCLUDED
#define MEGO_PREDEF_INLINE_H_INCLUDED

#include <mego/predef/has_attribute.h>

#if !defined(MG_INLINE) && defined(_MSC_VER)
#  if _MSC_VER >= 1200
#    define MG_INLINE __forceinline
#  else
#    define MG_INLINE __inline 
#  endif
#endif

#if !defined(MG_INLINE)
#  if MG_HAS_ATTRIBUTE(always_inline) || (defined(__GNUC__) && __GNUC__ >= 4)
#    define MG_INLINE __inline__ __attribute__((always_inline))
#  elif defined(__clang__) || defined(__GNUC__)
#    define MG_INLINE __inline__
#  elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#    define MG_INLINE inline
#  endif
#endif

#if !defined(MG_INLINE)
#  if defined(__cplusplus)
#    define MG_INLINE inline
#  endif
#endif

#if !defined(MG_INLINE)
#  define MG_INLINE
#endif

#if defined(__cplusplus)
#  define MG_CAPI_INLINE MG_INLINE
#else
#  define MG_CAPI_INLINE static MG_INLINE
#endif

#endif // !MEGO_PREDEF_INLINE_H_INCLUDED
