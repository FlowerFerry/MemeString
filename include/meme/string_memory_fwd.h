
#ifndef MEME_STRING_MEMORY_FWD_H_INCLUDED
#define MEME_STRING_MEMORY_FWD_H_INCLUDED

#include "common.h"

#include <stdint.h>
#include <stddef.h>

MEME_EXTERN_C_SCOPE_START

typedef void* (MemeString_MallocFunction_t)(size_t);
typedef void* (MemeString_ReallocFunction_t)(void*, size_t);
typedef void  (MemeString_FreeFunction_t)(void*);
typedef void* (MemeString_CallocFunction_t)(size_t, size_t);

typedef void MemeString_UserObjectDestruct_t(void* _object);
typedef const char* MemeString_UserObjectData_t(const void* _object);
typedef size_t MemeString_UserObjectSize_t(const void* _object);

typedef MemeString_MallocFunction_t  mmsmem_malloc_t;
typedef MemeString_ReallocFunction_t mmsmem_realloc_t;
typedef MemeString_FreeFunction_t    mmsmem_free_t;
typedef MemeString_CallocFunction_t  mmsmem_calloc_t;

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_STRING_MEMORY_FWD_H_INCLUDED
