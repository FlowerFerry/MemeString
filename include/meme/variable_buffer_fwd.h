
#ifndef MEME_VARIABLE_BUFFER_FWD_H_INCLUDED
#define MEME_VARIABLE_BUFFER_FWD_H_INCLUDED

#include "buffer_fwd.h"

MEME_EXTERN_C_SCOPE_START

typedef struct _MemeVariableBuffer_t* MemeVariableBuffer_t;
typedef const struct _MemeVariableBuffer_t* MemeVariableBuffer_Const_t;

typedef struct _MemeVariableBuffer_t* mmvb_t;
typedef const struct _MemeVariableBuffer_t* mmvb_const_t;

typedef MemeInteger_t MemeVariableBuffer_Storage_t;
enum _MemeVariableBuffer_Storage_t {
	MemeVariableBuffer_StorageType_none   = MemeString_StorageType_none,
	MemeVariableBuffer_StorageType_small  = MemeString_StorageType_small,
	MemeVariableBuffer_StorageType_large  = MemeString_StorageType_large,
	MemeVariableBuffer_StorageType_medium = MemeString_StorageType_medium,
	MemeVariableBuffer_StorageType_user   = MemeString_StorageType_user,
};

enum _MemeVariableBuffer_UnsafeStorage_t {
	MemeVariableBuffer_UnsafeStorageType_view = MemeString_UnsafeStorageType_view
};

typedef struct _MemeVariableBufferStack_t {
	MemeByte_t byte[MEME_STRING__OBJECT_SIZE];
} MemeVariableBufferStack_t;

typedef MemeVariableBufferStack_t mmvb_stack_t;

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_VARIABLE_BUFFER_FWD_H_INCLUDED
