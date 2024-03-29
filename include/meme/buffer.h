
#ifndef MEME_BUFFER_H_INCLUDED
#define MEME_BUFFER_H_INCLUDED

#include "buffer_fwd.h"
#include "mego/predef/symbol/deprecated.h"

MEME_EXTERN_C_SCOPE_START

MEME_API int
MEME_STDCALL MemeBufferStack_init(mmbufstk_t* _out, size_t _object_size);

//MEME_API int
//MEME_STDCALL MemeBufferStack_initTakeOverUserObject(
//	MemeBufferStack_t* _out, size_t _object_size,
//	void* _user_data,
//	MemeBuffer_UserObjectDestruct_t* _destruct_fn,
//	MemeBuffer_UserObjectData_t* _data_fn,
//	MemeBuffer_UserObjectSize_t* _size_fn);

MEME_API int
MEME_STDCALL MemeBufferStack_initByOther(
	mmbufstk_t* _out, size_t _object_size, const mmbufstk_t* _other);

MEME_API int
MEME_STDCALL MemeBufferStack_initByString(
	mmbufstk_t* _out, size_t _object_size, mms_const_t _s);

MEME_API int
MEME_STDCALL MemeBufferStack_initByBytes(
	mmbufstk_t* _out, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len);

MEME_API int
MEME_STDCALL MemeBufferStack_initByU8bytesAndType(
	mmbufstk_t* _out, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len,
		MemeBuffer_Storage_t _suggest);

//MEME_API int
//MEME_STDCALL MemeBufferStack_initByOtherAndType(
//	MemeBufferStack_t* _out, size_t _object_size,
//	const MemeBufferStack_t* _other, MemeBuffer_Storage_t _suggest
//);

MEME_API int
MEME_STDCALL MemeBufferStack_unInit(mmbufstk_t* _out, size_t _object_size);

MEME_API int
MEME_STDCALL MemeBufferStack_reset(mmbufstk_t* _out, size_t _object_size);

MEME_API int
MEME_STDCALL MemeBufferStack_assign(
	mmbufstk_t* _s, size_t _object_size, const MemeBufferStack_t* _other);

MEME_API MemeBuffer_Storage_t
MEME_STDCALL MemeBuffer_storageType(MemeBuffer_Const_t _s);

MEME_API int
MEME_STDCALL MemeBuffer_swap(MemeBuffer_t _lhs, MemeBuffer_t _rhs);

MEME_API int
MEME_STDCALL MemeBuffer_isNonempty(MemeBuffer_Const_t _s);

MEME_API int
MEME_STDCALL MemeBuffer_isEmpty(MemeBuffer_Const_t _s);

MEME_API const mmbyte_t*
MEME_STDCALL MemeBuffer_at(mmbuf_const_t _s, mmint_t _pos);

MEME_API const MemeByte_t*
MEME_STDCALL MemeBuffer_data(MemeBuffer_Const_t _s);

MEME_API MemeInteger_t
MEME_STDCALL MemeBuffer_size(MemeBuffer_Const_t _s);

MEME_API int
MEME_STDCALL MemeBuffer_isEqual(MemeBuffer_Const_t _s, const MemeByte_t* _buf, MemeInteger_t _len, int* _result);

MEME_API int
MEME_STDCALL MemeBuffer_isEqualWithOther(MemeBuffer_Const_t _lhs, MemeBuffer_Const_t _rhs, int* _result);

MEME_API mmint_t
MEME_STDCALL MemeBuffer_isSharedStorageTypes(mmbuf_const_t _b);

MEME_API MemeInteger_t
MEME_STDCALL MemeBuffer_indexOfWithBytes(
	MemeBuffer_Const_t _s, MemeInteger_t _offset,
	const MemeByte_t* _needle, MemeInteger_t _needle_len);

MEME_API MemeInteger_t
MEME_STDCALL MemeBuffer_indexOfWithOther(
	MemeBuffer_Const_t _s, MemeInteger_t _offset, MemeBuffer_Const_t _other);

MEME_API MemeInteger_t
MEME_STDCALL MemeBuffer_startsMatchWithBytes(
	MemeBuffer_Const_t _s,
	const MemeByte_t* _needle, MemeInteger_t _needle_len);

MEME_API MemeInteger_t
MEME_STDCALL MemeBuffer_startsMatchWithOther(
	MemeBuffer_Const_t _s, MemeBuffer_Const_t _other);

MEME_API MemeInteger_t
MEME_STDCALL MemeBuffer_endsMatchWithBytes(
	MemeBuffer_Const_t _s,
	const MemeByte_t* _needle, MemeInteger_t _needle_len);

MEME_API MemeInteger_t
MEME_STDCALL MemeBuffer_endsMatchWithOther(
	MemeBuffer_Const_t _s, MemeBuffer_Const_t _other);

MEME_API MemeInteger_t
MEME_STDCALL MemeBuffer_split(
	MemeBuffer_Const_t _s,
	const MemeByte_t* _key, MemeInteger_t _key_len,
	MemeFlag_SplitBehavior_t,
	MemeBufferStack_t* _out, MemeInteger_t* _out_count,
	MemeInteger_t* _search_index
);

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_BUFFER_H_INCLUDED
