
#include "meme/string.h"
#include "meme/impl/string.h"
#include "meme/impl/string_p__user.h"
#include "meme/impl/string_p__small.h"
#include "meme/impl/string_p__large.h"

#include <errno.h>
#include <assert.h>
#include <stdlib.h>

const uint8_t * MemeStringImpl_default()
{
	static const uint8_t s[8] = { 0 };
	return s;
}

MemeString_Storage_t MemeStringImpl_initSuggestType(
	size_t _len, MemeString_Storage_t _user_suggest)
{
	switch (_user_suggest) {
	case MemeString_StorageType_large: {
		return MemeString_StorageType_large;
	};
	//case MemeString_StorageType_viewUnsafe: {
	//	return MemeString_StorageType_viewUnsafe;
	//};
	default: {
		if (MemeString_StorageType_user < _user_suggest)
			return _user_suggest;

		if (_len <= MEME_STRING__GET_SMALL_BUFFER_SIZE)
			return MemeString_StorageType_small;
		do {
			size_t medium_limit = MemeStringOption_getStorageMediumLimit();
			if (medium_limit && _len <= medium_limit)
				return MemeString_StorageType_medium;
		} while (0);

		return MemeString_StorageType_large;
	};
	}

	return MemeString_StorageType_large;
}

MEME_API MemeString_Storage_t MEME_STDCALL MemeString_storageType(MemeString_Const_t _s)
{
	assert(_s);
	return _s->small_.type_;
}


MEME_EXTERN_C MEME_API int MEME_STDCALL MemeString_create(MemeString_t * _out)
{
	MemeString_MallocFunction_t* c_func = NULL;

	assert(_out);

	c_func = MemeString_getMallocFunction();
	*_out = (MemeString_t)(c_func(sizeof(struct _MemeString_t)));
	if (!(*_out))
		return -ENOMEM;

	memset(*_out, 0, sizeof(struct _MemeString_t));
	(*_out)->small_.type_ = MemeString_ImplType_small;
	(*_out)->small_.capacity_ = MEME_STRING__GET_SMALL_BUFFER_SIZE;
	return 0;
}

MEME_API int MEME_STDCALL MemeString_swap(MemeString_t _lhs, MemeString_t _rhs)
{
	assert(_lhs);
	assert(_rhs);

	MemeStringStack_t temp;
	memcpy(&temp, _lhs, MEME_STRING__OBJECT_SIZE);
	memcpy(_lhs,  _rhs, MEME_STRING__OBJECT_SIZE);
	memcpy(_rhs, &temp, MEME_STRING__OBJECT_SIZE);

	return 0;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeString_isEmpty(MemeString_Const_t _s)
{
	assert(_s);

	switch (MEME_STRING__GET_TYPE(*_s)) {
	case MemeString_ImplType_small:
	{
		return (_s->small_.capacity_ == MEME_STRING__GET_SMALL_BUFFER_SIZE ? 0: 1);
	} break;
	case MemeString_ImplType_medium:
	{
		return (_s->medium_.basic_.size_ == 0) ? 0 : 1;
	} break;
	case MemeString_ImplType_large:
	{
		return (_s->large_.basic_.size_ == 0) ? 0 : 1;
	} break;
	default: {
		return 1;
	} break;
	}

	return 1;
}

MEME_EXTERN_C MEME_API const char *MEME_STDCALL MemeString_cStr(MemeString_Const_t _s)
{
	assert(_s);

	switch (MEME_STRING__GET_TYPE(*_s)) {
	case MemeString_ImplType_user:
	{
		return MemeStringUser_cStr(&(_s->user_));
	}
	case MemeString_ImplType_small:
	{
		return (const char*)(_s->small_.buffer_);
	};
	case MemeString_ImplType_medium:
	{
		return (const char*)(_s->medium_.data_);
	};
	case MemeString_ImplType_large:
	{
		return (const char*)(_s->large_.ref_->data_);
	};
	case MemeString_ImplType_view:
	{
		return (const char*)_s->viewUnsafe_.data_;
	}
	default: {
		return (const char*)(MemeStringImpl_default());
	};
	}
}

MEME_EXTERN_C MEME_API MemeInteger_t MEME_STDCALL MemeString_byteSize(MemeString_Const_t _s)
{
	assert(_s);
	
	switch (MEME_STRING__GET_TYPE(*_s)) {
	case MemeString_ImplType_user:
	{
		return (MemeInteger_t)_s->user_.size_;
	};
	case MemeString_ImplType_small:
	{
		return (MemeInteger_t)(MEME_STRING__GET_SMALL_BUFFER_SIZE - _s->small_.capacity_);
	};
	case MemeString_ImplType_medium:
	{
		return (MemeInteger_t)(_s->medium_.basic_.size_);
	};
	case MemeString_ImplType_large:
	{
		return (MemeInteger_t)(_s->large_.basic_.size_);
	};
	case MemeString_ImplType_view:
	{
		return (MemeInteger_t)_s->viewUnsafe_.size_;
	}
	default: {
		return 0;
	};
	}
}

MEME_EXTERN_C MEME_API MemeInteger_t MEME_STDCALL MemeString_byteCapacity(MemeString_Const_t _s)
{
	assert(_s);

	switch (MEME_STRING__GET_TYPE(*_s)) {
	case MemeString_ImplType_user:
	{
		return 0;
	};
	case MemeString_ImplType_small:
	{
		return (MemeInteger_t)(_s->small_.capacity_);
	};
	case MemeString_ImplType_medium:
	{
		return (MemeInteger_t)(_s->medium_.basic_.capacity_);
	};
	case MemeString_ImplType_large:
	{
		return (MemeInteger_t)(_s->large_.basic_.capacity_);
	};
	default: {
		return 0;
	} break;
	}
}



static MemeString_MallocFunction_t** __MemeString_mallocFuncObject()
{
	static MemeString_MallocFunction_t* pointer = malloc;
	return &pointer;
}

static MemeString_FreeFunction_t** __MemeString_freeFuncObject()
{
	static MemeString_FreeFunction_t* pointer = free;
	return &pointer;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeString_setMallocFunction(MemeString_MallocFunction_t* _fn)
{
	if (!_fn)
		return -EINVAL;

	*__MemeString_mallocFuncObject() = _fn;
	return 0;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeString_setFreeFunction(MemeString_FreeFunction_t* _fn)
{
	if (!_fn)
		return -EINVAL;

	*__MemeString_freeFuncObject() = _fn;
	return 0;
}

MEME_EXTERN_C MEME_API MemeString_MallocFunction_t* MEME_STDCALL MemeString_getMallocFunction()
{
	return *__MemeString_mallocFuncObject();
}

MEME_EXTERN_C MEME_API MemeString_FreeFunction_t* MEME_STDCALL MemeString_getFreeFunction()
{
	return *__MemeString_freeFuncObject();
}

int MemeStringMemory_allocFunctionSwitch(
	MemeString_MallocFunction_t* _in_malloc_fn, MemeString_FreeFunction_t* _in_free_fn, 
	MemeString_MallocFunction_t** _out_malloc_fn, MemeString_FreeFunction_t** _out_free_fn)
{
	*_out_malloc_fn = (_in_malloc_fn ? _in_malloc_fn : MemeString_getMallocFunction());
	if (*_out_malloc_fn == _in_malloc_fn)
	{
		if (_in_free_fn == NULL)
			return -EINVAL;
		*_out_free_fn = _in_free_fn;
	}
	else {
		*_out_free_fn = MemeString_getFreeFunction();
	}
	return 0;
}



static size_t* __MemeStringOption_storageMediumLimit()
{
	static size_t limit = 64;
	return &limit;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringOption_setStorageMediumLimit(size_t _value)
{
	if (_value > (SIZE_MAX >> 1))
		return -EINVAL;

	if (_value <= MEME_STRING__GET_SMALL_BUFFER_SIZE) {
		*__MemeStringOption_storageMediumLimit() = 0;
		return 0;
	}

	*__MemeStringOption_storageMediumLimit() = _value;
	return 0;
}

MEME_EXTERN_C MEME_API size_t MEME_STDCALL MemeStringOption_getStorageMediumLimit()
{
	return *__MemeStringOption_storageMediumLimit();
}

MEME_API int MEME_STDCALL MemeString_isEqual(MemeString_Const_t _s, 
	const char* _str, MemeInteger_t _len, int* _result)
{
	MemeInteger_t lhslen = 0;
	MemeInteger_t rhslen = 0;
	const char* src = NULL;

	if (!_result)
		return -EINVAL;

	src = MemeString_cStr(_s);
	if (_len == -1) {
		*_result = (strcmp(src, _str) == 0 ? 1 : 0);
		return 0;
	}

	lhslen = MemeString_byteSize(_s);
	rhslen = _len;

	if (lhslen != rhslen)
	{
		*_result = 0;
		return 0;
	}
	if (lhslen == 0)
	{
		*_result = 1;
		return 0;
	}
	_len = (rhslen * 9) / 10;
	if (memcmp(src + _len, _str + _len, rhslen - _len) != 0)
	{
		*_result = 0;
		return 0;
	}

	*_result = (memcmp(src, _str, lhslen) == 0 ? 1: 0);
	return 0;
}

MEME_API int MEME_STDCALL MemeString_isEqualWithOther(MemeString_Const_t _lhs, 
	MemeString_Const_t _rhs, int* _result)
{
	MemeInteger_t lhslen = 0;
	MemeInteger_t rhslen = 0;
	const char* lhs = NULL;
	const char* rhs = NULL;

	if (!_result)
		return -EINVAL;

	lhslen = MemeString_byteSize(_lhs);
	rhslen = MemeString_byteSize(_rhs);

	if (lhslen != rhslen)
	{
		*_result = 0;
		return 0;
	}
	if (lhslen == 0)
	{
		*_result = 1;
		return 0;
	}

	lhs = MemeString_cStr(_lhs);
	rhs = MemeString_cStr(_rhs);
	rhslen = (lhslen * 9) / 10;
	if (memcmp(lhs + rhslen, rhs + rhslen, lhslen - rhslen) != 0)
	{
		*_result = 0;
		return 0;
	}

	*_result = (memcmp(lhs, rhs, lhslen) == 0 ? 1 : 0);
	return 0;
}
