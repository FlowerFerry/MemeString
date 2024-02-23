
#include "meme/string.h"
#include "meme/impl/string.h"
#include "meme/impl/string_p__user.h"
#include "meme/impl/string_p__small.h"
#include "meme/impl/string_p__medium.h"
#include "meme/impl/string_p__large.h"

#include "meme/impl/algorithm.h"
#include <meme/utf/converter.h>
#include <mego/predef/symbol/likely.h>
#include <mego/predef/endian.h>
#include <mego/err/ec.h>

#include <errno.h>
#include <assert.h>
#include <stdlib.h>

const uint8_t * MemeStringImpl_default()
{
	static const uint8_t s[8] = { 0 };
	return s;
}

MemeString_Storage_t MemeStringImpl_initSuggestType(
	MemeInteger_t _len, MemeString_Storage_t _user_suggest)
{
	switch (_user_suggest) {
	case MemeString_StorageType_medium: {
		return MemeString_StorageType_medium;
	};
	case MemeString_StorageType_large: {
		return MemeString_StorageType_large;
	};
	default: {
		if (MemeString_StorageType_user < _user_suggest)
			return _user_suggest;


		if (_len <= MMS__GET_SMALL_BUFFER_SIZE)
			return MemeString_StorageType_small;

		do {
			MemeInteger_t medium_limit = MemeStringOption_getStorageMediumLimit();
			//if (0 < medium_limit && _len <= medium_limit)
			if (((-medium_limit) & (_len - 1 - medium_limit)) & INTPTR_MIN)
				return MemeString_StorageType_medium;
		} while (0);

		return MemeString_StorageType_large;
	};
	}

}

int MemeStringImpl_isModifiableType(MemeString_Storage_t _type)
{
	switch (_type) {
	case MemeString_StorageType_small:
	case MemeString_StorageType_medium:
		return 1;
	default:
		return 0;
	}
}

int
MemeStringImpl_dumpToModifiable(const MemeStringStack_t* _s, MemeStringStack_t* _out)
{
	assert(_s);
	assert(_out);

	switch (MMS__GET_TYPE((MemeString_t)_s))
	{
	case MemeString_StorageType_small:
	{
		memcpy(_out, _s, MEME_STRING__OBJECT_SIZE);
	} break;
	case MemeString_StorageType_medium:
	{

	} break;
	case MemeString_StorageType_large:
	{

	} break;
	case MemeString_UnsafeStorageType_view:
	{

	} break;
	case MemeString_StorageType_user:
	{

	} break;
	default: {

	} break;
	}

	return 0;
}

int MemeStringImpl_capacityExpansionSmallToMedium(MemeStringStack_t* _s, MemeInteger_t _minSizeRequest)
{
	MemeInteger_t dstlen = 0;
	int result = 0;
	
	assert(_s);

	dstlen = (MemeInteger_t)(MemeString_maxByteCapacity((MemeString_Const_t)_s) * 1.667);
	if (dstlen < _minSizeRequest) {
		dstlen = (MemeInteger_t)(_minSizeRequest * 1.667);
	}
	
	MemeStringStack_t newString;
	result = MemeStringMedium_initWithCapacity((MemeStringMedium_t*)&newString, dstlen);
	if ((result != 0)) 
		return result;

	result = MemeStringMedium_assign((MemeStringMedium_t*)&newString, 
		MemeString_byteData((MemeString_Const_t)_s), MemeString_byteSize((MemeString_Const_t)_s));
	if ((result != 0)) 
	{
		MemeStringStack_unInit(&newString, MEME_STRING__OBJECT_SIZE);
		return result;
	}

	MemeString_swap((MemeString_t)_s, (MemeString_t)&newString);
	MemeStringStack_unInit(&newString, MEME_STRING__OBJECT_SIZE);
	return 0;
}

int MemeStringImpl_capacityExpansionWithModifiable(
	MemeStringStack_t* _s, MemeInteger_t _minSizeRequest)
{
	assert(_s != NULL && MemeStringImpl_capacityExpansionWithModifiable);
	assert(_minSizeRequest > 0 && MemeStringImpl_capacityExpansionWithModifiable);

	switch (MMS__GET_TYPE((MemeString_t)_s)) 
	{
	case MemeString_ImplType_small: {
		return MemeStringImpl_capacityExpansionSmallToMedium(_s, _minSizeRequest);
	} break;
	case MemeString_ImplType_medium: {
		return MemeStringMedium_capacityExpansion((MemeStringMedium_t*)_s, _minSizeRequest);
	} break;
	default:
	{
		return (MGEC__OPNOTSUPP);
	} break;
	}

}

void MemeStringImpl_setDataOffset(MemeStringStack_t* _s, MemeInteger_t _offset)
{
	switch (MMS__GET_TYPE((MemeString_t)_s)) {
	case MemeString_ImplType_large: {
		MemeStringLarge_setOffset((MemeStringLarge_t*)_s, _offset);
	} break;
	case MemeString_ImplType_user: {
		MemeStringUser_setOffset((MemeStringUser_t*)_s, _offset);
	} break;
	default: {

	} break;
	}
}

void MemeStringImpl_shrinkTailZero(MemeStringStack_t* _s)
{
	switch (MMS__GET_TYPE((MemeString_t)_s))
	{
	case MemeString_ImplType_small: {
		MemeStringSmall_shrinkTailZero((MemeStringSmall_t*)_s);
	} break;
	case MemeString_ImplType_large: {
		MemeStringLarge_shrinkTailZero((MemeStringLarge_t*)_s);
	} break;
	case MemeString_ImplType_medium: {
		MemeStringMedium_shrinkTailZero((MemeStringMedium_t*)_s);
	} break;
	case MemeString_ImplType_user: {
		MemeStringUser_shrinkTailZero((MemeStringUser_t*)_s);
	} break;
	default: {

	} break;
	}
}

MemeByte_t* MemeStringImpl_forcedData(MemeStringStack_t* _s)
{
	return (MemeByte_t*)MemeString_byteData((MemeString_t)_s);
}

MEME_API MemeString_Storage_t MEME_STDCALL MemeString_storageType(MemeString_Const_t _s)
{
	assert(_s);
    return MMS__GET_TYPE((MemeString_t)_s);
}


MEME_EXTERN_C MEME_API int MEME_STDCALL MemeString_create(MemeString_t * _out)
{
	//MemeString_MallocFunction_t* c_func = NULL;

	assert(_out);

	//c_func = MemeString_getMallocFunction();
	*_out = (MemeString_t)(mmsmem_malloc(sizeof(struct _MemeString_t)));
	if (!(*_out))
		return (MGEC__NOMEM);

	memset(*_out, 0, sizeof(struct _MemeString_t));
	(*_out)->small_.type_ = MemeString_ImplType_small;
	(*_out)->small_.capacity_ = MMS__GET_SMALL_BUFFER_SIZE;
	return 0;
}

MEME_API int MEME_STDCALL MemeString_destroy(MemeString_t* _out)
{
	int result = 0;
    
    assert(_out);
    assert(*_out);

	result = MemeStringStack_unInit((mmsstk_t*)(*_out), MMS__OBJECT_SIZE);
    
    mmsmem_free(*_out);
    *_out = NULL;
    return result;
}

MEME_API int MEME_STDCALL MemeString_assign(MemeString_t _s, MemeString_Const_t _other)
{
	return MemeStringStack_assign((mmsstk_t*)_s, MMS__OBJECT_SIZE, _other);
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

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeString_isNonempty(MemeString_Const_t _s)
{
	assert(_s);

	switch (MMS__GET_TYPE(_s)) {
	case MemeString_ImplType_small:
	{
		return (_s->small_.capacity_ == MMS__GET_SMALL_BUFFER_SIZE ? 0: 1);
	} break;
	case MemeString_ImplType_medium:
	{
		return (_s->medium_.size_ == 0) ? 0 : 1;
	} break;
	case MemeString_ImplType_large:
	{
		return (_s->large_.size_ == 0) ? 0 : 1;
	} break;
	case MemeString_ImplType_view:
	{
		return (_s->viewUnsafe_.size_ == 0) ? 0 : 1;
	}
	case MemeString_ImplType_user:
	{
		return (_s->user_.size_ == 0) ? 0 : 1;
	}
	default: {
		return (MGEC__OPNOTSUPP);
	} break;
	}

}

MEME_API int MEME_STDCALL MemeString_isEmpty(MemeString_Const_t _s)
{
	return MemeString_isNonempty(_s) == 0;
}

MEME_EXTERN_C MEME_API const char *MEME_STDCALL MemeString_cStr(MemeString_Const_t _s)
{
	assert(_s != NULL && MemeString_cStr);
	return (const char*)MemeString_byteData(_s);
}

MEME_EXTERN_C MEME_API MemeInteger_t MEME_STDCALL MemeString_maxByteCapacity(MemeString_Const_t _s)
{
	assert(_s);
	
	switch (MMS__GET_TYPE(_s)) {
	case MemeString_ImplType_small:
	{
		return MMS__GET_SMALL_BUFFER_SIZE;
	};
	case MemeString_ImplType_medium:
	{
		return MemeStringMedium_maxByteCapacity(&(_s->medium_));
	};
	case MemeString_ImplType_large:
	{
		return _s->large_.size_;
	};
	case MemeString_ImplType_view:
	{
		return _s->viewUnsafe_.size_;
	}
	case MemeString_ImplType_user:
	{
		return _s->user_.size_;
	};
	default: {
		return 0;
	};
	}
}

MEME_API const MemeByte_t* MEME_STDCALL MemeString_at(MemeString_Const_t _s, MemeInteger_t _index)
{
    assert(_s);

    //if (MEGO_SYMBOL__UNLIKELY(_index >= MemeString_byteSize(_s) || _index < 0))
    if ((((MemeString_byteSize(_s) - 1 - _index) | _index) < 0))
        return NULL;
    return MemeString_byteData(_s) + _index;
}

MEME_API MemeInteger_t MEME_STDCALL MemeString_maxByteSize(MemeString_Const_t _s)
{
	assert(_s);

	switch (MMS__GET_TYPE(_s)) {
	case MemeString_ImplType_small:
	{
		return MMS__GET_SMALL_BUFFER_SIZE;
	};
	case MemeString_ImplType_medium:
	{
		return MemeStringMedium_maxByteSize(&(_s->medium_));
	};
	case MemeString_ImplType_large:
	{
		return _s->large_.size_;
	};
	case MemeString_ImplType_view:
	{
		return _s->viewUnsafe_.size_;
	}
	case MemeString_ImplType_user:
	{
		return _s->user_.size_;
	};
	default: {
		return 0;
	};
	}
}

MEME_API MemeInteger_t MEME_STDCALL MemeString_byteSize(MemeString_Const_t _s)
{
	assert(_s);

	switch (MMS__GET_TYPE(_s)) {
	case MemeString_ImplType_small:
	{
		return (MemeInteger_t)(MMS__GET_SMALL_BUFFER_SIZE - _s->small_.capacity_);
	};
	case MemeString_ImplType_medium:
	{
		return _s->medium_.size_;
	};
	case MemeString_ImplType_large:
	{
		return _s->large_.size_;
	};
	case MemeString_ImplType_view:
	{
		return _s->viewUnsafe_.size_;
	}
	case MemeString_ImplType_user:
	{
		return _s->user_.size_;
	};
	default: {
		return 0;
	};
	}
}

MEME_API MemeInteger_t
MEME_STDCALL MemeString_runeSize(mmstr_cptr_t _s)
{
    return mmutf_u8rune_size(MemeString_byteData(_s), MemeString_byteSize(_s));
}

MEME_API MemeInteger_t
MEME_STDCALL MemeString_u16CharSize(mmstr_cptr_t _s)
{
    return mmutf_char_size_u16from8(MemeString_byteData(_s), MemeString_byteSize(_s));
}

MEME_EXTERN_C MEME_API MemeInteger_t MEME_STDCALL MemeString_availableByteCapacity(MemeString_Const_t _s)
{
	assert(_s);

	switch (MMS__GET_TYPE(_s)) {
	//case MemeString_ImplType_user:
	//{
	//	return 0;
	//};
	case MemeString_ImplType_small:
	{
		return (MemeInteger_t)(_s->small_.capacity_);
	};
	case MemeString_ImplType_medium:
	{
		return (MemeInteger_t)MemeStringMedium_availableByteCapacity(&(_s->medium_));
	};
	//case MemeString_ImplType_large:
	//{
	//	return (MemeInteger_t)(0);
	//};
	default: {
		return 0;
	} break;
	}
}

static MemeString_MallocFunction_t** __MemeString_mallocFuncObject()
{
	static MemeString_MallocFunction_t* pointer =
#if MMOPT__HEADTAIL_MEMCHECK_ENABLED
		&MemeCheck_Malloc;
#else
		&malloc;
#endif
	return &pointer;
}

static MemeString_FreeFunction_t** __MemeString_freeFuncObject()
{
	static MemeString_FreeFunction_t* pointer =
#if MMOPT__HEADTAIL_MEMCHECK_ENABLED
		&MemeCheck_Free;
#else
		&free;
#endif
	return &pointer;
}

//MEME_EXTERN_C MEME_API int MEME_STDCALL MemeString_setMallocFunction(MemeString_MallocFunction_t* _fn)
//{
//	return MMENO__POSIX_OFFSET(ENOTSUP);

	//if (!_fn)
	//	return MMENO__POSIX_OFFSET(EINVAL);

	//*__MemeString_mallocFuncObject() = _fn;
	//return 0;
//}

//MEME_EXTERN_C MEME_API int MEME_STDCALL MemeString_setFreeFunction(MemeString_FreeFunction_t* _fn)
//{
//	return MMENO__POSIX_OFFSET(ENOTSUP);

	//if (!_fn)
	//	return MMENO__POSIX_OFFSET(EINVAL);

	//*__MemeString_freeFuncObject() = _fn;
	//return 0;
//}
//
//MEME_EXTERN_C MEME_API MemeString_MallocFunction_t* MEME_STDCALL MemeString_getMallocFunction()
//{
//	return *__MemeString_mallocFuncObject();
//}
//
//MEME_EXTERN_C MEME_API MemeString_FreeFunction_t* MEME_STDCALL MemeString_getFreeFunction()
//{
//	return *__MemeString_freeFuncObject();
//}
//
//MEME_EXTERN_C MEME_API MemeString_ReallocFunction_t* MEME_STDCALL MemeString_getReallocFunction()
//{
//#if MMOPT__HEADTAIL_MEMCHECK_ENABLED
//	return MemeCheck_Realloc;
//#else
//	return realloc;
//#endif
//}
//
//MEME_EXTERN_C MEME_API MemeString_CallocFunction_t* MEME_STDCALL MemeString_getCallocFunction()
//{
//#if MMOPT__HEADTAIL_MEMCHECK_ENABLED
//    return MemeCheck_Calloc;
//#else
//    return calloc;
//#endif
//}

MEME_EXTERN_C MEME_API mmsmem_malloc_t* MEME_STDCALL mmsmem_get_malloc_func()
{
	return *__MemeString_mallocFuncObject();
}

MEME_EXTERN_C MEME_API mmsmem_calloc_t* MEME_STDCALL mmsmem_get_calloc_func()
{
#if MMOPT__HEADTAIL_MEMCHECK_ENABLED
	return MemeCheck_Calloc;
#else
	return calloc;
#endif
}

MEME_EXTERN_C MEME_API mmsmem_realloc_t* MEME_STDCALL mmsmem_get_realloc_func()
{
#if MMOPT__HEADTAIL_MEMCHECK_ENABLED
    return MemeCheck_Realloc;
#else
    return realloc;
#endif
}

MEME_EXTERN_C MEME_API mmsmem_free_t* MEME_STDCALL mmsmem_get_free_func()
{
    return *__MemeString_freeFuncObject();
}

static MemeInteger_t* __MemeStringOption_storageMediumLimit()
{
	static MemeInteger_t limit = sizeof(MemeStringStack_t) + sizeof(MemeStringLarge_RefCounted_t) * 3;
	return &limit;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringOption_setStorageMediumLimit(MemeInteger_t _value)
{
	if (_value < 0)
		return (MGEC__INVAL);

	if (_value <= MMS__GET_SMALL_BUFFER_SIZE) {
		*__MemeStringOption_storageMediumLimit() = 0;
		return 0;
	}

	*__MemeStringOption_storageMediumLimit() = _value;
	return 0;
}

MEME_EXTERN_C MEME_API MemeInteger_t MEME_STDCALL 
	MemeStringOption_getStorageMediumLimit()
{
	return *__MemeStringOption_storageMediumLimit();
}

MEME_API int MEME_STDCALL MemeString_isEqual(MemeString_Const_t _s, 
	const char* _str, MemeInteger_t _len, int* _result)
{
	MemeInteger_t lhslen = 0;
	MemeInteger_t rhslen = 0;
	const char* src = NULL;

	if ((!_result))
		return (MGEC__INVAL);

	src = MemeString_cStr(_s);
	if (_len < 0) {
        _len = (MemeInteger_t)strlen(_str);
	}

	lhslen = MemeString_byteSize(_s);
	rhslen = _len;

	if (lhslen != rhslen) {
		*_result = 0;
		return 0;
	}
	if (lhslen == 0) {
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

	if (MEGO_SYMBOL__UNLIKELY(_result == NULL))
		return (MGEC__INVAL);

	lhslen = MemeString_byteSize(_lhs);
	rhslen = MemeString_byteSize(_rhs);

	if (lhslen != rhslen) {
		*_result = 0;
		return 0;
	}
	if (lhslen == 0) {
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

MEME_API int MEME_STDCALL MemeString_containsOnlyAscii(mmstr_cptr_t _s, int* _result)
{
    const mmbyte_t* src = NULL;
    mmint_t len = 0;

    if (MEGO_SYMBOL__UNLIKELY(_result == NULL))
        return (MGEC__INVAL);

    src = MemeString_byteData(_s);
    len = MemeString_byteSize(_s);

    for (; len > 0; --len, ++src) {
        if (*src > 127) {
            *_result = 0;
            return 0;
        }
    }

    *_result = 1;
    return 0;
}

MEME_API int MEME_STDCALL MemeString_compare(mmstr_cptr_t _s, mmstr_cptr_t _other)
{
    const mmbyte_t* src = NULL;
    const mmbyte_t* dst = NULL;
    mmint_t srclen = 0;
	mmint_t dstlen = 0;

	assert(_s);
	assert(_other);

	if (MEGO_SYMBOL__UNLIKELY((void*)_s == (void*)_other))
		return 0;

    src = MemeString_byteData(_s);
    dst = MemeString_byteData(_other);
    srclen = MemeString_byteSize(_s);
    dstlen = MemeString_byteSize(_other);

    for (; srclen > 0 && dstlen > 0; --srclen, --dstlen, ++src, ++dst) 
	{
        if (*src != *dst)
            return (*src < *dst ? -1 : 1);
    }
    return (srclen == dstlen ? 0 : (srclen < dstlen ? -1 : 1));
}

MEME_API MemeInteger_t MEME_STDCALL MemeString_indexOfWithUtf8bytes(
	MemeString_Const_t _s, MemeInteger_t _offset,
	const MemeByte_t* _needle, MemeInteger_t _needle_len,
	MemeFlag_CaseSensitivity_t _cs)
{
	const char* pointer;
	MemeInteger_t count;
	MemeInteger_t index = -1;

	pointer = MemeString_cStr(_s);
	count = MemeString_byteSize(_s);

	if ((_offset < 0))
		_offset = 0;
	if (_offset >= count)
		return -1;

	index = MemeImpl_SearchByBoyerMooreWithSensitivity(
		(const MemeByte_t*)(pointer + _offset), count - _offset, _needle, _needle_len, _cs);
	return index == -1 ? -1 : _offset + index;
}

MEME_API MemeInteger_t MEME_STDCALL MemeString_indexOfWithUtf8bytesAndSizeLimit(
	mmstr_cptr_t _s, mmint_t _offset, mmint_t _limit,
	const mmbyte_t* _needle, mmint_t _needle_len, MemeFlag_CaseSensitivity_t _cs)
{
    const char* pointer;
    mmint_t count;
    mmint_t index = -1;

    pointer = MemeString_cStr(_s);
    count = MemeString_byteSize(_s);

    if (_offset < 0)
        _offset = 0;
    if (_offset >= count)
        return -1;

    if (_limit < 0)
        _limit = count - _offset;
    if (_limit > count - _offset)
        _limit = count - _offset;

    index = MemeImpl_SearchByBoyerMooreWithSensitivity(
        (const mmbyte_t*)(pointer + _offset), _limit, _needle, _needle_len, _cs);
    return index == -1 ? -1 : _offset + index;
}

MEME_API MemeInteger_t MEME_STDCALL MemeString_indexOfWithOther(
	MemeString_Const_t _s, MemeInteger_t _offset,
	MemeString_Const_t _other, MemeFlag_CaseSensitivity_t _cs)
{
	const MemeByte_t* pointer = MemeString_byteData(_s);
	MemeInteger_t count = MemeString_byteSize(_s);
	MemeInteger_t index = -1;

	if ((_offset < 0))
		_offset = 0;
	if ((_offset >= count))
		return -1; 

	index = MemeImpl_SearchByBoyerMooreWithSensitivity(
		pointer + _offset, count - _offset,
		MemeString_byteData(_other), MemeString_byteSize(_other), _cs);
	return index == -1 ? -1 : _offset + index;
}

MEME_API MemeInteger_t MEME_STDCALL 
MemeString_indexByCondByteFunc(
	MemeString_Const_t _s, MemeInteger_t _offset, MemeString_MatchCondByteFunc_t* _cond_func, void* _arg)
{
    const MemeByte_t* pointer = MemeString_byteData(_s);
    MemeInteger_t count = MemeString_byteSize(_s);

    if ((_offset < 0))
        _offset = 0;
    if ((_offset >= count))
        return -1;

    for (MemeInteger_t index = _offset, result = 0; index < count; ++index)
    {
        result = _cond_func(pointer[index], _arg);
        if (result == 1)
            return index;
        else if (result == -1)
            return (MGEC__CANCELED);
    }
	return -1;
}

MEME_API MemeInteger_t MEME_STDCALL 
MemeString_lastIndexOfWithUtf8bytes(
	MemeString_Const_t _s, MemeInteger_t _offset, 
	const MemeByte_t* _needle, MemeInteger_t _needle_len, MemeFlag_CaseSensitivity_t _cs)
{
    const MemeByte_t* pointer = MemeString_byteData(_s);
    MemeInteger_t count = MemeString_byteSize(_s);
    MemeInteger_t index = -1;

	if ((_offset < 0))
		_offset = count;
    
	if ((_offset > count - 1))
		_offset = count - 1;

    index = MemeImpl_ReverseSearchByBoyerMooreWithSensitivity(
        pointer, _offset + 1, _needle, _needle_len, _cs);
	return index == -1 ? -1 : index;
}

MEME_API MemeInteger_t MEME_STDCALL 
MemeString_matchCountWithUtf8bytes(
	MemeString_Const_t _s, MemeInteger_t _offset, const MemeByte_t* _needle, MemeInteger_t _needle_len, MemeFlag_CaseSensitivity_t _cs)
{
    //const MemeByte_t* pointer = MemeString_byteData(_s);
    MemeInteger_t count = MemeString_byteSize(_s);
    MemeInteger_t index = -1;
    MemeInteger_t match_count = 0;

    if ((_offset < 0))
        _offset = 0;
    if ((_offset > count - 1))
        _offset = count - 1;

    index = MemeString_indexOfWithUtf8bytes(_s, _offset, _needle, _needle_len, _cs);
    while (index != -1)
    {
        match_count++;
        index = MemeString_indexOfWithUtf8bytes(_s, index + 1, _needle, _needle_len, _cs);
    }
    return match_count;
}

MEME_API MemeInteger_t MEME_STDCALL 
MemeString_indexOfWithByte(MemeString_Const_t _s, MemeInteger_t _offset, MemeByte_t _byte, MemeFlag_CaseSensitivity_t _cs)
{
    const MemeByte_t* pointer = MemeString_byteData(_s);
    MemeInteger_t count = MemeString_byteSize(_s);
    MemeInteger_t index = -1;

	if ((_offset < 0))
		_offset = 0;
    if (_offset >= count)
        return -1;

    index = MemeImpl_SearchByViolenceWithSensitivity(
		pointer + _offset, count - _offset, _byte, _cs);
    return index == -1 ? -1 : _offset + index;
}

MEME_API MemeInteger_t MEME_STDCALL 
MemeString_startsMatchWithOther(MemeString_Const_t _s, MemeString_Const_t _other, MemeFlag_CaseSensitivity_t _cs)
{
	const MemeByte_t* pointer_l = MemeString_byteData(_s);
	MemeInteger_t count_l = MemeString_byteSize(_s);
	const MemeByte_t* pointer_r = MemeString_byteData(_other);
	MemeInteger_t count_r = MemeString_byteSize(_other);
    
    if (count_l < count_r)
        return 0;
    if (count_r == 0)
        return 1;
    
    return memcmp(pointer_l, pointer_r, count_r) == 0 ? 1 : 0;
}

MEME_API MemeInteger_t
MEME_STDCALL MemeString_startsMatchWithUtf8bytes(
    MemeString_Const_t _s, const MemeByte_t* _needle, MemeInteger_t _needle_len, MemeFlag_CaseSensitivity_t _cs)
{
    const MemeByte_t* pointer = MemeString_byteData(_s);
    MemeInteger_t count = MemeString_byteSize(_s);
    
	if (_needle_len < 0)
		_needle_len = strlen((const char*)_needle);

    if (count < _needle_len)
        return 0;
    if (_needle_len == 0)
        return 1;

    return memcmp(pointer, _needle, _needle_len) == 0 ? 1 : 0;
}

MEME_API MemeInteger_t MEME_STDCALL 
MemeString_endsMatchWithOther(
	MemeString_Const_t _s, MemeString_Const_t _other, MemeFlag_CaseSensitivity_t _cs)
{
    const MemeByte_t* pointer_l = MemeString_byteData(_s);
    MemeInteger_t count_l = MemeString_byteSize(_s);
    const MemeByte_t* pointer_r = MemeString_byteData(_other);
    MemeInteger_t count_r = MemeString_byteSize(_other);

    if (count_l < count_r)
        return 0;
    if (count_r == 0)
        return 1;

    return memcmp(pointer_l + count_l - count_r, pointer_r, count_r) == 0 ? 1 : 0;
}

MEME_API MemeInteger_t MEME_STDCALL 
MemeString_endsMatchWithUtf8bytes(
	MemeString_Const_t _s, const MemeByte_t* _needle, MemeInteger_t _needle_len, MemeFlag_CaseSensitivity_t _cs)
{
    const MemeByte_t* pointer = MemeString_byteData(_s);
    MemeInteger_t count = MemeString_byteSize(_s);

	if (_needle_len < 0)
        _needle_len = strlen((const char*)_needle);

    if (count < _needle_len)
        return 0;
    if (_needle_len == 0)
        return 1;

    return memcmp(pointer + count - _needle_len, _needle, _needle_len) == 0 ? 1 : 0;
}

MEME_API MemeInteger_t MEME_STDCALL MemeString_split(
	MemeString_Const_t _s, const char* _key, MemeInteger_t _key_len, 
	MemeFlag_SplitBehavior_t _behavior, MemeFlag_CaseSensitivity_t _sensitivity,
	MemeStringStack_t* MEGO_SYMBOL__RESTRICT _out, 
	MemeInteger_t* MEGO_SYMBOL__RESTRICT _out_count, 
	MemeInteger_t* MEGO_SYMBOL__RESTRICT _search_index)
{
	int result = 0;
	MemeInteger_t last_index = (_search_index == NULL ? 0 : *_search_index);
	MemeInteger_t curr_index = -1;
	MemeInteger_t output_index = 0;

	assert(_s != NULL				&& MemeString_split);
	assert(_out != NULL				&& MemeString_split);
	assert(_out_count != NULL		&& MemeString_split);

	if (*_out_count < 1)
		return (MGEC__INVAL);
	if (_key_len < 0)
		_key_len = strlen(_key);

	while (output_index < *_out_count)
	{
		curr_index =
			MemeString_indexOfWithUtf8bytes(
				_s, last_index, (const MemeByte_t*)_key, _key_len, _sensitivity);
		if (curr_index == -1) {
			if (last_index < MemeString_byteSize(_s))
			{
				MemeStringStack_initByU8bytes(_out + output_index,
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

		result = MemeStringStack_initByU8bytes(_out + output_index,
			MEME_STRING__OBJECT_SIZE, MemeString_byteData(_s) + last_index, curr_index - last_index);
		if (result != 0) {
            for (MemeInteger_t i = 0; i < output_index; ++i)
                MemeStringStack_unInit(_out + i, MMS__OBJECT_SIZE);
            return result;
		}
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

MEME_API mmint_t MEME_STDCALL MemeString_splitByCondByteFunc(
	mmstr_cptr_t _s, MemeString_MatchCondByteFunc_t* _cond_func, void* _user_data,
	mmstrstk_t* MEGO_SYMBOL__RESTRICT _out, 
	mmint_t* MEGO_SYMBOL__RESTRICT _out_count, 
	mmint_t* MEGO_SYMBOL__RESTRICT _search_index)
{
    int result = 0;
    mmint_t last_index = (_search_index == NULL ? 0 : *_search_index);
    mmint_t curr_index = -1;
    mmint_t output_index = 0;

    assert(_s != NULL && MemeString_splitByCondByteFunc);
    assert(_out != NULL && MemeString_splitByCondByteFunc);
    assert(_out_count != NULL && MemeString_splitByCondByteFunc);

    if (*_out_count < 1)
        return (MGEC__INVAL);

    while (output_index < *_out_count)
    {
        curr_index =
            MemeString_indexByCondByteFunc(
                _s, last_index, _cond_func, _user_data);
        if (curr_index == -1) {
            if (last_index < MemeString_byteSize(_s))
            {
                MemeStringStack_initByU8bytes(_out + output_index,
                    MEME_STRING__OBJECT_SIZE, MemeString_byteData(_s) + last_index,
                    MemeString_byteSize(_s) - last_index);
                *_out_count = output_index + 1;
            }
            else {
                *_out_count = output_index;
            }
            if (_search_index)
                *_search_index = -1;
            return 0;
        }

        if ((curr_index - last_index) == 0)
        {
            last_index += 1;
            continue;
        }

        result = MemeStringStack_initByU8bytes(_out + output_index,
            MEME_STRING__OBJECT_SIZE, MemeString_byteData(_s) + last_index, curr_index - last_index);
        if (result != 0) {
            for (mmint_t i = 0; i < output_index; ++i)
                MemeStringStack_unInit(_out + i, MMS__OBJECT_SIZE);
            return result;
        }
        last_index = curr_index + 1;
        ++output_index;
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

MEME_API mmint_t MEME_STDCALL MemeString_writeBytes(
	mmstr_cptr_t _s, mmint_t _offset, mmint_t _count, mmbyte_t* _out)
{
    assert(_s   != NULL && MemeString_writeBytes);
    assert(_out != NULL && MemeString_writeBytes);

	if (_offset < 0)
        _offset = 0;
	if (_offset >= MemeString_byteSize(_s))
		return (MGEC__INVAL);
    if (_count < 0)
        _count = MemeString_byteSize(_s) - _offset;
    if (_count + _offset > MemeString_byteSize(_s))
        _count = MemeString_byteSize(_s) - _offset;

    memcpy(_out, MemeString_byteData(_s) + _offset, _count);
    return 0;
}

MEME_API MemeInteger_t
MEME_STDCALL MemeString_writeU16Chars(mmstr_cptr_t _s, uint16_t* _out)
{
    return mmutf_convert_u8to16(MemeString_byteData(_s), MemeString_byteSize(_s), _out);
}

MEME_API const MemeByte_t* MEME_STDCALL MemeString_byteData(MemeString_Const_t _s)
{
	assert(_s != NULL && MemeString_byteData);

	switch (MMS__GET_TYPE(_s)) {
	case MemeString_ImplType_user:
	{
		return MemeStringUser_constData(&(_s->user_));
	}
	case MemeString_ImplType_small:
	{
		return (_s->small_.buffer_);
	};
	case MemeString_ImplType_medium:
	{
		return (MemeStringMedium_constData(&(_s->medium_)));
	};
	case MemeString_ImplType_large:
	{
		return MemeStringLarge_constData(&(_s->large_));
	};
	case MemeString_ImplType_view:
	{
		return _s->viewUnsafe_.data_;
	}
	default: {
		return (MemeStringImpl_default());
	};
	}
}

MEME_API MemeInteger_t MEME_STDCALL MemeString_isSharedStorageTypes(MemeString_Const_t _s)
{
	assert(_s != NULL && MemeString_isSharedStorageTypes);

	switch (MMS__GET_TYPE(_s)) {
	case MemeString_ImplType_user:
	case MemeString_ImplType_large:
	{
		return 1;
	}
	default: {
		return 0;
	};
	}
}

MEME_API MemeInteger_t MEME_STDCALL MemeString_getSharedHeapByteSize(MemeString_Const_t _s)
{
	if ((_s == NULL))
		return 0;
    
    switch (MMS__GET_TYPE(_s)) {
    case MemeString_ImplType_user:
    {
        return MemeStringUser_getSharedHeapByteSize(&(_s->user_));
    }
    case MemeString_ImplType_large:
    {
        return MemeStringLarge_getSharedHeapByteSize(&(_s->large_));
    }
    default: {
        return 0;
    };
    }
}

MEME_API MemeInteger_t MEME_STDCALL MemeString_getPrivateHeapByteSize(MemeString_Const_t _s)
{
    if ((_s == NULL))
        return 0;

    switch (MMS__GET_TYPE(_s)) {
    case MemeString_ImplType_large:
    {
        return MemeStringLarge_getPrivateHeapByteSize(&(_s->large_));
    }
	case MemeString_ImplType_medium:
	{
		return MemeStringMedium_maxByteCapacity(&(_s->medium_));
	}
	case MemeString_ImplType_user:
	{
		return MemeStringUser_getPrivateHeapByteSize(&(_s->user_));
	}
    default: {
        return 0;
    };
    }
}

MEME_API MemeInteger_t MEME_STDCALL MemeString_checkHeadTailMemory(MemeString_Const_t _s)
{
#if !(MMOPT__HEADTAIL_MEMCHECK_ENABLED)
	return 1;
#else
    if (_s == NULL)
        return 0;

    switch (MMS__GET_TYPE(_s)) {
	case MemeString_ImplType_medium: 
	{
        return MemeStringMedium_checkHeadTailMemory(&(_s->medium_));
    }
    case MemeString_ImplType_large:
    {
        return MemeStringLarge_checkHeadTailMemory(&(_s->large_));
    }
	case MemeString_ImplType_user: 
	{
        return MemeStringUser_checkHeadTailMemory(&(_s->user_));
	}
    default: {
        return 1;
    };
    }
#endif
}
