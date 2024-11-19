
#ifndef MEME_UNSAFE_VIEW_H_INCLUDED
#define MEME_UNSAFE_VIEW_H_INCLUDED

#include <meme/string_fwd.h>
#include <mego/predef/symbol/restrict.h>

MEME_EXTERN_C_SCOPE_START

MEME_API int
MEME_STDCALL MemeStringViewUnsafeStack_init(MemeStringStack_t* _s, size_t _object_size,
	const uint8_t* _buf, MemeInteger_t _len);

MEME_API int
MEME_STDCALL MemeStringViewUnsafeStack_initByOther(MemeStringStack_t* _s, size_t _object_size,
	const MemeStringStack_t* _other);

//! @param _s     must be initialized
//! @param _other must be initialized
MEME_API int
MEME_STDCALL MemeStringViewUnsafeStack_assignByOther(MemeStringStack_t* _s, size_t _object_size,
	const MemeStringStack_t* _other);

MEME_API mmint_t
MEME_STDCALL MemeStringViewUnsafeStack_split(
	const mmstrstk_t* _str,
	const char* _key, mmint_t _key_len,
	mmflag_split_behav_t, mmflag_case_sensit_t,
	mmstrstk_t* MEGO_SYMBOL__RESTRICT _out, mmint_t _obj_size, 
	mmint_t* MEGO_SYMBOL__RESTRICT _out_count,
	mmint_t* MEGO_SYMBOL__RESTRICT _search_index
);

//! @param _out do not initialize
//! @deprecated 将来可能会有ABI问题
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
