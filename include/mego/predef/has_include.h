
#ifndef MEGO_PREDEF_HAS_INCLUDE_H_INCLUDED
#define MEGO_PREDEF_HAS_INCLUDE_H_INCLUDED

#ifndef MG_HAS_INCLUDE
#  if defined(__has_include)
#    define MG_HAS_INCLUDE(x) __has_include(x)
#  else
#    define MG_HAS_INCLUDE(x) 0
#  endif
#endif // !MG_HAS_INCLUDE

#endif // !MEGO_PREDEF_HAS_INCLUDE_H_INCLUDED
