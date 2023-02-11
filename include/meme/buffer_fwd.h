
#ifndef MEME_BUFFER_FWD_H_INCLUDED
#define MEME_BUFFER_FWD_H_INCLUDED

#include "string_fwd.h"

MEME_EXTERN_C_SCOPE_START

typedef struct _MemeBuffer_t* MemeBuffer_t;
typedef const struct _MemeBuffer_t* MemeBuffer_Const_t;

typedef struct _MemeBuffer_t* mmbuf_t;
typedef const struct _MemeBuffer_t* mmbuf_const_t;

typedef MemeInteger_t MemeBuffer_Storage_t;
enum _MemeBuffer_Storage_t {
	MemeBuffer_StorageType_none   = MemeString_StorageType_none,
	MemeBuffer_StorageType_small  = MemeString_StorageType_small,
	MemeBuffer_StorageType_medium = MemeString_StorageType_medium,
	MemeBuffer_StorageType_large  = MemeString_StorageType_large,
	MemeBuffer_StorageType_user   = MemeString_StorageType_user
};
enum _MemeBuffer_UnsafeStorage_t {
	MemeBuffer_UnsafeStorageType_view = MemeString_UnsafeStorageType_view
};

typedef struct _MemeBufferStack_t {
	MemeByte_t byte[MEME_STRING__OBJECT_SIZE];
} MemeBufferStack_t, mmbuf_stack_t;

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_BUFFER_FWD_H_INCLUDED
