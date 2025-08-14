
#ifndef MEGO_PREDEF_SYMBOL_TARGET_NEON_H_INCLUDED
#define MEGO_PREDEF_SYMBOL_TARGET_NEON_H_INCLUDED

#include <mego/predef/architecture/arm.h>
#include <mego/predef/compiler/gcc.h>
#include <mego/predef/compiler/clang.h>

#if MEGO_ARCH__ARM && defined(__ARM_NEON) && !MEGO_ARCH__ARM64 && (MG_COMP__GCC_AVAIL || MG_COMP__CLANG_AVAIL)
#define MEGO_SYMBOL__TARGET_NEON __attribute__((target("fpu=neon")))
#else
#define MEGO_SYMBOL__TARGET_NEON
#endif

#ifndef MG_SYM__TARGET_NEON
#define MG_SYM__TARGET_NEON MEGO_SYMBOL__TARGET_NEON
#endif // !MG_SYM__TARGET_NEON

#endif // !MEGO_PREDEF_SYMBOL_TARGET_NEON_H_INCLUDED
