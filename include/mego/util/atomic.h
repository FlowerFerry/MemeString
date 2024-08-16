
#ifndef MEGO_UTIL_ATOMIC_H_INCLUDED
#define MEGO_UTIL_ATOMIC_H_INCLUDED

#include "atomic_fwd.h"
#include <mego/predef/symbol/inline.h>
#include <mego/predef/lang/version.h>
#include <mego/util/detail/msvc_atomic.h>

#if MG_COMP__MSVC_AVAIL

// Padding bits should not participate in cmpxchg comparison starting in C++20.
// Clang does not have __builtin_zero_non_value_bits to exclude these bits to implement this C++20 feature.
// The EDG front-end substitutes everything and runs into incomplete types passed to atomic<T>.
//#if MG_LANG__CXX20_AVAIL && !defined(__clang__) /* TRANSITION, LLVM-46685 */ && !defined(__EDG__)
//#define __MGU_ATOMIC_CMPXCHG_MASK_OUT_PADDING_BITS 1
//#else
//#define __MGU_ATOMIC_CMPXCHG_MASK_OUT_PADDING_BITS 0
//#endif

//! @see winnt.h
//MG_CAPI_INLINE
//VOID
//WriteNoFence32(
//    _Out_ _Interlocked_operand_ LONG volatile* Destination,
//    _In_ LONG Value
//)
//{
//    WriteNoFence(Destination, Value);
//}

#ifdef __cplusplus

MG_CAPI_INLINE bool __mgu_atomic_is_lock_free(volatile mgu_atomic_bool* _obj)   { return __mgu_atomic_is_lock_free_b  (_obj); }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free(volatile mgu_atomic_char* _obj)   { return __mgu_atomic_is_lock_free_i8 (_obj); }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free(volatile mgu_atomic_uchar* _obj)  { return __mgu_atomic_is_lock_free_u8 (_obj); }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free(volatile mgu_atomic_short* _obj)  { return __mgu_atomic_is_lock_free_i16(_obj); }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free(volatile mgu_atomic_ushort* _obj) { return __mgu_atomic_is_lock_free_u16(_obj); }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free(volatile mgu_atomic_long* _obj)   { return __mgu_atomic_is_lock_free_i32(_obj); }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free(volatile mgu_atomic_ulong* _obj)  { return __mgu_atomic_is_lock_free_u32(_obj); }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free(volatile mgu_atomic_llong* _obj)  { return __mgu_atomic_is_lock_free_i64(_obj); }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free(volatile mgu_atomic_ullong* _obj) { return __mgu_atomic_is_lock_free_u64(_obj); }

#elif MG_LANG__C11_AVAIL

#define __mgu_atomic_is_lock_free(obj) \
_Generic((obj), \
    volatile mgu_atomic_bool*  : __mgu_atomic_is_lock_free_b,    \
    volatile mgu_atomic_char*  : __mgu_atomic_is_lock_free_i8,   \
    volatile mgu_atomic_uchar* : __mgu_atomic_is_lock_free_u8,   \
    volatile mgu_atomic_short* : __mgu_atomic_is_lock_free_i16,  \
    volatile mgu_atomic_ushort*: __mgu_atomic_is_lock_free_u16,  \
    volatile mgu_atomic_long*  : __mgu_atomic_is_lock_free_i32,  \
    volatile mgu_atomic_ulong* : __mgu_atomic_is_lock_free_u32,  \
    volatile mgu_atomic_llong* : __mgu_atomic_is_lock_free_i64,  \
    volatile mgu_atomic_ullong*: __mgu_atomic_is_lock_free_u64   \
)(obj)

#else

MG_CAPI_INLINE bool __mgu_atomic_is_lock_free(volatile void* _obj)
{
    volatile mgu_atomic_none* p = (volatile mgu_atomic_none*)_obj;
    switch (p->type_) {
    case mgu_atomic_bool_type  : return __mgu_atomic_is_lock_free_b  (&(((volatile mgu_atomic_bool*)_obj)->value_));
    case mgu_atomic_char_type  : return __mgu_atomic_is_lock_free_i8 (&(((volatile mgu_atomic_char*)_obj)->value_));
    case mgu_atomic_uchar_type : return __mgu_atomic_is_lock_free_u8 (&(((volatile mgu_atomic_uchar*)_obj)->value_));
    case mgu_atomic_short_type : return __mgu_atomic_is_lock_free_i16(&(((volatile mgu_atomic_short*)_obj)->value_));
    case mgu_atomic_ushort_type: return __mgu_atomic_is_lock_free_u16(&(((volatile mgu_atomic_ushort*)_obj)->value_));
    case mgu_atomic_long_type  : return __mgu_atomic_is_lock_free_i32(&(((volatile mgu_atomic_long*)_obj)->value_));
    case mgu_atomic_ulong_type : return __mgu_atomic_is_lock_free_u32(&(((volatile mgu_atomic_ulong*)_obj)->value_));
    case mgu_atomic_llong_type : return __mgu_atomic_is_lock_free_i64(&(((volatile mgu_atomic_llong*)_obj)->value_));
    case mgu_atomic_ullong_type: return __mgu_atomic_is_lock_free_u64(&(((volatile mgu_atomic_ullong*)_obj)->value_));
    default: 
        assert(false);
        return false;
    }
}

#endif

#define mgu_atomic_is_lock_free(obj) __mgu_atomic_is_lock_free(obj)


#if defined(__cplusplus) || MG_LANG__C11_AVAIL

MG_CAPI_INLINE void mgu_atomic_init_b  (volatile mgu_atomic_bool*   _obj, bool     _desired) { return __mgu_atomic_init_b  (_obj, _desired); }
MG_CAPI_INLINE void mgu_atomic_init_i8 (volatile mgu_atomic_char*   _obj, int8_t   _desired) { return __mgu_atomic_init_i8 (_obj, _desired); }
MG_CAPI_INLINE void mgu_atomic_init_u8 (volatile mgu_atomic_uchar*  _obj, uint8_t  _desired) { return __mgu_atomic_init_u8 (_obj, _desired); }
MG_CAPI_INLINE void mgu_atomic_init_i16(volatile mgu_atomic_short*  _obj, int16_t  _desired) { return __mgu_atomic_init_i16(_obj, _desired); }
MG_CAPI_INLINE void mgu_atomic_init_u16(volatile mgu_atomic_ushort* _obj, uint16_t _desired) { return __mgu_atomic_init_u16(_obj, _desired); }
MG_CAPI_INLINE void mgu_atomic_init_i32(volatile mgu_atomic_long*   _obj, int32_t  _desired) { return __mgu_atomic_init_i32(_obj, _desired); }
MG_CAPI_INLINE void mgu_atomic_init_u32(volatile mgu_atomic_ulong*  _obj, uint32_t _desired) { return __mgu_atomic_init_u32(_obj, _desired); }
MG_CAPI_INLINE void mgu_atomic_init_i64(volatile mgu_atomic_llong*  _obj, int64_t  _desired) { return __mgu_atomic_init_i64(_obj, _desired); }
MG_CAPI_INLINE void mgu_atomic_init_u64(volatile mgu_atomic_ullong* _obj, uint64_t _desired) { return __mgu_atomic_init_u64(_obj, _desired); }

#endif

#ifdef __cplusplus

MG_CAPI_INLINE void __mgu_atomic_init(volatile mgu_atomic_bool*   _obj, bool     _desired) { __mgu_atomic_init_b  (_obj, _desired); }
MG_CAPI_INLINE void __mgu_atomic_init(volatile mgu_atomic_char*   _obj, int8_t   _desired) { __mgu_atomic_init_i8 (_obj, _desired); }
MG_CAPI_INLINE void __mgu_atomic_init(volatile mgu_atomic_uchar*  _obj, uint8_t  _desired) { __mgu_atomic_init_u8 (_obj, _desired); }
MG_CAPI_INLINE void __mgu_atomic_init(volatile mgu_atomic_short*  _obj, int16_t  _desired) { __mgu_atomic_init_i16(_obj, _desired); }
MG_CAPI_INLINE void __mgu_atomic_init(volatile mgu_atomic_ushort* _obj, uint16_t _desired) { __mgu_atomic_init_u16(_obj, _desired); }
MG_CAPI_INLINE void __mgu_atomic_init(volatile mgu_atomic_long*   _obj, int32_t  _desired) { __mgu_atomic_init_i32(_obj, _desired); }
MG_CAPI_INLINE void __mgu_atomic_init(volatile mgu_atomic_ulong*  _obj, uint32_t _desired) { __mgu_atomic_init_u32(_obj, _desired); }
MG_CAPI_INLINE void __mgu_atomic_init(volatile mgu_atomic_llong*  _obj, int64_t  _desired) { __mgu_atomic_init_i64(_obj, _desired); }
MG_CAPI_INLINE void __mgu_atomic_init(volatile mgu_atomic_ullong* _obj, uint64_t _desired) { __mgu_atomic_init_u64(_obj, _desired); }

#define mgu_atomic_init(obj,desired) __mgu_atomic_init(obj,desired)

#elif MG_LANG__C11_AVAIL

#define __mgu_atomic_init(obj,desired)                   \
_Generic((obj),                                          \
    volatile mgu_atomic_bool*  : __mgu_atomic_init_b,    \
    volatile mgu_atomic_char*  : __mgu_atomic_init_i8,   \
    volatile mgu_atomic_uchar* : __mgu_atomic_init_u8,   \
    volatile mgu_atomic_short* : __mgu_atomic_init_i16,  \
    volatile mgu_atomic_ushort*: __mgu_atomic_init_u16,  \
    volatile mgu_atomic_long*  : __mgu_atomic_init_i32,  \
    volatile mgu_atomic_ulong* : __mgu_atomic_init_u32,  \
    volatile mgu_atomic_llong* : __mgu_atomic_init_i64,  \
    volatile mgu_atomic_ullong*: __mgu_atomic_init_u64   \
)(obj,desired)

#define mgu_atomic_init(obj,desired) __mgu_atomic_init(obj,desired)

#else

MG_CAPI_INLINE void mgu_atomic_init_b(volatile mgu_atomic_bool*   _obj, bool _desired) 
{ 
    _obj->type_ = mgu_atomic_bool_type;
    __mgu_atomic_init_b(&(_obj->value_), _desired);
}
MG_CAPI_INLINE void mgu_atomic_init_i8(volatile mgu_atomic_char* _obj, int8_t _desired)
{
    _obj->type_ = mgu_atomic_char_type;
    __mgu_atomic_init_i8(&(_obj->value_), _desired);
}
MG_CAPI_INLINE void mgu_atomic_init_u8(volatile mgu_atomic_uchar* _obj, uint8_t _desired)
{
    _obj->type_ = mgu_atomic_uchar_type;
    __mgu_atomic_init_u8(&(_obj->value_), _desired);
}
MG_CAPI_INLINE void mgu_atomic_init_i16(volatile mgu_atomic_short* _obj, int16_t _desired)
{
    _obj->type_ = mgu_atomic_short_type;
    __mgu_atomic_init_i16(&(_obj->value_), _desired);
}
MG_CAPI_INLINE void mgu_atomic_init_u16(volatile mgu_atomic_ushort* _obj, uint16_t _desired)
{
    _obj->type_ = mgu_atomic_ushort_type;
    __mgu_atomic_init_u16(&(_obj->value_), _desired);
}
MG_CAPI_INLINE void mgu_atomic_init_i32(volatile mgu_atomic_long* _obj, int32_t _desired)
{
    _obj->type_ = mgu_atomic_long_type;
    __mgu_atomic_init_i32(&(_obj->value_), _desired);
}
MG_CAPI_INLINE void mgu_atomic_init_u32(volatile mgu_atomic_ulong* _obj, uint32_t _desired)
{
    _obj->type_ = mgu_atomic_ulong_type;
    __mgu_atomic_init_u32(&(_obj->value_), _desired);
}
MG_CAPI_INLINE void mgu_atomic_init_i64(volatile mgu_atomic_llong* _obj, int64_t _desired)
{
    _obj->type_ = mgu_atomic_llong_type;
    __mgu_atomic_init_i64(&(_obj->value_), _desired);
}
MG_CAPI_INLINE void mgu_atomic_init_u64(volatile mgu_atomic_ullong* _obj, uint64_t _desired)
{
    _obj->type_ = mgu_atomic_ullong_type;
    __mgu_atomic_init_u64(&(_obj->value_), _desired);
}

#endif


#ifdef __cplusplus

MG_CAPI_INLINE bool __mgu_atomic_exchange_explicit(
    volatile mgu_atomic_bool* _obj, bool _desired, mgu_memory_order _order) 
{ 
    return __mgu_atomic_exchange_explicit_b(_obj, _desired, _order);
}
MG_CAPI_INLINE int8_t __mgu_atomic_exchange_explicit(
    volatile mgu_atomic_char* _obj, int8_t _desired, mgu_memory_order _order)
{
    return __mgu_atomic_exchange_explicit_i8(_obj, _desired, _order);
}
MG_CAPI_INLINE uint8_t __mgu_atomic_exchange_explicit(
    volatile mgu_atomic_uchar* _obj, uint8_t _desired, mgu_memory_order _order)
{
    return __mgu_atomic_exchange_explicit_u8(_obj, _desired, _order);
}
MG_CAPI_INLINE int16_t __mgu_atomic_exchange_explicit(
    volatile mgu_atomic_short* _obj, int16_t _desired, mgu_memory_order _order)
{
    return __mgu_atomic_exchange_explicit_i16(_obj, _desired, _order);
}
MG_CAPI_INLINE uint16_t __mgu_atomic_exchange_explicit(
    volatile mgu_atomic_ushort* _obj, uint16_t _desired, mgu_memory_order _order)
{
    return __mgu_atomic_exchange_explicit_u16(_obj, _desired, _order);
}
MG_CAPI_INLINE int32_t __mgu_atomic_exchange_explicit(
    volatile mgu_atomic_long* _obj, int32_t _desired, mgu_memory_order _order)
{
    return __mgu_atomic_exchange_explicit_i32(_obj, _desired, _order);
}
MG_CAPI_INLINE uint32_t __mgu_atomic_exchange_explicit(
    volatile mgu_atomic_ulong* _obj, uint32_t _desired, mgu_memory_order _order)
{
    return __mgu_atomic_exchange_explicit_u32(_obj, _desired, _order);
}
MG_CAPI_INLINE int64_t __mgu_atomic_exchange_explicit(
    volatile mgu_atomic_llong* _obj, int64_t _desired, mgu_memory_order _order)
{
    return __mgu_atomic_exchange_explicit_i64(_obj, _desired, _order);
}
MG_CAPI_INLINE uint64_t __mgu_atomic_exchange_explicit(
    volatile mgu_atomic_ullong* _obj, uint64_t _desired, mgu_memory_order _order)
{
    return __mgu_atomic_exchange_explicit_u64(_obj, _desired, _order);
}

#elif MG_LANG__C11_AVAIL

#define __mgu_atomic_exchange_explicit(obj,desired,order)   \
_Generic((obj),                                             \
    mgu_atomic_bool*  : __mgu_atomic_exchange_explicit_b,   \
    mgu_atomic_char*  : __mgu_atomic_exchange_explicit_i8,  \
    mgu_atomic_uchar* : __mgu_atomic_exchange_explicit_u8,  \
    mgu_atomic_short* : __mgu_atomic_exchange_explicit_i16, \
    mgu_atomic_ushort*: __mgu_atomic_exchange_explicit_u16, \
    mgu_atomic_long*  : __mgu_atomic_exchange_explicit_i32, \
    mgu_atomic_ulong* : __mgu_atomic_exchange_explicit_u32, \
    mgu_atomic_llong* : __mgu_atomic_exchange_explicit_i64, \
    mgu_atomic_ullong*: __mgu_atomic_exchange_explicit_u64  \
)(obj,desired,order)

#else

MG_CAPI_INLINE int64_t __mgu_atomic_exchange_explicit(
    volatile void* _obj, int64_t _desired, mgu_memory_order _order)
{
    volatile mgu_atomic_none* p = (volatile mgu_atomic_none*)_obj;
    switch (p->type_) {
    case mgu_atomic_bool_type: 
        return (int64_t)__mgu_atomic_exchange_explicit_b  (&(((volatile mgu_atomic_bool*)_obj)->value_), (bool)_desired, _order);
    case mgu_atomic_char_type: 
        return (int64_t)__mgu_atomic_exchange_explicit_i8 (&(((volatile mgu_atomic_char*)_obj)->value_), (int8_t)_desired, _order);
    case mgu_atomic_uchar_type: 
        return (int64_t)__mgu_atomic_exchange_explicit_u8 (&(((volatile mgu_atomic_uchar*)_obj)->value_), (uint8_t)_desired, _order);
    case mgu_atomic_short_type: 
        return (int64_t)__mgu_atomic_exchange_explicit_i16(&(((volatile mgu_atomic_short*)_obj)->value_), (int16_t)_desired, _order);
    case mgu_atomic_ushort_type: 
        return (int64_t)__mgu_atomic_exchange_explicit_u16(&(((volatile mgu_atomic_ushort*)_obj)->value_), (uint16_t)_desired, _order);
    case mgu_atomic_long_type: 
        return (int64_t)__mgu_atomic_exchange_explicit_i32(&(((volatile mgu_atomic_long*)_obj)->value_), (int32_t)_desired, _order);
    case mgu_atomic_ulong_type: 
        return (int64_t)__mgu_atomic_exchange_explicit_u32(&(((volatile mgu_atomic_ulong*)_obj)->value_), (uint32_t)_desired, _order);
    case mgu_atomic_llong_type:
        return (int64_t)__mgu_atomic_exchange_explicit_i64(&(((volatile mgu_atomic_llong*)_obj)->value_), (int64_t)_desired, _order);
    case mgu_atomic_ullong_type: 
        return (int64_t)__mgu_atomic_exchange_explicit_u64(&(((volatile mgu_atomic_ullong*)_obj)->value_), (uint64_t)_desired, _order);
    default: 
        assert(false);
        return 0;
    }
}

#endif

#define mgu_atomic_exchange(obj,desired) __mgu_atomic_exchange_explicit(obj,desired, mgu_memory_order_seq_cst)
#define mgu_atomic_exchange_explicit(obj,desired,order) __mgu_atomic_exchange_explicit(obj,desired,order)


#ifdef __cplusplus

MG_CAPI_INLINE void __mgu_atomic_store_explicit(
    volatile mgu_atomic_bool* _obj, bool _desired, mgu_memory_order _order) 
{ 
    __mgu_atomic_store_explicit_b(_obj, _desired, _order);
}
MG_CAPI_INLINE void __mgu_atomic_store_explicit(
    volatile mgu_atomic_char* _obj, int8_t _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_i8(_obj, _desired, _order);
}
MG_CAPI_INLINE void __mgu_atomic_store_explicit(
    volatile mgu_atomic_uchar* _obj, uint8_t _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_u8(_obj, _desired, _order);
}
MG_CAPI_INLINE void __mgu_atomic_store_explicit(
    volatile mgu_atomic_short* _obj, int16_t _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_i16(_obj, _desired, _order);
}
MG_CAPI_INLINE void __mgu_atomic_store_explicit(
    volatile mgu_atomic_ushort* _obj, uint16_t _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_u16(_obj, _desired, _order);
}
MG_CAPI_INLINE void __mgu_atomic_store_explicit(
    volatile mgu_atomic_long* _obj, int32_t _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_i32(_obj, _desired, _order);
}
MG_CAPI_INLINE void __mgu_atomic_store_explicit(
    volatile mgu_atomic_ulong* _obj, uint32_t _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_u32(_obj, _desired, _order);
}
MG_CAPI_INLINE void __mgu_atomic_store_explicit(
    volatile mgu_atomic_llong* _obj, int64_t _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_i64(_obj, _desired, _order);
}
MG_CAPI_INLINE void __mgu_atomic_store_explicit(
    volatile mgu_atomic_ullong* _obj, uint64_t _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_u64(_obj, _desired, _order);
}

#elif MG_LANG__C11_AVAIL

#define __mgu_atomic_store_explicit(obj,desired,order)    \
_Generic((desired),                                       \
    mgu_atomic_bool*  : __mgu_atomic_store_explicit_b,    \
    mgu_atomic_char*  : __mgu_atomic_store_explicit_i8,   \
    mgu_atomic_uchar* : __mgu_atomic_store_explicit_u8,   \
    mgu_atomic_short* : __mgu_atomic_store_explicit_i16,  \
    mgu_atomic_ushort*: __mgu_atomic_store_explicit_u16,  \
    mgu_atomic_long*  : __mgu_atomic_store_explicit_i32,  \
    mgu_atomic_ulong* : __mgu_atomic_store_explicit_u32,  \
    mgu_atomic_llong* : __mgu_atomic_store_explicit_i64,  \
    mgu_atomic_ullong*: __mgu_atomic_store_explicit_u64,  \
)(obj, desired, order)

#else

MG_CAPI_INLINE void __mgu_atomic_store_explicit(
    volatile void* _obj, int64_t _desired, mgu_memory_order _order)
{
    volatile mgu_atomic_none* p = (volatile mgu_atomic_none*)_obj;
    switch (p->type_) {
    case mgu_atomic_bool_type:
        __mgu_atomic_store_explicit_b  (&(((volatile mgu_atomic_bool*)_obj)->value_), (bool)_desired, _order);
    case mgu_atomic_char_type:
        __mgu_atomic_store_explicit_i8 (&(((volatile mgu_atomic_char*)_obj)->value_), (int8_t)_desired, _order);
    case mgu_atomic_uchar_type:
        __mgu_atomic_store_explicit_u8 (&(((volatile mgu_atomic_uchar*)_obj)->value_), (uint8_t)_desired, _order);
    case mgu_atomic_short_type:
        __mgu_atomic_store_explicit_i16(&(((volatile mgu_atomic_short*)_obj)->value_), (int16_t)_desired, _order);
    case mgu_atomic_ushort_type:
        __mgu_atomic_store_explicit_u16(&(((volatile mgu_atomic_ushort*)_obj)->value_), (uint16_t)_desired, _order);
    case mgu_atomic_long_type:
        __mgu_atomic_store_explicit_i32(&(((volatile mgu_atomic_long*)_obj)->value_), (int32_t)_desired, _order);
    case mgu_atomic_ulong_type:
        __mgu_atomic_store_explicit_u32(&(((volatile mgu_atomic_ulong*)_obj)->value_), (uint32_t)_desired, _order);
    case mgu_atomic_llong_type:
        __mgu_atomic_store_explicit_i64(&(((volatile mgu_atomic_llong*)_obj)->value_), (int64_t)_desired, _order);
    case mgu_atomic_ullong_type:
        __mgu_atomic_store_explicit_u64(&(((volatile mgu_atomic_ullong*)_obj)->value_), (uint64_t)_desired, _order);
    default:
        assert(false);
    }
}

#endif

#define mgu_atomic_store(obj,desired)    __mgu_atomic_store_explicit(obj,desired, mgu_memory_order_seq_cst)
#define mgu_atomic_store_explicit(obj,desired,order) __mgu_atomic_store_explicit(obj,desired,order)


#ifdef __cplusplus

MG_CAPI_INLINE bool __mgu_atomic_load_explicit(const volatile mgu_atomic_bool* _obj, mgu_memory_order _order)
{ 
    return __mgu_atomic_load_explicit_b(_obj, _order);
}
MG_CAPI_INLINE int8_t __mgu_atomic_load_explicit(const volatile mgu_atomic_char* _obj, mgu_memory_order _order)
{
    return __mgu_atomic_load_explicit_i8(_obj, _order);
}
MG_CAPI_INLINE uint8_t __mgu_atomic_load_explicit(const volatile mgu_atomic_uchar* _obj, mgu_memory_order _order)
{
    return __mgu_atomic_load_explicit_u8(_obj, _order);
}
MG_CAPI_INLINE int16_t __mgu_atomic_load_explicit(const volatile mgu_atomic_short* _obj, mgu_memory_order _order)
{
    return __mgu_atomic_load_explicit_i16(_obj, _order);
}
MG_CAPI_INLINE uint16_t __mgu_atomic_load_explicit(const volatile mgu_atomic_ushort* _obj, mgu_memory_order _order)
{
    return __mgu_atomic_load_explicit_u16(_obj, _order);
}
MG_CAPI_INLINE int32_t __mgu_atomic_load_explicit(const volatile mgu_atomic_long* _obj, mgu_memory_order _order)
{
    return __mgu_atomic_load_explicit_i32(_obj, _order);
}
MG_CAPI_INLINE uint32_t __mgu_atomic_load_explicit(const volatile mgu_atomic_ulong* _obj, mgu_memory_order _order)
{
    return __mgu_atomic_load_explicit_u32(_obj, _order);
}
MG_CAPI_INLINE int64_t __mgu_atomic_load_explicit(const volatile mgu_atomic_llong* _obj, mgu_memory_order _order)
{
    return __mgu_atomic_load_explicit_i64(_obj, _order);
}
MG_CAPI_INLINE uint64_t __mgu_atomic_load_explicit(const volatile mgu_atomic_ullong* _obj, mgu_memory_order _order)
{
    return __mgu_atomic_load_explicit_u64(_obj, _order);
}

#elif MG_LANG__C11_AVAIL

#define __mgu_atomic_load_explicit(obj,order)               \
_Generic((obj),                                             \
    mgu_atomic_bool*   : __mgu_atomic_load_explicit_b,      \
    mgu_atomic_char*   : __mgu_atomic_load_explicit_i8,     \
    mgu_atomic_uchar*  : __mgu_atomic_load_explicit_u8,     \
    mgu_atomic_short*  : __mgu_atomic_load_explicit_i16,    \
    mgu_atomic_ushort* : __mgu_atomic_load_explicit_u16,    \
    mgu_atomic_long*   : __mgu_atomic_load_explicit_i32,    \
    mgu_atomic_ulong*  : __mgu_atomic_load_explicit_u32,    \
    mgu_atomic_llong*  : __mgu_atomic_load_explicit_i64,    \
    mgu_atomic_ullong* : __mgu_atomic_load_explicit_u64     \
)(obj,order)

#else

MG_CAPI_INLINE int64_t __mgu_atomic_load_explicit(const volatile void* _obj, mgu_memory_order _order)
{
    volatile mgu_atomic_none* p = (volatile mgu_atomic_none*)_obj;
    switch (p->type_) {
    case mgu_atomic_bool_type:
        return (int64_t)__mgu_atomic_load_explicit_b  (&(((volatile mgu_atomic_bool*)_obj)->value_), _order);
    case mgu_atomic_char_type:
        return (int64_t)__mgu_atomic_load_explicit_i8 (&(((volatile mgu_atomic_char*)_obj)->value_), _order);
    case mgu_atomic_uchar_type:
        return (int64_t)__mgu_atomic_load_explicit_u8 (&(((volatile mgu_atomic_uchar*)_obj)->value_), _order);
    case mgu_atomic_short_type:
        return (int64_t)__mgu_atomic_load_explicit_i16(&(((volatile mgu_atomic_short*)_obj)->value_), _order);
    case mgu_atomic_ushort_type:
        return (int64_t)__mgu_atomic_load_explicit_u16(&(((volatile mgu_atomic_ushort*)_obj)->value_), _order);
    case mgu_atomic_long_type:
        return (int64_t)__mgu_atomic_load_explicit_i32(&(((volatile mgu_atomic_long*)_obj)->value_), _order);
    case mgu_atomic_ulong_type:
        return (int64_t)__mgu_atomic_load_explicit_u32(&(((volatile mgu_atomic_ulong*)_obj)->value_), _order);
    case mgu_atomic_llong_type:
        return (int64_t)__mgu_atomic_load_explicit_i64(&(((volatile mgu_atomic_llong*)_obj)->value_), _order);
    case mgu_atomic_ullong_type:
        return (int64_t)__mgu_atomic_load_explicit_u64(&(((volatile mgu_atomic_ullong*)_obj)->value_), _order);
    default:
        assert(false);
        return 0;
    }
}

#endif

#define mgu_atomic_load(obj) __mgu_atomic_load_explicit(obj, mgu_memory_order_seq_cst)
#define mgu_atomic_load_explicit(obj,order) __mgu_atomic_load_explicit(obj,order)


#ifdef __cplusplus

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit(
    volatile mgu_atomic_bool* _obj, bool* _expected, bool _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_b(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit(
    volatile mgu_atomic_char* _obj, int8_t* _expected, int8_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_i8(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit(
    volatile mgu_atomic_uchar* _obj, uint8_t* _expected, uint8_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_u8(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit(
    volatile mgu_atomic_short* _obj, int16_t* _expected, int16_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_i16(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit(
    volatile mgu_atomic_ushort* _obj, uint16_t* _expected, uint16_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_u16(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit(
    volatile mgu_atomic_long* _obj, int32_t* _expected, int32_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_i32(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit(
    volatile mgu_atomic_ulong* _obj, uint32_t* _expected, uint32_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_u32(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit(
    volatile mgu_atomic_llong* _obj, int64_t* _expected, int64_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_i64(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit(
    volatile mgu_atomic_ullong* _obj, uint64_t* _expected, uint64_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_u64(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit(
    volatile mgu_atomic_bool* _obj, bool* _expected, bool _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_weak_explicit_b(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit(
    volatile mgu_atomic_char* _obj, int8_t* _expected, int8_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_weak_explicit_i8(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit(
    volatile mgu_atomic_uchar* _obj, uint8_t* _expected, uint8_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_weak_explicit_u8(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit(
    volatile mgu_atomic_short* _obj, int16_t* _expected, int16_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_weak_explicit_i16(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit(
    volatile mgu_atomic_ushort* _obj, uint16_t* _expected, uint16_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_weak_explicit_u16(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit(
    volatile mgu_atomic_long* _obj, int32_t* _expected, int32_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_weak_explicit_i32(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit(
    volatile mgu_atomic_ulong* _obj, uint32_t* _expected, uint32_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_weak_explicit_u32(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit(
    volatile mgu_atomic_llong* _obj, int64_t* _expected, int64_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_weak_explicit_i64(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit(
    volatile mgu_atomic_ullong* _obj, uint64_t* _expected, uint64_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_weak_explicit_u64(_obj, _expected, _desired, _success, _failure);
}

#elif MG_LANG__C11_AVAIL

#define __mgu_atomic_compare_exchange_strong_explicit(obj,expected,desired,success,failure)\
 _Generic((obj),                                                                           \
    mgu_atomic_bool*  : __mgu_atomic_compare_exchange_strong_explicit_b,                   \
    mgu_atomic_char*  : __mgu_atomic_compare_exchange_strong_explicit_i8,                  \
    mgu_atomic_uchar* : __mgu_atomic_compare_exchange_strong_explicit_u8,                  \
    mgu_atomic_short* : __mgu_atomic_compare_exchange_strong_explicit_i16,                 \
    mgu_atomic_ushort*: __mgu_atomic_compare_exchange_strong_explicit_u16,                 \
    mgu_atomic_long*  : __mgu_atomic_compare_exchange_strong_explicit_i32,                 \
    mgu_atomic_ulong* : __mgu_atomic_compare_exchange_strong_explicit_u32,                 \
    mgu_atomic_llong* : __mgu_atomic_compare_exchange_strong_explicit_i64,                 \
    mgu_atomic_ullong*: __mgu_atomic_compare_exchange_strong_explicit_u64,                 \
)(obj, expected, desired, success, failure)

#define __mgu_atomic_compare_exchange_weak_explicit(obj,expected,desired,success,failure)  \
_Generic((obj),                                                                            \
    mgu_atomic_bool*  : __mgu_atomic_compare_exchange_weak_explicit_b,                     \
    mgu_atomic_char*  : __mgu_atomic_compare_exchange_weak_explicit_i8,                    \
    mgu_atomic_uchar* : __mgu_atomic_compare_exchange_weak_explicit_u8,                    \
    mgu_atomic_short* : __mgu_atomic_compare_exchange_weak_explicit_i16,                   \
    mgu_atomic_ushort*: __mgu_atomic_compare_exchange_weak_explicit_u16,                   \
    mgu_atomic_long*  : __mgu_atomic_compare_exchange_weak_explicit_i32,                   \
    mgu_atomic_ulong* : __mgu_atomic_compare_exchange_weak_explicit_u32,                   \
    mgu_atomic_llong* : __mgu_atomic_compare_exchange_weak_explicit_i64,                   \
    mgu_atomic_ullong*: __mgu_atomic_compare_exchange_weak_explicit_u64,                   \
)(obj, expected, desired, success, failure)

#else

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit(
    volatile void* _obj, void* _expected, int64_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    volatile mgu_atomic_none* p = (volatile mgu_atomic_none*)_obj;
    switch (p->type_) {
    case mgu_atomic_bool_type:
        return __mgu_atomic_compare_exchange_strong_explicit_b(
            &(((volatile mgu_atomic_bool*)_obj)->value_), (bool*)_expected, (bool)_desired, _success, _failure);
    case mgu_atomic_char_type:
        return __mgu_atomic_compare_exchange_strong_explicit_i8(
            &(((volatile mgu_atomic_char*)_obj)->value_), (int8_t*)_expected, (int8_t)_desired, _success, _failure);
    case mgu_atomic_uchar_type:
        return __mgu_atomic_compare_exchange_strong_explicit_u8(
            &(((volatile mgu_atomic_uchar*)_obj)->value_), (uint8_t*)_expected, (uint8_t)_desired, _success, _failure);
    case mgu_atomic_short_type:
        return __mgu_atomic_compare_exchange_strong_explicit_i16(
            &(((volatile mgu_atomic_short*)_obj)->value_), (int16_t*)_expected, (int16_t)_desired, _success, _failure);
    case mgu_atomic_ushort_type:
        return __mgu_atomic_compare_exchange_strong_explicit_u16(
            &(((volatile mgu_atomic_ushort*)_obj)->value_), (uint16_t*)_expected, (uint16_t)_desired, _success, _failure);
    case mgu_atomic_long_type:
        return __mgu_atomic_compare_exchange_strong_explicit_i32(
            &(((volatile mgu_atomic_long*)_obj)->value_), (int32_t*)_expected, (int32_t)_desired, _success, _failure);
    case mgu_atomic_ulong_type:
        return __mgu_atomic_compare_exchange_strong_explicit_u32(
            &(((volatile mgu_atomic_ulong*)_obj)->value_), (uint32_t*)_expected, (uint32_t)_desired, _success, _failure);
    case mgu_atomic_llong_type:
        return __mgu_atomic_compare_exchange_strong_explicit_i64(
            &(((volatile mgu_atomic_llong*)_obj)->value_), (int64_t*)_expected, (int64_t)_desired, _success, _failure);
    case mgu_atomic_ullong_type:
        return __mgu_atomic_compare_exchange_strong_explicit_u64(
            &(((volatile mgu_atomic_ullong*)_obj)->value_), (uint64_t*)_expected, (uint64_t)_desired, _success, _failure);
    default:
        assert(false);
        return false;
    }
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit(
    volatile void* _obj, void* _expected, int64_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    volatile mgu_atomic_none* p = (volatile mgu_atomic_none*)_obj;
    switch (p->type_) {
    case mgu_atomic_bool_type:
        return __mgu_atomic_compare_exchange_weak_explicit_b  (
            &(((volatile mgu_atomic_bool*)_obj)->value_), (bool*)_expected, (bool)_desired, _success, _failure);
    case mgu_atomic_char_type:
        return __mgu_atomic_compare_exchange_weak_explicit_i8 (
            &(((volatile mgu_atomic_char*)_obj)->value_), (int8_t*)_expected, (int8_t)_desired, _success, _failure);
    case mgu_atomic_uchar_type:
        return __mgu_atomic_compare_exchange_weak_explicit_u8 (
            &(((volatile mgu_atomic_uchar*)_obj)->value_), (uint8_t*)_expected, (uint8_t)_desired, _success, _failure);
    case mgu_atomic_short_type:
        return __mgu_atomic_compare_exchange_weak_explicit_i16(
            &(((volatile mgu_atomic_short*)_obj)->value_), (int16_t*)_expected, (int16_t)_desired, _success, _failure);
    case mgu_atomic_ushort_type:
        return __mgu_atomic_compare_exchange_weak_explicit_u16(
            &(((volatile mgu_atomic_ushort*)_obj)->value_), (uint16_t*)_expected, (uint16_t)_desired, _success, _failure);
    case mgu_atomic_long_type:
        return __mgu_atomic_compare_exchange_weak_explicit_i32(
            &(((volatile mgu_atomic_long*)_obj)->value_), (int32_t*)_expected, (int32_t)_desired, _success, _failure);
    case mgu_atomic_ulong_type:
        return __mgu_atomic_compare_exchange_weak_explicit_u32(
            &(((volatile mgu_atomic_ulong*)_obj)->value_), (uint32_t*)_expected, (uint32_t)_desired, _success, _failure);
    case mgu_atomic_llong_type:
        return __mgu_atomic_compare_exchange_weak_explicit_i64(
            &(((volatile mgu_atomic_llong*)_obj)->value_), (int64_t*)_expected, (int64_t)_desired, _success, _failure);
    case mgu_atomic_ullong_type:
        return __mgu_atomic_compare_exchange_weak_explicit_u64(
            &(((volatile mgu_atomic_ullong*)_obj)->value_), (uint64_t*)_expected, (uint64_t)_desired, _success, _failure);
    default:
        assert(false);
        return false;
    }
}

#endif

#define mgu_atomic_compare_exchange_strong_explicit(obj,expected,desired,success,failure)   \
      __mgu_atomic_compare_exchange_strong_explicit(obj,expected,desired,success,failure)

#define mgu_atomic_compare_exchange_weak_explicit(obj,expected,desired,success,failure)   \
      __mgu_atomic_compare_exchange_weak_explicit(obj,expected,desired,success,failure)

#define mgu_atomic_compare_exchange_strong(obj,expected,desired) \
        __mgu_atomic_compare_exchange_strong_explicit(obj,expected,desired, mgu_memory_order_seq_cst)

#define mgu_atomic_compare_exchange_weak(obj,expected,desired) \
        __mgu_atomic_compare_exchange_weak_explicit(obj,expected,desired, mgu_memory_order_seq_cst)


#ifdef __cplusplus

MG_CAPI_INLINE char __mgu_atomic_fetch_add_explicit(
    volatile mgu_atomic_char* _obj, char _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_i8(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned char __mgu_atomic_fetch_add_explicit(
    volatile mgu_atomic_uchar* _obj, unsigned char _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_u8(_obj, _arg, _order);
}

MG_CAPI_INLINE short __mgu_atomic_fetch_add_explicit(
    volatile mgu_atomic_short* _obj, short _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_i16(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned short __mgu_atomic_fetch_add_explicit(
    volatile mgu_atomic_ushort* _obj, unsigned short _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_u16(_obj, _arg, _order);
}

MG_CAPI_INLINE long __mgu_atomic_fetch_add_explicit(
    volatile mgu_atomic_long* _obj, long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_i32(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned long __mgu_atomic_fetch_add_explicit(
    volatile mgu_atomic_ulong* _obj, unsigned long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_u32(_obj, _arg, _order);
}

MG_CAPI_INLINE long long __mgu_atomic_fetch_add_explicit(
    volatile mgu_atomic_llong* _obj, long long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_i64(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_fetch_add_explicit(
    volatile mgu_atomic_ullong* _obj, unsigned long long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_u64(_obj, _arg, _order);
}

#elif MG_LANG__C11_AVAIL

#define __mgu_atomic_fetch_add_explicit(obj,arg,order)             \
_Generic((obj),                                                    \
mgu_atomic_char*  : __mgu_atomic_fetch_add_explicit_i8,            \
mgu_atomic_uchar* : __mgu_atomic_fetch_add_explicit_u8,            \
mgu_atomic_short* : __mgu_atomic_fetch_add_explicit_i16,           \
mgu_atomic_ushort*: __mgu_atomic_fetch_add_explicit_u16,           \
mgu_atomic_long*  : __mgu_atomic_fetch_add_explicit_i32,           \
mgu_atomic_ulong* : __mgu_atomic_fetch_add_explicit_u32,           \
mgu_atomic_llong* : __mgu_atomic_fetch_add_explicit_i64,           \
mgu_atomic_ullong*: __mgu_atomic_fetch_add_explicit_u64            \
)(obj,arg,order)

#else

MG_CAPI_INLINE int64_t __mgu_atomic_fetch_add_explicit(
    volatile void* _obj, int64_t _arg, mgu_memory_order _order)
{
    volatile mgu_atomic_none* p = (volatile mgu_atomic_none*)_obj;
    switch (p->type_) {
    case mgu_atomic_char_type:
        return (int64_t)__mgu_atomic_fetch_add_explicit_i8 (&(((volatile mgu_atomic_char*)_obj)->value_), (int8_t)_arg, _order);
    case mgu_atomic_uchar_type:
        return (int64_t)__mgu_atomic_fetch_add_explicit_u8 (&(((volatile mgu_atomic_uchar*)_obj)->value_), (uint8_t)_arg, _order);
    case mgu_atomic_short_type:
        return (int64_t)__mgu_atomic_fetch_add_explicit_i16(&(((volatile mgu_atomic_short*)_obj)->value_), (int16_t)_arg, _order);
    case mgu_atomic_ushort_type:
        return (int64_t)__mgu_atomic_fetch_add_explicit_u16(&(((volatile mgu_atomic_ushort*)_obj)->value_), (uint16_t)_arg, _order);
    case mgu_atomic_long_type:
        return (int64_t)__mgu_atomic_fetch_add_explicit_i32(&(((volatile mgu_atomic_long*)_obj)->value_), (int32_t)_arg, _order);
    case mgu_atomic_ulong_type:
        return (int64_t)__mgu_atomic_fetch_add_explicit_u32(&(((volatile mgu_atomic_ulong*)_obj)->value_), (uint32_t)_arg, _order);
    case mgu_atomic_llong_type:
        return (int64_t)__mgu_atomic_fetch_add_explicit_i64(&(((volatile mgu_atomic_llong*)_obj)->value_), (int64_t)_arg, _order);
    case mgu_atomic_ullong_type:
        return (int64_t)__mgu_atomic_fetch_add_explicit_u64(&(((volatile mgu_atomic_ullong*)_obj)->value_), (uint64_t)_arg, _order);
    default:
        assert(false);
        return 0;
    }
}

#endif

#define mgu_atomic_fetch_add_explicit(obj,arg,order) __mgu_atomic_fetch_add_explicit(obj,arg,order)
#define mgu_atomic_fetch_add(obj,arg) __mgu_atomic_fetch_add_explicit(obj,arg,mgu_memory_order_seq_cst)


#ifdef __cplusplus

MG_CAPI_INLINE char __mgu_atomic_fetch_sub_explicit(
    volatile mgu_atomic_char* _obj, char _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_sub_explicit_i8(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned char __mgu_atomic_fetch_sub_explicit(
    volatile mgu_atomic_uchar* _obj, unsigned char _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_sub_explicit_u8(_obj, _arg, _order);
}

MG_CAPI_INLINE short __mgu_atomic_fetch_sub_explicit(
    volatile mgu_atomic_short* _obj, short _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_sub_explicit_i16(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned short __mgu_atomic_fetch_sub_explicit(
    volatile mgu_atomic_ushort* _obj, unsigned short _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_sub_explicit_u16(_obj, _arg, _order);
}

MG_CAPI_INLINE long __mgu_atomic_fetch_sub_explicit(
    volatile mgu_atomic_long* _obj, long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_sub_explicit_i32(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned long __mgu_atomic_fetch_sub_explicit(
    volatile mgu_atomic_ulong* _obj, unsigned long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_sub_explicit_u32(_obj, _arg, _order);
}

MG_CAPI_INLINE long long __mgu_atomic_fetch_sub_explicit(
    volatile mgu_atomic_llong* _obj, long long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_sub_explicit_i64(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_fetch_sub_explicit(
    volatile mgu_atomic_ullong* _obj, unsigned long long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_sub_explicit_u64(_obj, _arg, _order);
}

#elif MG_LANG__C11_AVAIL

#define __mgu_atomic_fetch_sub_explicit(obj,arg,order)                 \
_Generic((obj),                                                        \
    mgu_atomic_char*  : __mgu_atomic_fetch_sub_explicit_i8,            \
    mgu_atomic_uchar* : __mgu_atomic_fetch_sub_explicit_u8,            \
    mgu_atomic_short* : __mgu_atomic_fetch_sub_explicit_i16,           \
    mgu_atomic_ushort*: __mgu_atomic_fetch_sub_explicit_u16,           \
    mgu_atomic_long*  : __mgu_atomic_fetch_sub_explicit_i32,           \
    mgu_atomic_ulong* : __mgu_atomic_fetch_sub_explicit_u32,           \
    mgu_atomic_llong* : __mgu_atomic_fetch_sub_explicit_i64,           \
    mgu_atomic_ullong*: __mgu_atomic_fetch_sub_explicit_u64            \
)(obj,arg,order)

#else


MG_CAPI_INLINE int64_t __mgu_atomic_fetch_sub_explicit(
    volatile void* _obj, int64_t _arg, mgu_memory_order _order)
{
    volatile mgu_atomic_none* p = (volatile mgu_atomic_none*)_obj;
    switch (p->type_) {
    case mgu_atomic_char_type:
        return (int64_t)__mgu_atomic_fetch_sub_explicit_i8 (&(((volatile mgu_atomic_char*)_obj)->value_), (int8_t)_arg, _order);
    case mgu_atomic_uchar_type:
        return (int64_t)__mgu_atomic_fetch_sub_explicit_u8 (&(((volatile mgu_atomic_uchar*)_obj)->value_), (uint8_t)_arg, _order);
    case mgu_atomic_short_type:
        return (int64_t)__mgu_atomic_fetch_sub_explicit_i16(&(((volatile mgu_atomic_short*)_obj)->value_), (int16_t)_arg, _order);
    case mgu_atomic_ushort_type:
        return (int64_t)__mgu_atomic_fetch_sub_explicit_u16(&(((volatile mgu_atomic_ushort*)_obj)->value_), (uint16_t)_arg, _order);
    case mgu_atomic_long_type:
        return (int64_t)__mgu_atomic_fetch_sub_explicit_i32(&(((volatile mgu_atomic_long*)_obj)->value_), (int32_t)_arg, _order);
    case mgu_atomic_ulong_type:
        return (int64_t)__mgu_atomic_fetch_sub_explicit_u32(&(((volatile mgu_atomic_ulong*)_obj)->value_), (uint32_t)_arg, _order);
    case mgu_atomic_llong_type:
        return (int64_t)__mgu_atomic_fetch_sub_explicit_i64(&(((volatile mgu_atomic_llong*)_obj)->value_), (int64_t)_arg, _order);
    case mgu_atomic_ullong_type:
        return (int64_t)__mgu_atomic_fetch_sub_explicit_u64(&(((volatile mgu_atomic_ullong*)_obj)->value_), (uint64_t)_arg, _order);
    default:
        assert(false);
        return 0;
    }
}


#endif

#define mgu_atomic_fetch_sub_explicit(obj,arg,order) __mgu_atomic_fetch_sub_explicit(obj,arg,order)
#define mgu_atomic_fetch_sub(obj,arg) __mgu_atomic_fetch_sub_explicit(obj,arg,mgu_memory_order_seq_cst)


#ifdef __cplusplus

MG_CAPI_INLINE char __mgu_atomic_fetch_and_explicit(
    mgu_atomic_char* obj, char arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_and_explicit_i8(obj, arg, order);
}

MG_CAPI_INLINE unsigned char __mgu_atomic_fetch_and_explicit(
    mgu_atomic_uchar* obj, unsigned char arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_and_explicit_u8(obj, arg, order);
}

MG_CAPI_INLINE short __mgu_atomic_fetch_and_explicit(
    mgu_atomic_short* obj, short arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_and_explicit_i16(obj, arg, order);
}

MG_CAPI_INLINE unsigned short __mgu_atomic_fetch_and_explicit(
    mgu_atomic_ushort* obj, unsigned short arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_and_explicit_u16(obj, arg, order);
}

MG_CAPI_INLINE long __mgu_atomic_fetch_and_explicit(
    mgu_atomic_long* obj, long arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_and_explicit_i32(obj, arg, order);
}

MG_CAPI_INLINE unsigned long __mgu_atomic_fetch_and_explicit(
    mgu_atomic_ulong* obj, unsigned long arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_and_explicit_u32(obj, arg, order);
}

MG_CAPI_INLINE long long __mgu_atomic_fetch_and_explicit(
    mgu_atomic_llong* obj, long long arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_and_explicit_i64(obj, arg, order);
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_fetch_and_explicit(
    mgu_atomic_ullong* obj, unsigned long long arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_and_explicit_u64(obj, arg, order);
}

#elif MG_LANG__C11_AVAIL

#define __mgu_atomic_fetch_and_explicit(obj,arg,order)             \
_Generic((obj),                                                    \
mgu_atomic_char*  : __mgu_atomic_fetch_and_explicit_i8,            \
mgu_atomic_uchar* : __mgu_atomic_fetch_and_explicit_u8,            \
mgu_atomic_short* : __mgu_atomic_fetch_and_explicit_i16,           \
mgu_atomic_ushort*: __mgu_atomic_fetch_and_explicit_u16,           \
mgu_atomic_long*  : __mgu_atomic_fetch_and_explicit_i32,           \
mgu_atomic_ulong* : __mgu_atomic_fetch_and_explicit_u32,           \
mgu_atomic_llong* : __mgu_atomic_fetch_and_explicit_i64,           \
mgu_atomic_ullong*: __mgu_atomic_fetch_and_explicit_u64            \
)(obj,arg,order)

#else
    
MG_CAPI_INLINE int64_t __mgu_atomic_fetch_and_explicit(
    volatile void* _obj, int64_t _arg, mgu_memory_order _order)
{
    volatile mgu_atomic_none* p = (volatile mgu_atomic_none*)_obj;
    switch (p->type_) {
    case mgu_atomic_char_type:
        return (int64_t)__mgu_atomic_fetch_and_explicit_i8 (&(((volatile mgu_atomic_char*)_obj)->value_), (int8_t)_arg, _order);
    case mgu_atomic_uchar_type:
        return (int64_t)__mgu_atomic_fetch_and_explicit_u8 (&(((volatile mgu_atomic_uchar*)_obj)->value_), (uint8_t)_arg, _order);
    case mgu_atomic_short_type:
        return (int64_t)__mgu_atomic_fetch_and_explicit_i16(&(((volatile mgu_atomic_short*)_obj)->value_), (int16_t)_arg, _order);
    case mgu_atomic_ushort_type:
        return (int64_t)__mgu_atomic_fetch_and_explicit_u16(&(((volatile mgu_atomic_ushort*)_obj)->value_), (uint16_t)_arg, _order);
    case mgu_atomic_long_type:
        return (int64_t)__mgu_atomic_fetch_and_explicit_i32(&(((volatile mgu_atomic_long*)_obj)->value_), (int32_t)_arg, _order);
    case mgu_atomic_ulong_type:
        return (int64_t)__mgu_atomic_fetch_and_explicit_u32(&(((volatile mgu_atomic_ulong*)_obj)->value_), (uint32_t)_arg, _order);
    case mgu_atomic_llong_type:
        return (int64_t)__mgu_atomic_fetch_and_explicit_i64(&(((volatile mgu_atomic_llong*)_obj)->value_), (int64_t)_arg, _order);
    case mgu_atomic_ullong_type:
        return (int64_t)__mgu_atomic_fetch_and_explicit_u64(&(((volatile mgu_atomic_ullong*)_obj)->value_), (uint64_t)_arg, _order);
    default:
        assert(false);
        return 0;
    }
}

#endif

#define mgu_atomic_fetch_and_explicit(obj,arg,order) __mgu_atomic_fetch_and_explicit(obj,arg,order)
#define mgu_atomic_fetch_and(obj,arg,order) __mgu_atomic_fetch_and_explicit(obj,arg,mgu_memory_order_seq_cst)


#ifdef __cplusplus

MG_CAPI_INLINE char __mgu_atomic_fetch_or_explicit(
    volatile mgu_atomic_char* obj, char arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_or_explicit_i8(obj, arg, order);
}

MG_CAPI_INLINE unsigned char __mgu_atomic_fetch_or_explicit(
    volatile mgu_atomic_uchar* obj, unsigned char arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_or_explicit_u8(obj, arg, order);
}

MG_CAPI_INLINE short __mgu_atomic_fetch_or_explicit(
    volatile mgu_atomic_short* obj, short arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_or_explicit_i16(obj, arg, order);
}

MG_CAPI_INLINE unsigned short __mgu_atomic_fetch_or_explicit(
    volatile mgu_atomic_ushort* obj, unsigned short arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_or_explicit_u16(obj, arg, order);
}

MG_CAPI_INLINE long __mgu_atomic_fetch_or_explicit(
    volatile mgu_atomic_long* obj, long arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_or_explicit_i32(obj, arg, order);
}

MG_CAPI_INLINE unsigned long __mgu_atomic_fetch_or_explicit(
    volatile mgu_atomic_ulong* obj, unsigned long arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_or_explicit_u32(obj, arg, order);
}

MG_CAPI_INLINE long long __mgu_atomic_fetch_or_explicit(
    volatile mgu_atomic_llong* obj, long long arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_or_explicit_i64(obj, arg, order);
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_fetch_or_explicit(
    volatile mgu_atomic_ullong* obj, unsigned long long arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_or_explicit_u64(obj, arg, order);
}

#elif MG_LANG__C11_AVAIL

#define __mgu_atomic_fetch_or_explicit(obj,arg,order)             \
_Generic((obj),                                                   \
mgu_atomic_char*  : __mgu_atomic_fetch_or_explicit_i8,            \
mgu_atomic_uchar* : __mgu_atomic_fetch_or_explicit_u8,            \
mgu_atomic_short* : __mgu_atomic_fetch_or_explicit_i16,           \
mgu_atomic_ushort*: __mgu_atomic_fetch_or_explicit_u16,           \
mgu_atomic_long*  : __mgu_atomic_fetch_or_explicit_i32,           \
mgu_atomic_ulong* : __mgu_atomic_fetch_or_explicit_u32,           \
mgu_atomic_llong* : __mgu_atomic_fetch_or_explicit_i64,           \
mgu_atomic_ullong*: __mgu_atomic_fetch_or_explicit_u64            \
)(obj,arg,order)

#else

MG_CAPI_INLINE int64_t __mgu_atomic_fetch_or_explicit(
    volatile void* _obj, int64_t _arg, mgu_memory_order _order)
{
    volatile mgu_atomic_none* p = (volatile mgu_atomic_none*)_obj;
    switch (p->type_) {
    case mgu_atomic_char_type:
        return (int64_t)__mgu_atomic_fetch_or_explicit_i8(&(((volatile mgu_atomic_char*)_obj)->value_), (int8_t)_arg, _order);
    case mgu_atomic_uchar_type:
        return (int64_t)__mgu_atomic_fetch_or_explicit_u8(&(((volatile mgu_atomic_uchar*)_obj)->value_), (uint8_t)_arg, _order);
    case mgu_atomic_short_type:
        return (int64_t)__mgu_atomic_fetch_or_explicit_i16(&(((volatile mgu_atomic_short*)_obj)->value_), (int16_t)_arg, _order);
    case mgu_atomic_ushort_type:
        return (int64_t)__mgu_atomic_fetch_or_explicit_u16(&(((volatile mgu_atomic_ushort*)_obj)->value_), (uint16_t)_arg, _order);
    case mgu_atomic_long_type:
        return (int64_t)__mgu_atomic_fetch_or_explicit_i32(&(((volatile mgu_atomic_long*)_obj)->value_), (int32_t)_arg, _order);
    case mgu_atomic_ulong_type:
        return (int64_t)__mgu_atomic_fetch_or_explicit_u32(&(((volatile mgu_atomic_ulong*)_obj)->value_), (uint32_t)_arg, _order);
    case mgu_atomic_llong_type:
        return (int64_t)__mgu_atomic_fetch_or_explicit_i64(&(((volatile mgu_atomic_llong*)_obj)->value_), (int64_t)_arg, _order);
    case mgu_atomic_ullong_type:
        return (int64_t)__mgu_atomic_fetch_or_explicit_u64(&(((volatile mgu_atomic_ullong*)_obj)->value_), (uint64_t)_arg, _order);
    default:
        assert(false);
        return 0;
    }
}

#endif

#define mgu_atomic_fetch_or_explicit(obj,arg,order) __mgu_atomic_fetch_or_explicit(obj,arg,order)
#define mgu_atomic_fetch_or(obj,arg) __mgu_atomic_fetch_or_explicit(obj,arg,mgu_memory_order_seq_cst)


#ifdef __cplusplus

MG_CAPI_INLINE char __mgu_atomic_fetch_xor_explicit(
    volatile mgu_atomic_char* _obj, char _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_xor_explicit_i8(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned char __mgu_atomic_fetch_xor_explicit(
    volatile mgu_atomic_uchar* _obj, unsigned char _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_xor_explicit_u8(_obj, _arg, _order);
}

MG_CAPI_INLINE short __mgu_atomic_fetch_xor_explicit(
    volatile mgu_atomic_short* _obj, short _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_xor_explicit_i16(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned short __mgu_atomic_fetch_xor_explicit(
    volatile mgu_atomic_ushort* _obj, unsigned short _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_xor_explicit_u16(_obj, _arg, _order);
}

MG_CAPI_INLINE long __mgu_atomic_fetch_xor_explicit(
    volatile mgu_atomic_long* _obj, long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_xor_explicit_i32(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned long __mgu_atomic_fetch_xor_explicit(
    volatile mgu_atomic_ulong* _obj, unsigned long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_xor_explicit_u32(_obj, _arg, _order);
}

MG_CAPI_INLINE long long __mgu_atomic_fetch_xor_explicit(
    volatile mgu_atomic_llong* _obj, long long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_xor_explicit_i64(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_fetch_xor_explicit(
    volatile mgu_atomic_ullong* _obj, unsigned long long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_xor_explicit_u64(_obj, _arg, _order);
}

#elif MG_LANG__C11_AVAIL

#define __mgu_atomic_fetch_xor_explicit(obj,arg,order)            \
_Generic((obj),                                                   \
mgu_atomic_char*  : __mgu_atomic_fetch_xor_explicit_i8,           \
mgu_atomic_uchar* : __mgu_atomic_fetch_xor_explicit_u8,           \
mgu_atomic_short* : __mgu_atomic_fetch_xor_explicit_i16,          \
mgu_atomic_ushort*: __mgu_atomic_fetch_xor_explicit_u16,          \
mgu_atomic_long*  : __mgu_atomic_fetch_xor_explicit_i32,          \
mgu_atomic_ulong* : __mgu_atomic_fetch_xor_explicit_u32,          \
mgu_atomic_llong* : __mgu_atomic_fetch_xor_explicit_i64,          \
mgu_atomic_ullong*: __mgu_atomic_fetch_xor_explicit_u64           \
)(obj,arg,order)

#else

MG_CAPI_INLINE int64_t __mgu_atomic_fetch_xor_explicit(
    volatile void* _obj, int64_t _arg, mgu_memory_order _order)
{
    volatile mgu_atomic_none* p = (volatile mgu_atomic_none*)_obj;
    switch (p->type_) {
    case mgu_atomic_char_type:
        return (int64_t)__mgu_atomic_fetch_xor_explicit_i8(&(((volatile mgu_atomic_char*)_obj)->value_), (int8_t)_arg, _order);
    case mgu_atomic_uchar_type:
        return (int64_t)__mgu_atomic_fetch_xor_explicit_u8(&(((volatile mgu_atomic_uchar*)_obj)->value_), (uint8_t)_arg, _order);
    case mgu_atomic_short_type:
        return (int64_t)__mgu_atomic_fetch_xor_explicit_i16(&(((volatile mgu_atomic_short*)_obj)->value_), (int16_t)_arg, _order);
    case mgu_atomic_ushort_type:
        return (int64_t)__mgu_atomic_fetch_xor_explicit_u16(&(((volatile mgu_atomic_ushort*)_obj)->value_), (uint16_t)_arg, _order);
    case mgu_atomic_long_type:
        return (int64_t)__mgu_atomic_fetch_xor_explicit_i32(&(((volatile mgu_atomic_long*)_obj)->value_), (int32_t)_arg, _order);
    case mgu_atomic_ulong_type:
        return (int64_t)__mgu_atomic_fetch_xor_explicit_u32(&(((volatile mgu_atomic_ulong*)_obj)->value_), (uint32_t)_arg, _order);
    case mgu_atomic_llong_type:
        return (int64_t)__mgu_atomic_fetch_xor_explicit_i64(&(((volatile mgu_atomic_llong*)_obj)->value_), (int64_t)_arg, _order);
    case mgu_atomic_ullong_type:
        return (int64_t)__mgu_atomic_fetch_xor_explicit_u64(&(((volatile mgu_atomic_ullong*)_obj)->value_), (uint64_t)_arg, _order);
    default:
        assert(false);
        return 0;
    }
}

#endif

#define mgu_atomic_fetch_xor_explicit(obj,arg,order) __mgu_atomic_fetch_xor_explicit(obj,arg,order)
#define mgu_atomic_fetch_xor(obj,arg) __mgu_atomic_fetch_xor_explicit(obj,arg,mgu_memory_order_seq_cst)

MG_CAPI_INLINE bool mgu_atomic_flag_test_and_set_explicit(
    volatile mgu_atomic_flag* _obj, mgu_memory_order _order)
{
    bool o = false;
    return __mgu_atomic_compare_exchange_strong_explicit_b(
        &_obj->value_, &o, true, _order, mgu_memory_order_relaxed) ? 0 : 1;
}

MG_CAPI_INLINE void mgu_atomic_flag_clear_explicit(
    volatile mgu_atomic_flag* _obj, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_b(&_obj->value_, false, _order);
}

#define mgu_atomic_flag_test_and_set(obj) mgu_atomic_flag_test_and_set_explicit(obj, mgu_memory_order_seq_cst)
#define mgu_atomic_flag_clear(obj) mgu_atomic_flag_clear_explicit(obj, mgu_memory_order_release)

#elif defined(__STDC_NO_ATOMICS__) && MG_LANG__C11_AVAIL

#define mgu_atomic_is_lock_free(obj) atomic_is_lock_free(obj)
#define mgu_atomic_init(obj,desired) atomic_init(obj,desired)
#define mgu_atomic_store(obj,desired) atomic_store(obj,desired)
#define mgu_atomic_store_explicit(obj,desired,order) atomic_store_explicit(obj,desired,order)
#define mgu_atomic_load(obj) atomic_load(obj)
#define mgu_atomic_load_explicit(obj,order) atomic_load_explicit(obj,order)
#define mgu_atomic_exchange(obj,desired) atomic_exchange(obj,desired)
#define mgu_atomic_exchange_explicit(obj,desired,order) atomic_exchange_explicit(obj,desired,order)
#define mgu_atomic_compare_exchange_strong(obj,expected,desired) atomic_compare_exchange_strong(obj,expected,desired)
#define mgu_atomic_compare_exchange_weak(obj,expected,desired) atomic_compare_exchange_weak(obj,expected,desired)
#define mgu_atomic_compare_exchange_strong_explicit(obj,expected,desired,success,failure) atomic_compare_exchange_strong_explicit(obj,expected,desired,success,failure)
#define mgu_atomic_compare_exchange_weak_explicit(obj,expected,desired,success,failure) atomic_compare_exchange_weak_explicit(obj,expected,desired,success,failure)

#define mgu_atomic_fetch_add(obj,arg) atomic_fetch_add(obj,arg)
#define mgu_atomic_fetch_add_explicit(obj,arg,order) atomic_fetch_add_explicit(obj,arg,order)
#define mgu_atomic_fetch_sub(obj,arg) atomic_fetch_sub(obj,arg)
#define mgu_atomic_fetch_sub_explicit(obj,arg,order) atomic_fetch_sub_explicit(obj,arg,order)
#define mgu_atomic_fetch_and(obj,arg) atomic_fetch_and(obj,arg)
#define mgu_atomic_fetch_and_explicit(obj,arg,order) atomic_fetch_and_explicit(obj,arg,order)
#define mgu_atomic_fetch_or(obj,arg) atomic_fetch_or(obj,arg)
#define mgu_atomic_fetch_or_explicit(obj,arg,order) atomic_fetch_or_explicit(obj,arg,order)
#define mgu_atomic_fetch_xor(obj,arg) atomic_fetch_xor(obj,arg)
#define mgu_atomic_fetch_xor_explicit(obj,arg,order) atomic_fetch_xor_explicit(obj,arg,order)

#define mgu_atomic_flag_test_and_set(obj) atomic_flag_test_and_set(obj)
#define mgu_atomic_flag_test_and_set_explicit(obj,order) atomic_flag_test_and_set_explicit(obj,order)
#define mgu_atomic_flag_clear(obj) atomic_flag_clear(obj)
#define mgu_atomic_flag_clear_explicit(obj,order) atomic_flag_clear_explicit(obj,order)


#else

#  error "No mego::util::atomic support"

#endif

#endif // !MEGO_UTIL_ATOMIC_H_INCLUDED
