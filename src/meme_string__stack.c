
#include "mego/predef/symbol/likely.h"
#include "meme/string.h"
#include "meme/buffer.h"
#include "meme/impl/string.h"
#include "meme/impl/string_p__user.h"
#include "meme/impl/string_p__small.h"
#include "meme/impl/string_p__medium.h"
#include "meme/impl/string_p__large.h"
#include "meme/variable_buffer.h"

#include "meme/unsafe/view.h"

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_init(
	MemeStringStack_t* _out, size_t _object_size)
{
	MemeString_t obj = NULL;

	assert(_out);
	assert(_object_size != 0);

	//memset(_out, 0, _object_size);

	if (_object_size > sizeof(struct _MemeString_t))
		return MEME_ENO__POSIX_OFFSET(ENOTSUP);

	obj = (MemeString_t)_out;
	obj->small_.buffer_[0] = '\0';
	obj->small_.type_ = MemeString_ImplType_small;
	obj->small_.capacity_ = MEME_STRING__GET_SMALL_BUFFER_SIZE;
	return 0;
}

MEME_API MemeStringStack_t MEME_STDCALL MemeStringStack_getInitObject(size_t _object_size)
{
	MemeStringStack_t stack;
	MemeStringStack_init(&stack, _object_size);
	return stack;
}

MEME_API int MEME_STDCALL MemeStringStack_initTakeOverUserObject(
	MemeStringStack_t* _out, size_t _object_size, 
	void* _user_data, 
	MemeString_UserObjectDestruct_t* _destruct_fn, 
	MemeString_UserObjectData_t* _data_fn, 
	MemeString_UserObjectSize_t* _size_fn)
{
	size_t len = 0;
	int result = 0;

	assert(_out != NULL			&& "function is" && MemeStringStack_initTakeOverUserObject);
	assert(_user_data != NULL	&& "function is" && MemeStringStack_initTakeOverUserObject);
	assert(_destruct_fn != NULL && "function is" && MemeStringStack_initTakeOverUserObject);
	assert(_data_fn != NULL		&& "function is" && MemeStringStack_initTakeOverUserObject);
	assert(_size_fn != NULL		&& "function is" && MemeStringStack_initTakeOverUserObject);

	len = _size_fn(_user_data);
	if (len <= MEME_STRING__GET_SMALL_BUFFER_SIZE)
	{
		result = MemeStringStack_initByU8bytes(_out, _object_size, _data_fn(_user_data), len);
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

MEME_API int MEME_STDCALL MemeStringStack_unInit(MemeStringStack_t* _out, size_t _object_size)
{
	MemeString_t obj = (MemeString_t)_out;

	assert(_out && MemeStringStack_unInit);
	assert(_object_size != 0 && MemeStringStack_unInit);

	switch (MEME_STRING__GET_TYPE(obj)) {
	case MemeString_ImplType_user:
	{
		return MemeStringUser_unInit((MemeStringUser_t*)_out);
	};
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
	default: {
		return MEME_ENO__POSIX_OFFSET(ENOTSUP);
	};
	}
	return 0;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_initByU8bytesAndType(
	MemeStringStack_t* _out, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len,
	MemeString_Storage_t _suggest)
{
	MemeString_Storage_t type;
	MemeInteger_t len = (_len <= -1 ? strlen(_utf8): _len);

	assert(_out);
	assert(_object_size != 0);

	type = MemeStringImpl_initSuggestType(len, _suggest);
	switch (type) {
	case MemeString_StorageType_large: {
		return MemeStringLarge_initByU8bytes((MemeStringLarge_t*)_out, _utf8, len, NULL, NULL, 0, 0);
	} break;
	case MemeString_StorageType_medium: {
		// TO_DO
	} break;
	case MemeString_StorageType_small: {
		return MemeStringSmall_initByU8bytes((MemeStringSmall_t*)_out, _utf8, len);
	};
	//case MemeString_StorageType_viewUnsafe: {
	//	// TO_DO
	//} break;
	}

	if (MemeString_StorageType_user < type)
	{
		// TO_DO
	}

	return MemeStringLarge_initByU8bytes((MemeStringLarge_t*)_out, _utf8, len, NULL, NULL, 0, 0);
}

//MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_initByOtherAndType(
//	MemeStringStack_t* _out, size_t _object_size,
//	MemeString_Const_t _other, MemeString_Storage_t _suggest)
//{
//
//	assert(_out);
//	assert(_object_size != 0);
//
//	switch (_suggest) {
//	case MemeString_ImplType_large:
//	{
//	} break;
//
//	default: {
//
//	};
//	}
//	return 0;
//}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_initByOther(
	MemeStringStack_t* _out, size_t _object_size, MemeString_Const_t _other)
{

	assert(_out);
	assert(_other);
	assert(_object_size != 0);

	if (MEGO_SYMBOL__UNLIKELY((void*)_out == (void*)_other))
		return MEME_ENO__POSIX_OFFSET(ECANCELED);

	switch (MEME_STRING__GET_TYPE(_other)) {
	case MemeString_StorageType_small: {
		memcpy(_out, _other, MEME_STRING__OBJECT_SIZE);
	} break;
	case MemeString_StorageType_medium: {
		MemeStringStack_init(_out, MEME_STRING__OBJECT_SIZE);
		return (int)MemeVariableBuffer_appendWithBytes((MemeVariableBuffer_t)_out,
			MemeString_cStr(_other), MemeString_byteSize(_other));
	};
	case MemeString_StorageType_large: {
		return MemeStringLarge_initByOther((MemeStringLarge_t*)_out, &(_other->large_));
	};
	case MemeString_StorageType_user: {
		return MemeStringUser_initByOther((MemeStringUser_t*)_out, &(_other->user_));
	} break;
	default: {
		return MEME_ENO__POSIX_OFFSET(ENOTSUP);
	};
	}

	return 0;
}

MEME_API int MEME_STDCALL MemeStringStack_initByBuffer(
	MemeStringStack_t* _out, size_t _object_size, MemeBuffer_Const_t _other, MemeInteger_t _offset)
{
	MemeInteger_t length = 0;

	if (MEGO_SYMBOL__UNLIKELY((void*)_out == (void*)_other))
		return MEME_ENO__POSIX_OFFSET(ECANCELED);

	if (MEGO_SYMBOL__UNLIKELY(_offset < 0))
		_offset = 0;

	length = MemeBuffer_size(_other) - _offset;
	if (MEGO_SYMBOL__UNLIKELY(length <= 0)) 
	{
		return MemeStringStack_init(_out, _object_size);
	}
	else if (length <= MEME_STRING__GET_SMALL_BUFFER_SIZE) 
	{
		MemeStringSmall_initByU8bytes(
			(MemeStringSmall_t*)_out, MemeBuffer_data(_other) + _offset, length);
		MemeStringSmall_shrinkTailZero((MemeStringSmall_t*)_out);
		return 0;
	}

	switch (MEME_STRING__GET_TYPE((MemeString_t)_other))
	{
	case MemeString_StorageType_medium: {
		int result = 0;
		MemeStringStack_init(_out, MEME_STRING__OBJECT_SIZE);
		result = (int)MemeVariableBuffer_appendWithBytes((MemeVariableBuffer_t)_out,
			MemeBuffer_data(_other) + _offset, length);
		MemeStringMedium_shrinkTailZero((MemeStringMedium_t*)_out);
		return result;
	} break;
	case MemeString_StorageType_user:
	case MemeString_StorageType_large: {

		int result = MemeStringStack_initByOther(_out, _object_size, (MemeString_Const_t)_other);
		if (MEGO_SYMBOL__UNLIKELY(result != 0))
			return result;

		MemeStringImpl_setDataOffset(_out, _offset);
		MemeStringImpl_shrinkTailZero(_out);

	} break;
	default: {
		return MEME_ENO__POSIX_OFFSET(ENOTSUP);
	}
	}

	return 0;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_initByU8bytes(
	MemeStringStack_t* _out, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len)
{
	return MemeStringStack_initByU8bytesAndType(
		_out, _object_size, _utf8, _len, MemeString_StorageType_none);
}

MEME_API int 
MEME_STDCALL MemeStringStack_initWithHexadecimals(
	MemeStringStack_t* _out, size_t _object_size,
	const MemeByte_t* _interval, MemeInteger_t _ivlen, const uint8_t* _hexs, MemeInteger_t _len)
{
	const static char xdigits[] = "0123456789ABCDEF";
	MemeInteger_t total_len;
	MemeVariableBufferStack_t vbuf;
	MemeInteger_t result = 0;
	MemeByte_t* index = NULL;
	
	assert(_out  != NULL && MemeStringStack_initWithHexadecimals);
	assert(_hexs != NULL && MemeStringStack_initWithHexadecimals);

	if (_len <= 0) {
		MemeStringStack_init(_out, _object_size);
		return 0;
	}

	if (_ivlen < 0) {
		_ivlen = _interval ? strlen(_interval) : 0;
	}
	if (_interval == NULL)
		_ivlen = 0;

	total_len = _len * 2 + (_len - 1) * _ivlen;
	MemeVariableBufferStack_init(&vbuf, MEME_STRING__OBJECT_SIZE);
	result = MemeVariableBuffer_resize((MemeVariableBuffer_t)&vbuf, total_len);
	if (result)
		return (int)result;

	index = MemeVariableBuffer_dataWithNotConst((MemeVariableBuffer_t)&vbuf);
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

	result = MemeVariableBuffer_releaseToString((MemeVariableBuffer_t)&vbuf, _out, _object_size);
	return (int)result;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_reset(
	MemeStringStack_t* _out, size_t _object_size)
{
	MemeString_t obj = NULL;

	assert(_out);
	assert(_object_size != 0);

	obj = (MemeString_t)_out;
	switch (MEME_STRING__GET_TYPE(obj)) {
	case MemeString_ImplType_user:
	{
		return MemeStringUser_reset((MemeStringUser_t*)_out);
	}
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
	default: {
		return MEME_ENO__POSIX_OFFSET(ENOTSUP);
	};
	}
	return 0;
}

MEME_API int MEME_STDCALL MemeStringStack_assign_v02(
	MemeStringStack_t* _s, size_t _object_size, MemeString_Const_t _other)
{
	int result = 0;

	assert(_s != NULL		&& MemeStringStack_assign_v02);
	assert(_other != NULL	&& MemeStringStack_assign_v02);

	if (MEGO_SYMBOL__UNLIKELY((void*)_s == (void*)_other))
		return 0;

	switch (MEME_STRING__GET_TYPE(_other))
	{
	case MemeString_ImplType_small:
	case MemeString_ImplType_medium:
	case MemeString_ImplType_large:
	case MemeString_ImplType_user:
	case MemeString_ImplType_view:
		break;
	default: {
		return MEME_ENO__POSIX_OFFSET(ENOTSUP);
	};
	}

	result = MemeStringStack_unInit(_s, sizeof(MemeStringStack_t));
	if (result)
		return result;

	switch (MEME_STRING__GET_TYPE(_other)) 
	{
	case MemeString_ImplType_small: {
		memcpy(_s, &(_other->small_), MEME_STRING__OBJECT_SIZE);
	} break;
	case MemeString_ImplType_medium: {
		result = MemeStringMedium_initWithCapacity((MemeStringMedium_t*)_s, MemeString_maxByteSize(_other));
		if (result)
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
		return MEME_ENO__POSIX_OFFSET(ENOTSUP);
	};
	}
	return 0;
}

MEME_API int MEME_STDCALL MemeStringViewUnsafeStack_init(MemeStringStack_t* _s, size_t _object_size,
	const uint8_t* _buf, MemeInteger_t _len)
{
	MemeStringViewUnsafe_t* p = (MemeStringViewUnsafe_t*)_s;

	assert(_s != NULL		&& MemeStringViewUnsafeStack_init);
	assert(_buf != NULL		&& MemeStringViewUnsafeStack_init);

	if (_len == -1)
		_len = strlen(_buf);

	//if (_len <= MEME_STRING__GET_SMALL_BUFFER_SIZE)
	//	return MemeStringSmall_initByU8bytes((MemeStringSmall_t*)_s, _buf, _len);

	p->data_ = _buf;
	p->size_ = _len;
	p->offset_ = 0;
	p->type_ = MemeString_ImplType_view;
	return 0;
}

MEME_API int MEME_STDCALL MemeStringViewUnsafeStack_initByOther(
	MemeStringStack_t* _s, size_t _object_size, const MemeStringStack_t* _other)
{
	assert(_s != NULL		&& MemeStringViewUnsafeStack_initByOther);
	assert(_other != NULL	&& MemeStringViewUnsafeStack_initByOther);

	if (MEME_STRING__GET_TYPE((MemeString_t)_other) == MemeString_ImplType_view)
	{
		memcpy(_s, _other, MEME_STRING__OBJECT_SIZE);
		return 0;
	}
	else {
		return MemeStringStack_initByOther(_s, _object_size, (MemeString_t)_other);
	}
}

MEME_API int MEME_STDCALL MemeStringViewUnsafeStack_assignByOther(
	MemeStringStack_t* _s, size_t _object_size, const MemeStringStack_t* _other)
{
	assert(_s != NULL		&& MemeStringViewUnsafeStack_assignByOther);
	assert(_other != NULL	&& MemeStringViewUnsafeStack_assignByOther);

	if (MEME_STRING__GET_TYPE((MemeString_t)_other) == MemeString_ImplType_view)
	{
		int result = MemeStringStack_unInit(_s, sizeof(MemeStringStack_t));
		if (result)
			return result;
		memcpy(_s, _other, MEME_STRING__OBJECT_SIZE);
		return 0;
	}
	else {
		return MemeStringStack_assign_v02(_s, _object_size, (MemeString_t)_other);
	}
}
