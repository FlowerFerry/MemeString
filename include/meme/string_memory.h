
#ifndef MEME_STRING_MEMORY_H_INCLUDED
#define MEME_STRING_MEMORY_H_INCLUDED

#include "meme/common.h"
#include "meme/string_memory_fwd.h"

MEME_EXTERN_C MEME_API int 
	MEME_STDCALL MemeString_setMallocFunction(MemeString_MallocFunction_t* _fn);
MEME_EXTERN_C MEME_API int 
	MEME_STDCALL MemeString_setFreeFunction(MemeString_FreeFunction_t* _fn);

MEME_EXTERN_C MEME_API MemeString_MallocFunction_t* 
	MEME_STDCALL MemeString_getMallocFunction();
MEME_EXTERN_C MEME_API MemeString_FreeFunction_t* 
	MEME_STDCALL MemeString_getFreeFunction();
MEME_EXTERN_C MEME_API MemeString_ReallocFunction_t*
MEME_STDCALL MemeString_getReallocFunction();

#endif // !MEME_STRING_MEMORY_H_INCLUDED
