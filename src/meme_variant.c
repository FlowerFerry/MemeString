
#include <meme/variant.h>
#include <meme/string.h>
#include <meme/buffer.h>
#include <meme/variable_buffer.h>
#include <meme/rune.h>
#include <meme/unsafe/string_view.h>
#include <meme/unsafe/buffer_view.h>
#include <meme/impl/variant.h>

#include <stdlib.h>
#include <float.h>
#include <math.h>

MEME_EXTERN_C_SCOPE_START


mmint_t MemeVariantImpl_basicNumberToInt (const MemeVariant_t* _var)
{
    switch (_var->type) {
    case MMMETA_TYPID__BYTE:   return (mmint_t)_var->d.b;
    case MMMETA_TYPID__CHAR:   return (mmint_t)_var->d.c;
    case MMMETA_TYPID__WCHAR:  return (mmint_t)_var->d.wc;
    case MMMETA_TYPID__INT64:  return (mmint_t)_var->d.i64;
    case MMMETA_TYPID__UINT64: return (mmint_t)_var->d.u64;
    case MMMETA_TYPID__DOUBLE: return (mmint_t)_var->d.d;
    default: {
        assert(0);
        return 0; 
    }
    }
}

size_t  MemeVariantImpl_basicNumberToUInt(const MemeVariant_t* _var)
{
    switch (_var->type) {
    case MMMETA_TYPID__BYTE:   return (size_t)_var->d.b;
    case MMMETA_TYPID__CHAR:   return (size_t)_var->d.c;
    case MMMETA_TYPID__WCHAR:  return (size_t)_var->d.wc;
    case MMMETA_TYPID__INT64:  return (size_t)_var->d.i64;
    case MMMETA_TYPID__UINT64: return (size_t)_var->d.u64;
    case MMMETA_TYPID__DOUBLE: return (size_t)_var->d.d;
    default: {
        assert(0);
        return 0;
    }
    }
}

int64_t  MemeVariantImpl_basicNumberToInt64(const MemeVariant_t * _var)
{
    switch (_var->type) {
    case MMMETA_TYPID__BYTE:   return (int64_t)_var->d.b;
    case MMMETA_TYPID__CHAR:   return (int64_t)_var->d.c;
    case MMMETA_TYPID__WCHAR:  return (int64_t)_var->d.wc;
    case MMMETA_TYPID__INT64:  return (int64_t)_var->d.i64;
    case MMMETA_TYPID__UINT64: return (int64_t)_var->d.u64;
    case MMMETA_TYPID__DOUBLE: return (int64_t)_var->d.d;
    default: {
        assert(0);
        return 0;
    }
    }
}

uint64_t MemeVariantImpl_basicNumberToUInt64(const MemeVariant_t* _var)
{
    switch (_var->type) {
    case MMMETA_TYPID__BYTE:   return (uint64_t)_var->d.b;
    case MMMETA_TYPID__CHAR:   return (uint64_t)_var->d.c;
    case MMMETA_TYPID__WCHAR:  return (uint64_t)_var->d.wc;
    case MMMETA_TYPID__INT64:  return (uint64_t)_var->d.i64;
    case MMMETA_TYPID__UINT64: return (uint64_t)_var->d.u64;
    case MMMETA_TYPID__DOUBLE: return (uint64_t)_var->d.d;
    default: {
        assert(0);
        return 0;
    }
    }
}

double  MemeVariantImpl_basicNumberToDouble(const MemeVariant_t* _var)
{
    switch (_var->type) {
    case MMMETA_TYPID__BYTE:   return (double)_var->d.b;
    case MMMETA_TYPID__CHAR:   return (double)_var->d.c;
    case MMMETA_TYPID__WCHAR:  return (double)_var->d.wc;
    case MMMETA_TYPID__INT64:  return (double)_var->d.i64;
    case MMMETA_TYPID__UINT64: return (double)_var->d.u64;
    case MMMETA_TYPID__DOUBLE: return (double)_var->d.d;
    default: {
        assert(0);
        return 0;
    }
    }
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_init(
        mmvarstk_t* _obj, size_t _object_size)
{
    assert(_obj != NULL && MemeVariantStack_init != NULL);

    memset(_obj, 0, _object_size);
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByOther(
        mmvarstk_t* _obj, size_t _object_size, mmvar_cptr_t _var)
{
    mgec_t result = MGEC__OK;

    assert(_obj != NULL && MemeVariantStack_initByOther != NULL);

    if (MEGO_SYMBOL__UNLIKELY(_var == NULL))
    {
        MemeVariantStack_init(_obj, _object_size);
        return MGEC__INVAL;
    }

    switch (_var->type)
    {
    case MMMETA_TYPID__STRING:
    {
        return MemeVariantStack_initByString(
            _obj, _object_size, (mmstr_cptr_t)(&(_var->d.str)));
    };
    case MMMETA_TYPID__BUFFER:
    {
        return MemeVariantStack_initByBuffer(
            _obj, _object_size, (mmbuf_cptr_t)(&(_var->d.buf)));
    };
    case MMMETA_TYPID__VARBUF:
    {
        return MemeVariantStack_initByVariableBuffer(
            _obj, _object_size, (mmvb_cptr_t)(&(_var->d.vb)));
    };
    }

    if (_var->type > MMMETA_TYPID__USER)
        return MGEC__OPNOTSUPP;

    memcpy(_obj, _var, _object_size);
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByMove(
        mmvarstk_t* _obj, size_t _object_size, mmvarstk_t* _var)
{
    mgec_t result = MGEC__OK;

    assert(_obj != NULL && MemeVariantStack_initByMove != NULL);

    result = MemeVariantStack_init(_obj, _object_size);
    if (MEGO_SYMBOL__LIKELY(result != MGEC__OK))
        return result;

    if (MEGO_SYMBOL__UNLIKELY(_var == NULL))
        return MGEC__INVAL;
    return MemeVariantStack_swap(_obj, _var, _object_size);
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByByte(
        mmvarstk_t* _obj, size_t _object_size, mmbyte_t _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_initByByte != NULL);

    obj->type     = MMMETA_TYPID__BYTE;
    obj->d.b      = _val;
    obj->non_null = 1;
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByChar(
        mmvarstk_t* _obj, size_t _object_size, char _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_initByChar != NULL);

    obj->type     = MMMETA_TYPID__CHAR;
    obj->d.c      = _val;
    obj->non_null = 1;
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByWChar(
        mmvarstk_t* _obj, size_t _object_size, wchar_t _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_initByWChar != NULL);

    obj->type     = MMMETA_TYPID__WCHAR;
    obj->d.wc     = _val;
    obj->non_null = 1;
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByInt64(
        mmvarstk_t* _obj, size_t _object_size, int64_t _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_initByInt64 != NULL);

    obj->type     = MMMETA_TYPID__INT64;
    obj->d.i64    = _val;
    obj->non_null = 1;
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByUInt64(
        mmvarstk_t* _obj, size_t _object_size, uint64_t _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_initByUInt64 != NULL);

    obj->type     = MMMETA_TYPID__UINT64;
    obj->d.u64    = _val;
    obj->non_null = 1;
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByDouble(
        mmvarstk_t* _obj, size_t _object_size, double _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_initByDouble != NULL);

    obj->type     = MMMETA_TYPID__DOUBLE;
    obj->d.d      = _val;
    obj->non_null = 1;
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByString(
        mmvarstk_t* _obj, size_t _object_size, mmstr_cptr_t _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_initByString != NULL);
    assert(_val != NULL && MemeVariantStack_initByString != NULL);

    result = MemeStringStack_initByOther(&(obj->d.str), MMSTR__OBJ_SIZE, _val);
    if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
        return result;

    obj->type     = MMMETA_TYPID__STRING;
    obj->non_null = 1;
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByBuffer(
        mmvarstk_t* _obj, size_t _object_size, mmbuf_cptr_t _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_initByBuffer != NULL);
    assert(_val != NULL && MemeVariantStack_initByBuffer != NULL);

    result = MemeBufferStack_initByOther(
        &(obj->d.buf), MMSTR__OBJ_SIZE, (const mmbufstk_t*)_val);
    if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
        return result;

    obj->type     = MMMETA_TYPID__BUFFER;
    obj->non_null = 1;
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByVariableBuffer(
        mmvarstk_t* _obj, size_t _object_size, mmvb_cptr_t _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_initByVariableBuffer != NULL);
    assert(_val != NULL && MemeVariantStack_initByVariableBuffer != NULL);

    result = MemeVariableBufferStack_initByOther(
        &(obj->d.vb), MMSTR__OBJ_SIZE, (const mmvbstk_t*)_val);
    if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
        return result;

    obj->type     = MMMETA_TYPID__VARBUF;
    obj->non_null = 1;
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_initByRune(
        mmvarstk_t* _obj, size_t _object_size, const mmrune_t* _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_initByRune != NULL);
    assert(_val != NULL && MemeVariantStack_initByRune != NULL);

    obj->type     = MMMETA_TYPID__RUNE;
    obj->d.r      = *_val;
    obj->non_null = 1;
    return MGEC__OK;
}

MEME_API int MEME_STDCALL
MemeVariantStack_initAndConditionalConvert(
    mmvarstk_t* _out, size_t _object_size, mmvar_cptr_t _other)
{
    //mgec_t result = MGEC__OK;

    assert(_out != NULL && MemeVariantStack_initAndConditionalConvert != NULL);

    if (MEGO_SYMBOL__UNLIKELY(_other == NULL))
    {
        MemeVariantStack_init(_out, _object_size);
        return MGEC__INVAL;
    }

    switch (_other->type)
    {
    case MMMETA_TYPID__STRING:
    {
        mmvar_ptr_t var = (mmvar_ptr_t)_out;
        mgec_t   result = MemeStringStack_initAndConditionalConvert(
            &(var->d.str), _object_size, (mmstr_cptr_t)(&(_other->d.str)));
        if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
            return result;
        
        var->type     = MMMETA_TYPID__STRING;
        var->non_null = 1;
        return MGEC__OK;
    };
    case MMMETA_TYPID__BUFFER:
    {
        mmvar_ptr_t var = (mmvar_ptr_t)_out;
        mgec_t   result = MemeBufferStack_initAndConditionalConvert(
            &(var->d.buf), _object_size, (mmbuf_cptr_t)(&(_other->d.buf)));
        if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
            return result;
        
        var->type     = MMMETA_TYPID__BUFFER;
        var->non_null = 1;
        return MGEC__OK;
    };
    case MMMETA_TYPID__VARBUF:
    {
        return MemeVariantStack_initByVariableBuffer(
            _out, _object_size, (mmvb_cptr_t)(&(_other->d.vb)));
    };
    }

    if (_other->type > MMMETA_TYPID__USER)
        return MGEC__OPNOTSUPP;

    memcpy(_out, _other, _object_size);
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_unInit(
        mmvarstk_t* _obj, size_t _object_size)
{
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_unInit != NULL);

    switch (obj->type)
    {
    case MMMETA_TYPID__STRING:
    {
        return MemeStringStack_unInit(&(obj->d.str), MMSTR__OBJ_SIZE);
    };
    case MMMETA_TYPID__BUFFER:
    {
        return MemeBufferStack_unInit(&(obj->d.buf), MMSTR__OBJ_SIZE);
    };
    case MMMETA_TYPID__VARBUF:
    {
        return MemeVariableBufferStack_unInit(&(obj->d.vb), MMSTR__OBJ_SIZE);
    };
    }
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_assign(
        mmvarstk_t* _obj, size_t _object_size, mmvar_cptr_t _var)
{
    mgec_t result = MGEC__OK;

    assert(_obj != NULL && MemeVariantStack_assign != NULL);
    assert(_var != NULL && MemeVariantStack_assign != NULL);

    if (MEGO_SYMBOL__UNLIKELY(_var == NULL))
        return MGEC__INVAL;

    result = MemeVariantStack_unInit(_obj, _object_size);
    if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
        return result;

    switch (_var->type)
    {
    case MMMETA_TYPID__STRING:
    {
        return MemeVariantStack_initByString(_obj, _object_size, (mmstr_cptr_t)(&(_var->d.str)));
    };
    case MMMETA_TYPID__BUFFER:
    {
        return MemeVariantStack_initByBuffer(_obj, _object_size, (mmbuf_cptr_t)(&(_var->d.buf)));
    };
    case MMMETA_TYPID__VARBUF:
    {
        return MemeVariantStack_initByVariableBuffer(_obj, _object_size, (mmvb_cptr_t)(&(_var->d.vb)));
    };
    }

    memcpy(_obj, _var, _object_size);
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_swap(
        mmvarstk_t* _lhs, mmvarstk_t* _rhs, size_t _object_size)
{
    mgec_t result = MGEC__OK;
    mmvarstk_t temp;

    assert(_lhs != NULL && MemeVariantStack_swap != NULL);
    assert(_rhs != NULL && MemeVariantStack_swap != NULL);

    memcpy(&temp,  _lhs, _object_size);
    memcpy( _lhs,  _rhs, _object_size);
    memcpy( _rhs, &temp, _object_size);
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_reset(
        mmvarstk_t* _obj, size_t _object_size)
{
    mgec_t result = MGEC__OK;
    assert(_obj != NULL && MemeVariantStack_reset != NULL);

    result = MemeVariantStack_unInit(_obj, _object_size);
    if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
        return result;
    return MemeVariantStack_init(_obj, _object_size);
}

MEME_API MemeMetaTypeId_t MEME_STDCALL
    MemeVariantStack_typeId(
        const mmvarstk_t* _obj, size_t _object_size)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_typeId != NULL);

    return (MemeMetaTypeId_t)obj->type;
}        

MEME_API int MEME_STDCALL
    MemeVariantStack_isNull(
        const mmvarstk_t* _obj, size_t _object_size)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_isNull != NULL);

    return (obj->non_null == 0) ? 1 : 0;
}        

MEME_API int MEME_STDCALL
    MemeVariantStack_isByte(
        const mmvarstk_t* _obj, size_t _object_size)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_isByte != NULL);

    return (obj->type == MMMETA_TYPID__BYTE) ? 1 : 0;
}

MEME_API int MEME_STDCALL
    MemeVariantStack_isChar(
        const mmvarstk_t* _obj, size_t _object_size)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_isChar != NULL);

    return (obj->type == MMMETA_TYPID__CHAR) ? 1 : 0;
}

MEME_API int MEME_STDCALL
    MemeVariantStack_isWChar(
        const mmvarstk_t* _obj, size_t _object_size)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_isWChar != NULL);

    return (obj->type == MMMETA_TYPID__WCHAR) ? 1 : 0;
}

MEME_API int MEME_STDCALL
    MemeVariantStack_isInt64(
        const mmvarstk_t* _obj, size_t _object_size)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_isInt64 != NULL);

    return (obj->type == MMMETA_TYPID__INT64) ? 1 : 0;
}

MEME_API int MEME_STDCALL
    MemeVariantStack_isUInt64(
        const mmvarstk_t* _obj, size_t _object_size)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_isUInt64 != NULL);

    return (obj->type == MMMETA_TYPID__UINT64) ? 1 : 0;
}

MEME_API int MEME_STDCALL
    MemeVariantStack_isDouble(
        const mmvarstk_t* _obj, size_t _object_size)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_isDouble != NULL);

    return (obj->type == MMMETA_TYPID__DOUBLE) ? 1 : 0;
}

MEME_API int MEME_STDCALL
    MemeVariantStack_isString(
        const mmvarstk_t* _obj, size_t _object_size)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_isString != NULL);

    return (obj->type == MMMETA_TYPID__STRING) ? 1 : 0;
}

MEME_API int MEME_STDCALL
    MemeVariantStack_isBuffer(
        const mmvarstk_t* _obj, size_t _object_size)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_isBuffer != NULL);

    return (obj->type == MMMETA_TYPID__BUFFER) ? 1 : 0;
}

MEME_API int MEME_STDCALL
    MemeVariantStack_isVariableBuffer(
        const mmvarstk_t* _obj, size_t _object_size)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_isVariableBuffer != NULL);

    return (obj->type == MMMETA_TYPID__VARBUF) ? 1 : 0;
}

MEME_API int MEME_STDCALL
    MemeVariantStack_isRune(
        const mmvarstk_t* _obj, size_t _object_size)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_isRune != NULL);

    return (obj->type == MMMETA_TYPID__RUNE) ? 1 : 0;
}

MEME_API int MEME_STDCALL
    MemeVariantStack_isUser(
        const mmvarstk_t* _obj, size_t _object_size)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_isUser != NULL);

    return (obj->type > MMMETA_TYPID__USER) ? 1 : 0;
}

MEME_API int MEME_STDCALL
    MemeVariantStack_isType(
        const mmvarstk_t* _obj, size_t _object_size, MemeMetaTypeId_t _type)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_isType != NULL);

    if (_type == MMMETA_TYPID__USER)
        return 0;
    return (obj->type == _type) ? 1 : 0;
}


MEME_API mgec_t MEME_STDCALL
MemeVariantStack_convToInt(
    const mmvarstk_t* _obj, size_t _object_size, mmint_t* _out)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;
    assert(_obj != NULL && MemeVariantStack_convToInt != NULL);

    if (!_out)
        return MGEC__INVAL;

    switch (obj->type) {
    case MMMETA_TYPID__NULL: {
        return MGEC__OPNOTSUPP;
    }
    case MMMETA_TYPID__BYTE:
    case MMMETA_TYPID__CHAR:
    case MMMETA_TYPID__WCHAR:
    case MMMETA_TYPID__INT64:
    case MMMETA_TYPID__UINT64:
    case MMMETA_TYPID__DOUBLE: {
        *_out = MemeVariantImpl_basicNumberToInt(obj);
        return MGEC__OK;
    }
    case MMMETA_TYPID__STRING: {
        // The string class is not a view mode

        const char* ptr = MemeString_cStr((mmstr_cptr_t) & (obj->d.str));
        char* end = NULL;
        *_out = strtoll(ptr, &end, 0);
        if (ptr == end) {
            return MGEC__INVAL;
        }
        if (errno == ERANGE) {
            return MGEC__RANGE;
        }
        return MGEC__OK;
    }
    //case MMMETA_TYPID__RUNE: {

    //}
    default:
        return MGEC__OPNOTSUPP;
    }
        
}

MEME_API mgec_t MEME_STDCALL
MemeVariantStack_convToUInt(
    const mmvarstk_t* _obj, size_t _object_size, size_t * _out)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;
    assert(_obj != NULL && MemeVariantStack_convToUInt != NULL);

    if (!_out)
        return MGEC__INVAL;

    switch (obj->type) {
    case MMMETA_TYPID__NULL: {
        return MGEC__OPNOTSUPP;
    }
    case MMMETA_TYPID__BYTE:
    case MMMETA_TYPID__CHAR:
    case MMMETA_TYPID__WCHAR:
    case MMMETA_TYPID__INT64:
    case MMMETA_TYPID__UINT64:
    case MMMETA_TYPID__DOUBLE: {
        *_out = MemeVariantImpl_basicNumberToUInt(obj);
        return MGEC__OK;
    }
    case MMMETA_TYPID__STRING: {
        // The string class is not a view mode

        const char* ptr = MemeString_cStr((mmstr_cptr_t) & (obj->d.str));
        char* end = NULL;
        *_out = strtoull(ptr, &end, 0);
        if (ptr == end) {
            return MGEC__INVAL;
        }
        if (errno == ERANGE) {
            return MGEC__RANGE;
        }
        return MGEC__OK;
    }
    default:
        return MGEC__OPNOTSUPP;
    }
}

MEME_API mgec_t MEME_STDCALL MemeVariantStack_convToDouble(const mmvarstk_t* _obj, size_t _object_size, double* _out)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_convToDouble != NULL);

    if (!_out)
        return MGEC__INVAL;

    switch (obj->type) {
    case MMMETA_TYPID__NULL: {
        *_out = nan("");
        return MGEC__OPNOTSUPP; 
    }
    case MMMETA_TYPID__BYTE:
    case MMMETA_TYPID__CHAR:
    case MMMETA_TYPID__WCHAR:
    case MMMETA_TYPID__INT64:
    case MMMETA_TYPID__UINT64:
    case MMMETA_TYPID__DOUBLE: {
        *_out = MemeVariantImpl_basicNumberToDouble(obj);
        return MGEC__OK; 
    }
    case MMMETA_TYPID__STRING: {
        // The string class is not a view mode
        
        const char* ptr = MemeString_cStr((mmstr_cptr_t)&(obj->d.str));
        char* end = NULL;
        *_out = strtod(ptr, &end);
        if (ptr == end) {
            *_out = nan("");
            return MGEC__INVAL;
        }
        if (errno == ERANGE) {
            return MGEC__RANGE;
        }
        return MGEC__OK;
    }
    //case MMMETA_TYPID__RUNE: {

    //}
    default:
        *_out = nan("");
        return MGEC__OPNOTSUPP;
    }
}

//MEME_API int MEME_STDCALL
//    MemeVariantStack_isEqual(
//        const mmvarstk_t* _lhs, const mmvarstk_t* _rhs, size_t _object_size)
//{
//    mmvar_cptr_t lhs = (mmvar_cptr_t)_lhs;
//    mmvar_cptr_t rhs = (mmvar_cptr_t)_rhs;
//
//    assert(_lhs != NULL && _rhs != NULL && MemeVariantStack_isEqual != NULL);
//    
//    //switch (lhs->type) {
//    //case MMMETA_TYPID__NULL:
//    //    return (rhs->type == MMMETA_TYPID__NULL) ? 1 : 0;
//    //case MMMETA_TYPID__CHAR:
//    //case MMMETA_TYPID__WCHAR:
//    //{
//    //    MemeVariantImpl_basicNumberToInt(lhs);
//    //    switch (rhs->type) {
//    //    case MMMETA_TYPID__CHAR:
//    //    }
//    //}
//    //case MMMETA_TYPID__INT64:
//    //{
//    //}
//    //case MMMETA_TYPID__BYTE:
//    //{
//    //    MemeVariantImpl_basicNumberToUInt(lhs);
//    //}
//    //case MMMETA_TYPID__UINT64:
//    //{}
//    //}
//
//    return 0;
//}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setNull(
        mmvarstk_t* _obj, size_t _object_size)
{
    assert(_obj != NULL && MemeVariantStack_setNull != NULL);
    return MemeVariantStack_reset(_obj, _object_size);
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setByte(
        mmvarstk_t* _obj, size_t _object_size, mmbyte_t _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_setByte != NULL);

    result = MemeVariantStack_unInit(_obj, _object_size);
    if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
        return result;

    return MemeVariantStack_initByByte(_obj, _object_size, _val);
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setChar(
        mmvarstk_t* _obj, size_t _object_size, char _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_setChar != NULL);

    result = MemeVariantStack_unInit(_obj, _object_size);
    if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
        return result;

    return MemeVariantStack_initByChar(_obj, _object_size, _val);
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setWChar(
        mmvarstk_t* _obj, size_t _object_size, wchar_t _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_setWChar != NULL);

    result = MemeVariantStack_unInit(_obj, _object_size);
    if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
        return result;

    return MemeVariantStack_initByWChar(_obj, _object_size, _val);
}

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
MemeVariantStack_setInt(
    mmvarstk_t* _obj, size_t _object_size, mmint_t _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_setInt != NULL);

    result = MemeVariantStack_unInit(_obj, _object_size);
    if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
        return result;

    return MemeVariantStack_initByInt64(_obj, _object_size, _val);
}

//! @param _obj must be initialized
MEME_API mgec_t MEME_STDCALL
MemeVariantStack_setUInt(
    mmvarstk_t* _obj, size_t _object_size, size_t _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_setUInt != NULL);

    result = MemeVariantStack_unInit(_obj, _object_size);
    if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
        return result;

    return MemeVariantStack_initByUInt64(_obj, _object_size, _val);
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setInt64(
        mmvarstk_t* _obj, size_t _object_size, int64_t _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_setInt64 != NULL);

    result = MemeVariantStack_unInit(_obj, _object_size);
    if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
        return result;

    return MemeVariantStack_initByInt64(_obj, _object_size, _val);
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setUInt64(
        mmvarstk_t* _obj, size_t _object_size, uint64_t _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_setUInt64 != NULL);

    result = MemeVariantStack_unInit(_obj, _object_size);
    if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
        return result;

    return MemeVariantStack_initByUInt64(_obj, _object_size, _val);
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setDouble(
        mmvarstk_t* _obj, size_t _object_size, double _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_setDouble != NULL);

    result = MemeVariantStack_unInit(_obj, _object_size);
    if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
        return result;

    return MemeVariantStack_initByDouble(_obj, _object_size, _val);
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setString(
        mmvarstk_t* _obj, size_t _object_size, mmstr_cptr_t _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_setString != NULL);
    assert(_val != NULL && MemeVariantStack_setString != NULL);

    result = MemeVariantStack_unInit(_obj, _object_size);
    if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
        return result;

    return MemeVariantStack_initByString(_obj, _object_size, _val);
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setBuffer(
        mmvarstk_t* _obj, size_t _object_size, mmbuf_cptr_t _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_setBuffer != NULL);
    assert(_val != NULL && MemeVariantStack_setBuffer != NULL);

    result = MemeVariantStack_unInit(_obj, _object_size);
    if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
        return result;

    return MemeVariantStack_initByBuffer(_obj, _object_size, _val);
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setVariableBuffer(
        mmvarstk_t* _obj, size_t _object_size, mmvb_cptr_t _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_setVariableBuffer != NULL);
    assert(_val != NULL && MemeVariantStack_setVariableBuffer != NULL);

    result = MemeVariantStack_unInit(_obj, _object_size);
    if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
        return result;

    return MemeVariantStack_initByVariableBuffer(_obj, _object_size, _val);
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_setRune(
        mmvarstk_t* _obj, size_t _object_size, const mmrune_t* _val)
{
    mgec_t result = MGEC__OK;
    mmvar_ptr_t obj = (mmvar_ptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_setRune != NULL);
    assert(_val != NULL && MemeVariantStack_setRune != NULL);

    result = MemeVariantStack_unInit(_obj, _object_size);
    if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
        return result;

    return MemeVariantStack_initByRune(_obj, _object_size, _val);
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getByte(
        const mmvarstk_t* _obj, size_t _object_size, mmbyte_t* _out)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_getByte != NULL);
    assert(_out != NULL && MemeVariantStack_getByte != NULL);

    if (MEGO_SYMBOL__UNLIKELY(obj->type != MMMETA_TYPID__BYTE))
        return MGEC__INVAL;

    *_out = obj->d.b;
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getChar(
        const mmvarstk_t* _obj, size_t _object_size, char* _out)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_getChar != NULL);
    assert(_out != NULL && MemeVariantStack_getChar != NULL);

    if (MEGO_SYMBOL__UNLIKELY(obj->type != MMMETA_TYPID__CHAR))
        return MGEC__INVAL;

    *_out = obj->d.c;
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getWChar(
        const mmvarstk_t* _obj, size_t _object_size, wchar_t* _out)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_getWChar != NULL);
    assert(_out != NULL && MemeVariantStack_getWChar != NULL);

    if (MEGO_SYMBOL__UNLIKELY(obj->type != MMMETA_TYPID__WCHAR))
        return MGEC__INVAL;

    *_out = obj->d.wc;
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getInt64(
        const mmvarstk_t* _obj, size_t _object_size, int64_t* _out)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_getInt64 != NULL);
    assert(_out != NULL && MemeVariantStack_getInt64 != NULL);

    if (MEGO_SYMBOL__UNLIKELY(obj->type != MMMETA_TYPID__INT64))
        return MGEC__INVAL;

    *_out = obj->d.i64;
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getUInt64(
        const mmvarstk_t* _obj, size_t _object_size, uint64_t* _out)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_getUInt64 != NULL);
    assert(_out != NULL && MemeVariantStack_getUInt64 != NULL);

    if (MEGO_SYMBOL__UNLIKELY(obj->type != MMMETA_TYPID__UINT64))
        return MGEC__INVAL;

    *_out = obj->d.u64;
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getDouble(
        const mmvarstk_t* _obj, size_t _object_size, double* _out)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_getDouble != NULL);
    assert(_out != NULL && MemeVariantStack_getDouble != NULL);

    if (MEGO_SYMBOL__UNLIKELY(obj->type != MMMETA_TYPID__DOUBLE))
        return MGEC__INVAL;

    *_out = obj->d.d;
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getString(
        const mmvarstk_t* _obj, size_t _object_size, mmstrstk_t* _out, int _is_str_inited)
{
    mgec_t result = MGEC__OK;
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_getString != NULL);
    assert(_out != NULL && MemeVariantStack_getString != NULL);

    if (MEGO_SYMBOL__UNLIKELY(obj->type != MMMETA_TYPID__STRING))
        return MGEC__INVAL;

    if (_is_str_inited)
        result = MemeStringStack_assign(_out, MMSTR__OBJ_SIZE, (mmstr_cptr_t)&(obj->d.str));
    else
        result = MemeStringStack_initByOther(_out, MMSTR__OBJ_SIZE, (mmstr_cptr_t)&(obj->d.str));
    if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
        return result;

    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getBuffer(
        const mmvarstk_t* _obj, size_t _object_size, mmbufstk_t* _out, int _is_buf_inited)
{
    mgec_t result = MGEC__OK;
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_getBuffer != NULL);
    assert(_out != NULL && MemeVariantStack_getBuffer != NULL);

    if (MEGO_SYMBOL__UNLIKELY(obj->type != MMMETA_TYPID__BUFFER))
        return MGEC__INVAL;

    if (_is_buf_inited)
        result = MemeBufferStack_assign(_out, MMSTR__OBJ_SIZE, &(obj->d.buf));
    else
        result = MemeBufferStack_initByOther(_out, MMSTR__OBJ_SIZE, &(obj->d.buf));
    if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
        return result;

    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getVariableBuffer(
        const mmvarstk_t* _obj, size_t _object_size, mmvbstk_t* _out, int _is_vbuf_inited)
{
    mgec_t result = MGEC__OK;
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_getVariableBuffer != NULL);
    assert(_out != NULL && MemeVariantStack_getVariableBuffer != NULL);

    if (MEGO_SYMBOL__UNLIKELY(obj->type != MMMETA_TYPID__VARBUF))
        return MGEC__INVAL;

    if (_is_vbuf_inited)
        result = MemeVariableBufferStack_assign(_out, MMSTR__OBJ_SIZE, &(obj->d.vb));
    else
        result = MemeVariableBufferStack_initByOther(_out, MMSTR__OBJ_SIZE, &(obj->d.vb));
    if (MEGO_SYMBOL__UNLIKELY(result != MGEC__OK))
        return result;

    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getRune(
        const mmvarstk_t* _obj, size_t _object_size, mmrune_t* _out)
{
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_getRune != NULL);
    assert(_out != NULL && MemeVariantStack_getRune != NULL);

    if (MEGO_SYMBOL__UNLIKELY(obj->type != MMMETA_TYPID__RUNE))
        return MGEC__INVAL;

    *_out = obj->d.r;
    return MGEC__OK;
}

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getStringView(
        const mmvarstk_t* _obj, size_t _object_size, mmstrstk_t* _out, int _is_inited)
{
    mgec_t result = MGEC__OK;
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_getStringView != NULL);
    assert(_out != NULL && MemeVariantStack_getStringView != NULL);

    if (MEGO_SYMBOL__UNLIKELY(obj->type != MMMETA_TYPID__STRING))
        return MGEC__INVAL;

    if (_is_inited)
        result = MemeStringViewUnsafeStack_assignByOther(
            _out, MMSTR__OBJ_SIZE, &(obj->d.str));
    else
        result = MemeStringViewUnsafeStack_initByOther(
            _out, MMSTR__OBJ_SIZE, &(obj->d.str));

    return result;
}        

MEME_API mgec_t MEME_STDCALL
    MemeVariantStack_getBufferView(
        const mmvarstk_t* _obj, size_t _object_size, mmbufstk_t* _out, int _is_inited)
{
    mgec_t result = MGEC__OK;
    mmvar_cptr_t obj = (mmvar_cptr_t)_obj;

    assert(_obj != NULL && MemeVariantStack_getBufferView != NULL);
    assert(_out != NULL && MemeVariantStack_getBufferView != NULL);

    if (_is_inited) {
        switch (obj->type)
        {
        case MMMETA_TYPID__STRING:
            result = MemeBufferViewUnsafeStack_assignByString(
                _out, MMSTR__OBJ_SIZE, &(obj->d.str));
            break;
        case MMMETA_TYPID__BUFFER:
            result = MemeBufferViewUnsafeStack_assignByOther(
                _out, MMSTR__OBJ_SIZE, &(obj->d.buf));
            break;
        case MMMETA_TYPID__VARBUF:
            result = MemeBufferViewUnsafeStack_assignByVariableBuffer(
                _out, MMSTR__OBJ_SIZE, &(obj->d.vb));
            break;
        default:
            return MGEC__INVAL;
        }
    }
    else {
        switch (obj->type)
        {
        case MMMETA_TYPID__STRING:
            result = MemeBufferViewUnsafeStack_initByString(
                _out, MMSTR__OBJ_SIZE, &(obj->d.str));
            break;
        case MMMETA_TYPID__BUFFER:
            result = MemeBufferViewUnsafeStack_initByOther(
                _out, MMSTR__OBJ_SIZE, &(obj->d.buf));
            break;
        case MMMETA_TYPID__VARBUF:
            result = MemeBufferViewUnsafeStack_initByVariableBuffer(
                _out, MMSTR__OBJ_SIZE, &(obj->d.vb));
            break;
        default:
            return MGEC__INVAL;
        }
    }
    
    return result;
}

MEME_EXTERN_C_SCOPE_ENDED
