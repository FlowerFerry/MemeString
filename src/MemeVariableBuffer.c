
#include "meme/variable_buffer.h"
#include "meme/string.h"

MEME_API int 
MEME_STDCALL MemeVariableBufferStack_init(MemeVariableBufferStack_t* _out, size_t _object_size)
{
	return MemeStringStack_init((MemeStringStack_t*)_out, _object_size);
}

MEME_API int 
MEME_STDCALL MemeVariableBufferStack_initByOther(
	MemeVariableBufferStack_t* _out, size_t _object_size, const MemeVariableBufferStack_t* _other)
{
	return MemeStringStack_initByOther((MemeStringStack_t*)_out, _object_size, (MemeString_Const_t)_other);
}

MEME_API int 
MEME_STDCALL MemeVariableBufferStack_initByBytes(
	MemeVariableBufferStack_t* _out, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len)
{
	return MemeStringStack_initByU8bytes((MemeStringStack_t*)_out, _object_size, _utf8, _len);
}

MEME_API int 
MEME_STDCALL MemeVariableBufferStack_unInit(MemeVariableBufferStack_t* _out, size_t _object_size)
{
	return MemeStringStack_unInit((MemeStringStack_t*)_out, _object_size);
}

MEME_API int 
MEME_STDCALL MemeVariableBufferStack_reset(MemeVariableBufferStack_t* _out, size_t _object_size)
{
	return MemeStringStack_reset((MemeStringStack_t*)_out, _object_size);
}

MEME_API int 
MEME_STDCALL MemeVariableBufferStack_assign(
	MemeVariableBufferStack_t* _s, size_t _object_size, const MemeVariableBufferStack_t* _other)
{
	return MemeStringStack_assign_v02((MemeStringStack_t*)_s, _object_size, (MemeString_Const_t)_other);
}


MEME_API MemeVariableBuffer_Storage_t
MEME_STDCALL MemeVariableBuffer_storageType(MemeVariableBuffer_Const_t _s)
{
	return MemeString_storageType((MemeString_Const_t)_s);
}

MEME_API int
MEME_STDCALL MemeVariableBuffer_swap(MemeVariableBuffer_t _lhs, MemeVariableBuffer_t _rhs)
{
	return MemeString_swap((MemeString_t)_lhs, (MemeString_t)_rhs);
}

MEME_API int
MEME_STDCALL MemeVariableBuffer_isEmpty(MemeVariableBuffer_Const_t _s)
{
	return MemeString_isEmpty((MemeString_Const_t)_s);
}

MEME_API const MemeByte_t*
MEME_STDCALL MemeVariableBuffer_data(MemeVariableBuffer_Const_t _s)
{
	return MemeString_cStr((MemeString_Const_t)_s);
}

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_size(MemeVariableBuffer_Const_t _s)
{
	return MemeString_byteSize((MemeString_Const_t)_s);
}

MEME_API int
MEME_STDCALL MemeVariableBuffer_isEqual(
	MemeVariableBuffer_Const_t _s, const MemeByte_t* _buf, MemeInteger_t _len, int* _result)
{
	return MemeString_isEqual((MemeString_Const_t)_s, _buf, _len, _result);
}

MEME_API int
MEME_STDCALL MemeVariableBuffer_isEqualWithOther(
	MemeVariableBuffer_Const_t _lhs, MemeVariableBuffer_Const_t _rhs, int* _result)
{
	return MemeString_isEqualWithOther(
		(MemeString_Const_t)_lhs, (MemeString_Const_t)_rhs, _result);
}

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_indexOfWithBytes(
	MemeVariableBuffer_Const_t _s, MemeInteger_t _offset, const MemeByte_t* _needle, MemeInteger_t _needle_len)
{
	return MemeString_indexOfWithUtf8bytes(
		(MemeString_Const_t)_s, _offset, _needle, _needle_len, MemeFlag_CaseSensitive);
}

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_indexOfWithOther(
	MemeVariableBuffer_Const_t _s, MemeInteger_t _offset, MemeVariableBuffer_Const_t _other)
{
	return MemeString_indexOfWithOther(
		(MemeString_Const_t)_s, _offset, (MemeString_Const_t)_other, MemeFlag_CaseSensitive);
}

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_split(
	MemeVariableBuffer_Const_t _s, const MemeByte_t* _key, MemeInteger_t _key_len,
	MemeFlag_SplitBehavior_t _sb, MemeVariableBufferStack_t* _out, MemeInteger_t* _out_count,
	MemeInteger_t* _search_index
)
{
	return MemeString_split(
		(MemeString_Const_t)_s, _key, _key_len, _sb, MemeFlag_CaseSensitive,
		(MemeStringStack_t*)_out, _out_count, _search_index);
}
