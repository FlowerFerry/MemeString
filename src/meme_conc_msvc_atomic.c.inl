
#include <meme/impl/conc/msvc_atomic.h>
#include <meme/common.h>

MEME_EXTERN_C_SCOPE_START

MEME_API void MEME_STDCALL 
    mmconc_atomic_impl_bl_init(volatile mmconc_atomic_bool_t* _obj, int _obj_size, int _desired)
{
    _obj->base.type = mmconc_atomic_bool_type;
    mmconc_msvc_atmc_bl_init(&(((volatile mmconc_native_atmc_bl_t*)_obj)->data), (bool)_desired);
}

MEME_API void MEME_STDCALL 
    mmconc_atomic_impl_i8_init(volatile mmconc_atomic_int8_t* _obj, int _obj_size, int8_t _desired)
{
    _obj->base.type = mmconc_atomic_int8_type;
    mmconc_msvc_atmc_i8_init(&(((volatile mmconc_native_atmc_i8_t*)_obj)->data), _desired);
}

MEME_API void MEME_STDCALL 
    mmconc_atomic_impl_u8_init(volatile mmconc_atomic_uint8_t* _obj, int _obj_size, uint8_t _desired)
{
    _obj->base.type = mmconc_atomic_uint8_type;
    mmconc_msvc_atmc_u8_init(&(((volatile mmconc_native_atmc_u8_t*)_obj)->data), _desired);
}

MEME_API void MEME_STDCALL 
    mmconc_atomic_impl_i16_init(volatile mmconc_atomic_int16_t* _obj, int _obj_size, int16_t _desired)
{
    _obj->base.type = mmconc_atomic_int16_type;
    mmconc_msvc_atmc_i16_init(&(((volatile mmconc_native_atmc_i16_t*)_obj)->data), _desired);
}

MEME_API void MEME_STDCALL 
    mmconc_atomic_impl_u16_init(volatile mmconc_atomic_uint16_t* _obj, int _obj_size, uint16_t _desired)
{
    _obj->base.type = mmconc_atomic_uint16_type;
    mmconc_msvc_atmc_u16_init(&(((volatile mmconc_native_atmc_u16_t*)_obj)->data), _desired);
}

MEME_API void MEME_STDCALL 
    mmconc_atomic_impl_i32_init(volatile mmconc_atomic_int32_t* _obj, int _obj_size, int32_t _desired)
{
    _obj->base.type = mmconc_atomic_int32_type;
    mmconc_msvc_atmc_i32_init(&(((volatile mmconc_native_atmc_i32_t*)_obj)->data), _desired);
}

MEME_API void MEME_STDCALL 
    mmconc_atomic_impl_u32_init(volatile mmconc_atomic_uint32_t* _obj, int _obj_size, uint32_t _desired)
{
    _obj->base.type = mmconc_atomic_uint32_type;
    mmconc_msvc_atmc_u32_init(&(((volatile mmconc_native_atmc_u32_t*)_obj)->data), _desired);
}

MEME_API void MEME_STDCALL 
    mmconc_atomic_impl_i64_init(volatile mmconc_atomic_int64_t* _obj, int _obj_size, int64_t _desired)
{
    _obj->base.type = mmconc_atomic_int64_type;
    mmconc_msvc_atmc_i64_init(&(((volatile mmconc_native_atmc_i64_t*)_obj)->data), _desired);
}

MEME_API void MEME_STDCALL 
    mmconc_atomic_impl_u64_init(volatile mmconc_atomic_uint64_t* _obj, int _obj_size, uint64_t _desired)
{
    _obj->base.type = mmconc_atomic_uint64_type;
    mmconc_msvc_atmc_u64_init(&(((volatile mmconc_native_atmc_u64_t*)_obj)->data), _desired);
}

MEME_API void MEME_STDCALL 
    mmconc_atomic_impl_flag_init(volatile mmconc_atomic_flag_t* _obj, int _obj_size)
{
    _obj->base.type = mmconc_atomic_flag_type;
    mmconc_msvc_atmc_bl_init(&(((volatile mmconc_native_atmc_bl_t*)_obj)->data), false);
}


MEME_API int MEME_STDCALL mmconc_atomic_impl_bl_is_lock_free (const volatile mmconc_atomic_bool_t  * _obj)
{
    return mmconc_msvc_atmc_bl_is_lock_free(&(((const volatile mmconc_native_atmc_bl_t*)_obj)->data));
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_i8_is_lock_free (const volatile mmconc_atomic_int8_t  * _obj)
{
    return mmconc_msvc_atmc_i8_is_lock_free(&(((const volatile mmconc_native_atmc_i8_t*)_obj)->data));
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_u8_is_lock_free (const volatile mmconc_atomic_uint8_t * _obj)
{
    return mmconc_msvc_atmc_u8_is_lock_free(&(((const volatile mmconc_native_atmc_u8_t*)_obj)->data));
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_i16_is_lock_free(const volatile mmconc_atomic_int16_t * _obj)
{
    return mmconc_msvc_atmc_i16_is_lock_free(&(((const volatile mmconc_native_atmc_i16_t*)_obj)->data));
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_u16_is_lock_free(const volatile mmconc_atomic_uint16_t* _obj)
{
    return mmconc_msvc_atmc_u16_is_lock_free(&(((const volatile mmconc_native_atmc_u16_t*)_obj)->data));
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_i32_is_lock_free(const volatile mmconc_atomic_int32_t * _obj)
{
    return mmconc_msvc_atmc_i32_is_lock_free(&(((const volatile mmconc_native_atmc_i32_t*)_obj)->data));
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_u32_is_lock_free(const volatile mmconc_atomic_uint32_t* _obj)
{
    return mmconc_msvc_atmc_u32_is_lock_free(&(((const volatile mmconc_native_atmc_u32_t*)_obj)->data));
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_i64_is_lock_free(const volatile mmconc_atomic_int64_t * _obj)
{
    return mmconc_msvc_atmc_i64_is_lock_free(&(((const volatile mmconc_native_atmc_i64_t*)_obj)->data));
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_u64_is_lock_free(const volatile mmconc_atomic_uint64_t* _obj)
{
    return mmconc_msvc_atmc_u64_is_lock_free(&(((const volatile mmconc_native_atmc_u64_t*)_obj)->data));
}

MEME_API void MEME_STDCALL mmconc_atomic_impl_bl_store_explicit (volatile mmconc_atomic_bool_t*   _obj, int      _desired, mmconc_memory_order_e _order)
{
    mmconc_msvc_atmc_bl_store_explicit(&(((volatile mmconc_native_atmc_bl_t*)_obj)->data), _desired, _order);
}

MEME_API void MEME_STDCALL mmconc_atomic_impl_i8_store_explicit (volatile mmconc_atomic_int8_t*   _obj, int8_t   _desired, mmconc_memory_order_e _order)
{
    mmconc_msvc_atmc_i8_store_explicit(&(((volatile mmconc_native_atmc_i8_t*)_obj)->data), _desired, _order);
}

MEME_API void MEME_STDCALL mmconc_atomic_impl_u8_store_explicit (volatile mmconc_atomic_uint8_t*  _obj, uint8_t  _desired, mmconc_memory_order_e _order)
{
    mmconc_msvc_atmc_u8_store_explicit(&(((volatile mmconc_native_atmc_u8_t*)_obj)->data), _desired, _order);
}

MEME_API void MEME_STDCALL mmconc_atomic_impl_i16_store_explicit(volatile mmconc_atomic_int16_t*  _obj, int16_t  _desired, mmconc_memory_order_e _order)
{
    mmconc_msvc_atmc_i16_store_explicit(&(((volatile mmconc_native_atmc_i16_t*)_obj)->data), _desired, _order);
}

MEME_API void MEME_STDCALL mmconc_atomic_impl_u16_store_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _desired, mmconc_memory_order_e _order)
{
    mmconc_msvc_atmc_u16_store_explicit(&(((volatile mmconc_native_atmc_u16_t*)_obj)->data), _desired, _order);
}

MEME_API void MEME_STDCALL mmconc_atomic_impl_i32_store_explicit(volatile mmconc_atomic_int32_t*  _obj, int32_t  _desired, mmconc_memory_order_e _order)
{
    mmconc_msvc_atmc_i32_store_explicit(&(((volatile mmconc_native_atmc_i32_t*)_obj)->data), _desired, _order);
}

MEME_API void MEME_STDCALL mmconc_atomic_impl_u32_store_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _desired, mmconc_memory_order_e _order)
{
    mmconc_msvc_atmc_u32_store_explicit(&(((volatile mmconc_native_atmc_u32_t*)_obj)->data), _desired, _order);
}

MEME_API void MEME_STDCALL mmconc_atomic_impl_i64_store_explicit(volatile mmconc_atomic_int64_t*  _obj, int64_t  _desired, mmconc_memory_order_e _order)
{
    mmconc_msvc_atmc_i64_store_explicit(&(((volatile mmconc_native_atmc_i64_t*)_obj)->data), _desired, _order);
}

MEME_API void MEME_STDCALL mmconc_atomic_impl_u64_store_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _desired, mmconc_memory_order_e _order)
{
    mmconc_msvc_atmc_u64_store_explicit(&(((volatile mmconc_native_atmc_u64_t*)_obj)->data), _desired, _order);
}

MEME_API int 
MEME_STDCALL mmconc_atomic_impl_bl_load_explicit (const volatile mmconc_atomic_bool_t  * _obj, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_bl_load_explicit(&(((const volatile mmconc_native_atmc_bl_t*)_obj)->data), _order);
}

MEME_API int8_t
MEME_STDCALL mmconc_atomic_impl_i8_load_explicit (const volatile mmconc_atomic_int8_t  * _obj, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i8_load_explicit(&(((const volatile mmconc_native_atmc_i8_t*)_obj)->data), _order);
}

MEME_API uint8_t
MEME_STDCALL mmconc_atomic_impl_u8_load_explicit (const volatile mmconc_atomic_uint8_t * _obj, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u8_load_explicit(&(((const volatile mmconc_native_atmc_u8_t*)_obj)->data), _order);
}

MEME_API int16_t
MEME_STDCALL mmconc_atomic_impl_i16_load_explicit(const volatile mmconc_atomic_int16_t * _obj, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i16_load_explicit(&(((const volatile mmconc_native_atmc_i16_t*)_obj)->data), _order);
}

MEME_API uint16_t
MEME_STDCALL mmconc_atomic_impl_u16_load_explicit(const volatile mmconc_atomic_uint16_t* _obj, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u16_load_explicit(&(((const volatile mmconc_native_atmc_u16_t*)_obj)->data), _order);
}

MEME_API int32_t
MEME_STDCALL mmconc_atomic_impl_i32_load_explicit(const volatile mmconc_atomic_int32_t * _obj, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i32_load_explicit(&(((const volatile mmconc_native_atmc_i32_t*)_obj)->data), _order);
}

MEME_API uint32_t
MEME_STDCALL mmconc_atomic_impl_u32_load_explicit(const volatile mmconc_atomic_uint32_t* _obj, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u32_load_explicit(&(((const volatile mmconc_native_atmc_u32_t*)_obj)->data), _order);
}

MEME_API int64_t
MEME_STDCALL mmconc_atomic_impl_i64_load_explicit(const volatile mmconc_atomic_int64_t * _obj, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i64_load_explicit(&(((const volatile mmconc_native_atmc_i64_t*)_obj)->data), _order);
}

MEME_API uint64_t
MEME_STDCALL mmconc_atomic_impl_u64_load_explicit(const volatile mmconc_atomic_uint64_t* _obj, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u64_load_explicit(&(((const volatile mmconc_native_atmc_u64_t*)_obj)->data), _order);
}

MEME_API int 
MEME_STDCALL mmconc_atomic_impl_bl_exchange_explicit(
    volatile mmconc_atomic_bool_t* _obj, int _desired, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_bl_exchange_explicit(&(((volatile mmconc_native_atmc_bl_t*)_obj)->data), _desired, _order);
}

MEME_API int8_t 
MEME_STDCALL mmconc_atomic_impl_i8_exchange_explicit(
    volatile mmconc_atomic_int8_t* _obj, int8_t _desired, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i8_exchange_explicit(&(((volatile mmconc_native_atmc_i8_t*)_obj)->data), _desired, _order);
}

MEME_API uint8_t 
MEME_STDCALL mmconc_atomic_impl_u8_exchange_explicit(
    volatile mmconc_atomic_uint8_t* _obj, uint8_t _desired, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u8_exchange_explicit(&(((volatile mmconc_native_atmc_u8_t*)_obj)->data), _desired, _order);
}

MEME_API int16_t 
MEME_STDCALL mmconc_atomic_impl_i16_exchange_explicit(
    volatile mmconc_atomic_int16_t* _obj, int16_t _desired, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i16_exchange_explicit(&(((volatile mmconc_native_atmc_i16_t*)_obj)->data), _desired, _order);
}

MEME_API uint16_t 
MEME_STDCALL mmconc_atomic_impl_u16_exchange_explicit(
    volatile mmconc_atomic_uint16_t* _obj, uint16_t _desired, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u16_exchange_explicit(&(((volatile mmconc_native_atmc_u16_t*)_obj)->data), _desired, _order);
}

MEME_API int32_t 
MEME_STDCALL mmconc_atomic_impl_i32_exchange_explicit(
    volatile mmconc_atomic_int32_t* _obj, int32_t _desired, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i32_exchange_explicit(&(((volatile mmconc_native_atmc_i32_t*)_obj)->data), _desired, _order);
}

MEME_API uint32_t 
MEME_STDCALL mmconc_atomic_impl_u32_exchange_explicit(
    volatile mmconc_atomic_uint32_t* _obj, uint32_t _desired, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u32_exchange_explicit(&(((volatile mmconc_native_atmc_u32_t*)_obj)->data), _desired, _order);
}

MEME_API int64_t 
MEME_STDCALL mmconc_atomic_impl_i64_exchange_explicit(
    volatile mmconc_atomic_int64_t* _obj, int64_t _desired, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i64_exchange_explicit(&(((volatile mmconc_native_atmc_i64_t*)_obj)->data), _desired, _order);
}

MEME_API uint64_t 
MEME_STDCALL mmconc_atomic_impl_u64_exchange_explicit(
    volatile mmconc_atomic_uint64_t* _obj, uint64_t _desired, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u64_exchange_explicit(&(((volatile mmconc_native_atmc_u64_t*)_obj)->data), _desired, _order);
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_bl_compare_exchange_strong_explicit(
    volatile mmconc_atomic_bool_t* _obj, int* _expected, int _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    bool expected = (bool)*_expected;
    int result = mmconc_msvc_atmc_bl_compare_exchange_strong_explicit(
        &(((volatile mmconc_native_atmc_bl_t*)_obj)->data), &expected, (bool)_desired, _success, _failure);
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_i8_compare_exchange_strong_explicit(
    volatile mmconc_atomic_int8_t* _obj, int8_t* _expected, int8_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    int8_t expected = *_expected;
    int result = mmconc_msvc_atmc_i8_compare_exchange_strong_explicit(
        &(((volatile mmconc_native_atmc_i8_t*)_obj)->data), &expected, _desired, _success, _failure);
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_u8_compare_exchange_strong_explicit(
    volatile mmconc_atomic_uint8_t* _obj, uint8_t* _expected, uint8_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    uint8_t expected = *_expected;
    int result = mmconc_msvc_atmc_u8_compare_exchange_strong_explicit(
        &(((volatile mmconc_native_atmc_u8_t*)_obj)->data), &expected, _desired, _success, _failure);
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_i16_compare_exchange_strong_explicit(
    volatile mmconc_atomic_int16_t* _obj, int16_t* _expected, int16_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    int16_t expected = *_expected;
    int result = mmconc_msvc_atmc_i16_compare_exchange_strong_explicit(
        &(((volatile mmconc_native_atmc_i16_t*)_obj)->data), &expected, _desired, _success, _failure);
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_u16_compare_exchange_strong_explicit(
    volatile mmconc_atomic_uint16_t* _obj, uint16_t* _expected, uint16_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    uint16_t expected = *_expected;
    int result = mmconc_msvc_atmc_u16_compare_exchange_strong_explicit(
        &(((volatile mmconc_native_atmc_u16_t*)_obj)->data), &expected, _desired, _success, _failure);
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_i32_compare_exchange_strong_explicit(
    volatile mmconc_atomic_int32_t* _obj, int32_t* _expected, int32_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    int32_t expected = *_expected;
    int result = mmconc_msvc_atmc_i32_compare_exchange_strong_explicit(
        &(((volatile mmconc_native_atmc_i32_t*)_obj)->data), &expected, _desired, _success, _failure);
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_u32_compare_exchange_strong_explicit(
    volatile mmconc_atomic_uint32_t* _obj, uint32_t* _expected, uint32_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    uint32_t expected = *_expected;
    int result = mmconc_msvc_atmc_u32_compare_exchange_strong_explicit(
        &(((volatile mmconc_native_atmc_u32_t*)_obj)->data), &expected, _desired, _success, _failure);
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_i64_compare_exchange_strong_explicit(
    volatile mmconc_atomic_int64_t* _obj, int64_t* _expected, int64_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    int64_t expected = *_expected;
    int result = mmconc_msvc_atmc_i64_compare_exchange_strong_explicit(
        &(((volatile mmconc_native_atmc_i64_t*)_obj)->data), &expected, _desired, _success, _failure);
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_u64_compare_exchange_strong_explicit(
    volatile mmconc_atomic_uint64_t* _obj, uint64_t* _expected, uint64_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    uint64_t expected = *_expected;
    int result = mmconc_msvc_atmc_u64_compare_exchange_strong_explicit(
        &(((volatile mmconc_native_atmc_u64_t*)_obj)->data), &expected, _desired, _success, _failure);
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_bl_compare_exchange_weak_explicit(
    volatile mmconc_atomic_bool_t* _obj, int* _expected, int _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    bool expected = (bool)*_expected;
    int result = mmconc_msvc_atmc_bl_compare_exchange_weak_explicit(
        &(((volatile mmconc_native_atmc_bl_t*)_obj)->data), &expected, (bool)_desired, _success, _failure);
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_i8_compare_exchange_weak_explicit(
    volatile mmconc_atomic_int8_t* _obj, int8_t* _expected, int8_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    int8_t expected = *_expected;
    int result = mmconc_msvc_atmc_i8_compare_exchange_weak_explicit(
        &(((volatile mmconc_native_atmc_i8_t*)_obj)->data), &expected, _desired, _success, _failure);
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_u8_compare_exchange_weak_explicit(
    volatile mmconc_atomic_uint8_t* _obj, uint8_t* _expected, uint8_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    uint8_t expected = *_expected;
    int result = mmconc_msvc_atmc_u8_compare_exchange_weak_explicit(
        &(((volatile mmconc_native_atmc_u8_t*)_obj)->data), &expected, _desired, _success, _failure);
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_i16_compare_exchange_weak_explicit(
    volatile mmconc_atomic_int16_t* _obj, int16_t* _expected, int16_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    int16_t expected = *_expected;
    int result = mmconc_msvc_atmc_i16_compare_exchange_weak_explicit(
        &(((volatile mmconc_native_atmc_i16_t*)_obj)->data), &expected, _desired, _success, _failure);
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_u16_compare_exchange_weak_explicit(
    volatile mmconc_atomic_uint16_t* _obj, uint16_t* _expected, uint16_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    uint16_t expected = *_expected;
    int result = mmconc_msvc_atmc_u16_compare_exchange_weak_explicit(
        &(((volatile mmconc_native_atmc_u16_t*)_obj)->data), &expected, _desired, _success, _failure);
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_i32_compare_exchange_weak_explicit(
    volatile mmconc_atomic_int32_t* _obj, int32_t* _expected, int32_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    int32_t expected = *_expected;
    int result = mmconc_msvc_atmc_i32_compare_exchange_weak_explicit(
        &(((volatile mmconc_native_atmc_i32_t*)_obj)->data), &expected, _desired, _success, _failure);
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_u32_compare_exchange_weak_explicit(
    volatile mmconc_atomic_uint32_t* _obj, uint32_t* _expected, uint32_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    uint32_t expected = *_expected;
    int result = mmconc_msvc_atmc_u32_compare_exchange_weak_explicit(
        &(((volatile mmconc_native_atmc_u32_t*)_obj)->data), &expected, _desired, _success, _failure);
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_i64_compare_exchange_weak_explicit(
    volatile mmconc_atomic_int64_t* _obj, int64_t* _expected, int64_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    int64_t expected = *_expected;
    int result = mmconc_msvc_atmc_i64_compare_exchange_weak_explicit(
        &(((volatile mmconc_native_atmc_i64_t*)_obj)->data), &expected, _desired, _success, _failure);
    *_expected = expected;
    return result;
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_u64_compare_exchange_weak_explicit(
    volatile mmconc_atomic_uint64_t* _obj, uint64_t* _expected, uint64_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    uint64_t expected = *_expected;
    int result = mmconc_msvc_atmc_u64_compare_exchange_weak_explicit(
        &(((volatile mmconc_native_atmc_u64_t*)_obj)->data), &expected, _desired, _success, _failure);
    *_expected = expected;
    return result;
}

MEME_API int8_t 
MEME_STDCALL mmconc_atomic_impl_i8_fetch_add_explicit (volatile mmconc_atomic_int8_t*  _obj, int8_t  _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i8_fetch_add_explicit(&(((volatile mmconc_native_atmc_i8_t*)_obj)->data), _arg, _order);
}

MEME_API uint8_t
MEME_STDCALL mmconc_atomic_impl_u8_fetch_add_explicit (volatile mmconc_atomic_uint8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u8_fetch_add_explicit(&(((volatile mmconc_native_atmc_u8_t*)_obj)->data), _arg, _order);
}

MEME_API int16_t
MEME_STDCALL mmconc_atomic_impl_i16_fetch_add_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i16_fetch_add_explicit(&(((volatile mmconc_native_atmc_i16_t*)_obj)->data), _arg, _order);
}

MEME_API uint16_t
MEME_STDCALL mmconc_atomic_impl_u16_fetch_add_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u16_fetch_add_explicit(&(((volatile mmconc_native_atmc_u16_t*)_obj)->data), _arg, _order);
}

MEME_API int32_t
MEME_STDCALL mmconc_atomic_impl_i32_fetch_add_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i32_fetch_add_explicit(&(((volatile mmconc_native_atmc_i32_t*)_obj)->data), _arg, _order);
}

MEME_API uint32_t
MEME_STDCALL mmconc_atomic_impl_u32_fetch_add_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u32_fetch_add_explicit(&(((volatile mmconc_native_atmc_u32_t*)_obj)->data), _arg, _order);
}

MEME_API int64_t
MEME_STDCALL mmconc_atomic_impl_i64_fetch_add_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i64_fetch_add_explicit(&(((volatile mmconc_native_atmc_i64_t*)_obj)->data), _arg, _order);
}

MEME_API uint64_t
MEME_STDCALL mmconc_atomic_impl_u64_fetch_add_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u64_fetch_add_explicit(&(((volatile mmconc_native_atmc_u64_t*)_obj)->data), _arg, _order);
}

MEME_API int8_t
MEME_STDCALL mmconc_atomic_impl_i8_fetch_sub_explicit (volatile mmconc_atomic_int8_t*  _obj, int8_t  _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i8_fetch_sub_explicit(&(((volatile mmconc_native_atmc_i8_t*)_obj)->data), _arg, _order);
}

MEME_API uint8_t
MEME_STDCALL mmconc_atomic_impl_u8_fetch_sub_explicit (volatile mmconc_atomic_uint8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u8_fetch_sub_explicit(&(((volatile mmconc_native_atmc_u8_t*)_obj)->data), _arg, _order);
}

MEME_API int16_t
MEME_STDCALL mmconc_atomic_impl_i16_fetch_sub_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i16_fetch_sub_explicit(&(((volatile mmconc_native_atmc_i16_t*)_obj)->data), _arg, _order);
}

MEME_API uint16_t
MEME_STDCALL mmconc_atomic_impl_u16_fetch_sub_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u16_fetch_sub_explicit(&(((volatile mmconc_native_atmc_u16_t*)_obj)->data), _arg, _order);
}

MEME_API int32_t
MEME_STDCALL mmconc_atomic_impl_i32_fetch_sub_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i32_fetch_sub_explicit(&(((volatile mmconc_native_atmc_i32_t*)_obj)->data), _arg, _order);
}

MEME_API uint32_t
MEME_STDCALL mmconc_atomic_impl_u32_fetch_sub_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u32_fetch_sub_explicit(&(((volatile mmconc_native_atmc_u32_t*)_obj)->data), _arg, _order);
}

MEME_API int64_t
MEME_STDCALL mmconc_atomic_impl_i64_fetch_sub_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i64_fetch_sub_explicit(&(((volatile mmconc_native_atmc_i64_t*)_obj)->data), _arg, _order);
}

MEME_API uint64_t
MEME_STDCALL mmconc_atomic_impl_u64_fetch_sub_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u64_fetch_sub_explicit(&(((volatile mmconc_native_atmc_u64_t*)_obj)->data), _arg, _order);
}

MEME_API int8_t
MEME_STDCALL mmconc_atomic_impl_i8_fetch_or_explicit (volatile mmconc_atomic_int8_t*  _obj, int8_t  _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i8_fetch_or_explicit(&(((volatile mmconc_native_atmc_i8_t*)_obj)->data), _arg, _order);
}

MEME_API uint8_t
MEME_STDCALL mmconc_atomic_impl_u8_fetch_or_explicit (volatile mmconc_atomic_uint8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u8_fetch_or_explicit(&(((volatile mmconc_native_atmc_u8_t*)_obj)->data), _arg, _order);
}

MEME_API int16_t
MEME_STDCALL mmconc_atomic_impl_i16_fetch_or_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i16_fetch_or_explicit(&(((volatile mmconc_native_atmc_i16_t*)_obj)->data), _arg, _order);
}

MEME_API uint16_t
MEME_STDCALL mmconc_atomic_impl_u16_fetch_or_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u16_fetch_or_explicit(&(((volatile mmconc_native_atmc_u16_t*)_obj)->data), _arg, _order);
}

MEME_API int32_t
MEME_STDCALL mmconc_atomic_impl_i32_fetch_or_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i32_fetch_or_explicit(&(((volatile mmconc_native_atmc_i32_t*)_obj)->data), _arg, _order);
}

MEME_API uint32_t
MEME_STDCALL mmconc_atomic_impl_u32_fetch_or_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u32_fetch_or_explicit(&(((volatile mmconc_native_atmc_u32_t*)_obj)->data), _arg, _order);
}

MEME_API int64_t
MEME_STDCALL mmconc_atomic_impl_i64_fetch_or_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i64_fetch_or_explicit(&(((volatile mmconc_native_atmc_i64_t*)_obj)->data), _arg, _order);
}

MEME_API uint64_t
MEME_STDCALL mmconc_atomic_impl_u64_fetch_or_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u64_fetch_or_explicit(&(((volatile mmconc_native_atmc_u64_t*)_obj)->data), _arg, _order);
}

MEME_API int8_t
MEME_STDCALL mmconc_atomic_impl_i8_fetch_xor_explicit (volatile mmconc_atomic_int8_t*  _obj, int8_t  _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i8_fetch_xor_explicit(&(((volatile mmconc_native_atmc_i8_t*)_obj)->data), _arg, _order);
}

MEME_API uint8_t
MEME_STDCALL mmconc_atomic_impl_u8_fetch_xor_explicit (volatile mmconc_atomic_uint8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u8_fetch_xor_explicit(&(((volatile mmconc_native_atmc_u8_t*)_obj)->data), _arg, _order);
}

MEME_API int16_t
MEME_STDCALL mmconc_atomic_impl_i16_fetch_xor_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i16_fetch_xor_explicit(&(((volatile mmconc_native_atmc_i16_t*)_obj)->data), _arg, _order);
}

MEME_API uint16_t
MEME_STDCALL mmconc_atomic_impl_u16_fetch_xor_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u16_fetch_xor_explicit(&(((volatile mmconc_native_atmc_u16_t*)_obj)->data), _arg, _order);
}

MEME_API int32_t
MEME_STDCALL mmconc_atomic_impl_i32_fetch_xor_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i32_fetch_xor_explicit(&(((volatile mmconc_native_atmc_i32_t*)_obj)->data), _arg, _order);
}

MEME_API uint32_t
MEME_STDCALL mmconc_atomic_impl_u32_fetch_xor_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u32_fetch_xor_explicit(&(((volatile mmconc_native_atmc_u32_t*)_obj)->data), _arg, _order);
}

MEME_API int64_t
MEME_STDCALL mmconc_atomic_impl_i64_fetch_xor_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i64_fetch_xor_explicit(&(((volatile mmconc_native_atmc_i64_t*)_obj)->data), _arg, _order);
}

MEME_API uint64_t
MEME_STDCALL mmconc_atomic_impl_u64_fetch_xor_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u64_fetch_xor_explicit(&(((volatile mmconc_native_atmc_u64_t*)_obj)->data), _arg, _order);
}

MEME_API int8_t
MEME_STDCALL mmconc_atomic_impl_i8_fetch_and_explicit (volatile mmconc_atomic_int8_t*  _obj, int8_t  _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i8_fetch_and_explicit(&(((volatile mmconc_native_atmc_i8_t*)_obj)->data), _arg, _order);
}

MEME_API uint8_t
MEME_STDCALL mmconc_atomic_impl_u8_fetch_and_explicit (volatile mmconc_atomic_uint8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u8_fetch_and_explicit(&(((volatile mmconc_native_atmc_u8_t*)_obj)->data), _arg, _order);
}

MEME_API int16_t
MEME_STDCALL mmconc_atomic_impl_i16_fetch_and_explicit(volatile mmconc_atomic_int16_t* _obj, int16_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i16_fetch_and_explicit(&(((volatile mmconc_native_atmc_i16_t*)_obj)->data), _arg, _order);
}

MEME_API uint16_t
MEME_STDCALL mmconc_atomic_impl_u16_fetch_and_explicit(volatile mmconc_atomic_uint16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u16_fetch_and_explicit(&(((volatile mmconc_native_atmc_u16_t*)_obj)->data), _arg, _order);
}

MEME_API int32_t
MEME_STDCALL mmconc_atomic_impl_i32_fetch_and_explicit(volatile mmconc_atomic_int32_t* _obj, int32_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i32_fetch_and_explicit(&(((volatile mmconc_native_atmc_i32_t*)_obj)->data), _arg, _order);
}

MEME_API uint32_t
MEME_STDCALL mmconc_atomic_impl_u32_fetch_and_explicit(volatile mmconc_atomic_uint32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u32_fetch_and_explicit(&(((volatile mmconc_native_atmc_u32_t*)_obj)->data), _arg, _order);
}

MEME_API int64_t
MEME_STDCALL mmconc_atomic_impl_i64_fetch_and_explicit(volatile mmconc_atomic_int64_t* _obj, int64_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i64_fetch_and_explicit(&(((volatile mmconc_native_atmc_i64_t*)_obj)->data), _arg, _order);
}

MEME_API uint64_t
MEME_STDCALL mmconc_atomic_impl_u64_fetch_and_explicit(volatile mmconc_atomic_uint64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_u64_fetch_and_explicit(&(((volatile mmconc_native_atmc_u64_t*)_obj)->data), _arg, _order);
}

MEME_API int MEME_STDCALL mmconc_atomic_impl_flag_test_and_set_explicit(
    volatile mmconc_atomic_flag_t* _obj, mmconc_memory_order_e _order)
{    
    bool o = false;
    return mmconc_msvc_atmc_bl_compare_exchange_strong_explicit(
        &(((volatile mmconc_native_atmc_bl_t*)_obj)->data), &o, true, _order, mmconc_memory_order_relaxed) ? 0 : 1;
}

MEME_API void MEME_STDCALL mmconc_atomic_impl_flag_clear_explicit(
    volatile mmconc_atomic_flag_t* _obj, mmconc_memory_order_e _order)
{
    mmconc_msvc_atmc_bl_store_explicit(&(((volatile mmconc_native_atmc_bl_t*)_obj)->data), false, _order);
}

MEME_EXTERN_C_SCOPE_ENDED
