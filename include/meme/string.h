
#ifndef MEME_STRING_H_INCLUDED
#define MEME_STRING_H_INCLUDED

#include "meme/common.h"
#include "meme/string_fwd.h"
#include "meme/string_memory_fwd.h"
#include "meme/buffer_fwd.h"

MEME_EXTERN_C_SCOPE_START

MEME_API int
	MEME_STDCALL MemeStringOption_setStorageMediumLimit(size_t _value);

MEME_API size_t
	MEME_STDCALL MemeStringOption_getStorageMediumLimit();



MEME_API int 
	MEME_STDCALL MemeStringStack_init(MemeStringStack_t* _out, size_t _object_size);

MEME_API MemeStringStack_t
	MEME_STDCALL MemeStringStack_getInitObject(size_t _object_size);

MEME_API int
	MEME_STDCALL MemeStringStack_initTakeOverUserObject(
		MemeStringStack_t* _out, size_t _object_size,
		void* _user_data, 
		MemeString_UserObjectDestruct_t* _destruct_fn, 
		MemeString_UserObjectData_t* _data_fn, 
		MemeString_UserObjectSize_t* _size_fn);

MEME_API int
	MEME_STDCALL MemeStringStack_initByOther(
		MemeStringStack_t* _out, size_t _object_size, MemeString_Const_t _other);

MEME_API int
MEME_STDCALL MemeStringStack_initByBuffer(
	MemeStringStack_t* _out, size_t _object_size, MemeBuffer_Const_t _other, MemeInteger_t _offset);

MEME_API int
	MEME_STDCALL MemeStringStack_initByWord(
		MemeStringStack_t* _out, size_t _object_size, MemeInteger_t _count, MemeWord_t _ch);

MEME_API int
	MEME_STDCALL MemeStringStack_initByU8bytes(
		MemeStringStack_t* _out, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len);

MEME_API int
	MEME_STDCALL MemeStringStack_initByU8bytesAndType(
		MemeStringStack_t* _out, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len,
		MemeString_Storage_t _suggest);

//MEME_API int 
//	MEME_STDCALL MemeStringStack_initByOtherAndType(
//		MemeStringStack_t* _out, size_t _object_size, 
//		MemeString_Const_t _other, MemeString_Storage_t _suggest
//	);

MEME_API int
	MEME_STDCALL MemeStringStack_initWithHexadecimals(
		MemeStringStack_t* _out, size_t _object_size, 
		const MemeByte_t* _interval, MemeInteger_t _ivlen, const uint8_t* _hexs, MemeInteger_t _len);

MEME_API int
	MEME_STDCALL MemeStringStack_unInit(MemeStringStack_t* _out, size_t _object_size);

MEME_API int 
	MEME_STDCALL MemeStringStack_reset (MemeStringStack_t* _out, size_t _object_size);

MEME_API int
	MEME_STDCALL MemeStringStack_assign_v02(MemeStringStack_t* _s, size_t _object_size, MemeString_Const_t _other);


MEME_API MemeString_Storage_t
	MEME_STDCALL MemeString_storageType(MemeString_Const_t _s);

MEME_API int 
	MEME_STDCALL MemeString_create(MemeString_t* _out);

MEME_API int 
	MEME_STDCALL MemeString_createWithType(MemeString_t* _out, MemeString_Storage_t);

MEME_API int 
	MEME_STDCALL MemeString_createByOther(MemeString_t* _out, MemeString_Const_t _other);

MEME_API int 
	MEME_STDCALL MemeString_createByWord(MemeString_t* _out, size_t _len, MemeWord_t _ch);

MEME_API int 
	MEME_STDCALL MemeString_createByUtf8bytes(MemeString_t* _out, const MemeByte_t* _utf8, size_t _len);

MEME_API int 
	MEME_STDCALL MemeString_destroy(MemeString_t* _out);

MEME_API int 
	MEME_STDCALL MemeString_assign(MemeString_t _s, MemeString_Const_t _other);

MEME_API int 
	MEME_STDCALL MemeString_swap(MemeString_t _lhs, MemeString_t _rhs);

MEME_API int 
	MEME_STDCALL MemeString_isEmpty(MemeString_Const_t _s);

MEME_API int
	MEME_STDCALL MemeString_isEmpty_v02(MemeString_Const_t _s);

MEME_API const char* 
	MEME_STDCALL MemeString_cStr(MemeString_Const_t _s);

MEME_API MemeInteger_t 
	MEME_STDCALL MemeString_byteSize(MemeString_Const_t _s);
MEME_API MemeInteger_t 
	MEME_STDCALL MemeString_wordSize(MemeString_Const_t _s);
MEME_API MemeInteger_t 
	MEME_STDCALL MemeString_availableByteCapacity(MemeString_Const_t _s);
MEME_API MemeInteger_t
	MEME_STDCALL MemeString_maxByteSize(MemeString_Const_t _s);
MEME_API MemeInteger_t
MEME_STDCALL MemeString_maxByteCapacity(MemeString_Const_t _s);

MEME_API MemeWordIndex_t 
	MEME_STDCALL MemeString_at(MemeString_Const_t _s, size_t _index);

MEME_API int
	MEME_STDCALL MemeString_isEqual(MemeString_Const_t _s, const char* _str, MemeInteger_t _len, int* _result);
MEME_API int
	MEME_STDCALL MemeString_isEqualWithOther(MemeString_Const_t _lhs, MemeString_Const_t _rhs, int* _result);

MEME_API MemeInteger_t
	MEME_STDCALL MemeString_indexOfWithUtf8bytes(
		MemeString_Const_t _s, MemeInteger_t _offset,
		const MemeByte_t* _needle, MemeInteger_t _needle_len,
		MemeFlag_CaseSensitivity_t _cs);

MEME_API MemeInteger_t
	MEME_STDCALL MemeString_indexOfWithOther(
		MemeString_Const_t _s, MemeInteger_t _offset,
		MemeString_Const_t _other, MemeFlag_CaseSensitivity_t _cs);

MEME_API MemeInteger_t
MEME_STDCALL MemeString_split(
	MemeString_Const_t _s,
	const char* _key, MemeInteger_t _key_len, 
	MemeFlag_SplitBehavior_t, MemeFlag_CaseSensitivity_t,
	MemeStringStack_t* _out, MemeInteger_t* _out_count,
	MemeInteger_t* _search_index
);

MEME_API const MemeByte_t*
	MEME_STDCALL MemeString_byteData(MemeString_Const_t _s);

MEME_API MemeInteger_t
MEME_STDCALL MemeString_isSharedStorageTypes(MemeString_Const_t _s);


MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_STRING_H_INCLUDED
