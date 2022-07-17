
#ifndef MEME_STRING_EDITOR_FWD_H_INCLUDED
#define MEME_STRING_EDITOR_FWD_H_INCLUDED

#include "string_fwd.h"

MEME_EXTERN_C_SCOPE_START


#ifdef MEME_STRING__EDITOR_SIZE
#undef MEME_STRING__EDITOR_SIZE
#endif
#define MEME_STRING__EDITOR_SIZE (sizeof(intptr_t) * 6)

typedef struct _MemeStringEditorStack_t {
	MemeByte_t byte[MEME_STRING__EDITOR_SIZE];
} MemeStringEditorStack_t;


MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_STRING_EDITOR_FWD_H_INCLUDED
