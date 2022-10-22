
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

MEME_API MemeInteger_t
MEME_STDCALL MemeStringViewUnsafe_split(
	MemeString_Const_t _s,
	const char* _key, MemeInteger_t _key_len,
	MemeFlag_SplitBehavior_t, MemeFlag_CaseSensitivity_t,
	MemeStringStack_t* _out, MemeInteger_t* _out_count,
	MemeInteger_t* _search_index
);

MEME_EXTERN_C_SCOPE_ENDED

#endif // !MEME_UNSAFE_VIEW_H_INCLUDED
