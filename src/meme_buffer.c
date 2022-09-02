
#include "meme/buffer.h"
#include "meme/string.h"
#include <errno.h>

MEME_API int 
MEME_STDCALL MemeBufferStack_init(MemeBufferStack_t* _out, size_t _object_size)
{
	return MemeStringStack_init((MemeStringStack_t*)_out, _object_size);
}

MEME_API int 
MEME_STDCALL MemeBufferStack_initByOther(
	MemeBufferStack_t* _out, size_t _object_size, const MemeBufferStack_t* _other)
{
	return MemeStringStack_initByOther((MemeStringStack_t*)_out, _object_size, (MemeString_Const_t)_other);
}

MEME_API int 
MEME_STDCALL MemeBufferStack_initByBytes(
	MemeBufferStack_t* _out, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len)
{
	return MemeStringStack_initByU8bytesAndType(
		(MemeStringStack_t*)_out, _object_size, _utf8, _len, MemeString_StorageType_large);
}

MEME_API int 
MEME_STDCALL MemeBufferStack_initByU8bytesAndType(
	MemeBufferStack_t* _out, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len, MemeBuffer_Storage_t _suggest)
{
	//return MemeStringStack_initByU8bytesAndType(
	//	(MemeStringStack_t*)_out, _object_size, _utf8, _len, _suggest);

	return MEME_ENO__POSIX_OFFSET(ENOTSUP);
}

//MEME_API int 
//MEME_STDCALL MemeBufferStack_initByOtherAndType(
//	MemeBufferStack_t* _out, size_t _object_size, const MemeBufferStack_t* _other, MemeBuffer_Storage_t _suggest)
//{
//	return MemeStringStack_initByOtherAndType(
//		(MemeStringStack_t*)_out, _object_size, (MemeString_Const_t)_other, _suggest);
//}

MEME_API int 
MEME_STDCALL MemeBufferStack_unInit(MemeBufferStack_t* _out, size_t _object_size)
{
	return MemeStringStack_unInit((MemeStringStack_t*)_out, _object_size);
}

MEME_API int 
MEME_STDCALL MemeBufferStack_reset(
	MemeBufferStack_t* _out, size_t _object_size)
{
	return MemeStringStack_reset((MemeStringStack_t*)_out, _object_size);
}

MEME_API int 
MEME_STDCALL MemeBufferStack_assign(
	MemeBufferStack_t* _s, size_t _object_size, const MemeBufferStack_t* _other)
{
	return MemeStringStack_assign_v02((MemeStringStack_t*)_s, _object_size, (MemeString_Const_t)_other);
}

MEME_API MemeBuffer_Storage_t 
MEME_STDCALL MemeBuffer_storageType(MemeBuffer_Const_t _s)
{
	return MemeString_storageType((MemeString_Const_t)_s);
}

MEME_API int 
MEME_STDCALL MemeBuffer_swap(MemeBuffer_t _lhs, MemeBuffer_t _rhs)
{
	return MemeString_swap((MemeString_t)_lhs, (MemeString_t)_rhs);
}

MEME_API int 
MEME_STDCALL MemeBuffer_isEmpty(MemeBuffer_Const_t _s)
{
	return MemeString_isEmpty((MemeString_Const_t)_s);
}

MEME_API int MEME_STDCALL MemeBuffer_isEmpty_v02(MemeBuffer_Const_t _s)
{
	return MemeString_isEmpty_v02((MemeString_Const_t)_s);
}

MEME_API const MemeByte_t*
MEME_STDCALL MemeBuffer_data(MemeBuffer_Const_t _s)
{
	return MemeString_cStr((MemeString_Const_t)_s);
}

MEME_API MemeInteger_t 
MEME_STDCALL MemeBuffer_size(MemeBuffer_Const_t _s)
{
	return MemeString_byteSize((MemeString_Const_t)_s);
}

MEME_API int 
MEME_STDCALL MemeBuffer_isEqual(
	MemeBuffer_Const_t _s, const MemeByte_t* _buf, MemeInteger_t _len, int* _result)
{
	return MemeString_isEqual((MemeString_Const_t)_s, _buf, _len, _result);
}

MEME_API int 
MEME_STDCALL MemeBuffer_isEqualWithOther(
	MemeBuffer_Const_t _lhs, MemeBuffer_Const_t _rhs, int* _result)
{
	return MemeString_isEqualWithOther(
		(MemeString_Const_t)_lhs, (MemeString_Const_t)_rhs, _result);
}

MEME_API MemeInteger_t 
MEME_STDCALL MemeBuffer_indexOfWithBytes(
	MemeBuffer_Const_t _s, MemeInteger_t _offset, const MemeByte_t* _needle, MemeInteger_t _needle_len)
{
	return MemeString_indexOfWithUtf8bytes(
		(MemeString_Const_t)_s, _offset, _needle, _needle_len, MemeFlag_CaseSensitive);
}

MEME_API MemeInteger_t 
MEME_STDCALL MemeBuffer_indexOfWithOther(
	MemeBuffer_Const_t _s, MemeInteger_t _offset, MemeBuffer_Const_t _other)
{
	return MemeString_indexOfWithOther(
		(MemeString_Const_t)_s, _offset, (MemeString_Const_t)_other, MemeFlag_CaseSensitive);
}

MEME_API MemeInteger_t 
MEME_STDCALL MemeBuffer_split(
	MemeBuffer_Const_t _s, const MemeByte_t* _key, MemeInteger_t _key_len, 
	MemeFlag_SplitBehavior_t _sb, MemeBufferStack_t* _out, MemeInteger_t* _out_count, 
	MemeInteger_t* _search_index
)
{
	return MemeString_split(
		(MemeString_Const_t)_s, _key, _key_len, _sb, MemeFlag_CaseSensitive,
		(MemeStringStack_t*)_out, _out_count, _search_index);
}
