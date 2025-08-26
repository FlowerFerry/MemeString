
#ifndef MEME_CONC_ATOMIC_H_INCLUDED
#define MEME_CONC_ATOMIC_H_INCLUDED

#include <mego/predef/symbol/inline.h>
#include <meme/conc/atomic_fwd.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

MEME_EXTERN_C_SCOPE_START

MEME_API void MEME_STDCALL mmconc_atomic_bl_base_init (volatile mmconc_atomic_bool_t*   _obj, int _obj_size, int      _desired);
MEME_API void MEME_STDCALL mmconc_atomic_i8_base_init (volatile mmconc_atomic_int8_t*   _obj, int _obj_size, int8_t   _desired);
MEME_API void MEME_STDCALL mmconc_atomic_u8_base_init (volatile mmconc_atomic_uint8_t*  _obj, int _obj_size, uint8_t  _desired);
MEME_API void MEME_STDCALL mmconc_atomic_i16_base_init(volatile mmconc_atomic_int16_t*  _obj, int _obj_size, int16_t  _desired);
MEME_API void MEME_STDCALL mmconc_atomic_u16_base_init(volatile mmconc_atomic_uint16_t* _obj, int _obj_size, uint16_t _desired);
MEME_API void MEME_STDCALL mmconc_atomic_i32_base_init(volatile mmconc_atomic_int32_t*  _obj, int _obj_size, int32_t  _desired);
MEME_API void MEME_STDCALL mmconc_atomic_u32_base_init(volatile mmconc_atomic_uint32_t* _obj, int _obj_size, uint32_t _desired);
MEME_API void MEME_STDCALL mmconc_atomic_i64_base_init(volatile mmconc_atomic_int64_t*  _obj, int _obj_size, int64_t  _desired);
MEME_API void MEME_STDCALL mmconc_atomic_u64_base_init(volatile mmconc_atomic_uint64_t* _obj, int _obj_size, uint64_t _desired);

MEME_API void MEME_STDCALL mmconc_atomic_flag_base_init(volatile mmconc_atomic_flag_t* _obj, int _obj_size);

MG_CAPI_INLINE void mmconc_atomic_bl_init(volatile mmconc_atomic_bool_t* _obj, int _obj_size, int _desired)
{
    assert(_obj != NULL && "mmconc_atomic_bl_init: _obj is NULL");
    mmconc_atomic_bl_base_init(_obj, _obj_size, _desired);
}

MG_CAPI_INLINE void mmconc_atomic_i8_init(volatile mmconc_atomic_int8_t* _obj, int _obj_size, int8_t _desired)
{
    assert(_obj != NULL && "mmconc_atomic_i8_init: _obj is NULL");
    mmconc_atomic_i8_base_init(_obj, _obj_size, _desired);
}

MG_CAPI_INLINE void mmconc_atomic_u8_init(volatile mmconc_atomic_uint8_t* _obj, int _obj_size, uint8_t _desired)
{
    assert(_obj != NULL && "mmconc_atomic_u8_init: _obj is NULL");
    mmconc_atomic_u8_base_init(_obj, _obj_size, _desired);
}

MG_CAPI_INLINE void mmconc_atomic_i16_init(volatile mmconc_atomic_int16_t* _obj, int _obj_size, int16_t _desired)
{
    assert(_obj != NULL && "mmconc_atomic_i16_init: _obj is NULL");
    mmconc_atomic_i16_base_init(_obj, _obj_size, _desired);
}

MG_CAPI_INLINE void mmconc_atomic_u16_init(volatile mmconc_atomic_uint16_t* _obj, int _obj_size, uint16_t _desired)
{
    assert(_obj != NULL && "mmconc_atomic_u16_init: _obj is NULL");
    mmconc_atomic_u16_base_init(_obj, _obj_size, _desired);
}

MG_CAPI_INLINE void mmconc_atomic_i32_init(volatile mmconc_atomic_int32_t* _obj, int _obj_size, int32_t _desired)
{
    assert(_obj != NULL && "mmconc_atomic_i32_init: _obj is NULL");
    mmconc_atomic_i32_base_init(_obj, _obj_size, _desired);
}

MG_CAPI_INLINE void mmconc_atomic_u32_init(volatile mmconc_atomic_uint32_t* _obj, int _obj_size, uint32_t _desired)
{
    assert(_obj != NULL && "mmconc_atomic_u32_init: _obj is NULL");
    mmconc_atomic_u32_base_init(_obj, _obj_size, _desired);
}

MG_CAPI_INLINE void mmconc_atomic_i64_init(volatile mmconc_atomic_int64_t* _obj, int _obj_size, int64_t _desired)
{
    assert(_obj != NULL && "mmconc_atomic_i64_init: _obj is NULL");
    mmconc_atomic_i64_base_init(_obj, _obj_size, _desired);
}

MG_CAPI_INLINE void mmconc_atomic_u64_init(volatile mmconc_atomic_uint64_t* _obj, int _obj_size, uint64_t _desired)
{
    assert(_obj != NULL && "mmconc_atomic_u64_init: _obj is NULL");
    mmconc_atomic_u64_base_init(_obj, _obj_size, _desired);
}

MG_CAPI_INLINE void mmconc_atomic_flag_init(volatile mmconc_atomic_flag_t* _obj, int _obj_size)
{
    assert(_obj != NULL && "mmconc_atomic_flag_init: _obj is NULL");
    mmconc_atomic_flag_base_init(_obj, _obj_size);
}

MG_CAPI_INLINE mmconc_atomic_bool_t mmconc_atomic_bl_get_init(int _desired)
{
    mmconc_atomic_bool_t obj;
    obj.base.st_size = sizeof(obj);
    mmconc_atomic_bl_init(&obj, -1, _desired);
    return obj;
}

MG_CAPI_INLINE mmconc_atomic_int8_t mmconc_atomic_i8_get_init(int8_t _desired)
{
    mmconc_atomic_int8_t obj;
    obj.base.st_size = sizeof(obj);
    mmconc_atomic_i8_init(&obj, -1, _desired);
    return obj;
}

MG_CAPI_INLINE mmconc_atomic_uint8_t mmconc_atomic_u8_get_init(uint8_t _desired)
{
    mmconc_atomic_uint8_t obj;
    obj.base.st_size = sizeof(obj);
    mmconc_atomic_u8_init(&obj, -1, _desired);
    return obj;
}

MG_CAPI_INLINE mmconc_atomic_int16_t mmconc_atomic_i16_get_init(int16_t _desired)
{
    mmconc_atomic_int16_t obj;
    obj.base.st_size = sizeof(obj);
    mmconc_atomic_i16_init(&obj, -1, _desired);
    return obj;
}

MG_CAPI_INLINE mmconc_atomic_uint16_t mmconc_atomic_u16_get_init(uint16_t _desired)
{
    mmconc_atomic_uint16_t obj;
    obj.base.st_size = sizeof(obj);
    mmconc_atomic_u16_init(&obj, -1, _desired);
    return obj;
}

MG_CAPI_INLINE mmconc_atomic_int32_t mmconc_atomic_i32_get_init(int32_t _desired)
{
    mmconc_atomic_int32_t obj;
    obj.base.st_size = sizeof(obj);
    mmconc_atomic_i32_init(&obj, -1, _desired);
    return obj;
}

MG_CAPI_INLINE mmconc_atomic_uint32_t mmconc_atomic_u32_get_init(uint32_t _desired)
{
    mmconc_atomic_uint32_t obj;
    obj.base.st_size = sizeof(obj);
    mmconc_atomic_u32_init(&obj, -1, _desired);
    return obj;
}

MG_CAPI_INLINE mmconc_atomic_int64_t mmconc_atomic_i64_get_init(int64_t _desired)
{
    mmconc_atomic_int64_t obj;
    obj.base.st_size = sizeof(obj);
    mmconc_atomic_i64_init(&obj, -1, _desired);
    return obj;
}

MG_CAPI_INLINE mmconc_atomic_uint64_t mmconc_atomic_u64_get_init(uint64_t _desired)
{
    mmconc_atomic_uint64_t obj;
    obj.base.st_size = sizeof(obj);
    mmconc_atomic_u64_init(&obj, -1, _desired);
    return obj;
}

MG_CAPI_INLINE mmconc_atomic_flag_t mmconc_atomic_flag_get_init()
{
    mmconc_atomic_flag_t obj;
    obj.base.st_size = sizeof(obj);
    mmconc_atomic_flag_init(&obj, -1);
    return obj;
}

MEME_API int MEME_STDCALL mmconc_atomic_bl_base_is_lock_free (const volatile mmconc_atomic_bool_t  * _obj);
MEME_API int MEME_STDCALL mmconc_atomic_i8_base_is_lock_free (const volatile mmconc_atomic_int8_t  * _obj);
MEME_API int MEME_STDCALL mmconc_atomic_u8_base_is_lock_free (const volatile mmconc_atomic_uint8_t * _obj);
MEME_API int MEME_STDCALL mmconc_atomic_i16_base_is_lock_free(const volatile mmconc_atomic_int16_t * _obj);
MEME_API int MEME_STDCALL mmconc_atomic_u16_base_is_lock_free(const volatile mmconc_atomic_uint16_t* _obj);
MEME_API int MEME_STDCALL mmconc_atomic_i32_base_is_lock_free(const volatile mmconc_atomic_int32_t * _obj);
MEME_API int MEME_STDCALL mmconc_atomic_u32_base_is_lock_free(const volatile mmconc_atomic_uint32_t* _obj);
MEME_API int MEME_STDCALL mmconc_atomic_i64_base_is_lock_free(const volatile mmconc_atomic_int64_t * _obj);
MEME_API int MEME_STDCALL mmconc_atomic_u64_base_is_lock_free(const volatile mmconc_atomic_uint64_t* _obj);

MG_CAPI_INLINE int mmconc_atomic_bl_is_lock_free (const volatile mmconc_atomic_bool_t  * _obj)
{
    assert(_obj != NULL && "mmconc_atomic_bl_is_lock_free: _obj is NULL");
    return mmconc_atomic_bl_base_is_lock_free(_obj);
}

MG_CAPI_INLINE int mmconc_atomic_i8_is_lock_free (const volatile mmconc_atomic_int8_t  * _obj)
{
    assert(_obj != NULL && "mmconc_atomic_i8_is_lock_free: _obj is NULL");
    return mmconc_atomic_i8_base_is_lock_free(_obj);
}

MG_CAPI_INLINE int mmconc_atomic_u8_is_lock_free (const volatile mmconc_atomic_uint8_t * _obj)
{
    assert(_obj != NULL && "mmconc_atomic_u8_is_lock_free: _obj is NULL");
    return mmconc_atomic_u8_base_is_lock_free(_obj);
}

MG_CAPI_INLINE int mmconc_atomic_i16_is_lock_free(const volatile mmconc_atomic_int16_t * _obj)
{
    assert(_obj != NULL && "mmconc_atomic_i16_is_lock_free: _obj is NULL");
    return mmconc_atomic_i16_base_is_lock_free(_obj);
}

MG_CAPI_INLINE int mmconc_atomic_u16_is_lock_free(const volatile mmconc_atomic_uint16_t* _obj)
{
    assert(_obj != NULL && "mmconc_atomic_u16_is_lock_free: _obj is NULL");
    return mmconc_atomic_u16_base_is_lock_free(_obj);
}

MG_CAPI_INLINE int mmconc_atomic_i32_is_lock_free(const volatile mmconc_atomic_int32_t * _obj)
{
    assert(_obj != NULL && "mmconc_atomic_i32_is_lock_free: _obj is NULL");
    return mmconc_atomic_i32_base_is_lock_free(_obj);
}

MG_CAPI_INLINE int mmconc_atomic_u32_is_lock_free(const volatile mmconc_atomic_uint32_t* _obj)
{
    assert(_obj != NULL && "mmconc_atomic_u32_is_lock_free: _obj is NULL");
    return mmconc_atomic_u32_base_is_lock_free(_obj);
}

MG_CAPI_INLINE int mmconc_atomic_i64_is_lock_free(const volatile mmconc_atomic_int64_t * _obj)
{
    assert(_obj != NULL && "mmconc_atomic_i64_is_lock_free: _obj is NULL");
    return mmconc_atomic_i64_base_is_lock_free(_obj);
}

MG_CAPI_INLINE int mmconc_atomic_u64_is_lock_free(const volatile mmconc_atomic_uint64_t* _obj)
{
    assert(_obj != NULL && "mmconc_atomic_u64_is_lock_free: _obj is NULL");
    return mmconc_atomic_u64_base_is_lock_free(_obj);
}

MG_CAPI_INLINE bool mmconc_atomic_impl_is_lock_free(volatile mmconc_atomic_none_t* _obj)
{
    assert(_obj != NULL && "mmconc_atomic_impl_is_lock_free: _obj is NULL");

    switch (_obj->type) {
    case mmconc_atomic_bool_type:  return (bool)mmconc_atomic_bl_is_lock_free ((volatile mmconc_atomic_bool_t*)_obj);
    case mmconc_atomic_int8_type:  return (bool)mmconc_atomic_i8_is_lock_free ((volatile mmconc_atomic_int8_t*)_obj);
    case mmconc_atomic_uint8_type: return (bool)mmconc_atomic_u8_is_lock_free ((volatile mmconc_atomic_uint8_t*)_obj);
    case mmconc_atomic_int16_type: return (bool)mmconc_atomic_i16_is_lock_free((volatile mmconc_atomic_int16_t*)_obj);
    case mmconc_atomic_uint16_type:return (bool)mmconc_atomic_u16_is_lock_free((volatile mmconc_atomic_uint16_t*)_obj);
    case mmconc_atomic_int32_type: return (bool)mmconc_atomic_i32_is_lock_free((volatile mmconc_atomic_int32_t*)_obj);
    case mmconc_atomic_uint32_type:return (bool)mmconc_atomic_u32_is_lock_free((volatile mmconc_atomic_uint32_t*)_obj);
    case mmconc_atomic_int64_type: return (bool)mmconc_atomic_i64_is_lock_free((volatile mmconc_atomic_int64_t*)_obj);
    case mmconc_atomic_uint64_type:return (bool)mmconc_atomic_u64_is_lock_free((volatile mmconc_atomic_uint64_t*)_obj);
    default: return false;
    }
}

#define mmconc_atomic_is_lock_free(ptr) \
    (ptr != NULL && ((const mmconc_atomic_none_t*)ptr)->st_size > sizeof(mmconc_atomic_none_t) ? mmconc_atomic_impl_is_lock_free(ptr) : false)

MEME_API void MEME_STDCALL mmconc_atomic_bl_base_store_explicit (volatile mmconc_atomic_bool_t*   _obj, int      _desired, mmconc_memory_order_e _order);
MEME_API void MEME_STDCALL mmconc_atomic_i8_base_store_explicit (volatile mmconc_atomic_int8_t*   _obj, int8_t   _desired, mmconc_memory_order_e _order);
MEME_API void MEME_STDCALL mmconc_atomic_u8_base_store_explicit (volatile mmconc_atomic_uint8_t*  _obj, uint8_t  _desired, mmconc_memory_order_e _order);
MEME_API void MEME_STDCALL mmconc_atomic_i16_base_store_explicit(volatile mmconc_atomic_int16_t*  _obj, int16_t  _desired, mmconc_memory_order_e _order);
MEME_API void MEME_STDCALL mmconc_atomic_u16_base_store_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _desired, mmconc_memory_order_e _order);
MEME_API void MEME_STDCALL mmconc_atomic_i32_base_store_explicit(volatile mmconc_atomic_int32_t*  _obj, int32_t  _desired, mmconc_memory_order_e _order);
MEME_API void MEME_STDCALL mmconc_atomic_u32_base_store_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _desired, mmconc_memory_order_e _order);
MEME_API void MEME_STDCALL mmconc_atomic_i64_base_store_explicit(volatile mmconc_atomic_int64_t*  _obj, int64_t  _desired, mmconc_memory_order_e _order);
MEME_API void MEME_STDCALL mmconc_atomic_u64_base_store_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _desired, mmconc_memory_order_e _order);

MG_CAPI_INLINE void mmconc_atomic_bl_store_explicit(volatile mmconc_atomic_bool_t* _obj, int _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_bl_store_explicit: _obj is NULL");
    mmconc_atomic_bl_base_store_explicit(_obj, _desired, _order);
}

MG_CAPI_INLINE void mmconc_atomic_i8_store_explicit(volatile mmconc_atomic_int8_t* _obj, int8_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i8_store_explicit: _obj is NULL");
    mmconc_atomic_i8_base_store_explicit(_obj, _desired, _order);
}

MG_CAPI_INLINE void mmconc_atomic_u8_store_explicit(volatile mmconc_atomic_uint8_t* _obj, uint8_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u8_store_explicit: _obj is NULL");
    mmconc_atomic_u8_base_store_explicit(_obj, _desired, _order);
}

MG_CAPI_INLINE void mmconc_atomic_i16_store_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i16_store_explicit: _obj is NULL");
    mmconc_atomic_i16_base_store_explicit(_obj, _desired, _order);
}

MG_CAPI_INLINE void mmconc_atomic_u16_store_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u16_store_explicit: _obj is NULL");
    mmconc_atomic_u16_base_store_explicit(_obj, _desired, _order);
}

MG_CAPI_INLINE void mmconc_atomic_i32_store_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i32_store_explicit: _obj is NULL");
    mmconc_atomic_i32_base_store_explicit(_obj, _desired, _order);
}

MG_CAPI_INLINE void mmconc_atomic_u32_store_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u32_store_explicit: _obj is NULL");
    mmconc_atomic_u32_base_store_explicit(_obj, _desired, _order);
}

MG_CAPI_INLINE void mmconc_atomic_i64_store_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i64_store_explicit: _obj is NULL");
    mmconc_atomic_i64_base_store_explicit(_obj, _desired, _order);
}

MG_CAPI_INLINE void mmconc_atomic_u64_store_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u64_store_explicit: _obj is NULL");
    mmconc_atomic_u64_base_store_explicit(_obj, _desired, _order);
}

#define mmconc_atomic_bl_store(ptr, desired)  mmconc_atomic_bl_store_explicit (ptr, desired, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i8_store(ptr, desired)  mmconc_atomic_i8_store_explicit (ptr, desired, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u8_store(ptr, desired)  mmconc_atomic_u8_store_explicit (ptr, desired, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i16_store(ptr, desired) mmconc_atomic_i16_store_explicit(ptr, desired, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u16_store(ptr, desired) mmconc_atomic_u16_store_explicit(ptr, desired, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i32_store(ptr, desired) mmconc_atomic_i32_store_explicit(ptr, desired, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u32_store(ptr, desired) mmconc_atomic_u32_store_explicit(ptr, desired, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i64_store(ptr, desired) mmconc_atomic_i64_store_explicit(ptr, desired, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u64_store(ptr, desired) mmconc_atomic_u64_store_explicit(ptr, desired, mmconc_memory_order_seq_cst)

#define mmconc_atomic_store_explicit(ptr, desired, order) \
    do { \
        if ((ptr) == NULL) \
            break; \
        if (((const mmconc_atomic_none_t*)ptr)->st_size <= sizeof(mmconc_atomic_none_t)) \
            break; \
        switch (((const mmconc_atomic_none_t*)ptr)->type) { \
        case mmconc_atomic_bool_type:  mmconc_atomic_bl_store_explicit ((volatile mmconc_atomic_bool_t*)ptr, desired, order); break; \
        case mmconc_atomic_int8_type:  mmconc_atomic_i8_store_explicit ((volatile mmconc_atomic_int8_t*)ptr, desired, order); break; \
        case mmconc_atomic_uint8_type: mmconc_atomic_u8_store_explicit ((volatile mmconc_atomic_uint8_t*)ptr, desired, order); break; \
        case mmconc_atomic_int16_type: mmconc_atomic_i16_store_explicit((volatile mmconc_atomic_int16_t*)ptr, desired, order); break; \
        case mmconc_atomic_uint16_type:mmconc_atomic_u16_store_explicit((volatile mmconc_atomic_uint16_t*)ptr, desired, order); break; \
        case mmconc_atomic_int32_type: mmconc_atomic_i32_store_explicit((volatile mmconc_atomic_int32_t*)ptr, desired, order); break; \
        case mmconc_atomic_uint32_type:mmconc_atomic_u32_store_explicit((volatile mmconc_atomic_uint32_t*)ptr, desired, order); break; \
        case mmconc_atomic_int64_type: mmconc_atomic_i64_store_explicit((volatile mmconc_atomic_int64_t*)ptr, desired, order); break; \
        case mmconc_atomic_uint64_type:mmconc_atomic_u64_store_explicit((volatile mmconc_atomic_uint64_t*)ptr, desired, order); break; \
        default: break; \
        } \
    } while (0)

#define mmconc_atomic_store(ptr, desired) \
    mmconc_atomic_store_explicit(ptr, desired, mmconc_memory_order_seq_cst)

MEME_API int 
MEME_STDCALL mmconc_atomic_bl_base_load_explicit (const volatile mmconc_atomic_bool_t  * _obj, mmconc_memory_order_e _order);
MEME_API int8_t
MEME_STDCALL mmconc_atomic_i8_base_load_explicit (const volatile mmconc_atomic_int8_t  * _obj, mmconc_memory_order_e _order);
MEME_API uint8_t
MEME_STDCALL mmconc_atomic_u8_base_load_explicit (const volatile mmconc_atomic_uint8_t * _obj, mmconc_memory_order_e _order);
MEME_API int16_t
MEME_STDCALL mmconc_atomic_i16_base_load_explicit(const volatile mmconc_atomic_int16_t * _obj, mmconc_memory_order_e _order);
MEME_API uint16_t
MEME_STDCALL mmconc_atomic_u16_base_load_explicit(const volatile mmconc_atomic_uint16_t* _obj, mmconc_memory_order_e _order);
MEME_API int32_t
MEME_STDCALL mmconc_atomic_i32_base_load_explicit(const volatile mmconc_atomic_int32_t * _obj, mmconc_memory_order_e _order);
MEME_API uint32_t
MEME_STDCALL mmconc_atomic_u32_base_load_explicit(const volatile mmconc_atomic_uint32_t* _obj, mmconc_memory_order_e _order);
MEME_API int64_t
MEME_STDCALL mmconc_atomic_i64_base_load_explicit(const volatile mmconc_atomic_int64_t * _obj, mmconc_memory_order_e _order);
MEME_API uint64_t
MEME_STDCALL mmconc_atomic_u64_base_load_explicit(const volatile mmconc_atomic_uint64_t* _obj, mmconc_memory_order_e _order);

MG_CAPI_INLINE int mmconc_atomic_bl_load_explicit(const volatile mmconc_atomic_bool_t* _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_bl_load_explicit: _obj is NULL");
    return mmconc_atomic_bl_base_load_explicit(_obj, _order);
}

MG_CAPI_INLINE int8_t mmconc_atomic_i8_load_explicit(const volatile mmconc_atomic_int8_t* _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i8_load_explicit: _obj is NULL");
    return mmconc_atomic_i8_base_load_explicit(_obj, _order);
}

MG_CAPI_INLINE uint8_t mmconc_atomic_u8_load_explicit(const volatile mmconc_atomic_uint8_t* _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u8_load_explicit: _obj is NULL");
    return mmconc_atomic_u8_base_load_explicit(_obj, _order);
}

MG_CAPI_INLINE int16_t mmconc_atomic_i16_load_explicit(const volatile mmconc_atomic_int16_t* _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i16_load_explicit: _obj is NULL");
    return mmconc_atomic_i16_base_load_explicit(_obj, _order);
}

MG_CAPI_INLINE uint16_t mmconc_atomic_u16_load_explicit(const volatile mmconc_atomic_uint16_t* _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u16_load_explicit: _obj is NULL");
    return mmconc_atomic_u16_base_load_explicit(_obj, _order);
}

MG_CAPI_INLINE int32_t mmconc_atomic_i32_load_explicit(const volatile mmconc_atomic_int32_t* _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i32_load_explicit: _obj is NULL");
    return mmconc_atomic_i32_base_load_explicit(_obj, _order);
}

MG_CAPI_INLINE uint32_t mmconc_atomic_u32_load_explicit(const volatile mmconc_atomic_uint32_t* _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u32_load_explicit: _obj is NULL");
    return mmconc_atomic_u32_base_load_explicit(_obj, _order);
}

MG_CAPI_INLINE int64_t mmconc_atomic_i64_load_explicit(const volatile mmconc_atomic_int64_t* _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i64_load_explicit: _obj is NULL");
    return mmconc_atomic_i64_base_load_explicit(_obj, _order);
}

MG_CAPI_INLINE uint64_t mmconc_atomic_u64_load_explicit(const volatile mmconc_atomic_uint64_t* _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u64_load_explicit: _obj is NULL");
    return mmconc_atomic_u64_base_load_explicit(_obj, _order);
}

#define mmconc_atomic_bl_load(ptr)  mmconc_atomic_bl_load_explicit (ptr, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i8_load(ptr)  mmconc_atomic_i8_load_explicit (ptr, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u8_load(ptr)  mmconc_atomic_u8_load_explicit (ptr, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i16_load(ptr) mmconc_atomic_i16_load_explicit(ptr, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u16_load(ptr) mmconc_atomic_u16_load_explicit(ptr, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i32_load(ptr) mmconc_atomic_i32_load_explicit(ptr, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u32_load(ptr) mmconc_atomic_u32_load_explicit(ptr, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i64_load(ptr) mmconc_atomic_i64_load_explicit(ptr, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u64_load(ptr) mmconc_atomic_u64_load_explicit(ptr, mmconc_memory_order_seq_cst)

MEME_API int 
MEME_STDCALL mmconc_atomic_bl_base_exchange_explicit(
    volatile mmconc_atomic_bool_t* _obj, int _desired, mmconc_memory_order_e _order);
MEME_API int8_t 
MEME_STDCALL mmconc_atomic_i8_base_exchange_explicit(
    volatile mmconc_atomic_int8_t* _obj, int8_t _desired, mmconc_memory_order_e _order);
MEME_API uint8_t 
MEME_STDCALL mmconc_atomic_u8_base_exchange_explicit(
    volatile mmconc_atomic_uint8_t* _obj, uint8_t _desired, mmconc_memory_order_e _order);
MEME_API int16_t 
MEME_STDCALL mmconc_atomic_i16_base_exchange_explicit(
    volatile mmconc_atomic_int16_t* _obj, int16_t _desired, mmconc_memory_order_e _order);
MEME_API uint16_t 
MEME_STDCALL mmconc_atomic_u16_base_exchange_explicit(
    volatile mmconc_atomic_uint16_t* _obj, uint16_t _desired, mmconc_memory_order_e _order);
MEME_API int32_t 
MEME_STDCALL mmconc_atomic_i32_base_exchange_explicit(
    volatile mmconc_atomic_int32_t* _obj, int32_t _desired, mmconc_memory_order_e _order);
MEME_API uint32_t 
MEME_STDCALL mmconc_atomic_u32_base_exchange_explicit(
    volatile mmconc_atomic_uint32_t* _obj, uint32_t _desired, mmconc_memory_order_e _order);
MEME_API int64_t 
MEME_STDCALL mmconc_atomic_i64_base_exchange_explicit(
    volatile mmconc_atomic_int64_t* _obj, int64_t _desired, mmconc_memory_order_e _order);
MEME_API uint64_t 
MEME_STDCALL mmconc_atomic_u64_base_exchange_explicit(
    volatile mmconc_atomic_uint64_t* _obj, uint64_t _desired, mmconc_memory_order_e _order);

MG_CAPI_INLINE int mmconc_atomic_bl_exchange_explicit(volatile mmconc_atomic_bool_t* _obj, int _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_bl_exchange_explicit: _obj is NULL");
    return mmconc_atomic_bl_base_exchange_explicit(_obj, _desired, _order);
}

MG_CAPI_INLINE int8_t mmconc_atomic_i8_exchange_explicit(volatile mmconc_atomic_int8_t* _obj, int8_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i8_exchange_explicit: _obj is NULL");
    return mmconc_atomic_i8_base_exchange_explicit(_obj, _desired, _order);
}

MG_CAPI_INLINE uint8_t mmconc_atomic_u8_exchange_explicit(volatile mmconc_atomic_uint8_t* _obj, uint8_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u8_exchange_explicit: _obj is NULL");
    return mmconc_atomic_u8_base_exchange_explicit(_obj, _desired, _order);
}

MG_CAPI_INLINE int16_t mmconc_atomic_i16_exchange_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i16_exchange_explicit: _obj is NULL");
    return mmconc_atomic_i16_base_exchange_explicit(_obj, _desired, _order);
}

MG_CAPI_INLINE uint16_t mmconc_atomic_u16_exchange_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u16_exchange_explicit: _obj is NULL");
    return mmconc_atomic_u16_base_exchange_explicit(_obj, _desired, _order);
}

MG_CAPI_INLINE int32_t mmconc_atomic_i32_exchange_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i32_exchange_explicit: _obj is NULL");
    return mmconc_atomic_i32_base_exchange_explicit(_obj, _desired, _order);
}

MG_CAPI_INLINE uint32_t mmconc_atomic_u32_exchange_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u32_exchange_explicit: _obj is NULL");
    return mmconc_atomic_u32_base_exchange_explicit(_obj, _desired, _order);
}

MG_CAPI_INLINE int64_t mmconc_atomic_i64_exchange_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i64_exchange_explicit: _obj is NULL");
    return mmconc_atomic_i64_base_exchange_explicit(_obj, _desired, _order);
}

MG_CAPI_INLINE uint64_t mmconc_atomic_u64_exchange_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u64_exchange_explicit: _obj is NULL");
    return mmconc_atomic_u64_base_exchange_explicit(_obj, _desired, _order);
}

#define mmconc_atomic_bl_exchange(ptr, desired)  mmconc_atomic_bl_exchange_explicit (ptr, desired, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i8_exchange(ptr, desired)  mmconc_atomic_i8_exchange_explicit (ptr, desired, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u8_exchange(ptr, desired)  mmconc_atomic_u8_exchange_explicit (ptr, desired, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i16_exchange(ptr, desired) mmconc_atomic_i16_exchange_explicit(ptr, desired, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u16_exchange(ptr, desired) mmconc_atomic_u16_exchange_explicit(ptr, desired, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i32_exchange(ptr, desired) mmconc_atomic_i32_exchange_explicit(ptr, desired, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u32_exchange(ptr, desired) mmconc_atomic_u32_exchange_explicit(ptr, desired, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i64_exchange(ptr, desired) mmconc_atomic_i64_exchange_explicit(ptr, desired, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u64_exchange(ptr, desired) mmconc_atomic_u64_exchange_explicit(ptr, desired, mmconc_memory_order_seq_cst)

MEME_API int MEME_STDCALL mmconc_atomic_bl_base_compare_exchange_strong_explicit(
    volatile mmconc_atomic_bool_t* _obj, int* _expected, int _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure);
MEME_API int MEME_STDCALL mmconc_atomic_i8_base_compare_exchange_strong_explicit(
    volatile mmconc_atomic_int8_t* _obj, int8_t* _expected, int8_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure);
MEME_API int MEME_STDCALL mmconc_atomic_u8_base_compare_exchange_strong_explicit(
    volatile mmconc_atomic_uint8_t* _obj, uint8_t* _expected, uint8_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure);
MEME_API int MEME_STDCALL mmconc_atomic_i16_base_compare_exchange_strong_explicit(
    volatile mmconc_atomic_int16_t* _obj, int16_t* _expected, int16_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure);
MEME_API int MEME_STDCALL mmconc_atomic_u16_base_compare_exchange_strong_explicit(
    volatile mmconc_atomic_uint16_t* _obj, uint16_t* _expected, uint16_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure);
MEME_API int MEME_STDCALL mmconc_atomic_i32_base_compare_exchange_strong_explicit(
    volatile mmconc_atomic_int32_t* _obj, int32_t* _expected, int32_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure);
MEME_API int MEME_STDCALL mmconc_atomic_u32_base_compare_exchange_strong_explicit(
    volatile mmconc_atomic_uint32_t* _obj, uint32_t* _expected, uint32_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure);
MEME_API int MEME_STDCALL mmconc_atomic_i64_base_compare_exchange_strong_explicit(
    volatile mmconc_atomic_int64_t* _obj, int64_t* _expected, int64_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure);
MEME_API int MEME_STDCALL mmconc_atomic_u64_base_compare_exchange_strong_explicit(
    volatile mmconc_atomic_uint64_t* _obj, uint64_t* _expected, uint64_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure);

MG_CAPI_INLINE int mmconc_atomic_bl_compare_exchange_strong_explicit(
    volatile mmconc_atomic_bool_t* _obj, int* _expected, int _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_bl_compare_exchange_strong_explicit: _obj is NULL");
    assert(_expected != NULL && "mmconc_atomic_bl_compare_exchange_strong_explicit: _expected is NULL");
    return mmconc_atomic_bl_base_compare_exchange_strong_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE int mmconc_atomic_i8_compare_exchange_strong_explicit(
    volatile mmconc_atomic_int8_t* _obj, int8_t* _expected, int8_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_i8_compare_exchange_strong_explicit: _obj is NULL");
    assert(_expected != NULL && "mmconc_atomic_i8_compare_exchange_strong_explicit: _expected is NULL");
    return mmconc_atomic_i8_base_compare_exchange_strong_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE int mmconc_atomic_u8_compare_exchange_strong_explicit(
    volatile mmconc_atomic_uint8_t* _obj, uint8_t* _expected, uint8_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_u8_compare_exchange_strong_explicit: _obj is NULL");
    assert(_expected != NULL && "mmconc_atomic_u8_compare_exchange_strong_explicit: _expected is NULL");
    return mmconc_atomic_u8_base_compare_exchange_strong_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE int mmconc_atomic_i16_compare_exchange_strong_explicit(
    volatile mmconc_atomic_int16_t* _obj, int16_t* _expected, int16_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_i16_compare_exchange_strong_explicit: _obj is NULL");
    assert(_expected != NULL && "mmconc_atomic_i16_compare_exchange_strong_explicit: _expected is NULL");
    return mmconc_atomic_i16_base_compare_exchange_strong_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE int mmconc_atomic_u16_compare_exchange_strong_explicit(
    volatile mmconc_atomic_uint16_t* _obj, uint16_t* _expected, uint16_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_u16_compare_exchange_strong_explicit: _obj is NULL");
    assert(_expected != NULL && "mmconc_atomic_u16_compare_exchange_strong_explicit: _expected is NULL");
    return mmconc_atomic_u16_base_compare_exchange_strong_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE int mmconc_atomic_i32_compare_exchange_strong_explicit(
    volatile mmconc_atomic_int32_t* _obj, int32_t* _expected, int32_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_i32_compare_exchange_strong_explicit: _obj is NULL");
    assert(_expected != NULL && "mmconc_atomic_i32_compare_exchange_strong_explicit: _expected is NULL");
    return mmconc_atomic_i32_base_compare_exchange_strong_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE int mmconc_atomic_u32_compare_exchange_strong_explicit(
    volatile mmconc_atomic_uint32_t* _obj, uint32_t* _expected, uint32_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_u32_compare_exchange_strong_explicit: _obj is NULL");
    assert(_expected != NULL && "mmconc_atomic_u32_compare_exchange_strong_explicit: _expected is NULL");
    return mmconc_atomic_u32_base_compare_exchange_strong_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE int mmconc_atomic_i64_compare_exchange_strong_explicit(
    volatile mmconc_atomic_int64_t* _obj, int64_t* _expected, int64_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_i64_compare_exchange_strong_explicit: _obj is NULL");
    assert(_expected != NULL && "mmconc_atomic_i64_compare_exchange_strong_explicit: _expected is NULL");
    return mmconc_atomic_i64_base_compare_exchange_strong_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE int mmconc_atomic_u64_compare_exchange_strong_explicit(
    volatile mmconc_atomic_uint64_t* _obj, uint64_t* _expected, uint64_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_u64_compare_exchange_strong_explicit: _obj is NULL");
    assert(_expected != NULL && "mmconc_atomic_u64_compare_exchange_strong_explicit: _expected is NULL");
    return mmconc_atomic_u64_base_compare_exchange_strong_explicit(_obj, _expected, _desired, _success, _failure);
}

#define mmconc_atomic_bl_compare_exchange_strong(ptr, expected, desired) \
    mmconc_atomic_bl_compare_exchange_strong_explicit (ptr, expected, desired, mmconc_memory_order_seq_cst, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i8_compare_exchange_strong(ptr, expected, desired) \
    mmconc_atomic_i8_compare_exchange_strong_explicit (ptr, expected, desired, mmconc_memory_order_seq_cst, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u8_compare_exchange_strong(ptr, expected, desired) \
    mmconc_atomic_u8_compare_exchange_strong_explicit (ptr, expected, desired, mmconc_memory_order_seq_cst, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i16_compare_exchange_strong(ptr, expected, desired) \
    mmconc_atomic_i16_compare_exchange_strong_explicit(ptr, expected, desired, mmconc_memory_order_seq_cst, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u16_compare_exchange_strong(ptr, expected, desired) \
    mmconc_atomic_u16_compare_exchange_strong_explicit(ptr, expected, desired, mmconc_memory_order_seq_cst, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i32_compare_exchange_strong(ptr, expected, desired) \
    mmconc_atomic_i32_compare_exchange_strong_explicit(ptr, expected, desired, mmconc_memory_order_seq_cst, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u32_compare_exchange_strong(ptr, expected, desired) \
    mmconc_atomic_u32_compare_exchange_strong_explicit(ptr, expected, desired, mmconc_memory_order_seq_cst, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i64_compare_exchange_strong(ptr, expected, desired) \
    mmconc_atomic_i64_compare_exchange_strong_explicit(ptr, expected, desired, mmconc_memory_order_seq_cst, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u64_compare_exchange_strong(ptr, expected, desired) \
    mmconc_atomic_u64_compare_exchange_strong_explicit(ptr, expected, desired, mmconc_memory_order_seq_cst, mmconc_memory_order_seq_cst)

#define mmconc_atomic_compare_exchange_strong_explicit(ptr, expected, desired, success, failure) \
    do { \
        if ((ptr) == NULL) \
            break; \
        if (((const mmconc_atomic_none_t*)ptr)->st_size <= sizeof(mmconc_atomic_none_t)) \
            break; \
        switch (((const mmconc_atomic_none_t*)ptr)->type) { \
        case mmconc_atomic_bool_type:  mmconc_atomic_bl_compare_exchange_strong_explicit ((volatile mmconc_atomic_bool_t*)ptr, expected, desired, success, failure); break; \
        case mmconc_atomic_int8_type:  mmconc_atomic_i8_compare_exchange_strong_explicit ((volatile mmconc_atomic_int8_t*)ptr, expected, desired, success, failure); break; \
        case mmconc_atomic_uint8_type: mmconc_atomic_u8_compare_exchange_strong_explicit ((volatile mmconc_atomic_uint8_t*)ptr, expected, desired, success, failure); break; \
        case mmconc_atomic_int16_type: mmconc_atomic_i16_compare_exchange_strong_explicit((volatile mmconc_atomic_int16_t*)ptr, expected, desired, success, failure); break; \
        case mmconc_atomic_uint16_type:mmconc_atomic_u16_compare_exchange_strong_explicit((volatile mmconc_atomic_uint16_t*)ptr, expected, desired, success, failure); break; \
        case mmconc_atomic_int32_type: mmconc_atomic_i32_compare_exchange_strong_explicit((volatile mmconc_atomic_int32_t*)ptr, expected, desired, success, failure); break; \
        case mmconc_atomic_uint32_type:mmconc_atomic_u32_compare_exchange_strong_explicit((volatile mmconc_atomic_uint32_t*)ptr, expected, desired, success, failure); break; \
        case mmconc_atomic_int64_type: mmconc_atomic_i64_compare_exchange_strong_explicit((volatile mmconc_atomic_int64_t*)ptr, expected, desired, success, failure); break; \
        case mmconc_atomic_uint64_type:mmconc_atomic_u64_compare_exchange_strong_explicit((volatile mmconc_atomic_uint64_t*)ptr, expected, desired, success, failure); break; \
        default: break; \
        } \
    } while (0)

#define mmconc_atomic_compare_exchange_strong(ptr, expected, desired) \
    mmconc_atomic_compare_exchange_strong_explicit(ptr, expected, desired, mmconc_memory_order_seq_cst, mmconc_memory_order_seq_cst)

MEME_API int MEME_STDCALL mmconc_atomic_bl_base_compare_exchange_weak_explicit(
    volatile mmconc_atomic_bool_t* _obj, int* _expected, int _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure);
MEME_API int MEME_STDCALL mmconc_atomic_i8_base_compare_exchange_weak_explicit(
    volatile mmconc_atomic_int8_t* _obj, int8_t* _expected, int8_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure);
MEME_API int MEME_STDCALL mmconc_atomic_u8_base_compare_exchange_weak_explicit(
    volatile mmconc_atomic_uint8_t* _obj, uint8_t* _expected, uint8_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure);
MEME_API int MEME_STDCALL mmconc_atomic_i16_base_compare_exchange_weak_explicit(
    volatile mmconc_atomic_int16_t* _obj, int16_t* _expected, int16_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure);
MEME_API int MEME_STDCALL mmconc_atomic_u16_base_compare_exchange_weak_explicit(
    volatile mmconc_atomic_uint16_t* _obj, uint16_t* _expected, uint16_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure);
MEME_API int MEME_STDCALL mmconc_atomic_i32_base_compare_exchange_weak_explicit(
    volatile mmconc_atomic_int32_t* _obj, int32_t* _expected, int32_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure);
MEME_API int MEME_STDCALL mmconc_atomic_u32_base_compare_exchange_weak_explicit(
    volatile mmconc_atomic_uint32_t* _obj, uint32_t* _expected, uint32_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure);
MEME_API int MEME_STDCALL mmconc_atomic_i64_base_compare_exchange_weak_explicit(
    volatile mmconc_atomic_int64_t* _obj, int64_t* _expected, int64_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure);
MEME_API int MEME_STDCALL mmconc_atomic_u64_base_compare_exchange_weak_explicit(
    volatile mmconc_atomic_uint64_t* _obj, uint64_t* _expected, uint64_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure);

MG_CAPI_INLINE int mmconc_atomic_bl_compare_exchange_weak_explicit(
    volatile mmconc_atomic_bool_t* _obj, int* _expected, int _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_bl_compare_exchange_weak_explicit: _obj is NULL");
    assert(_expected != NULL && "mmconc_atomic_bl_compare_exchange_weak_explicit: _expected is NULL");
    return mmconc_atomic_bl_base_compare_exchange_weak_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE int mmconc_atomic_i8_compare_exchange_weak_explicit(
    volatile mmconc_atomic_int8_t* _obj, int8_t* _expected, int8_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_i8_compare_exchange_weak_explicit: _obj is NULL");
    assert(_expected != NULL && "mmconc_atomic_i8_compare_exchange_weak_explicit: _expected is NULL");
    return mmconc_atomic_i8_base_compare_exchange_weak_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE int mmconc_atomic_u8_compare_exchange_weak_explicit(
    volatile mmconc_atomic_uint8_t* _obj, uint8_t* _expected, uint8_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_u8_compare_exchange_weak_explicit: _obj is NULL");
    assert(_expected != NULL && "mmconc_atomic_u8_compare_exchange_weak_explicit: _expected is NULL");
    return mmconc_atomic_u8_base_compare_exchange_weak_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE int mmconc_atomic_i16_compare_exchange_weak_explicit(
    volatile mmconc_atomic_int16_t* _obj, int16_t* _expected, int16_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_i16_compare_exchange_weak_explicit: _obj is NULL");
    assert(_expected != NULL && "mmconc_atomic_i16_compare_exchange_weak_explicit: _expected is NULL");
    return mmconc_atomic_i16_base_compare_exchange_weak_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE int mmconc_atomic_u16_compare_exchange_weak_explicit(
    volatile mmconc_atomic_uint16_t* _obj, uint16_t* _expected, uint16_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_u16_compare_exchange_weak_explicit: _obj is NULL");
    assert(_expected != NULL && "mmconc_atomic_u16_compare_exchange_weak_explicit: _expected is NULL");
    return mmconc_atomic_u16_base_compare_exchange_weak_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE int mmconc_atomic_i32_compare_exchange_weak_explicit(
    volatile mmconc_atomic_int32_t* _obj, int32_t* _expected, int32_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_i32_compare_exchange_weak_explicit: _obj is NULL");
    assert(_expected != NULL && "mmconc_atomic_i32_compare_exchange_weak_explicit: _expected is NULL");
    return mmconc_atomic_i32_base_compare_exchange_weak_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE int mmconc_atomic_u32_compare_exchange_weak_explicit(
    volatile mmconc_atomic_uint32_t* _obj, uint32_t* _expected, uint32_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_u32_compare_exchange_weak_explicit: _obj is NULL");
    assert(_expected != NULL && "mmconc_atomic_u32_compare_exchange_weak_explicit: _expected is NULL");
    return mmconc_atomic_u32_base_compare_exchange_weak_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE int mmconc_atomic_i64_compare_exchange_weak_explicit(
    volatile mmconc_atomic_int64_t* _obj, int64_t* _expected, int64_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_i64_compare_exchange_weak_explicit: _obj is NULL");
    assert(_expected != NULL && "mmconc_atomic_i64_compare_exchange_weak_explicit: _expected is NULL");
    return mmconc_atomic_i64_base_compare_exchange_weak_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE int mmconc_atomic_u64_compare_exchange_weak_explicit(
    volatile mmconc_atomic_uint64_t* _obj, uint64_t* _expected, uint64_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_u64_compare_exchange_weak_explicit: _obj is NULL");
    assert(_expected != NULL && "mmconc_atomic_u64_compare_exchange_weak_explicit: _expected is NULL");
    return mmconc_atomic_u64_base_compare_exchange_weak_explicit(_obj, _expected, _desired, _success, _failure);
}

#define mmconc_atomic_bl_compare_exchange_weak(ptr, expected, desired) \
    mmconc_atomic_bl_compare_exchange_weak_explicit (ptr, expected, desired, mmconc_memory_order_seq_cst, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i8_compare_exchange_weak(ptr, expected, desired) \
    mmconc_atomic_i8_compare_exchange_weak_explicit (ptr, expected, desired, mmconc_memory_order_seq_cst, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u8_compare_exchange_weak(ptr, expected, desired) \
    mmconc_atomic_u8_compare_exchange_weak_explicit (ptr, expected, desired, mmconc_memory_order_seq_cst, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i16_compare_exchange_weak(ptr, expected, desired) \
    mmconc_atomic_i16_compare_exchange_weak_explicit(ptr, expected, desired, mmconc_memory_order_seq_cst, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u16_compare_exchange_weak(ptr, expected, desired) \
    mmconc_atomic_u16_compare_exchange_weak_explicit(ptr, expected, desired, mmconc_memory_order_seq_cst, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i32_compare_exchange_weak(ptr, expected, desired) \
    mmconc_atomic_i32_compare_exchange_weak_explicit(ptr, expected, desired, mmconc_memory_order_seq_cst, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u32_compare_exchange_weak(ptr, expected, desired) \
    mmconc_atomic_u32_compare_exchange_weak_explicit(ptr, expected, desired, mmconc_memory_order_seq_cst, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i64_compare_exchange_weak(ptr, expected, desired) \
    mmconc_atomic_i64_compare_exchange_weak_explicit(ptr, expected, desired, mmconc_memory_order_seq_cst, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u64_compare_exchange_weak(ptr, expected, desired) \
    mmconc_atomic_u64_compare_exchange_weak_explicit(ptr, expected, desired, mmconc_memory_order_seq_cst, mmconc_memory_order_seq_cst)

#define mmconc_atomic_compare_exchange_weak_explicit(ptr, expected, desired, success, failure) \
    do { \
        if ((ptr) == NULL) \
            break; \
        if (((const mmconc_atomic_none_t*)ptr)->st_size <= sizeof(mmconc_atomic_none_t)) \
            break; \
        switch (((const mmconc_atomic_none_t*)ptr)->type) { \
        case mmconc_atomic_bool_type:  mmconc_atomic_bl_compare_exchange_weak_explicit ((volatile mmconc_atomic_bool_t*)ptr, expected, desired, success, failure); break; \
        case mmconc_atomic_int8_type:  mmconc_atomic_i8_compare_exchange_weak_explicit ((volatile mmconc_atomic_int8_t*)ptr, expected, desired, success, failure); break; \
        case mmconc_atomic_uint8_type: mmconc_atomic_u8_compare_exchange_weak_explicit ((volatile mmconc_atomic_uint8_t*)ptr, expected, desired, success, failure); break; \
        case mmconc_atomic_int16_type: mmconc_atomic_i16_compare_exchange_weak_explicit((volatile mmconc_atomic_int16_t*)ptr, expected, desired, success, failure); break; \
        case mmconc_atomic_uint16_type:mmconc_atomic_u16_compare_exchange_weak_explicit((volatile mmconc_atomic_uint16_t*)ptr, expected, desired, success, failure); break; \
        case mmconc_atomic_int32_type: mmconc_atomic_i32_compare_exchange_weak_explicit((volatile mmconc_atomic_int32_t*)ptr, expected, desired, success, failure); break; \
        case mmconc_atomic_uint32_type:mmconc_atomic_u32_compare_exchange_weak_explicit((volatile mmconc_atomic_uint32_t*)ptr, expected, desired, success, failure); break; \
        case mmconc_atomic_int64_type: mmconc_atomic_i64_compare_exchange_weak_explicit((volatile mmconc_atomic_int64_t*)ptr, expected, desired, success, failure); break; \
        case mmconc_atomic_uint64_type:mmconc_atomic_u64_compare_exchange_weak_explicit((volatile mmconc_atomic_uint64_t*)ptr, expected, desired, success, failure); break; \
        default: break; \
        } \
    } while (0)

#define mmconc_atomic_compare_exchange_weak(ptr, expected, desired) \
    mmconc_atomic_compare_exchange_weak_explicit(ptr, expected, desired, mmconc_memory_order_seq_cst, mmconc_memory_order_seq_cst)

MEME_API int8_t 
MEME_STDCALL mmconc_atomic_i8_base_fetch_add_explicit (volatile mmconc_atomic_int8_t*  _obj, int8_t  _arg, mmconc_memory_order_e _order);
MEME_API uint8_t
MEME_STDCALL mmconc_atomic_u8_base_fetch_add_explicit (volatile mmconc_atomic_uint8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order);
MEME_API int16_t
MEME_STDCALL mmconc_atomic_i16_base_fetch_add_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _arg, mmconc_memory_order_e _order);
MEME_API uint16_t
MEME_STDCALL mmconc_atomic_u16_base_fetch_add_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order);
MEME_API int32_t
MEME_STDCALL mmconc_atomic_i32_base_fetch_add_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _arg, mmconc_memory_order_e _order);
MEME_API uint32_t
MEME_STDCALL mmconc_atomic_u32_base_fetch_add_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order);
MEME_API int64_t
MEME_STDCALL mmconc_atomic_i64_base_fetch_add_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _arg, mmconc_memory_order_e _order);
MEME_API uint64_t
MEME_STDCALL mmconc_atomic_u64_base_fetch_add_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order);

MG_CAPI_INLINE int8_t mmconc_atomic_i8_fetch_add_explicit(volatile mmconc_atomic_int8_t* _obj, int8_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i8_fetch_add_explicit: _obj is NULL");
    return mmconc_atomic_i8_base_fetch_add_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE uint8_t mmconc_atomic_u8_fetch_add_explicit(volatile mmconc_atomic_uint8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u8_fetch_add_explicit: _obj is NULL");
    return mmconc_atomic_u8_base_fetch_add_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE int16_t mmconc_atomic_i16_fetch_add_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i16_fetch_add_explicit: _obj is NULL");
    return mmconc_atomic_i16_base_fetch_add_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE uint16_t mmconc_atomic_u16_fetch_add_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u16_fetch_add_explicit: _obj is NULL");
    return mmconc_atomic_u16_base_fetch_add_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE int32_t mmconc_atomic_i32_fetch_add_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i32_fetch_add_explicit: _obj is NULL");
    return mmconc_atomic_i32_base_fetch_add_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE uint32_t mmconc_atomic_u32_fetch_add_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u32_fetch_add_explicit: _obj is NULL");
    return mmconc_atomic_u32_base_fetch_add_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE int64_t mmconc_atomic_i64_fetch_add_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i64_fetch_add_explicit: _obj is NULL");
    return mmconc_atomic_i64_base_fetch_add_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE uint64_t mmconc_atomic_u64_fetch_add_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u64_fetch_add_explicit: _obj is NULL");
    return mmconc_atomic_u64_base_fetch_add_explicit(_obj, _arg, _order);
}

#define mmconc_atomic_i8_fetch_add(ptr, arg)  mmconc_atomic_i8_fetch_add_explicit (ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u8_fetch_add(ptr, arg)  mmconc_atomic_u8_fetch_add_explicit (ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i16_fetch_add(ptr, arg) mmconc_atomic_i16_fetch_add_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u16_fetch_add(ptr, arg) mmconc_atomic_u16_fetch_add_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i32_fetch_add(ptr, arg) mmconc_atomic_i32_fetch_add_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u32_fetch_add(ptr, arg) mmconc_atomic_u32_fetch_add_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i64_fetch_add(ptr, arg) mmconc_atomic_i64_fetch_add_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u64_fetch_add(ptr, arg) mmconc_atomic_u64_fetch_add_explicit(ptr, arg, mmconc_memory_order_seq_cst)

MEME_API int8_t
MEME_STDCALL mmconc_atomic_i8_base_fetch_sub_explicit (volatile mmconc_atomic_int8_t*  _obj, int8_t  _arg, mmconc_memory_order_e _order);
MEME_API uint8_t
MEME_STDCALL mmconc_atomic_u8_base_fetch_sub_explicit (volatile mmconc_atomic_uint8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order);
MEME_API int16_t
MEME_STDCALL mmconc_atomic_i16_base_fetch_sub_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _arg, mmconc_memory_order_e _order);
MEME_API uint16_t
MEME_STDCALL mmconc_atomic_u16_base_fetch_sub_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order);
MEME_API int32_t
MEME_STDCALL mmconc_atomic_i32_base_fetch_sub_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _arg, mmconc_memory_order_e _order);
MEME_API uint32_t
MEME_STDCALL mmconc_atomic_u32_base_fetch_sub_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order);
MEME_API int64_t
MEME_STDCALL mmconc_atomic_i64_base_fetch_sub_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _arg, mmconc_memory_order_e _order);
MEME_API uint64_t
MEME_STDCALL mmconc_atomic_u64_base_fetch_sub_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order);

MG_CAPI_INLINE int8_t mmconc_atomic_i8_fetch_sub_explicit(volatile mmconc_atomic_int8_t* _obj, int8_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i8_fetch_sub_explicit: _obj is NULL");
    return mmconc_atomic_i8_base_fetch_sub_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE uint8_t mmconc_atomic_u8_fetch_sub_explicit(volatile mmconc_atomic_uint8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u8_fetch_sub_explicit: _obj is NULL");
    return mmconc_atomic_u8_base_fetch_sub_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE int16_t mmconc_atomic_i16_fetch_sub_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i16_fetch_sub_explicit: _obj is NULL");
    return mmconc_atomic_i16_base_fetch_sub_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE uint16_t mmconc_atomic_u16_fetch_sub_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u16_fetch_sub_explicit: _obj is NULL");
    return mmconc_atomic_u16_base_fetch_sub_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE int32_t mmconc_atomic_i32_fetch_sub_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i32_fetch_sub_explicit: _obj is NULL");
    return mmconc_atomic_i32_base_fetch_sub_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE uint32_t mmconc_atomic_u32_fetch_sub_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u32_fetch_sub_explicit: _obj is NULL");
    return mmconc_atomic_u32_base_fetch_sub_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE int64_t mmconc_atomic_i64_fetch_sub_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i64_fetch_sub_explicit: _obj is NULL");
    return mmconc_atomic_i64_base_fetch_sub_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE uint64_t mmconc_atomic_u64_fetch_sub_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u64_fetch_sub_explicit: _obj is NULL");
    return mmconc_atomic_u64_base_fetch_sub_explicit(_obj, _arg, _order);
}

#define mmconc_atomic_i8_fetch_sub(ptr, arg)  mmconc_atomic_i8_fetch_sub_explicit (ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u8_fetch_sub(ptr, arg)  mmconc_atomic_u8_fetch_sub_explicit (ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i16_fetch_sub(ptr, arg) mmconc_atomic_i16_fetch_sub_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u16_fetch_sub(ptr, arg) mmconc_atomic_u16_fetch_sub_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i32_fetch_sub(ptr, arg) mmconc_atomic_i32_fetch_sub_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u32_fetch_sub(ptr, arg) mmconc_atomic_u32_fetch_sub_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i64_fetch_sub(ptr, arg) mmconc_atomic_i64_fetch_sub_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u64_fetch_sub(ptr, arg) mmconc_atomic_u64_fetch_sub_explicit(ptr, arg, mmconc_memory_order_seq_cst)

MEME_API int8_t
MEME_STDCALL mmconc_atomic_i8_base_fetch_or_explicit (volatile mmconc_atomic_int8_t*  _obj, int8_t  _arg, mmconc_memory_order_e _order);
MEME_API uint8_t
MEME_STDCALL mmconc_atomic_u8_base_fetch_or_explicit (volatile mmconc_atomic_uint8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order);
MEME_API int16_t
MEME_STDCALL mmconc_atomic_i16_base_fetch_or_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _arg, mmconc_memory_order_e _order);
MEME_API uint16_t
MEME_STDCALL mmconc_atomic_u16_base_fetch_or_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order);
MEME_API int32_t
MEME_STDCALL mmconc_atomic_i32_base_fetch_or_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _arg, mmconc_memory_order_e _order);
MEME_API uint32_t
MEME_STDCALL mmconc_atomic_u32_base_fetch_or_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order);
MEME_API int64_t
MEME_STDCALL mmconc_atomic_i64_base_fetch_or_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _arg, mmconc_memory_order_e _order);
MEME_API uint64_t
MEME_STDCALL mmconc_atomic_u64_base_fetch_or_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order);

MG_CAPI_INLINE int8_t mmconc_atomic_i8_fetch_or_explicit(volatile mmconc_atomic_int8_t* _obj, int8_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i8_fetch_or_explicit: _obj is NULL");
    return mmconc_atomic_i8_base_fetch_or_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE uint8_t mmconc_atomic_u8_fetch_or_explicit(volatile mmconc_atomic_uint8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u8_fetch_or_explicit: _obj is NULL");
    return mmconc_atomic_u8_base_fetch_or_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE int16_t mmconc_atomic_i16_fetch_or_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i16_fetch_or_explicit: _obj is NULL");
    return mmconc_atomic_i16_base_fetch_or_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE uint16_t mmconc_atomic_u16_fetch_or_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u16_fetch_or_explicit: _obj is NULL");
    return mmconc_atomic_u16_base_fetch_or_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE int32_t mmconc_atomic_i32_fetch_or_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i32_fetch_or_explicit: _obj is NULL");
    return mmconc_atomic_i32_base_fetch_or_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE uint32_t mmconc_atomic_u32_fetch_or_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u32_fetch_or_explicit: _obj is NULL");
    return mmconc_atomic_u32_base_fetch_or_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE int64_t mmconc_atomic_i64_fetch_or_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i64_fetch_or_explicit: _obj is NULL");
    return mmconc_atomic_i64_base_fetch_or_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE uint64_t mmconc_atomic_u64_fetch_or_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u64_fetch_or_explicit: _obj is NULL");
    return mmconc_atomic_u64_base_fetch_or_explicit(_obj, _arg, _order);
}

#define mmconc_atomic_i8_fetch_or(ptr, arg)  mmconc_atomic_i8_fetch_or_explicit (ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u8_fetch_or(ptr, arg)  mmconc_atomic_u8_fetch_or_explicit (ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i16_fetch_or(ptr, arg) mmconc_atomic_i16_fetch_or_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u16_fetch_or(ptr, arg) mmconc_atomic_u16_fetch_or_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i32_fetch_or(ptr, arg) mmconc_atomic_i32_fetch_or_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u32_fetch_or(ptr, arg) mmconc_atomic_u32_fetch_or_explicit(ptr, arg, mmconc_memory_order_seq_cst) 
#define mmconc_atomic_i64_fetch_or(ptr, arg) mmconc_atomic_i64_fetch_or_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u64_fetch_or(ptr, arg) mmconc_atomic_u64_fetch_or_explicit(ptr, arg, mmconc_memory_order_seq_cst)

MEME_API int8_t
MEME_STDCALL mmconc_atomic_i8_base_fetch_xor_explicit (volatile mmconc_atomic_int8_t*  _obj, int8_t  _arg, mmconc_memory_order_e _order);
MEME_API uint8_t
MEME_STDCALL mmconc_atomic_u8_base_fetch_xor_explicit (volatile mmconc_atomic_uint8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order);
MEME_API int16_t
MEME_STDCALL mmconc_atomic_i16_base_fetch_xor_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _arg, mmconc_memory_order_e _order);
MEME_API uint16_t
MEME_STDCALL mmconc_atomic_u16_base_fetch_xor_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order);
MEME_API int32_t
MEME_STDCALL mmconc_atomic_i32_base_fetch_xor_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _arg, mmconc_memory_order_e _order);
MEME_API uint32_t
MEME_STDCALL mmconc_atomic_u32_base_fetch_xor_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order);
MEME_API int64_t
MEME_STDCALL mmconc_atomic_i64_base_fetch_xor_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _arg, mmconc_memory_order_e _order);
MEME_API uint64_t
MEME_STDCALL mmconc_atomic_u64_base_fetch_xor_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order);

MG_CAPI_INLINE int8_t mmconc_atomic_i8_fetch_xor_explicit(volatile mmconc_atomic_int8_t* _obj, int8_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i8_fetch_xor_explicit: _obj is NULL");
    return mmconc_atomic_i8_base_fetch_xor_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE uint8_t mmconc_atomic_u8_fetch_xor_explicit(volatile mmconc_atomic_uint8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u8_fetch_xor_explicit: _obj is NULL");
    return mmconc_atomic_u8_base_fetch_xor_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE int16_t mmconc_atomic_i16_fetch_xor_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i16_fetch_xor_explicit: _obj is NULL");
    return mmconc_atomic_i16_base_fetch_xor_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE uint16_t mmconc_atomic_u16_fetch_xor_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u16_fetch_xor_explicit: _obj is NULL");
    return mmconc_atomic_u16_base_fetch_xor_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE int32_t mmconc_atomic_i32_fetch_xor_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i32_fetch_xor_explicit: _obj is NULL");
    return mmconc_atomic_i32_base_fetch_xor_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE uint32_t mmconc_atomic_u32_fetch_xor_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u32_fetch_xor_explicit: _obj is NULL");
    return mmconc_atomic_u32_base_fetch_xor_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE int64_t mmconc_atomic_i64_fetch_xor_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i64_fetch_xor_explicit: _obj is NULL");
    return mmconc_atomic_i64_base_fetch_xor_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE uint64_t mmconc_atomic_u64_fetch_xor_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u64_fetch_xor_explicit: _obj is NULL");
    return mmconc_atomic_u64_base_fetch_xor_explicit(_obj, _arg, _order);
}

#define mmconc_atomic_i8_fetch_xor(ptr, arg)  mmconc_atomic_i8_fetch_xor_explicit (ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u8_fetch_xor(ptr, arg)  mmconc_atomic_u8_fetch_xor_explicit (ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i16_fetch_xor(ptr, arg) mmconc_atomic_i16_fetch_xor_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u16_fetch_xor(ptr, arg) mmconc_atomic_u16_fetch_xor_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i32_fetch_xor(ptr, arg) mmconc_atomic_i32_fetch_xor_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u32_fetch_xor(ptr, arg) mmconc_atomic_u32_fetch_xor_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i64_fetch_xor(ptr, arg) mmconc_atomic_i64_fetch_xor_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u64_fetch_xor(ptr, arg) mmconc_atomic_u64_fetch_xor_explicit(ptr, arg, mmconc_memory_order_seq_cst)

MEME_API int8_t
MEME_STDCALL mmconc_atomic_i8_base_fetch_and_explicit (volatile mmconc_atomic_int8_t*  _obj, int8_t  _arg, mmconc_memory_order_e _order);
MEME_API uint8_t
MEME_STDCALL mmconc_atomic_u8_base_fetch_and_explicit (volatile mmconc_atomic_uint8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order);
MEME_API int16_t
MEME_STDCALL mmconc_atomic_i16_base_fetch_and_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _arg, mmconc_memory_order_e _order);
MEME_API uint16_t
MEME_STDCALL mmconc_atomic_u16_base_fetch_and_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order);
MEME_API int32_t
MEME_STDCALL mmconc_atomic_i32_base_fetch_and_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _arg, mmconc_memory_order_e _order);
MEME_API uint32_t
MEME_STDCALL mmconc_atomic_u32_base_fetch_and_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order);
MEME_API int64_t
MEME_STDCALL mmconc_atomic_i64_base_fetch_and_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _arg, mmconc_memory_order_e _order);
MEME_API uint64_t
MEME_STDCALL mmconc_atomic_u64_base_fetch_and_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order);

MG_CAPI_INLINE int8_t mmconc_atomic_i8_fetch_and_explicit(volatile mmconc_atomic_int8_t* _obj, int8_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i8_fetch_and_explicit: _obj is NULL");
    return mmconc_atomic_i8_base_fetch_and_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE uint8_t mmconc_atomic_u8_fetch_and_explicit(volatile mmconc_atomic_uint8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u8_fetch_and_explicit: _obj is NULL");
    return mmconc_atomic_u8_base_fetch_and_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE int16_t mmconc_atomic_i16_fetch_and_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i16_fetch_and_explicit: _obj is NULL");
    return mmconc_atomic_i16_base_fetch_and_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE uint16_t mmconc_atomic_u16_fetch_and_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u16_fetch_and_explicit: _obj is NULL");
    return mmconc_atomic_u16_base_fetch_and_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE int32_t mmconc_atomic_i32_fetch_and_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i32_fetch_and_explicit: _obj is NULL");
    return mmconc_atomic_i32_base_fetch_and_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE uint32_t mmconc_atomic_u32_fetch_and_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u32_fetch_and_explicit: _obj is NULL");
    return mmconc_atomic_u32_base_fetch_and_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE int64_t mmconc_atomic_i64_fetch_and_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i64_fetch_and_explicit: _obj is NULL");
    return mmconc_atomic_i64_base_fetch_and_explicit(_obj, _arg, _order);
}

MG_CAPI_INLINE uint64_t mmconc_atomic_u64_fetch_and_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u64_fetch_and_explicit: _obj is NULL");
    return mmconc_atomic_u64_base_fetch_and_explicit(_obj, _arg, _order);
}

#define mmconc_atomic_i8_fetch_and(ptr, arg)  mmconc_atomic_i8_fetch_and_explicit (ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u8_fetch_and(ptr, arg)  mmconc_atomic_u8_fetch_and_explicit (ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i16_fetch_and(ptr, arg) mmconc_atomic_i16_fetch_and_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u16_fetch_and(ptr, arg) mmconc_atomic_u16_fetch_and_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i32_fetch_and(ptr, arg) mmconc_atomic_i32_fetch_and_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u32_fetch_and(ptr, arg) mmconc_atomic_u32_fetch_and_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_i64_fetch_and(ptr, arg) mmconc_atomic_i64_fetch_and_explicit(ptr, arg, mmconc_memory_order_seq_cst)
#define mmconc_atomic_u64_fetch_and(ptr, arg) mmconc_atomic_u64_fetch_and_explicit(ptr, arg, mmconc_memory_order_seq_cst)

MEME_API int MEME_STDCALL mmconc_atomic_flag_base_test_and_set_explicit(volatile mmconc_atomic_flag_t* _obj, mmconc_memory_order_e _order);

MG_CAPI_INLINE int mmconc_atomic_flag_test_and_set_explicit(volatile mmconc_atomic_flag_t* _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_flag_test_and_set_explicit: _obj is NULL");
    return mmconc_atomic_flag_base_test_and_set_explicit(_obj, _order);
}

#define mmconc_atomic_flag_test_and_set(ptr) \
    mmconc_atomic_flag_test_and_set_explicit(ptr, mmconc_memory_order_seq_cst)

MEME_API void MEME_STDCALL mmconc_atomic_flag_base_clear_explicit(
    volatile mmconc_atomic_flag_t* _obj, mmconc_memory_order_e _order);

MG_CAPI_INLINE void mmconc_atomic_flag_clear_explicit(volatile mmconc_atomic_flag_t* _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_flag_clear_explicit: _obj is NULL");
    mmconc_atomic_flag_base_clear_explicit(_obj, _order);
}

#define mmconc_atomic_flag_clear(ptr) \
    mmconc_atomic_flag_clear_explicit(ptr, mmconc_memory_order_seq_cst)



#if INTPTR_MAX == INT64_MAX
#  define mmconc_atomic_iptr_init(obj,desired) mmconc_atomic_i64_init(obj,sizeof(mmconc_atomic_int64_t),desired)
#  define mmconc_atomic_iptr_is_lock_free mmconc_atomic_i64_is_lock_free
#  define mmconc_atomic_iptr_store_explicit mmconc_atomic_i64_store_explicit
#  define mmconc_atomic_iptr_load_explicit mmconc_atomic_i64_load_explicit
#  define mmconc_atomic_iptr_exchange_explicit mmconc_atomic_i64_exchange_explicit
#  define mmconc_atomic_iptr_compare_exchange_strong_explicit mmconc_atomic_i64_compare_exchange_strong_explicit
#  define mmconc_atomic_iptr_compare_exchange_weak_explicit mmconc_atomic_i64_compare_exchange_weak_explicit
#  define mmconc_atomic_iptr_fetch_add_explicit mmconc_atomic_i64_fetch_add_explicit
#  define mmconc_atomic_iptr_fetch_sub_explicit mmconc_atomic_i64_fetch_sub_explicit
#  define mmconc_atomic_iptr_fetch_or_explicit mmconc_atomic_i64_fetch_or_explicit
#  define mmconc_atomic_iptr_fetch_xor_explicit mmconc_atomic_i64_fetch_xor_explicit
#  define mmconc_atomic_iptr_fetch_and_explicit mmconc_atomic_i64_fetch_and_explicit
#  define mmconc_atomic_iptr_store mmconc_atomic_i64_store
#  define mmconc_atomic_iptr_load mmconc_atomic_i64_load
#  define mmconc_atomic_iptr_exchange mmconc_atomic_i64_exchange
#  define mmconc_atomic_iptr_compare_exchange_strong mmconc_atomic_i64_compare_exchange_strong
#  define mmconc_atomic_iptr_compare_exchange_weak mmconc_atomic_i64_compare_exchange_weak
#  define mmconc_atomic_iptr_fetch_add mmconc_atomic_i64_fetch_add
#  define mmconc_atomic_iptr_fetch_sub mmconc_atomic_i64_fetch_sub
#  define mmconc_atomic_iptr_fetch_or mmconc_atomic_i64_fetch_or
#  define mmconc_atomic_iptr_fetch_xor mmconc_atomic_i64_fetch_xor
#  define mmconc_atomic_iptr_fetch_and mmconc_atomic_i64_fetch_and

#  define mmconc_atomic_uptr_init(obj,desired) mmconc_atomic_u64_init(obj,sizeof(mmconc_atomic_uint64_t),desired)
#  define mmconc_atomic_uptr_is_lock_free mmconc_atomic_u64_is_lock_free
#  define mmconc_atomic_uptr_store_explicit mmconc_atomic_u64_store_explicit
#  define mmconc_atomic_uptr_load_explicit mmconc_atomic_u64_load_explicit
#  define mmconc_atomic_uptr_exchange_explicit mmconc_atomic_u64_exchange_explicit
#  define mmconc_atomic_uptr_compare_exchange_strong_explicit mmconc_atomic_u64_compare_exchange_strong_explicit
#  define mmconc_atomic_uptr_compare_exchange_weak_explicit mmconc_atomic_u64_compare_exchange_weak_explicit
#  define mmconc_atomic_uptr_fetch_add_explicit mmconc_atomic_u64_fetch_add_explicit
#  define mmconc_atomic_uptr_fetch_sub_explicit mmconc_atomic_u64_fetch_sub_explicit
#  define mmconc_atomic_uptr_fetch_or_explicit mmconc_atomic_u64_fetch_or_explicit
#  define mmconc_atomic_uptr_fetch_xor_explicit mmconc_atomic_u64_fetch_xor_explicit
#  define mmconc_atomic_uptr_fetch_and_explicit mmconc_atomic_u64_fetch_and_explicit
#  define mmconc_atomic_uptr_store mmconc_atomic_u64_store
#  define mmconc_atomic_uptr_load mmconc_atomic_u64_load
#  define mmconc_atomic_uptr_exchange mmconc_atomic_u64_exchange
#  define mmconc_atomic_uptr_compare_exchange_strong mmconc_atomic_u64_compare_exchange_strong
#  define mmconc_atomic_uptr_compare_exchange_weak mmconc_atomic_u64_compare_exchange_weak
#  define mmconc_atomic_uptr_fetch_add mmconc_atomic_u64_fetch_add
#  define mmconc_atomic_uptr_fetch_sub mmconc_atomic_u64_fetch_sub
#  define mmconc_atomic_uptr_fetch_or mmconc_atomic_u64_fetch_or
#  define mmconc_atomic_uptr_fetch_xor mmconc_atomic_u64_fetch_xor
#  define mmconc_atomic_uptr_fetch_and mmconc_atomic_u64_fetch_and

#  define mmconc_atomic_size_init(obj,desired) mmconc_atomic_u64_init(obj,sizeof(mmconc_atomic_uint64_t),desired)
#  define mmconc_atomic_size_is_lock_free mmconc_atomic_u64_is_lock_free
#  define mmconc_atomic_size_store_explicit mmconc_atomic_u64_store_explicit
#  define mmconc_atomic_size_load_explicit mmconc_atomic_u64_load_explicit
#  define mmconc_atomic_size_exchange_explicit mmconc_atomic_u64_exchange_explicit
#  define mmconc_atomic_size_compare_exchange_strong_explicit mmconc_atomic_u64_compare_exchange_strong_explicit
#  define mmconc_atomic_size_compare_exchange_weak_explicit mmconc_atomic_u64_compare_exchange_weak_explicit
#  define mmconc_atomic_size_fetch_add_explicit mmconc_atomic_u64_fetch_add_explicit
#  define mmconc_atomic_size_fetch_sub_explicit mmconc_atomic_u64_fetch_sub_explicit
#  define mmconc_atomic_size_fetch_or_explicit mmconc_atomic_u64_fetch_or_explicit
#  define mmconc_atomic_size_fetch_xor_explicit mmconc_atomic_u64_fetch_xor_explicit
#  define mmconc_atomic_size_fetch_and_explicit mmconc_atomic_u64_fetch_and_explicit
#  define mmconc_atomic_size_store mmconc_atomic_u64_store
#  define mmconc_atomic_size_load mmconc_atomic_u64_load
#  define mmconc_atomic_size_exchange mmconc_atomic_u64_exchange
#  define mmconc_atomic_size_compare_exchange_strong mmconc_atomic_u64_compare_exchange_strong
#  define mmconc_atomic_size_compare_exchange_weak mmconc_atomic_u64_compare_exchange_weak
#  define mmconc_atomic_size_fetch_add mmconc_atomic_u64_fetch_add
#  define mmconc_atomic_size_fetch_sub mmconc_atomic_u64_fetch_sub
#  define mmconc_atomic_size_fetch_or mmconc_atomic_u64_fetch_or
#  define mmconc_atomic_size_fetch_xor mmconc_atomic_u64_fetch_xor
#  define mmconc_atomic_size_fetch_and mmconc_atomic_u64_fetch_and

#elif INTPTR_MAX == INT32_MAX
#  define mmconc_atomic_iptr_init(obj,desired) mmconc_atomic_i32_init(obj,sizeof(mmconc_atomic_int32_t),desired)
#  define mmconc_atomic_iptr_is_lock_free mmconc_atomic_i32_is_lock_free
#  define mmconc_atomic_iptr_store_explicit mmconc_atomic_i32_store_explicit
#  define mmconc_atomic_iptr_load_explicit mmconc_atomic_i32_load_explicit
#  define mmconc_atomic_iptr_exchange_explicit mmconc_atomic_i32_exchange_explicit
#  define mmconc_atomic_iptr_compare_exchange_strong_explicit mmconc_atomic_i32_compare_exchange_strong_explicit
#  define mmconc_atomic_iptr_compare_exchange_weak_explicit mmconc_atomic_i32_compare_exchange_weak_explicit
#  define mmconc_atomic_iptr_fetch_add_explicit mmconc_atomic_i32_fetch_add_explicit
#  define mmconc_atomic_iptr_fetch_sub_explicit mmconc_atomic_i32_fetch_sub_explicit
#  define mmconc_atomic_iptr_fetch_or_explicit mmconc_atomic_i32_fetch_or_explicit
#  define mmconc_atomic_iptr_fetch_xor_explicit mmconc_atomic_i32_fetch_xor_explicit
#  define mmconc_atomic_iptr_fetch_and_explicit mmconc_atomic_i32_fetch_and_explicit
#  define mmconc_atomic_iptr_store mmconc_atomic_i32_store
#  define mmconc_atomic_iptr_load mmconc_atomic_i32_load
#  define mmconc_atomic_iptr_exchange mmconc_atomic_i32_exchange
#  define mmconc_atomic_iptr_compare_exchange_strong mmconc_atomic_i32_compare_exchange_strong
#  define mmconc_atomic_iptr_compare_exchange_weak mmconc_atomic_i32_compare_exchange_weak
#  define mmconc_atomic_iptr_fetch_add mmconc_atomic_i32_fetch_add
#  define mmconc_atomic_iptr_fetch_sub mmconc_atomic_i32_fetch_sub
#  define mmconc_atomic_iptr_fetch_or mmconc_atomic_i32_fetch_or
#  define mmconc_atomic_iptr_fetch_xor mmconc_atomic_i32_fetch_xor
#  define mmconc_atomic_iptr_fetch_and mmconc_atomic_i32_fetch_and

#  define mmconc_atomic_uptr_init(obj,desired) mmconc_atomic_u32_init(obj,sizeof(mmconc_atomic_uint32_t),desired)
#  define mmconc_atomic_uptr_is_lock_free mmconc_atomic_u32_is_lock_free
#  define mmconc_atomic_uptr_store_explicit mmconc_atomic_u32_store_explicit
#  define mmconc_atomic_uptr_load_explicit mmconc_atomic_u32_load_explicit
#  define mmconc_atomic_uptr_exchange_explicit mmconc_atomic_u32_exchange_explicit
#  define mmconc_atomic_uptr_compare_exchange_strong_explicit mmconc_atomic_u32_compare_exchange_strong_explicit
#  define mmconc_atomic_uptr_compare_exchange_weak_explicit mmconc_atomic_u32_compare_exchange_weak_explicit
#  define mmconc_atomic_uptr_fetch_add_explicit mmconc_atomic_u32_fetch_add_explicit
#  define mmconc_atomic_uptr_fetch_sub_explicit mmconc_atomic_u32_fetch_sub_explicit
#  define mmconc_atomic_uptr_fetch_or_explicit mmconc_atomic_u32_fetch_or_explicit
#  define mmconc_atomic_uptr_fetch_xor_explicit mmconc_atomic_u32_fetch_xor_explicit
#  define mmconc_atomic_uptr_fetch_and_explicit mmconc_atomic_u32_fetch_and_explicit
#  define mmconc_atomic_uptr_store mmconc_atomic_u32_store
#  define mmconc_atomic_uptr_load mmconc_atomic_u32_load
#  define mmconc_atomic_uptr_exchange mmconc_atomic_u32_exchange
#  define mmconc_atomic_uptr_compare_exchange_strong mmconc_atomic_u32_compare_exchange_strong
#  define mmconc_atomic_uptr_compare_exchange_weak mmconc_atomic_u32_compare_exchange_weak
#  define mmconc_atomic_uptr_fetch_add mmconc_atomic_u32_fetch_add
#  define mmconc_atomic_uptr_fetch_sub mmconc_atomic_u32_fetch_sub
#  define mmconc_atomic_uptr_fetch_or mmconc_atomic_u32_fetch_or
#  define mmconc_atomic_uptr_fetch_xor mmconc_atomic_u32_fetch_xor
#  define mmconc_atomic_uptr_fetch_and mmconc_atomic_u32_fetch_and

#  define mmconc_atomic_size_init(obj,desired) mmconc_atomic_u32_init(obj,sizeof(mmconc_atomic_uint32_t),desired)
#  define mmconc_atomic_size_is_lock_free mmconc_atomic_u32_is_lock_free
#  define mmconc_atomic_size_store_explicit mmconc_atomic_u32_store_explicit
#  define mmconc_atomic_size_load_explicit mmconc_atomic_u32_load_explicit
#  define mmconc_atomic_size_exchange_explicit mmconc_atomic_u32_exchange_explicit
#  define mmconc_atomic_size_compare_exchange_strong_explicit mmconc_atomic_u32_compare_exchange_strong_explicit
#  define mmconc_atomic_size_compare_exchange_weak_explicit mmconc_atomic_u32_compare_exchange_weak_explicit
#  define mmconc_atomic_size_fetch_add_explicit mmconc_atomic_u32_fetch_add_explicit
#  define mmconc_atomic_size_fetch_sub_explicit mmconc_atomic_u32_fetch_sub_explicit
#  define mmconc_atomic_size_fetch_or_explicit mmconc_atomic_u32_fetch_or_explicit
#  define mmconc_atomic_size_fetch_xor_explicit mmconc_atomic_u32_fetch_xor_explicit
#  define mmconc_atomic_size_fetch_and_explicit mmconc_atomic_u32_fetch_and_explicit
#  define mmconc_atomic_size_store mmconc_atomic_u32_store
#  define mmconc_atomic_size_load mmconc_atomic_u32_load
#  define mmconc_atomic_size_exchange mmconc_atomic_u32_exchange
#  define mmconc_atomic_size_compare_exchange_strong mmconc_atomic_u32_compare_exchange_strong
#  define mmconc_atomic_size_compare_exchange_weak mmconc_atomic_u32_compare_exchange_weak
#  define mmconc_atomic_size_fetch_add mmconc_atomic_u32_fetch_add
#  define mmconc_atomic_size_fetch_sub mmconc_atomic_u32_fetch_sub
#  define mmconc_atomic_size_fetch_or mmconc_atomic_u32_fetch_or
#  define mmconc_atomic_size_fetch_xor mmconc_atomic_u32_fetch_xor
#  define mmconc_atomic_size_fetch_and mmconc_atomic_u32_fetch_and

#else
#  error "Unsupported platform"
#endif


MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_CONC_ATOMIC_H_INCLUDED
