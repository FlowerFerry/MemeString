
#ifndef MEGO_PREDEF_SYMBOL_VISIBILITY_HIDDEN_H_INCLUDED
#define MEGO_PREDEF_SYMBOL_VISIBILITY_HIDDEN_H_INCLUDED

#include <mego/predef/compiler/clang.h>
#include <mego/predef/compiler/gcc.h>

#if MG_COMP__CLANG_AVAIL || MG_COMP__GCC_AVAIL
#   define MEGO_SYMBOL__VISIBILITY_HIDDEN __attribute__((visibility("hidden")))
#else
#   define MEGO_SYMBOL__VISIBILITY_HIDDEN
#endif

#ifndef MG_SYM__VIS_HIDDEN
#define MG_SYM__VIS_HIDDEN MEGO_SYMBOL__VISIBILITY_HIDDEN
#endif

#endif // !MEGO_PREDEF_SYMBOL_VISIBILITY_HIDDEN_H_INCLUDED
