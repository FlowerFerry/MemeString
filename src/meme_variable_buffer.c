
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
