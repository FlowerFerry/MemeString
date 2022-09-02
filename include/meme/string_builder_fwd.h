
#ifndef MEME_STRING_BUILDER_FWD_H_INCLUDED
#define MEME_STRING_BUILDER_FWD_H_INCLUDED

#include "string_fwd.h"

MEME_EXTERN_C_SCOPE_START


#ifdef MEME_STRING__BUILDER_SIZE
#undef MEME_STRING__BUILDER_SIZE
#endif
#define MEME_STRING_BUILDER__OBJECT_SIZE (sizeof(intptr_t) * 4)

typedef struct _MemeStringBuilderStack_t {
	MemeByte_t byte[MEME_STRING_BUILDER__OBJECT_SIZE];
} MemeStringBuilderStack_t;


MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_STRING_BUILDER_FWD_H_INCLUDED
