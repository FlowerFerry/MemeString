
#include <meme/conc/atomic.h>
#include <mego/predef/compiler/visualc.h>

#if MG_COMP__MSVC_AVAIL
#  include "meme_conc_msvc_atomic.c.inl"
#else
#  include "meme_conc_std_atomic.c.inl"
#endif

MEME_EXTERN_C_SCOPE_START

MEME_EXTERN_C_SCOPE_ENDED
