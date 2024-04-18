
#ifndef MEME_VARIANT_H_INCLUDED
#define MEME_VARIANT_H_INCLUDED

#include "variant_fwd.h"
#include <meme/string_fwd.h>
#include <meme/buffer_fwd.h>
#include <meme/variable_buffer_fwd.h>
#include <meme/rune.h>
#include <mego/err/ec.h>
#include <mego/predef/symbol/inline.h>

#include <stddef.h>

MEME_EXTERN_C_SCOPE_START

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_init(
        mmvarstk_t* _obj, size_t _object_size);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByOther(
        mmvarstk_t* _obj, size_t _object_size, mmvar_cptr_t _var);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByMove(
        mmvarstk_t* _obj, size_t _object_size, mmvarstk_t* _var);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByByte(
        mmvarstk_t* _obj, size_t _object_size, mmbyte_t _val);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByChar(
        mmvarstk_t* _obj, size_t _object_size, char _val);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByWChar(
        mmvarstk_t* _obj, size_t _object_size, wchar_t _val);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByInt64(
        mmvarstk_t* _obj, size_t _object_size, int64_t _val);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByUInt64(
        mmvarstk_t* _obj, size_t _object_size, uint64_t _val);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByDouble(
        mmvarstk_t* _obj, size_t _object_size, double _val);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByString(
        mmvarstk_t* _obj, size_t _object_size, mmstr_cptr_t _val);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByBuffer(
        mmvarstk_t* _obj, size_t _object_size, mmbuf_cptr_t _val);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByVariableBuffer(
        mmvarstk_t* _obj, size_t _object_size, mmvb_cptr_t _val);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByRune(
        mmvarstk_t* _obj, size_t _object_size, const mmrune_t* _val);

MEME_API int MEME_STDCALL 
    MemeVariantStack_initAndConditionalConvert(
        mmvarstk_t* _out, size_t _object_size, mmvar_cptr_t _other);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_unInit(
        mmvarstk_t* _obj, size_t _object_size);

//! @param _obj must be initialized
//! @param _var must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_assign(
        mmvarstk_t* _obj, size_t _object_size, mmvar_cptr_t _var);

//! @param _lhs must be initialized
//! @param _rhs must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_swap(
        mmvarstk_t* _lhs, mmvarstk_t* _rhs, size_t _object_size);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_reset(
        mmvarstk_t* _obj, size_t _object_size);

//! @param _obj must be initialized
MEME_API MemeMetaTypeId_t MEME_STDCALL
    MemeVariantStack_typeId(
        const mmvarstk_t* _obj, size_t _object_size);

//! @param _obj must be initialized
MEME_API int MEME_STDCALL
    MemeVariantStack_isNull(
        const mmvarstk_t* _obj, size_t _object_size);

//! @param _obj must be initialized
MEME_API int MEME_STDCALL
    MemeVariantStack_isByte(
        const mmvarstk_t* _obj, size_t _object_size);

//! @param _obj must be initialized
MEME_API int MEME_STDCALL
    MemeVariantStack_isChar(
        const mmvarstk_t* _obj, size_t _object_size);

//! @param _obj must be initialized
MEME_API int MEME_STDCALL
    MemeVariantStack_isWChar(
        const mmvarstk_t* _obj, size_t _object_size);

//! @param _obj must be initialized
MEME_API int MEME_STDCALL
    MemeVariantStack_isInt64(
        const mmvarstk_t* _obj, size_t _object_size);

//! @param _obj must be initialized
MEME_API int MEME_STDCALL
    MemeVariantStack_isUInt64(
        const mmvarstk_t* _obj, size_t _object_size);

//! @param _obj must be initialized
MEME_API int MEME_STDCALL
    MemeVariantStack_isDouble(
        const mmvarstk_t* _obj, size_t _object_size);

//! @param _obj must be initialized
MEME_API int MEME_STDCALL
    MemeVariantStack_isString(
        const mmvarstk_t* _obj, size_t _object_size);

//! @param _obj must be initialized
MEME_API int MEME_STDCALL
    MemeVariantStack_isBuffer(
        const mmvarstk_t* _obj, size_t _object_size);

//! @param _obj must be initialized
MEME_API int MEME_STDCALL
    MemeVariantStack_isVariableBuffer(
        const mmvarstk_t* _obj, size_t _object_size);

//! @param _obj must be initialized
MEME_API int MEME_STDCALL
    MemeVariantStack_isRune(
        const mmvarstk_t* _obj, size_t _object_size);

//! @param _obj must be initialized
MEME_API int MEME_STDCALL
    MemeVariantStack_isUser(
        const mmvarstk_t* _obj, size_t _object_size);

//! @param _obj must be initialized
MEME_API int MEME_STDCALL
    MemeVariantStack_isType(
        const mmvarstk_t* _obj, size_t _object_size, MemeMetaTypeId_t _type);

////! @param _obj must be initialized
//MEME_API int MEME_STDCALL
//    MemeVariantStack_isEqual(
//        const mmvarstk_t* _lhs, const mmvarstk_t* _rhs, size_t _object_size);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_convToByte(
        const mmvarstk_t* _obj, size_t _object_size, mmbyte_t* _out);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_convToChar(
        const mmvarstk_t* _obj, size_t _object_size, char* _out);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_convToWChar(
        const mmvarstk_t* _obj, size_t _object_size, wchar_t* _out);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_convToInt(
        const mmvarstk_t* _obj, size_t _object_size, mmint_t* _out);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_convToUInt(
        const mmvarstk_t* _obj, size_t _object_size, size_t* _out);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_convToInt64(
        const mmvarstk_t* _obj, size_t _object_size, int64_t* _out);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_convToUInt64(
        const mmvarstk_t* _obj, size_t _object_size, uint64_t* _out);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_convToDouble(
        const mmvarstk_t* _obj, size_t _object_size, double* _out);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_convToString(
        const mmvarstk_t* _obj, size_t _object_size, mmstrstk_t* _out);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_convToBuffer(
        const mmvarstk_t* _obj, size_t _object_size, mmbufstk_t* _out);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_convToVariableBuffer(
        const mmvarstk_t* _obj, size_t _object_size, mmvbstk_t* _out);

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_convToRune(
        const mmvarstk_t* _obj, size_t _object_size, mmrune_t* _out);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setNull(
        mmvarstk_t* _obj, size_t _object_size);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setByte(
        mmvarstk_t* _obj, size_t _object_size, mmbyte_t _val);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setChar(
        mmvarstk_t* _obj, size_t _object_size, char _val); 

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setWChar(
        mmvarstk_t* _obj, size_t _object_size, wchar_t _val);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setInt(
        mmvarstk_t* _obj, size_t _object_size, mmint_t _val);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setUInt(
        mmvarstk_t* _obj, size_t _object_size, size_t _val);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setInt64(
        mmvarstk_t* _obj, size_t _object_size, int64_t _val);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setUInt64(
        mmvarstk_t* _obj, size_t _object_size, uint64_t _val);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setDouble(
        mmvarstk_t* _obj, size_t _object_size, double _val);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setString(
        mmvarstk_t* _obj, size_t _object_size, mmstr_cptr_t _val);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setBuffer(
        mmvarstk_t* _obj, size_t _object_size, mmbuf_cptr_t _val);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setVariableBuffer(
        mmvarstk_t* _obj, size_t _object_size, mmvb_cptr_t _val);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setRune(
        mmvarstk_t* _obj, size_t _object_size, const mmrune_t* _val);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getByte(
        const mmvarstk_t* _obj, size_t _object_size, mmbyte_t* _out);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getChar(
        const mmvarstk_t* _obj, size_t _object_size, char* _out);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getWChar(
        const mmvarstk_t* _obj, size_t _object_size, wchar_t* _out);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getInt64(
        const mmvarstk_t* _obj, size_t _object_size, int64_t* _out);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getUInt64(
        const mmvarstk_t* _obj, size_t _object_size, uint64_t* _out);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getDouble(
        const mmvarstk_t* _obj, size_t _object_size, double* _out);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getString(
        const mmvarstk_t* _obj, size_t _object_size, mmstrstk_t* _out, int _is_str_inited);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getBuffer(
        const mmvarstk_t* _obj, size_t _object_size, mmbufstk_t* _out, int _is_buf_inited);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getVariableBuffer(
        const mmvarstk_t* _obj, size_t _object_size, mmvbstk_t* _out, int _is_vbuf_inited);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getRune(
        const mmvarstk_t* _obj, size_t _object_size, mmrune_t* _out);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getStringView(
        const mmvarstk_t* _obj, size_t _object_size, mmstrstk_t* _out, int _is_inited);

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getBufferView(
        const mmvarstk_t* _obj, size_t _object_size, mmbufstk_t* _out, int _is_inited);



MG_CAPI_INLINE mgec_t mmvarstk_init(mmvarstk_t* _obj, size_t _object_size)
{
    assert(_obj != NULL && "mmvarstk_init");
    assert(_object_size > 0 && "mmvarstk_init");
    
    return MemeVariantStack_init(_obj, _object_size);
}

MG_CAPI_INLINE mgec_t mmvarstk_init_by_other(
    mmvarstk_t* _obj, size_t _object_size, mmvar_cptr_t _other)
{
    assert(_obj != NULL && "mmvarstk_init_by_other");
    assert(_object_size > 0 && "mmvarstk_init_by_other");

    return MemeVariantStack_initByOther(_obj, _object_size, _other);
}

MG_CAPI_INLINE mgec_t mmvarstk_init_by_move(
    mmvarstk_t* _obj, size_t _object_size, mmvarstk_t* _other)
{
    assert(_obj != NULL && "mmvarstk_init_by_move");
    assert(_object_size > 0 && "mmvarstk_init_by_move");

    return MemeVariantStack_initByMove(_obj, _object_size, _other);
}

MG_CAPI_INLINE mgec_t mmvarstk_init_by_byte(
    mmvarstk_t* _obj, size_t _object_size, mmbyte_t _val)
{
    assert(_obj != NULL && "mmvarstk_init_by_byte");
    assert(_object_size > 0 && "mmvarstk_init_by_byte");

    return MemeVariantStack_initByByte(_obj, _object_size, _val);
}

MG_CAPI_INLINE mgec_t mmvarstk_init_by_char(
    mmvarstk_t* _obj, size_t _object_size, char _val)
{
    assert(_obj != NULL && "mmvarstk_init_by_char");
    assert(_object_size > 0 && "mmvarstk_init_by_char");

    return MemeVariantStack_initByChar(_obj, _object_size, _val);
}

MG_CAPI_INLINE mgec_t mmvarstk_init_by_wchar(
    mmvarstk_t* _obj, size_t _object_size, wchar_t _val)
{
    assert(_obj != NULL && "mmvarstk_init_by_wchar");
    assert(_object_size > 0 && "mmvarstk_init_by_wchar");

    return MemeVariantStack_initByWChar(_obj, _object_size, _val);
}

MG_CAPI_INLINE mgec_t mmvarstk_init_by_int64(
    mmvarstk_t* _obj, size_t _object_size, int64_t _val)
{
    assert(_obj != NULL && "mmvarstk_init_by_int64");
    assert(_object_size > 0 && "mmvarstk_init_by_int64");

    return MemeVariantStack_initByInt64(_obj, _object_size, _val);
}

MG_CAPI_INLINE mgec_t mmvarstk_init_by_uint64(
    mmvarstk_t* _obj, size_t _object_size, uint64_t _val)
{
    assert(_obj != NULL && "mmvarstk_init_by_uint64");
    assert(_object_size > 0 && "mmvarstk_init_by_uint64");

    return MemeVariantStack_initByUInt64(_obj, _object_size, _val);
}

MG_CAPI_INLINE mgec_t mmvarstk_init_by_double(
    mmvarstk_t* _obj, size_t _object_size, double _val)
{
    assert(_obj != NULL && "mmvarstk_init_by_double");
    assert(_object_size > 0 && "mmvarstk_init_by_double");

    return MemeVariantStack_initByDouble(_obj, _object_size, _val);
}

MG_CAPI_INLINE mgec_t mmvarstk_init_by_str(
    mmvarstk_t* _obj, size_t _object_size, mmstr_cptr_t _val)
{
    assert(_obj != NULL && "mmvarstk_init_by_string");
    assert(_object_size > 0 && "mmvarstk_init_by_string");

    return MemeVariantStack_initByString(_obj, _object_size, _val);
}

MG_CAPI_INLINE mgec_t mmvarstk_init_by_buf(
    mmvarstk_t* _obj, size_t _object_size, mmbuf_cptr_t _val)
{
    assert(_obj != NULL && "mmvarstk_init_by_buffer");
    assert(_object_size > 0 && "mmvarstk_init_by_buffer");

    return MemeVariantStack_initByBuffer(_obj, _object_size, _val);
}

MG_CAPI_INLINE mgec_t mmvarstk_init_by_rune(
    mmvarstk_t* _obj, size_t _object_size, const mmrune_t* _val)
{
    assert(_obj != NULL && "mmvarstk_init_by_rune");
    assert(_object_size > 0 && "mmvarstk_init_by_rune");

    return MemeVariantStack_initByRune(_obj, _object_size, _val);
}

MG_CAPI_INLINE mgec_t mmvarstk_uninit(mmvarstk_t* _obj, size_t _object_size)
{
    assert(_obj != NULL && "mmvarstk_uninit");

    return MemeVariantStack_unInit(_obj, _object_size);
}

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_VARIANT_H_INCLUDED
