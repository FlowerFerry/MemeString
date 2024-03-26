
#ifndef MEME_STRING_H_INCLUDED
#define MEME_STRING_H_INCLUDED

#include "meme/common.h"
#include "meme/string_fwd.h"
#include "meme/string_memory_fwd.h"
#include "meme/buffer_fwd.h"
#include <mego/err/ec.h>
#include <mego/predef/symbol/compiler/gcc/format.h>
#include <mego/predef/symbol/compiler/msvc/format.h>
#include <mego/predef/symbol/restrict.h>
#include <mego/predef/symbol/inline.h>
#include <mego/predef/symbol/likely.h>

#include <stdarg.h>
#include <assert.h>

MEME_EXTERN_C_SCOPE_START

MEME_API int
	MEME_STDCALL MemeStringOption_setStorageMediumLimit(MemeInteger_t _value);

MEME_API MemeInteger_t
	MEME_STDCALL MemeStringOption_getStorageMediumLimit();

MEME_API int 
	MEME_STDCALL MemeStringStack_init(mmsstk_t* _out, size_t _object_size);

MEME_API mmsstk_t
	MEME_STDCALL MemeStringStack_getInitObject(size_t _object_size);

MEME_API int
	MEME_STDCALL MemeStringStack_initTakeOverUserObject(
		mmsstk_t* _out, size_t _object_size,
		void* _user_data, 
		MemeString_UserObjectDestruct_t* _destruct_fn, 
		MemeString_UserObjectData_t* _data_fn, 
		MemeString_UserObjectSize_t* _size_fn);

MEME_API int
	MEME_STDCALL MemeStringStack_initByOther(
		mmstrstk_t* _out, size_t _object_size, mmstr_cptr_t _other);

MEME_API int
MEME_STDCALL MemeStringStack_initByBuffer(
	mmsstk_t* _out, size_t _object_size, MemeBuffer_Const_t _other, MemeInteger_t _offset);

MEME_API int
	MEME_STDCALL MemeStringStack_initByRune(
		mmsstk_t* _out, size_t _object_size, mmint_t _count, MemeRune_t _ch);

MEME_API int
	MEME_STDCALL MemeStringStack_initByU8bytes(
		mmsstk_t* _out, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len);

MEME_API int
	MEME_STDCALL MemeStringStack_initByU8bytesAndType(
		mmsstk_t* _out, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len,
		MemeString_Storage_t _suggest);

MEME_API int
	MEME_STDCALL MemeStringStack_initByU16bytes(
		mmsstk_t* _out, size_t _object_size, const uint16_t* _buf, MemeInteger_t _len);

MEME_API int
	MEME_STDCALL MemeStringStack_initByU16bytesAndType(
		mmsstk_t* _out, size_t _object_size, const uint16_t* _buf, MemeInteger_t _len,
		MemeString_Storage_t _suggest);

MEME_API int
	MEME_STDCALL MemeStringStack_initWithHexadecimals(
		mmsstk_t* _out, size_t _object_size,
		const MemeByte_t* _interval, MemeInteger_t _ivlen, 
		const uint8_t* _hexs, MemeInteger_t _len);

MEME_API int
	MEME_STDCALL MemeStringStack_initAndConditionalConvert(
        mmstrstk_t* _out, size_t _object_size, mmstr_cptr_t _other);

//! @param _s The string stack object, must be initialized.
MEME_API int
	MEME_STDCALL MemeStringStack_unInit(mmsstk_t* _out, size_t _object_size);

//! @param _s The string stack object, must be initialized.
MEME_API int 
	MEME_STDCALL MemeStringStack_reset (mmsstk_t* _out, size_t _object_size);

//! @param _s The string stack object, must be initialized.
MEME_API int
	MEME_STDCALL MemeStringStack_assign(
		mmsstk_t* _s, size_t _object_size, MemeString_Const_t _other);

//! @param _s The string stack object, must be initialized.
MEME_API int
MEME_STDCALL MemeStringStack_assignByU8bytes(
	mmsstk_t* _s, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len);

//! @param _s The string stack object, must be initialized.
MEME_API int
MEME_STDCALL MemeStringStack_assignByBuffer(
	mmsstk_t* _out, size_t _object_size, MemeBuffer_Const_t _other, MemeInteger_t _offset);

//! @param _s The string stack object, must be initialized.
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_mid(
	const mmsstk_t* _s, size_t _object_size, MemeInteger_t _offset, MemeInteger_t _len);

//! @param _s The string stack object, must be initialized.
//! @param _other The string stack object, must be initialized.
MEME_API mmstrstk_t
MEME_STDCALL MemeStringStack_concat(
    const mmstrstk_t* _s, size_t _object_size, const mmstrstk_t* _other);

//! @param _s The string stack object, must be initialized.
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_toEnUpper(
	const mmsstk_t* _s, size_t _object_size);

//! @param _s The string stack object, must be initialized.
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_toEnLower(
	const mmsstk_t* _s, size_t _object_size);

//! @param _s The string stack object, must be initialized.
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_trimSpace(
    const mmsstk_t* _s, size_t _object_size);

//! @param _s The string stack object, must be initialized.
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_trimLeftSpace(
    const mmsstk_t* _s, size_t _object_size);

//! @param _s The string stack object, must be initialized.
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_trimRightSpace(
    const mmsstk_t* _s, size_t _object_size);

//! @param _s The string stack object, must be initialized.
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_trimByCuts(
    const mmsstk_t* _s, size_t _object_size,
	const char* _cuts, MemeInteger_t _cuts_len);

//! @param _s The string stack object, must be initialized.
//! @param _cond_func The condition function; if the function returns true, the byte will be skipped.
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_trimByCondByteFunc(
    const mmsstk_t* _s, size_t _object_size,
	MemeString_MatchCondByteFunc_t* _cond_func, void* _user_data);

//! \code
//! MemeStringStack_t s = MemeStringStack_getRepeat(MEME_STRING__OBJECT_SIZE, 3, "abc", 3);
//! // s is "abcabcabc";
//! \endcode
//! @param _s The string stack object, must be initialized.
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_getRepeat(
	size_t _object_size, mmint_t _count, const char* _s, mmint_t _len);

//! @param _s The string stack object, must be initialized.
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_replace(
    const mmsstk_t* _s, size_t _object_size,
    const char* _from, mmint_t _from_len,
    const char* _to, mmint_t _to_len, mmint_t _max_count);

//! @param _s The string stack object, must be initialized.
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_toValidUtf8(
    const mmsstk_t* _s, size_t _object_size);

MEME_API mmstrstk_t
MEME_STDCALL MemeStringStack_fromInt(
    size_t _object_size, mmint_t _value, int _radix);

MEME_API mmstrstk_t
MEME_STDCALL MemeStringStack_fromInt16(
    size_t _object_size, int16_t _value, int _radix);

MEME_API mmstrstk_t
MEME_STDCALL MemeStringStack_fromInt32(
    size_t _object_size, int32_t _value, int _radix);

MEME_API mmstrstk_t
MEME_STDCALL MemeStringStack_fromInt64(
    size_t _object_size, int64_t _value, int _radix);

MEME_API mmstrstk_t
MEME_STDCALL MemeStringStack_fromUInt16(
    size_t _object_size, uint16_t _value, int _radix);

MEME_API mmstrstk_t
MEME_STDCALL MemeStringStack_fromUInt32(
    size_t _object_size, uint32_t _value, int _radix);

MEME_API mmstrstk_t
MEME_STDCALL MemeStringStack_fromUInt64(
    size_t _object_size, uint64_t _value, int _radix);

//MEME_API mmstrstk_t
//MEME_STDCALL MemeStringStack_fromFloat(
//    size_t _object_size, float _value, int _radix);
//
//MEME_API mmstrstk_t
//MEME_STDCALL MemeStringStack_fromDouble(
//    size_t _object_size, double _value, int _radix);

//! @param _s The string stack object, must be initialized.
//! @param _mapping_fn The mapping function; if the function returns less than 0, the rune will be convert.
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_mappingConvert(
    const mmsstk_t* _s, size_t _object_size,
    MemeString_MappingConvertFunc_t _mapping_fn, void* _user_data);

MEME_API MemeStringStack_t 
MemeStringStack_vformatInCstyle(
    size_t _object_size, 
	const char* _format,
	MemeInteger_t _size_limit, va_list _args);

MEME_API MemeStringStack_t
MemeStringStack_formatInCstyle(
    size_t _object_size, 
	const char* _format,
	MemeInteger_t _size_limit, ...);

//! @param _s The string stack object, must be initialized.
MEME_API mmsstk_t MEME_STDCALL
MemeStringStack_vformatInCstyle_v2(
	size_t _object_size,
	MemeInteger_t _size_limit,
	MEGO_SYMBOL__MSVC_FORMAT_STRING(const char* _format),
	va_list _args);

MEME_API mmstrstk_t MEME_STDCALL
MemeStringStack_vformatWithLimitInCstyle(
	size_t _object_size,
	mmint_t _size_limit,
	mmint_t _pre_size,
	MEGO_SYMBOL__MSVC_FORMAT_STRING(const char* _format),
	va_list _args);

//! @param _s The string stack object, must be initialized.
MEME_API mmsstk_t
MemeStringStack_formatInCstyle_v2(
	size_t _object_size,
	MemeInteger_t _size_limit,
	MEGO_SYMBOL__MSVC_FORMAT_STRING(const char* _format),
	...)
	MEGO_SYMBOL__GCC_ATTRIBUTE_FORMAT(printf, 3, 4);

//MEME_API MemeInteger_t
//MEME_STDCALL MemeStringStack_toInteger(
//    const MemeStringStack_t* _s, size_t _object_size, MemeInteger_t* _out);
//
//MEME_API MemeInteger_t
//MEME_STDCALL MemeStringStack_toDouble(
//    const MemeStringStack_t* _s, size_t _object_size, double* _out);

MEME_API MemeString_Storage_t
	MEME_STDCALL MemeString_storageType(MemeString_Const_t _s);

MEME_API int 
	MEME_STDCALL MemeString_create(MemeString_t* _out);

//MEME_API int 
//	MEME_STDCALL MemeString_createWithType(MemeString_t* _out, MemeString_Storage_t);

MEME_API int 
	MEME_STDCALL MemeString_createByOther(mmstr_ptr_t* _out, mmstr_cptr_t _other);

MEME_API int 
	MEME_STDCALL MemeString_createByRune(mmstr_ptr_t* _out, size_t _len, mmrune_t _ch);

MEME_API int 
	MEME_STDCALL MemeString_createByUtf8bytes(
		mmstr_ptr_t* _out, const mmbyte_t* _utf8, mmint_t _len);

MEME_API int 
	MEME_STDCALL MemeString_destroy(MemeString_t* _out);

MEME_API int 
	MEME_STDCALL MemeString_assign(MemeString_t _s, MemeString_Const_t _other);

MEME_API int 
	MEME_STDCALL MemeString_swap(MemeString_t _lhs, MemeString_t _rhs);

MEME_API int 
	MEME_STDCALL MemeString_isNonempty(MemeString_Const_t _s);

MEME_API int
	MEME_STDCALL MemeString_isEmpty(MemeString_Const_t _s);

MEME_API const char* 
	MEME_STDCALL MemeString_cStr(MemeString_Const_t _s);

//! @brief Get the length of the string in bytes.
//! @return The length of the string in bytes.
//! @note The length of the string in bytes is not necessarily equal to the length of the string in words.
MEME_API MemeInteger_t 
	MEME_STDCALL MemeString_byteSize(MemeString_Const_t _s);

//! @brief Get the length of the string in words.
//! @return The length of the string in words.
//! @note The length of the string in words is not necessarily equal to the length of the string in bytes.
MEME_API MemeInteger_t 
	MEME_STDCALL MemeString_runeSize(mmstr_cptr_t _s);
MEME_API MemeInteger_t
	MEME_STDCALL MemeString_u16CharSize(mmstr_cptr_t _s);
MEME_API MemeInteger_t 
	MEME_STDCALL MemeString_availableByteCapacity(MemeString_Const_t _s);
MEME_API MemeInteger_t
	MEME_STDCALL MemeString_maxByteSize(MemeString_Const_t _s);
MEME_API MemeInteger_t
MEME_STDCALL MemeString_maxByteCapacity(MemeString_Const_t _s);

//! @brief Get the byte pointer at the specified index.
//! @param _s The string.
//! @param _index The index of the byte.
//! @return The byte pointer at the specified index.
//! @note The index must be in the range [0, MemeString_wordSize(_s)).
MEME_API const MemeByte_t*
	MEME_STDCALL MemeString_at(MemeString_Const_t _s, MemeInteger_t _index);

MEME_API int
	MEME_STDCALL MemeString_isEqual(
		MemeString_Const_t _s, const char* _str, MemeInteger_t _len, int* _result);
MEME_API int
	MEME_STDCALL MemeString_isEqualWithOther(
		MemeString_Const_t _lhs, MemeString_Const_t _rhs, int* _result);

//! deprecated
MEME_API int
	MEME_STDCALL MemeString_containsOnlyAscii(mmstr_cptr_t _s, int* _result);

MEME_API int
	MEME_STDCALL MemeString_isOnlyAscii(mmstr_cptr_t _s, int* _result);

MEME_API int
MEME_STDCALL MemeString_compare(mmstr_cptr_t _s, mmstr_cptr_t _other);

MEME_API MemeInteger_t
	MEME_STDCALL MemeString_indexOfWithUtf8bytes(
		MemeString_Const_t _s, MemeInteger_t _offset,
		const MemeByte_t* _needle, MemeInteger_t _needle_len,
		MemeFlag_CaseSensitivity_t _cs);

MEME_API MemeInteger_t
	MEME_STDCALL MemeString_indexOfWithUtf8bytesAndSizeLimit(
		mmstr_cptr_t _s, mmint_t _offset, mmint_t _limit,
		const mmbyte_t* _needle, mmint_t _needle_len,
		MemeFlag_CaseSensitivity_t _cs);

MEME_API mmint_t
	MEME_STDCALL MemeString_indexOfUtf8bytes(
		mmstr_cptr_t _str, mmint_t _offset, mmint_t _limit,
		const mmbyte_t* _needle, mmint_t _needle_len,
		int _full_match, mmflag_case_sensit_t _cs);

MEME_API MemeInteger_t
MEME_STDCALL MemeString_indexOfWithByte(
	MemeString_Const_t _s, MemeInteger_t _offset, MemeByte_t _byte,
	MemeFlag_CaseSensitivity_t _cs);

MEME_API MemeInteger_t
	MEME_STDCALL MemeString_indexOfWithOther(
		MemeString_Const_t _s, MemeInteger_t _offset,
		MemeString_Const_t _other, MemeFlag_CaseSensitivity_t _cs);

MEME_API mmint_t
	MEME_STDCALL MemeString_indexOfOther(
		mmstr_cptr_t _str, mmint_t _offset, mmint_t _limit,
		mmstr_cptr_t _other, mmint_t _count, int _full_match, mmflag_case_sensit_t _cs);

//! param _cond_func The function to determine whether the byte at the specified index meets the condition; return type is mmflag_cbproc_t.
MEME_API MemeInteger_t
MEME_STDCALL MemeString_indexByCondByteFunc(
    MemeString_Const_t _s, MemeInteger_t _offset,
    MemeString_MatchCondByteFunc_t* _cond_func, void* _user_data);

MEME_API MemeInteger_t
MEME_STDCALL MemeString_lastIndexOfWithUtf8bytes(
    MemeString_Const_t _s, MemeInteger_t _limit,
    const MemeByte_t* _needle, MemeInteger_t _needle_len,
    MemeFlag_CaseSensitivity_t _cs);

MEME_API mmint_t
MEME_STDCALL MemeString_lastIndexOfUtf8bytes(
	mmstr_cptr_t _s, mmint_t _offset, mmint_t _limit,
    const mmbyte_t* _needle, mmint_t _needle_len, 
	int _full_match, mmflag_case_sensit_t _cs);

MEME_API mmint_t
MEME_STDCALL MemeString_lastIndexOfOther(
	mmstr_cptr_t _s, mmint_t _offset, mmint_t _limit, 
	mmstr_cptr_t _other, mmint_t _count, int _full_match, mmflag_case_sensit_t _cs);

MEME_API MemeInteger_t
MEME_STDCALL MemeString_matchCountWithUtf8bytes(
    MemeString_Const_t _s, MemeInteger_t _offset,
    const MemeByte_t* _needle, MemeInteger_t _needle_len,
    MemeFlag_CaseSensitivity_t _cs);

MEME_API MemeInteger_t
MEME_STDCALL MemeString_startsMatchWithOther(
	MemeString_Const_t _s, MemeString_Const_t _other, MemeFlag_CaseSensitivity_t _cs);

MEME_API MemeInteger_t
MEME_STDCALL MemeString_startsMatchWithUtf8bytes(
    MemeString_Const_t _s, 
	const MemeByte_t* _needle, MemeInteger_t _needle_len, MemeFlag_CaseSensitivity_t _cs);

MEME_API MemeInteger_t
MEME_STDCALL MemeString_endsMatchWithOther(
	MemeString_Const_t _s, MemeString_Const_t _other, MemeFlag_CaseSensitivity_t _cs);

MEME_API MemeInteger_t
MEME_STDCALL MemeString_endsMatchWithUtf8bytes(
	MemeString_Const_t _s, 
	const MemeByte_t* _needle, MemeInteger_t _needle_len, MemeFlag_CaseSensitivity_t _cs);

MEME_API mmint_t
MEME_STDCALL MemeString_foreach(
    mmstr_cptr_t _str, mmstr_foreach_rune_cb_t* _cb, void* _user_data);

//! @brief Split the string into substrings.
//! @param _s The string.
//! @param _key The key to split the string.
//! @param _key_len The length of the key.
//! @param _behavior The behavior of the split.
//! @param _cs The case sensitivity of the split.
//! @param _out The output array of substrings.
//! @param _out_count The number of substrings.
MEME_API MemeInteger_t 
	MEME_STDCALL MemeString_split(
	MemeString_Const_t _s,
	const char* _key, MemeInteger_t _key_len, 
	MemeFlag_SplitBehavior_t, MemeFlag_CaseSensitivity_t,
	MemeStringStack_t* MEGO_SYMBOL__RESTRICT _out, 
	MemeInteger_t* MEGO_SYMBOL__RESTRICT _out_count,
	MemeInteger_t* MEGO_SYMBOL__RESTRICT _search_index
);

//! This is an example of how to use MemeString_splitByCondByteFunc.
//! \code 
//! int notalpha_func(void*, MemeByte_t _byte) {
//!     return !isalpha(_byte);
//! }
//! 
//! ...
//! 
//! MemeStringStack_t stack[3];
//! MemeInteger_t count = 3;
//! MemeInteger_t search_index;
//! MemeInteger_t result = 
//!     MemeString_splitByCondByteFunc(
//!         _s, // _s is ":::abc-def77vv..."
//!         notalpha_func, NULL,
//!         &stack, &count, &search_index
//!     );
//! // stack list is [ "abc", "def", "vv" ]
//! 
//! ...
//! 
//! \endcode
MEME_API mmint_t
	MEME_STDCALL MemeString_splitByCondByteFunc
	(
		mmstr_cptr_t _s,
		MemeString_MatchCondByteFunc_t* _cond_func, void* _user_data,
		mmstrstk_t* MEGO_SYMBOL__RESTRICT _out,
		mmint_t * MEGO_SYMBOL__RESTRICT _out_count,
		mmint_t * MEGO_SYMBOL__RESTRICT _search_index
	);

MEME_API mmint_t
MEME_STDCALL MemeString_writeBytes(
	mmstr_cptr_t _s, mmint_t _offset, mmint_t _count, mmbyte_t* _out);

MEME_API MemeInteger_t
MEME_STDCALL MemeString_writeU16Chars(mmstr_cptr_t _s, uint16_t* _out);

MEME_API const MemeByte_t*
	MEME_STDCALL MemeString_byteData(MemeString_Const_t _s);

MEME_API MemeInteger_t
	MEME_STDCALL MemeString_isSharedStorageTypes(MemeString_Const_t _s);

MEME_API MemeInteger_t
	MEME_STDCALL MemeString_getSharedHeapByteSize(MemeString_Const_t _s);

MEME_API MemeInteger_t
	MEME_STDCALL MemeString_getPrivateHeapByteSize(MemeString_Const_t _s);

MEME_API MemeInteger_t
	MEME_STDCALL MemeString_checkHeadTailMemory(MemeString_Const_t _s);



//****************************************************************************/

MG_CAPI_INLINE int 
mmstrstk_init(mmstrstk_t* _out, size_t _object_size) 
{
	assert(_out != NULL && mmstrstk_init != NULL);
	return MemeStringStack_init(_out, _object_size);
}

MG_CAPI_INLINE int 
mmstrstk_init_by_other(mmstrstk_t* _out, size_t _object_size, mmstr_cptr_t _other)
{
    assert(_out != NULL && mmstrstk_init_by_other != NULL);
	if (MEGO_SYMBOL__UNLIKELY(_other == NULL))
		return MemeStringStack_init(_out, _object_size);
    return MemeStringStack_initByOther(_out, _object_size, _other);
}

MG_CAPI_INLINE int 
mmstrstk_init_by_buf(
	mmstrstk_t* _out, size_t _object_size, mmbuf_cptr_t _other, mmint_t _offset)
{
    assert(_out != NULL && mmstrstk_init_by_buf != NULL);
    if (MEGO_SYMBOL__UNLIKELY(_other == NULL))
        return MemeStringStack_init(_out, _object_size);
    return MemeStringStack_initByBuffer(_out, _object_size, _other, _offset);
}

MG_CAPI_INLINE int
mmstrstk_init_by_utf8(
    mmstrstk_t* _out, size_t _object_size, const char* _utf8, mmint_t _utf8_len)
{
    assert(_out != NULL && mmstrstk_init_by_utf8 != NULL);
    if (MEGO_SYMBOL__UNLIKELY(_utf8 == NULL))
        return MemeStringStack_init(_out, _object_size);
    return MemeStringStack_initByU8bytes(_out, _object_size, (const mmbyte_t*)_utf8, _utf8_len);
}

MG_CAPI_INLINE int
mmstrstk_init_by_utf8_v2(
    mmstrstk_t* _out, size_t _object_size, 
	const char* _utf8, mmint_t _utf8_len, mmstr_strg_t _suggest)
{
    assert(_out != NULL && mmstrstk_init_by_utf8_v2 != NULL);
    if (MEGO_SYMBOL__UNLIKELY(_utf8 == NULL))
        return MemeStringStack_init(_out, _object_size);
    return MemeStringStack_initByU8bytesAndType(_out, _object_size, (const mmbyte_t*)_utf8, _utf8_len, _suggest);
}

MG_CAPI_INLINE int
mmstrstk_init_by_hexs(
	mmstrstk_t* _out, size_t _object_size,
	const mmbyte_t* _interval, mmint_t _ivlen, const uint8_t* _hexs, mmint_t _len)
{
    assert(_out != NULL && mmstrstk_init_by_hexs != NULL);
	if (_hexs == NULL)
		return MemeStringStack_init(_out, _object_size);
	return MemeStringStack_initWithHexadecimals(_out, _object_size, _interval, _ivlen, _hexs, _len);
}

MG_CAPI_INLINE int
mmstrstk_init_by_user(
	mmstrstk_t* _out, size_t _object_size,
	void* _user_data,
	MemeString_UserObjectDestruct_t* _destruct_fn,
	MemeString_UserObjectData_t* _data_fn,
	MemeString_UserObjectSize_t* _size_fn)
{
    assert(_out != NULL && mmstrstk_init_by_user != NULL);
	if (_user_data == NULL
		|| _destruct_fn == NULL
		|| _data_fn == NULL
		|| _size_fn == NULL)
		return MemeStringStack_init(_out, _object_size);
	return MemeStringStack_initTakeOverUserObject(
		_out, _object_size, _user_data, _destruct_fn, _data_fn, _size_fn);
}

MG_CAPI_INLINE int
mmstrstk_uninit(mmstrstk_t* _out, size_t _object_size)
{
    assert(_out != NULL && mmstrstk_uninit != NULL);
	return MemeStringStack_unInit(_out, _object_size);
}

MG_CAPI_INLINE int
mmstrstk_reset(mmstrstk_t* _out, size_t _object_size)
{
    assert(_out != NULL && mmstrstk_reset != NULL);
	return MemeStringStack_reset(_out, _object_size);
}

MG_CAPI_INLINE int
mmstr_assign(mmstr_ptr_t _s, mmstr_cptr_t _other)
{
    assert(_s != NULL && mmstr_assign != NULL);
	if (_other == NULL)
		return MemeStringStack_reset((mmstrstk_t*)_s, MMSTR__OBJ_SIZE);
	return MemeString_assign(_s, _other);
}

MG_CAPI_INLINE int
mmstr_assign_by_utf8(mmstr_ptr_t _s, const mmbyte_t* _utf8, mmint_t _len)
{
    assert(_s != NULL && mmstr_assign_by_utf8 != NULL);
	if (_utf8 == NULL)
		return MemeStringStack_reset((mmstrstk_t*)_s, MMSTR__OBJ_SIZE);
	return MemeStringStack_assignByU8bytes((mmstrstk_t*)_s, MMSTR__OBJ_SIZE, _utf8, _len);
}

MG_CAPI_INLINE int
mmstr_assign_by_buf(mmstr_ptr_t _out, mmbuf_cptr_t _other, mmint_t _offset)
{
    assert(_out != NULL && mmstr_assign_by_buf != NULL);
	if (_other == NULL)
		return MemeStringStack_reset((mmstrstk_t*)_out, MMSTR__OBJ_SIZE);
	return MemeStringStack_assignByBuffer((mmstrstk_t*)_out, MMSTR__OBJ_SIZE, _other, _offset);
}

//MEME_API int 
//MEME_STDCALL mmsstk_init(mmstrstk_t* _out, size_t _object_size);
//
//MEME_API int
//MEME_STDCALL mmsstk_init_by_other(mmstrstk_t* _out, size_t _object_size, mmstr_cptr_t _other);
//
//MEME_API int
//MEME_STDCALL mmsstk_init_by_buf(
//	mmstrstk_t* _out, size_t _object_size, mmbuf_cptr_t _other, mmint_t _offset);
//
//MEME_API int
//MEME_STDCALL mmsstk_init_by_utf8(
//	mmstrstk_t* _out, size_t _object_size, const mmbyte_t* _utf8, mmint_t _len);
//
//MEME_API int
//MEME_STDCALL mmsstk_init_by_utf8_v2(
//	mmstrstk_t* _out, size_t _object_size, const mmbyte_t* _utf8, mmint_t _len,
//	MemeString_Storage_t _suggest);
//
//MEME_API int
//MEME_STDCALL mmsstk_init_by_hexs(
//	mmstrstk_t* _out, size_t _object_size,
//	const mmbyte_t* _interval, mmint_t _ivlen, const uint8_t* _hexs, mmint_t _len);
//
//MEME_API int
//MEME_STDCALL mmsstk_init_by_user(
//	mmstrstk_t* _out, size_t _object_size,
//	void* _user_data,
//	MemeString_UserObjectDestruct_t* _destruct_fn,
//	MemeString_UserObjectData_t* _data_fn,
//	MemeString_UserObjectSize_t* _size_fn);
//
//MEME_API int
//MEME_STDCALL mmsstk_uninit(mmstrstk_t* _out, size_t _object_size);
//
//MEME_API int
//MEME_STDCALL mmsstk_reset(mmstrstk_t* _out, size_t _object_size);
//
//MEME_API int
//MEME_STDCALL mms_assign(mmstr_ptr_t _s, mmstr_cptr_t _other);
//
//MEME_API int
//MEME_STDCALL mms_assign_by_utf8(
//	mmstr_ptr_t _s, const mmbyte_t* _utf8, mmint_t _len);
//
//MEME_API int
//MEME_STDCALL mms_assign_by_buf(
//	mmstr_ptr_t _out, mmbuf_cptr_t _other, mmint_t _offset);


MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_STRING_H_INCLUDED
