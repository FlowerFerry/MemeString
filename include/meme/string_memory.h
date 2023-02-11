
#ifndef MEME_STRING_MEMORY_H_INCLUDED
#define MEME_STRING_MEMORY_H_INCLUDED

#include "meme/common.h"
#include "meme/string_memory_fwd.h"

MEME_EXTERN_C MEME_API mmsmem_malloc_t*
	MEME_STDCALL mmsmem_get_malloc_func();
MEME_EXTERN_C MEME_API mmsmem_calloc_t*
	MEME_STDCALL mmsmem_get_calloc_func();
MEME_EXTERN_C MEME_API mmsmem_realloc_t*
	MEME_STDCALL mmsmem_get_realloc_func();
MEME_EXTERN_C MEME_API mmsmem_free_t*
	MEME_STDCALL mmsmem_get_free_func();

MEME_EXTERN_C MEME_API void*
	MEME_STDCALL mmsmem_malloc (size_t _size);
MEME_EXTERN_C MEME_API void*
	MEME_STDCALL mmsmem_calloc (size_t _count, size_t _size);
MEME_EXTERN_C MEME_API void*
	MEME_STDCALL mmsmem_realloc(void* _ptr, size_t _size);
MEME_EXTERN_C MEME_API void
	MEME_STDCALL mmsmem_free(void* _ptr);

#endif // !MEME_STRING_MEMORY_H_INCLUDED
