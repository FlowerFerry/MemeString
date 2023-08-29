
#ifndef MEME_STRING_BUILDER_FWD_H_INCLUDED
#define MEME_STRING_BUILDER_FWD_H_INCLUDED

#include "string_fwd.h"

MEME_EXTERN_C_SCOPE_START

typedef struct _MemeStringBuilder_t* MemeStringBuilder_t;
typedef const struct _MemeStringBuilder_t* MemeStringBuilder_Const_t;

typedef struct _MemeStringBuilder_t* mmsbldr_t;
typedef const struct _MemeStringBuilder_t* mmsbldr_const_t;

#ifdef MEME_STRING_BUILDER__OBJECT_SIZE
#undef MEME_STRING_BUILDER__OBJECT_SIZE
#endif
#define MEME_STRING_BUILDER__OBJECT_SIZE (sizeof(intptr_t) * 4)

#ifndef MMSB__OBJECT_SIZE
#define MMSBUILDER__OBJECT_SIZE MEME_STRING_BUILDER__OBJECT_SIZE
#endif

typedef struct _MemeStringBuilderStack_t {
	MemeByte_t byte[MMSBUILDER__OBJECT_SIZE];
} MemeStringBuilderStack_t;
typedef MemeStringBuilderStack_t mmsbldrstk_t;

enum {
    mmsbuilder_flag__copy = 1
};

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_STRING_BUILDER_FWD_H_INCLUDED
