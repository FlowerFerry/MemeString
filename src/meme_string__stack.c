
#include "mego/predef/symbol/likely.h"
#include "meme/string.h"
#include "meme/buffer.h"
#include <meme/rune.h>
#include "meme/impl/string.h"
#include "meme/impl/string_p__user.h"
#include "meme/impl/string_p__small.h"
#include "meme/impl/string_p__medium.h"
#include "meme/impl/string_p__large.h"
#include "meme/variable_buffer.h"
#include <meme/utf/converter.h>
#include <meme/utf/u8rune.h>

#include "meme/unsafe/string_view.h"
#include <meme/std/string16.h>

#include <ctype.h>
#include <stdio.h>
#include <string.h>

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_init(
	mmsstk_t* _out, size_t _object_size)
{
	MemeString_t obj = NULL;

	assert(_out);
	assert(_object_size != 0);
    
	if ((_object_size > sizeof(struct _MemeString_t)))
		return (MGEC__OPNOTSUPP);

	obj = (mms_t)_out;
	obj->small_.buffer_[0] = '\0';
	obj->small_.type_ = MemeString_ImplType_small;
	obj->small_.capacity_ = MMS__GET_SMALL_BUFFER_SIZE;
	return 0;
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL MemeStringStack_getInitObject(size_t _object_size)
{
	mmsstk_t stack;
	MemeStringStack_init(&stack, _object_size);
	return stack;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_initTakeOverUserObject(
	mmsstk_t* _out, size_t _object_size,
	void* _user_data, 
	MemeString_UserObjectDestruct_t* _destruct_fn, 
	MemeString_UserObjectData_t* _data_fn, 
	MemeString_UserObjectSize_t* _size_fn)
{
	size_t len = 0;
	//int result = 0;

	assert(_out != NULL			&& "function is" && MemeStringStack_initTakeOverUserObject);
	assert(_user_data != NULL	&& "function is" && MemeStringStack_initTakeOverUserObject);
	assert(_destruct_fn != NULL && "function is" && MemeStringStack_initTakeOverUserObject);
	assert(_data_fn != NULL		&& "function is" && MemeStringStack_initTakeOverUserObject);
	assert(_size_fn != NULL		&& "function is" && MemeStringStack_initTakeOverUserObject);
    
	len = _size_fn(_user_data);
	if (len <= MMS__GET_SMALL_BUFFER_SIZE)
	{
		int result = MemeStringStack_initByU8bytes(
			_out, _object_size, (const MemeByte_t*)_data_fn(_user_data), len);
		if (result)
			return result;

		_destruct_fn(_user_data);
	}
	else {
		return MemeStringUser_initTakeOver(
			(MemeStringUser_t*)_out, NULL, NULL, _user_data, len, _destruct_fn, _data_fn);
	}
	return 0;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_unInit(mmsstk_t* _out, size_t _object_size)
{
	mms_t obj = (mms_t)_out;

	assert(obj && MemeStringStack_unInit);
	assert(_object_size != 0 && MemeStringStack_unInit);

	switch (MMS__GET_TYPE(obj)) {
	case MemeString_ImplType_small:
	{
		// do nothing
	} break;
	case MemeString_ImplType_medium:
	{
		return MemeStringMedium_unInit((MemeStringMedium_t*)_out);
	} break;
	case MemeString_ImplType_large:
	{
		return MemeStringLarge_unInit((MemeStringLarge_t*)_out);
	};
	case MemeString_ImplType_view:
	{
		// do nothing
	} break;
	case MemeString_ImplType_user:
	{
		return MemeStringUser_unInit((MemeStringUser_t*)_out);
	};
	default: {
		return (MGEC__OPNOTSUPP);
	};
	}
	return 0;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_initByU8bytesAndType(
	mmsstk_t* _out, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len,
	MemeString_Storage_t _suggest)
{
	MemeString_Storage_t type;

	assert(_out && MemeStringStack_initByU8bytesAndType);
	assert(_object_size != 0 && MemeStringStack_initByU8bytesAndType);

    if ((_utf8 == NULL))
        return (MGEC__INVAL);

	if (_len < 0)
		_len = strlen((const char*)_utf8);

	type = MemeStringImpl_initSuggestType(_len, _suggest);
	switch (type) {
	case MemeString_StorageType_large: {
		return MemeStringLarge_initByU8bytes((MemeStringLarge_t*)_out, _utf8, _len, NULL, NULL, 0, 0);
	} break;
	case MemeString_StorageType_medium: {
		int result = MemeStringMedium_initWithCapacity((MemeStringMedium_t*)_out, _len);
		if (result)
			return result;
		return MemeStringMedium_appendWithBytes((MemeStringMedium_t*)_out, _utf8, _len);
	} break;
	case MemeString_StorageType_small: {
		return MemeStringSmall_initByU8bytes((MemeStringSmall_t*)_out, _utf8, _len);
	};
	}

	if (MemeString_StorageType_user < type)
	{
		// TO_DO
	}

	return MemeStringLarge_initByU8bytes((MemeStringLarge_t*)_out, _utf8, _len, NULL, NULL, 0, 0);
}

MEME_EXTERN_C MEME_API int
MEME_STDCALL MemeStringStack_initByU16bytes(
	mmsstk_t* _out, size_t _object_size, const uint16_t* _buf, MemeInteger_t _len)
{
    return MemeStringStack_initByU16bytesAndType(
        _out, _object_size, _buf, _len, MemeString_StorageType_none);
}

MEME_EXTERN_C MEME_API int
MEME_STDCALL MemeStringStack_initByU16bytesAndType(
	mmsstk_t* _out, size_t _object_size, const uint16_t* _buf, MemeInteger_t _len,
	MemeString_Storage_t _suggest)
{
	MemeInteger_t u8len = -1;
	MemeString_Storage_t type;
	
    assert(_out && MemeStringStack_initByU16bytesAndType);
    assert(_object_size != 0 && MemeStringStack_initByU16bytesAndType);
	
    if ((_buf == NULL))
        return (MGEC__INVAL);
	
    if (_len < 0)
        _len = strlen16(_buf);
	
    u8len = mmutf_char_size_u8from16(_buf, _len);
	type  = MemeStringImpl_initSuggestType(u8len, _suggest);
	switch (type) {
	case MemeString_StorageType_medium: {
		MemeInteger_t pos = 0;
		int result = MemeStringMedium_initWithCapacity((MemeStringMedium_t*)_out, u8len);
		if (result)
			return result;
		result = MemeStringMedium_resizeWithByte((MemeStringMedium_t*)_out, u8len, 0);
        if (result)
            return result;
		pos = mmutf_convert_u16to8(_buf, _len, MemeStringMedium_data((MemeStringMedium_t*)_out));
        if (pos != u8len)
			return MemeStringMedium_resizeWithByte((MemeStringMedium_t*)_out, pos, 0);
		else 
            return 0;
	} break;
	case MemeString_StorageType_small: {
		MemeInteger_t pos = 0;
		MemeStringStack_init((mmsstk_t*)_out, _object_size);
		MemeStringSmall_resizeWithByte((MemeStringSmall_t*)_out, u8len, 0);
		pos = mmutf_convert_u16to8(_buf, _len, MemeStringImpl_forcedData((mmsstk_t*)_out));
        if (pos != u8len)
			return MemeStringSmall_resizeWithByte((MemeStringSmall_t*)_out, pos, 0);
        else
            return 0;
	} break;
	default: {
		MemeInteger_t pos = 0;
		mmvbstk_t vbuf;
		int result = 0;
		MemeVariableBufferStack_init(&vbuf, MMS__OBJECT_SIZE);
		result = (int)MemeVariableBuffer_resize((mmvb_t)&vbuf, u8len);
		if (result) {
			MemeVariableBufferStack_unInit(&vbuf, MMS__OBJECT_SIZE);
			return result;
		}
        pos = mmutf_convert_u16to8(_buf, _len, MemeVariableBuffer_dataWithNotConst((mmvb_t)&vbuf));
        result = (int)MemeVariableBuffer_resize((mmvb_t)&vbuf, pos);
		if (result) {
			MemeVariableBufferStack_unInit(&vbuf, MMS__OBJECT_SIZE);
			return result;
		}

		result = (int)MemeVariableBuffer_releaseToString((mmvb_t)&vbuf, _out, _object_size);
		if (result) {
			return result;
		}
        return result;
	} break;
	}
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_initByOther(
	mmsstk_t* _out, size_t _object_size, mms_const_t _other)
{

	assert(_out);
	assert(_other);
	assert(_object_size != 0);

	if (((void*)_out == (void*)_other))
		return (MGEC__CANCELED);

	switch (MMS__GET_TYPE(_other)) {
	case MemeString_StorageType_small: {
		memcpy(_out, _other, MMS__OBJECT_SIZE);
	} break;
	case MemeString_StorageType_medium: {
		MemeStringStack_init(_out, MMS__OBJECT_SIZE);
		return (int)MemeVariableBuffer_appendWithBytes((mmvb_t)_out,
			MemeString_byteData(_other), MemeString_byteSize(_other));
	};
	case MemeString_StorageType_large: {
		return MemeStringLarge_initByOther((MemeStringLarge_t*)_out, &(_other->large_));
	};
	case MemeString_StorageType_user: {
		return MemeStringUser_initByOther((MemeStringUser_t*)_out, &(_other->user_));
	} break;
	case MemeString_ImplType_view: {
		return MemeStringStack_initByU8bytes(_out, MMS__OBJECT_SIZE,
			MemeString_byteData(_other), MemeString_byteSize(_other));
	} break;
	default: {
		return (MGEC__OPNOTSUPP);
	};
	}

	return 0;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_initByBuffer(
	mmsstk_t* _out, size_t _object_size, MemeBuffer_Const_t _other, MemeInteger_t _offset)
{
	MemeInteger_t length = 0;

	if (((void*)_out == (void*)_other))
		return (MGEC__CANCELED);

	if (_offset < 0)
		_offset = 0;

	length = MemeBuffer_size(_other) - _offset;
	if ((length <= 0)) 
	{
		return MemeStringStack_init(_out, _object_size);
	}
	else if (length <= MMS__GET_SMALL_BUFFER_SIZE) 
	{
		MemeStringSmall_initByU8bytes(
			(MemeStringSmall_t*)_out, MemeBuffer_data(_other) + _offset, length);
		MemeStringSmall_shrinkTailZero((MemeStringSmall_t*)_out);
		return 0;
	}

	switch (MMS__GET_TYPE((mms_t)_other))
	{
	case MemeString_StorageType_medium: {
		int result = 0;
		MemeStringStack_init(_out, MEME_STRING__OBJECT_SIZE);
		result = (int)MemeVariableBuffer_appendWithBytes((mmvb_t)_out,
			MemeBuffer_data(_other) + _offset, length);
		MemeStringMedium_shrinkTailZero((MemeStringMedium_t*)_out);
		return result;
	} break;
	case MemeString_StorageType_user:
	case MemeString_StorageType_large: {

		int result = MemeStringStack_initByOther(_out, _object_size, (mms_const_t)_other);
		if ((result != 0))
			return result;

		MemeStringImpl_setDataOffset(_out, _offset);
		MemeStringImpl_shrinkTailZero(_out);

	} break;
	default: {
		return (MGEC__OPNOTSUPP);
	}
	}

	return 0;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_initByRune(
	mmsstk_t* _out, size_t _object_size, mmint_t _count, MemeRune_t _ch)
{
	if (MemeRune_isValid(&_ch) == 0)
        return (MGEC__INVAL);

	return MemeStringStack_initByU8bytesAndType(
		_out, _object_size, MemeRune_data(&_ch), MemeRune_size(&_ch), MemeString_StorageType_none);
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_initByU8bytes(
	mmsstk_t* _out, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len)
{
	return MemeStringStack_initByU8bytesAndType(
		_out, _object_size, _utf8, _len, MemeString_StorageType_none);
}

MEME_EXTERN_C MEME_API int
MEME_STDCALL MemeStringStack_initWithHexadecimals(
	mmsstk_t* _out, size_t _object_size,
	const MemeByte_t* _interval, MemeInteger_t _ivlen, const uint8_t* _hexs, MemeInteger_t _len)
{
	const static char xdigits[] = "0123456789ABCDEF";
	MemeInteger_t total_len;
	mmvbstk_t vbuf;
	MemeInteger_t result = 0;
	MemeByte_t* index = NULL;
	
	assert(_out  != NULL && MemeStringStack_initWithHexadecimals);

	if ((_hexs == NULL))
		return (MGEC__INVAL);

	if ((_len <= 0)) 
	{
		MemeStringStack_init(_out, _object_size);
		return 0;
	}

	if (_ivlen < 0) {
		_ivlen = _interval ? strlen((const char*)_interval) : 0;
	}
	if (_interval == NULL)
		_ivlen = 0;

	total_len = _len * 2 + (_len - 1) * _ivlen;
	MemeVariableBufferStack_init(&vbuf, MEME_STRING__OBJECT_SIZE);
	result = MemeVariableBuffer_resize((mmvb_t)&vbuf, total_len);
	if (result) {
		MemeVariableBufferStack_unInit(&vbuf, MMS__OBJECT_SIZE);
		return (int)result;
	}

	index = MemeVariableBuffer_dataWithNotConst((mmvb_t)&vbuf);
	for (const uint8_t* inEnd = _hexs + _len;;)
	{
		*index++ = xdigits[*_hexs / 16];
		*index++ = xdigits[*_hexs % 16];

		if (++_hexs < inEnd)
		{
			if (_ivlen) {
				memcpy(index, _interval, _ivlen);
				index += _ivlen;
			}
		}
		else {
			break;
		}
	}

	result = MemeVariableBuffer_releaseToString((mmvb_t)&vbuf, _out, _object_size);
	if (result) {
        MemeVariableBufferStack_unInit(&vbuf, MMS__OBJECT_SIZE);
	}
	return (int)result;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_reset(
	mmsstk_t* _out, size_t _object_size)
{
	mms_t obj = NULL;

	assert(_out);
	assert(_object_size != 0);

	obj = (mms_t)_out;
	switch (MMS__GET_TYPE(obj)) {
	case MemeString_UnsafeStorageType_view:
	case MemeString_ImplType_small:
	{
		MemeStringSmall_clear(&(obj->small_));
	} break;
	case MemeString_ImplType_medium:
	{
		return MemeStringMedium_reset((MemeStringMedium_t*)_out);
	} break;
	case MemeString_ImplType_large:
	{
		return MemeStringLarge_reset((MemeStringLarge_t*)_out);
	};
	case MemeString_ImplType_user:
	{
		return MemeStringUser_reset((MemeStringUser_t*)_out);
	}
	default: {
		return (MGEC__OPNOTSUPP);
	};
	}
	return 0;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_assign(
	mmsstk_t* _s, size_t _object_size, MemeString_Const_t _other)
{
	int result = 0;

	assert(_s != NULL		&& MemeStringStack_assign);
	assert(_other != NULL	&& MemeStringStack_assign);

	if (((void*)_s == (void*)_other))
		return 0;

	switch (MMS__GET_TYPE(_other))
	{
	case MemeString_ImplType_small:
	case MemeString_ImplType_medium:
	case MemeString_ImplType_large:
	case MemeString_ImplType_user:
	case MemeString_ImplType_view:
		break;
	default: {
		return (MGEC__OPNOTSUPP);
	};
	}

	result = MemeStringStack_unInit(_s, sizeof(mmsstk_t));
	if ((result != 0))
		return result;

	switch (MMS__GET_TYPE(_other)) 
	{
	case MemeString_ImplType_small: {
		memcpy(_s, &(_other->small_), MEME_STRING__OBJECT_SIZE);
	} break;
	case MemeString_ImplType_medium: {
		result = MemeStringMedium_initWithCapacity((MemeStringMedium_t*)_s, MemeString_byteSize(_other));
		if ((result != 0))
			return result;
		MemeStringMedium_assign((MemeStringMedium_t*)_s, 
			MemeString_byteData(_other), MemeString_byteSize(_other));
	} break;
	case MemeString_ImplType_large: {
		MemeStringLarge_RefCount_increment(_other->large_.ref_);
		memcpy(_s, &(_other->user_), MEME_STRING__OBJECT_SIZE);
	} break;
	case MemeString_ImplType_user: {
		MemeStringUser_RefCount_increment(_other->user_.ref_);
		memcpy(_s, &(_other->user_), MEME_STRING__OBJECT_SIZE);
	} break;
	case MemeString_ImplType_view: {
		memcpy(_s, &(_other->viewUnsafe_), MEME_STRING__OBJECT_SIZE);
	} break;
	default: {
		return (MGEC__OPNOTSUPP);
	};
	}
	return 0;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_assignByU8bytes(
	mmsstk_t* _s, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len)
{
	int result = 0;

	assert(_s != NULL    && MemeStringStack_assignByU8bytes);
    
	if ((MemeString_byteData((mms_t)_s) == _utf8))
		return 0;

	result = MemeStringStack_unInit(_s, sizeof(mmsstk_t));
	if ((result != 0))
		return result;
	result = MemeStringStack_initByU8bytes(_s, sizeof(mmsstk_t), _utf8, _len);
	return result;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_assignByBuffer(
	mmsstk_t* _s, size_t _object_size, MemeBuffer_Const_t _other, MemeInteger_t _offset)
{
	int result = 0;

	assert(_s != NULL		&& MemeStringStack_assignByBuffer);
	assert(_other != NULL   && MemeStringStack_assignByBuffer);

	if (((void*)_s == (void*)_other))
		return 0;

	result = MemeStringStack_unInit(_s, sizeof(mmsstk_t));
	if ((result != 0))
		return result;
	return MemeStringStack_initByBuffer(_s, sizeof(mmsstk_t), _other, _offset);
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL
MemeStringStack_mid(
	const mmsstk_t* _s, size_t _object_size, MemeInteger_t _offset, MemeInteger_t _len)
{
	mmsstk_t out;
    mms_t obj = NULL;
	MemeInteger_t srcSize = 0;

    assert(_s != NULL && MemeStringStack_mid);
    assert(_object_size != 0 && MemeStringStack_mid);

    obj = (mms_t)_s;
    srcSize = MemeString_byteSize(obj);
    if ((_offset > srcSize))
        return MemeStringStack_getInitObject(_object_size);
	
    if ((_len < 0))
        _len = srcSize - _offset;
    else if ((_len > srcSize - _offset))
        _len = srcSize - _offset;

    if (MemeString_isSharedStorageTypes(obj) && _len + _offset == srcSize)
    {
		int eno = MemeStringStack_initByOther(&out, _object_size, obj);
		if (eno)
			return MemeStringStack_getInitObject(_object_size);
		MemeStringImpl_setDataOffset(&out, _offset);
    }
	else if (MMS__GET_TYPE(obj) == MemeString_ImplType_view) 
	{
        MemeStringViewUnsafeStack_init(&out, _object_size,
            MemeString_byteData(obj) + _offset, _len);
	}
	else {
        int eno = MemeStringStack_initByU8bytes(&out, _object_size,
            MemeString_byteData(obj) + _offset, _len);
        if (eno)
            return MemeStringStack_getInitObject(_object_size);
	}
    return out;
}

MEME_EXTERN_C MEME_API mmstrstk_t MEME_STDCALL MemeStringStack_concat(
	const mmstrstk_t* _s, size_t _object_size, const mmstrstk_t* _other)
{
    mmstrstk_t out;
    mmint_t result  = 0;
    mmint_t rhsSize = 0;
    mmint_t lhsSize = 0;
	
    assert(_s != NULL && MemeStringStack_concat);
    assert(_object_size != 0 && MemeStringStack_concat);
	assert(_other != NULL && MemeStringStack_concat);
	
    lhsSize = MemeString_byteSize((mmstr_const_t)_s);
    rhsSize = MemeString_byteSize((mmstr_const_t)_other);
	
    if (lhsSize + rhsSize == 0)
        return MemeStringStack_getInitObject(_object_size);

	result = MemeStringMedium_initWithCapacity((MemeStringMedium_t*)&out, 
		lhsSize + rhsSize);
    if (result)
        return MemeStringStack_getInitObject(_object_size);
	
	result = MemeVariableBuffer_appendWithBytes(
		(mmvb_t)&out, MemeString_byteData((mmstr_const_t)_s), lhsSize);
    if (result)
        return MemeStringStack_getInitObject(_object_size);
	
	result = MemeVariableBuffer_appendWithBytes(
		(mmvb_t)&out, MemeString_byteData((mmstr_const_t)_other), rhsSize);
    if (result)
        return MemeStringStack_getInitObject(_object_size);

    return out;
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL
MemeStringStack_toEnUpper(
	const mmsstk_t* _s, size_t _object_size)
{
	int result = 0;
	mmsstk_t stack;
	mms_t s = (mms_t)_s;
	MemeByte_t* it  = NULL;
	MemeByte_t* end = NULL;

	assert(_s != NULL && MemeStringStack_toEnUpper);

	result = MemeStringStack_initByU8bytes(&stack, sizeof(stack),
		MemeString_byteData(s), MemeString_byteSize(s));
	if (result) {
		MemeStringStack_init(&stack, sizeof(stack));
		return stack;
	}

	it  = MemeStringImpl_forcedData(&stack);
	end = it + MemeString_byteSize((mms_t)&stack);

	for (; it != end; ++it)
		*it = (MemeByte_t)toupper(*it);
	
	return stack;
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL
MemeStringStack_toEnLower(
	const mmsstk_t* _s, size_t _object_size)
{
	int result = 0;
	mmsstk_t stack;
	mms_t s = (mms_t)_s;
	MemeByte_t* it  = NULL;
	MemeByte_t* end = NULL;

	assert(_s != NULL && MemeStringStack_toEnLower);

	result = MemeStringStack_initByU8bytes(&stack, sizeof(stack),
		MemeString_byteData(s), MemeString_byteSize(s));
	if (result) {
		MemeStringStack_init(&stack, sizeof(stack));
		return stack;
	}

	it = MemeStringImpl_forcedData(&stack);
	end = it + MemeString_byteSize((mms_t)&stack);

	for (; it != end; ++it)
		*it = (MemeByte_t)tolower(*it);

	return stack;
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL
MemeStringStack_trimSpace(const mmsstk_t* _s, size_t _object_size)
{
	int result = 0;
	mmsstk_t stack;
	mms_t s = (mms_t)_s;
	const MemeByte_t* it = NULL;
	const MemeByte_t* end = NULL;

	assert(_s != NULL && MemeStringStack_trimSpace);

	it = MemeString_byteData(s);
	end = it + MemeString_byteSize(s);

	for (; it != end; ++it)
		if (!isspace(*it))
			break;
	
	if (it != end)
		--end;
	for (; it - 1 != end; --end)
        if (!isspace(*end))
            break;
    
    stack = MemeStringStack_mid(_s, _object_size, it - MemeString_byteData(s), end - it + 1);
    return stack;
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL
MemeStringStack_trimLeftSpace(const mmsstk_t* _s, size_t _object_size)
{
    int result = 0;
	mmsstk_t stack;
    mms_t s  = (mms_t)_s;
    const MemeByte_t* it  = NULL;
	const MemeByte_t* begin = NULL;
	const MemeByte_t* end = NULL;

    assert(_s != NULL && MemeStringStack_trimLeftSpace);

	begin = it = MemeString_byteData(s);
    end = it + MemeString_byteSize(s);

    for (; it != end; ++it)
        if (!isspace(*it))
            break;

    stack = MemeStringStack_mid(_s, _object_size, it - begin, -1);
    return stack;
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL
MemeStringStack_trimRightSpace(const mmsstk_t* _s, size_t _object_size)
{
    int result = 0;
	mmsstk_t stack;
    mms_t s = (mms_t)_s;
	const MemeByte_t* it = NULL;
	const MemeByte_t* end = NULL;

    assert(_s != NULL && MemeStringStack_trimRightSpace);

    it = MemeString_byteData(s);
    end = it + MemeString_byteSize(s);

    if (it != end)
        --end;
    for (; it - 1 != end; --end)
        if (!isspace(*end))
            break;
    
    stack = MemeStringStack_mid(_s, _object_size, 0, end - it + 1);
    return stack;
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL
MemeStringStack_trimByCuts(
	const mmsstk_t* _s, size_t _object_size, const char* _cuts, MemeInteger_t _cuts_len)
{
    int result = 0;
	mmsstk_t stack;
    mms_t s = (mms_t)_s;
    const MemeByte_t* it = NULL;
    const MemeByte_t* end = NULL;

    assert(_s != NULL && MemeStringStack_trimByCuts);

    it = MemeString_byteData(s);
    end = it + MemeString_byteSize(s);

    for (; it != end; ++it)
        if (!memchr(_cuts, *it, _cuts_len))
            break;

    if (it != end)
        --end;
    for (; it - 1 != end; --end)
        if (!memchr(_cuts, *it, _cuts_len))
            break;

    stack = MemeStringStack_mid(_s, _object_size, it - MemeString_byteData(s), end - it + 1);
    return stack;
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL
MemeStringStack_trimByCondByteFunc(
	const mmsstk_t* _s, size_t _object_size,
	MemeString_MatchCondByteFunc_t* _cond_func, void* _user_data)
{
    int result = 0;
	mmsstk_t stack;
    mms_t s = (mms_t)_s;
    const MemeByte_t* it = NULL;
    const MemeByte_t* end = NULL;

    assert(_s != NULL && MemeStringStack_trimByCondByteFunc);

    it = MemeString_byteData(s);
    end = it + MemeString_byteSize(s);

    for (; it != end; ++it)
        if (!_cond_func(*it, _user_data))
            break;

    if (it != end)
        --end;
    for (; it - 1 != end; --end)
        if (!_cond_func(*end, _user_data))
            break;

    stack = MemeStringStack_mid(_s, _object_size, it - MemeString_byteData(s), end - it + 1);
    return stack;
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL MemeStringStack_getRepeat(
	size_t _object_size, mmint_t _count, const char* _s, mmint_t _len)
{
	mmint_t result = 0;
    mmsstk_t stack;
    mmvbstk_t vb;
    assert(_s != NULL && MemeStringStack_getRepeat);
	
	result = MemeVariableBufferStack_init(&vb, _object_size);
    if (result) {
        MemeStringStack_init(&stack, _object_size);
		return stack;
	}
	
	if (_len < 0)
        _len = strlen(_s);

    for (; _count > 0; --_count) {
		result = MemeVariableBuffer_appendWithBytes((mmvb_t)&vb, (const mmbyte_t*)_s, _len);
		if (result) {
			MemeStringStack_init(&stack, _object_size);
			return stack;
		}
	}

	result = MemeVariableBuffer_releaseToString((mmvb_t)&vb, &stack, _object_size);
	if (result) {
		MemeStringStack_init(&stack, _object_size);
		return stack;
	}
    return stack;
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL MemeStringStack_replace(
	const mmsstk_t* _s, size_t _object_size, 
	const char* _from, mmint_t _from_len, 
	const char* _to, mmint_t _to_len, mmint_t _max_count)
{
	mmint_t result = 0;
    mmsstk_t stack;
	mmvbstk_t vb;
    mms_t s = (mms_t)_s;
    const mmbyte_t* it   = NULL;
    const mmbyte_t* end  = NULL;
    const mmbyte_t* from = NULL;
    const mmbyte_t* to   = NULL;
    const mmbyte_t* from_end = NULL;
    const mmbyte_t* to_end = NULL;
	mmint_t count = 0;
	mmint_t size = 0;
	//mmint_t pos = 0;

    assert(_s != NULL && MemeStringStack_replace);

    if (_from_len < 0)
        _from_len = strlen(_from);
    if (_to_len < 0)
        _to_len = strlen(_to);

    it = MemeString_byteData(s);
    end = it + MemeString_byteSize(s);
    from = (const MemeByte_t*)_from;
    to = (const MemeByte_t*)_to;
    from_end = from + _from_len;
    to_end = to + _to_len;

    for (; it != end; ++it) {
		
        if (it + _from_len > end)
            break;
		
        if (memcmp(it, from, _from_len) == 0)
        {
            ++count;
            if (_max_count > 0 && count >= _max_count)
                break;
            it += _from_len - 1;
        }
    }
	MemeVariableBufferStack_init(&vb, _object_size);

    size = MemeString_byteSize(s) + (_to_len - _from_len) * count;
    it = MemeString_byteData(s);
    end = it + MemeString_byteSize(s);
    
	for (; it != end; ++it) {
		
        if (it + _from_len > end)
            break;
		
        if (memcmp(it, from, _from_len) == 0)
        {
			result = MemeVariableBuffer_appendWithBytes((mmvb_t)&vb, to, _to_len);
			if (result) {
                MemeStringStack_init(&stack, _object_size);
                return stack;
			}
			
            ++count;
            if (_max_count > 0 && count >= _max_count)
                break;
            it += _from_len - 1;
        }
		else {
			result = MemeVariableBuffer_appendWithBytes((mmvb_t)&vb, it, 1);
			if (result) {
				MemeStringStack_init(&stack, _object_size);
				return stack;
			}

		}
    }
	
    if (it != end) {
		result = MemeVariableBuffer_appendWithBytes((mmvb_t)&vb, it, end - it);
		if (result) {
			MemeStringStack_init(&stack, _object_size);
			return stack;
		}
	}
	
	result = MemeVariableBuffer_releaseToString((mmvb_t)&vb, &stack, _object_size);
    if (result) {
        MemeStringStack_init(&stack, _object_size);
        return stack;
    }
    return stack;
}

MEME_EXTERN_C MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_toValidUtf8(const mmsstk_t* _s, size_t _object_size)
{
	MemeInteger_t pos = 0;

    assert(_s != NULL && MemeStringStack_toValidUtf8);
    pos = mmutf_u8valid(MemeString_byteData((mms_t)_s), MemeString_byteSize((mms_t)_s));
	if (pos == MemeString_byteSize((mms_t)_s))
	{
		mmsstk_t s;
        MemeStringStack_initByOther(&s, _object_size, (mms_t)_s);
        return s;
	}
    return MemeStringStack_mid(_s, _object_size, 0, pos);
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL MemeStringStack_mappingConvert(
	const mmsstk_t* _s, size_t _object_size, MemeString_MappingConvertFunc_t _mapping_fn, void* _user_data)
{
	mms_const_t s = (mms_const_t)_s;
	//MemeInteger_t pos  = 0;
    MemeInteger_t size  = 0;
	MemeInteger_t result = 0;
    const MemeByte_t* data = NULL;
    mmsstk_t stack;
    mmvbstk_t vb;

    assert(_s != NULL && MemeStringStack_mappingConvert);
	
    MemeVariableBufferStack_init(&vb, MMS__OBJECT_SIZE);
	
    size = MemeString_byteSize(s);
    data = MemeString_byteData(s);
    for (MemeInteger_t pos = 0; pos < size; )
	{
		int len = mmutf_u8rune_char_size(data[pos]);
        if (len < 0)
            break;
		MemeRune_t rune;
        MemeRune_initByUtf8Bytes(&rune, data + pos, len);
        pos += len;
		result = _mapping_fn(&rune, _user_data);
		if (result < 0)
			continue;
		if (!MemeRune_isValid(&rune))
			continue;
		result = MemeVariableBuffer_appendWithBytes((mmvb_t)&vb, MemeRune_data(&rune), MemeRune_size(&rune));
		if (result < 0) {
			MemeVariableBufferStack_unInit(&vb, MMS__OBJECT_SIZE);
			return MemeStringStack_getInitObject(_object_size);
		}
	}
	
	result = MemeVariableBuffer_releaseToString((mmvb_t)&vb, &stack, _object_size);
	if (result < 0) {
		MemeVariableBufferStack_unInit(&vb, MMS__OBJECT_SIZE);
		return MemeStringStack_getInitObject(_object_size);
	}
    return stack;
}

MEME_EXTERN_C MEME_API MemeStringStack_t
MemeStringStack_vformatInCstyle(
	size_t _object_size, const char* _format, MemeInteger_t _size_limit, va_list _args)
{
    return MemeStringStack_vformatInCstyle_v2(_object_size, _size_limit, _format, _args);
}

MEME_EXTERN_C MEME_API MemeStringStack_t
MemeStringStack_formatInCstyle(size_t _object_size, const char* _format, MemeInteger_t _size_limit, ...)
{
    va_list args;
    MemeStringStack_t out;

    va_start(args, _size_limit);
    out = MemeStringStack_vformatInCstyle(_object_size, _format, _size_limit, args);
    va_end(args);

    return out;
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL
MemeStringStack_vformatInCstyle_v2(
	size_t _object_size,
	MemeInteger_t _size_limit,
	MEGO_SYMBOL__MSVC_FORMAT_STRING(const char* _format),
	va_list _args)
{
	MemeInteger_t result = 0;
	mmvbstk_t vbuf;
	mmsstk_t out;
	MemeByte_t* data = NULL;
	MemeInteger_t len = 0;
    va_list calc_args;

	//assert(_format != NULL && MemeStringStack_vformatInCstyle);

	if ((_format == NULL))
		return MemeStringStack_getInitObject(_object_size);

	va_copy(calc_args, _args);
	len = vsnprintf(NULL, 0, _format, calc_args);
	va_end(calc_args);
	if ((len <= 0))
		return MemeStringStack_getInitObject(_object_size);

	if (_size_limit > 0 && len > _size_limit)
		len = _size_limit;

	MemeVariableBufferStack_init(&vbuf, MMS__OBJECT_SIZE);
	result = MemeVariableBuffer_resize((mmvb_t)&vbuf, len + 1);
	if (result) {
		MemeVariableBufferStack_unInit(&vbuf, MMS__OBJECT_SIZE);
		return MemeStringStack_getInitObject(_object_size);
	}

	data = MemeVariableBuffer_dataWithNotConst((mmvb_t)&vbuf);

	len = vsnprintf((char*)data, len + 1, _format, _args);
	if (len <= 0) {
		MemeVariableBufferStack_unInit(&vbuf, MMS__OBJECT_SIZE);
		return MemeStringStack_getInitObject(_object_size);
	}
	
	result = MemeVariableBuffer_resize((mmvb_t)&vbuf, len);
    if (result) {
		MemeVariableBufferStack_unInit(&vbuf, MMS__OBJECT_SIZE);
        return MemeStringStack_getInitObject(_object_size);
    }

	result = MemeVariableBuffer_releaseToString((mmvb_t)&vbuf, &out, _object_size);
	if (result) {
		MemeVariableBufferStack_unInit(&vbuf, MMS__OBJECT_SIZE);
		return MemeStringStack_getInitObject(_object_size);
	}

	return out;
}

MEME_EXTERN_C MEME_API mmsstk_t
MemeStringStack_formatInCstyle_v2(
	size_t _object_size,
	MemeInteger_t _size_limit,
	MEGO_SYMBOL__MSVC_FORMAT_STRING(const char* _format),
	...)
{
    va_list args;
	mmsstk_t out;

    va_start(args, _format);
    out = MemeStringStack_vformatInCstyle_v2(_object_size, _size_limit, _format, args);
    va_end(args);

    return out;
}


MEME_EXTERN_C MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringViewUnsafeStack_init(MemeStringStack_t* _s, size_t _object_size,
	const uint8_t* _buf, MemeInteger_t _len)
{
	MemeStringViewUnsafe_t* p = (MemeStringViewUnsafe_t*)_s;

	assert(_s != NULL		&& MemeStringViewUnsafeStack_init);

    if ((_buf == NULL))
        return (MGEC__INVAL);

	if (_len < 0)
		_len = strlen((const char*)_buf);

	//if (_len <= MMS__GET_SMALL_BUFFER_SIZE)
	//	return MemeStringSmall_initByU8bytes((MemeStringSmall_t*)_s, _buf, _len);

	p->data_   = _buf;
	p->size_   = _len;
	p->_res_   = 0;
	p->type_   = MemeString_ImplType_view;
	return 0;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringViewUnsafeStack_initByOther(
	MemeStringStack_t* _s, size_t _object_size, const MemeStringStack_t* _other)
{
	assert(_s != NULL		&& MemeStringViewUnsafeStack_initByOther);
	assert(_other != NULL	&& MemeStringViewUnsafeStack_initByOther);

	if (MMS__GET_TYPE((mms_t)_other) == MemeString_ImplType_view)
	{
		memcpy(_s, _other, MEME_STRING__OBJECT_SIZE);
		return 0;
	}
	else if (MemeString_isSharedStorageTypes((mms_t)_other) == 1)
	{
		return MemeStringStack_initByOther(_s, _object_size, (mms_t)_other);
	}
	else {
        return MemeStringViewUnsafeStack_init(
			_s, _object_size, 
			MemeString_byteData((mms_t)_other), 
			MemeString_byteSize((mms_t)_other));
	}
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringViewUnsafeStack_assignByOther(
	MemeStringStack_t* _s, size_t _object_size, const MemeStringStack_t* _other)
{
	assert(_s != NULL		&& MemeStringViewUnsafeStack_assignByOther);
	assert(_other != NULL	&& MemeStringViewUnsafeStack_assignByOther);

	if (MMS__GET_TYPE((mms_t)_other) == MemeString_ImplType_view)
	{
		int result = MemeStringStack_unInit(_s, sizeof(mmsstk_t));
		if (result)
			return result;
		memcpy(_s, _other, MEME_STRING__OBJECT_SIZE);
		return 0;
	}
	else if (MemeString_isSharedStorageTypes((mms_t)_other) == 1)
	{
		return MemeStringStack_assign(_s, _object_size, (mms_t)_other);
	}
	else {
		int result = MemeStringStack_unInit(_s, sizeof(mmsstk_t));
		if (result)
			return result;
        return MemeStringViewUnsafeStack_init(
            _s, _object_size,
            MemeString_byteData((mms_t)_other),
            MemeString_byteSize((mms_t)_other));
	}
}

MEME_EXTERN_C MEME_API MemeInteger_t MEME_STDCALL
MemeStringViewUnsafe_split(
	MemeString_Const_t _s, const char* _key, MemeInteger_t _key_len,
	MemeFlag_SplitBehavior_t _behavior, MemeFlag_CaseSensitivity_t _sensitivity,
	MemeStringStack_t* _out, MemeInteger_t* _out_count, MemeInteger_t* _search_index)
{
	MemeInteger_t last_index = (_search_index == NULL ? 0 : *_search_index);
	MemeInteger_t curr_index = -1;
	MemeInteger_t output_index = 0;

	assert(_s != NULL && MemeString_split);
	assert(_out != NULL && MemeString_split);
	assert(_out_count != NULL && MemeString_split);

	if (*_out_count < 1)
		return (MGEC__INVAL);
	if (_key_len == -1)
		_key_len = strlen(_key);

	while (output_index < *_out_count)
	{
		curr_index =
			MemeString_indexOfWithUtf8bytes(
				_s, last_index, (const MemeByte_t*)_key, _key_len, _sensitivity);
		if (curr_index == -1) {
			if (last_index < MemeString_byteSize(_s))
			{
				MemeStringViewUnsafeStack_init(_out + output_index,
					MEME_STRING__OBJECT_SIZE, MemeString_byteData(_s) + last_index,
					MemeString_byteSize(_s) - last_index);
				*_out_count = output_index + 1;
			}
			else if (_behavior == MemeFlag_KeepEmptyParts)
			{
				MemeStringStack_init(_out + output_index,
					MEME_STRING__OBJECT_SIZE);
				*_out_count = output_index + 1;
			}
			else {
				*_out_count = output_index;
			}
			if (_search_index)
				*_search_index = -1;
			return 0;
		}

		if ((curr_index - last_index) == 0 && _behavior == MemeFlag_SkipEmptyParts)
		{
			last_index += _key_len;
			continue;
		}

		MemeStringViewUnsafeStack_init(_out + output_index,
			MEME_STRING__OBJECT_SIZE, MemeString_byteData(_s) + last_index, curr_index - last_index);
		last_index = curr_index + _key_len;
		++output_index;
	}
	if (_behavior == MemeFlag_SkipEmptyParts)
	{
		curr_index =
			MemeString_indexOfWithUtf8bytes(
				_s, last_index, (const MemeByte_t*)_key, _key_len, _sensitivity);
		if ((curr_index - last_index) == 0)
		{
			last_index += _key_len;
		}
	}

	*_out_count = output_index;
	if (last_index < MemeString_byteSize(_s))
	{
		if (_search_index)
			*_search_index = last_index;
		return 0;
	}
	else {
		if (_search_index)
			*_search_index = -1;
		return 0;
	}
}

MEME_EXTERN_C MEME_API int MEME_STDCALL mmsstk_init(mmsstk_t* _out, size_t _object_size)
{
    if (_out == NULL)
        return (MGEC__INVAL);
    return MemeStringStack_init(_out, _object_size);
}

MEME_EXTERN_C MEME_API int MEME_STDCALL mmsstk_init_by_other(mmsstk_t* _out, size_t _object_size, mms_const_t _other)
{
    if (_out == NULL)
        return (MGEC__INVAL);
	if (_other == NULL) 
        return MemeStringStack_init(_out, _object_size);
    return MemeStringStack_initByOther(_out, _object_size, _other);
}

MEME_EXTERN_C MEME_API int MEME_STDCALL mmsstk_init_by_buf(
	mmsstk_t* _out, size_t _object_size, mmbuf_const_t _other, mmint_t _offset)
{
    if (_out == NULL)
        return (MGEC__INVAL);
    if (_other == NULL)
		return MemeStringStack_init(_out, _object_size);
    return MemeStringStack_initByBuffer(_out, _object_size, _other, _offset);
}

MEME_EXTERN_C MEME_API int MEME_STDCALL mmsstk_init_by_utf8(
	mmsstk_t* _out, size_t _object_size, const mmbyte_t* _utf8, mmint_t _len)
{
    if (_out == NULL)
        return (MGEC__INVAL);
    if (_utf8 == NULL)
		return MemeStringStack_init(_out, _object_size);
    return MemeStringStack_initByU8bytes(_out, _object_size, _utf8, _len);
}

MEME_EXTERN_C MEME_API int MEME_STDCALL mmsstk_init_by_utf8_v2(
	mmsstk_t* _out, size_t _object_size,
	const mmbyte_t* _utf8, mmint_t _len, MemeString_Storage_t _suggest)
{
    if (_out == NULL)
        return (MGEC__INVAL);
    if (_utf8 == NULL)
        return MemeStringStack_init(_out, _object_size);
    return MemeStringStack_initByU8bytesAndType(_out, _object_size, _utf8, _len, _suggest);
}

MEME_EXTERN_C MEME_API int MEME_STDCALL mmsstk_init_by_hexs(
	mmsstk_t* _out, size_t _object_size, const mmbyte_t* _interval, mmint_t _ivlen, const uint8_t* _hexs, mmint_t _len)
{
    if (_out == NULL)
        return (MGEC__INVAL);
    if (_hexs == NULL)
        return MemeStringStack_init(_out, _object_size);
    return MemeStringStack_initWithHexadecimals(_out, _object_size, _interval, _ivlen, _hexs, _len);
}

MEME_EXTERN_C MEME_API int MEME_STDCALL mmsstk_init_by_user(
	mmsstk_t* _out, size_t _object_size, void* _user_data,
	MemeString_UserObjectDestruct_t* _destruct_fn, 
	MemeString_UserObjectData_t* _data_fn, 
	MemeString_UserObjectSize_t* _size_fn)
{
    if (_out == NULL)
        return (MGEC__INVAL);
    if (_user_data == NULL
	 || _destruct_fn == NULL
	 || _data_fn == NULL
	 || _size_fn == NULL)
        return MemeStringStack_init(_out, _object_size);
    return MemeStringStack_initTakeOverUserObject(
		_out, _object_size, _user_data, _destruct_fn, _data_fn, _size_fn);
}

MEME_EXTERN_C MEME_API int MEME_STDCALL mmsstk_uninit(mmsstk_t* _out, size_t _object_size)
{
    if (_out == NULL)
        return (MGEC__INVAL);
    return MemeStringStack_unInit(_out, _object_size);
}

MEME_EXTERN_C MEME_API int MEME_STDCALL mmsstk_reset(mmsstk_t* _out, size_t _object_size)
{
    if (_out == NULL)
        return (MGEC__INVAL);
    return MemeStringStack_reset(_out, _object_size);
}

MEME_EXTERN_C MEME_API int MEME_STDCALL mms_assign(mms_t _s, mms_const_t _other)
{
    if (_s == NULL)
        return (MGEC__INVAL);
    if (_other == NULL)
        return MemeStringStack_reset((mmsstk_t*)_s, sizeof(*_s));
    return MemeStringStack_assign((mmsstk_t*)_s, sizeof(*_s), _other);
}

MEME_EXTERN_C MEME_API int MEME_STDCALL mms_assign_by_utf8(
	mms_t _s, const mmbyte_t* _utf8, mmint_t _len)
{
    if (_s == NULL)
        return (MGEC__INVAL);
    if (_utf8 == NULL)
        return MemeStringStack_reset((mmsstk_t*)_s, sizeof(*_s));
    return MemeStringStack_assignByU8bytes((mmsstk_t*)_s, sizeof(*_s), _utf8, _len);
}

MEME_EXTERN_C MEME_API int MEME_STDCALL mms_assign_by_buf(
	mms_t _out, mmbuf_const_t _other, mmint_t _offset)
{
    if (_out == NULL)
        return (MGEC__INVAL);
    if (_other == NULL)
        return MemeStringStack_reset((mmsstk_t*)_out, sizeof(*_out));
    return MemeStringStack_assignByBuffer((mmsstk_t*)_out, sizeof(*_out), _other, _offset);
}
