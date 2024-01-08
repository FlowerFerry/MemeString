
#ifndef MEGO_PREDEF_HAS_ATTRIBUTE_H_INCLUDED
#define MEGO_PREDEF_HAS_ATTRIBUTE_H_INCLUDED

#ifndef MG_HAS_ATTRIBUTE
#  if defined(__has_attribute)
#    define MG_HAS_ATTRIBUTE(x) __has_attribute(x)
#  else
#    define MG_HAS_ATTRIBUTE(x) 0
#  endif
#endif // !MG_HAS_ATTRIBUTE

#endif // !MEGO_PREDEF_HAS_ATTRIBUTE_H_INCLUDED
