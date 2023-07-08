
#include "meme/buffer.h"
#include "meme/string.h"
#include "meme/unsafe/string_view.h"
#include "meme/unsafe/buffer_view.h"
#include "meme/impl/string.h"

#include <errno.h>
#include <assert.h>

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

MEME_API int MEME_STDCALL MemeBufferStack_initByString(
	MemeBufferStack_t* _out, size_t _object_size, mms_const_t _s)
{
    return MemeStringStack_initByOther((MemeStringStack_t*)_out, _object_size, _s);
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
	return MemeStringStack_initByU8bytesAndType(
		(mmsstk_t*)_out, _object_size, _utf8, _len, _suggest);
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
	return MemeStringStack_assign((MemeStringStack_t*)_s, _object_size, (MemeString_Const_t)_other);
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
MEME_STDCALL MemeBuffer_isNonempty(MemeBuffer_Const_t _s)
{
	return MemeString_isNonempty((MemeString_Const_t)_s);
}

MEME_API int MEME_STDCALL MemeBuffer_isEmpty(MemeBuffer_Const_t _s)
{
	return MemeString_isEmpty((MemeString_Const_t)_s);
}

MEME_API const mmbyte_t* 
MEME_STDCALL MemeBuffer_at(mmbuf_const_t _s, mmint_t _pos)
{
    return MemeString_at((MemeString_Const_t)_s, _pos);
}

MEME_API const MemeByte_t*
MEME_STDCALL MemeBuffer_data(MemeBuffer_Const_t _s)
{
	return MemeString_byteData((MemeString_Const_t)_s);
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
	return MemeString_isEqual((MemeString_Const_t)_s, (const char*)_buf, _len, _result);
}

MEME_API int 
MEME_STDCALL MemeBuffer_isEqualWithOther(
	MemeBuffer_Const_t _lhs, MemeBuffer_Const_t _rhs, int* _result)
{
	return MemeString_isEqualWithOther(
		(MemeString_Const_t)_lhs, (MemeString_Const_t)_rhs, _result);
}

MEME_API mmint_t MEME_STDCALL MemeBuffer_isSharedStorageTypes(mmbuf_const_t _b)
{
    return MemeString_isSharedStorageTypes((mms_const_t)_b);
}

MEME_API MemeInteger_t 
MEME_STDCALL MemeBuffer_indexOfWithBytes(
	MemeBuffer_Const_t _s, MemeInteger_t _offset, const MemeByte_t* _needle, MemeInteger_t _needle_len)
{
	return MemeString_indexOfWithUtf8bytes(
		(MemeString_Const_t)_s, _offset, _needle, _needle_len, MemeFlag_AllSensitive);
}

MEME_API MemeInteger_t 
MEME_STDCALL MemeBuffer_indexOfWithOther(
	MemeBuffer_Const_t _s, MemeInteger_t _offset, MemeBuffer_Const_t _other)
{
	return MemeString_indexOfWithOther(
		(MemeString_Const_t)_s, _offset, (MemeString_Const_t)_other, MemeFlag_AllSensitive);
}

MEME_API MemeInteger_t 
MEME_STDCALL MemeBuffer_startsMatchWithBytes(
	MemeBuffer_Const_t _s, const MemeByte_t* _needle, MemeInteger_t _needle_len)
{
    return MemeString_startsMatchWithUtf8bytes(
        (MemeString_Const_t)_s, _needle, _needle_len, MemeFlag_AllSensitive);
}

MEME_API MemeInteger_t 
MEME_STDCALL MemeBuffer_startsMatchWithOther(MemeBuffer_Const_t _s, MemeBuffer_Const_t _other)
{
    return MemeString_startsMatchWithOther(
        (MemeString_Const_t)_s, (MemeString_Const_t)_other, MemeFlag_AllSensitive);
}

MEME_API MemeInteger_t 
MEME_STDCALL MemeBuffer_endsMatchWithBytes(
	MemeBuffer_Const_t _s, const MemeByte_t* _needle, MemeInteger_t _needle_len)
{
    return MemeString_endsMatchWithUtf8bytes(
        (MemeString_Const_t)_s, _needle, _needle_len, MemeFlag_AllSensitive);
}

MEME_API MemeInteger_t 
MEME_STDCALL MemeBuffer_endsMatchWithOther(MemeBuffer_Const_t _s, MemeBuffer_Const_t _other)
{
    return MemeString_endsMatchWithOther(
        (MemeString_Const_t)_s, (MemeString_Const_t)_other, MemeFlag_AllSensitive);
}

MEME_API MemeInteger_t 
MEME_STDCALL MemeBuffer_split(
	MemeBuffer_Const_t _s, const MemeByte_t* _key, MemeInteger_t _key_len, 
	MemeFlag_SplitBehavior_t _sb, MemeBufferStack_t* _out, MemeInteger_t* _out_count, 
	MemeInteger_t* _search_index
)
{
	return MemeString_split(
		(MemeString_Const_t)_s, (const char*)_key, _key_len, _sb, MemeFlag_AllSensitive,
		(MemeStringStack_t*)_out, _out_count, _search_index);
}

MEME_API int
MEME_STDCALL MemeBufferViewUnsafeStack_init(
	mmbufstk_t* _self,
    size_t _object_size,
	const MemeByte_t* _buf, MemeInteger_t _len
)
{	
	MemeStringViewUnsafe_t* p = (MemeStringViewUnsafe_t*)_self;

	assert(_self != NULL && MemeBufferViewUnsafeStack_init);
	
    if (_buf == NULL)
	{
        p->data_ = NULL;
        p->size_ = 0;
        p->_res_ = 0;
        p->type_ = MemeBuffer_UnsafeStorageType_view;
        return 0;
    }

	if (_len < 0)
		_len = strlen((const char*)_buf);
	
	p->data_   = _buf;
	p->size_   = _len;
	p->_res_   = 0;
	p->type_   = MemeBuffer_UnsafeStorageType_view;
	return 0;
}

MEME_API int
MEME_STDCALL MemeBufferViewUnsafeStack_initByOther(
	mmbufstk_t* _self,
	size_t _object_size,
	const mmbufstk_t* _other
)
{
	mmsstk_t* s = (mmsstk_t*)_self;
	
	assert(_self != NULL  && MemeBufferViewUnsafeStack_initByOther);
	assert(_other != NULL && MemeBufferViewUnsafeStack_initByOther);

	if (MMS__GET_TYPE((MemeString_t)_other) == MemeString_ImplType_view)
	{
		memcpy(_self, _other, MEME_STRING__OBJECT_SIZE);
		return 0;
	}
	else if (MemeString_isSharedStorageTypes((MemeString_t)_other) == 1)
	{
		return MemeStringStack_initByOther(s, _object_size, (MemeString_t)_other);
	}
	else {
		return MemeStringViewUnsafeStack_init(
			s, _object_size,
			MemeString_byteData((MemeString_t)_other),
			MemeString_byteSize((MemeString_t)_other));
	}
}

MEME_API int MEME_STDCALL MemeBufferViewUnsafeStack_initByString(
	mmbufstk_t* _self, size_t _object_size, const mmsstk_t* _other)
{
    return MemeBufferViewUnsafeStack_initByOther(_self, _object_size, (mmbufstk_t*)_other);
}

MEME_API int MEME_STDCALL MemeBufferViewUnsafeStack_initByVariableBuffer(
	mmbufstk_t* _self, size_t _object_size, const mmvbstk_t* _other)
{
	mmsstk_t* s = (mmsstk_t*)_self;

	assert(_self != NULL  && MemeBufferViewUnsafeStack_initByOther);
	assert(_other != NULL && MemeBufferViewUnsafeStack_initByOther);

	return MemeStringViewUnsafeStack_init(
		s, _object_size,
		MemeString_byteData((MemeString_t)_other),
		MemeString_byteSize((MemeString_t)_other));
}

MEME_API int
MEME_STDCALL MemeBufferViewUnsafeStack_assignByOther(
	mmbufstk_t* _self,
	size_t _object_size,
	const mmbufstk_t* _other
)
{
	mmsstk_t* s = (mmsstk_t*)_self;
	
	assert(_self != NULL  && MemeStringViewUnsafeStack_assignByOther);
	assert(_other != NULL && MemeStringViewUnsafeStack_assignByOther);

	if (MMS__GET_TYPE((MemeString_t)_other) == MemeString_ImplType_view)
	{
		int result = MemeStringStack_unInit(s, sizeof(MemeStringStack_t));
		if (result)
			return result;
		memcpy(_self, _other, MEME_STRING__OBJECT_SIZE);
		return 0;
	}
	else if (MemeString_isSharedStorageTypes((MemeString_t)_other) == 1)
	{
		return MemeStringStack_assign(s, _object_size, (MemeString_t)_other);
	}
	else {
		int result = MemeStringStack_unInit(s, sizeof(MemeStringStack_t));
		if (result)
			return result;
		return MemeStringViewUnsafeStack_init(
			s, _object_size,
			MemeString_byteData((MemeString_t)_other),
			MemeString_byteSize((MemeString_t)_other));
	}
}

MEME_API int MEME_STDCALL MemeBufferViewUnsafeStack_assignByString(
	mmbufstk_t* _self, size_t _object_size, const mmsstk_t* _other)
{
    return MemeBufferViewUnsafeStack_assignByOther(_self, _object_size, (mmbufstk_t*)_other);
}

MEME_API int MEME_STDCALL MemeBufferViewUnsafeStack_assignByVariableBuffer(
	mmbufstk_t* _self, size_t _object_size, const mmvbstk_t* _other)
{
	mmsstk_t* s = (mmsstk_t*)_self;

    assert(_self != NULL  && MemeStringViewUnsafeStack_assignByOther);
    assert(_other != NULL && MemeStringViewUnsafeStack_assignByOther);

    int result = MemeStringStack_unInit(s, sizeof(MemeStringStack_t));
    if (result)
        return result;
    return MemeStringViewUnsafeStack_init(
        s, _object_size,
        MemeString_byteData((MemeString_t)_other),
        MemeString_byteSize((MemeString_t)_other));
}
