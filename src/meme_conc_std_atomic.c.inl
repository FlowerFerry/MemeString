
#include <meme/impl/conc/std_atomic.h>
#include <meme/common.h>
#include <stdatomic.h>
#include <assert.h>

MEME_EXTERN_C_SCOPE_START

MEME_API void MEME_STDCALL 
    mmconc_atomic_bl_base_init(volatile mmconc_atomic_bool_t* _obj, int _obj_size, int _desired)
{
    assert(_obj != NULL && "mmconc_atomic_bl_base_init: _obj is NULL");

    _obj->base.type = mmconc_atomic_bool_type;
    atomic_init(&(((volatile mmconc_std_atmc_bl_t*)_obj)->data), (bool)_desired);
}

MEME_API void MEME_STDCALL 
    mmconc_atomic_i8_base_init(volatile mmconc_atomic_int8_t* _obj, int _obj_size, int8_t _desired)
{
    assert(_obj != NULL && "mmconc_atomic_i8_base_init: _obj is NULL");

    _obj->base.type = mmconc_atomic_int8_type;
    atomic_init(&(((volatile mmconc_std_atmc_i8_t*)_obj)->data), _desired);
}

MEME_API void MEME_STDCALL 
    mmconc_atomic_u8_base_init(volatile mmconc_atomic_uint8_t* _obj, int _obj_size, uint8_t _desired)
{
    assert(_obj != NULL && "mmconc_atomic_u8_base_init: _obj is NULL");

    _obj->base.type = mmconc_atomic_uint8_type;
    atomic_init(&(((volatile mmconc_std_atmc_u8_t*)_obj)->data), _desired);
}

MEME_API void MEME_STDCALL 
    mmconc_atomic_i16_base_init(volatile mmconc_atomic_int16_t* _obj, int _obj_size, int16_t _desired)
{
    assert(_obj != NULL && "mmconc_atomic_i16_base_init: _obj is NULL");

    _obj->base.type = mmconc_atomic_int16_type;
    atomic_init(&(((volatile mmconc_std_atmc_i16_t*)_obj)->data), _desired);
}

MEME_API void MEME_STDCALL 
    mmconc_atomic_u16_base_init(volatile mmconc_atomic_uint16_t* _obj, int _obj_size, uint16_t _desired)
{
    assert(_obj != NULL && "mmconc_atomic_u16_base_init: _obj is NULL");

    _obj->base.type = mmconc_atomic_uint16_type;
    atomic_init(&(((volatile mmconc_std_atmc_u16_t*)_obj)->data), _desired);
}

MEME_API void MEME_STDCALL 
    mmconc_atomic_i32_base_init(volatile mmconc_atomic_int32_t* _obj, int _obj_size, int32_t _desired)
{
    assert(_obj != NULL && "mmconc_atomic_i32_base_init: _obj is NULL");

    _obj->base.type = mmconc_atomic_int32_type;
    atomic_init(&(((volatile mmconc_std_atmc_i32_t*)_obj)->data), _desired);
}

MEME_API void MEME_STDCALL 
    mmconc_atomic_u32_base_init(volatile mmconc_atomic_uint32_t* _obj, int _obj_size, uint32_t _desired)
{
    assert(_obj != NULL && "mmconc_atomic_u32_base_init: _obj is NULL");

    _obj->base.type = mmconc_atomic_uint32_type;
    atomic_init(&(((volatile mmconc_std_atmc_u32_t*)_obj)->data), _desired);
}

MEME_API void MEME_STDCALL 
    mmconc_atomic_i64_base_init(volatile mmconc_atomic_int64_t* _obj, int _obj_size, int64_t _desired)
{
    assert(_obj != NULL && "mmconc_atomic_i64_base_init: _obj is NULL");

    _obj->base.type = mmconc_atomic_int64_type;
    atomic_init(&(((volatile mmconc_std_atmc_i64_t*)_obj)->data), _desired);
}

MEME_API void MEME_STDCALL 
    mmconc_atomic_u64_base_init(volatile mmconc_atomic_uint64_t* _obj, int _obj_size, uint64_t _desired)
{
    assert(_obj != NULL && "mmconc_atomic_u64_base_init: _obj is NULL");

    _obj->base.type = mmconc_atomic_uint64_type;
    atomic_init(&(((volatile mmconc_std_atmc_u64_t*)_obj)->data), _desired);
}

MEME_API void MEME_STDCALL 
    mmconc_atomic_flag_base_init(volatile mmconc_atomic_flag_t* _obj, int _obj_size)
{
    assert(_obj != NULL && "mmconc_atomic_flag_base_init: _obj is NULL");

    _obj->base.type = mmconc_atomic_flag_type;
    atomic_flag_clear(&(((volatile mmconc_std_atmc_flag_t*)_obj)->data));
}


MEME_API int MEME_STDCALL mmconc_atomic_bl_base_is_lock_free (const volatile mmconc_atomic_bool_t  * _obj)
{
    assert(_obj != NULL && "mmconc_atomic_bl_base_is_lock_free: _obj is NULL");
    return atomic_is_lock_free(&(((const volatile mmconc_std_atmc_bl_t*)_obj)->data));
}

MEME_API int MEME_STDCALL mmconc_atomic_i8_base_is_lock_free (const volatile mmconc_atomic_int8_t  * _obj)
{
    assert(_obj != NULL && "mmconc_atomic_i8_base_is_lock_free: _obj is NULL");
    return atomic_is_lock_free(&(((const volatile mmconc_std_atmc_i8_t*)_obj)->data));
}

MEME_API int MEME_STDCALL mmconc_atomic_u8_base_is_lock_free (const volatile mmconc_atomic_uint8_t * _obj)
{
    assert(_obj != NULL && "mmconc_atomic_u8_base_is_lock_free: _obj is NULL");
    return atomic_is_lock_free(&(((const volatile mmconc_std_atmc_u8_t*)_obj)->data));
}

MEME_API int MEME_STDCALL mmconc_atomic_i16_base_is_lock_free(const volatile mmconc_atomic_int16_t * _obj)
{
    assert(_obj != NULL && "mmconc_atomic_i16_base_is_lock_free: _obj is NULL");
    return atomic_is_lock_free(&(((const volatile mmconc_std_atmc_i16_t*)_obj)->data));
}

MEME_API int MEME_STDCALL mmconc_atomic_u16_base_is_lock_free(const volatile mmconc_atomic_uint16_t* _obj)
{
    assert(_obj != NULL && "mmconc_atomic_u16_base_is_lock_free: _obj is NULL");
    return atomic_is_lock_free(&(((const volatile mmconc_std_atmc_u16_t*)_obj)->data));
}

MEME_API int MEME_STDCALL mmconc_atomic_i32_base_is_lock_free(const volatile mmconc_atomic_int32_t * _obj)
{
    assert(_obj != NULL && "mmconc_atomic_i32_base_is_lock_free: _obj is NULL");
    return atomic_is_lock_free(&(((const volatile mmconc_std_atmc_i32_t*)_obj)->data));
}

MEME_API int MEME_STDCALL mmconc_atomic_u32_base_is_lock_free(const volatile mmconc_atomic_uint32_t* _obj)
{
    assert(_obj != NULL && "mmconc_atomic_u32_base_is_lock_free: _obj is NULL");
    return atomic_is_lock_free(&(((const volatile mmconc_std_atmc_u32_t*)_obj)->data));
}

MEME_API int MEME_STDCALL mmconc_atomic_i64_base_is_lock_free(const volatile mmconc_atomic_int64_t * _obj)
{
    assert(_obj != NULL && "mmconc_atomic_i64_base_is_lock_free: _obj is NULL");
    return atomic_is_lock_free(&(((const volatile mmconc_std_atmc_i64_t*)_obj)->data));
}

MEME_API int MEME_STDCALL mmconc_atomic_u64_base_is_lock_free(const volatile mmconc_atomic_uint64_t* _obj)
{
    assert(_obj != NULL && "mmconc_atomic_u64_base_is_lock_free: _obj is NULL");
    return atomic_is_lock_free(&(((const volatile mmconc_std_atmc_u64_t*)_obj)->data));
}

MEME_API void MEME_STDCALL mmconc_atomic_bl_base_store_explicit (volatile mmconc_atomic_bool_t*   _obj, int      _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_bl_base_store_explicit: _obj is NULL");
    atomic_store_explicit(
        &(((volatile mmconc_std_atmc_bl_t*)_obj)->data), (bool)_desired, mmconc_memory_order_to_std_order(_order));
}

MEME_API void MEME_STDCALL mmconc_atomic_i8_base_store_explicit (volatile mmconc_atomic_int8_t*   _obj, int8_t   _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i8_base_store_explicit: _obj is NULL");
    atomic_store_explicit(
        &(((volatile mmconc_std_atmc_i8_t*)_obj)->data), _desired, mmconc_memory_order_to_std_order(_order));
}

MEME_API void MEME_STDCALL mmconc_atomic_u8_base_store_explicit (volatile mmconc_atomic_uint8_t*  _obj, uint8_t  _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u8_base_store_explicit: _obj is NULL");
    atomic_store_explicit(
        &(((volatile mmconc_std_atmc_u8_t*)_obj)->data), _desired, mmconc_memory_order_to_std_order(_order));
}

MEME_API void MEME_STDCALL mmconc_atomic_i16_base_store_explicit(volatile mmconc_atomic_int16_t*  _obj, int16_t  _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i16_base_store_explicit: _obj is NULL");
    atomic_store_explicit(
        &(((volatile mmconc_std_atmc_i16_t*)_obj)->data), _desired, mmconc_memory_order_to_std_order(_order));
}

MEME_API void MEME_STDCALL mmconc_atomic_u16_base_store_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u16_base_store_explicit: _obj is NULL");
    atomic_store_explicit(
        &(((volatile mmconc_std_atmc_u16_t*)_obj)->data), _desired, mmconc_memory_order_to_std_order(_order));
}

MEME_API void MEME_STDCALL mmconc_atomic_i32_base_store_explicit(volatile mmconc_atomic_int32_t*  _obj, int32_t  _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i32_base_store_explicit: _obj is NULL");
    atomic_store_explicit(
        &(((volatile mmconc_std_atmc_i32_t*)_obj)->data), _desired, mmconc_memory_order_to_std_order(_order));
}

MEME_API void MEME_STDCALL mmconc_atomic_u32_base_store_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u32_base_store_explicit: _obj is NULL");
    atomic_store_explicit(
        &(((volatile mmconc_std_atmc_u32_t*)_obj)->data), _desired, mmconc_memory_order_to_std_order(_order));
}

MEME_API void MEME_STDCALL mmconc_atomic_i64_base_store_explicit(volatile mmconc_atomic_int64_t*  _obj, int64_t  _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i64_base_store_explicit: _obj is NULL");
    atomic_store_explicit(
        &(((volatile mmconc_std_atmc_i64_t*)_obj)->data), _desired, mmconc_memory_order_to_std_order(_order));
}

MEME_API void MEME_STDCALL mmconc_atomic_u64_base_store_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u64_base_store_explicit: _obj is NULL");
    atomic_store_explicit(
        &(((volatile mmconc_std_atmc_u64_t*)_obj)->data), _desired, mmconc_memory_order_to_std_order(_order));
}

MEME_API int 
MEME_STDCALL mmconc_atomic_bl_base_load_explicit (const volatile mmconc_atomic_bool_t  * _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_bl_base_load_explicit: _obj is NULL");
    return atomic_load_explicit(
        &(((const volatile mmconc_std_atmc_bl_t*)_obj)->data), mmconc_memory_order_to_std_order(_order));
}

MEME_API int8_t
MEME_STDCALL mmconc_atomic_i8_base_load_explicit (const volatile mmconc_atomic_int8_t  * _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i8_base_load_explicit: _obj is NULL");
    return atomic_load_explicit(
        &(((const volatile mmconc_std_atmc_i8_t*)_obj)->data), mmconc_memory_order_to_std_order(_order));
}

MEME_API uint8_t
MEME_STDCALL mmconc_atomic_u8_base_load_explicit (const volatile mmconc_atomic_uint8_t * _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u8_base_load_explicit: _obj is NULL");
    return atomic_load_explicit(
        &(((const volatile mmconc_std_atmc_u8_t*)_obj)->data), mmconc_memory_order_to_std_order(_order));
}

MEME_API int16_t
MEME_STDCALL mmconc_atomic_i16_base_load_explicit(const volatile mmconc_atomic_int16_t * _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i16_base_load_explicit: _obj is NULL");
    return atomic_load_explicit(
        &(((const volatile mmconc_std_atmc_i16_t*)_obj)->data), mmconc_memory_order_to_std_order(_order));
}

MEME_API uint16_t
MEME_STDCALL mmconc_atomic_u16_base_load_explicit(const volatile mmconc_atomic_uint16_t* _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u16_base_load_explicit: _obj is NULL");
    return atomic_load_explicit(
        &(((const volatile mmconc_std_atmc_u16_t*)_obj)->data), mmconc_memory_order_to_std_order(_order));
}

MEME_API int32_t
MEME_STDCALL mmconc_atomic_i32_base_load_explicit(const volatile mmconc_atomic_int32_t * _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i32_base_load_explicit: _obj is NULL");
    return atomic_load_explicit(
        &(((const volatile mmconc_std_atmc_i32_t*)_obj)->data), mmconc_memory_order_to_std_order(_order));
}

MEME_API uint32_t
MEME_STDCALL mmconc_atomic_u32_base_load_explicit(const volatile mmconc_atomic_uint32_t* _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u32_base_load_explicit: _obj is NULL");
    return atomic_load_explicit(
        &(((const volatile mmconc_std_atmc_u32_t*)_obj)->data), mmconc_memory_order_to_std_order(_order));
}

MEME_API int64_t
MEME_STDCALL mmconc_atomic_i64_base_load_explicit(const volatile mmconc_atomic_int64_t * _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i64_base_load_explicit: _obj is NULL");
    return atomic_load_explicit(
        &(((const volatile mmconc_std_atmc_i64_t*)_obj)->data), mmconc_memory_order_to_std_order(_order));
}

MEME_API uint64_t
MEME_STDCALL mmconc_atomic_u64_base_load_explicit(const volatile mmconc_atomic_uint64_t* _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u64_base_load_explicit: _obj is NULL");
    return atomic_load_explicit(
        &(((const volatile mmconc_std_atmc_u64_t*)_obj)->data), mmconc_memory_order_to_std_order(_order));
}

MEME_API int 
MEME_STDCALL mmconc_atomic_bl_base_exchange_explicit(
    volatile mmconc_atomic_bool_t* _obj, int _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_bl_base_exchange_explicit: _obj is NULL");
    return atomic_exchange_explicit(
        &(((volatile mmconc_std_atmc_bl_t*)_obj)->data), (bool)_desired, mmconc_memory_order_to_std_order(_order));
}

MEME_API int8_t 
MEME_STDCALL mmconc_atomic_i8_base_exchange_explicit(
    volatile mmconc_atomic_int8_t* _obj, int8_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i8_base_exchange_explicit: _obj is NULL");
    return atomic_exchange_explicit(
        &(((volatile mmconc_std_atmc_i8_t*)_obj)->data), _desired, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint8_t 
MEME_STDCALL mmconc_atomic_u8_base_exchange_explicit(
    volatile mmconc_atomic_uint8_t* _obj, uint8_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u8_base_exchange_explicit: _obj is NULL");
    return atomic_exchange_explicit(
        &(((volatile mmconc_std_atmc_u8_t*)_obj)->data), _desired, mmconc_memory_order_to_std_order(_order));
}

MEME_API int16_t 
MEME_STDCALL mmconc_atomic_i16_base_exchange_explicit(
    volatile mmconc_atomic_int16_t* _obj, int16_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i16_base_exchange_explicit: _obj is NULL");
    return atomic_exchange_explicit(
        &(((volatile mmconc_std_atmc_i16_t*)_obj)->data), _desired, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint16_t 
MEME_STDCALL mmconc_atomic_u16_base_exchange_explicit(
    volatile mmconc_atomic_uint16_t* _obj, uint16_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u16_base_exchange_explicit: _obj is NULL");
    return atomic_exchange_explicit(
        &(((volatile mmconc_std_atmc_u16_t*)_obj)->data), _desired, mmconc_memory_order_to_std_order(_order));
}

MEME_API int32_t 
MEME_STDCALL mmconc_atomic_i32_base_exchange_explicit(
    volatile mmconc_atomic_int32_t* _obj, int32_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i32_base_exchange_explicit: _obj is NULL");
    return atomic_exchange_explicit(
        &(((volatile mmconc_std_atmc_i32_t*)_obj)->data), _desired, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint32_t 
MEME_STDCALL mmconc_atomic_u32_base_exchange_explicit(
    volatile mmconc_atomic_uint32_t* _obj, uint32_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u32_base_exchange_explicit: _obj is NULL");
    return atomic_exchange_explicit(
        &(((volatile mmconc_std_atmc_u32_t*)_obj)->data), _desired, mmconc_memory_order_to_std_order(_order));
}

MEME_API int64_t 
MEME_STDCALL mmconc_atomic_i64_base_exchange_explicit(
    volatile mmconc_atomic_int64_t* _obj, int64_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i64_base_exchange_explicit: _obj is NULL");
    return atomic_exchange_explicit(
        &(((volatile mmconc_std_atmc_i64_t*)_obj)->data), _desired, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint64_t 
MEME_STDCALL mmconc_atomic_u64_base_exchange_explicit(
    volatile mmconc_atomic_uint64_t* _obj, uint64_t _desired, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u64_base_exchange_explicit: _obj is NULL");
    return atomic_exchange_explicit(
        &(((volatile mmconc_std_atmc_u64_t*)_obj)->data), _desired, mmconc_memory_order_to_std_order(_order));
}

MEME_API int MEME_STDCALL mmconc_atomic_bl_base_compare_exchange_strong_explicit(
    volatile mmconc_atomic_bool_t* _obj, int* _expected, int _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_bl_base_compare_exchange_strong_explicit: _obj is NULL");

    bool expected = (bool)*_expected;
    int result = atomic_compare_exchange_strong_explicit(
        &(((volatile mmconc_std_atmc_bl_t*)_obj)->data), &expected, (bool)_desired, 
        mmconc_memory_order_to_std_order(_success), mmconc_memory_order_to_std_order(_failure));
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_i8_base_compare_exchange_strong_explicit(
    volatile mmconc_atomic_int8_t* _obj, int8_t* _expected, int8_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_i8_base_compare_exchange_strong_explicit: _obj is NULL");

    int8_t expected = *_expected;
    int result = atomic_compare_exchange_strong_explicit(
        &(((volatile mmconc_std_atmc_i8_t*)_obj)->data), &expected, _desired, 
        mmconc_memory_order_to_std_order(_success), mmconc_memory_order_to_std_order(_failure));
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_u8_base_compare_exchange_strong_explicit(
    volatile mmconc_atomic_uint8_t* _obj, uint8_t* _expected, uint8_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_u8_base_compare_exchange_strong_explicit: _obj is NULL");

    uint8_t expected = *_expected;
    int result = atomic_compare_exchange_strong_explicit(
        &(((volatile mmconc_std_atmc_u8_t*)_obj)->data), &expected, _desired, 
        mmconc_memory_order_to_std_order(_success), mmconc_memory_order_to_std_order(_failure));
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_i16_base_compare_exchange_strong_explicit(
    volatile mmconc_atomic_int16_t* _obj, int16_t* _expected, int16_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_i16_base_compare_exchange_strong_explicit: _obj is NULL");

    int16_t expected = *_expected;
    int result = atomic_compare_exchange_strong_explicit(
        &(((volatile mmconc_std_atmc_i16_t*)_obj)->data), &expected, _desired, 
        mmconc_memory_order_to_std_order(_success), mmconc_memory_order_to_std_order(_failure));
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_u16_base_compare_exchange_strong_explicit(
    volatile mmconc_atomic_uint16_t* _obj, uint16_t* _expected, uint16_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_u16_base_compare_exchange_strong_explicit: _obj is NULL");

    uint16_t expected = *_expected;
    int result = atomic_compare_exchange_strong_explicit(
        &(((volatile mmconc_std_atmc_u16_t*)_obj)->data), &expected, _desired, 
        mmconc_memory_order_to_std_order(_success), mmconc_memory_order_to_std_order(_failure));
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_i32_base_compare_exchange_strong_explicit(
    volatile mmconc_atomic_int32_t* _obj, int32_t* _expected, int32_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_i32_base_compare_exchange_strong_explicit: _obj is NULL");

    int32_t expected = *_expected;
    int result = atomic_compare_exchange_strong_explicit(
        &(((volatile mmconc_std_atmc_i32_t*)_obj)->data), &expected, _desired, 
        mmconc_memory_order_to_std_order(_success), mmconc_memory_order_to_std_order(_failure));
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_u32_base_compare_exchange_strong_explicit(
    volatile mmconc_atomic_uint32_t* _obj, uint32_t* _expected, uint32_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_u32_base_compare_exchange_strong_explicit: _obj is NULL");

    uint32_t expected = *_expected;
    int result = atomic_compare_exchange_strong_explicit(
        &(((volatile mmconc_std_atmc_u32_t*)_obj)->data), &expected, _desired, 
        mmconc_memory_order_to_std_order(_success), mmconc_memory_order_to_std_order(_failure));
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_i64_base_compare_exchange_strong_explicit(
    volatile mmconc_atomic_int64_t* _obj, int64_t* _expected, int64_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_i64_base_compare_exchange_strong_explicit: _obj is NULL");

    int64_t expected = *_expected;
    int result = atomic_compare_exchange_strong_explicit(
        &(((volatile mmconc_std_atmc_i64_t*)_obj)->data), &expected, _desired, 
        mmconc_memory_order_to_std_order(_success), mmconc_memory_order_to_std_order(_failure));
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_u64_base_compare_exchange_strong_explicit(
    volatile mmconc_atomic_uint64_t* _obj, uint64_t* _expected, uint64_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_u64_base_compare_exchange_strong_explicit: _obj is NULL");

    uint64_t expected = *_expected;
    int result = atomic_compare_exchange_strong_explicit(
        &(((volatile mmconc_std_atmc_u64_t*)_obj)->data), &expected, _desired, 
        mmconc_memory_order_to_std_order(_success), mmconc_memory_order_to_std_order(_failure));
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_bl_base_compare_exchange_weak_explicit(
    volatile mmconc_atomic_bool_t* _obj, int* _expected, int _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_bl_base_compare_exchange_weak_explicit: _obj is NULL");

    bool expected = (bool)*_expected;
    int result = atomic_compare_exchange_weak_explicit(
        &(((volatile mmconc_std_atmc_bl_t*)_obj)->data), &expected, (bool)_desired, 
        mmconc_memory_order_to_std_order(_success), mmconc_memory_order_to_std_order(_failure));
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_i8_base_compare_exchange_weak_explicit(
    volatile mmconc_atomic_int8_t* _obj, int8_t* _expected, int8_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_i8_base_compare_exchange_weak_explicit: _obj is NULL");

    int8_t expected = *_expected;
    int result = atomic_compare_exchange_weak_explicit(
        &(((volatile mmconc_std_atmc_i8_t*)_obj)->data), &expected, _desired, 
        mmconc_memory_order_to_std_order(_success), mmconc_memory_order_to_std_order(_failure));
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_u8_base_compare_exchange_weak_explicit(
    volatile mmconc_atomic_uint8_t* _obj, uint8_t* _expected, uint8_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_u8_base_compare_exchange_weak_explicit: _obj is NULL");

    uint8_t expected = *_expected;
    int result = atomic_compare_exchange_weak_explicit(
        &(((volatile mmconc_std_atmc_u8_t*)_obj)->data), &expected, _desired, 
        mmconc_memory_order_to_std_order(_success), mmconc_memory_order_to_std_order(_failure));
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_i16_base_compare_exchange_weak_explicit(
    volatile mmconc_atomic_int16_t* _obj, int16_t* _expected, int16_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_i16_base_compare_exchange_weak_explicit: _obj is NULL");

    int16_t expected = *_expected;
    int result = atomic_compare_exchange_weak_explicit(
        &(((volatile mmconc_std_atmc_i16_t*)_obj)->data), &expected, _desired, 
        mmconc_memory_order_to_std_order(_success), mmconc_memory_order_to_std_order(_failure));
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_u16_base_compare_exchange_weak_explicit(
    volatile mmconc_atomic_uint16_t* _obj, uint16_t* _expected, uint16_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_u16_base_compare_exchange_weak_explicit: _obj is NULL");

    uint16_t expected = *_expected;
    int result = atomic_compare_exchange_weak_explicit(
        &(((volatile mmconc_std_atmc_u16_t*)_obj)->data), &expected, _desired, 
        mmconc_memory_order_to_std_order(_success), mmconc_memory_order_to_std_order(_failure));
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_i32_base_compare_exchange_weak_explicit(
    volatile mmconc_atomic_int32_t* _obj, int32_t* _expected, int32_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_i32_base_compare_exchange_weak_explicit: _obj is NULL");

    int32_t expected = *_expected;
    int result = atomic_compare_exchange_weak_explicit(
        &(((volatile mmconc_std_atmc_i32_t*)_obj)->data), &expected, _desired, 
        mmconc_memory_order_to_std_order(_success), mmconc_memory_order_to_std_order(_failure));
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_u32_base_compare_exchange_weak_explicit(
    volatile mmconc_atomic_uint32_t* _obj, uint32_t* _expected, uint32_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_u32_base_compare_exchange_weak_explicit: _obj is NULL");

    uint32_t expected = *_expected;
    int result = atomic_compare_exchange_weak_explicit(
        &(((volatile mmconc_std_atmc_u32_t*)_obj)->data), &expected, _desired, 
        mmconc_memory_order_to_std_order(_success), mmconc_memory_order_to_std_order(_failure));
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_i64_base_compare_exchange_weak_explicit(
    volatile mmconc_atomic_int64_t* _obj, int64_t* _expected, int64_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_i64_base_compare_exchange_weak_explicit: _obj is NULL");

    int64_t expected = *_expected;
    int result = atomic_compare_exchange_weak_explicit(
        &(((volatile mmconc_std_atmc_i64_t*)_obj)->data), &expected, _desired, 
        mmconc_memory_order_to_std_order(_success), mmconc_memory_order_to_std_order(_failure));
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_u64_base_compare_exchange_weak_explicit(
    volatile mmconc_atomic_uint64_t* _obj, uint64_t* _expected, uint64_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    assert(_obj != NULL && "mmconc_atomic_u64_base_compare_exchange_weak_explicit: _obj is NULL");

    uint64_t expected = *_expected;
    int result = atomic_compare_exchange_weak_explicit(
        &(((volatile mmconc_std_atmc_u64_t*)_obj)->data), &expected, _desired, 
        mmconc_memory_order_to_std_order(_success), mmconc_memory_order_to_std_order(_failure));
    *_expected = expected;
    return result;
}

MEME_API int8_t 
MEME_STDCALL mmconc_atomic_i8_base_fetch_add_explicit (volatile mmconc_atomic_int8_t*  _obj, int8_t  _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i8_base_fetch_add_explicit: _obj is NULL");
    return atomic_fetch_add_explicit(
        &(((volatile mmconc_std_atmc_i8_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint8_t
MEME_STDCALL mmconc_atomic_u8_base_fetch_add_explicit (volatile mmconc_atomic_uint8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u8_base_fetch_add_explicit: _obj is NULL");
    return atomic_fetch_add_explicit(
        &(((volatile mmconc_std_atmc_u8_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API int16_t
MEME_STDCALL mmconc_atomic_i16_base_fetch_add_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i16_base_fetch_add_explicit: _obj is NULL");
    return atomic_fetch_add_explicit(
        &(((volatile mmconc_std_atmc_i16_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint16_t
MEME_STDCALL mmconc_atomic_u16_base_fetch_add_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u16_base_fetch_add_explicit: _obj is NULL");
    return atomic_fetch_add_explicit(
        &(((volatile mmconc_std_atmc_u16_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API int32_t
MEME_STDCALL mmconc_atomic_i32_base_fetch_add_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i32_base_fetch_add_explicit: _obj is NULL");
    return atomic_fetch_add_explicit(
        &(((volatile mmconc_std_atmc_i32_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint32_t
MEME_STDCALL mmconc_atomic_u32_base_fetch_add_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u32_base_fetch_add_explicit: _obj is NULL");
    return atomic_fetch_add_explicit(
        &(((volatile mmconc_std_atmc_u32_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API int64_t
MEME_STDCALL mmconc_atomic_i64_base_fetch_add_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i64_base_fetch_add_explicit: _obj is NULL");
    return atomic_fetch_add_explicit(
        &(((volatile mmconc_std_atmc_i64_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint64_t
MEME_STDCALL mmconc_atomic_u64_base_fetch_add_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u64_base_fetch_add_explicit: _obj is NULL");
    return atomic_fetch_add_explicit(
        &(((volatile mmconc_std_atmc_u64_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API int8_t
MEME_STDCALL mmconc_atomic_i8_base_fetch_sub_explicit (volatile mmconc_atomic_int8_t*  _obj, int8_t  _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i8_base_fetch_sub_explicit: _obj is NULL");
    return atomic_fetch_sub_explicit(
        &(((volatile mmconc_std_atmc_i8_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint8_t
MEME_STDCALL mmconc_atomic_u8_base_fetch_sub_explicit (volatile mmconc_atomic_uint8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u8_base_fetch_sub_explicit: _obj is NULL");
    return atomic_fetch_sub_explicit(
        &(((volatile mmconc_std_atmc_u8_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API int16_t
MEME_STDCALL mmconc_atomic_i16_base_fetch_sub_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i16_base_fetch_sub_explicit: _obj is NULL");
    return atomic_fetch_sub_explicit(
        &(((volatile mmconc_std_atmc_i16_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint16_t
MEME_STDCALL mmconc_atomic_u16_base_fetch_sub_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u16_base_fetch_sub_explicit: _obj is NULL");
    return atomic_fetch_sub_explicit(
        &(((volatile mmconc_std_atmc_u16_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API int32_t
MEME_STDCALL mmconc_atomic_i32_base_fetch_sub_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i32_base_fetch_sub_explicit: _obj is NULL");
    return atomic_fetch_sub_explicit(
        &(((volatile mmconc_std_atmc_i32_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint32_t
MEME_STDCALL mmconc_atomic_u32_base_fetch_sub_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u32_base_fetch_sub_explicit: _obj is NULL");
    return atomic_fetch_sub_explicit(
        &(((volatile mmconc_std_atmc_u32_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API int64_t
MEME_STDCALL mmconc_atomic_i64_base_fetch_sub_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i64_base_fetch_sub_explicit: _obj is NULL");
    return atomic_fetch_sub_explicit(
        &(((volatile mmconc_std_atmc_i64_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint64_t
MEME_STDCALL mmconc_atomic_u64_base_fetch_sub_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u64_base_fetch_sub_explicit: _obj is NULL");
    return atomic_fetch_sub_explicit(
        &(((volatile mmconc_std_atmc_u64_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API int8_t
MEME_STDCALL mmconc_atomic_i8_base_fetch_or_explicit (volatile mmconc_atomic_int8_t*  _obj, int8_t  _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i8_base_fetch_or_explicit: _obj is NULL");
    return atomic_fetch_or_explicit(
        &(((volatile mmconc_std_atmc_i8_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint8_t
MEME_STDCALL mmconc_atomic_u8_base_fetch_or_explicit (volatile mmconc_atomic_uint8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u8_base_fetch_or_explicit: _obj is NULL");
    return atomic_fetch_or_explicit(
        &(((volatile mmconc_std_atmc_u8_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API int16_t
MEME_STDCALL mmconc_atomic_i16_base_fetch_or_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i16_base_fetch_or_explicit: _obj is NULL");
    return atomic_fetch_or_explicit(
        &(((volatile mmconc_std_atmc_i16_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}
MEME_API uint16_t
MEME_STDCALL mmconc_atomic_u16_base_fetch_or_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u16_base_fetch_or_explicit: _obj is NULL");
    return atomic_fetch_or_explicit(
        &(((volatile mmconc_std_atmc_u16_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API int32_t
MEME_STDCALL mmconc_atomic_i32_base_fetch_or_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i32_base_fetch_or_explicit: _obj is NULL");
    return atomic_fetch_or_explicit(
        &(((volatile mmconc_std_atmc_i32_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint32_t
MEME_STDCALL mmconc_atomic_u32_base_fetch_or_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u32_base_fetch_or_explicit: _obj is NULL");
    return atomic_fetch_or_explicit(
        &(((volatile mmconc_std_atmc_u32_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API int64_t
MEME_STDCALL mmconc_atomic_i64_base_fetch_or_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i64_base_fetch_or_explicit: _obj is NULL");
    return atomic_fetch_or_explicit(
        &(((volatile mmconc_std_atmc_i64_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint64_t
MEME_STDCALL mmconc_atomic_u64_base_fetch_or_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u64_base_fetch_or_explicit: _obj is NULL");
    return atomic_fetch_or_explicit(
        &(((volatile mmconc_std_atmc_u64_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API int8_t
MEME_STDCALL mmconc_atomic_i8_base_fetch_xor_explicit (volatile mmconc_atomic_int8_t*  _obj, int8_t  _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i8_base_fetch_xor_explicit: _obj is NULL");
    return atomic_fetch_xor_explicit(
        &(((volatile mmconc_std_atmc_i8_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint8_t
MEME_STDCALL mmconc_atomic_u8_base_fetch_xor_explicit (volatile mmconc_atomic_uint8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u8_base_fetch_xor_explicit: _obj is NULL");
    return atomic_fetch_xor_explicit(
        &(((volatile mmconc_std_atmc_u8_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API int16_t
MEME_STDCALL mmconc_atomic_i16_base_fetch_xor_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i16_base_fetch_xor_explicit: _obj is NULL");
    return atomic_fetch_xor_explicit(
        &(((volatile mmconc_std_atmc_i16_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint16_t
MEME_STDCALL mmconc_atomic_u16_base_fetch_xor_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u16_base_fetch_xor_explicit: _obj is NULL");
    return atomic_fetch_xor_explicit(
        &(((volatile mmconc_std_atmc_u16_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API int32_t
MEME_STDCALL mmconc_atomic_i32_base_fetch_xor_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i32_base_fetch_xor_explicit: _obj is NULL");
    return atomic_fetch_xor_explicit(
        &(((volatile mmconc_std_atmc_i32_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint32_t
MEME_STDCALL mmconc_atomic_u32_base_fetch_xor_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u32_base_fetch_xor_explicit: _obj is NULL");
    return atomic_fetch_xor_explicit(
        &(((volatile mmconc_std_atmc_u32_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API int64_t
MEME_STDCALL mmconc_atomic_i64_base_fetch_xor_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i64_base_fetch_xor_explicit: _obj is NULL");
    return atomic_fetch_xor_explicit(
        &(((volatile mmconc_std_atmc_i64_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint64_t
MEME_STDCALL mmconc_atomic_u64_base_fetch_xor_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u64_base_fetch_xor_explicit: _obj is NULL");
    return atomic_fetch_xor_explicit(
        &(((volatile mmconc_std_atmc_u64_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API int8_t
MEME_STDCALL mmconc_atomic_i8_base_fetch_and_explicit (volatile mmconc_atomic_int8_t*  _obj, int8_t  _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i8_base_fetch_and_explicit: _obj is NULL");
    return atomic_fetch_and_explicit(
        &(((volatile mmconc_std_atmc_i8_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint8_t
MEME_STDCALL mmconc_atomic_u8_base_fetch_and_explicit (volatile mmconc_atomic_uint8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u8_base_fetch_and_explicit: _obj is NULL");
    return atomic_fetch_and_explicit(
        &(((volatile mmconc_std_atmc_u8_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API int16_t
MEME_STDCALL mmconc_atomic_i16_base_fetch_and_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i16_base_fetch_and_explicit: _obj is NULL");
    return atomic_fetch_and_explicit(
        &(((volatile mmconc_std_atmc_i16_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint16_t
MEME_STDCALL mmconc_atomic_u16_base_fetch_and_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u16_base_fetch_and_explicit: _obj is NULL");
    return atomic_fetch_and_explicit(
        &(((volatile mmconc_std_atmc_u16_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API int32_t
MEME_STDCALL mmconc_atomic_i32_base_fetch_and_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i32_base_fetch_and_explicit: _obj is NULL");
    return atomic_fetch_and_explicit(
        &(((volatile mmconc_std_atmc_i32_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint32_t
MEME_STDCALL mmconc_atomic_u32_base_fetch_and_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u32_base_fetch_and_explicit: _obj is NULL");
    return atomic_fetch_and_explicit(
        &(((volatile mmconc_std_atmc_u32_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API int64_t
MEME_STDCALL mmconc_atomic_i64_base_fetch_and_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_i64_base_fetch_and_explicit: _obj is NULL");
    return atomic_fetch_and_explicit(
        &(((volatile mmconc_std_atmc_i64_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API uint64_t
MEME_STDCALL mmconc_atomic_u64_base_fetch_and_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_u64_base_fetch_and_explicit: _obj is NULL");
    return atomic_fetch_and_explicit(
        &(((volatile mmconc_std_atmc_u64_t*)_obj)->data), _arg, mmconc_memory_order_to_std_order(_order));
}

MEME_API int MEME_STDCALL mmconc_atomic_flag_base_test_and_set_explicit(
    volatile mmconc_atomic_flag_t* _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_flag_base_test_and_set_explicit: _obj is NULL");
    return atomic_flag_test_and_set_explicit(
        &(((volatile mmconc_std_atmc_flag_t*)_obj)->data), mmconc_memory_order_to_std_order(_order));
}

MEME_API void MEME_STDCALL mmconc_atomic_flag_base_clear_explicit(
    volatile mmconc_atomic_flag_t* _obj, mmconc_memory_order_e _order)
{
    assert(_obj != NULL && "mmconc_atomic_flag_base_clear_explicit: _obj is NULL");
    atomic_flag_clear_explicit(
        &(((volatile mmconc_std_atmc_flag_t*)_obj)->data), mmconc_memory_order_to_std_order(_order));
}



MEME_EXTERN_C_SCOPE_ENDED
