
#ifndef MEGO_PREDEF_SYMBOL_COMPILER_MSVC_FORMAT_H_INCLUDED
#define MEGO_PREDEF_SYMBOL_COMPILER_MSVC_FORMAT_H_INCLUDED

#include "mego/predef/compiler/visualc.h"

#if MEGO_COMP__MSVC__AVAILABLE

#undef MEGO_SYMBOL__MSVC_FORMAT_STRING
#if _MSC_VER >= 1400
# include <sal.h>
# if _MSC_VER > 1400
#  define MEGO_SYMBOL__MSVC_FORMAT_STRING(p) _Printf_format_string_ p
# else
#  define MEGO_SYMBOL__MSVC_FORMAT_STRING(p) __format_string p
# endif /* FORMAT_STRING */
#else
# define MEGO_SYMBOL__MSVC_FORMAT_STRING(p) p
#endif /* _MSC_VER */

#endif

#ifndef MEGO_SYMBOL__MSVC_FORMAT_STRING
#define MEGO_SYMBOL__MSVC_FORMAT_STRING(STRING) STRING
#endif

#endif // !MEGO_PREDEF_SYMBOL_COMPILER_MSVC_FORMAT_H_INCLUDED
