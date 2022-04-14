
#ifndef MEME_IMPL_STRING_MEMORY_H_INCLUDED
#define MEME_IMPL_STRING_MEMORY_H_INCLUDED

#include "meme/string_memory.h"

int MemeStringMemory_allocFunctionSwitch(
	MemeString_MallocFunction_t* _in_malloc_fn,
	MemeString_FreeFunction_t* _in_free_fn,
	MemeString_MallocFunction_t** _out_malloc_fn,
	MemeString_FreeFunction_t** _out_free_fn);

#endif // !MEME_IMPL_STRING_MEMORY_H_INCLUDED
