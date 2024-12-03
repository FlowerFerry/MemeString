
#ifndef MEGOPP_PREDEF_HAS_FORMAT_H_INCLUDED
#define MEGOPP_PREDEF_HAS_FORMAT_H_INCLUDED

#include <mego/predef/lang/version.h>

#ifdef __has_include
#  if __has_include(<format>) && MG_LANG__CXX20_AVAIL
#    define MEGOPP__HAS_FORMAT 1
#    define MEGOPP__HAS_STD_FORMAT 1
#  elif __has_include(<fmt/core.h>)
#    define MEGOPP__HAS_FORMAT 1
#    define MEGOPP__HAS_FMT_FORMAT 1
#  endif
#elif defined(FMT_HEADER_ONLY)
#  define MEGOPP__HAS_FORMAT 1
#  define MEGOPP__HAS_FMT_FORMAT 1
#endif

#ifndef MEGOPP__HAS_FORMAT
#define MEGOPP__HAS_FORMAT 0
#endif

#ifndef MEGOPP__HAS_STD_FORMAT
#define MEGOPP__HAS_STD_FORMAT 0
#endif

#ifndef MEGOPP__HAS_FMT_FORMAT
#define MEGOPP__HAS_FMT_FORMAT 0
#endif

#endif // !MEGOPP_PREDEF_HAS_FORMAT_H_INCLUDED
