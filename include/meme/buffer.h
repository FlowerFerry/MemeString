
#ifndef MEME_BUFFER_H_INCLUDED
#define MEME_BUFFER_H_INCLUDED

#include "buffer_fwd.h"
#include "mego/predef/symbol/deprecated.h"

MEME_EXTERN_C_SCOPE_START

MEME_API int
MEME_STDCALL MemeBufferStack_init(mmbufstk_t* _out, size_t _object_size);

//MEME_API int
//MEME_STDCALL MemeBufferStack_initTakeOverUserObject(
//	MemeBufferStack_t* _out, size_t _object_size,
//	void* _user_data,
//	MemeBuffer_UserObjectDestruct_t* _destruct_fn,
//	MemeBuffer_UserObjectData_t* _data_fn,
//	MemeBuffer_UserObjectSize_t* _size_fn);

MEME_API int
MEME_STDCALL MemeBufferStack_initByOther(
	mmbufstk_t* _out, size_t _object_size, const mmbufstk_t* _other);

MEME_API int
MEME_STDCALL MemeBufferStack_initByString(
	mmbufstk_t* _out, size_t _object_size, mmstr_cptr_t _s);

MEME_API int
MEME_STDCALL MemeBufferStack_initByBytes(
	mmbufstk_t* _out, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len);

MEME_API int
MEME_STDCALL MemeBufferStack_initByU8bytesAndType(
	mmbufstk_t* _out, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len,
		MemeBuffer_Storage_t _suggest);

MEME_API int
MEME_STDCALL MemeBufferStack_initAndConditionalConvert(
	mmbufstk_t* _out, size_t _object_size, mmbuf_cptr_t _other);

//! @param _out must be initialized
MEME_API int
MEME_STDCALL MemeBufferStack_unInit(mmbufstk_t* _out, size_t _object_size);

//! @param _out must be initialized
MEME_API int
MEME_STDCALL MemeBufferStack_reset(mmbufstk_t* _out, size_t _object_size);

//! @param _s     must be initialized
//! @param _other must be initialized
MEME_API int
MEME_STDCALL MemeBufferStack_assign(
	mmbufstk_t* _s, size_t _object_size, const MemeBufferStack_t* _other);

MEME_API MemeBuffer_Storage_t
MEME_STDCALL MemeBuffer_storageType(MemeBuffer_Const_t _s);

MEME_API int
MEME_STDCALL MemeBuffer_swap(MemeBuffer_t _lhs, MemeBuffer_t _rhs);

MEME_API int
MEME_STDCALL MemeBuffer_isNonempty(MemeBuffer_Const_t _s);

MEME_API int
MEME_STDCALL MemeBuffer_isEmpty(MemeBuffer_Const_t _s);

MEME_API const mmbyte_t*
MEME_STDCALL MemeBuffer_at(mmbuf_cptr_t _s, mmint_t _pos);

MEME_API const MemeByte_t*
MEME_STDCALL MemeBuffer_data(MemeBuffer_Const_t _s);

MEME_API MemeInteger_t
MEME_STDCALL MemeBuffer_size(MemeBuffer_Const_t _s);

MEME_API int
MEME_STDCALL MemeBuffer_isEqual(
	MemeBuffer_Const_t _s, const MemeByte_t* _buf, MemeInteger_t _len, int* _result);

MEME_API int
MEME_STDCALL MemeBuffer_isEqualWithOther(
	MemeBuffer_Const_t _lhs, MemeBuffer_Const_t _rhs, int* _result);

MEME_API mmint_t
MEME_STDCALL MemeBuffer_isSharedStorageTypes(mmbuf_const_t _b);

MEME_API MemeInteger_t
MEME_STDCALL MemeBuffer_indexOfWithBytes(
	MemeBuffer_Const_t _s, MemeInteger_t _offset,
	const MemeByte_t* _needle, MemeInteger_t _needle_len);

MEME_API MemeInteger_t
MEME_STDCALL MemeBuffer_indexOfWithOther(
	MemeBuffer_Const_t _s, MemeInteger_t _offset, MemeBuffer_Const_t _other);

MEME_API MemeInteger_t
MEME_STDCALL MemeBuffer_startsMatchWithBytes(
	MemeBuffer_Const_t _s,
	const MemeByte_t* _needle, MemeInteger_t _needle_len);

MEME_API MemeInteger_t
MEME_STDCALL MemeBuffer_startsMatchWithOther(
	MemeBuffer_Const_t _s, MemeBuffer_Const_t _other);

MEME_API MemeInteger_t
MEME_STDCALL MemeBuffer_endsMatchWithBytes(
	MemeBuffer_Const_t _s,
	const MemeByte_t* _needle, MemeInteger_t _needle_len);

MEME_API MemeInteger_t
MEME_STDCALL MemeBuffer_endsMatchWithOther(
	MemeBuffer_Const_t _s, MemeBuffer_Const_t _other);

MEME_API MemeInteger_t
MEME_STDCALL MemeBuffer_split(
	MemeBuffer_Const_t _s,
	const MemeByte_t* _key, MemeInteger_t _key_len,
	MemeFlag_SplitBehavior_t,
	MemeBufferStack_t* _out, MemeInteger_t* _out_count,
	MemeInteger_t* _search_index
);


MG_CAPI_INLINE int
mmbufstk_init(mmbufstk_t* _out, size_t _object_size)
{
    assert(_out != NULL && "mmbufstk_init: null pointer");
	
    return MemeBufferStack_init(_out, _object_size);
}

MG_CAPI_INLINE int
mmbufstk_init_by_other(mmbufstk_t* _out, size_t _object_size, const mmbufstk_t* _other)
{
    assert(_out != NULL && "mmbufstk_init_by_other: null pointer");

    return MemeBufferStack_initByOther(_out, _object_size, _other);
}

MG_CAPI_INLINE int
mmbufstk_init_by_str(mmbufstk_t* _out, size_t _object_size, mmstr_cptr_t _s)
{
    assert(_out != NULL && "mmbufstk_init_by_string: null pointer");

    return MemeBufferStack_initByString(_out, _object_size, _s);
}

MG_CAPI_INLINE int
mmbufstk_init_by_bytes(
	mmbufstk_t* _out, size_t _object_size, const mmbyte_t* _utf8, mmint_t _len)
{
    assert(_out != NULL && "mmbufstk_init_by_bytes: null pointer");

    return MemeBufferStack_initByBytes(_out, _object_size, _utf8, _len);
}

MG_CAPI_INLINE int
mmbufstk_init_by_bytes_with_type(
    mmbufstk_t* _out, size_t _object_size, const mmbyte_t* _utf8, mmint_t _len, mmbuf_strg_t _type)
{
    assert(_out != NULL && "mmbufstk_init_by_bytes_with_type: null pointer");

    return MemeBufferStack_initByU8bytesAndType(_out, _object_size, _utf8, _len, _type);
}

MG_CAPI_INLINE int
mmbufstk_init_cond_copy(
    mmbufstk_t* _out, size_t _object_size, mmbuf_cptr_t _other)
{
    assert(_out != NULL && "mmbufstk_init_cond_copy: null pointer");

    return MemeBufferStack_initAndConditionalConvert(_out, _object_size, _other);
}

MG_CAPI_INLINE int
mmbufstk_uninit(mmbufstk_t* _out, size_t _object_size)
{
    assert(_out != NULL && "mmbufstk_uninit: null pointer");

    return MemeBufferStack_unInit(_out, _object_size);
}

MG_CAPI_INLINE int
mmbufstk_reset(mmbufstk_t* _out, size_t _object_size)
{
    assert(_out != NULL && "mmbufstk_reset: null pointer");

    return MemeBufferStack_reset(_out, _object_size);
}

MG_CAPI_INLINE int
mmbufstk_assign(mmbufstk_t* _out, size_t _object_size, const mmbufstk_t* _other)
{
    assert(_out != NULL && "mmbufstk_assign: null pointer");

    return MemeBufferStack_assign(_out, _object_size, _other);
}

MG_CAPI_INLINE mmbuf_strg_t mmbuf_strg_type(mmbuf_cptr_t _b)
{
    assert(_b != NULL && "mmbuf_strg_type: null pointer");

    return MemeBuffer_storageType(_b);
}

MG_CAPI_INLINE int mmbuf_swap(mmbuf_ptr_t _a, mmbuf_ptr_t _b)
{
    assert(_a != NULL && "mmbuf_swap: null pointer");
    assert(_b != NULL && "mmbuf_swap: null pointer");

    return MemeBuffer_swap(_a, _b);
}

MG_CAPI_INLINE int mmbuf_is_empty(mmbuf_cptr_t _b)
{
    assert(_b != NULL && "mmbuf_is_empty: null pointer");

    return MemeBuffer_isEmpty(_b);
}

MG_CAPI_INLINE const mmbyte_t* mmbuf_at(mmbuf_cptr_t _b, mmint_t _index)
{
    assert(_b != NULL && "mmbuf_at: null pointer");

    return MemeBuffer_at(_b, _index);
}

MG_CAPI_INLINE const mmbyte_t* mmbuf_data(mmbuf_cptr_t _b)
{
    assert(_b != NULL && "mmbuf_data: null pointer");

    return MemeBuffer_data(_b);
}

MG_CAPI_INLINE mmint_t mmbuf_size(mmbuf_cptr_t _b)
{
    assert(_b != NULL && "mmbuf_size: null pointer");

    return MemeBuffer_size(_b);
}

MG_CAPI_INLINE int mmbuf_is_equal(
    mmbuf_cptr_t _b, const mmbyte_t* _buf, mmint_t _len, int* _result)
{
    assert(_b != NULL && "mmbuf_is_equal: null pointer");

    return MemeBuffer_isEqual(_b, _buf, _len, _result);
}

MG_CAPI_INLINE int mmbuf_is_equal_other(mmbuf_cptr_t _b, mmbuf_cptr_t _other, int* _result)
{
    assert(_b != NULL && "mmbuf_is_equal_other: null pointer");

    return MemeBuffer_isEqualWithOther(_b, _other, _result);
}

MG_CAPI_INLINE int mmbuf_is_shared_strg_type(mmbuf_cptr_t _b)
{
    assert(_b != NULL && "mmbuf_is_shared_strg_type: null pointer");

    return (int)MemeBuffer_isSharedStorageTypes(_b);
}

MG_CAPI_INLINE mmint_t mmbuf_index_of_bytes(
    mmbuf_cptr_t _b, mmint_t _offset, const mmbyte_t* _buf, mmint_t _len)
{
    assert(_b != NULL && "mmbuf_index_of_bytes: null pointer");

    return MemeBuffer_indexOfWithBytes(_b, _offset, _buf, _len);
}

MG_CAPI_INLINE mmint_t mmbuf_index_of_other(
    mmbuf_cptr_t _b, mmint_t _offset, mmbuf_cptr_t _other)
{
    assert(_b != NULL && "mmbuf_index_of_other: null pointer");

    return MemeBuffer_indexOfWithOther(_b, _offset, _other);
}

MG_CAPI_INLINE int mmbuf_starts_match_with_bytes(
    mmbuf_cptr_t _b, const mmbyte_t* _buf, mmint_t _len)
{
    assert(_b != NULL && "mmbuf_starts_with_bytes: null pointer");

    return (int)MemeBuffer_startsMatchWithBytes(_b, _buf, _len);
}

MG_CAPI_INLINE int mmbuf_starts_match_with_other(mmbuf_cptr_t _b, mmbuf_cptr_t _other)
{
    assert(_b != NULL && "mmbuf_starts_with_other: null pointer");

    return (int)MemeBuffer_startsMatchWithOther(_b, _other);
}

MG_CAPI_INLINE int mmbuf_ends_match_with_bytes(
    mmbuf_cptr_t _b, const mmbyte_t* _buf, mmint_t _len)
{
    assert(_b != NULL && "mmbuf_ends_with_bytes: null pointer");

    return (int)MemeBuffer_endsMatchWithBytes(_b, _buf, _len);
}

MG_CAPI_INLINE int mmbuf_ends_match_with_other(mmbuf_cptr_t _b, mmbuf_cptr_t _other)
{
    assert(_b != NULL && "mmbuf_ends_with_other: null pointer");

    return (int)MemeBuffer_endsMatchWithOther(_b, _other);
}

MG_CAPI_INLINE mmint_t mmbuf_split(
    mmbuf_cptr_t _b, const mmbyte_t* _key, mmint_t _key_len,
    mmflag_split_behav_t _behav,
    mmbufstk_t* _out, mmint_t* _out_count,
    mmint_t* _last_index)
{
    assert(_b != NULL && "mmbuf_split: null pointer");

    return MemeBuffer_split(
        _b, _key, _key_len, _behav, _out, _out_count, _last_index);
}

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_BUFFER_H_INCLUDED
