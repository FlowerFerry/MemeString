
#ifndef MEME_STRING_FWD_H_INCLUDED
#define MEME_STRING_FWD_H_INCLUDED

#include "common.h"

#include <stdint.h>
#include <stddef.h>

MEME_EXTERN_C_SCOPE_START

typedef uint8_t  MemeByte_t;
typedef intptr_t MemeInteger_t;

typedef struct _MemeString_t* MemeString_t;
typedef const struct _MemeString_t* MemeString_Const_t;

typedef struct _MemeWordIndex_t {
	MemeInteger_t size;
    const MemeByte_t * data;
} MemeWordIndex_t;

typedef struct _MemeWord_t {
    MemeByte_t byte[6];
    MemeByte_t __spare__[2];
} MemeWord_t;

typedef MemeInteger_t MemeFlag_CaseSensitivity_t;
enum _MemeFlag_CaseSensitivity_t 
{
    MemeFlag_CaseSensitive
};

typedef MemeInteger_t MemeString_Storage_t;
enum _MemeString_Storage_t {
	MemeString_StorageType_none			= 0,
	MemeString_StorageType_small		= 1,
	MemeString_StorageType_medium		= 2,
	MemeString_StorageType_large		= 3,
	MemeString_StorageType_user			= 63
};
enum _MemeString_UnsafeStorage_t {
	MemeString_UnsafeStorageType_view   = 62
};

#ifdef MEME_STRING__OBJECT_SIZE
#undef MEME_STRING__OBJECT_SIZE
#endif
#define MEME_STRING__OBJECT_SIZE (sizeof(intptr_t) * 3)


typedef struct _MemeStringStack_t {
	MemeByte_t byte[MEME_STRING__OBJECT_SIZE];
} MemeStringStack_t;

#ifndef MEME_ERRNO__OFFSET
#define MEME_ERRNO__OFFSET (-100000)
#endif

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_STRING_FWD_H_INCLUDED
