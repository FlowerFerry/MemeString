
#ifndef MEGO_PREDEF_SYMBOL_TARGET_AVX2_H_INCLUDED
#define MEGO_PREDEF_SYMBOL_TARGET_AVX2_H_INCLUDED

#include <mego/predef/architecture/x86.h>
#include <mego/predef/compiler/gcc.h>
#include <mego/predef/compiler/clang.h>

#if (MEGO_ARCH__X86 || MEGO_ARCH__AMD64) && (MG_COMP__GCC_AVAIL || MG_COMP__CLANG_AVAIL)
#define MEGO_SYMBOL__TARGET_AVX2 __attribute__((target("avx2")))
#else
#define MEGO_SYMBOL__TARGET_AVX2
#endif

#ifndef MG_SYM__TARGET_AVX2
#define MG_SYM__TARGET_AVX2 MEGO_SYMBOL__TARGET_AVX2
#endif // !MG_SYM__TARGET_AVX2

#endif // !MEGO_PREDEF_SYMBOL_TARGET_AVX2_H_INCLUDED
