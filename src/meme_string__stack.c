
#include "mego/predef/symbol/likely.h"
#include <mego/util/itoa.h>
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
	mmstr_ptr_t obj = NULL;

	assert(_out && "MemeStringStack_init");
	assert(_object_size != 0 && _object_size <= MMSTR__MAX_REG_BYTE_SIZE && "MemeStringStack_init");
    
	if (MEGO_SYMBOL__UNLIKELY(_object_size > MMSTR__OBJ_SIZE))
		return (MGEC__OPNOTSUPP);

	obj = (mmstr_ptr_t)_out;
	obj->small_.reg_size_ = (mmbyte_t)(_object_size / sizeof(mmint_t));
	obj->small_.buffer_[0] = '\0';
	obj->small_.type_ = MemeString_ImplType_small;
	obj->small_.capacity_ = MMSTR__GET_SMALL_BUF_MAX_SIZE;
	return 0;
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL MemeStringStack_getInitObject(size_t _object_size)
{
	mmstrstk_t stack;
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

	if (MEGO_SYMBOL__UNLIKELY(_out == NULL || _user_data == NULL || _destruct_fn == NULL || _data_fn == NULL || _size_fn == NULL))
		return MGEC__INVAL;

	len = _size_fn(_user_data);
	if (len <= MMSTR__GET_SMALL_BUF_MAX_SIZE)
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

	assert((mmstr_cptr_t)_out != NULL && "MemeStringStack_unInit");

	switch (MMSTR__GET_IMPLTYPE((mmstr_cptr_t)_out)) {
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
	MemeString_ImplType_t type;

	assert(_out && "MemeStringStack_initByU8bytesAndType");
	assert(_object_size != 0 && _object_size <= MMSTR__MAX_REG_BYTE_SIZE && "MemeStringStack_initByU8bytesAndType");

	if (MEGO_SYMBOL__UNLIKELY(_utf8 == NULL))
		return mmstrstk_init_v0(_out, _object_size);

	if (_len < 0)
		_len = strlen((const char*)_utf8);

	type = MemeStringImpl_initSuggestType(_len, mmstrimpl_strgtype_to_impltype(_suggest));
	switch (type) {
	case MemeString_ImplType_large: {
		return MemeStringLarge_initByU8bytes((MemeStringLarge_t*)_out, _utf8, _len, NULL, NULL, 0, 0);
	} break;
	case MemeString_ImplType_medium: {
		int result = MemeStringMedium_initWithCapacity((MemeStringMedium_t*)_out, _len);
		if (result)
			return result;
		result = MemeStringMedium_appendWithBytes((MemeStringMedium_t*)_out, _utf8, _len);
		if (result) {
			MemeStringMedium_unInit((MemeStringMedium_t*)_out);
		}
		return result;
	} break;
	case MemeString_ImplType_small: {
		return MemeStringSmall_initByU8bytes((MemeStringSmall_t*)_out, _utf8, _len);
	};
	}

	if (MemeString_ImplType_user < type)
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
	MemeString_ImplType_t type;
	
    assert(_out && "MemeStringStack_initByU16bytesAndType");
    assert(_object_size != 0 && _object_size <= MMSTR__MAX_REG_BYTE_SIZE && "MemeStringStack_initByU16bytesAndType");
	
	if (MEGO_SYMBOL__UNLIKELY(_buf == NULL))
		return mmstrstk_init_v0(_out, _object_size);
	
    if (_len < 0)
        _len = strlen16(_buf);
	
    u8len = mmutf_char_size_u8from16(_buf, _len);
	type  = MemeStringImpl_initSuggestType(u8len, mmstrimpl_strgtype_to_impltype(_suggest));
	switch (type) {
	case MemeString_ImplType_medium: {
		MemeInteger_t pos = 0;
		int result = MemeStringMedium_initWithCapacity((MemeStringMedium_t*)_out, u8len);
		if (result)
			return result;
		result = MemeStringMedium_resizeWithByte((MemeStringMedium_t*)_out, u8len, 0);
        if (result) {
			MemeStringMedium_unInit((MemeStringMedium_t*)_out);
			return result;
		}
		pos = mmutf_convert_u16to8(_buf, _len, MemeStringMedium_data((MemeStringMedium_t*)_out));
        if (pos != u8len) {
			result = MemeStringMedium_resizeWithByte((MemeStringMedium_t*)_out, pos, 0);
			if (result) {
				MemeStringMedium_unInit((MemeStringMedium_t*)_out);
			}
			return result;
		}
		else 
            return 0;
	} break;
	case MemeString_ImplType_small: {
		MemeInteger_t pos = 0;
		MemeStringStack_init((mmsstk_t*)_out, _object_size);
		MemeStringSmall_resizeWithByte((MemeStringSmall_t*)_out, u8len, 0);
		pos = mmutf_convert_u16to8(_buf, _len, MemeStringImpl_forcedData((mmsstk_t*)_out));
        if (pos != u8len) {
			int result = MemeStringSmall_resizeWithByte((MemeStringSmall_t*)_out, pos, 0);
			if (result) {
				MemeStringStack_unInit((mmsstk_t*)_out, _object_size);
			}
			return result;
		}
        else
            return 0;
	} break;
	default: {
		mmint_t pos = 0;
		mmvbstk_t vbuf;
		int result = 0;
		MemeVariableBufferStack_init(&vbuf, MMSTR__OBJ_SIZE);
		result = (int)MemeVariableBuffer_resize((mmvb_ptr_t)&vbuf, u8len);
		if (result) {
			MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
			return result;
		}
        pos = mmutf_convert_u16to8(_buf, _len, MemeVariableBuffer_dataWithNotConst((mmvb_ptr_t)&vbuf));
        result = (int)MemeVariableBuffer_resize((mmvb_ptr_t)&vbuf, pos);
		if (result) {
			MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
			return result;
		}

		// result = (int)MemeVariableBuffer_releaseToString((mmvb_ptr_t)&vbuf, _out, _object_size);
		result = (int)MemeStringLarge_initByU8bytes(
			(MemeStringLarge_t*)_out, MemeVariableBuffer_data((mmvb_ptr_t)&vbuf), pos, NULL, NULL, 0, 0);
		MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
		if (result) {
			return result;
		}
        return 0;
	} break;
	}
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_initByOther(
	mmstrstk_t* _out, size_t _object_size, mmstr_cptr_t _other)
{

	assert(_out);
	assert(_object_size != 0 && _object_size <= MMSTR__MAX_REG_BYTE_SIZE);

	if (MEGO_SYMBOL__UNLIKELY(_other == NULL))
		return mmstrstk_init_v0(_out, _object_size);
	
	if (((void*)_out == (void*)_other))
		return (MGEC__CANCELED);

	switch (MMSTR__GET_IMPLTYPE(_other)) {
	case MemeString_ImplType_small: {
		memcpy(_out, _other, MMS__OBJECT_SIZE);
	} break;
	case MemeString_ImplType_medium: {
		mgec_t eno;
		mmstrstk_init_v0(_out, _object_size);
		eno = (int)MemeVariableBuffer_appendWithBytes((mmvb_t)_out,
			MemeString_byteData(_other), MemeString_byteSize(_other));
		if (eno) {
			mmstrstk_uninit_v0(_out, _object_size);
			return eno;
		}
	} break;
	case MemeString_ImplType_large: {
		return MemeStringLarge_initByOther((MemeStringLarge_t*)_out, &(_other->large_));
	} break;
	case MemeString_ImplType_user: {
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

//MEME_API mgec_t MEME_STDCALL MemeStringStack_initByOtherStack(
//	mmstrstk_t* _out, size_t _object_size, const mmstrstk_t* _other)
//{
//	return 0;
//}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_initByBuffer(
	mmsstk_t* _out, size_t _object_size, MemeBuffer_Const_t _other, MemeInteger_t _offset)
{
	MemeInteger_t length = 0;

	if (MEGO_SYMBOL__UNLIKELY(_other == NULL))
		return mmstrstk_init_v0(_out, _object_size);

	if (((void*)_out == (void*)_other))
		return (MGEC__CANCELED);

	if (_offset < 0)
		_offset = 0;

	length = MemeBuffer_size(_other) - _offset;
	if ((length <= 0)) 
	{
		return MemeStringStack_init(_out, _object_size);
	}
	else if (length <= MMSTR__GET_SMALL_BUF_MAX_SIZE)
	{
		MemeStringSmall_initByU8bytes(
			(MemeStringSmall_t*)_out, MemeBuffer_data(_other) + _offset, length);
		MemeStringSmall_shrinkTailZero((MemeStringSmall_t*)_out);
		return 0;
	}

	switch (MMSTR__GET_IMPLTYPE((mmstr_cptr_t)_other))
	{
	case MemeString_ImplType_medium: {
		int result = 0;
		MemeStringStack_init(_out, MEME_STRING__OBJECT_SIZE);
		result = (int)MemeVariableBuffer_appendWithBytes((mmvb_t)_out,
			MemeBuffer_data(_other) + _offset, length);
		if (result) {
			mmstrstk_uninit_v0(_out, _object_size);
			return result;
		}
		MemeStringMedium_shrinkTailZero((MemeStringMedium_t*)_out);
		return result;
	} break;
	case MemeString_ImplType_user:
	case MemeString_ImplType_large: {

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
	if (MemeRune_isValid(&_ch) == 0) {
		return MGEC__INVAL;
	}

	if (_count <= 0) {
		return mmstrstk_init_v0(_out, _object_size);
	}
	else if (_count == 1) {
		return MemeStringStack_initByU8bytesAndType(
			_out, _object_size, MemeRune_data(&_ch), MemeRune_size(&_ch), mmstr_strg_none);
	}
	else {
		mmint_t total_len = _count * MemeRune_size(&_ch);
		mmvbstk_t vbuf;
		int result = 0;

		MemeVariableBufferStack_init(&vbuf, MMSTR__OBJ_SIZE);
		result = (int)MemeVariableBuffer_resize((mmvb_ptr_t)&vbuf, total_len);
		if (result) {
			MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
			return result;
		}
		
		for (mmint_t idx = 0; idx < _count; ++idx) {
			result = (int)MemeVariableBuffer_appendWithBytes((mmvb_ptr_t)&vbuf, MemeRune_data(&_ch), MemeRune_size(&_ch));
			if (result) {
				MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
				return result;
			}
		}

		result = MemeStringStack_initByU8bytesAndType(
			_out, _object_size, MemeVariableBuffer_data((mmvb_ptr_t)&vbuf), total_len, mmstr_strg_none);
		
		MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);

		return result;
	}
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
	
	assert(_out  != NULL && MemeStringStack_initWithHexadecimals != NULL);

	if (MEGO_SYMBOL__UNLIKELY(_hexs == NULL))
		return mmstrstk_init_v0(_out, _object_size);

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

MEME_EXTERN_C MEME_API int
MEME_STDCALL MemeStringStack_initAndConditionalConvert(
	mmstrstk_t* _out, size_t _object_size, mmstr_cptr_t _other)
{
    assert(_out != NULL);
	
    if (_other == NULL) {
		return mmstrstk_init_v0(_out, _object_size);
    }

    if (MemeString_storageType(_other) == MemeString_StorageType_user 
	 || MemeString_storageType(_other) == MemeString_UnsafeStorageType_view)
	{
        return MemeStringStack_initByU8bytes(
			_out, MMS__OBJECT_SIZE, MemeString_byteData(_other), MemeString_byteSize(_other));
    }
    else {
        return MemeStringStack_initByOther(_out, MMS__OBJECT_SIZE, _other);
    }
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_reset(
	mmsstk_t* _out, size_t _object_size)
{
	mms_t obj = NULL;

	assert(_out);

	obj = (mms_t)_out;
	switch (MMSTR__GET_IMPLTYPE(obj)) {
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

	assert(_s != NULL && "MemeStringStack_assign");

	if (_other == NULL)
		return mmstrstk_reset_v0(_s, _object_size);
	
	if (((void*)_s == (void*)_other))
		return 0;
	
	mmstrstk_uninit_v0(_s, 0);
	
	// switch (MMSTR__GET_IMPLTYPE(_other))
	// {
	// case MemeString_ImplType_small:
	// case MemeString_ImplType_medium:
	// case MemeString_ImplType_large:
	// case MemeString_ImplType_user:
	// case MemeString_ImplType_view:
	// 	break;
	// default: {
	// 	mmstrstk_init_v0(_s, _object_size);
	// 	return (MGEC__OPNOTSUPP);
	// };
	// }

	switch (MMSTR__GET_IMPLTYPE(_other))
	{
	case MemeString_ImplType_small: {
		memcpy(_s, &(_other->small_), MEME_STRING__OBJECT_SIZE);
	} break;
	case MemeString_ImplType_medium: {
		result = MemeStringMedium_initWithCapacity((MemeStringMedium_t*)_s, MemeString_byteSize(_other));
		if (result != 0) {
			mmstrstk_uninit_v0(_s, _object_size);
			return result;
		}
		MemeStringMedium_assign((MemeStringMedium_t*)_s, 
			MemeString_byteData(_other), MemeString_byteSize(_other));
	} break;
	case MemeString_ImplType_large: {
		MemeStringLarge_RefCount_increment(_other->large_.ref_);
		memcpy(_s, &(_other->large_), MEME_STRING__OBJECT_SIZE);
	} break;
	case MemeString_ImplType_user: {
		MemeStringUser_RefCount_increment(_other->user_.ref_);
		memcpy(_s, &(_other->user_), MEME_STRING__OBJECT_SIZE);
	} break;
	case MemeString_ImplType_view: {
		memcpy(_s, &(_other->viewUnsafe_), MEME_STRING__OBJECT_SIZE);
	} break;
	default: {
		mmstrstk_init_v0(_s, _object_size);
		return (MGEC__OPNOTSUPP);
	};
	}
	return 0;
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_assignByU8bytes(
	mmsstk_t* _s, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len)
{
	int result = 0;

	assert(_s != NULL && MemeStringStack_assignByU8bytes != NULL);

	if (_utf8 == NULL)
		return mmstrstk_reset_v0((mmstrstk_t*)_s, _object_size);
	
	if ((MemeString_byteData((mms_t)_s) == _utf8))
		return 0;

	result = MemeStringStack_unInit(_s, sizeof(mmsstk_t));
	if ((result != 0))
		return result;
	result = MemeStringStack_initByU8bytes(_s, sizeof(mmsstk_t), _utf8, _len);
	return result;
}

/**
 * @brief Assign the content of a MemeBuffer slice to an initialized stack string.
 *
 * Implementation steps:
 *  1. Assert that both @p _s and @p _other are non-NULL.
 *  2. @c NULL-source guard – if @p _other is @c NULL, reset @p _s to an empty
 *     string via @c mmstrstk_reset_v0() and return @c 0.
 *  3. Self-assignment guard – if @p _s and @p _other share the same address,
 *     return @c 0 without touching @p _s.
 *  4. Uninitialize @p _s via MemeStringStack_unInit() to release any
 *     heap-allocated storage.  On failure, return the error immediately; @p _s
 *     is left in an indeterminate state.
 *  5. Reinitialize @p _s from @p _other via MemeStringStack_initByBuffer(),
 *     which applies @p _offset, strips trailing null bytes, and selects the
 *     appropriate storage tier.  On failure, @p _s is in an indeterminate
 *     state; do not call MemeStringStack_unInit() on it.
 *
 * @param[in,out] _s           Already-initialized stack string to overwrite.
 *                             Must not be @c NULL (asserted).  On success,
 *                             contains the new content.  On failure, left in an
 *                             indeterminate state; do not call
 *                             MemeStringStack_unInit() on it.
 * @param[in]     _object_size Byte size of the @p _s instance.
 * @param[in]     _other       Source buffer.  Must not be @c NULL (asserted).
 *                             If @c NULL at runtime (defensive path), @p _s is
 *                             reset and @c 0 is returned.
 * @param[in]     _offset      Byte offset into @p _other.  Negative values are
 *                             treated as @c 0 by MemeStringStack_initByBuffer().
 *
 * @return @c 0 on success, or a non-zero error code on failure.
 */
MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_assignByBuffer(
	mmsstk_t* _s, size_t _object_size, MemeBuffer_Const_t _other, MemeInteger_t _offset)
{
	int result = 0;

	assert(_s != NULL     && MemeStringStack_assignByBuffer != NULL);
	assert(_other != NULL && MemeStringStack_assignByBuffer != NULL);

	if (_other == NULL)
		return mmstrstk_reset_v0(_s, _object_size);
	
	if (((void*)_s == (void*)_other))
		return 0;

	result = MemeStringStack_unInit(_s, sizeof(mmsstk_t));
	if ((result != 0))
		return result;
	return MemeStringStack_initByBuffer(_s, sizeof(mmsstk_t), _other, _offset);
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_regSize(const mmstrstk_t* _str)
{
	assert(_str && "MemeString_regSize");
	return MemeStringImpl_objRegSize((mmstr_cptr_t)_str);
}

MEME_EXTERN_C MEME_API int MEME_STDCALL MemeStringStack_objSize(const mmstrstk_t* _str)
{
    assert(_str && "MemeString_objSize");
    return MemeStringImpl_objByteSize((mmstr_cptr_t)_str);
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL
MemeStringStack_mid(
	const mmsstk_t* _s, size_t _object_size, MemeInteger_t _offset, MemeInteger_t _len)
{
	mmstrstk_t out;

    assert(_s != NULL && MemeStringStack_mid != NULL);

	MemeStringStack_mid_v2(_s, _offset, _len, &out, MMSTR__OBJ_SIZE);
	return out;

 //   obj = (mms_t)_s;
 //   srcSize = MemeString_byteSize(obj);
 //   if ((_offset > srcSize))
 //       return mmstrstk_get_init_v0(_object_size);
	//
 //   if ((_len < 0))
 //       _len = srcSize - _offset;
 //   else if ((_len > srcSize - _offset))
 //       _len = srcSize - _offset;

 //   if (MemeString_isSharedStorageTypes(obj) && _len + _offset == srcSize)
 //   {
	//	int eno = MemeStringStack_initByOther(&out, _object_size, obj);
	//	if (eno)
	//		return mmstrstk_get_init_v0(_object_size);
	//	MemeStringImpl_setDataOffset(&out, _offset);
 //   }
	//else if (MMSTR__GET_IMPLTYPE(obj) == MemeString_ImplType_view)
	//{
 //       MemeStringViewUnsafeStack_init(&out, _object_size,
 //           MemeString_byteData(obj) + _offset, _len);
	//}
	//else {
 //       int eno = MemeStringStack_initByU8bytes(&out, _object_size,
 //           MemeString_byteData(obj) + _offset, _len);
 //       if (eno)
 //           return mmstrstk_get_init_v0(_object_size);
	//}
 //   return out;
}

MEME_EXTERN_C MEME_API mgec_t
MEME_STDCALL MemeStringStack_mid_v2(
	const mmstrstk_t* _str, mmint_t _offset, mmint_t _count, mmstrstk_t* _out, mmint_t _obj_size)
{
	mmstr_cptr_t str = (mmstr_cptr_t)_str;
	mmint_t srcSize = 0;

	assert(_str != NULL && "MemeStringStack_mid_v2");
	assert(_out != NULL && "MemeStringStack_mid_v2");

	if (MG_SYM__UNLIKELY(_offset < 0))
		_offset = 0;

	if (_obj_size <= 0) {
		_obj_size = MemeStringImpl_objByteSize((mmstr_cptr_t)_out);
        mmstrstk_uninit_v0(_out, 0);
	}

	srcSize = MemeString_byteSize(str);
	if (MG_SYM__UNLIKELY(_offset > srcSize))
	{
		return mmstrstk_init_v0(_out, (size_t)_obj_size);
	}
	
	if (_count < 0)
		_count = srcSize - _offset;
	else if (_count > srcSize - _offset)
		_count = srcSize - _offset;

	if (MemeString_isSharedStorageTypes(str) && _offset == 0 && _count == srcSize)
	{
		mgec_t eno = 0;
		eno = MemeStringStack_initByOther(_out, (size_t)_obj_size, str);
		if (eno)
			return eno;
		MemeStringImpl_setDataOffset(_out, _offset);
	}
	else if (MMSTR__GET_IMPLTYPE(str) == MemeString_ImplType_view)
	{
		MemeStringViewUnsafeStack_init(
			_out, (size_t)_obj_size, MemeString_byteData(str) + _offset, _count);
	}
	else {
		mgec_t eno = 0;
		eno = MemeStringStack_initByU8bytes(
				_out, (size_t)_obj_size, MemeString_byteData(str) + _offset, _count);
		if (eno)
			return eno;
	}
	
	return 0;
}

MEME_EXTERN_C MEME_API mmstrstk_t MEME_STDCALL MemeStringStack_concat(
	const mmstrstk_t* _s, size_t _object_size, const mmstrstk_t* _other)
{
    mmstrstk_t out;
	
	MemeStringStack_concat_v2(_s, _other, &out, MMSTR__OBJ_SIZE);
	return out;

 //   assert(_s != NULL && MemeStringStack_concat != NULL);
	//assert(_other != NULL && MemeStringStack_concat != NULL);
	//
 //   lhsSize = MemeString_byteSize((mmstr_const_t)_s);
 //   rhsSize = MemeString_byteSize((mmstr_const_t)_other);
	//
 //   if (lhsSize + rhsSize == 0)
 //       return mmstrstk_get_init_v0(_object_size);

	//result = MemeStringMedium_initWithCapacity((MemeStringMedium_t*)&out, 
	//	lhsSize + rhsSize);
 //   if (result)
 //       return mmstrstk_get_init_v0(_object_size);
	//
	//result = MemeVariableBuffer_appendWithBytes(
	//	(mmvb_t)&out, MemeString_byteData((mmstr_const_t)_s), lhsSize);
 //   if (result)
 //       return mmstrstk_get_init_v0(_object_size);
	//
	//result = MemeVariableBuffer_appendWithBytes(
	//	(mmvb_t)&out, MemeString_byteData((mmstr_const_t)_other), rhsSize);
 //   if (result)
 //       return mmstrstk_get_init_v0(_object_size);

    //return out;
}

MEME_EXTERN_C MEME_API mgec_t MEME_STDCALL MemeStringStack_concat_v2(
	const mmstrstk_t* _str, const mmstrstk_t* _other, mmstrstk_t* _out, mmint_t _obj_size)
{
	mgec_t  result  = 0;
	mmint_t rhsSize = 0;
	mmint_t lhsSize = 0;

	assert(_str != NULL && "MemeStringStack_concat_v2");
	assert(_other != NULL && "MemeStringStack_concat_v2");

	lhsSize = MemeString_byteSize((mmstr_cptr_t)_str);
	rhsSize = MemeString_byteSize((mmstr_cptr_t)_other);

	if (_obj_size <= 0) {
		_obj_size = MemeStringImpl_objByteSize((mmstr_cptr_t)_out);
		mmstrstk_uninit_v0(_out, 0);
	}

	if (lhsSize + rhsSize == 0) {
		return mmstrstk_init_v0(_out, (size_t)_obj_size);
	}

	result = MemeStringMedium_initWithCapacity((MemeStringMedium_t*)_out, lhsSize + rhsSize);
	if (result) {
		mmstrstk_init_v0(_out, (size_t)_obj_size);
		return result;
	}

	result = (mgec_t)MemeVariableBuffer_appendWithBytes(
		(mmvb_ptr_t)_out, MemeString_byteData((mmstr_cptr_t)_str), lhsSize);
	if (result) {
		mmstrstk_reset_v0(_out, 0);
		return result;
	}

	result = (mgec_t)MemeVariableBuffer_appendWithBytes(
		(mmvb_ptr_t)_out, MemeString_byteData((mmstr_cptr_t)_other), rhsSize);
	if (result) {
		mmstrstk_reset_v0(_out, 0);
		return result;
	}

	return 0;
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL
MemeStringStack_toEnUpper(
	const mmsstk_t* _s, size_t _object_size)
{
	mmstrstk_t stack;
	MemeStringStack_toEnUpper_v2(_s, &stack, MMSTR__OBJ_SIZE);
	return stack;

	//int result = 0;
	//mmsstk_t stack;
	//mms_t s = (mms_t)_s;
	//MemeByte_t* it = NULL;
	//MemeByte_t* end = NULL;

	//assert(_s != NULL && MemeStringStack_toEnUpper != NULL);

	//result = MemeStringStack_initByU8bytes(&stack, sizeof(stack),
	//	MemeString_byteData(s), MemeString_byteSize(s));
	//if (result) {
	//	MemeStringStack_init(&stack, sizeof(stack));
	//	return stack;
	//}

	//it  = MemeStringImpl_forcedData(&stack);
	//end = it + MemeString_byteSize((mms_t)&stack);

	//for (; it != end; ++it)
	//	*it = (MemeByte_t)toupper(*it);
	//
	//return stack;
}

/**
 * @brief Convert all ASCII lowercase letters in a stack string to uppercase.
 *
 * Implementation steps:
 *  1. Assert that @p _str is non-NULL.
 *  2. If @p _obj_size is non-positive, read the actual object size from
 *     @p _out's internal metadata via MemeStringImpl_objByteSize(), then
 *     uninitialize @p _out with @c mmstrstk_uninit() to release any existing
 *     storage.
 *  3. Initialize @p _out by copying every byte of @p _str via
 *     MemeStringStack_initByU8bytes().  If this step fails, explicitly
 *     reinitialize @p _out to an empty string with @c mmstrstk_init_v0() and
 *     return the error code.  @p _out is always left in a valid state.
 *  4. Obtain a mutable pointer to @p _out's raw byte buffer via
 *     @c MemeStringImpl_forcedData().
 *  5. Iterate over every byte in the buffer and apply @c toupper() in-place.
 *     Only ASCII bytes in 'a'–'z' (0x61–0x7A) are changed; all other bytes,
 *     including multi-byte UTF-8 sequences, are left untouched.
 *
 * @note The byte-by-byte loop is a known optimization opportunity
 *       (marked TO_DO in the source).  SIMD or lookup-table acceleration
 *       could improve throughput for large strings.
 *
 * @param[in]  _str      Source string.  Must be initialized and non-NULL.
 * @param[out] _out      Receives the result.  Positive @p _obj_size means
 *                       uninitialized on entry; non-positive means already
 *                       initialized.  Always left in a valid initialized state
 *                       on return (empty string on failure).
 * @param[in]  _obj_size Object byte size of @p _out, or a non-positive value
 *                       if @p _out is already initialized.
 *
 * @return @c 0 on success, or a non-zero error code from
 *         MemeStringStack_initByU8bytes() on failure.
 */
MEME_EXTERN_C MEME_API mgec_t MEME_STDCALL
MemeStringStack_toEnUpper_v2(const mmstrstk_t* _str, mmstrstk_t* _out, mmint_t _obj_size)
{
	mgec_t result = 0;
	mmstr_cptr_t str = (mmstr_cptr_t)_str;
	mmbyte_t* it  = NULL;
	mmbyte_t* end = NULL;

	assert(_str != NULL && "MemeStringStack_toEnUpper_v2");
	
	if (_obj_size <= 0) {
        _obj_size = MemeStringImpl_objByteSize((mmstr_cptr_t)_out);
		mmstrstk_uninit(_out);
	}

	result = MemeStringStack_initByU8bytes(
		_out, _obj_size, MemeString_byteData(str), MemeString_byteSize(str));
	if (result) {
		mmstrstk_init_v0(_out, _obj_size);
		return result;
	}

	it = MemeStringImpl_forcedData(_out);
	end = it + MemeString_byteSize((mmstr_cptr_t)_out);

	// TO_DO : It can also be optimized
	for (; it != end; ++it)
		*it = (mmbyte_t)toupper(*it);

	return 0;
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL
MemeStringStack_toEnLower(
	const mmsstk_t* _s, size_t _object_size)
{
	mmstrstk_t stack;
	MemeStringStack_toEnLower_v2(_s, &stack, MMSTR__OBJ_SIZE);
	return stack;

	//int result = 0;
	//mmsstk_t stack;
	//mms_t s = (mms_t)_s;
	//MemeByte_t* it  = NULL;
	//MemeByte_t* end = NULL;

	//assert(_s != NULL && MemeStringStack_toEnLower != NULL);

	//result = MemeStringStack_initByU8bytes(&stack, sizeof(stack),
	//	MemeString_byteData(s), MemeString_byteSize(s));
	//if (result) {
	//	MemeStringStack_init(&stack, sizeof(stack));
	//	return stack;
	//}

	//it = MemeStringImpl_forcedData(&stack);
	//end = it + MemeString_byteSize((mms_t)&stack);

	//for (; it != end; ++it)
	//	*it = (MemeByte_t)tolower(*it);

	//return stack;
}

/**
 * @brief Convert all ASCII uppercase letters in a stack string to lowercase.
 *
 * Implementation steps:
 *  1. Assert that @p _str is non-NULL.
 *  2. If @p _obj_size is non-positive, read the actual object size from
 *     @p _out's internal metadata via MemeStringImpl_objByteSize(), then
 *     uninitialize @p _out with @c mmstrstk_uninit() to release any existing
 *     storage.
 *  3. Initialize @p _out by copying every byte of @p _str via
 *     MemeStringStack_initByU8bytes().  If this step fails, explicitly
 *     reinitialize @p _out to an empty string with @c mmstrstk_init_v0() and
 *     return the error code.  @p _out is always left in a valid state.
 *  4. Obtain a mutable pointer to @p _out's raw byte buffer via
 *     @c MemeStringImpl_forcedData().
 *  5. Iterate over every byte in the buffer and apply @c tolower() in-place.
 *     Only ASCII bytes in 'A'–'Z' (0x41–0x5A) are changed; all other bytes,
 *     including multi-byte UTF-8 sequences, are left untouched.
 *
 * @note The byte-by-byte loop is a known optimization opportunity
 *       (marked TO_DO in the source).  SIMD or lookup-table acceleration
 *       could improve throughput for large strings.
 *
 * @param[in]  _str      Source string.  Must be initialized and non-NULL.
 * @param[out] _out      Receives the result.  Positive @p _obj_size means
 *                       uninitialized on entry; non-positive means already
 *                       initialized.  Always left in a valid initialized state
 *                       on return (empty string on failure).
 * @param[in]  _obj_size Object byte size of @p _out, or a non-positive value
 *                       if @p _out is already initialized.
 *
 * @return @c 0 on success, or a non-zero error code from
 *         MemeStringStack_initByU8bytes() on failure.
 */
MEME_EXTERN_C MEME_API mgec_t MEME_STDCALL
MemeStringStack_toEnLower_v2(const mmstrstk_t* _str, mmstrstk_t* _out, mmint_t _obj_size)
{
	mgec_t result = 0;
	mmstr_cptr_t str = (mmstr_cptr_t)_str;
	mmbyte_t* it  = NULL;
	mmbyte_t* end = NULL;

	assert(_str != NULL && "MemeStringStack_toEnLower_v2");

	if (_obj_size <= 0) {
		_obj_size = MemeStringImpl_objByteSize((mmstr_cptr_t)_out);
		mmstrstk_uninit(_out);
	}

	result = MemeStringStack_initByU8bytes(
		_out, _obj_size, MemeString_byteData(str), MemeString_byteSize(str));
	if (result) {
		mmstrstk_init_v0(_out, _obj_size);
		return result;
	}

	it = MemeStringImpl_forcedData(_out);
	end = it + MemeString_byteSize((mmstr_cptr_t)_out);

	// TO_DO : It can also be optimized
	for (; it != end; ++it)
		*it = (mmbyte_t)tolower(*it);

	return 0;
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL
MemeStringStack_trimSpace(const mmsstk_t* _s, size_t _object_size)
{
	mmstrstk_t stack;
	MemeStringStack_trimSpace_v2(_s, &stack, MMSTR__OBJ_SIZE);
	return stack;

	//int result = 0;
	//mmsstk_t stack;
	//mms_t s = (mms_t)_s;
	//const MemeByte_t* it  = NULL;
	//const MemeByte_t* end = NULL;

	//assert(_s != NULL && MemeStringStack_trimSpace != NULL);

	//it = MemeString_byteData(s);
	//end = it + MemeString_byteSize(s);

	//for (int runeSize = -1; it != end; it += runeSize)
	//{
	//	runeSize = mmutf_u8rune_char_size(*it);
	//	if (runeSize < 0) {
	//		break;
	//	}
	//	else {
	//		if (!MemeRuneIndex_isSpace(it, runeSize))
	//			break;
	//	}
	//}
	//
	//for (int runeSize = -1; it != end; end -= runeSize)
	//{
 //       runeSize = mmutf_u8rune_prev_char_size(it, end);
 //       if (runeSize < 0) {
 //           break;
 //       }
 //       else {
 //           if (!MemeRuneIndex_isSpace(end - runeSize, runeSize))
 //               break;
 //       }
	//}
 //   
 //   stack = MemeStringStack_mid(_s, _object_size, it - MemeString_byteData(s), end - it);
 //   return stack;
}

/**
 * @brief Strip leading and trailing Unicode whitespace from a stack string.
 *
 * Implementation detail:
 *
 * Two pointer scans are performed over the raw UTF-8 byte buffer obtained from
 * @p _str:
 *
 * 1. <b>Left scan</b> – @c it advances forward one rune at a time.
 *    @c mmutf_u8rune_char_size() decodes the leading byte of the current rune
 *    to determine the number of bytes it occupies.  If the rune is a
 *    whitespace character according to @c MemeRuneIndex_isSpace(), @c it
 *    advances past it; otherwise the scan stops.  A negative return value
 *    from the size helper (ill-formed UTF-8) also halts the scan.
 *
 * 2. <b>Right scan</b> – @c end retreats backward one rune at a time.
 *    @c mmutf_u8rune_prev_char_size() walks back through the continuation
 *    bytes to locate the start of the preceding rune and returns its byte
 *    length.  If that rune is whitespace, @c end is moved back by that many
 *    bytes; otherwise the scan stops.  Ill-formed sequences also halt the
 *    scan.
 *
 * After both scans, the half-open byte range [@c it, @c end) contains only
 * non-whitespace content (plus any interior whitespace).  The byte offset
 * and byte count of this range are forwarded to @c MemeStringStack_mid_v2,
 * which allocates and initialises @p _out.
 *
 * @param[in]  _str      The source string to trim.  Must be initialized.
 *                       Must not be @c NULL (enforced by assert).
 * @param[out] _out      Receives the trimmed result.  Treated as uninitialized
 *                       raw storage when @p _obj_size is positive, or as an
 *                       already-initialized object when @p _obj_size is
 *                       negative (see @c MemeStringStack_mid_v2 for the full
 *                       initialization contract).  On failure, @p _out is left
 *                       in an indeterminate state; do not call
 *                       @c MemeStringStack_unInit() on it.
 * @param[in]  _obj_size Byte size of the @p _out object
 *                       (e.g. @c MMSTR__OBJ_SIZE), or any negative value if
 *                       @p _out is already initialized.
 *
 * @return @c 0 on success, or a non-zero @c mgec_t error code propagated from
 *         @c MemeStringStack_mid_v2.
 */
MEME_EXTERN_C MEME_API mgec_t MEME_STDCALL
MemeStringStack_trimSpace_v2(const mmstrstk_t* _str, mmstrstk_t* _out, mmint_t _obj_size)
{
	mgec_t result = 0;
	mmstr_cptr_t str = (mmstr_cptr_t)_str;
	const mmbyte_t* it  = NULL;
	const mmbyte_t* end = NULL;

	assert(_str != NULL && "MemeStringStack_trimSpace_v2");

	it  = MemeString_byteData(str);
	end = it + MemeString_byteSize(str);

	/* Left scan: skip leading whitespace runes. */
	for (int runeSize = -1; it != end; it += runeSize)
	{
		runeSize = mmutf_u8rune_char_size(*it);
		if (runeSize < 0) {
			break;
		}
		else {
			if (!MemeRuneIndex_isSpace(it, runeSize))
				break;
		}
	}

	/* Right scan: skip trailing whitespace runes. */
	for (int runeSize = -1; it != end; end -= runeSize)
	{
		runeSize = mmutf_u8rune_prev_char_size(it, end);
		if (runeSize < 0) {
			break;
		}
		else {
			if (!MemeRuneIndex_isSpace(end - runeSize, runeSize))
				break;
		}
	}

	/* Slice the byte range [it, end) into _out. */
	return MemeStringStack_mid_v2(_str, it - MemeString_byteData(str), end - it, _out, _obj_size);
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL
MemeStringStack_trimLeftSpace(const mmsstk_t* _s, size_t _object_size)
{
	mmstrstk_t stack;
	MemeStringStack_trimLeftSpace_v2(_s, &stack, MMSTR__OBJ_SIZE);
	return stack;

 //   int result = 0;
	//mmsstk_t stack;
 //   mms_t s  = (mms_t)_s;
 //   const MemeByte_t* it  = NULL;
	//const MemeByte_t* begin = NULL;
	//const MemeByte_t* end = NULL;

 //   assert(_s != NULL && MemeStringStack_trimLeftSpace != NULL);

	//begin = it = MemeString_byteData(s);
 //   end   = it + MemeString_byteSize(s);

 //   for (int runeSize = -1; it != end; it += runeSize)
	//{
 //       runeSize = mmutf_u8rune_char_size(*it);
	//	if (runeSize < 0) {
	//		break;
	//	}
	//	else {
	//		if (!MemeRuneIndex_isSpace(it, runeSize))
	//			break;
	//	}
	//}

 //   stack = MemeStringStack_mid(_s, _object_size, it - begin, -1);
 //   return stack;
}

MEME_EXTERN_C MEME_API mgec_t MEME_STDCALL
MemeStringStack_trimLeftSpace_v2(const mmstrstk_t* _str, mmstrstk_t* _out, mmint_t _obj_size)
{
	mgec_t result = 0;
	mmstr_cptr_t str = (mmstr_cptr_t)_str;
	const mmbyte_t* it = NULL;
	const mmbyte_t* begin = NULL;
	const mmbyte_t* end = NULL;

	assert(str != NULL && "MemeStringStack_trimLeftSpace_v2");

	begin = it = MemeString_byteData(str);
	end = it + MemeString_byteSize(str);

	for (int runeSize = -1; it != end; it += runeSize)
	{
		runeSize = mmutf_u8rune_char_size(*it);
		if (runeSize < 0) {
			break;
		}
		else {
			if (!MemeRuneIndex_isSpace(it, runeSize))
				break;
		}
	}

	return MemeStringStack_mid_v2(_str, it - begin, -1, _out, _obj_size);
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL
MemeStringStack_trimRightSpace(const mmsstk_t* _s, size_t _object_size)
{
	mmstrstk_t stack;
	MemeStringStack_trimRightSpace_v2(_s, &stack, MMSTR__OBJ_SIZE);
	return stack;

 //   int result = 0;
	//mmsstk_t stack;
 //   mms_t s = (mms_t)_s;
	//const MemeByte_t* it  = NULL;
	//const MemeByte_t* end = NULL;

 //   assert(_s != NULL && MemeStringStack_trimRightSpace != NULL);

 //   it = MemeString_byteData(s);
 //   end = it + MemeString_byteSize(s);

	//for (int runeSize = -1; it != end; end -= runeSize)
	//{
	//	runeSize = mmutf_u8rune_prev_char_size(it, end);
	//	if (runeSize < 0) {
	//		break;
	//	}
	//	else {
	//		if (!MemeRuneIndex_isSpace(end - runeSize, runeSize))
	//			break;
	//	}
	//}
 //   
 //   stack = MemeStringStack_mid(_s, _object_size, 0, end - it);
 //   return stack;
}

MEME_EXTERN_C MEME_API mgec_t MEME_STDCALL
MemeStringStack_trimRightSpace_v2(const mmstrstk_t* _str, mmstrstk_t* _out, mmint_t _obj_size)
{
	mgec_t result = 0;
	mmstr_cptr_t str = (mmstr_cptr_t)_str;
	const mmbyte_t* it  = NULL;
	const mmbyte_t* end = NULL;

	assert(_str != NULL && "MemeStringStack_trimRightSpace_v2");

	it = MemeString_byteData(str);
	end = it + MemeString_byteSize(str);

	for (int runeSize = -1; it != end; end -= runeSize)
	{
		runeSize = mmutf_u8rune_prev_char_size(it, end);
		if (runeSize < 0) {
			break;
		}
		else {
			if (!MemeRuneIndex_isSpace(end - runeSize, runeSize))
				break;
		}
	}
	
	return MemeStringStack_mid_v2(_str, 0, end - it, _out, _obj_size);
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL
MemeStringStack_trimByCuts(
	const mmsstk_t* _s, size_t _object_size, const char* _cuts, MemeInteger_t _cuts_len)
{
	mmstrstk_t stack;
    MemeStringStack_trimByCuts_v2(_s, _cuts, _cuts_len, &stack, MMSTR__OBJ_SIZE);
    return stack;

 //   int result = 0;
	//mmsstk_t stack;
 //   mms_t s = (mms_t)_s;
 //   const MemeByte_t* it  = NULL;
 //   const MemeByte_t* end = NULL;

 //   assert(_s != NULL && MemeStringStack_trimByCuts != NULL);

 //   it = MemeString_byteData(s);
 //   end = it + MemeString_byteSize(s);

 //   for (; it != end; ++it)
 //       if (!memchr(_cuts, *it, _cuts_len))
 //           break;

 //   if (it != end)
 //       --end;
 //   for (; it - 1 != end; --end)
 //       if (!memchr(_cuts, *it, _cuts_len))
 //           break;

 //   stack = MemeStringStack_mid(_s, _object_size, it - MemeString_byteData(s), end - it + 1);
 //   return stack;
}

MEME_API mgec_t MEME_STDCALL 
MemeStringStack_trimByCuts_v2(
	const mmstrstk_t* _str, const char* _cuts, mmint_t _cuts_len, mmstrstk_t* _out, mmint_t _obj_size)
{
	mmstr_ptr_t str = (mmstr_ptr_t)_str;
	const mmbyte_t* it  = NULL;
	const mmbyte_t* end = NULL;

	assert(_str != NULL && "MemeStringStack_trimByCuts_v2");

	if (_cuts_len < 0)
        _cuts_len = strlen(_cuts);

	it = MemeString_byteData(str);
	end = it + MemeString_byteSize(str);

	for (; it != end; ++it)
		if (!memchr(_cuts, *it, _cuts_len))
			break;

	if (it != end)
		--end;
	for (; it - 1 != end; --end)
		if (!memchr(_cuts, *end, _cuts_len))
			break;

	return MemeStringStack_mid_v2(_str, it - MemeString_byteData(str), end - it + 1, _out, _obj_size);
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL
MemeStringStack_trimByCondByteFunc(
	const mmsstk_t* _s, size_t _object_size,
	MemeString_MatchCondByteFunc_t* _cond_func, void* _user_data)
{
	mmstrstk_t stack;
    MemeStringStack_trimByCondByteFunc_v2(_s, _cond_func, _user_data, &stack, MMSTR__OBJ_SIZE);
    return stack;

 //   int result = 0;
	//mmsstk_t stack;
 //   mms_t s = (mms_t)_s;
 //   const MemeByte_t* it = NULL;
 //   const MemeByte_t* end = NULL;

 //   assert(_s != NULL && MemeStringStack_trimByCondByteFunc != NULL);

 //   it = MemeString_byteData(s);
 //   end = it + MemeString_byteSize(s);

 //   for (; it != end; ++it)
 //       if (!_cond_func(*it, _user_data))
 //           break;

 //   if (it != end)
 //       --end;
 //   for (; it - 1 != end; --end)
 //       if (!_cond_func(*end, _user_data))
 //           break;

 //   stack = MemeStringStack_mid(_s, _object_size, it - MemeString_byteData(s), end - it + 1);
 //   return stack;
}

MEME_API mgec_t MEME_STDCALL 
MemeStringStack_trimByCondByteFunc_v2(
	const mmstrstk_t* _str, mmstr_match_cond_byte_cb_t* _cond_func, void* _user_data, 
	mmstrstk_t* _out, mmint_t _obj_size)
{
	mmstr_ptr_t str = (mmstr_ptr_t)_str;
	const mmbyte_t* it  = NULL;
	const mmbyte_t* end = NULL;

	assert(_str != NULL && "MemeStringStack_trimByCondByteFunc_v2");

	it = MemeString_byteData(str);
	end = it + MemeString_byteSize(str);

	for (; it != end; ++it)
		if (!_cond_func(*it, _user_data))
			break;

	if (it != end)
		--end;
	for (; it - 1 != end; --end)
		if (!_cond_func(*end, _user_data))
			break;

	return MemeStringStack_mid_v2(_str, it - MemeString_byteData(str), end - it + 1, _out, _obj_size);
}

MEME_API mgec_t MEME_STDCALL
MemeStringStack_trimByCondRuneFunc(
	const mmstrstk_t* _str, mmstr_match_cond_rune_cb_t* _cond_func, void* _user_data,
	mmstrstk_t* _out, mmint_t _obj_size)
{
	mmstr_cptr_t str = (mmstr_cptr_t)_str;
	const mmbyte_t* begin = NULL;
	const mmbyte_t* it    = NULL;
	const mmbyte_t* end   = NULL;
	int runeSize = 0;

	assert(_str != NULL && "MemeStringStack_trimByCondRuneFunc");

	begin = MemeString_byteData(str);
	it    = begin;
	end   = begin + MemeString_byteSize(str);

	for (; it != end; it += runeSize) {
		MemeRune_t rune;
		runeSize = mmutf_u8rune_char_size(*it);
		if (runeSize <= 0)
			break;
		MemeRune_initByUtf8Bytes(&rune, it, runeSize);
		if (!_cond_func(&rune, _user_data))
			break;
	}

	for (; it != end;) {
		MemeRune_t rune;
		runeSize = mmutf_u8rune_prev_char_size(it, end);
		if (runeSize <= 0)
			break;
		MemeRune_initByUtf8Bytes(&rune, end - runeSize, runeSize);
		if (!_cond_func(&rune, _user_data))
			break;
		end -= runeSize;
	}

	return MemeStringStack_mid_v2(_str, it - begin, end - it, _out, _obj_size);
}

/**
 * @brief Return a copy of a stack string with a given byte prefix removed.
 *
 * Implementation steps:
 *  1. Assert that @p _str is non-NULL.
 *  2. If @p _prefix_len is negative, compute it via @c strlen(@p _prefix).
 *  3. If @p _obj_size is non-positive, read the actual object size from
 *     @p _out's internal metadata via MemeStringImpl_objByteSize(), then
 *     uninitialize @p _out via @c mmstrstk_uninit_v0().
 *  4. Obtain @p srcSize = MemeString_byteSize(@p _str).
 *  5. <b>Prefix-found path</b>: if @p _prefix_len > 0, srcSize ≥ @p _prefix_len,
 *     and @c memcmp(begin, @p _prefix, @p _prefix_len) == 0, delegate to
 *     MemeStringStack_mid_v2() with @p _offset = @p _prefix_len and
 *     @p _count = @c -1 (i.e. to end of string).  Return its result directly.
 *  6. <b>No-match path</b>: perform a redundant @p _obj_size re-check
 *     (effectively unreachable after step 3 already normalised @p _obj_size
 *     to a positive value), then copy @p _str into @p _out unchanged:
 *     - If @p _str uses view storage: MemeStringViewUnsafeStack_initByOther().
 *     - Otherwise: MemeStringStack_initByOther().
 *
 * @note On failure, @p _out is left in an indeterminate state.  Do not call
 *       MemeStringStack_unInit() on it if a non-zero error code is returned.
 *
 * @param[in]  _str        Source string.  Must be initialized and non-NULL.
 * @param[in]  _prefix     Byte sequence to match at the start of @p _str.
 * @param[in]  _prefix_len Byte length of @p _prefix, or < 0 for auto via strlen.
 * @param[out] _out        Receives the result.  Indeterminate on failure.
 * @param[in]  _obj_size   Object byte size of @p _out, or non-positive if
 *                         @p _out is already initialized.
 *
 * @return @c 0 on success, or a non-zero error code on failure.
 */
MEME_API mgec_t MEME_STDCALL
MemeStringStack_trimPrefix(
	const mmstrstk_t* _str, const char* _prefix, mmint_t _prefix_len, mmstrstk_t* _out, mmint_t _obj_size)
{
	mmstr_cptr_t str = (mmstr_cptr_t)_str;
	mmint_t srcSize = 0;
	mgec_t eno = 0;

	assert(_str != NULL && "MemeStringStack_trimPrefix");

	if (_prefix_len < 0)
		_prefix_len = (mmint_t)strlen(_prefix);

	if (_obj_size <= 0) {
		_obj_size = MemeStringImpl_objByteSize((mmstr_cptr_t)_out);
		mmstrstk_uninit_v0(_out, 0);
	}

	srcSize = MemeString_byteSize(str);
	if (_prefix_len > 0 && srcSize >= _prefix_len &&
		memcmp(MemeString_byteData(str), _prefix, _prefix_len) == 0)
	{
		return MemeStringStack_mid_v2(_str, _prefix_len, -1, _out, _obj_size);
	}

	if (_obj_size <= 0) {
		_obj_size = MemeStringImpl_objByteSize((mmstr_cptr_t)_out);
		mmstrstk_uninit_v0(_out, 0);
	}

	if (MMSTR__GET_IMPLTYPE(str) == mmstr_impltype_view)
		return MemeStringViewUnsafeStack_initByOther(_out, (size_t)_obj_size, _str);
	return MemeStringStack_initByOther(_out, (size_t)_obj_size, str);
}

/**
 * @brief Return a copy of a stack string with a given byte suffix removed.
 *
 * Implementation steps:
 *  1. Assert that @p _str is non-NULL.
 *  2. If @p _suffix_len is negative, compute it via @c strlen(@p _suffix).
 *  3. If @p _obj_size is non-positive, read the actual object size from
 *     @p _out's internal metadata via MemeStringImpl_objByteSize(), then
 *     uninitialize @p _out via @c mmstrstk_uninit_v0().
 *  4. Obtain @p srcSize = MemeString_byteSize(@p _str).
 *  5. <b>Suffix-found path</b>: if @p _suffix_len > 0, srcSize ≥ @p _suffix_len,
 *     and @c memcmp(begin + srcSize - @p _suffix_len, @p _suffix,
 *     @p _suffix_len) == 0, delegate to MemeStringStack_mid_v2() with
 *     @p _offset = @c 0 and @p _count = srcSize − @p _suffix_len.
 *     Return its result directly.
 *  6. <b>No-match path</b>: perform a redundant @p _obj_size re-check
 *     (effectively unreachable after step 3 already normalised @p _obj_size
 *     to a positive value), then copy @p _str into @p _out unchanged:
 *     - If @p _str uses view storage: MemeStringViewUnsafeStack_initByOther().
 *     - Otherwise: MemeStringStack_initByOther().
 *
 * @note On failure, @p _out is left in an indeterminate state.  Do not call
 *       MemeStringStack_unInit() on it if a non-zero error code is returned.
 *
 * @param[in]  _str        Source string.  Must be initialized and non-NULL.
 * @param[in]  _suffix     Byte sequence to match at the end of @p _str.
 * @param[in]  _suffix_len Byte length of @p _suffix, or < 0 for auto via strlen.
 * @param[out] _out        Receives the result.  Indeterminate on failure.
 * @param[in]  _obj_size   Object byte size of @p _out, or non-positive if
 *                         @p _out is already initialized.
 *
 * @return @c 0 on success, or a non-zero error code on failure.
 */
MEME_API mgec_t MEME_STDCALL
MemeStringStack_trimSuffix(
	const mmstrstk_t* _str, const char* _suffix, mmint_t _suffix_len, mmstrstk_t* _out, mmint_t _obj_size)
{
	mmstr_cptr_t str = (mmstr_cptr_t)_str;
	mmint_t srcSize = 0;
	mgec_t eno = 0;

	assert(_str != NULL && "MemeStringStack_trimSuffix");

	if (_suffix_len < 0)
		_suffix_len = (mmint_t)strlen(_suffix);

	if (_obj_size <= 0) {
		_obj_size = MemeStringImpl_objByteSize((mmstr_cptr_t)_out);
		mmstrstk_uninit_v0(_out, 0);
	}

	srcSize = MemeString_byteSize(str);
	if (_suffix_len > 0 && srcSize >= _suffix_len &&
		memcmp(MemeString_byteData(str) + srcSize - _suffix_len, _suffix, _suffix_len) == 0)
	{
		return MemeStringStack_mid_v2(_str, 0, srcSize - _suffix_len, _out, _obj_size);
	}
	
	if (_obj_size <= 0) {
		_obj_size = MemeStringImpl_objByteSize((mmstr_cptr_t)_out);
		mmstrstk_uninit_v0(_out, 0);
	}
	
	if (MMSTR__GET_IMPLTYPE(str) == mmstr_impltype_view)
		return MemeStringViewUnsafeStack_initByOther(_out, (size_t)_obj_size, _str);
	return MemeStringStack_initByOther(_out, (size_t)_obj_size, str);
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL MemeStringStack_getRepeat(
	size_t _object_size, mmint_t _count, const char* _s, mmint_t _len)
{
	mmstrstk_t stack;
    MemeStringStack_getRepeat_v2(&stack, MMSTR__OBJ_SIZE, _count, _s, _len);
    return stack;

	//mmint_t result = 0;
 //   mmsstk_t stack;
 //   mmvbstk_t vb;
	//
 //   assert(_s != NULL && MemeStringStack_getRepeat != NULL);
	//
	//result = MemeVariableBufferStack_init(&vb, _object_size);
 //   if (result) {
 //       MemeStringStack_init(&stack, _object_size);
	//	return stack;
	//}
	//
	//if (_len < 0)
 //       _len = strlen(_s);

 //   for (; _count > 0; --_count) {
	//	result = MemeVariableBuffer_appendWithBytes((mmvb_t)&vb, (const mmbyte_t*)_s, _len);
	//	if (result) {
	//		MemeStringStack_init(&stack, _object_size);
	//		return stack;
	//	}
	//}

	//result = MemeVariableBuffer_releaseToString((mmvb_t)&vb, &stack, _object_size);
	//if (result) {
	//	MemeStringStack_init(&stack, _object_size);
	//	return stack;
	//}
 //   return stack;
}

/**
 * @brief Construct a string by repeating a byte sequence a given number of times.
 *
 * Implementation steps:
 *  1. Assert @p _out is non-NULL.
 *  2. If @p _obj_size is non-positive, read the actual object size from
 *     @p _out's internal metadata (MemeStringStack_regSize() × sizeof(mmint_t))
 *     and uninitialize @p _out via @c mmstrstk_uninit_v0().
 *  3. Initialize a stack-local @c mmvbstk_t variable buffer @c vb via
 *     MemeVariableBufferStack_init().  On failure, initialize @p _out to an
 *     empty string and return the error.
 *  4. If @p _in_len < 0, resolve it once via @c strlen(@p _in) (or 0 if
 *     @p _in is @c NULL) before entering the loop.
 *  5. Loop @p _count times (skipped entirely when @p _count ≤ 0):
 *     each iteration calls MemeVariableBuffer_appendWithBytes() with
 *     (@p _in, @p _in_len).  On any append failure, initialize @p _out to
 *     an empty string, uninitialize @c vb, and return the error.
 *  5. Call MemeVariableBuffer_releaseToString() to move the accumulated
 *     content from @c vb into @p _out without copying heap data.  On failure,
 *     initialize @p _out to an empty string, uninitialize @c vb, and return
 *     the error.
 *  6. Uninitialize @c vb (now empty after the move) and return @c 0.
 *
 * @note Negative @p _in_len is resolved to @c strlen(@p _in) once before
 *       the loop, so callers may pass @c -1 for NUL-terminated strings.
 *
 * @note Every failure path calls @c mmstrstk_init_v0() before returning,
 *       so @p _out is always left in a valid, empty-initialized state.
 *
 * @param[out] _out      Receives the result.  Always valid on return.
 * @param[in]  _obj_size Object byte size of @p _out, or non-positive if
 *                       @p _out is already initialized.
 * @param[in]  _count    Repeat count.  ≤ 0 yields an empty string.
 * @param[in]  _in       Byte sequence to repeat.
 * @param[in]  _in_len   Byte length of @p _in per repetition.
 *
 * @return @c 0 on success, or a non-zero error code on failure.
 */
MEME_EXTERN_C MEME_API mgec_t
MEME_STDCALL MemeStringStack_getRepeat_v2(
	mmstrstk_t* _out, mmint_t _obj_size, mmint_t _count, const char* _in, mmint_t _in_len)
{
	mgec_t result = 0;
	mmvbstk_t vb;

	assert(_out != NULL && "MemeStringStack_getRepeat_v2");

	if (_obj_size <= 0) {
		_obj_size = MemeStringStack_regSize(_out) * sizeof(mmint_t);
		mmstrstk_uninit_v0(_out, _obj_size);
	}

	result = (mgec_t)MemeVariableBufferStack_init(&vb, MMSTR__OBJ_SIZE);
	if (result) {
		mmstrstk_init_v0(_out, _obj_size);
		return result;
	}

	if (_in_len < 0)
		_in_len = _in ? (mmint_t)strlen(_in) : 0;

	for (; _count > 0; --_count) {
		result = (mgec_t)MemeVariableBuffer_appendWithBytes((mmvb_ptr_t)&vb, (const mmbyte_t*)_in, _in_len);
		if (result) {
			mmstrstk_init_v0(_out, _obj_size);
			MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
			return result;
		}
	}

	result = (mgec_t)MemeVariableBuffer_releaseToString((mmvb_ptr_t)&vb, _out, _obj_size);
	if (result) {
		mmstrstk_init_v0(_out, _obj_size);
		MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
		return result;
	}

	MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
	return 0;
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL MemeStringStack_replace(
	const mmsstk_t* _s, size_t _object_size, 
	const char* _from, mmint_t _from_len, 
	const char* _to, mmint_t _to_len, mmint_t _max_count)
{
    mmstrstk_t stack;
	MemeStringStack_replace_v2(
		_s, _from, _from_len, _to, _to_len, _max_count, &stack, MMSTR__OBJ_SIZE);
	return stack;

	//mmint_t result = 0;
	//mmsstk_t stack;
	//mmvbstk_t vb;
	//mms_t s = (mms_t)_s;
	//const mmbyte_t* it = NULL;
	//const mmbyte_t* end = NULL;
	//const mmbyte_t* from = NULL;
	//const mmbyte_t* to = NULL;
	//const mmbyte_t* from_end = NULL;
	//const mmbyte_t* to_end = NULL;
	//mmint_t count = 0;
	//mmint_t size = 0;

 //   assert(_s != NULL && MemeStringStack_replace != NULL);

 //   if (_from_len < 0)
 //       _from_len = strlen(_from);
 //   if (_to_len < 0)
 //       _to_len = strlen(_to);

 //   it = MemeString_byteData(s);
 //   end = it + MemeString_byteSize(s);
 //   from = (const MemeByte_t*)_from;
 //   to = (const MemeByte_t*)_to;
 //   from_end = from + _from_len;
 //   to_end = to + _to_len;

 //   for (; it != end; ++it) {
	//	
 //       if (it + _from_len > end)
 //           break;
	//	
 //       if (memcmp(it, from, _from_len) == 0)
 //       {
 //           ++count;
 //           if (_max_count > 0 && count >= _max_count)
 //               break;
 //           it += _from_len - 1;
 //       }
 //   }
	//MemeVariableBufferStack_init(&vb, _object_size);

 //   size = MemeString_byteSize(s) + (_to_len - _from_len) * count;
 //   it = MemeString_byteData(s);
 //   end = it + MemeString_byteSize(s);
 //   
	//for (; it != end; ++it) {
	//	
 //       if (it + _from_len > end)
 //           break;
	//	
 //       if (memcmp(it, from, _from_len) == 0)
 //       {
	//		result = MemeVariableBuffer_appendWithBytes((mmvb_t)&vb, to, _to_len);
	//		if (result) {
 //               MemeStringStack_init(&stack, _object_size);
 //               return stack;
	//		}
	//		
 //           ++count;
 //           if (_max_count > 0 && count >= _max_count)
 //               break;
 //           it += _from_len - 1;
 //       }
	//	else {
	//		result = MemeVariableBuffer_appendWithBytes((mmvb_t)&vb, it, 1);
	//		if (result) {
	//			MemeStringStack_init(&stack, _object_size);
	//			return stack;
	//		}

	//	}
 //   }
	//
 //   if (it != end) {
	//	result = MemeVariableBuffer_appendWithBytes((mmvb_t)&vb, it, end - it);
	//	if (result) {
	//		MemeStringStack_init(&stack, _object_size);
	//		return stack;
	//	}
	//}
	//
	//result = MemeVariableBuffer_releaseToString((mmvb_t)&vb, &stack, _object_size);
 //   if (result) {
 //       MemeStringStack_init(&stack, _object_size);
 //       return stack;
 //   }
 //   return stack;
}

/**
 * @brief Replace non-overlapping occurrences of @p _from in @p _str with
 *        @p _to, storing the result in @p _out.
 *
 * Implementation steps:
 *  1. Assert @p _str is non-NULL.
 *  2. Resolve lengths: if @p _from_len or @p _to_len is negative, call
 *     @c strlen() on the respective pointer once.
 *  3. If @p _obj_size is non-positive, read the actual object size from
 *     @p _out's internal metadata and uninitialize @p _out.
 *  4. If @p _from_len is zero, clone @p _str into @p _out via
 *     MemeStringStack_initByOther() and return immediately.
 *  5. First pass — counting: scan @p _str byte-by-byte.  On each match
 *     advance by @p _from_len and increment the replacement counter, stopping
 *     when @p _max_count is reached or fewer than @p _from_len bytes remain.
 *  6. Initialize a stack-local variable buffer @c vb via
 *     MemeVariableBufferStack_init().
 *  7. Second pass — building: same scan; on match append @p _to bytes, on
 *     non-match append the current single byte.  Stop at the same conditions
 *     as the first pass.
 *  8. Append any tail bytes that follow the final match (or that could not
 *     form a complete @p _from match near the end of @p _str).
 *  9. Move @c vb into @p _out via MemeVariableBuffer_releaseToString().
 * 10. Uninitialize @c vb and return @c 0.
 *
 * @note Every failure path calls @c MemeStringStack_init() before returning,
 *       so @p _out is always left in a valid, empty-initialized state.
 */
MEME_EXTERN_C MEME_API mgec_t
MEME_STDCALL MemeStringStack_replace_v2(
	const mmstrstk_t* _str, 
	const char* _from, mmint_t _from_len, 
	const char* _to, mmint_t _to_len, mmint_t _max_count, 
	mmstrstk_t* _out, mmint_t _obj_size)
{
	mmint_t result = 0;
	mmvbstk_t vb;
	mmstr_ptr_t str = (mmstr_ptr_t)_str;
	const mmbyte_t* it = NULL;
	const mmbyte_t* end = NULL;
	const mmbyte_t* from = NULL;
	const mmbyte_t* to = NULL;
	const mmbyte_t* from_end = NULL;
	const mmbyte_t* to_end = NULL;
	mmint_t count = 0;
	mmint_t size  = 0;

	assert(_str != NULL && "MemeStringStack_replace_v2");

	if (_from_len < 0)
		_from_len = strlen(_from);
	if (_to_len < 0)
		_to_len = strlen(_to);

	if (_obj_size <= 0) {
		_obj_size = MemeStringStack_regSize(_out) * sizeof(mmint_t);
		mmstrstk_uninit_v0(_out, _obj_size);
	}

	if (_from_len == 0)
		return MemeStringStack_initByOther(_out, _obj_size, (mmstr_cptr_t)_str);

	it = MemeString_byteData(str);
	end = it + MemeString_byteSize(str);
	from = (const mmbyte_t*)_from;
	to = (const mmbyte_t*)_to;
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
	MemeVariableBufferStack_init(&vb, MMSTR__OBJ_SIZE);

	size = MemeString_byteSize(str) + (_to_len - _from_len) * count;
	it = MemeString_byteData(str);
	end = it + MemeString_byteSize(str);

	for (; it != end; ++it) {

		if (it + _from_len > end)
			break;

		if (memcmp(it, from, _from_len) == 0)
		{
			result = MemeVariableBuffer_appendWithBytes((mmvb_t)&vb, to, _to_len);
			if (result) {
				MemeStringStack_init(_out, _obj_size);
				MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
				return (mgec_t)result;
			}

			++count;
			if (_max_count > 0 && count >= _max_count)
				break;
			it += _from_len - 1;
		}
		else {
			result = MemeVariableBuffer_appendWithBytes((mmvb_t)&vb, it, 1);
			if (result) {
				MemeStringStack_init(_out, _obj_size);
				MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
				return (mgec_t)result;
			}

		}
	}

	if (it != end) {
		result = MemeVariableBuffer_appendWithBytes((mmvb_t)&vb, it, end - it);
		if (result) {
			MemeStringStack_init(_out, _obj_size);
			MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
			return (mgec_t)result;
		}
	}

	result = MemeVariableBuffer_releaseToString((mmvb_t)&vb, _out, _obj_size);
	if (result) {
		MemeStringStack_init(_out, _obj_size);
		MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
		return (mgec_t)result;
	}

	MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
	return 0;
}

MEME_EXTERN_C MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_toValidUtf8(const mmsstk_t* _s, size_t _object_size)
{
	mmstrstk_t str;
    MemeStringStack_toValidUtf8_v2(_s, &str, MMSTR__OBJ_SIZE);
	return str;

	//MemeInteger_t pos = 0;

 //   assert(_s != NULL && MemeStringStack_toValidUtf8 != NULL);
 //   pos = mmutf_u8valid(MemeString_byteData((mms_t)_s), MemeString_byteSize((mms_t)_s));
	//if (pos == MemeString_byteSize((mms_t)_s))
	//{
	//	mmsstk_t s;
 //       MemeStringStack_initByOther(&s, _object_size, (mms_t)_s);
 //       return s;
	//}
 //   return MemeStringStack_mid(_s, _object_size, 0, pos);
}

/**
 * @brief Truncate @p _str to its longest valid UTF-8 prefix and store the
 *        result in @p _out.
 *
 * Implementation steps:
 *  1. Assert @p _str is non-NULL.
 *  2. Return @c MGEC__INVAL immediately if @p _out is @c NULL.
 *  3. If @p _obj_size is non-positive, read the actual object size from
 *     @p _out's internal metadata and uninitialize @p _out.
 *  4. Call mmutf_u8valid() on the raw byte data of @p _str to obtain @c pos,
 *     the byte offset of the first ill-formed UTF-8 sequence.
 *     If the entire string is valid UTF-8, @c pos equals the byte length.
 *  5. Delegate to MemeStringStack_mid_v2(@p _str, 0, @c pos, @p _out,
 *     @p _obj_size) to extract bytes [0, pos) into @p _out.
 *
 * @note On failure, @p _out is left in an indeterminate state (inherited from
 *       MemeStringStack_mid_v2()).  Do not call MemeStringStack_unInit() on it.
 */
MEME_EXTERN_C MEME_API mgec_t
MEME_STDCALL MemeStringStack_toValidUtf8_v2(const mmstrstk_t* _str, mmstrstk_t* _out, mmint_t _obj_size)
{
	mmint_t pos = 0;

	assert(_str != NULL && "MemeStringStack_toValidUtf8_v2");

	if (MG_SYM__UNLIKELY(_out == NULL))
		return MGEC__INVAL;

	if (_obj_size <= 0) {
		_obj_size = MemeStringStack_regSize(_out) * sizeof(mmint_t);
		mmstrstk_uninit_v0(_out, _obj_size);
	}

	pos = mmutf_u8valid(MemeString_byteData((mmstr_cptr_t)_str), MemeString_byteSize((mmstr_cptr_t)_str));
	//if (pos == MemeString_byteSize((mmstr_cptr_t)_str))
	//{
	//	return MemeStringStack_initByOther(_out, _obj_size, (mmstr_cptr_t)_str);
	//}
	return MemeStringStack_mid_v2(_str, 0, pos, _out, _obj_size);
}

MEME_EXTERN_C MEME_API mgec_t MEME_STDCALL 
	MemeStringStack_fromUInt16(uint16_t _value, int _radix, mmstrstk_t* _out, mmint_t _obj_size)
{
	mgec_t result;
	mmvbstk_t vb;
	size_t buf_size = (size_t)MGU_MAX_UNSIGNED_BASESTRLEN(sizeof(uint16_t) * CHAR_BIT, _radix) + 1;
	
	MemeVariableBufferStack_init(&vb, MMSTR__OBJ_SIZE);
	result = (mgec_t)MemeVariableBuffer_resize(
		(mmvb_ptr_t)&vb, buf_size);
	if (result) {
		MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
		return result;
	}

	{
		char* buf = (char*)MemeVariableBuffer_dataWithNotConst((mmvb_ptr_t)&vb);
		if (mgu_utoa16(_value, _radix, buf, buf_size) == NULL)
		{
			MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
			return MGEC__INVAL;
		}
	}
	
	if (_obj_size <= 0) {
		_obj_size = MemeStringStack_regSize(_out) * sizeof(mmint_t);
		mmstrstk_uninit_v0(_out, _obj_size);
	}
	
	result = (mgec_t)MemeVariableBuffer_releaseToString((mmvb_ptr_t)&vb, _out, _obj_size);
	return result;
}

MEME_EXTERN_C MEME_API mgec_t MEME_STDCALL 
	MemeStringStack_fromUInt32(uint32_t _value, int _radix, mmstrstk_t* _out, mmint_t _obj_size)
{
	mgec_t result;
	mmvbstk_t vb;
	size_t buf_size = (size_t)MGU_MAX_UNSIGNED_BASESTRLEN(sizeof(uint32_t) * CHAR_BIT, _radix) + 1;
	
	MemeVariableBufferStack_init(&vb, MMSTR__OBJ_SIZE);
	result = (mgec_t)MemeVariableBuffer_resize(
		(mmvb_ptr_t)&vb, buf_size);
	if (result) {
		MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
		return result;
	}

	{
		char* buf = (char*)MemeVariableBuffer_dataWithNotConst((mmvb_ptr_t)&vb);
		if (mgu_utoa32(_value, _radix, buf, buf_size) == NULL)
		{
			MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
			return MGEC__INVAL;
		}
		MemeVariableBuffer_resize((mmvb_ptr_t)&vb, (MemeInteger_t)strlen(buf));
	}
	
	if (_obj_size <= 0) {
		_obj_size = MemeStringStack_regSize(_out) * sizeof(mmint_t);
		mmstrstk_uninit_v0(_out, _obj_size);
	}
	
	result = (mgec_t)MemeVariableBuffer_releaseToString((mmvb_ptr_t)&vb, _out, _obj_size);
	return result;
}

MEME_EXTERN_C MEME_API mgec_t MEME_STDCALL 
	MemeStringStack_fromUInt64(uint64_t _value, int _radix, mmstrstk_t* _out, mmint_t _obj_size)
{
	mgec_t result;
	mmvbstk_t vb;
	size_t buf_size = (size_t)MGU_MAX_UNSIGNED_BASESTRLEN(sizeof(uint64_t) * CHAR_BIT, _radix) + 1;
	
	MemeVariableBufferStack_init(&vb, MMSTR__OBJ_SIZE);
	result = (mgec_t)MemeVariableBuffer_resize(
		(mmvb_ptr_t)&vb, buf_size);
	if (result) {
		MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
		return result;
	}

	{
		char* buf = (char*)MemeVariableBuffer_dataWithNotConst((mmvb_ptr_t)&vb);
		if (mgu_utoa64(_value, _radix, buf, buf_size) == NULL)
		{
			MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
			return MGEC__INVAL;
		}
	}
	
	if (_obj_size <= 0) {
		_obj_size = MemeStringStack_regSize(_out) * sizeof(mmint_t);
		mmstrstk_uninit_v0(_out, _obj_size);
	}
	
	result = (mgec_t)MemeVariableBuffer_releaseToString((mmvb_ptr_t)&vb, _out, _obj_size);
	return result;
}

MEME_EXTERN_C MEME_API mgec_t MEME_STDCALL 
	MemeStringStack_fromInt16(int16_t _value, int _radix, mmstrstk_t* _out, mmint_t _obj_size)
{
	mgec_t result;
	mmvbstk_t vb;
	size_t buf_size = (size_t)MGU_MAX_SIGNED_BASESTRLEN(sizeof(int16_t) * CHAR_BIT, _radix) + 1;
	
	MemeVariableBufferStack_init(&vb, MMSTR__OBJ_SIZE);
	result = (mgec_t)MemeVariableBuffer_resize(
		(mmvb_ptr_t)&vb, buf_size);
	if (result) {
		MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
		return result;
	}

	{
		char* buf = (char*)MemeVariableBuffer_dataWithNotConst((mmvb_ptr_t)&vb);
		if (mgu_itoa16(_value, _radix, buf, buf_size) == NULL)
		{
			MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
			return MGEC__INVAL;
		}
	}
	
	if (_obj_size <= 0) {
		_obj_size = MemeStringStack_regSize(_out) * sizeof(mmint_t);
		mmstrstk_uninit_v0(_out, _obj_size);
	}
	
	result = (mgec_t)MemeVariableBuffer_releaseToString((mmvb_ptr_t)&vb, _out, _obj_size);
	return result;
}

MEME_EXTERN_C MEME_API mgec_t MEME_STDCALL 
	MemeStringStack_fromInt32(int32_t _value, int _radix, mmstrstk_t* _out, mmint_t _obj_size)
{
	mgec_t result;
	mmvbstk_t vb;
	size_t buf_size = (size_t)MGU_MAX_SIGNED_BASESTRLEN(sizeof(int32_t) * CHAR_BIT, _radix) + 1;
	
	MemeVariableBufferStack_init(&vb, MMSTR__OBJ_SIZE);
	result = (mgec_t)MemeVariableBuffer_resize(
		(mmvb_ptr_t)&vb, buf_size);
	if (result) {
		MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
		return result;
	}

	{
		char* buf = (char*)MemeVariableBuffer_dataWithNotConst((mmvb_ptr_t)&vb);
		if (mgu_itoa32(_value, _radix, buf, buf_size) == NULL)
		{
			MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
			return MGEC__INVAL;
		}
	}
	
	if (_obj_size <= 0) {
		_obj_size = MemeStringStack_regSize(_out) * sizeof(mmint_t);
		mmstrstk_uninit_v0(_out, _obj_size);
	}
	
	result = (mgec_t)MemeVariableBuffer_releaseToString((mmvb_ptr_t)&vb, _out, _obj_size);
	return result;
}

MEME_EXTERN_C MEME_API mgec_t MEME_STDCALL 
	MemeStringStack_fromInt64(int64_t _value, int _radix, mmstrstk_t* _out, mmint_t _obj_size)
{
	mgec_t result;
	mmvbstk_t vb;
	size_t buf_size = (size_t)MGU_MAX_SIGNED_BASESTRLEN(sizeof(int64_t) * CHAR_BIT, _radix) + 1;
	
	MemeVariableBufferStack_init(&vb, MMSTR__OBJ_SIZE);
	result = (mgec_t)MemeVariableBuffer_resize(
		(mmvb_ptr_t)&vb, buf_size);
	if (result) {
		MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
		return result;
	}

	{
		char* buf = (char*)MemeVariableBuffer_dataWithNotConst((mmvb_ptr_t)&vb);
		if (mgu_itoa64(_value, _radix, buf, buf_size) == NULL)
		{
			MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
			return MGEC__INVAL;
		}
	}
	
	if (_obj_size <= 0) {
		_obj_size = MemeStringStack_regSize(_out) * sizeof(mmint_t);
		mmstrstk_uninit_v0(_out, _obj_size);
	}
	
	result = (mgec_t)MemeVariableBuffer_releaseToString((mmvb_ptr_t)&vb, _out, _obj_size);
	return result;
}

MEME_EXTERN_C MEME_API mmsstk_t MEME_STDCALL MemeStringStack_mappingConvert(
	const mmsstk_t* _s, size_t _object_size, MemeString_MappingConvertFunc_t _mapping_fn, void* _user_data)
{
	mmstrstk_t stack;
    MemeStringStack_mappingConvert_v2(_s, _mapping_fn, _user_data, &stack, MMSTR__OBJ_SIZE);
    return stack;

	//mms_const_t s = (mms_const_t)_s;
	////MemeInteger_t pos  = 0;
 //   MemeInteger_t size  = 0;
	//MemeInteger_t result = 0;
 //   const MemeByte_t* data = NULL;
 //   mmsstk_t stack;
 //   mmvbstk_t vb;

 //   assert(_s != NULL && MemeStringStack_mappingConvert != NULL);
	//
 //   MemeVariableBufferStack_init(&vb, MMS__OBJECT_SIZE);
	//
 //   size = MemeString_byteSize(s);
 //   data = MemeString_byteData(s);
 //   for (MemeInteger_t pos = 0; pos < size; )
	//{
	//	int len = mmutf_u8rune_char_size(data[pos]);
 //       if (len < 0)
 //           break;
	//	MemeRune_t rune;
 //       MemeRune_initByUtf8Bytes(&rune, data + pos, len);
 //       pos += len;
	//	result = _mapping_fn(&rune, _user_data);
	//	if (result < 0)
	//		continue;
	//	if (!MemeRune_isValid(&rune))
	//		continue;
	//	result = MemeVariableBuffer_appendWithBytes((mmvb_t)&vb, MemeRune_data(&rune), MemeRune_size(&rune));
	//	if (result < 0) {
	//		MemeVariableBufferStack_unInit(&vb, MMS__OBJECT_SIZE);
	//		return mmstrstk_get_init_v0(_object_size);
	//	}
	//}
	//
	//result = MemeVariableBuffer_releaseToString((mmvb_t)&vb, &stack, _object_size);
	//if (result < 0) {
	//	MemeVariableBufferStack_unInit(&vb, MMS__OBJECT_SIZE);
	//	return mmstrstk_get_init_v0(_object_size);
	//}
 //   return stack;
}

MEME_API mgec_t MEME_STDCALL 
MemeStringStack_mappingConvert_v2(
	const mmstrstk_t* _str, mmstr_mapping_conv_cb_t* _mapping_fn, void* _user_data, 
	mmstrstk_t* _out, mmint_t _obj_size)
{
	mmstr_ptr_t str = (mmstr_ptr_t)_str;
	mmint_t size = 0;
	mmint_t result = 0;
	const mmbyte_t* data = NULL;
	mmvbstk_t vb;

	assert(_str != NULL && "MemeStringStack_mappingConvert_v2");

	if (_obj_size <= 0) {
		_obj_size = MemeStringStack_regSize(_out) * sizeof(mmint_t);
		mmstrstk_uninit_v0(_out, _obj_size);
	}

	MemeVariableBufferStack_init(&vb, MMSTR__OBJ_SIZE);

	size = MemeString_byteSize(str);
	data = MemeString_byteData(str);
	for (mmint_t pos = 0; pos < size; )
	{
		int len = mmutf_u8rune_char_size(data[pos]);
		if (len < 0)
			break;
		mmrune_t rune;
		MemeRune_initByUtf8Bytes(&rune, data + pos, len);
		pos += len;
		result = _mapping_fn(&rune, _user_data);
		if (result < 0)
			continue;
		if (!MemeRune_isValid(&rune))
			continue;
		result = MemeVariableBuffer_appendWithBytes((mmvb_ptr_t)&vb, MemeRune_data(&rune), MemeRune_size(&rune));
		if (result < 0) {
			MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
			mmstrstk_init_v0(_out, _obj_size);
			return (mgec_t)result;
		}
	}

	result = MemeVariableBuffer_releaseToString((mmvb_ptr_t)&vb, _out, _obj_size);
	MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
	if (result < 0) {
		mmstrstk_init_v0(_out, _obj_size);
		return (mgec_t)result;
	}

	return 0;
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
	mmstrstk_t str;
	MemeStringStack_vformatWithLimitInCstyle_v2(
		&str, MMSTR__OBJ_SIZE, _size_limit, -1, _format, _args);
	return str;

	//MemeInteger_t result = 0;
	//mmvbstk_t vbuf;
	//mmsstk_t out;
	//MemeByte_t* data = NULL;
	//MemeInteger_t len = 0;
 //   va_list calc_args;

	////assert(_format != NULL && MemeStringStack_vformatInCstyle != NULL);

	//if ((_format == NULL))
	//	return MemeStringStack_getInitObject(_object_size);

	//va_copy(calc_args, _args);
	//len = vsnprintf(NULL, 0, _format, calc_args);
	//va_end(calc_args);
	//if ((len <= 0))
	//	return MemeStringStack_getInitObject(_object_size);

	//if (_size_limit > 0 && len > _size_limit)
	//	len = _size_limit;

	//MemeVariableBufferStack_init(&vbuf, MMS__OBJECT_SIZE);
	//result = MemeVariableBuffer_resize((mmvb_t)&vbuf, len + 1);
	//if (result) {
	//	MemeVariableBufferStack_unInit(&vbuf, MMS__OBJECT_SIZE);
	//	return MemeStringStack_getInitObject(_object_size);
	//}

	//data = MemeVariableBuffer_dataWithNotConst((mmvb_t)&vbuf);

	//len = vsnprintf((char*)data, len + 1, _format, _args);
	//if (len <= 0) {
	//	MemeVariableBufferStack_unInit(&vbuf, MMS__OBJECT_SIZE);
	//	return MemeStringStack_getInitObject(_object_size);
	//}
	//
	//result = MemeVariableBuffer_resize((mmvb_t)&vbuf, len);
 //   if (result) {
	//	MemeVariableBufferStack_unInit(&vbuf, MMS__OBJECT_SIZE);
 //       return MemeStringStack_getInitObject(_object_size);
 //   }

	//result = MemeVariableBuffer_releaseToString((mmvb_t)&vbuf, &out, _object_size);
	//if (result) {
	//	MemeVariableBufferStack_unInit(&vbuf, MMS__OBJECT_SIZE);
	//	return MemeStringStack_getInitObject(_object_size);
	//}

	//return out;
}

MEME_EXTERN_C MEME_API mmstrstk_t MEME_STDCALL
MemeStringStack_vformatWithLimitInCstyle(
	size_t _object_size,
	mmint_t _size_limit,
	mmint_t _pre_size,
	MEGO_SYMBOL__MSVC_FORMAT_STRING(const char* _format),
	va_list _args)
{
	mmstrstk_t str;
	MemeStringStack_vformatWithLimitInCstyle_v2(
        &str, MMSTR__OBJ_SIZE, _size_limit, _pre_size, _format, _args);
	return str;
	
	//mmint_t result = 0;
	//mmvbstk_t vbuf;
	//mmstrstk_t out;
	//mmbyte_t* data = NULL;
	//mmint_t len = 0;
	//va_list calcArgs;

	//if ((_format == NULL))
	//	return mmstrstk_get_init_v0(_object_size);

	//if (_pre_size <= 0) {
	//	va_copy(calcArgs, _args);
	//	len = vsnprintf(NULL, 0, _format, calcArgs);
	//	va_end(calcArgs);
	//	if (len <= 0)
	//		return mmstrstk_get_init_v0(_object_size);
	//}
	//else {
	//	MemeVariableBufferStack_init(&vbuf, MMSTR__OBJ_SIZE);
	//	result = MemeVariableBuffer_resize((mmvb_ptr_t)&vbuf, _pre_size + 1);
	//	if (result) {
	//		MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
	//		return mmstrstk_get_init_v0(_object_size);
	//	}

	//	va_copy(calcArgs, _args);
	//	len = vsnprintf((char*)MemeVariableBuffer_dataWithNotConst((mmvb_ptr_t)&vbuf),
	//		_pre_size + 1, _format, calcArgs);
	//	va_end(calcArgs);
	//	if (len <= 0) {
	//		MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
	//		return mmstrstk_get_init_v0(_object_size);
	//	}

	//	if (_pre_size >= len) {

	//		if (_size_limit > 0 && len > _size_limit)
	//			len = _size_limit;

	//		result = MemeVariableBuffer_resize((mmvb_ptr_t)&vbuf, len);
	//		if (result) {
	//			MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
	//			return mmstrstk_get_init_v0(_object_size);
	//		}

	//		result = MemeVariableBuffer_releaseToString((mmvb_ptr_t)&vbuf, &out, _object_size);
	//		MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
	//		if (result) {
	//			return mmstrstk_get_init_v0(_object_size);
	//		}

	//		return out;
	//	}
	//	MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
	//}

	//if (_size_limit > 0 && len > _size_limit)
	//	len = _size_limit;
	//else
	//	_size_limit = len;

	//MemeVariableBufferStack_init(&vbuf, MMSTR__OBJ_SIZE);
	//result = MemeVariableBuffer_resize((mmvb_ptr_t)&vbuf, len + 1);
	//if (result) {
	//	MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
	//	return mmstrstk_get_init_v0(_object_size);
	//}

	//data = MemeVariableBuffer_dataWithNotConst((mmvb_ptr_t)&vbuf);

	//len = vsnprintf((char*)data, len + 1, _format, _args);
	//if (len <= 0) {
	//	MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
	//	return mmstrstk_get_init_v0(_object_size);
	//}

	//result = MemeVariableBuffer_resize((mmvb_ptr_t)&vbuf, _size_limit);
	//if (result) {
	//	MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
	//	return mmstrstk_get_init_v0(_object_size);
	//}

	//result = MemeVariableBuffer_releaseToString((mmvb_ptr_t)&vbuf, &out, _object_size);
	//MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
	//if (result) {
	//	return mmstrstk_get_init_v0(_object_size);
	//}
	//
	//return out;
}

MEME_EXTERN_C MEME_API mgec_t MEME_STDCALL
MemeStringStack_vformatWithLimitInCstyle_v2(
	mmstrstk_t* _str,
	mmint_t _object_size,
	mmint_t _size_limit,
	mmint_t _pre_size,
	MG_SYM__MSVC_FMT_STR(const char* _format),
	va_list _args)
{
	mgec_t result = 0;
	mmvbstk_t vbuf;
	mmbyte_t* data = NULL;
	mmint_t len = 0;
	va_list calcArgs;

	if (MG_SYM__UNLIKELY(_str == NULL))
		return MGEC__INVAL;

	if (_object_size <= 0) {
		_object_size = MemeStringStack_regSize(_str) * sizeof(mmint_t);
		mmstrstk_uninit_v0(_str, _object_size);
	}

	if (_format == NULL) {
		return 0;
	}

	if (_pre_size <= 0) {
		va_copy(calcArgs, _args);
		len = vsnprintf(NULL, 0, _format, calcArgs);
		va_end(calcArgs);
		if (len < 0) {
			return MGEC__ERR;
		}
		if (len == 0) {
			return mmstrstk_init_v0(_str, _object_size);
		}
	}
	else {
		MemeVariableBufferStack_init(&vbuf, MMSTR__OBJ_SIZE);
		result = (mgec_t)MemeVariableBuffer_resize((mmvb_ptr_t)&vbuf, _pre_size + 1);
		if (result) {
			MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
			return result;
		}

		data = MemeVariableBuffer_dataWithNotConst((mmvb_ptr_t)&vbuf);

		va_copy(calcArgs, _args);
		len = vsnprintf((char*)data, _pre_size + 1, _format, calcArgs);
		va_end(calcArgs);
		if (len < 0) {
			MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
			return MGEC__ERR;
		}
		if (len == 0) {
			MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
			return mmstrstk_init_v0(_str, _object_size);
		}

		if (_pre_size >= len) {

			if (_size_limit > 0 && len > _size_limit)
				len = _size_limit;

			result = (mgec_t)MemeVariableBuffer_resize((mmvb_ptr_t)&vbuf, len);
			if (result) {
				MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
				return result;
			}

			result = (mgec_t)MemeVariableBuffer_releaseToString((mmvb_ptr_t)&vbuf, _str, _object_size);
			MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
			if (result) {
				return result;
			}

			return 0;
		}
		MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
	}

	if (_size_limit > 0 && len > _size_limit)
		len = _size_limit;
	else
		_size_limit = len;

	MemeVariableBufferStack_init(&vbuf, MMSTR__OBJ_SIZE);
	result = (mgec_t)MemeVariableBuffer_resize((mmvb_ptr_t)&vbuf, _size_limit + 1);
	if (result) {
		MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
		return result;
	}

	data = MemeVariableBuffer_dataWithNotConst((mmvb_ptr_t)&vbuf);

	len = vsnprintf((char*)data, _size_limit + 1, _format, _args);
	if (len <= 0) {
		MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
		return MGEC__ERR;
	}

	result = (mgec_t)MemeVariableBuffer_resize((mmvb_ptr_t)&vbuf, _size_limit);
	if (result) {
		MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
		return result;
	}

	result = (mgec_t)MemeVariableBuffer_releaseToString((mmvb_ptr_t)&vbuf, _str, _object_size);
	MemeVariableBufferStack_unInit(&vbuf, MMSTR__OBJ_SIZE);
	if (result) {
		return result;
	}

	return 0;
}

MEME_EXTERN_C MEME_API mgec_t MEME_STDCALL
MemeStringStack_formatWithLimitInCstyle(
	mmstrstk_t* _str,
	mmint_t _object_size,
	mmint_t _size_limit,
	mmint_t _pre_size,
	MG_SYM__MSVC_FMT_STR(const char* _format),
	...)
{
	mgec_t ec = 0;
	va_list args;
	va_start(args, _format);
	ec = MemeStringStack_vformatWithLimitInCstyle_v2(
		_str, _object_size, _size_limit, _pre_size, _format, args);
	va_end(args);
	
	return ec;
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

MEME_EXTERN_C MEME_API mgec_t
MEME_STDCALL MemeStringStack_join(
	mmstrstk_t* _str, mmint_t _obj_size, const char* _separator, mmint_t _separator_len,
	const mmstrstk_t* _items, mmint_t _item_count)
{
	mgec_t result = 0;
	mmvbstk_t vb;
	mmint_t i = 0;
	mmint_t item_stride = 0;

	assert(_str    != NULL && "MemeStringStack_join");
	assert(_items  != NULL || _item_count == 0);

	if (_obj_size <= 0) {
		_obj_size = MemeStringStack_regSize(_str) * sizeof(mmint_t);
		mmstrstk_uninit_v0(_str, _obj_size);
	}

	if (_separator_len < 0)
		_separator_len = _separator ? (mmint_t)strlen(_separator) : 0;

	if (_item_count <= 0) {
		mmstrstk_init_v0(_str, _obj_size);
		return 0;
	}

	item_stride = MemeStringStack_regSize(_items) * (mmint_t)sizeof(mmint_t);

	result = (mgec_t)MemeVariableBufferStack_init(&vb, MMSTR__OBJ_SIZE);
	if (result) {
		mmstrstk_init_v0(_str, _obj_size);
		return result;
	}

	for (i = 0; i < _item_count; ++i) {
		const mmstrstk_t* item = (const mmstrstk_t*)((const uint8_t*)_items + i * item_stride);
		mmstr_cptr_t s = (mmstr_cptr_t)item;
		mmint_t item_size = MemeString_byteSize(s);

		if (i > 0 && _separator_len > 0) {
			result = (mgec_t)MemeVariableBuffer_appendWithBytes(
				(mmvb_ptr_t)&vb, (const mmbyte_t*)_separator, _separator_len);
			if (result) {
				mmstrstk_init_v0(_str, _obj_size);
				MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
				return result;
			}
		}

		if (item_size > 0) {
			result = (mgec_t)MemeVariableBuffer_appendWithBytes(
				(mmvb_ptr_t)&vb, MemeString_byteData(s), item_size);
			if (result) {
				mmstrstk_init_v0(_str, _obj_size);
				MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
				return result;
			}
		}
	}

	result = (mgec_t)MemeVariableBuffer_releaseToString((mmvb_ptr_t)&vb, _str, _obj_size);
	if (result) {
		mmstrstk_init_v0(_str, _obj_size);
		MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
		return result;
	}

	MemeVariableBufferStack_unInit(&vb, MMSTR__OBJ_SIZE);
	return 0;
}

MEME_EXTERN_C MEME_API mmint_t
MEME_STDCALL MemeStringStack_split(
	const mmstrstk_t* _str, const char* _key, mmint_t _key_len, 
	mmflag_split_behav_t _behavior, mmflag_case_sensit_t _sensitivity,
	mmstrstk_t* MEGO_SYMBOL__RESTRICT _out, mmint_t _obj_size, 
	mmint_t* MEGO_SYMBOL__RESTRICT _out_count, 
	mmint_t* MEGO_SYMBOL__RESTRICT _search_index)
{
	int result = 0;
	mmint_t last_index = (_search_index == NULL ? 0 : *_search_index);
	mmint_t curr_index = -1;
	mmint_t output_index = 0;
	mmstrstk_t* output_item = NULL;
	mmstr_cptr_t str = (mmstr_cptr_t)_str;

	assert(_str != NULL && "MemeStringStack_split");
	assert(_out != NULL && "MemeStringStack_split");
	assert(_key != NULL && "MemeStringStack_split");
	assert(_out_count != NULL && "MemeStringStack_split");

	if (*_out_count < 1)
		return (MGEC__INVAL);
	if (_key_len < 0)
		_key_len = strlen(_key);

	while (output_index < *_out_count)
	{
		output_item = (mmstrstk_t*)((uint8_t*)_out + output_index * _obj_size);

		curr_index =
			MemeString_indexOfWithUtf8bytes(
				str, last_index, (const mmbyte_t*)_key, _key_len, _sensitivity);
		if (curr_index == -1) {
			if (last_index < MemeString_byteSize(str))
			{
				MemeStringStack_initByU8bytes(
					output_item, _obj_size,
					MemeString_byteData(str) + last_index,
					MemeString_byteSize(str) - last_index);
				*_out_count = output_index + 1;
			}
			else if (_behavior == MemeFlag_KeepEmptyParts)
			{
				MemeStringStack_init(output_item, _obj_size);
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

		result = MemeStringStack_initByU8bytes(
			output_item, _obj_size, MemeString_byteData(str) + last_index, curr_index - last_index);
		if (result != 0) {
			for (mmint_t i = 0; i < output_index; ++i)
				MemeStringStack_unInit(_out + i, _obj_size);
			return result;
		}
		last_index = curr_index + _key_len;
		++output_index;
	}
	if (_behavior == MemeFlag_SkipEmptyParts)
	{
		curr_index =
			MemeString_indexOfWithUtf8bytes(
				str, last_index, (const mmbyte_t*)_key, _key_len, _sensitivity);
		if ((curr_index - last_index) == 0)
		{
			last_index += _key_len;
		}
	}

	*_out_count = output_index;
	if (last_index < MemeString_byteSize(str))
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

MEME_API mmint_t MEME_STDCALL 
MemeStringStack_splitByCondByteFunc(
	const mmstrstk_t* _str, mmstr_match_cond_byte_cb_t* _cond_func, void* _user_data, 
	mmstrstk_t* MEGO_SYMBOL__RESTRICT _out, mmint_t _obj_size, 
	mmint_t* MEGO_SYMBOL__RESTRICT _out_count, mmint_t* MEGO_SYMBOL__RESTRICT _search_index)
{
	int result = 0;
	mmint_t last_index = (_search_index == NULL ? 0 : *_search_index);
	mmint_t curr_index = -1;
	mmint_t output_index = 0;
	mmstrstk_t* output_item = NULL;
	mmstr_cptr_t str = (mmstr_cptr_t)_str;

	assert(_str != NULL && "MemeStringStack_splitByCondByteFunc");
	assert(_out != NULL && "MemeStringStack_splitByCondByteFunc");
	assert(_out_count != NULL && "MemeStringStack_splitByCondByteFunc");

	if (*_out_count < 1)
		return (MGEC__INVAL);

	while (output_index < *_out_count)
	{
		output_item = (mmstrstk_t*)((uint8_t*)_out + output_index * _obj_size);
		curr_index =
			MemeString_indexByCondByteFunc(
				str, last_index, _cond_func, _user_data);
		if (curr_index < 0) {
			if (last_index < MemeString_byteSize(str))
			{
				MemeStringStack_initByU8bytes(
					output_item, _obj_size, MemeString_byteData(str) + last_index,
					MemeString_byteSize(str) - last_index);
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

		result = MemeStringStack_initByU8bytes(
			output_item, _obj_size, 
			MemeString_byteData(str) + last_index, curr_index - last_index);
		if (result != 0) {
			for (mmint_t i = 0; i < output_index; ++i)
				MemeStringStack_unInit(_out + i, _obj_size);
			return result;
		}
		last_index = curr_index + 1;
		++output_index;
	}

	*_out_count = output_index;
	if (last_index < MemeString_byteSize(str))
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

MEME_EXTERN_C MEME_API int 
MEME_STDCALL MemeStringViewUnsafeStack_init(
	MemeStringStack_t* _s, size_t _object_size, const uint8_t* _buf, MemeInteger_t _len)
{
	MemeStringViewUnsafe_t* p = (MemeStringViewUnsafe_t*)_s;

	assert(_s != NULL && "MemeStringViewUnsafeStack_init");
	assert(_object_size != 0 && _object_size <= MMSTR__MAX_REG_BYTE_SIZE && "MemeStringViewUnsafeStack_init");

	if (_buf == NULL)
		_buf = MemeStringImpl_default();
	
	if (_len < 0)
		_len = strlen((const char*)_buf);

	//if (_len <= MMS__GET_SMALL_BUFFER_SIZE)
	//	return MemeStringSmall_initByU8bytes((MemeStringSmall_t*)_s, _buf, _len);

	p->reg_size_ = (mmbyte_t)(_object_size / sizeof(mmint_t));
	p->data_   = _buf;
	p->size_   = _len;
	p->type_   = MemeString_ImplType_view;
	return 0;
}

MEME_EXTERN_C MEME_API int 
MEME_STDCALL MemeStringViewUnsafeStack_initByOther(
	MemeStringStack_t* _s, size_t _object_size, const MemeStringStack_t* _other)
{
	assert(_s != NULL		&& MemeStringViewUnsafeStack_initByOther != NULL);
	assert(_other != NULL	&& MemeStringViewUnsafeStack_initByOther != NULL);
	assert(_object_size != 0 && _object_size <= MMSTR__MAX_REG_BYTE_SIZE);

	if (MMSTR__GET_IMPLTYPE((mmstr_cptr_t)_other) == MemeString_ImplType_view)
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

MEME_EXTERN_C MEME_API int 
MEME_STDCALL MemeStringViewUnsafeStack_assignByOther(
	MemeStringStack_t* _s, size_t _object_size, const MemeStringStack_t* _other)
{
	assert(_s != NULL		&& MemeStringViewUnsafeStack_assignByOther != NULL);
	assert(_other != NULL	&& MemeStringViewUnsafeStack_assignByOther != NULL);

	if (MMSTR__GET_IMPLTYPE((mmstr_cptr_t)_other) == MemeString_ImplType_view)
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

MEME_EXTERN_C MEME_API mmint_t 
MEME_STDCALL MemeStringViewUnsafeStack_split(
	const mmstrstk_t* _str, 
	const char* _key, mmint_t _key_len, 
	mmflag_split_behav_t _behavior, mmflag_case_sensit_t _sensitivity,
	mmstrstk_t* MEGO_SYMBOL__RESTRICT _out, mmint_t _obj_size, 
	mmint_t* MEGO_SYMBOL__RESTRICT _out_count,
	mmint_t* MEGO_SYMBOL__RESTRICT _search_index)
{
	mmint_t last_index = (_search_index == NULL ? 0 : *_search_index);
	mmint_t curr_index = -1;
	mmint_t output_index = 0;
	mmstrstk_t* output_item = NULL;
    mmstr_cptr_t str = (mmstr_cptr_t)_str;

	assert(_str != NULL && "MemeStringViewUnsafeStack_split");
	assert(_out != NULL && "MemeStringViewUnsafeStack_split");
	assert(_key != NULL && "MemeStringViewUnsafeStack_split");
	assert(_out_count != NULL && "MemeStringViewUnsafeStack_split");

	if (*_out_count < 1)
		return (MGEC__INVAL);
	if (_key_len < 0)
		_key_len = strlen(_key);

	while (output_index < *_out_count)
	{
		output_item = (mmstrstk_t*)((uint8_t*)_out + output_index * _obj_size);
		curr_index = MemeString_indexOfWithUtf8bytes(
				str, last_index, (const mmbyte_t*)_key, _key_len, _sensitivity);
		if (curr_index == -1) {
			if (last_index < MemeString_byteSize(str))
			{
				MemeStringViewUnsafeStack_init(
					output_item, _obj_size, 
					MemeString_byteData(str) + last_index,
					MemeString_byteSize(str) - last_index);
				*_out_count = output_index + 1;
			}
			else if (_behavior == MemeFlag_KeepEmptyParts)
			{
				MemeStringStack_init(output_item, _obj_size);
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

		MemeStringViewUnsafeStack_init(
			output_item, _obj_size, MemeString_byteData(str) + last_index, curr_index - last_index);
		last_index = curr_index + _key_len;
		++output_index;
	}
	if (_behavior == MemeFlag_SkipEmptyParts)
	{
		curr_index =
			MemeString_indexOfWithUtf8bytes(
				str, last_index, (const mmbyte_t*)_key, _key_len, _sensitivity);
		if ((curr_index - last_index) == 0)
		{
			last_index += _key_len;
		}
	}

	*_out_count = output_index;
	if (last_index < MemeString_byteSize(str))
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

MEME_EXTERN_C MEME_API MemeInteger_t MEME_STDCALL
MemeStringViewUnsafe_split(
	MemeString_Const_t _s, const char* _key, MemeInteger_t _key_len,
	MemeFlag_SplitBehavior_t _behavior, MemeFlag_CaseSensitivity_t _sensitivity,
	MemeStringStack_t* _out, MemeInteger_t* _out_count, MemeInteger_t* _search_index)
{
	return MemeStringViewUnsafeStack_split(
		(const mmstrstk_t*)_s, _key, _key_len, 
		_behavior, _sensitivity, _out, MMSTR__OBJ_SIZE, _out_count, _search_index);

	//MemeInteger_t last_index = (_search_index == NULL ? 0 : *_search_index);
	//MemeInteger_t curr_index = -1;
	//MemeInteger_t output_index = 0;

	//assert(_s != NULL && MemeString_split != NULL);
	//assert(_out != NULL && MemeString_split != NULL);
	//assert(_out_count != NULL && MemeString_split != NULL);

	//if (*_out_count < 1)
	//	return (MGEC__INVAL);
	//if (_key_len < 0)
	//	_key_len = strlen(_key);

	//while (output_index < *_out_count)
	//{
	//	curr_index =
	//		MemeString_indexOfWithUtf8bytes(
	//			_s, last_index, (const MemeByte_t*)_key, _key_len, _sensitivity);
	//	if (curr_index == -1) {
	//		if (last_index < MemeString_byteSize(_s))
	//		{
	//			MemeStringViewUnsafeStack_init(_out + output_index,
	//				MEME_STRING__OBJECT_SIZE, MemeString_byteData(_s) + last_index,
	//				MemeString_byteSize(_s) - last_index);
	//			*_out_count = output_index + 1;
	//		}
	//		else if (_behavior == MemeFlag_KeepEmptyParts)
	//		{
	//			MemeStringStack_init(_out + output_index,
	//				MEME_STRING__OBJECT_SIZE);
	//			*_out_count = output_index + 1;
	//		}
	//		else {
	//			*_out_count = output_index;
	//		}
	//		if (_search_index)
	//			*_search_index = -1;
	//		return 0;
	//	}

	//	if ((curr_index - last_index) == 0 && _behavior == MemeFlag_SkipEmptyParts)
	//	{
	//		last_index += _key_len;
	//		continue;
	//	}

	//	MemeStringViewUnsafeStack_init(_out + output_index,
	//		MEME_STRING__OBJECT_SIZE, MemeString_byteData(_s) + last_index, curr_index - last_index);
	//	last_index = curr_index + _key_len;
	//	++output_index;
	//}
	//if (_behavior == MemeFlag_SkipEmptyParts)
	//{
	//	curr_index =
	//		MemeString_indexOfWithUtf8bytes(
	//			_s, last_index, (const MemeByte_t*)_key, _key_len, _sensitivity);
	//	if ((curr_index - last_index) == 0)
	//	{
	//		last_index += _key_len;
	//	}
	//}

	//*_out_count = output_index;
	//if (last_index < MemeString_byteSize(_s))
	//{
	//	if (_search_index)
	//		*_search_index = last_index;
	//	return 0;
	//}
	//else {
	//	if (_search_index)
	//		*_search_index = -1;
	//	return 0;
	//}
}

//! @deprecated This function is deprecated
MEME_EXTERN_C MEME_API int MEME_STDCALL mmsstk_init(mmsstk_t* _out, size_t _object_size)
{
    if (_out == NULL)
        return (MGEC__INVAL);
    return MemeStringStack_init(_out, _object_size);
}

//! @deprecated This function is deprecated
MEME_EXTERN_C MEME_API int MEME_STDCALL mmsstk_init_by_other(mmsstk_t* _out, size_t _object_size, mms_const_t _other)
{
    if (_out == NULL)
        return (MGEC__INVAL);
	if (_other == NULL) 
        return MemeStringStack_init(_out, _object_size);
    return MemeStringStack_initByOther(_out, _object_size, _other);
}

//! @deprecated This function is deprecated
MEME_EXTERN_C MEME_API int MEME_STDCALL mmsstk_init_by_buf(
	mmsstk_t* _out, size_t _object_size, mmbuf_const_t _other, mmint_t _offset)
{
    if (_out == NULL)
        return (MGEC__INVAL);
    if (_other == NULL)
		return MemeStringStack_init(_out, _object_size);
    return MemeStringStack_initByBuffer(_out, _object_size, _other, _offset);
}

//! @deprecated This function is deprecated
MEME_EXTERN_C MEME_API int MEME_STDCALL mmsstk_init_by_utf8(
	mmsstk_t* _out, size_t _object_size, const mmbyte_t* _utf8, mmint_t _len)
{
    if (_out == NULL)
        return (MGEC__INVAL);
    if (_utf8 == NULL)
		return MemeStringStack_init(_out, _object_size);
    return MemeStringStack_initByU8bytes(_out, _object_size, _utf8, _len);
}

//! @deprecated This function is deprecated
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

//! @deprecated This function is deprecated
MEME_EXTERN_C MEME_API int MEME_STDCALL mmsstk_init_by_hexs(
	mmsstk_t* _out, size_t _object_size, const mmbyte_t* _interval, mmint_t _ivlen, const uint8_t* _hexs, mmint_t _len)
{
    if (_out == NULL)
        return (MGEC__INVAL);
    if (_hexs == NULL)
        return MemeStringStack_init(_out, _object_size);
    return MemeStringStack_initWithHexadecimals(_out, _object_size, _interval, _ivlen, _hexs, _len);
}

//! @deprecated This function is deprecated
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

//! @deprecated This function is deprecated
MEME_EXTERN_C MEME_API int MEME_STDCALL mmsstk_uninit(mmsstk_t* _out, size_t _object_size)
{
    if (_out == NULL)
        return (MGEC__INVAL);
    return MemeStringStack_unInit(_out, _object_size);
}

//! @deprecated This function is deprecated
MEME_EXTERN_C MEME_API int MEME_STDCALL mmsstk_reset(mmsstk_t* _out, size_t _object_size)
{
    if (_out == NULL)
        return (MGEC__INVAL);
    return MemeStringStack_reset(_out, _object_size);
}

//! @deprecated This function is deprecated
MEME_EXTERN_C MEME_API int MEME_STDCALL mms_assign(mms_t _s, mms_const_t _other)
{
    if (_s == NULL)
        return (MGEC__INVAL);
    if (_other == NULL)
        return MemeStringStack_reset((mmsstk_t*)_s, sizeof(*_s));
    return MemeStringStack_assign((mmsstk_t*)_s, sizeof(*_s), _other);
}

//! @deprecated This function is deprecated
MEME_EXTERN_C MEME_API int MEME_STDCALL mms_assign_by_utf8(
	mms_t _s, const mmbyte_t* _utf8, mmint_t _len)
{
    if (_s == NULL)
        return (MGEC__INVAL);
    if (_utf8 == NULL)
        return MemeStringStack_reset((mmsstk_t*)_s, sizeof(*_s));
    return MemeStringStack_assignByU8bytes((mmsstk_t*)_s, sizeof(*_s), _utf8, _len);
}

//! @deprecated This function is deprecated
MEME_EXTERN_C MEME_API int MEME_STDCALL mms_assign_by_buf(
	mms_t _out, mmbuf_const_t _other, mmint_t _offset)
{
    if (_out == NULL)
        return (MGEC__INVAL);
    if (_other == NULL)
        return MemeStringStack_reset((mmsstk_t*)_out, sizeof(*_out));
    return MemeStringStack_assignByBuffer((mmsstk_t*)_out, sizeof(*_out), _other, _offset);
}
