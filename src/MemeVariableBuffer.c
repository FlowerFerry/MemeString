
#include "meme/variable_buffer.h"
#include "meme/string.h"
#include "meme/impl/string.h"
#include "meme/impl/string_p__medium.h"
#include "meme/impl/string_p__small.h"

#include <assert.h>

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
	MemeVariableBufferStack_t* _out, size_t _object_size, const MemeByte_t* _buf, MemeInteger_t _len)
{
	return MemeStringStack_initByU8bytes((MemeStringStack_t*)_out, _object_size, _buf, _len);
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
	return MemeString_byteData((MemeString_Const_t)_s);
}

MEME_API MemeByte_t* MEME_STDCALL MemeVariableBuffer_dataWithNotConst(MemeVariableBuffer_t _s)
{
	return (MemeByte_t*)MemeString_byteData((MemeString_Const_t)_s);
}

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_size(MemeVariableBuffer_Const_t _s)
{
	return MemeString_byteSize((MemeString_Const_t)_s);
}

MEME_API MemeInteger_t 
MEME_STDCALL MemeVariableBuffer_maxByteSize(MemeVariableBuffer_Const_t _s)
{
	return MemeString_maxByteSize((MemeString_Const_t)_s);
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
MEME_STDCALL MemeVariableBuffer_availableByteCapacity(MemeVariableBuffer_Const_t _s)
{
	return MemeString_availableByteCapacity((MemeString_t)_s);
}

MEME_API const MemeByte_t* 
MEME_STDCALL MemeVariableBuffer_constBackItem(MemeVariableBuffer_Const_t _s)
{
	const MemeByte_t* p = MemeVariableBuffer_data(_s) + MemeVariableBuffer_size(_s) - 1;
	return p;
}

MEME_API MemeByte_t* 
MEME_STDCALL MemeVariableBuffer_backItem(MemeVariableBuffer_t _s)
{
	const MemeByte_t* p = MemeVariableBuffer_data(_s) + MemeVariableBuffer_size(_s) - 1;
	return (MemeByte_t*)p;
}

MEME_API const MemeByte_t* 
MEME_STDCALL MemeVariableBuffer_constAt(MemeVariableBuffer_Const_t _s, MemeInteger_t _pos)
{
	if (!_s)
		return NULL;
	if (_pos < 0 || MemeVariableBuffer_size(_s) <= _pos)
		return NULL;

	return MemeVariableBuffer_data(_s) + _pos;
}

MEME_API MemeByte_t* 
MEME_STDCALL MemeVariableBuffer_at(MemeVariableBuffer_t _s, MemeInteger_t _pos)
{
	if (!_s)
		return NULL;
	if (_pos < 0 || MemeVariableBuffer_size(_s) <= _pos)
		return NULL;

	return MemeVariableBuffer_dataWithNotConst(_s) + _pos;
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
MEME_STDCALL MemeVariableBuffer_appendWithByte(MemeVariableBuffer_t _s, MemeByte_t _byte)
{
	assert(_s && MemeVariableBuffer_appendWithByte);

	return MemeVariableBuffer_appendWithBytes(_s, &_byte, 1);
}

MEME_API MemeInteger_t 
MEME_STDCALL MemeVariableBuffer_appendWithBytes(
	MemeVariableBuffer_t _s, const MemeByte_t* _buf, MemeInteger_t _len)
{
	MemeString_Const_t s = (MemeString_Const_t)_s;
	int result = 0;

	assert(_s && MemeVariableBuffer_appendWithBytes);
	assert(MemeStringImpl_isModifiableType(MEME_STRING__GET_TYPE(s)) == 1
		&& MemeVariableBuffer_appendWithBytes);
	assert(_buf);

	if (_len < 0)
		_len = strlen((const char*)_buf);

	switch (MEME_STRING__GET_TYPE(s))
	{
	case MemeString_StorageType_small:
	{
		if (0 == MemeStringSmall_canBeAppendIt((const MemeStringSmall_t*)s, _len))
		{
			return MemeStringSmall_appendWithBytes((MemeStringSmall_t*)s, _buf, _len);
		}
		else {
			result = MemeStringImpl_capacityExpansionWithModifiable(
				(MemeStringStack_t*)s, MemeString_byteSize(s) + _len);
			if (result)
				return result;
			return MemeStringMedium_appendWithBytes((MemeStringMedium_t*)s, _buf, _len);
		}
	} break;
	case MemeString_StorageType_medium:
	{
		if (0 != MemeStringMedium_canBeAppendIt((const MemeStringMedium_t*)s, _len))
		{
			result = MemeStringImpl_capacityExpansionWithModifiable(
				(MemeStringStack_t*)s, MemeString_byteSize(s) + _len);
			if (result)
				return result;
		}
		return MemeStringMedium_appendWithBytes((MemeStringMedium_t*)s, _buf, _len);
	} break;
	default: {
		return MEME_ENO__POSIX_OFFSET(ENOTSUP);
	} break;
	}

	return 0;
}

MEME_API MemeInteger_t 
MEME_STDCALL MemeVariableBuffer_appendWithOther(
	MemeVariableBuffer_t _s, MemeVariableBuffer_Const_t _other)
{
	MemeString_Const_t s = (MemeString_Const_t)_s;
	int result = 0;

	assert(_s != NULL && MemeVariableBuffer_appendWithOther);
	assert(_other != NULL && MemeVariableBuffer_appendWithOther);
	assert(MemeStringImpl_isModifiableType(MEME_STRING__GET_TYPE(s)) == 1
		&& MemeVariableBuffer_appendWithOther);

	switch (MEME_STRING__GET_TYPE(s))
	{
	case MemeString_StorageType_small:
	{
		if (0 == MemeStringSmall_canBeAppendIt(
			(const MemeStringSmall_t*)s, MemeVariableBuffer_size(_other)))
		{
			return MemeStringSmall_appendWithBytes(
				(MemeStringSmall_t*)s, MemeVariableBuffer_data(_other), MemeVariableBuffer_size(_other));
		}
		else {
			result = MemeStringImpl_capacityExpansionWithModifiable(
				(MemeStringStack_t*)s, MemeString_byteSize(s) + MemeVariableBuffer_size(_other));
			if (result)
				return result;
			return MemeStringMedium_appendWithBytes(
				(MemeStringMedium_t*)s, MemeVariableBuffer_data(_other), MemeVariableBuffer_size(_other));
		}
	} break;
	case MemeString_StorageType_medium:
	{
		if (0 != MemeStringMedium_canBeAppendIt(
			(const MemeStringMedium_t*)s, MemeVariableBuffer_size(_other)))
		{
			result = MemeStringImpl_capacityExpansionWithModifiable(
				(MemeStringStack_t*)s, MemeString_byteSize(s) + MemeVariableBuffer_size(_other));
			if (result)
				return result;
		}
		return MemeStringMedium_appendWithBytes(
			(MemeStringMedium_t*)s, MemeVariableBuffer_data(_other), MemeVariableBuffer_size(_other));
	} break;
	default: {
		return MEME_ENO__POSIX_OFFSET(ENOTSUP);
	} break;
	}

	return 0;
}

MEME_API MemeInteger_t 
MEME_STDCALL MemeVariableBuffer_clear(MemeVariableBuffer_t _s)
{
	MemeString_t s = (MemeString_t)_s;

	assert(_s != NULL && MemeVariableBuffer_clear);
	assert(MemeStringImpl_isModifiableType(MEME_STRING__GET_TYPE(s)) == 1);

	switch (MEME_STRING__GET_TYPE(s)) {
	case MemeString_StorageType_small:
	{
		MemeStringSmall_clear(&(s->small_));
	} break;
	case MemeString_StorageType_medium:
	{
		MemeStringMedium_clear(&(s->medium_));
	} break;
	default: {
		return MEME_ENO__POSIX_OFFSET(ENOTSUP);
	} break;
	}

	return 0;
}

MEME_API MemeInteger_t 
MEME_STDCALL MemeVariableBuffer_resize(MemeVariableBuffer_t _s, MemeInteger_t _size)
{
	return MemeVariableBuffer_resizeWithByte(_s, _size, 0);
}

MEME_API MemeInteger_t 
MEME_STDCALL MemeVariableBuffer_resizeWithByte(MemeVariableBuffer_t _s, MemeInteger_t _size, MemeByte_t _byte)
{
	MemeString_t s = (MemeString_t)_s;

	assert(_s != NULL && MemeVariableBuffer_resizeWithByte);
	assert(_size >= 0 && MemeVariableBuffer_resizeWithByte);
	assert(MemeStringImpl_isModifiableType(MEME_STRING__GET_TYPE(s)) == 1);

	switch (MEME_STRING__GET_TYPE(s)) {
	case MemeString_StorageType_small:
	{
		if (_size <= MEME_STRING__GET_SMALL_BUFFER_SIZE)
		{
			return MemeStringSmall_resizeWithByte((MemeStringSmall_t*)_s, _size, _byte);
		}
		else {
			int result = MemeStringImpl_capacityExpansionWithModifiable((MemeStringStack_t*)s, _size);
			if (result)
				return result;
			return MemeStringMedium_resizeWithByte((MemeStringMedium_t*)_s, _size, _byte);
		}
	} break;
	case MemeString_StorageType_medium:
	{
		return MemeStringMedium_resizeWithByte((MemeStringMedium_t*)_s, _size, _byte);
	} break;
	default: {
		return MEME_ENO__POSIX_OFFSET(ENOTSUP);
	} break;
	}

	return 0;
}

MEME_API MemeInteger_t 
MEME_STDCALL MemeVariableBuffer_release(
	MemeVariableBuffer_t _s, MemeBufferStack_t* _out, MemeInteger_t _objectSize)
{
	return MEME_ENO__POSIX_OFFSET(ENOTSUP);
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
