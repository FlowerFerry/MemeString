
#ifndef MEME_UNSAFE_VIEW_H_INCLUDED
#define MEME_UNSAFE_VIEW_H_INCLUDED

#include "meme/string_fwd.h"

MEME_EXTERN_C_SCOPE_START

MEME_API int
MEME_STDCALL MemeStringViewUnsafeStack_init(MemeStringStack_t* _s, size_t _object_size,
	const uint8_t* _buf, MemeInteger_t _len);

MEME_API int
MEME_STDCALL MemeStringViewUnsafeStack_initByOther(MemeStringStack_t* _s, size_t _object_size,
	const MemeStringStack_t* _other);

MEME_API int
MEME_STDCALL MemeStringViewUnsafeStack_assignByOther(MemeStringStack_t* _s, size_t _object_size,
	const MemeStringStack_t* _other);

MEME_EXTERN_C_SCOPE_ENDED

#endif // !MEME_UNSAFE_VIEW_H_INCLUDED
