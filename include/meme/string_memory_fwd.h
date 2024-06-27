
#ifndef MEME_STRING_MEMORY_FWD_H_INCLUDED
#define MEME_STRING_MEMORY_FWD_H_INCLUDED

#include "common.h"

#include <stdint.h>
#include <stddef.h>

MEME_EXTERN_C_SCOPE_START

//! DEPRECATED 
typedef void* (MemeString_MallocFunction_t)(size_t);

//! DEPRECATED
typedef void* (MemeString_ReallocFunction_t)(void*, size_t);

//! DEPRECATED
typedef void  (MemeString_FreeFunction_t)(void*);

//! DEPRECATED
typedef void* (MemeString_CallocFunction_t)(size_t, size_t);

typedef void MemeString_UserObjectDestruct_t(void* _object);
typedef const char* MemeString_UserObjectData_t(const void* _object);
typedef size_t MemeString_UserObjectSize_t(const void* _object);

typedef void* (MemeMemory_MallocFunc_t)(size_t);
typedef void* (MemeMemory_ReallocFunc_t)(void*, size_t);
typedef void  (MemeMemory_FreeFunc_t)(void*);
typedef void* (MemeMemory_CallocFunc_t)(size_t, size_t);

typedef void* (MemeMemory_UserMallocFunc_t)(size_t, void*);
typedef void* (MemeMemory_UserReallocFunc_t)(void*, size_t, void*);
typedef void  (MemeMemory_UserFreeFunc_t)(void*, void*);
typedef void* (MemeMemory_UserCallocFunc_t)(size_t, size_t, void*);

//! DEPRECATED
typedef MemeString_MallocFunction_t  mmsmem_malloc_t;

//! DEPRECATED
typedef MemeString_ReallocFunction_t mmsmem_realloc_t;

//! DEPRECATED
typedef MemeString_FreeFunction_t    mmsmem_free_t;

//! DEPRECATED
typedef MemeString_CallocFunction_t  mmsmem_calloc_t;

typedef MemeString_UserObjectDestruct_t mmstr_usr_obj_destruct_fn_t;
typedef MemeString_UserObjectData_t     mmstr_usr_obj_data_fn_t;
typedef MemeString_UserObjectSize_t     mmstr_usr_obj_size_fn_t;

typedef MemeMemory_MallocFunc_t  mmmem_malloc_fn_t;
typedef MemeMemory_ReallocFunc_t mmmem_realloc_fn_t;
typedef MemeMemory_FreeFunc_t    mmmem_free_fn_t;
typedef MemeMemory_CallocFunc_t  mmmem_calloc_fn_t;

typedef MemeMemory_UserMallocFunc_t  mmmem_usr_malloc_fn_t;
typedef MemeMemory_UserReallocFunc_t mmmem_usr_realloc_fn_t;
typedef MemeMemory_UserFreeFunc_t    mmmem_usr_free_fn_t;
typedef MemeMemory_UserCallocFunc_t  mmmem_usr_calloc_fn_t;

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_STRING_MEMORY_FWD_H_INCLUDED
