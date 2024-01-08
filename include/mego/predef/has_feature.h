
#ifndef MEGO_PREDEF_HAS_FEATURE_H_INCLUDED
#define MEGO_PREDEF_HAS_FEATURE_H_INCLUDED

#ifndef MG_HAS_FEATURE
#  if defined(__has_feature)
#    define MG_HAS_FEATURE(x) __has_feature(x)
#  else
#    define MG_HAS_FEATURE(x) 0
#  endif
#endif // !MG_HAS_FEATURE

#endif // !MEGO_PREDEF_HAS_FEATURE_H_INCLUDED
