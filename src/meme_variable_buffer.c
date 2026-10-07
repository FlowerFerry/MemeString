
#include "meme/variable_buffer.h"
#include "meme/string.h"
#include "meme/buffer.h"
#include "meme/impl/string.h"
#include "meme/impl/string_p__medium.h"
#include "meme/impl/string_p__small.h"
#include "meme/impl/string_p__large.h"

#include <assert.h>

MEME_EXTERN_C MEME_API int
MEME_STDCALL MemeVariableBufferStack_init(mmvbstk_t* _out, size_t _object_size)
{
	return MemeStringStack_init((MemeStringStack_t*)_out, _object_size);
}

MEME_EXTERN_C MEME_API int
MEME_STDCALL MemeVariableBufferStack_initByOther(
	mmvbstk_t* _out, size_t _object_size, const mmvbstk_t* _other)
{

	assert(_out);
	assert(_other);
	assert(_object_size != 0 && _object_size <= MMSTR__MAX_REG_BYTE_SIZE);

	switch (MMSTR__GET_IMPLTYPE((mmstr_cptr_t)_other)) 
	{
	case MemeString_ImplType_small: {
		memcpy(_out, _other, MEME_STRING__OBJECT_SIZE);
	} break;
	case MemeString_ImplType_user:
	case MemeString_ImplType_large:
	case MemeString_ImplType_medium: {
		MemeStringStack_init((MemeStringStack_t*)_out, MEME_STRING__OBJECT_SIZE);
		return (int)MemeVariableBuffer_appendWithBytes((MemeVariableBuffer_t)_out,
			MemeString_byteData((MemeString_t)_other), MemeString_byteSize((MemeString_t)_other));
	};
	default: {
		return (MGEC__OPNOTSUPP);
	};
	}

	return 0;
}

MEME_EXTERN_C MEME_API int
MEME_STDCALL MemeVariableBufferStack_initByBytes(
	mmvbstk_t* _out, size_t _object_size, const MemeByte_t* _buf, MemeInteger_t _len)
{
	assert(_len >= 0 && "MemeVariableBufferStack_initByBytes");
	assert(_out && "MemeVariableBufferStack_initByBytes");
	assert(_object_size != 0 && _object_size <= MMSTR__MAX_REG_BYTE_SIZE && "MemeVariableBufferStack_initByBytes");

	MemeStringStack_init((MemeStringStack_t*)_out, MEME_STRING__OBJECT_SIZE);
	return (int)MemeVariableBuffer_appendWithBytes((MemeVariableBuffer_t)_out, _buf, _len);
}

MEME_EXTERN_C MEME_API int
MEME_STDCALL MemeVariableBufferStack_initWithRepeatBytes(
	mmvbstk_t* _out, size_t _object_size, MemeInteger_t _count, MemeByte_t _byte)
{
	assert(_count >= 0 && "MemeVariableBufferStack_initWithRepeatBytes");
	assert(_out && "MemeVariableBufferStack_initWithRepeatBytes");
	assert(_object_size != 0 && _object_size <= MMSTR__MAX_REG_BYTE_SIZE && "MemeVariableBufferStack_initWithRepeatBytes");

	MemeStringStack_init((MemeStringStack_t*)_out, MEME_STRING__OBJECT_SIZE);
	return (int)MemeVariableBuffer_appendWithRepeatBytes((MemeVariableBuffer_t)_out, _count, _byte);
}

MEME_EXTERN_C MEME_API int
MEME_STDCALL MemeVariableBufferStack_unInit(mmvbstk_t* _out, size_t _object_size)
{
	return MemeStringStack_unInit((MemeStringStack_t*)_out, _object_size);
}

MEME_EXTERN_C MEME_API int
MEME_STDCALL MemeVariableBufferStack_reset(MemeVariableBufferStack_t* _out, size_t _object_size)
{
	return MemeStringStack_reset((MemeStringStack_t*)_out, _object_size);
}

MEME_EXTERN_C MEME_API int
MEME_STDCALL MemeVariableBufferStack_assign(
	mmvbstk_t* _s, size_t _object_size, const mmvbstk_t* _other)
{
	return MemeStringStack_assign((MemeStringStack_t*)_s, _object_size, (MemeString_Const_t)_other);
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeVariableBufferStack_regSize(const mmvbstk_t* _obj)
{
	return MemeStringStack_regSize((const mmstrstk_t*)_obj);
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeVariableBufferStack_objSize(const mmvbstk_t* _obj)
{
    return MemeStringStack_objSize((const mmstrstk_t*)_obj);
}

MEME_EXTERN_C MEME_API mmint_t
MEME_STDCALL MemeVariableBufferStack_split(
	const mmvbstk_t* _buf, const mmbyte_t* _key, mmint_t _key_len, 
	mmflag_split_behav_t _sb,
	mmvbstk_t* MEGO_SYMBOL__RESTRICT _out, mmint_t _obj_size, 
	mmint_t* MEGO_SYMBOL__RESTRICT _out_count, 
	mmint_t* MEGO_SYMBOL__RESTRICT _search_index)
{
	return MemeStringStack_split(
		(const mmstrstk_t*)_buf, (const char*)_key, _key_len, _sb, MemeFlag_AllSensitive,
		(mmstrstk_t*)_out, _obj_size, _out_count, _search_index);
}

MEME_EXTERN_C MEME_API MemeVariableBuffer_Storage_t
MEME_STDCALL MemeVariableBuffer_storageType(MemeVariableBuffer_Const_t _s)
{
	return MemeString_storageType((MemeString_Const_t)_s);
}

MEME_EXTERN_C MEME_API int
MEME_STDCALL MemeVariableBuffer_swap(MemeVariableBuffer_t _lhs, MemeVariableBuffer_t _rhs)
{
	return MemeString_swap((MemeString_t)_lhs, (MemeString_t)_rhs);
}

MEME_EXTERN_C MEME_API int
MEME_STDCALL MemeVariableBuffer_isNonempty(MemeVariableBuffer_Const_t _s)
{
	return MemeString_isNonempty((MemeString_Const_t)_s);
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeVariableBuffer_isEmpty(MemeVariableBuffer_Const_t _s)
{
	return MemeString_isEmpty((MemeString_Const_t)_s);
}

MEME_EXTERN_C MEME_API const MemeByte_t*
MEME_STDCALL MemeVariableBuffer_data(MemeVariableBuffer_Const_t _s)
{
	return MemeString_byteData((MemeString_Const_t)_s);
}

MEME_EXTERN_C MEME_API MemeByte_t* MEME_STDCALL MemeVariableBuffer_dataWithNotConst(MemeVariableBuffer_t _s)
{
	return (MemeByte_t*)MemeString_byteData((MemeString_Const_t)_s);
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_size(MemeVariableBuffer_Const_t _s)
{
	return MemeString_byteSize((MemeString_Const_t)_s);
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_maxByteSize(MemeVariableBuffer_Const_t _s)
{
	return MemeString_maxByteSize((MemeString_Const_t)_s);
}

MEME_EXTERN_C MEME_API int
MEME_STDCALL MemeVariableBuffer_isEqual(
	MemeVariableBuffer_Const_t _s, const MemeByte_t* _buf, MemeInteger_t _len, int* _result)
{
	return MemeString_isEqual((MemeString_Const_t)_s, (const char*)_buf, _len, _result);
}

MEME_EXTERN_C MEME_API int
MEME_STDCALL MemeVariableBuffer_isEqualWithOther(
	MemeVariableBuffer_Const_t _lhs, MemeVariableBuffer_Const_t _rhs, int* _result)
{
	return MemeString_isEqualWithOther(
		(MemeString_Const_t)_lhs, (MemeString_Const_t)_rhs, _result);
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeVariableBuffer_compare(
	mmvb_cptr_t _lhs, mmvb_cptr_t _rhs)
{
    return MemeString_compare((mmstr_cptr_t)_lhs, (mmstr_cptr_t)_rhs);
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeVariableBuffer_compareWithBytes(
	mmvb_cptr_t _s, const mmbyte_t* _buf, mmint_t _len)
{
    return MemeString_compareByUtf8bytes((mmstr_cptr_t)_s, _buf, _len);
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_availableByteCapacity(MemeVariableBuffer_Const_t _s)
{
	return MemeString_availableByteCapacity((MemeString_t)_s);
}

MEME_EXTERN_C MEME_API const MemeByte_t*
MEME_STDCALL MemeVariableBuffer_constBackItem(MemeVariableBuffer_Const_t _s)
{
	const MemeByte_t* p = MemeVariableBuffer_data(_s) + MemeVariableBuffer_size(_s) - 1;
	return p;
}

MEME_EXTERN_C MEME_API MemeByte_t*
MEME_STDCALL MemeVariableBuffer_backItem(MemeVariableBuffer_t _s)
{
	const MemeByte_t* p = MemeVariableBuffer_data(_s) + MemeVariableBuffer_size(_s) - 1;
	return (MemeByte_t*)p;
}

MEME_EXTERN_C MEME_API const MemeByte_t*
MEME_STDCALL MemeVariableBuffer_constAt(MemeVariableBuffer_Const_t _s, MemeInteger_t _pos)
{
	if (!_s)
		return NULL;
	if (_pos < 0 || MemeVariableBuffer_size(_s) <= _pos)
		return NULL;

	return MemeVariableBuffer_data(_s) + _pos;
}

MEME_EXTERN_C MEME_API MemeByte_t*
MEME_STDCALL MemeVariableBuffer_at(MemeVariableBuffer_t _s, MemeInteger_t _pos)
{
	if (!_s)
		return NULL;
	if (_pos < 0 || MemeVariableBuffer_size(_s) <= _pos)
		return NULL;

	return MemeVariableBuffer_dataWithNotConst(_s) + _pos;
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_indexOfWithBytes(
	MemeVariableBuffer_Const_t _s, MemeInteger_t _offset, const MemeByte_t* _needle, MemeInteger_t _needle_len)
{
	return MemeString_indexOfWithUtf8bytes(
		(MemeString_Const_t)_s, _offset, _needle, _needle_len, MemeFlag_AllSensitive);
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_indexOfWithOther(
	MemeVariableBuffer_Const_t _s, MemeInteger_t _offset, MemeVariableBuffer_Const_t _other)
{
	return MemeString_indexOfWithOther(
		(MemeString_Const_t)_s, _offset, (MemeString_Const_t)_other, MemeFlag_AllSensitive);
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_indexOfWithByte(
	MemeVariableBuffer_Const_t _s, MemeInteger_t _offset, MemeByte_t _byte)
{
    return MemeString_indexOfWithByte(
        (MemeString_Const_t)_s, _offset, _byte, MemeFlag_AllSensitive);
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_lastIndexOfWithBytes(
	MemeVariableBuffer_Const_t _s, MemeInteger_t _limit,
	const MemeByte_t* _needle, MemeInteger_t _needle_len)
{
    return MemeString_lastIndexOfWithUtf8bytes(
        (MemeString_Const_t)_s, _limit, _needle, _needle_len, MemeFlag_AllSensitive);
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_lastIndexOfWithOther(
	MemeVariableBuffer_Const_t _s, MemeInteger_t _limit,
	MemeVariableBuffer_Const_t _other)
{
    return MemeString_lastIndexOfWithUtf8bytes(
        (MemeString_Const_t)_s, _limit,
        MemeVariableBuffer_data(_other), MemeVariableBuffer_size(_other),
        MemeFlag_AllSensitive);
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_lastIndexOfWithByte(
	MemeVariableBuffer_Const_t _s, MemeInteger_t _limit, MemeByte_t _byte)
{
    return MemeString_lastIndexOfWithByte(
        (MemeString_Const_t)_s, _limit, _byte, MemeFlag_AllSensitive);
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_countWithBytes(
	MemeVariableBuffer_Const_t _s,
	const MemeByte_t* _needle, MemeInteger_t _needle_len)
{
	const MemeInteger_t total = MemeVariableBuffer_size(_s);
	MemeInteger_t match_count = 0;
	MemeInteger_t pos = 0;

	// Empty needle can never match; needle longer than buffer cannot match either.
	if (_needle_len <= 0 || _needle_len > total)
		return 0;

	// Non-overlapping: after a hit, resume search strictly past the matched region.
	while (pos <= total - _needle_len)
	{
		const MemeInteger_t idx = MemeVariableBuffer_indexOfWithBytes(
			_s, pos, _needle, _needle_len);
		if (idx < 0)
			break;
		++match_count;
		pos = idx + _needle_len;
	}
	return match_count;
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_countWithByte(
	MemeVariableBuffer_Const_t _s, MemeByte_t _byte)
{
	const MemeInteger_t total = MemeVariableBuffer_size(_s);
	MemeInteger_t match_count = 0;
	MemeInteger_t pos = 0;

	while (pos < total)
	{
		const MemeInteger_t idx = MemeVariableBuffer_indexOfWithByte(_s, pos, _byte);
		if (idx < 0)
			break;
		++match_count;
		pos = idx + 1;
	}
	return match_count;
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_appendWithByte(MemeVariableBuffer_t _s, MemeByte_t _byte)
{
	assert(_s && "MemeVariableBuffer_appendWithByte");

	return MemeVariableBuffer_appendWithBytes(_s, &_byte, 1);
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_startsMatchWithBytes(
	MemeVariableBuffer_Const_t _s, const MemeByte_t* _needle, MemeInteger_t _needle_len)
{
    return MemeString_startsMatchWithUtf8bytes(
        (MemeString_Const_t)_s, _needle, _needle_len, MemeFlag_AllSensitive);
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_endsMatchWithBytes(
	MemeVariableBuffer_Const_t _s, const MemeByte_t* _needle, MemeInteger_t _needle_len)
{
    return MemeString_endsMatchWithUtf8bytes(
        (MemeString_Const_t)_s, _needle, _needle_len, MemeFlag_AllSensitive);
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_appendWithBytes(
	MemeVariableBuffer_t _s, const MemeByte_t* _buf, MemeInteger_t _len)
{
	MemeString_Const_t s = (MemeString_Const_t)_s;

	assert(s && "MemeVariableBuffer_appendWithBytes");
	assert(MemeStringImpl_isModifiableType(MMSTR__GET_IMPLTYPE(s)) == 1
		&& "MemeVariableBuffer_appendWithBytes");

	if (MG_SYM__UNLIKELY(_buf == NULL))
		_len = 0;
	else if (_len < 0)
		_len = strlen((const char*)_buf);

	if (MemeVariableBuffer_data(_s) == _buf) 
	{
		mmint_t result = 0;
		mmvbstk_t vb;
		MemeVariableBufferStack_init(&vb, MMSTR__OBJ_SIZE);
		result = MemeVariableBuffer_reserve((mmvb_ptr_t)&vb,
			MemeVariableBuffer_size(_s) + _len);
		if (result)
			return result;
		result = MemeVariableBuffer_appendWithBytes(
			(mmvb_ptr_t)&vb, MemeVariableBuffer_data(_s), MemeVariableBuffer_size(_s));
		if (result) {
			MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
			return result;
		}

		result = MemeVariableBuffer_appendWithBytes((mmvb_ptr_t)&vb, _buf, _len);
		if (result) {
			MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
			return result;
		}

		MemeVariableBuffer_swap((mmvb_ptr_t)&vb, _s);
		MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
		return 0;
	}
	
	switch (MMSTR__GET_IMPLTYPE(s))
	{
	case MemeString_ImplType_small:
	{
		//int result = 0;
		if (0 == MemeStringSmall_canBeAppendIt((const MemeStringSmall_t*)s, _len))
		{
			return MemeStringSmall_appendWithBytes((MemeStringSmall_t*)s, _buf, _len);
		}
		else {
			int result = MemeStringImpl_capacityExpansionSmallToMedium(
				(MemeStringStack_t*)s, MemeString_byteSize(s) + _len);
			if (result)
				return result;
			return MemeStringMedium_appendWithBytes((MemeStringMedium_t*)s, _buf, _len);
		}
	} break;
	case MemeString_ImplType_medium:
	{
		return MemeStringMedium_appendWithBytes((MemeStringMedium_t*)s, _buf, _len);
	};
	default: {
		return (MGEC__OPNOTSUPP);
	} break;
	}

	//return 0;
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_appendWithRepeatBytes(
	MemeVariableBuffer_t _s, MemeInteger_t _count, MemeByte_t _byte)
{
	MemeString_Const_t s = (MemeString_Const_t)_s;

	assert(s && "MemeVariableBuffer_appendWithRepeatBytes");
	assert(MemeStringImpl_isModifiableType(MMSTR__GET_IMPLTYPE(s)) == 1
		&& "MemeVariableBuffer_appendWithRepeatBytes");
	assert(_count >= 0 && "MemeVariableBuffer_appendWithRepeatBytes");

	switch (MMSTR__GET_IMPLTYPE(s))
	{
	case MemeString_ImplType_small:
	{
		//int result = 0;
		if (0 == MemeStringSmall_canBeAppendIt((const MemeStringSmall_t*)s, _count))
		{
			return MemeStringSmall_appendWithByte((MemeStringSmall_t*)s, _count, _byte);
		}
		else {
			int result = MemeStringImpl_capacityExpansionSmallToMedium(
				(MemeStringStack_t*)s, MemeString_byteSize(s) + _count);
			if (result)
				return result;
			return MemeStringMedium_appendWithByte((MemeStringMedium_t*)s, _count, _byte);
		}
	} break;
	case MemeString_ImplType_medium:
	{
		return MemeStringMedium_appendWithByte((MemeStringMedium_t*)s, _count, _byte);
	};
	default: {
		return (MGEC__OPNOTSUPP);
	} break;
	}

	//return 0;
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_appendWithOther(
	MemeVariableBuffer_t _s, MemeVariableBuffer_Const_t _other)
{
	mmstr_ptr_t  str   = (mmstr_ptr_t)_s;
    mmstr_cptr_t other = (mmstr_cptr_t)_other;

	assert(str != NULL && "MemeVariableBuffer_appendWithOther");
	assert(_other != NULL && "MemeVariableBuffer_appendWithOther");
	assert(MemeStringImpl_isModifiableType(MMSTR__GET_IMPLTYPE(str)) == 1
		&& "MemeVariableBuffer_appendWithOther");

	if (str == other) {
		mmint_t result = 0;
		mmvbstk_t vb;
		MemeVariableBufferStack_init(&vb, MMSTR__OBJ_SIZE);
		result = MemeVariableBuffer_reserve((mmvb_ptr_t)&vb, 
			MemeVariableBuffer_size(_s) + MemeVariableBuffer_size(_other));
        if (result)
            return result;
        result = MemeVariableBuffer_appendWithBytes(
			(mmvb_ptr_t)&vb, MemeVariableBuffer_data(_s), MemeVariableBuffer_size(_s));
        if (result) {
            MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
            return result;
        }
		
		result = MemeVariableBuffer_appendWithBytes(
			(mmvb_ptr_t)&vb, MemeVariableBuffer_data(_other), MemeVariableBuffer_size(_other));
		if (result) {
			MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
			return result;
		}

		MemeVariableBuffer_swap((mmvb_ptr_t)&vb, _s);
		MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
		return 0;
	}
	
	switch (MMSTR__GET_IMPLTYPE(str))
	{
	case MemeString_ImplType_small:
	{
		if (0 == MemeStringSmall_canBeAppendIt(
			(const MemeStringSmall_t*)str, MemeVariableBuffer_size(_other)))
		{
			return MemeStringSmall_appendWithBytes(
				(MemeStringSmall_t*)str, MemeVariableBuffer_data(_other), MemeVariableBuffer_size(_other));
		}
		else {
			int result = MemeStringImpl_capacityExpansionSmallToMedium(
				(MemeStringStack_t*)str, MemeString_byteSize(str) + MemeVariableBuffer_size(_other));
			if (result)
				return result;
			return MemeStringMedium_appendWithBytes(
				(MemeStringMedium_t*)str, MemeVariableBuffer_data(_other), MemeVariableBuffer_size(_other));
		}
	} break;
	case MemeString_ImplType_medium:
	{
		return MemeStringMedium_appendWithBytes(
			(MemeStringMedium_t*)str, MemeVariableBuffer_data(_other), MemeVariableBuffer_size(_other));
	} break;
	default: {
		return (MGEC__OPNOTSUPP);
	} break;
	}

	//return 0;
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_insertWithBytes(
	MemeVariableBuffer_t _s, MemeInteger_t _pos, const MemeByte_t* _buf, MemeInteger_t _len)
{
    MemeString_Const_t s = (MemeString_Const_t)_s;

    assert(s != 0 && "MemeVariableBuffer_insertWithBytes");
    assert(MemeStringImpl_isModifiableType(MMSTR__GET_IMPLTYPE(s)) == 1
        && "MemeVariableBuffer_insertWithBytes");

	if ((_len < 0))
		_len = strlen((const char*)_buf);
	if ((_buf == NULL || _len == 0))
		return (MGEC__INVAL);
    //if (_pos < 0 || _pos > MemeVariableBuffer_size(s))
	if ((_pos & (MemeString_byteSize(s) - _pos)) & INTPTR_MIN)
		return (MGEC__INVAL);
	
    switch (MMSTR__GET_IMPLTYPE(s))
    {
    case MemeString_ImplType_small:
    {
        if (0 == MemeStringSmall_canBeAppendIt((const MemeStringSmall_t*)s, _len))
        {
            return MemeStringSmall_insertWithBytes((MemeStringSmall_t*)s, _pos, _buf, _len);
        }
        else {
            int result = MemeStringImpl_capacityExpansionSmallToMedium(
                (MemeStringStack_t*)s, MemeString_byteSize(s) + _len);
            if ((result != 0))
                return result;
            return MemeStringMedium_insertWithBytes((MemeStringMedium_t*)s, _pos, _buf, _len);
        }
    } break;
    case MemeString_ImplType_medium:
    {
        return MemeStringMedium_insertWithBytes((MemeStringMedium_t*)s, _pos, _buf, _len);
    };
    default: {
        return (MGEC__OPNOTSUPP);
    } break;
    }

    //return 0;
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_clear(MemeVariableBuffer_t _s)
{
	MemeString_t s = (MemeString_t)_s;

	assert(s != NULL && "MemeVariableBuffer_clear");
	assert(MemeStringImpl_isModifiableType(MMSTR__GET_IMPLTYPE(s)) == 1);

	switch (MMSTR__GET_IMPLTYPE(s)) {
	case MemeString_ImplType_small:
	{
		MemeStringSmall_clear(&(s->small_));
	} break;
	case MemeString_ImplType_medium:
	{
		MemeStringMedium_clear(&(s->medium_));
	} break;
	default: {
		return (MGEC__OPNOTSUPP);
	} break;
	}

	return 0;
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_insertWithByte(
	MemeVariableBuffer_t _s, MemeInteger_t _pos, MemeByte_t _byte)
{
	assert(_s && "MemeVariableBuffer_insertWithByte");

	return MemeVariableBuffer_insertWithBytes(_s, _pos, &_byte, 1);
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_resize(MemeVariableBuffer_t _s, MemeInteger_t _size)
{
	return MemeVariableBuffer_resizeWithByte(_s, _size, 0);
}

MEME_EXTERN_C MEME_API mgec_t MEME_STDCALL MemeVariableBuffer_resizeAndOverwrite(mmvb_ptr_t _b, mmint_t _size)
{
    mmstr_ptr_t s = (mmstr_ptr_t)_b;
	
    assert(s != NULL  && "MemeVariableBuffer_resizeAndOverwrite");
    assert(_size >= 0 && "MemeVariableBuffer_resizeAndOverwrite");
    assert(MemeStringImpl_isModifiableType(MMSTR__GET_IMPLTYPE(s)) == 1 && "MemeVariableBuffer_resizeAndOverwrite");

	switch (MMSTR__GET_IMPLTYPE(s)) {
	case mmstr_impltype_small: {
		if (_size <= MMSTR__GET_SMALL_BUF_MAX_SIZE)
			return MemeStringSmall_resizeAndOverwrite((MemeStringSmall_t*)s, _size);
		else {
            int rc = MemeStringImpl_capacityExpansionSmallToMedium((mmstrstk_t*)s, _size);
            if (rc)
                return rc;
			
            return MemeStringMedium_resizeAndOverwrite((MemeStringMedium_t*)s, _size);
		}
	} break;
	case mmstr_impltype_medium: {
        return MemeStringMedium_resizeAndOverwrite((MemeStringMedium_t*)s, _size);
    } break;
    default: {
        return MGEC__OPNOTSUPP;
    } break;
	}

	//return 0;
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_resizeWithByte(MemeVariableBuffer_t _s, MemeInteger_t _size, MemeByte_t _byte)
{
	MemeString_t s = (MemeString_t)_s;

	assert(s != NULL && "MemeVariableBuffer_resizeWithByte");
	assert(_size >= 0 && "MemeVariableBuffer_resizeWithByte");
	assert(MemeStringImpl_isModifiableType(MMSTR__GET_IMPLTYPE(s)) == 1);

	switch (MMSTR__GET_IMPLTYPE(s)) {
	case MemeString_ImplType_small:
	{
		if (_size <= MMSTR__GET_SMALL_BUF_MAX_SIZE)
		{
			return MemeStringSmall_resizeWithByte((MemeStringSmall_t*)_s, _size, _byte);
		}
		else {
			int result = MemeStringImpl_capacityExpansionSmallToMedium((MemeStringStack_t*)s, _size);
			if (result)
				return result;
			return MemeStringMedium_resizeWithByte((MemeStringMedium_t*)_s, _size, _byte);
		}
	} break;
	case MemeString_ImplType_medium:
	{
		return MemeStringMedium_resizeWithByte((MemeStringMedium_t*)_s, _size, _byte);
	} break;
	default: {
		return (MGEC__OPNOTSUPP);
	} break;
	}

	//return 0;
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_remove(
	MemeVariableBuffer_t _s, MemeInteger_t _pos, MemeInteger_t _count)
{
    MemeString_t s = (MemeString_t)_s;

    assert(s != NULL && "MemeVariableBuffer_remove");
    assert(MemeStringImpl_isModifiableType(MMSTR__GET_IMPLTYPE(s)) == 1);

	if (_pos < 0)
		_pos = 0;

	if (_count == 0)
		return 0;

    switch (MMSTR__GET_IMPLTYPE(s)) {
    case MemeString_ImplType_small:
    {
		return MemeStringSmall_remove ((MemeStringSmall_t*)_s, _pos, _count);
    } break;
    case MemeString_ImplType_medium:
    {
        return MemeStringMedium_remove((MemeStringMedium_t*)_s, _pos, _count);
    } break;
    default: {
        return (MGEC__OPNOTSUPP);
    } break;
    }

    //return 0;
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_releaseToBuffer(
	MemeVariableBuffer_t _s, MemeBufferStack_t* _out, MemeInteger_t _objectSize)
{
	MemeString_t s = (MemeString_t)_s;

	assert(s != NULL    && "MemeVariableBuffer_releaseToBuffer");
	assert(_out != NULL && "MemeVariableBuffer_releaseToBuffer");

	switch (MMSTR__GET_IMPLTYPE(s))
	{
	case MemeString_ImplType_small:
	{
		MemeBufferStack_initByBytes(
			_out, MEME_STRING__OBJECT_SIZE, 
			MemeVariableBuffer_data(_s), MemeVariableBuffer_size(_s));
		MemeStringSmall_clear(&(s->small_));
		return 0;
	} break;
	case MemeString_ImplType_medium:
	{
		MemeByte_t* data_pointer = s->medium_.real_;
		int result = MemeStringLarge_initAndTakeover(
			(MemeStringLarge_t*)_out, data_pointer,
			MemeStringMedium_realByteSize (&(s->medium_)),
			MemeStringMedium_frontCapacity(&(s->medium_)), s->medium_.size_
		);
		if (result)
			return result;

		s->medium_.real_ = NULL;
		MemeStringMedium_reset(&(s->medium_));

		return 0;
	} break;
	default:
	{
		return (MGEC__OPNOTSUPP);
	} break;
	}
}

/**
 * @brief Transfer the content of a MemeVariableBuffer into a stack string
 *        without copying heap memory where possible.
 *
 * Implementation dispatches on the internal storage type of @p _s:
 *
 *  <b>Small storage</b>
 *   - @c memcpy the entire small-string struct into @p _out (fixed
 *     @c MEME_STRING__OBJECT_SIZE bytes, regardless of @p _objectSize).
 *   - Strip trailing null bytes via @c MemeStringSmall_shrinkTailZero().
 *   - Clear the source slot via @c MemeStringSmall_clear().
 *   - Return @c 0.  Cannot fail.
 *
 *  <b>Medium storage</b>
 *   - Snapshot @c real_ (the heap buffer pointer) from the medium struct.
 *   - Call @c MemeStringLarge_initAndTakeover() to hand the pointer, total
 *     capacity, front-capacity, and content size directly to @p _out as a
 *     large-string object — no heap allocation, no data copy.
 *   - On failure of the takeover, return the error immediately.  @p _out is
 *     indeterminate; @p _s still owns the buffer.
 *   - Strip trailing null bytes via @c MemeStringLarge_shrinkTailZero().
 *   - Null @c s->medium_.real_ and call @c MemeStringMedium_reset() to mark
 *     the source as empty.  These steps happen only after a successful
 *     takeover, so the source buffer is never double-freed.
 *   - Return @c 0.
 *
 *  <b>All other types</b>
 *   - Return @c MGEC__OPNOTSUPP.  Neither @p _s nor @p _out is modified.
 *
 * @param[in,out] _s          Variable buffer to release.  Must be non-NULL.
 *                            Left empty on success; unchanged on failure.
 * @param[out]    _out        Uninitialized raw storage for the result.
 *                            Must be non-NULL.  Indeterminate on medium
 *                            takeover failure; untouched on MGEC__OPNOTSUPP.
 * @param[in]     _objectSize Accepted but currently unused by the
 *                            implementation.
 *
 * @return @c 0 on success, @c MGEC__OPNOTSUPP for unsupported types, or a
 *         non-zero error code from @c MemeStringLarge_initAndTakeover.
 */
MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_releaseToString(
	MemeVariableBuffer_t _s, MemeStringStack_t* _out, MemeInteger_t _objectSize)
{
	MemeString_t s = (MemeString_t)_s;

	assert(s != NULL    && "MemeVariableBuffer_releaseToString");
	assert(_out != NULL && "MemeVariableBuffer_releaseToString");

	switch (MMSTR__GET_IMPLTYPE(s))
	{
	case MemeString_ImplType_small:
	{
		memcpy(_out, &(s->small_), MEME_STRING__OBJECT_SIZE);
		MemeStringSmall_shrinkTailZero((MemeStringSmall_t*)_out);
		MemeStringSmall_clear(&(s->small_));
		return 0;
	} break;
	case MemeString_ImplType_medium:
	{
		MemeByte_t* data_pointer = s->medium_.real_;
		int result = MemeStringLarge_initAndTakeover(
			(MemeStringLarge_t*)_out, data_pointer,
			MemeStringMedium_realByteSize (&(s->medium_)),
			MemeStringMedium_frontCapacity(&(s->medium_)), s->medium_.size_
		);
		if (result)
			return result;
		MemeStringLarge_shrinkTailZero((MemeStringLarge_t*)_out);

		s->medium_.real_ = NULL;
		MemeStringMedium_reset(&(s->medium_));
		return 0;
	} break;
	default:
	{
		return (MGEC__OPNOTSUPP);
	} break;
	}
	//return 0;
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_split(
	MemeVariableBuffer_Const_t _s, const MemeByte_t* _key, MemeInteger_t _key_len,
	MemeFlag_SplitBehavior_t _sb, MemeVariableBufferStack_t* _out, MemeInteger_t* _out_count,
	MemeInteger_t* _search_index
)
{
	return MemeStringStack_split(
		(const mmstrstk_t*)_s, (const char*)_key, _key_len, _sb, MemeFlag_AllSensitive,
		(mmstrstk_t*)_out, MMSTR__OBJ_SIZE, _out_count, _search_index);
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_reserve(MemeVariableBuffer_t _s, MemeInteger_t _size)
{
	assert(_s != NULL && "MemeVariableBuffer_reserve");
	assert(_size >= 0 && "MemeVariableBuffer_reserve");
	assert(MemeStringImpl_isModifiableType(MMSTR__GET_IMPLTYPE((MemeString_t)_s)) == 1);

	return MemeStringImpl_capacityExpansionWithModifiable((MemeStringStack_t*)_s, _size);
}

MEME_EXTERN_C MEME_API MemeInteger_t MEME_STDCALL MemeVariableBuffer_selfChop(MemeVariableBuffer_t _s, MemeInteger_t _n)
{
	assert(_s != NULL && "MemeVariableBuffer_selfChop");
	assert(MemeStringImpl_isModifiableType(MMSTR__GET_IMPLTYPE((MemeString_t)_s)) == 1);

	if (_n <= 0)
		return 0;
	else if (_n > MemeString_byteSize((MemeString_t)_s)) 
	{
		MemeVariableBuffer_clear(_s);
		return 0;
	}

	switch (MMSTR__GET_IMPLTYPE((mmstr_cptr_t)_s)) 
	{
	case MemeString_ImplType_small:
	{
		MemeStringSmall_byteSizeOffsetAndSetZero(&(((MemeString_t)_s)->small_), _n);
	} break;
	case MemeString_ImplType_medium:
	{
		MemeStringMedium_byteSizeOffsetAndSetZero(&(((MemeString_t)_s)->medium_), _n);
	} break;
	default: {
		return (MGEC__OPNOTSUPP);
	} break;
	}
	return 0;
}

MEME_EXTERN_C MEME_API MemeInteger_t MEME_STDCALL MemeVariableBuffer_capacityCorrectness(MemeVariableBuffer_Const_t _s)
{
	if (_s == NULL)
		return 0;

	if (MemeString_maxByteCapacity((MemeString_Const_t)_s) ==
		MemeString_availableByteCapacity((MemeString_Const_t)_s) + MemeString_byteSize((MemeString_Const_t)_s))
		return 1;
	else
		return 0;
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_replace(
	MemeVariableBuffer_t _s,
	const MemeByte_t* _from, MemeInteger_t _from_len,
	const MemeByte_t* _to, MemeInteger_t _to_len,
	MemeInteger_t _max_count)
{
	MemeString_t s = (MemeString_t)_s;
	MemeInteger_t total;
	MemeInteger_t delta;
	MemeInteger_t count;
	MemeInteger_t pos;
	MemeByte_t* data;

	assert(s != NULL && "MemeVariableBuffer_replace");
	assert(MemeStringImpl_isModifiableType(MMSTR__GET_IMPLTYPE(s)) == 1);

	/* Boundary checks: empty pattern or zero replacements -> no-op */
	if (_from_len < 0 || _to_len < 0)
		return MGEC__INVAL;
	if (_from_len == 0 || _max_count == 0)
		return 0;

	total = MemeVariableBuffer_size(_s);
	delta = _to_len - _from_len;

	/* --- Equal-length fast path: direct memcpy, zero allocation --- */
	if (delta == 0) {
		data = MemeVariableBuffer_dataWithNotConst(_s);
		pos = 0;
		count = 0;
		while (pos <= total - _from_len) {
			if (_max_count > 0 && count >= _max_count)
				break;
			if (memcmp(data + pos, _from, _from_len) == 0) {
				memcpy(data + pos, _to, _from_len);
				++count;
				pos += _from_len;
			} else {
				++pos;
			}
		}
		return 0;
	}

	/* --- Unequal-length path: reserve if needed, then splice match by match --- */

	/* Pass 1: count non-overlapping greedy matches (up to _max_count, -1 = all) */
	count = 0;
	pos = 0;
	while (pos <= total - _from_len) {
		if (_max_count > 0 && count >= _max_count)
			break;
		if (memcmp(MemeVariableBuffer_data((MemeVariableBuffer_Const_t)_s) + pos, _from, _from_len) == 0) {
			++count;
			pos += _from_len;   /* non-overlapping: resume strictly past the match */
		} else {
			++pos;
		}
	}

	if (count == 0)
		return 0;

	/* Grow capacity once up-front when the buffer will become longer */
	if (delta > 0) {
		MemeInteger_t new_size = total + delta * count;
		MemeInteger_t rc = MemeVariableBuffer_reserve(_s, new_size);
		if (rc)
			return rc;
	}

	/*
	 * Zero-extra-allocation splice, left to right.  The match set must be the
	 * greedy, left-to-right, non-overlapping set counted above.  Searching the
	 * live buffer from @c pos each step reproduces it exactly: after replacing
	 * at @c idx we resume at @c idx + _to_len, which is past the inserted text
	 * and therefore re-examines only the ORIGINAL tail bytes, while everything
	 * before @c idx is already final.
	 *
	 * memmove is safe in both directions of overlap:
	 *   - growing  (_to_len > _from_len): tail moves right, then _to written;
	 *   - shrinking(_to_len < _from_len): tail moves left, then _to written.
	 */
	pos = 0;
	{
		MemeInteger_t remaining = count;
		while (remaining > 0) {
			MemeInteger_t idx = MemeVariableBuffer_indexOfWithBytes(
				_s, pos, _from, _from_len);
			if (idx < 0)
				return MGEC__INVAL;   /* cannot happen: pass 1 counted `count` */

			MemeInteger_t tail_start = idx + _from_len;
			MemeInteger_t tail_len = total - tail_start;

			data = MemeVariableBuffer_dataWithNotConst(_s);
			memmove(data + idx + _to_len, data + tail_start, tail_len);
			memcpy(data + idx, _to, _to_len);

			total += delta;

			/* Keep the logical size in sync after every splice so the next
			   indexOf search reflects the current (grown or shrunk) content.
			   A stale size would truncate the search window and break the
			   all-replacements (count < 0) case. */
			switch (MMSTR__GET_IMPLTYPE(s)) {
			case MemeString_ImplType_small:
				MemeStringSmall_byteSizeOffsetAndSetZero(&s->small_, delta);
				break;
			case MemeString_ImplType_medium:
				MemeStringMedium_byteSizeOffsetAndSetZero(&s->medium_, delta);
				break;
			default:
				return MGEC__OPNOTSUPP;
			}

			pos = idx + _to_len;
			--remaining;
		}
	}

	return 0;
}

MEME_EXTERN_C MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_slice(
	MemeVariableBuffer_Const_t _s,
	MemeInteger_t _pos, MemeInteger_t _count,
	MemeVariableBufferStack_t* _out, mmint_t _object_size)
{
	assert(_s != NULL && "MemeVariableBuffer_slice");
	assert(_out != NULL && "MemeVariableBuffer_slice");

	const MemeInteger_t total = MemeVariableBuffer_size(_s);

	/* pos out of range: return empty buffer */
	if (_pos < 0 || _pos > total)
	{
		if (_object_size > 0)
		{
			int rc = MemeVariableBufferStack_init(_out, (size_t)_object_size);
			if (rc)
				return rc;
		}
		else
		{
			int rc = MemeVariableBuffer_clear((MemeVariableBuffer_t)_out);
			if (rc)
				return rc;
		}
		return 0;
	}

	if (_count < 0 || _pos + _count > total)
		_count = total - _pos;

	if (_object_size > 0)
	{
		int rc = MemeVariableBufferStack_init(_out, (size_t)_object_size);
		if (rc)
			return rc;
	}
	else
	{
		int rc = MemeVariableBuffer_clear((MemeVariableBuffer_t)_out);
		if (rc)
			return rc;
	}

	return MemeVariableBuffer_appendWithBytes(
		(MemeVariableBuffer_t)_out,
		MemeVariableBuffer_data(_s) + _pos, _count);
}
