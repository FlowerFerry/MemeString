
#ifndef MEME_STRING_COMMON_H_INCLUDED
#define MEME_STRING_COMMON_H_INCLUDED

#include "mego/predef/os/windows.h"
#include "mego/predef/keyword/export.h"
#include "mego/predef/keyword/import.h"

#ifndef MEME_API
#	if defined(MEME_OPTION__BUILD_SHARED)
#		define MEME_API MEGO_SYMBOL__EXPORT
#	elif !defined(MEME_OPTION__BUILD_STATIC)
#		define MEME_API MEGO_SYMBOL__IMPORT
#	else
#		define MEME_API
#	endif
#endif 


#ifndef MEME_EXTERN_C
#	ifdef __cplusplus
#		define MEME_EXTERN_C extern "C"
#	else
#		define MEME_EXTERN_C
#	endif
#endif 

#ifndef MEME_EXTERN_C_SCOPE_START
#	ifdef __cplusplus
#		define MEME_EXTERN_C_SCOPE_START extern "C" {
#	else
#		define MEME_EXTERN_C_SCOPE_START
#	endif
#endif

#ifndef MEME_EXTERN_C_SCOPE_ENDED
#	ifdef __cplusplus
#		define MEME_EXTERN_C_SCOPE_ENDED } // extern "C"
#	else
#		define MEME_EXTERN_C_SCOPE_ENDED
#	endif
#endif


#ifndef MEME_STDCALL
#	if MEGO_OS__WINDOWS__AVAILABLE
#		define MEME_STDCALL __stdcall
#	else
#		define MEME_STDCALL
#	endif
#endif

#endif // !MEME_STRING_COMMON_H_INCLUDED
