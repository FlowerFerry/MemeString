
#ifndef MEGO_PRAGMA_MESSAGE_H_INCLUDED
#define MEGO_PRAGMA_MESSAGE_H_INCLUDED

#include "mego/predef/compiler.h"

#ifndef MEGO__PRAGMA_MSG_ENABLE
#define MEGO__PRAGMA_MSG_ENABLE (0)
#endif

#if !defined(MEGO__PRAGMA_MSG) && MEGO__PRAGMA_MSG_ENABLE
#   if MEGO_COMP__MSVC__AVAILABLE
#       define MEGO__PRAGMA_MSG(X) __pragma(X)
#   endif
#   if MEGO_COMP__GCC__AVAILABLE
#       define MEGO__PRAGMA_MSG(X) _Pragma(X)
#   endif

#   ifndef MEGO__PRAGMA_MSG
#   define MEGO__PRAGMA_MSG(X)
#   endif
#endif // !

#endif // !MEGO_PRAGMA_MESSAGE_H_INCLUDED
