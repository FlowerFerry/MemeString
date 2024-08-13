
#ifndef MEGO_UTIL_ATOMIC_H_INCLUDED
#define MEGO_UTIL_ATOMIC_H_INCLUDED

#include "atomic_fwd.h"
#include <mego/predef/symbol/inline.h>
#include <mego/predef/lang/version.h>

#if MG_COMP__MSVC_AVAIL 

#include <string.h>
#include <yvals.h>
#include <xatomic.h>
#include <xthreads.h>

// Padding bits should not participate in cmpxchg comparison starting in C++20.
// Clang does not have __builtin_zero_non_value_bits to exclude these bits to implement this C++20 feature.
// The EDG front-end substitutes everything and runs into incomplete types passed to atomic<T>.
#if MG_LANG__CXX20_AVAIL && !defined(__clang__) /* TRANSITION, LLVM-46685 */ && !defined(__EDG__)
#define __MGU_ATOMIC_CMPXCHG_MASK_OUT_PADDING_BITS 1
#else
#define __MGU_ATOMIC_CMPXCHG_MASK_OUT_PADDING_BITS 0
#endif

// Controls whether ARM64 ldar/ldapr/stlr should be used
#ifndef __MGU_ATOMIC_USE_ARM64_LDAR_STLR
#  if defined(_M_ARM64) || defined(_M_ARM64EC)
#    ifdef __clang__ // TRANSITION, LLVM-62103
#      define __MGU_ATOMIC_USE_ARM64_LDAR_STLR 0
#    else // ^^^ Clang doesn't support new intrinsics / __load_acquire/__stlr intrinsics are available vvv
#      define __MGU_ATOMIC_USE_ARM64_LDAR_STLR 1
#    endif // ^^^ __load_acquire/__stlr intrinsics are available ^^^
#  else // ^^^ ARM64/ARM64EC / Other architectures vvv
#    define __MGU_ATOMIC_USE_ARM64_LDAR_STLR 0
#  endif // ^^^ Other architectures ^^^
#endif // __MGU_ATOMIC_USE_ARM64_LDAR_STLR

#ifndef __MGU_ATOMIC_INVALID_MEMORY_ORDER
#  ifdef _DEBUG
#    define __MGU_ATOMIC_INVALID_MEMORY_ORDER _STL_REPORT_ERROR("Invalid memory order")
#  else // ^^^ defined(_DEBUG) / !defined(_DEBUG) vvv
#    define __MGU_ATOMIC_INVALID_MEMORY_ORDER
#  endif // ^^^ !defined(_DEBUG) ^^^
#endif // __MGU_ATOMIC_INVALID_MEMORY_ORDER

#define __MGU_ATOMIC_STORE_RELEASE(_Width, _Ptr, _Desired)  \
    _Compiler_or_memory_barrier();                          \
    __iso_volatile_store##_Width((_Ptr), (_Desired));

#define __MGU_ATOMIC_STORE_SWITCH_PREFIX(_Width, _Ptr, _Desired) \
    case mgu_memory_order_relaxed:                               \
        __iso_volatile_store##_Width((_Ptr), (_Desired));        \
        return;                                                  \
    case mgu_memory_order_release:                               \
        __MGU_ATOMIC_STORE_RELEASE(_Width, _Ptr, _Desired)       \
        return;                                                  \
    default:                                                     \
    case mgu_memory_order_consume:                               \
    case mgu_memory_order_acquire:                               \
    case mgu_memory_order_acq_rel:                               \
        __MGU_ATOMIC_INVALID_MEMORY_ORDER;                       \
        _FALLTHROUGH;

#define __MGU_ATOMIC_POST_LOAD_BARRIER_AS_NEEDED(_Order_var) \
    switch (_Order_var) {                                    \
    case mgu_memory_order_relaxed:                           \
        break;                                               \
    case mgu_memory_order_consume:                           \
    case mgu_memory_order_acquire:                           \
    case mgu_memory_order_seq_cst:                           \
        _Compiler_or_memory_barrier();                       \
        break;                                               \
    case mgu_memory_order_release:                           \
    case mgu_memory_order_acq_rel:                           \
    default:                                                 \
        __MGU_ATOMIC_INVALID_MEMORY_ORDER;                   \
        break;                                               \
    }

#if defined(_M_IX86) || (defined(_M_X64) && !defined(_M_ARM64EC))
#define __MGU_ATOMIC_CHOOSE_INTRINSIC(_Order, _Result, _Intrinsic, ...) \
    __mgu_atomic_check_memory_order(_Order);                            \
    _Result = _Intrinsic(__VA_ARGS__)
#elif defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC)
#define __MGU_ATOMIC_CHOOSE_INTRINSIC(_Order, _Result, _Intrinsic, ...) \
    switch (_Order) {                                                   \
    case mgu_memory_order_relaxed:                                      \
        _Result = _INTRIN_RELAXED(_Intrinsic)(__VA_ARGS__);             \
        break;                                                          \
    case mgu_memory_order_consume:                                      \
    case mgu_memory_order_acquire:                                      \
        _Result = _INTRIN_ACQUIRE(_Intrinsic)(__VA_ARGS__);             \
        break;                                                          \
    case mgu_memory_order_release:                                      \
        _Result = _INTRIN_RELEASE(_Intrinsic)(__VA_ARGS__);             \
        break;                                                          \
    default:                                                            \
        __MGU_ATOMIC_INVALID_MEMORY_ORDER;                              \
        _FALLTHROUGH;                                                   \
    case mgu_memory_order_acq_rel:                                      \
    case mgu_memory_order_seq_cst:                                      \
        _Result = _Intrinsic(__VA_ARGS__);                              \
        break;                                                          \
    }
#endif 

#if __MGU_ATOMIC_USE_ARM64_LDAR_STLR == 1

#define __MGU_ATOMIC_LOAD_ACQUIRE_ARM64(_Width, _Ptr) \
    static_cast<__int##_Width>(__load_acquire##_Width(reinterpret_cast<const volatile unsigned __int##_Width*>(_Ptr)))

#define __MGU_ATOMIC_LOAD_ARM64(_Result, _Width, _Ptr, _Order_var) \
    switch (_Order_var) {                                          \
    case mgu_memory_order_relaxed:                                 \
        _Result = __iso_volatile_load##_Width(_Ptr);               \
        break;                                                     \
    case mgu_memory_order_consume:                                 \
    case mgu_memory_order_acquire:                                 \
    case mgu_memory_order_seq_cst:                                 \
        _Result = __MGU_ATOMIC_LOAD_ACQUIRE_ARM64(_Width, _Ptr);   \
        _Compiler_barrier();                                       \
        break;                                                     \
    case mgu_memory_order_release:                                 \
    case mgu_memory_order_acq_rel:                                 \
    default:                                                       \
        _Result = __iso_volatile_load##_Width(_Ptr);               \
        __MGU_ATOMIC_INVALID_MEMORY_ORDER;                         \
        break;                                                     \
    }

#endif

extern "C" inline void __mgu_atomic_check_memory_order(const unsigned int _Order) noexcept {
    if (_Order > mgu_memory_order_seq_cst) {
        _STL_REPORT_ERROR("Invalid memory order");
    }
}

MG_CAPI_INLINE bool __mgu_atomic_is_lock_free_bool  (volatile mgu_atomic_bool*   _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free_int8  (volatile mgu_atomic_char*   _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free_uint8 (volatile mgu_atomic_uchar*  _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free_int16 (volatile mgu_atomic_short*  _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free_uint16(volatile mgu_atomic_ushort* _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free_int32 (volatile mgu_atomic_long*   _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free_uint32(volatile mgu_atomic_ulong*  _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free_int64 (volatile mgu_atomic_llong*  _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free_uint64(volatile mgu_atomic_ullong* _obj) { return true; }
 
#ifdef __cplusplus

MG_CAPI_INLINE bool __mgu_atomic_is_lock_free(volatile mgu_atomic_bool*   _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free(volatile mgu_atomic_char*   _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free(volatile mgu_atomic_uchar*  _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free(volatile mgu_atomic_short*  _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free(volatile mgu_atomic_ushort* _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free(volatile mgu_atomic_long*   _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free(volatile mgu_atomic_ulong*  _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free(volatile mgu_atomic_llong*  _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free(volatile mgu_atomic_ullong* _obj) { return true; }

#elif !defined(__STDC_NO_ATOMICS__) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#define __mgu_atomic_is_lock_free(obj) \
_Generic((obj), \
    volatile mgu_atomic_bool*: __mgu_atomic_is_lock_free_bool,     \
    volatile mgu_atomic_char*: __mgu_atomic_is_lock_free_int8,     \
    volatile mgu_atomic_uchar*: __mgu_atomic_is_lock_free_uint8,   \
    volatile mgu_atomic_short*: __mgu_atomic_is_lock_free_int16,   \
    volatile mgu_atomic_ushort*: __mgu_atomic_is_lock_free_uint16, \
    volatile mgu_atomic_long*: __mgu_atomic_is_lock_free_int32,    \
    volatile mgu_atomic_ulong*: __mgu_atomic_is_lock_free_uint32,  \
    volatile mgu_atomic_llong*: __mgu_atomic_is_lock_free_int64,   \
    volatile mgu_atomic_ullong*: __mgu_atomic_is_lock_free_uint64  \
)(obj)

#else
#  error "C11 or C++11 required for _Generic support"
#endif

#define mgu_atomic_is_lock_free(obj) __mgu_atomic_is_lock_free(obj)


MG_CAPI_INLINE void __mgu_atomic_init_int8(
    volatile mgu_atomic_char* _obj, char _desired) { *_obj = _desired; }

MG_CAPI_INLINE void __mgu_atomic_init_uint8(
    volatile mgu_atomic_uchar* _obj, unsigned char _desired) { *_obj = _desired; }

MG_CAPI_INLINE void __mgu_atomic_init_int16(
    volatile mgu_atomic_short* _obj, short _desired) { *_obj = _desired; }

MG_CAPI_INLINE void __mgu_atomic_init_uint16(
    volatile mgu_atomic_ushort* _obj, unsigned short _desired) { *_obj = _desired; }

MG_CAPI_INLINE void __mgu_atomic_init_int32(
    volatile mgu_atomic_long* _obj, long _desired) { *_obj = _desired; }

MG_CAPI_INLINE void __mgu_atomic_init_uint32(
    volatile mgu_atomic_ulong* _obj, unsigned long _desired) { *_obj = _desired; }

MG_CAPI_INLINE void __mgu_atomic_init_int64(
    volatile mgu_atomic_llong* _obj, long long _desired) { *_obj = _desired; }

MG_CAPI_INLINE void __mgu_atomic_init_uint64(
    volatile mgu_atomic_ullong* _obj, unsigned long long _desired) { *_obj = _desired; }

MG_CAPI_INLINE void __mgu_atomic_init_bool(volatile mgu_atomic_bool* _obj, bool _desired) { *_obj = _desired; }


#ifdef __cplusplus

MG_CAPI_INLINE void __mgu_atomic_init(volatile mgu_atomic_bool*   _obj, bool _desired) { __mgu_atomic_init_bool(_obj, _desired); }
MG_CAPI_INLINE void __mgu_atomic_init(volatile mgu_atomic_char*   _obj, char _desired) { __mgu_atomic_init_int8(_obj, _desired); }
MG_CAPI_INLINE void __mgu_atomic_init(volatile mgu_atomic_uchar*  _obj, unsigned char _desired) { __mgu_atomic_init_uint8(_obj, _desired); }
MG_CAPI_INLINE void __mgu_atomic_init(volatile mgu_atomic_short*  _obj, short _desired) { __mgu_atomic_init_int16(_obj, _desired); }
MG_CAPI_INLINE void __mgu_atomic_init(volatile mgu_atomic_ushort* _obj, unsigned short _desired) { __mgu_atomic_init_uint16(_obj, _desired); }
MG_CAPI_INLINE void __mgu_atomic_init(volatile mgu_atomic_long*   _obj, long _desired) { __mgu_atomic_init_int32(_obj, _desired); }
MG_CAPI_INLINE void __mgu_atomic_init(volatile mgu_atomic_ulong*  _obj, unsigned long _desired) { __mgu_atomic_init_uint32(_obj, _desired); }
MG_CAPI_INLINE void __mgu_atomic_init(volatile mgu_atomic_llong*  _obj, long long _desired) { __mgu_atomic_init_int64(_obj, _desired); }
MG_CAPI_INLINE void __mgu_atomic_init(volatile mgu_atomic_ullong* _obj, unsigned long long _desired) { __mgu_atomic_init_uint64(_obj, _desired); }

#elif !defined(__STDC_NO_ATOMICS__) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#define __mgu_atomic_init(obj,desired)                     \
_Generic((obj),                                            \
    volatile mgu_atomic_bool*: __mgu_atomic_init_bool,     \
    volatile mgu_atomic_char*: __mgu_atomic_init_int8,     \
    volatile mgu_atomic_uchar*: __mgu_atomic_init_uint8,   \
    volatile mgu_atomic_short*: __mgu_atomic_init_int16,   \
    volatile mgu_atomic_ushort*: __mgu_atomic_init_uint16, \
    volatile mgu_atomic_long*: __mgu_atomic_init_int32,    \
    volatile mgu_atomic_ulong*: __mgu_atomic_init_uint32,  \
    volatile mgu_atomic_llong*: __mgu_atomic_init_int64,   \
    volatile mgu_atomic_ullong*: __mgu_atomic_init_uint64  \
)(obj,desired)

#else
#  error "C11 or C++11 required for _Generic support"
#endif

#define mgu_atomic_init(obj,desired) __mgu_atomic_init(obj,desired)


MG_CAPI_INLINE char __mgu_atomic_exchange_int8(
    volatile mgu_atomic_char* _obj, char _desired)
{
    return InterlockedExchange8(_obj, _desired);
}

MG_CAPI_INLINE unsigned char __mgu_atomic_exchange_uint8(
    volatile mgu_atomic_uchar* _obj, unsigned char _desired)
{
    char ret = InterlockedExchange8((volatile char*)_obj, *((char*)&_desired));
    return *((unsigned char*)&ret);
}

MG_CAPI_INLINE short __mgu_atomic_exchange_int16(
    volatile mgu_atomic_short* _obj, short _desired)
{
    return InterlockedExchange16(_obj, _desired);
}

MG_CAPI_INLINE unsigned short __mgu_atomic_exchange_uint16(
    volatile mgu_atomic_ushort* _obj, unsigned short _desired)
{
    short ret = InterlockedExchange16((volatile short*)_obj, *((short*)&_desired));
    return *((unsigned short*)&ret);
}

MG_CAPI_INLINE long __mgu_atomic_exchange_int32(
    volatile mgu_atomic_long* _obj, long _desired)
{
    return InterlockedExchange(_obj, _desired);
}

MG_CAPI_INLINE unsigned long __mgu_atomic_exchange_uint32(
    volatile mgu_atomic_ulong* _obj, unsigned long _desired)
{
    long ret = InterlockedExchange((volatile long*)_obj, *((long*)&_desired));
    return *((unsigned long*)&ret);
}

MG_CAPI_INLINE long long __mgu_atomic_exchange_int64(
    volatile mgu_atomic_llong* _obj, long long _desired)
{
    return InterlockedExchange64(_obj, _desired);
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_exchange_uint64(
    volatile mgu_atomic_ullong* _obj, unsigned long long _desired)
{
    long long ret = InterlockedExchange64((volatile long long*)_obj, *((long long*)&_desired));
    return *((unsigned long long*)&ret);
}

MG_CAPI_INLINE bool __mgu_atomic_exchange_bool(
    volatile mgu_atomic_bool* _obj, bool _desired)
{
    return (bool)__mgu_atomic_exchange_int8((volatile mgu_atomic_char*)_obj, (char)_desired);
}

#ifdef __cplusplus

MG_CAPI_INLINE bool __mgu_atomic_exchange(volatile mgu_atomic_bool* _obj, bool _desired) { return __mgu_atomic_exchange_bool(_obj, _desired); }
MG_CAPI_INLINE char __mgu_atomic_exchange(volatile mgu_atomic_char* _obj, char _desired) { return __mgu_atomic_exchange_int8(_obj, _desired); }
MG_CAPI_INLINE unsigned char __mgu_atomic_exchange(volatile mgu_atomic_uchar* _obj, unsigned char _desired) { return __mgu_atomic_exchange_uint8(_obj, _desired); }
MG_CAPI_INLINE short __mgu_atomic_exchange(volatile mgu_atomic_short* _obj, short _desired) { return __mgu_atomic_exchange_int16(_obj, _desired); }
MG_CAPI_INLINE unsigned short __mgu_atomic_exchange(volatile mgu_atomic_ushort* _obj, unsigned short _desired) { return __mgu_atomic_exchange_uint16(_obj, _desired); }
MG_CAPI_INLINE long __mgu_atomic_exchange(volatile mgu_atomic_long* _obj, long _desired) { return __mgu_atomic_exchange_int32(_obj, _desired); }
MG_CAPI_INLINE unsigned long __mgu_atomic_exchange(volatile mgu_atomic_ulong* _obj, unsigned long _desired) { return __mgu_atomic_exchange_uint32(_obj, _desired); }
MG_CAPI_INLINE long long __mgu_atomic_exchange(volatile mgu_atomic_llong* _obj, long long _desired) { return __mgu_atomic_exchange_int64(_obj, _desired); }
MG_CAPI_INLINE unsigned long long __mgu_atomic_exchange(volatile mgu_atomic_ullong* _obj, unsigned long long _desired) { return __mgu_atomic_exchange_uint64(_obj, _desired); }

#elif !defined(__STDC_NO_ATOMICS__) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#define __mgu_atomic_exchange(obj,desired)        \
_Generic((obj),                                   \
mgu_atomic_bool*: __mgu_atomic_exchange_bool,     \
mgu_atomic_char*: __mgu_atomic_exchange_int8,     \
mgu_atomic_uchar*: __mgu_atomic_exchange_uint8,   \
mgu_atomic_short*: __mgu_atomic_exchange_int16,   \
mgu_atomic_ushort*: __mgu_atomic_exchange_uint16, \
mgu_atomic_long*: __mgu_atomic_exchange_int32,    \
mgu_atomic_ulong*: __mgu_atomic_exchange_uint32,  \
mgu_atomic_llong*: __mgu_atomic_exchange_int64,   \
mgu_atomic_ullong*: __mgu_atomic_exchange_uint64  \
)(obj, desired)

#else
#  error "C11 or C++11 required for _Generic support"
#endif

#define mgu_atomic_exchange(obj,desired) __mgu_atomic_exchange(obj,desired)
#define mgu_atomic_store(obj,desired)    __mgu_atomic_exchange(obj,desired)


MG_CAPI_INLINE char __mgu_atomic_exchange_explicit_int8(
    volatile mgu_atomic_char* _obj, char _desired, mgu_memory_order _order)
{
    char ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, InterlockedExchange8, _obj, _desired);
    return ret;
}

MG_CAPI_INLINE unsigned char __mgu_atomic_exchange_explicit_uint8(
    volatile mgu_atomic_uchar* _obj, unsigned char _desired, mgu_memory_order _order)
{
    char ret = __mgu_atomic_exchange_explicit_int8(
        (volatile mgu_atomic_char*)_obj, *((char*)&_desired), _order);
    return *((unsigned char*)&ret);
}

MG_CAPI_INLINE short __mgu_atomic_exchange_explicit_int16(
    volatile mgu_atomic_short* _obj, short _desired, mgu_memory_order _order)
{
    short ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, InterlockedExchange16, _obj, _desired);
    return ret;
}

MG_CAPI_INLINE unsigned short __mgu_atomic_exchange_explicit_uint16(
    volatile mgu_atomic_ushort* _obj, unsigned short _desired, mgu_memory_order _order)
{
    short ret = __mgu_atomic_exchange_explicit_int16(
        (volatile mgu_atomic_short*)_obj, *((short*)&_desired), _order);
    return *((unsigned short*)&ret);
}

MG_CAPI_INLINE long __mgu_atomic_exchange_explicit_int32(
    volatile mgu_atomic_long* _obj, long _desired, mgu_memory_order _order)
{
    long ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, InterlockedExchange, _obj, _desired);
    return ret;
}

MG_CAPI_INLINE unsigned long __mgu_atomic_exchange_explicit_uint32(
    volatile mgu_atomic_ulong* _obj, unsigned long _desired, mgu_memory_order _order)
{
    long ret = __mgu_atomic_exchange_explicit_int32(
        (volatile mgu_atomic_long*)_obj, *((long*)&_desired), _order);
    return *((unsigned long*)&ret);
}

MG_CAPI_INLINE long long __mgu_atomic_exchange_explicit_int64(
    volatile mgu_atomic_llong* _obj, long long _desired, mgu_memory_order _order)
{
    long long ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, InterlockedExchange64, _obj, _desired);
    return ret;
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_exchange_explicit_uint64(
    volatile mgu_atomic_ullong* _obj, unsigned long long _desired, mgu_memory_order _order)
{
    long long ret = __mgu_atomic_exchange_explicit_int64(
        (volatile mgu_atomic_llong*)_obj, *((long long*)&_desired), _order);
    return *((unsigned long long*)&ret);
}

MG_CAPI_INLINE bool __mgu_atomic_exchange_explicit_bool(
    volatile mgu_atomic_bool* _obj, bool _desired, mgu_memory_order _order)
{
    return (bool)__mgu_atomic_exchange_explicit_int8(
        (volatile mgu_atomic_char*)_obj, (char)_desired, _order);
}

#ifdef __cplusplus

MG_CAPI_INLINE bool __mgu_atomic_exchange_explicit(
    volatile mgu_atomic_bool* _obj, bool _desired, mgu_memory_order _order) 
{ 
    return __mgu_atomic_exchange_explicit_bool(_obj, _desired, _order); 
}
MG_CAPI_INLINE char __mgu_atomic_exchange_explicit(
    volatile mgu_atomic_char* _obj, char _desired, mgu_memory_order _order)
{
    return __mgu_atomic_exchange_explicit_int8(_obj, _desired, _order);
}
MG_CAPI_INLINE unsigned char __mgu_atomic_exchange_explicit(
    volatile mgu_atomic_uchar* _obj, unsigned char _desired, mgu_memory_order _order)
{
    return __mgu_atomic_exchange_explicit_uint8(_obj, _desired, _order);
}
MG_CAPI_INLINE short __mgu_atomic_exchange_explicit(
    volatile mgu_atomic_short* _obj, short _desired, mgu_memory_order _order)
{
    return __mgu_atomic_exchange_explicit_int16(_obj, _desired, _order);
}
MG_CAPI_INLINE unsigned short __mgu_atomic_exchange_explicit(
    volatile mgu_atomic_ushort* _obj, unsigned short _desired, mgu_memory_order _order)
{
    return __mgu_atomic_exchange_explicit_uint16(_obj, _desired, _order);
}
MG_CAPI_INLINE long __mgu_atomic_exchange_explicit(
    volatile mgu_atomic_long* _obj, long _desired, mgu_memory_order _order)
{
    return __mgu_atomic_exchange_explicit_int32(_obj, _desired, _order);
}
MG_CAPI_INLINE unsigned long __mgu_atomic_exchange_explicit(
    volatile mgu_atomic_ulong* _obj, unsigned long _desired, mgu_memory_order _order)
{
    return __mgu_atomic_exchange_explicit_uint32(_obj, _desired, _order);
}
MG_CAPI_INLINE long long __mgu_atomic_exchange_explicit(
    volatile mgu_atomic_llong* _obj, long long _desired, mgu_memory_order _order)
{
    return __mgu_atomic_exchange_explicit_int64(_obj, _desired, _order);
}
MG_CAPI_INLINE unsigned long long __mgu_atomic_exchange_explicit(
    volatile mgu_atomic_ullong* _obj, unsigned long long _desired, mgu_memory_order _order)
{
    return __mgu_atomic_exchange_explicit_uint64(_obj, _desired, _order);
}

#elif !defined(__STDC_NO_ATOMICS__) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#define __mgu_atomic_exchange_explicit(obj,desired,order)      \
_Generic((obj),                                                \
    mgu_atomic_bool*: __mgu_atomic_exchange_explicit_bool,     \
    mgu_atomic_char*: __mgu_atomic_exchange_explicit_int8,     \
    mgu_atomic_uchar*: __mgu_atomic_exchange_explicit_uint8,   \
    mgu_atomic_short*: __mgu_atomic_exchange_explicit_int16,   \
    mgu_atomic_ushort*: __mgu_atomic_exchange_explicit_uint16, \
    mgu_atomic_long*: __mgu_atomic_exchange_explicit_int32,    \
    mgu_atomic_ulong*: __mgu_atomic_exchange_explicit_uint32,  \
    mgu_atomic_llong*: __mgu_atomic_exchange_explicit_int64,   \
    mgu_atomic_ullong*: __mgu_atomic_exchange_explicit_uint64  \
)(obj,desired,order)

#else
#  error "C11 or C++11 required for _Generic support"
#endif

#define mgu_atomic_exchange_explicit(obj,desired,order) __mgu_atomic_exchange_explicit(obj,desired,order)


MG_CAPI_INLINE void __mgu_atomic_store_explicit_int8(
    volatile mgu_atomic_char* _obj, char _desired, mgu_memory_order _order)
{
    switch (_order) {
    __MGU_ATOMIC_STORE_SWITCH_PREFIX(8, _obj, _desired)
    case mgu_memory_order_seq_cst:
        InterlockedExchange8(_obj, _desired);
        return;
    }
}

MG_CAPI_INLINE void __mgu_atomic_store_explicit_uint8(
    volatile mgu_atomic_uchar* _obj, unsigned char _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_int8(
        (volatile mgu_atomic_char*)_obj, *((char*)&_desired), _order);
}

MG_CAPI_INLINE void __mgu_atomic_store_explicit_int16(
    volatile mgu_atomic_short* _obj, short _desired, mgu_memory_order _order)
{
    switch (_order) {
    __MGU_ATOMIC_STORE_SWITCH_PREFIX(16, _obj, _desired)
    case mgu_memory_order_seq_cst:
        InterlockedExchange16(_obj, _desired);
        return;
    }
}

MG_CAPI_INLINE void __mgu_atomic_store_explicit_uint16(
    volatile mgu_atomic_ushort* _obj, unsigned short _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_int16(
        (volatile mgu_atomic_short*)_obj, *((short*)&_desired), _order);
}

MG_CAPI_INLINE void __mgu_atomic_store_explicit_int32(
    volatile mgu_atomic_long* _obj, long _desired, mgu_memory_order _order)
{
    switch (_order) {
    __MGU_ATOMIC_STORE_SWITCH_PREFIX(32, (volatile int*)_obj, _desired)
    case mgu_memory_order_seq_cst:
        InterlockedExchange(_obj, _desired);
        return;
    }
}

MG_CAPI_INLINE void __mgu_atomic_store_explicit_uint32(
    volatile mgu_atomic_ulong* _obj, unsigned long _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_int32(
        (volatile mgu_atomic_long*)_obj, *((long*)&_desired), _order);
}

MG_CAPI_INLINE void __mgu_atomic_store_explicit_int64(
    volatile mgu_atomic_llong* _obj, long long _desired, mgu_memory_order _order)
{
    switch (_order) {
    __MGU_ATOMIC_STORE_SWITCH_PREFIX(64, _obj, _desired)
    case mgu_memory_order_seq_cst:
        InterlockedExchange64(_obj, _desired);
        return;
    }
}

MG_CAPI_INLINE void __mgu_atomic_store_explicit_uint64(
    volatile mgu_atomic_ullong* _obj, unsigned long long _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_int64(
        (volatile mgu_atomic_llong*)_obj, *((long long*)&_desired), _order);
}

MG_CAPI_INLINE void __mgu_atomic_store_explicit_bool(
    volatile mgu_atomic_bool* _obj, bool _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_uint8(
        (volatile mgu_atomic_uchar*)_obj, *((unsigned char*)&_desired), _order);
}

#ifdef __cplusplus

MG_CAPI_INLINE void __mgu_atomic_store_explicit(
    volatile mgu_atomic_bool* _obj, bool _desired, mgu_memory_order _order) 
{ 
    __mgu_atomic_store_explicit_bool(_obj, _desired, _order); 
}
MG_CAPI_INLINE void __mgu_atomic_store_explicit(
    volatile mgu_atomic_char* _obj, char _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_int8(_obj, _desired, _order);
}
MG_CAPI_INLINE void __mgu_atomic_store_explicit(
    volatile mgu_atomic_uchar* _obj, unsigned char _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_uint8(_obj, _desired, _order);
}
MG_CAPI_INLINE void __mgu_atomic_store_explicit(
    volatile mgu_atomic_short* _obj, short _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_int16(_obj, _desired, _order);
}
MG_CAPI_INLINE void __mgu_atomic_store_explicit(
    volatile mgu_atomic_ushort* _obj, unsigned short _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_uint16(_obj, _desired, _order);
}
MG_CAPI_INLINE void __mgu_atomic_store_explicit(
    volatile mgu_atomic_long* _obj, long _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_int32(_obj, _desired, _order);
}
MG_CAPI_INLINE void __mgu_atomic_store_explicit(
    volatile mgu_atomic_ulong* _obj, unsigned long _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_uint32(_obj, _desired, _order);
}
MG_CAPI_INLINE void __mgu_atomic_store_explicit(
    volatile mgu_atomic_llong* _obj, long long _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_int64(_obj, _desired, _order);
}
MG_CAPI_INLINE void __mgu_atomic_store_explicit(
    volatile mgu_atomic_ullong* _obj, unsigned long long _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_uint64(_obj, _desired, _order);
}

#elif !defined(__STDC_NO_ATOMICS__) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#define __mgu_atomic_store_explicit(obj,desired,order)      \
_Generic((desired),                                         \
    mgu_atomic_bool*: __mgu_atomic_store_explicit_bool,     \
    mgu_atomic_char*: __mgu_atomic_store_explicit_int8,     \
    mgu_atomic_uchar*: __mgu_atomic_store_explicit_uint8,   \
    mgu_atomic_short*: __mgu_atomic_store_explicit_int16,   \
    mgu_atomic_ushort*: __mgu_atomic_store_explicit_uint16, \
    mgu_atomic_long*: __mgu_atomic_store_explicit_int32,    \
    mgu_atomic_ulong*: __mgu_atomic_store_explicit_uint32,  \
    mgu_atomic_llong*: __mgu_atomic_store_explicit_int64,   \
    mgu_atomic_ullong*: __mgu_atomic_store_explicit_uint64, \
)(obj, desired, order)

#endif

#define mgu_atomic_store_explicit(obj,desired,order) __mgu_atomic_store_explicit(obj,desired,order)


MG_CAPI_INLINE char __mgu_atomic_load_int8(const volatile mgu_atomic_char* _obj)
{
    char ret = __iso_volatile_load8(_obj);
    _Compiler_or_memory_barrier();
    return ret;
}

MG_CAPI_INLINE unsigned char __mgu_atomic_load_uint8(const volatile mgu_atomic_uchar* _obj)
{
    char ret = __mgu_atomic_load_int8((const volatile mgu_atomic_char*)_obj);
    return *((unsigned char*)&ret);
}

MG_CAPI_INLINE short __mgu_atomic_load_int16(const volatile mgu_atomic_short* _obj)
{
    short ret = __iso_volatile_load16(_obj);
    _Compiler_or_memory_barrier();
    return ret;
}

MG_CAPI_INLINE unsigned short __mgu_atomic_load_uint16(const volatile mgu_atomic_ushort* _obj)
{
    short ret = __mgu_atomic_load_int16((const volatile mgu_atomic_short*)_obj);
    return *((unsigned short*)&ret);
}

MG_CAPI_INLINE long __mgu_atomic_load_int32(const volatile mgu_atomic_long* _obj)
{        
    int ret = __iso_volatile_load32((volatile int*)_obj);
    _Compiler_or_memory_barrier();
    return ret;
}

MG_CAPI_INLINE unsigned long __mgu_atomic_load_uint32(const volatile mgu_atomic_ulong* _obj)
{
    long ret = __mgu_atomic_load_int32((const volatile mgu_atomic_long*)_obj);
    return *((unsigned long*)&ret);
}

MG_CAPI_INLINE long long __mgu_atomic_load_int64(const volatile mgu_atomic_llong* _obj)
{
    long long ret = __iso_volatile_load64(_obj);
    _Compiler_or_memory_barrier();
    return ret;
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_load_uint64(const volatile mgu_atomic_ullong* _obj)
{
    long long ret = __mgu_atomic_load_int64((const volatile mgu_atomic_llong*)_obj);
    return *((unsigned long long*) & ret);
}

MG_CAPI_INLINE bool __mgu_atomic_load_bool(const volatile mgu_atomic_bool* _obj)
{
    bool ret = __iso_volatile_load8((const volatile char*)_obj);
    _Compiler_or_memory_barrier();
    return ret;
}

#ifdef __cplusplus

MG_CAPI_INLINE bool __mgu_atomic_load(const volatile mgu_atomic_bool* _obj) { return __mgu_atomic_load_bool(_obj); }
MG_CAPI_INLINE char __mgu_atomic_load(const volatile mgu_atomic_char* _obj) { return __mgu_atomic_load_int8(_obj); }
MG_CAPI_INLINE unsigned char __mgu_atomic_load(const volatile mgu_atomic_uchar* _obj) { return __mgu_atomic_load_uint8(_obj); }
MG_CAPI_INLINE short __mgu_atomic_load(const volatile mgu_atomic_short* _obj) { return __mgu_atomic_load_int16(_obj); }
MG_CAPI_INLINE unsigned short __mgu_atomic_load(const volatile mgu_atomic_ushort* _obj) { return __mgu_atomic_load_uint16(_obj); }
MG_CAPI_INLINE long __mgu_atomic_load(const volatile mgu_atomic_long* _obj) { return __mgu_atomic_load_int32(_obj); }
MG_CAPI_INLINE unsigned long __mgu_atomic_load(const volatile mgu_atomic_ulong* _obj) { return __mgu_atomic_load_uint32(_obj); }
MG_CAPI_INLINE long long __mgu_atomic_load(const volatile mgu_atomic_llong* _obj) { return __mgu_atomic_load_int64(_obj); }
MG_CAPI_INLINE unsigned long long __mgu_atomic_load(const volatile mgu_atomic_ullong* _obj) { return __mgu_atomic_load_uint64(_obj); }

#elif !defined(__STDC_NO_ATOMICS__) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#define __mgu_atomic_load(obj)                 \
_Generic((obj),                                \
mgu_atomic_char*: __mgu_atomic_load_int8,      \
mgu_atomic_uchar*: __mgu_atomic_load_uint8,    \
mgu_atomic_short*: __mgu_atomic_load_int16,    \
mgu_atomic_ushort*: __mgu_atomic_load_uint16,  \
mgu_atomic_long*: __mgu_atomic_load_int32,     \
mgu_atomic_ulong*: __mgu_atomic_load_uint32,   \
mgu_atomic_llong*: __mgu_atomic_load_int64,    \
mgu_atomic_ullong*: __mgu_atomic_load_uint64   \
)(obj)

#endif

#define mgu_atomic_load(obj) __mgu_atomic_load(obj)


MG_CAPI_INLINE char __mgu_atomic_load_explicit_int8(
    const volatile mgu_atomic_char* _obj, mgu_memory_order _order)
{
    char ret;
    ret = __iso_volatile_load8(_obj);
    __MGU_ATOMIC_POST_LOAD_BARRIER_AS_NEEDED((_order))
    return ret;
}

MG_CAPI_INLINE unsigned char __mgu_atomic_load_explicit_uint8(
    const volatile mgu_atomic_uchar* _obj, mgu_memory_order _order)
{
    char ret = __mgu_atomic_load_explicit_int8(
        (volatile mgu_atomic_char*)_obj, _order);
    return *((unsigned char*)&ret);
}

MG_CAPI_INLINE short __mgu_atomic_load_explicit_int16(
    const volatile mgu_atomic_short* _obj, mgu_memory_order _order)
{
    short ret;
    ret = __iso_volatile_load16(_obj);
    __MGU_ATOMIC_POST_LOAD_BARRIER_AS_NEEDED((_order))
    return ret;
}

MG_CAPI_INLINE unsigned short __mgu_atomic_load_explicit_uint16(
    const volatile mgu_atomic_ushort* _obj, mgu_memory_order _order)
{
    short ret = __mgu_atomic_load_explicit_int16(
        (volatile mgu_atomic_short*)_obj, _order);
    return *((unsigned short*)&ret);
}

MG_CAPI_INLINE long __mgu_atomic_load_explicit_int32(
    const volatile mgu_atomic_long* _obj, mgu_memory_order _order)
{
    long ret;
#if __MGU_ATOMIC_USE_ARM64_LDAR_STLR == 1
        __MGU_ATOMIC_LOAD_ARM64(ret, 32, _obj, (_order))
#else
        ret = __iso_volatile_load32((const volatile int*)_obj);
        __MGU_ATOMIC_POST_LOAD_BARRIER_AS_NEEDED((_order))
#endif
    return ret;
}

MG_CAPI_INLINE unsigned long __mgu_atomic_load_explicit_uint32(
    const volatile mgu_atomic_ulong* _obj, mgu_memory_order _order)
{
    long ret = __mgu_atomic_load_explicit_int32(
        (volatile mgu_atomic_long*)_obj, _order);
    return *((unsigned long*)&ret);
}

MG_CAPI_INLINE long long __mgu_atomic_load_explicit_int64(
    const volatile mgu_atomic_llong* _obj, mgu_memory_order _order)
{
    long long ret;
#if __MGU_ATOMIC_USE_ARM64_LDAR_STLR == 1
    __MGU_ATOMIC_LOAD_ARM64(ret, 64, _obj, (_order))
#else
    ret = __iso_volatile_load64(_obj);
    __MGU_ATOMIC_POST_LOAD_BARRIER_AS_NEEDED((_order))
#endif
    return ret;
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_load_explicit_uint64(
    const volatile mgu_atomic_ullong* _obj, mgu_memory_order _order)
{
    long long ret = __mgu_atomic_load_explicit_int64(
        (volatile mgu_atomic_llong*)_obj, _order);
    return *((unsigned long long*)&ret);
}

MG_CAPI_INLINE bool __mgu_atomic_load_explicit_bool(
    const volatile mgu_atomic_bool* _obj, mgu_memory_order _order)
{
    return (bool)__mgu_atomic_load_explicit_int8(
        (volatile mgu_atomic_char*)_obj, _order);
}

#ifdef __cplusplus

MG_CAPI_INLINE bool __mgu_atomic_load_explicit(
    const volatile mgu_atomic_bool* _obj, mgu_memory_order _order)
{ 
    return __mgu_atomic_load_explicit_bool(_obj, _order); 
}
MG_CAPI_INLINE char __mgu_atomic_load_explicit(
    const volatile mgu_atomic_char* _obj, mgu_memory_order _order)
{
    return __mgu_atomic_load_explicit_int8(_obj, _order);
}
MG_CAPI_INLINE unsigned char __mgu_atomic_load_explicit(
    const volatile mgu_atomic_uchar* _obj, mgu_memory_order _order)
{
    return __mgu_atomic_load_explicit_uint8(_obj, _order);
}
MG_CAPI_INLINE short __mgu_atomic_load_explicit(
    const volatile mgu_atomic_short* _obj, mgu_memory_order _order)
{
    return __mgu_atomic_load_explicit_int16(_obj, _order);
}
MG_CAPI_INLINE unsigned short __mgu_atomic_load_explicit(
    const volatile mgu_atomic_ushort* _obj, mgu_memory_order _order)
{
    return __mgu_atomic_load_explicit_uint16(_obj, _order);
}
MG_CAPI_INLINE long __mgu_atomic_load_explicit(
    const volatile mgu_atomic_long* _obj, mgu_memory_order _order)
{
    return __mgu_atomic_load_explicit_int32(_obj, _order);
}
MG_CAPI_INLINE unsigned long __mgu_atomic_load_explicit(
    const volatile mgu_atomic_ulong* _obj, mgu_memory_order _order)
{
    return __mgu_atomic_load_explicit_uint32(_obj, _order);
}
MG_CAPI_INLINE long long __mgu_atomic_load_explicit(
    const volatile mgu_atomic_llong* _obj, mgu_memory_order _order)
{
    return __mgu_atomic_load_explicit_int64(_obj, _order);
}
MG_CAPI_INLINE unsigned long long __mgu_atomic_load_explicit(
    const volatile mgu_atomic_ullong* _obj, mgu_memory_order _order)
{
    return __mgu_atomic_load_explicit_uint64(_obj, _order);
}

#elif !defined(__STDC_NO_ATOMICS__) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#define __mgu_atomic_load_explicit(obj,order)                 \
_Generic((obj),                                               \
    mgu_atomic_bool* : __mgu_atomic_load_explicit_bool,       \
    mgu_atomic_char* : __mgu_atomic_load_explicit_int8,       \
    mgu_atomic_uchar* : __mgu_atomic_load_explicit_uint8,     \
    mgu_atomic_short* : __mgu_atomic_load_explicit_int16,     \
    mgu_atomic_ushort* : __mgu_atomic_load_explicit_uint16,   \
    mgu_atomic_long* : __mgu_atomic_load_explicit_int32,      \
    mgu_atomic_ulong* : __mgu_atomic_load_explicit_uint32,    \
    mgu_atomic_llong* : __mgu_atomic_load_explicit_int64,     \
    mgu_atomic_ullong* : __mgu_atomic_load_explicit_uint64    \
)(obj,order)

#endif

#define mgu_atomic_load_explicit(obj,order) __mgu_atomic_load_explicit(obj,order)

MG_CAPI_INLINE char __mgu_atomic_compare_exchange_strong_explicit_int8(
    volatile mgu_atomic_char* _obj, char* _expected, char _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    char expected_value = *_expected;
    char ret;

    __MGU_ATOMIC_CHOOSE_INTRINSIC(_success, ret, _InterlockedCompareExchange8, _obj, _desired, expected_value);
    if (ret == expected_value) {
        return true;
    }

    memcpy(_expected, &ret, sizeof(char));
    return false;
}

MG_CAPI_INLINE char __mgu_atomic_compare_exchange_weak_explicit_int8(
    volatile mgu_atomic_char* _obj, char* _expected, char _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_int8(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE unsigned char __mgu_atomic_compare_exchange_strong_explicit_uint8(
    volatile mgu_atomic_uchar* _obj, unsigned char* _expected, unsigned char _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    char expected_value = *((char*)_expected);
    char ret = __mgu_atomic_compare_exchange_strong_explicit_int8(
        (volatile mgu_atomic_char*)_obj, &expected_value, *((char*)&_desired), _success, _failure);
    memcpy(_expected, &expected_value, sizeof(unsigned char));
    return *((unsigned char*)&ret);
}

MG_CAPI_INLINE unsigned char __mgu_atomic_compare_exchange_weak_explicit_uint8(
    volatile mgu_atomic_uchar* _obj, unsigned char* _expected, unsigned char _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_uint8(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE short __mgu_atomic_compare_exchange_strong_explicit_int16(
    volatile mgu_atomic_short* _obj, short* _expected, short _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    short expected_value = *_expected;
    short ret;

    __MGU_ATOMIC_CHOOSE_INTRINSIC(_success, ret, InterlockedCompareExchange16, _obj, _desired, expected_value);
    if (ret == expected_value) {
        return true;
    }

    memcpy(_expected, &ret, sizeof(short));
    return false;
}

MG_CAPI_INLINE short __mgu_atomic_compare_exchange_weak_explicit_int16(
    volatile mgu_atomic_short* _obj, short* _expected, short _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_int16(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE unsigned short __mgu_atomic_compare_exchange_strong_explicit_uint16(
    volatile mgu_atomic_ushort* _obj, unsigned short* _expected, unsigned short _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    short expected_value = *((short*)_expected);
    short ret = __mgu_atomic_compare_exchange_strong_explicit_int16(
        (volatile mgu_atomic_short*)_obj, &expected_value, *((short*)&_desired), _success, _failure);
    memcpy(_expected, &expected_value, sizeof(unsigned short));
    return *((unsigned short*)&ret);
}

MG_CAPI_INLINE unsigned short __mgu_atomic_compare_exchange_weak_explicit_uint16(
    volatile mgu_atomic_ushort* _obj, unsigned short* _expected, unsigned short _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_uint16(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE long __mgu_atomic_compare_exchange_strong_explicit_int32(
    volatile mgu_atomic_long* _obj, long* _expected, long _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    long expected_value = *_expected;
    long ret;

    __MGU_ATOMIC_CHOOSE_INTRINSIC(_success, ret, _InterlockedCompareExchange, _obj, _desired, expected_value);
    if (ret == expected_value) {
        return true;
    }

    memcpy(_expected, &ret, sizeof(long));
    return false;
}

MG_CAPI_INLINE long __mgu_atomic_compare_exchange_weak_explicit_int32(
    volatile mgu_atomic_long* _obj, long* _expected, long _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_int32(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE unsigned long __mgu_atomic_compare_exchange_strong_explicit_uint32(
    volatile mgu_atomic_ulong* _obj, unsigned long* _expected, unsigned long _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    long expected_value = *((long*)_expected);
    long ret = __mgu_atomic_compare_exchange_strong_explicit_int32(
        (volatile mgu_atomic_long*)_obj, &expected_value, *((long*)&_desired), _success, _failure);
    memcpy(_expected, &expected_value, sizeof(unsigned long));
    return *((unsigned long*)&ret);
}

MG_CAPI_INLINE unsigned long __mgu_atomic_compare_exchange_weak_explicit_uint32(
    volatile mgu_atomic_ulong* _obj, unsigned long* _expected, unsigned long _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_uint32(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE long long __mgu_atomic_compare_exchange_strong_explicit_int64(
    volatile mgu_atomic_llong* _obj, long long* _expected, long long _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    long long expected_value = *_expected;
    long long ret;

    __MGU_ATOMIC_CHOOSE_INTRINSIC(_success, ret, _InterlockedCompareExchange64, _obj, _desired, expected_value);
    if (ret == expected_value) {
        return true;
    }

    memcpy(_expected, &ret, sizeof(long long));
    return false;
}

MG_CAPI_INLINE long long __mgu_atomic_compare_exchange_weak_explicit_int64(
    volatile mgu_atomic_llong* _obj, long long* _expected, long long _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_int64(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_compare_exchange_strong_explicit_uint64(
    volatile mgu_atomic_ullong* _obj, unsigned long long* _expected, unsigned long long _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    long long expected_value = *((long long*)_expected);
    long long ret = __mgu_atomic_compare_exchange_strong_explicit_int64(
        (volatile mgu_atomic_llong*)_obj, &expected_value, *((long long*)&_desired), _success, _failure);
    memcpy(_expected, &expected_value, sizeof(unsigned long long));
    return *((unsigned long long*)&ret);
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_compare_exchange_weak_explicit_uint64(
    volatile mgu_atomic_ullong* _obj, unsigned long long* _expected, unsigned long long _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_uint64(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit_bool(
    volatile mgu_atomic_bool* _obj, bool* _expected, bool _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return (bool)__mgu_atomic_compare_exchange_strong_explicit_int8(
        (volatile mgu_atomic_char*)_obj, (char*)_expected, (char)_desired, _success, _failure);
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit_bool(
    volatile mgu_atomic_bool* _obj, bool* _expected, bool _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return (bool)__mgu_atomic_compare_exchange_weak_explicit_int8(
        (volatile mgu_atomic_char*)_obj, (char*)_expected, (char)_desired, _success, _failure);
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit(
    volatile mgu_atomic_bool* _obj, bool* _expected, bool _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_bool(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE char __mgu_atomic_compare_exchange_strong_explicit(
    volatile mgu_atomic_char* _obj, char* _expected, char _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_int8(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE unsigned char __mgu_atomic_compare_exchange_strong_explicit(
    volatile mgu_atomic_uchar* _obj, unsigned char* _expected, unsigned char _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_uint8(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE short __mgu_atomic_compare_exchange_strong_explicit(
    volatile mgu_atomic_short* _obj, short* _expected, short _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_int16(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE unsigned short __mgu_atomic_compare_exchange_strong_explicit(
    volatile mgu_atomic_ushort* _obj, unsigned short* _expected, unsigned short _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_uint16(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE long __mgu_atomic_compare_exchange_strong_explicit(
    volatile mgu_atomic_long* _obj, long* _expected, long _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_int32(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE unsigned long __mgu_atomic_compare_exchange_strong_explicit(
    volatile mgu_atomic_ulong* _obj, unsigned long* _expected, unsigned long _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_uint32(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE long long __mgu_atomic_compare_exchange_strong_explicit(
    volatile mgu_atomic_llong* _obj, long long* _expected, long long _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_int64(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE unsigned long long __mgu_atomic_compare_exchange_strong_explicit(
    volatile mgu_atomic_ullong* _obj, unsigned long long* _expected, unsigned long long _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_uint64(_obj, _expected, _desired, _success, _failure);
}

#ifdef __cplusplus

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit(
    volatile mgu_atomic_bool* _obj, bool* _expected, bool _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_weak_explicit_bool(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE char __mgu_atomic_compare_exchange_weak_explicit(
    volatile mgu_atomic_char* _obj, char* _expected, char _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_weak_explicit_int8(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE unsigned char __mgu_atomic_compare_exchange_weak_explicit(
    volatile mgu_atomic_uchar* _obj, unsigned char* _expected, unsigned char _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_weak_explicit_uint8(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE short __mgu_atomic_compare_exchange_weak_explicit(
    volatile mgu_atomic_short* _obj, short* _expected, short _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_weak_explicit_int16(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE unsigned short __mgu_atomic_compare_exchange_weak_explicit(
    volatile mgu_atomic_ushort* _obj, unsigned short* _expected, unsigned short _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_weak_explicit_uint16(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE long __mgu_atomic_compare_exchange_weak_explicit(
    volatile mgu_atomic_long* _obj, long* _expected, long _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_weak_explicit_int32(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE unsigned long __mgu_atomic_compare_exchange_weak_explicit(
    volatile mgu_atomic_ulong* _obj, unsigned long* _expected, unsigned long _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_weak_explicit_uint32(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE long long __mgu_atomic_compare_exchange_weak_explicit(
    volatile mgu_atomic_llong* _obj, long long* _expected, long long _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_weak_explicit_int64(_obj, _expected, _desired, _success, _failure);
}
MG_CAPI_INLINE unsigned long long __mgu_atomic_compare_exchange_weak_explicit(
    volatile mgu_atomic_ullong* _obj, unsigned long long* _expected, unsigned long long _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_weak_explicit_uint64(_obj, _expected, _desired, _success, _failure);
}

#elif !defined(__STDC_NO_ATOMICS__) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#define __mgu_atomic_compare_exchange_strong_explicit(obj,expected,desired,success,failure) \
 _Generic((obj),                                                                            \
    mgu_atomic_bool*: __mgu_atomic_compare_exchange_strong_explicit_bool,                   \
    mgu_atomic_char*: __mgu_atomic_compare_exchange_strong_explicit_int8,                   \
    mgu_atomic_uchar*: __mgu_atomic_compare_exchange_strong_explicit_uint8,                 \
    mgu_atomic_short*: __mgu_atomic_compare_exchange_strong_explicit_int16,                 \
    mgu_atomic_ushort*: __mgu_atomic_compare_exchange_strong_explicit_uint16,               \
    mgu_atomic_int*: __mgu_atomic_compare_exchange_strong_explicit_int32,                   \
    mgu_atomic_uint*: __mgu_atomic_compare_exchange_strong_explicit_uint32,                 \
    mgu_atomic_long*: __mgu_atomic_compare_exchange_strong_explicit_int32,                  \
    mgu_atomic_ulong*: __mgu_atomic_compare_exchange_strong_explicit_uint32,                \
    mgu_atomic_llong*: __mgu_atomic_compare_exchange_strong_explicit_int64,                 \
    mgu_atomic_ullong*: __mgu_atomic_compare_exchange_strong_explicit_uint64,               \
)(obj, expected, desired, success, failure)

#define __mgu_atomic_compare_exchange_weak_explicit(obj,expected,desired,success,failure)   \
_Generic((obj),                                                                             \
    mgu_atomic_bool*: __mgu_atomic_compare_exchange_weak_explicit_bool,                     \
    mgu_atomic_char*: __mgu_atomic_compare_exchange_weak_explicit_int8,                     \
    mgu_atomic_uchar*: __mgu_atomic_compare_exchange_weak_explicit_uint8,                   \
    mgu_atomic_short*: __mgu_atomic_compare_exchange_weak_explicit_int16,                   \
    mgu_atomic_ushort*: __mgu_atomic_compare_exchange_weak_explicit_uint16,                 \
    mgu_atomic_int*: __mgu_atomic_compare_exchange_weak_explicit_int32,                     \
    mgu_atomic_uint*: __mgu_atomic_compare_exchange_weak_explicit_uint32,                   \
    mgu_atomic_long*: __mgu_atomic_compare_exchange_weak_explicit_int32,                    \
    mgu_atomic_ulong*: __mgu_atomic_compare_exchange_weak_explicit_uint32,                  \
    mgu_atomic_llong*: __mgu_atomic_compare_exchange_weak_explicit_int64,                   \
    mgu_atomic_ullong*: __mgu_atomic_compare_exchange_weak_explicit_uint64,                 \
)(obj, expected, desired, success, failure)

#endif

#define mgu_atomic_compare_exchange_strong_explicit(obj,expected,desired,success,failure)   \
      __mgu_atomic_compare_exchange_strong_explicit(obj,expected,desired,success,failure)

#define mgu_atomic_compare_exchange_weak_explicit(obj,expected,desired,success,failure)   \
      __mgu_atomic_compare_exchange_weak_explicit(obj,expected,desired,success,failure)


MG_CAPI_INLINE char __mgu_atomic_compare_exchange_strong_int8(
    volatile mgu_atomic_char* _obj, char* _expected, char _desired)
{
    return __mgu_atomic_compare_exchange_strong_explicit_int8(_obj, _expected, _desired, mgu_memory_order_seq_cst, mgu_memory_order_seq_cst);
}

MG_CAPI_INLINE char __mgu_atomic_compare_exchange_weak_int8(
    volatile mgu_atomic_char* _obj, char* _expected, char _desired)
{
    return __mgu_atomic_compare_exchange_weak_explicit_int8(_obj, _expected, _desired, mgu_memory_order_seq_cst, mgu_memory_order_seq_cst);
}

MG_CAPI_INLINE unsigned char __mgu_atomic_compare_exchange_strong_uint8(
    volatile mgu_atomic_uchar* _obj, unsigned char* _expected, unsigned char _desired)
{
    return __mgu_atomic_compare_exchange_strong_explicit_uint8(_obj, _expected, _desired, mgu_memory_order_seq_cst, mgu_memory_order_seq_cst);
}

MG_CAPI_INLINE unsigned char __mgu_atomic_compare_exchange_weak_uint8(
    volatile mgu_atomic_uchar* _obj, unsigned char* _expected, unsigned char _desired)
{
    return __mgu_atomic_compare_exchange_weak_explicit_uint8(_obj, _expected, _desired, mgu_memory_order_seq_cst, mgu_memory_order_seq_cst);
}

MG_CAPI_INLINE short __mgu_atomic_compare_exchange_strong_int16(
    volatile mgu_atomic_short* _obj, short* _expected, short _desired)
{
    return __mgu_atomic_compare_exchange_strong_explicit_int16(_obj, _expected, _desired, mgu_memory_order_seq_cst, mgu_memory_order_seq_cst);
}

MG_CAPI_INLINE short __mgu_atomic_compare_exchange_weak_int16(
    volatile mgu_atomic_short* _obj, short* _expected, short _desired)
{
    return __mgu_atomic_compare_exchange_weak_explicit_int16(_obj, _expected, _desired, mgu_memory_order_seq_cst, mgu_memory_order_seq_cst);
}

MG_CAPI_INLINE unsigned short __mgu_atomic_compare_exchange_strong_uint16(
    volatile mgu_atomic_ushort* _obj, unsigned short* _expected, unsigned short _desired)
{
    return __mgu_atomic_compare_exchange_strong_explicit_uint16(_obj, _expected, _desired, mgu_memory_order_seq_cst, mgu_memory_order_seq_cst);
}

MG_CAPI_INLINE unsigned short __mgu_atomic_compare_exchange_weak_uint16(
    volatile mgu_atomic_ushort* _obj, unsigned short* _expected, unsigned short _desired)
{
    return __mgu_atomic_compare_exchange_weak_explicit_uint16(_obj, _expected, _desired, mgu_memory_order_seq_cst, mgu_memory_order_seq_cst);
}

MG_CAPI_INLINE long __mgu_atomic_compare_exchange_strong_int32(
    volatile mgu_atomic_long* _obj, long* _expected, long _desired)
{
    return __mgu_atomic_compare_exchange_strong_explicit_int32(_obj, _expected, _desired, mgu_memory_order_seq_cst, mgu_memory_order_seq_cst);
}

MG_CAPI_INLINE long __mgu_atomic_compare_exchange_weak_int32(
    volatile mgu_atomic_long* _obj, long* _expected, long _desired)
{
    return __mgu_atomic_compare_exchange_weak_explicit_int32(_obj, _expected, _desired, mgu_memory_order_seq_cst, mgu_memory_order_seq_cst);
}

MG_CAPI_INLINE unsigned long __mgu_atomic_compare_exchange_strong_uint32(
    volatile mgu_atomic_ulong* _obj, unsigned long* _expected, unsigned long _desired)
{
    return __mgu_atomic_compare_exchange_strong_explicit_uint32(_obj, _expected, _desired, mgu_memory_order_seq_cst, mgu_memory_order_seq_cst);
}

MG_CAPI_INLINE unsigned long __mgu_atomic_compare_exchange_weak_uint32(
    volatile mgu_atomic_ulong* _obj, unsigned long* _expected, unsigned long _desired)
{
    return __mgu_atomic_compare_exchange_weak_explicit_uint32(_obj, _expected, _desired, mgu_memory_order_seq_cst, mgu_memory_order_seq_cst);
}

MG_CAPI_INLINE long long __mgu_atomic_compare_exchange_strong_int64(
    volatile mgu_atomic_llong* _obj, long long* _expected, long long _desired)
{
    return __mgu_atomic_compare_exchange_strong_explicit_int64(_obj, _expected, _desired, mgu_memory_order_seq_cst, mgu_memory_order_seq_cst);
}

MG_CAPI_INLINE long long __mgu_atomic_compare_exchange_weak_int64(
    volatile mgu_atomic_llong* _obj, long long* _expected, long long _desired)
{
    return __mgu_atomic_compare_exchange_weak_explicit_int64(_obj, _expected, _desired, mgu_memory_order_seq_cst, mgu_memory_order_seq_cst);
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_compare_exchange_strong_uint64(
    volatile mgu_atomic_ullong* _obj, unsigned long long* _expected, unsigned long long _desired)
{
    return __mgu_atomic_compare_exchange_strong_explicit_uint64(_obj, _expected, _desired, mgu_memory_order_seq_cst, mgu_memory_order_seq_cst);
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_compare_exchange_weak_uint64(
    volatile mgu_atomic_ullong* _obj, unsigned long long* _expected, unsigned long long _desired)
{
    return __mgu_atomic_compare_exchange_weak_explicit_uint64(_obj, _expected, _desired, mgu_memory_order_seq_cst, mgu_memory_order_seq_cst);
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_bool(
    volatile mgu_atomic_bool* _obj, bool* _expected, bool _desired)
{
    return __mgu_atomic_compare_exchange_strong_explicit_bool(_obj, _expected, _desired, mgu_memory_order_seq_cst, mgu_memory_order_seq_cst);
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_bool(
    volatile mgu_atomic_bool* _obj, bool* _expected, bool _desired)
{
    return __mgu_atomic_compare_exchange_weak_explicit_bool(_obj, _expected, _desired, mgu_memory_order_seq_cst, mgu_memory_order_seq_cst);
}

#if __cplusplus

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong(
    volatile mgu_atomic_bool* _obj, bool* _expected, bool _desired)
{
    return __mgu_atomic_compare_exchange_strong_bool(_obj, _expected, _desired);
}
MG_CAPI_INLINE char __mgu_atomic_compare_exchange_strong(
    volatile mgu_atomic_char* _obj, char* _expected, char _desired)
{
    return __mgu_atomic_compare_exchange_strong_int8(_obj, _expected, _desired);
}
MG_CAPI_INLINE unsigned char __mgu_atomic_compare_exchange_strong(
    volatile mgu_atomic_uchar* _obj, unsigned char* _expected, unsigned char _desired)
{
    return __mgu_atomic_compare_exchange_strong_uint8(_obj, _expected, _desired);
}
MG_CAPI_INLINE short __mgu_atomic_compare_exchange_strong(
    volatile mgu_atomic_short* _obj, short* _expected, short _desired)
{
    return __mgu_atomic_compare_exchange_strong_int16(_obj, _expected, _desired);
}
MG_CAPI_INLINE unsigned short __mgu_atomic_compare_exchange_strong(
    volatile mgu_atomic_ushort* _obj, unsigned short* _expected, unsigned short _desired)
{
    return __mgu_atomic_compare_exchange_strong_uint16(_obj, _expected, _desired);
}
MG_CAPI_INLINE long __mgu_atomic_compare_exchange_strong(
    volatile mgu_atomic_long* _obj, long* _expected, long _desired)
{
    return __mgu_atomic_compare_exchange_strong_int32(_obj, _expected, _desired);
}
MG_CAPI_INLINE unsigned long __mgu_atomic_compare_exchange_strong(
    volatile mgu_atomic_ulong* _obj, unsigned long* _expected, unsigned long _desired)
{
    return __mgu_atomic_compare_exchange_strong_uint32(_obj, _expected, _desired);
}
MG_CAPI_INLINE long long __mgu_atomic_compare_exchange_strong(
    volatile mgu_atomic_llong* _obj, long long* _expected, long long _desired)
{
    return __mgu_atomic_compare_exchange_strong_int64(_obj, _expected, _desired);
}
MG_CAPI_INLINE unsigned long long __mgu_atomic_compare_exchange_strong(
    volatile mgu_atomic_ullong* _obj, unsigned long long* _expected, unsigned long long _desired)
{
    return __mgu_atomic_compare_exchange_strong_uint64(_obj, _expected, _desired);
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak(
    volatile mgu_atomic_bool* _obj, bool* _expected, bool _desired)
{
    return __mgu_atomic_compare_exchange_weak_bool(_obj, _expected, _desired);
}
MG_CAPI_INLINE char __mgu_atomic_compare_exchange_weak(
    volatile mgu_atomic_char* _obj, char* _expected, char _desired)
{
    return __mgu_atomic_compare_exchange_weak_int8(_obj, _expected, _desired);
}
MG_CAPI_INLINE unsigned char __mgu_atomic_compare_exchange_weak(
    volatile mgu_atomic_uchar* _obj, unsigned char* _expected, unsigned char _desired)
{
    return __mgu_atomic_compare_exchange_weak_uint8(_obj, _expected, _desired);
}
MG_CAPI_INLINE short __mgu_atomic_compare_exchange_weak(
    volatile mgu_atomic_short* _obj, short* _expected, short _desired)
{
    return __mgu_atomic_compare_exchange_weak_int16(_obj, _expected, _desired);
}
MG_CAPI_INLINE unsigned short __mgu_atomic_compare_exchange_weak(
    volatile mgu_atomic_ushort* _obj, unsigned short* _expected, unsigned short _desired)
{
    return __mgu_atomic_compare_exchange_weak_uint16(_obj, _expected, _desired);
}
MG_CAPI_INLINE long __mgu_atomic_compare_exchange_weak(
    volatile mgu_atomic_long* _obj, long* _expected, long _desired)
{
    return __mgu_atomic_compare_exchange_weak_int32(_obj, _expected, _desired);
}
MG_CAPI_INLINE unsigned long __mgu_atomic_compare_exchange_weak(
    volatile mgu_atomic_ulong* _obj, unsigned long* _expected, unsigned long _desired)
{
    return __mgu_atomic_compare_exchange_weak_uint32(_obj, _expected, _desired);
}
MG_CAPI_INLINE long long __mgu_atomic_compare_exchange_weak(
    volatile mgu_atomic_llong* _obj, long long* _expected, long long _desired)
{
    return __mgu_atomic_compare_exchange_weak_int64(_obj, _expected, _desired);
}
MG_CAPI_INLINE unsigned long long __mgu_atomic_compare_exchange_weak(
    volatile mgu_atomic_ullong* _obj, unsigned long long* _expected, unsigned long long _desired)
{
    return __mgu_atomic_compare_exchange_weak_uint64(_obj, _expected, _desired);
}

#elif !defined(__STDC_NO_ATOMICS__) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#define __mgu_atomic_compare_exchange_strong(obj,expected,desired)   \
_Generic((obj), \
    mgu_atomic_bool*: __mgu_atomic_compare_exchange_strong_bool,     \
    mgu_atomic_char*: __mgu_atomic_compare_exchange_strong_int8,     \
    mgu_atomic_uchar*: __mgu_atomic_compare_exchange_strong_uint8,   \
    mgu_atomic_short*: __mgu_atomic_compare_exchange_strong_int16,   \
    mgu_atomic_ushort*: __mgu_atomic_compare_exchange_strong_uint16, \
    mgu_atomic_long*: __mgu_atomic_compare_exchange_strong_int32,    \
    mgu_atomic_ulong*: __mgu_atomic_compare_exchange_strong_uint32,  \
    mgu_atomic_llong*: __mgu_atomic_compare_exchange_strong_int64,   \
    mgu_atomic_ullong*: __mgu_atomic_compare_exchange_strong_uint64  \
)(obj, expected, desired)

#define __mgu_atomic_compare_exchange_weak(obj,expected,desired)     \
_Generic((obj), \
    mgu_atomic_bool*: __mgu_atomic_compare_exchange_weak_bool,       \
    mgu_atomic_char*: __mgu_atomic_compare_exchange_weak_int8,       \
    mgu_atomic_uchar*: __mgu_atomic_compare_exchange_weak_uint8,     \
    mgu_atomic_short*: __mgu_atomic_compare_exchange_weak_int16,     \
    mgu_atomic_ushort*: __mgu_atomic_compare_exchange_weak_uint16,   \
    mgu_atomic_long*: __mgu_atomic_compare_exchange_weak_int32,      \
    mgu_atomic_ulong*: __mgu_atomic_compare_exchange_weak_uint32,    \
    mgu_atomic_llong*: __mgu_atomic_compare_exchange_weak_int64,     \
    mgu_atomic_ullong*: __mgu_atomic_compare_exchange_weak_uint64    \
)(obj, expected, desired)

#endif

#define mgu_atomic_compare_exchange_strong(obj,expected,desired) __mgu_atomic_compare_exchange_strong(obj,expected,desired)

#define mgu_atomic_compare_exchange_weak(obj,expected,desired) __mgu_atomic_compare_exchange_weak(obj,expected,desired)


MG_CAPI_INLINE char __mgu_atomic_fetch_add_explicit_int8(
    volatile mgu_atomic_char* _obj, char _arg, mgu_memory_order _order)
{
    char ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedExchangeAdd8, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE unsigned char __mgu_atomic_fetch_add_explicit_uint8(
    volatile mgu_atomic_uchar* _obj, unsigned char _arg, mgu_memory_order _order)
{
    char ret = __mgu_atomic_fetch_add_explicit_int8(
        (volatile mgu_atomic_char*)_obj, *((char*)&_arg), _order);
    return *((unsigned char*)&ret);
}

MG_CAPI_INLINE short __mgu_atomic_fetch_add_explicit_int16(
    volatile mgu_atomic_short* _obj, short _arg, mgu_memory_order _order)
{
    short ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedExchangeAdd16, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE unsigned short __mgu_atomic_fetch_add_explicit_uint16(
    volatile mgu_atomic_ushort* _obj, unsigned short _arg, mgu_memory_order _order)
{
    short ret = __mgu_atomic_fetch_add_explicit_int16(
        (volatile mgu_atomic_short*)_obj, *((short*)&_arg), _order);
    return *((unsigned short*)&ret);
}

MG_CAPI_INLINE long __mgu_atomic_fetch_add_explicit_int32(
    volatile mgu_atomic_long* _obj, long _arg, mgu_memory_order _order)
{
    long ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedExchangeAdd, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE unsigned long __mgu_atomic_fetch_add_explicit_uint32(
    volatile mgu_atomic_ulong* _obj, unsigned long _arg, mgu_memory_order _order)
{
    long ret = __mgu_atomic_fetch_add_explicit_int32(
        (volatile mgu_atomic_long*)_obj, *((long*)&_arg), _order);
    return *((unsigned long*)&ret);
}

MG_CAPI_INLINE long long __mgu_atomic_fetch_add_explicit_int64(
    volatile mgu_atomic_llong* _obj, long long _arg, mgu_memory_order _order)
{
    long long ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedExchangeAdd64, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_fetch_add_explicit_uint64(
    volatile mgu_atomic_ullong* _obj, unsigned long long _arg, mgu_memory_order _order)
{
    long long ret = __mgu_atomic_fetch_add_explicit_int64(
        (volatile mgu_atomic_llong*)_obj, *((long long*)&_arg), _order);
    return *((unsigned long long*)&ret);
}

#ifdef __cplusplus

MG_CAPI_INLINE char __mgu_atomic_fetch_add_explicit(
    volatile mgu_atomic_char* _obj, char _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_int8(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned char __mgu_atomic_fetch_add_explicit(
    volatile mgu_atomic_uchar* _obj, unsigned char _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_uint8(_obj, _arg, _order);
}

MG_CAPI_INLINE short __mgu_atomic_fetch_add_explicit(
    volatile mgu_atomic_short* _obj, short _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_int16(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned short __mgu_atomic_fetch_add_explicit(
    volatile mgu_atomic_ushort* _obj, unsigned short _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_uint16(_obj, _arg, _order);
}

MG_CAPI_INLINE long __mgu_atomic_fetch_add_explicit(
    volatile mgu_atomic_long* _obj, long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_int32(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned long __mgu_atomic_fetch_add_explicit(
    volatile mgu_atomic_ulong* _obj, unsigned long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_uint32(_obj, _arg, _order);
}

MG_CAPI_INLINE long long __mgu_atomic_fetch_add_explicit(
    volatile mgu_atomic_llong* _obj, long long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_int64(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_fetch_add_explicit(
    volatile mgu_atomic_ullong* _obj, unsigned long long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_uint64(_obj, _arg, _order);
}

#elif !defined(__STDC_NO_ATOMICS__) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#define __mgu_atomic_fetch_add_explicit(obj,arg,order)               \
_Generic((obj),                                                      \
mgu_atomic_char*: __mgu_atomic_fetch_add_explicit_int8,              \
mgu_atomic_uchar*: __mgu_atomic_fetch_add_explicit_uint8,            \
mgu_atomic_short*: __mgu_atomic_fetch_add_explicit_int16,            \
mgu_atomic_ushort*: __mgu_atomic_fetch_add_explicit_uint16,          \
mgu_atomic_long*: __mgu_atomic_fetch_add_explicit_int32,             \
mgu_atomic_ulong*: __mgu_atomic_fetch_add_explicit_uint32,           \
mgu_atomic_llong*: __mgu_atomic_fetch_add_explicit_int64,            \
mgu_atomic_ullong*: __mgu_atomic_fetch_add_explicit_uint64           \
)(obj,arg,order)

#endif

#define mgu_atomic_fetch_add_explicit(obj,arg,order) __mgu_atomic_fetch_add_explicit(obj,arg,order)
#define mgu_atomic_fetch_add(obj,arg) __mgu_atomic_fetch_add_explicit(obj,arg,mgu_memory_order_seq_cst)

MG_CAPI_INLINE char __mgu_atomic_fetch_sub_explicit_int8(
    volatile mgu_atomic_char* _obj, char _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_int8(_obj, -_arg, _order);
}

MG_CAPI_INLINE unsigned char __mgu_atomic_fetch_sub_explicit_uint8(
    volatile mgu_atomic_uchar* _obj, unsigned char _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_int8(
        (volatile mgu_atomic_char*)_obj, -(*((char*)&_arg)), _order);
}

MG_CAPI_INLINE short __mgu_atomic_fetch_sub_explicit_int16(
    volatile mgu_atomic_short* _obj, short _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_int16(_obj, -_arg, _order);
}

MG_CAPI_INLINE unsigned short __mgu_atomic_fetch_sub_explicit_uint16(
    volatile mgu_atomic_ushort* _obj, unsigned short _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_int16(
        (volatile mgu_atomic_short*)_obj, -(*((short*)&_arg)), _order);
}

MG_CAPI_INLINE long __mgu_atomic_fetch_sub_explicit_int32(
    volatile mgu_atomic_long* _obj, long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_int32(_obj, -_arg, _order);
}

MG_CAPI_INLINE unsigned long __mgu_atomic_fetch_sub_explicit_uint32(
    volatile mgu_atomic_ulong* _obj, unsigned long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_int32(
        (volatile mgu_atomic_long*)_obj, -(*((long*)&_arg)), _order);
}

MG_CAPI_INLINE long long __mgu_atomic_fetch_sub_explicit_int64(
    volatile mgu_atomic_llong* _obj, long long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_int64(_obj, -_arg, _order);
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_fetch_sub_explicit_uint64(
    volatile mgu_atomic_ullong* _obj, unsigned long long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_int64(
        (volatile mgu_atomic_llong*)_obj, -(*((long long*)&_arg)), _order);
}

#ifdef __cplusplus

MG_CAPI_INLINE char __mgu_atomic_fetch_sub_explicit(
    volatile mgu_atomic_char* _obj, char _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_sub_explicit_int8(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned char __mgu_atomic_fetch_sub_explicit(
    volatile mgu_atomic_uchar* _obj, unsigned char _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_sub_explicit_uint8(_obj, _arg, _order);
}

MG_CAPI_INLINE short __mgu_atomic_fetch_sub_explicit(
    volatile mgu_atomic_short* _obj, short _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_sub_explicit_int16(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned short __mgu_atomic_fetch_sub_explicit(
    volatile mgu_atomic_ushort* _obj, unsigned short _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_sub_explicit_uint16(_obj, _arg, _order);
}

MG_CAPI_INLINE long __mgu_atomic_fetch_sub_explicit(
    volatile mgu_atomic_long* _obj, long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_sub_explicit_int32(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned long __mgu_atomic_fetch_sub_explicit(
    volatile mgu_atomic_ulong* _obj, unsigned long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_sub_explicit_uint32(_obj, _arg, _order);
}

MG_CAPI_INLINE long long __mgu_atomic_fetch_sub_explicit(
    volatile mgu_atomic_llong* _obj, long long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_sub_explicit_int64(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_fetch_sub_explicit(
    volatile mgu_atomic_ullong* _obj, unsigned long long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_sub_explicit_uint64(_obj, _arg, _order);
}

#elif !defined(__STDC_NO_ATOMICS__) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#define __mgu_atomic_fetch_sub_explicit(obj,arg,order)                   \
_Generic((obj),                                                          \
    mgu_atomic_char*: __mgu_atomic_fetch_sub_explicit_int8,              \
    mgu_atomic_uchar*: __mgu_atomic_fetch_sub_explicit_uint8,            \
    mgu_atomic_short*: __mgu_atomic_fetch_sub_explicit_int16,            \
    mgu_atomic_ushort*: __mgu_atomic_fetch_sub_explicit_uint16,          \
    mgu_atomic_long*: __mgu_atomic_fetch_sub_explicit_int32,             \
    mgu_atomic_ulong*: __mgu_atomic_fetch_sub_explicit_uint32,           \
    mgu_atomic_llong*: __mgu_atomic_fetch_sub_explicit_int64,            \
    mgu_atomic_ullong*: __mgu_atomic_fetch_sub_explicit_uint64           \
)(obj,arg,order)

#endif

#define mgu_atomic_fetch_sub_explicit(obj,arg,order) __mgu_atomic_fetch_sub_explicit(obj,arg,order)
#define mgu_atomic_fetch_sub(obj,arg) __mgu_atomic_fetch_sub_explicit(obj,arg,mgu_memory_order_seq_cst)

MG_CAPI_INLINE char __mgu_atomic_fetch_and_explicit_int8(
    volatile mgu_atomic_char* _obj, char _arg, mgu_memory_order _order)
{
    char ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedAnd8, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE unsigned char __mgu_atomic_fetch_and_explicit_uint8(
    volatile mgu_atomic_uchar* _obj, unsigned char _arg, mgu_memory_order _order)
{
    char ret = __mgu_atomic_fetch_and_explicit_int8(
        (volatile mgu_atomic_char*)_obj, *((char*)&_arg), _order);
    return *((unsigned char*)&ret);
}

MG_CAPI_INLINE short __mgu_atomic_fetch_and_explicit_int16(
    volatile mgu_atomic_short* _obj, short _arg, mgu_memory_order _order)
{
    short ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedAnd16, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE unsigned short __mgu_atomic_fetch_and_explicit_uint16(
    volatile mgu_atomic_ushort* _obj, unsigned short _arg, mgu_memory_order _order)
{
    short ret = __mgu_atomic_fetch_and_explicit_int16(
        (volatile mgu_atomic_short*)_obj, *((short*)&_arg), _order);
    return *((unsigned short*)&ret);
}

MG_CAPI_INLINE long __mgu_atomic_fetch_and_explicit_int32(
    volatile mgu_atomic_long* _obj, long _arg, mgu_memory_order _order)
{
    long ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedAnd, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE unsigned long __mgu_atomic_fetch_and_explicit_uint32(
    volatile mgu_atomic_ulong* _obj, unsigned long _arg, mgu_memory_order _order)
{
    long ret = __mgu_atomic_fetch_and_explicit_int32(
        (volatile mgu_atomic_long*)_obj, *((long*)&_arg), _order);
    return *((unsigned long*)&ret);
}

MG_CAPI_INLINE long long __mgu_atomic_fetch_and_explicit_int64(
    volatile mgu_atomic_llong* _obj, long long _arg, mgu_memory_order _order)
{
    long long ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedAnd64, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_fetch_and_explicit_uint64(
    volatile mgu_atomic_ullong* _obj, unsigned long long _arg, mgu_memory_order _order)
{
    long long ret = __mgu_atomic_fetch_and_explicit_int64(
        (volatile mgu_atomic_llong*)_obj, *((long long*)&_arg), _order);
    return *((unsigned long long*)&ret);
}

#ifdef __cplusplus

MG_CAPI_INLINE char __mgu_atomic_fetch_and_explicit(
    mgu_atomic_char* obj, char arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_and_explicit_int8(obj, arg, order);
}

MG_CAPI_INLINE unsigned char __mgu_atomic_fetch_and_explicit(
    mgu_atomic_uchar* obj, unsigned char arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_and_explicit_uint8(obj, arg, order);
}

MG_CAPI_INLINE short __mgu_atomic_fetch_and_explicit(
    mgu_atomic_short* obj, short arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_and_explicit_int16(obj, arg, order);
}

MG_CAPI_INLINE unsigned short __mgu_atomic_fetch_and_explicit(
    mgu_atomic_ushort* obj, unsigned short arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_and_explicit_uint16(obj, arg, order);
}

MG_CAPI_INLINE long __mgu_atomic_fetch_and_explicit(
    mgu_atomic_long* obj, long arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_and_explicit_int32(obj, arg, order);
}

MG_CAPI_INLINE unsigned long __mgu_atomic_fetch_and_explicit(
    mgu_atomic_ulong* obj, unsigned long arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_and_explicit_uint32(obj, arg, order);
}

MG_CAPI_INLINE long long __mgu_atomic_fetch_and_explicit(
    mgu_atomic_llong* obj, long long arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_and_explicit_int64(obj, arg, order);
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_fetch_and_explicit(
    mgu_atomic_ullong* obj, unsigned long long arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_and_explicit_uint64(obj, arg, order);
}

#elif !defined(__STDC_NO_ATOMICS__) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#define __mgu_atomic_fetch_and_explicit(obj,arg,order)               \
_Generic((obj),                                                      \
mgu_atomic_char*: __mgu_atomic_fetch_and_explicit_int8,              \
mgu_atomic_uchar*: __mgu_atomic_fetch_and_explicit_uint8,            \
mgu_atomic_short*: __mgu_atomic_fetch_and_explicit_int16,            \
mgu_atomic_ushort*: __mgu_atomic_fetch_and_explicit_uint16,          \
mgu_atomic_long*: __mgu_atomic_fetch_and_explicit_int32,             \
mgu_atomic_ulong*: __mgu_atomic_fetch_and_explicit_uint32,           \
mgu_atomic_llong*: __mgu_atomic_fetch_and_explicit_int64,            \
mgu_atomic_ullong*: __mgu_atomic_fetch_and_explicit_uint64           \
)(obj,arg,order)

#endif

#define mgu_atomic_fetch_and_explicit(obj,arg,order) __mgu_atomic_fetch_and_explicit(obj,arg,order)

MG_CAPI_INLINE char __mgu_atomic_fetch_or_explicit_int8(
    volatile mgu_atomic_char* _obj, char _arg, mgu_memory_order _order)
{
    char ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedOr8, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE unsigned char __mgu_atomic_fetch_or_explicit_uint8(
    volatile mgu_atomic_uchar* _obj, unsigned char _arg, mgu_memory_order _order)
{
    char ret = __mgu_atomic_fetch_or_explicit_int8(
        (volatile mgu_atomic_char*)_obj, *((char*)&_arg), _order);
    return *((unsigned char*)&ret);
}

MG_CAPI_INLINE short __mgu_atomic_fetch_or_explicit_int16(
    volatile mgu_atomic_short* _obj, short _arg, mgu_memory_order _order)
{
    short ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedOr16, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE unsigned short __mgu_atomic_fetch_or_explicit_uint16(
    volatile mgu_atomic_ushort* _obj, unsigned short _arg, mgu_memory_order _order)
{
    short ret = __mgu_atomic_fetch_or_explicit_int16(
        (volatile mgu_atomic_short*)_obj, *((short*)&_arg), _order);
    return *((unsigned short*)&ret);
}

MG_CAPI_INLINE long __mgu_atomic_fetch_or_explicit_int32(
    volatile mgu_atomic_long* _obj, long _arg, mgu_memory_order _order)
{
    long ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedOr, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE unsigned long __mgu_atomic_fetch_or_explicit_uint32(
    volatile mgu_atomic_ulong* _obj, unsigned long _arg, mgu_memory_order _order)
{
    long ret = __mgu_atomic_fetch_or_explicit_int32(
        (volatile mgu_atomic_long*)_obj, *((long*)&_arg), _order);
    return *((unsigned long*)&ret);
}

MG_CAPI_INLINE long long __mgu_atomic_fetch_or_explicit_int64(
    volatile mgu_atomic_llong* _obj, long long _arg, mgu_memory_order _order)
{
    long long ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedOr64, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_fetch_or_explicit_uint64(
    volatile mgu_atomic_ullong* _obj, unsigned long long _arg, mgu_memory_order _order)
{
    long long ret = __mgu_atomic_fetch_or_explicit_int64(
        (volatile mgu_atomic_llong*)_obj, *((long long*)&_arg), _order);
    return *((unsigned long long*)&ret);
}

#ifdef __cplusplus

MG_CAPI_INLINE char __mgu_atomic_fetch_or_explicit(
    volatile mgu_atomic_char* obj, char arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_or_explicit_int8(obj, arg, order);
}

MG_CAPI_INLINE unsigned char __mgu_atomic_fetch_or_explicit(
    volatile mgu_atomic_uchar* obj, unsigned char arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_or_explicit_uint8(obj, arg, order);
}

MG_CAPI_INLINE short __mgu_atomic_fetch_or_explicit(
    volatile mgu_atomic_short* obj, short arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_or_explicit_int16(obj, arg, order);
}

MG_CAPI_INLINE unsigned short __mgu_atomic_fetch_or_explicit(
    volatile mgu_atomic_ushort* obj, unsigned short arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_or_explicit_uint16(obj, arg, order);
}

MG_CAPI_INLINE long __mgu_atomic_fetch_or_explicit(
    volatile mgu_atomic_long* obj, long arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_or_explicit_int32(obj, arg, order);
}

MG_CAPI_INLINE unsigned long __mgu_atomic_fetch_or_explicit(
    volatile mgu_atomic_ulong* obj, unsigned long arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_or_explicit_uint32(obj, arg, order);
}

MG_CAPI_INLINE long long __mgu_atomic_fetch_or_explicit(
    volatile mgu_atomic_llong* obj, long long arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_or_explicit_int64(obj, arg, order);
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_fetch_or_explicit(
    volatile mgu_atomic_ullong* obj, unsigned long long arg, mgu_memory_order order)
{
    return __mgu_atomic_fetch_or_explicit_uint64(obj, arg, order);
}

#elif !defined(__STDC_NO_ATOMICS__) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#define __mgu_atomic_fetch_or_explicit(obj,arg,order)              \
_Generic((obj),                                                    \
mgu_atomic_char*: __mgu_atomic_fetch_or_explicit_int8,             \
mgu_atomic_uchar*: __mgu_atomic_fetch_or_explicit_uint8,           \
mgu_atomic_short*: __mgu_atomic_fetch_or_explicit_int16,           \
mgu_atomic_ushort*: __mgu_atomic_fetch_or_explicit_uint16,         \
mgu_atomic_long*: __mgu_atomic_fetch_or_explicit_int32,            \
mgu_atomic_ulong*: __mgu_atomic_fetch_or_explicit_uint32,          \
mgu_atomic_llong*: __mgu_atomic_fetch_or_explicit_int64,           \
mgu_atomic_ullong*: __mgu_atomic_fetch_or_explicit_uint64          \
)(obj,arg,order)

#endif

#define mgu_atomic_fetch_or_explicit(obj,arg,order) __mgu_atomic_fetch_or_explicit(obj,arg,order)
#define mgu_atomic_fetch_or(obj,arg) __mgu_atomic_fetch_or_explicit(obj,arg,mgu_memory_order_seq_cst)


MG_CAPI_INLINE char __mgu_atomic_fetch_xor_explicit_int8(
    volatile mgu_atomic_char* _obj, char _arg, mgu_memory_order _order)
{
    char ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedXor8, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE unsigned char __mgu_atomic_fetch_xor_explicit_uint8(
    volatile mgu_atomic_uchar* _obj, unsigned char _arg, mgu_memory_order _order)
{
    char ret = __mgu_atomic_fetch_xor_explicit_int8(
        (volatile mgu_atomic_char*)_obj, *((char*)&_arg), _order);
    return *((unsigned char*)&ret);
}

MG_CAPI_INLINE short __mgu_atomic_fetch_xor_explicit_int16(
    volatile mgu_atomic_short* _obj, short _arg, mgu_memory_order _order)
{
    short ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedXor16, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE unsigned short __mgu_atomic_fetch_xor_explicit_uint16(
    volatile mgu_atomic_ushort* _obj, unsigned short _arg, mgu_memory_order _order)
{
    short ret = __mgu_atomic_fetch_xor_explicit_int16(
        (volatile mgu_atomic_short*)_obj, *((short*)&_arg), _order);
    return *((unsigned short*)&ret);
}

MG_CAPI_INLINE long __mgu_atomic_fetch_xor_explicit_int32(
    volatile mgu_atomic_long* _obj, long _arg, mgu_memory_order _order)
{
    long ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedXor, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE unsigned long __mgu_atomic_fetch_xor_explicit_uint32(
    volatile mgu_atomic_ulong* _obj, unsigned long _arg, mgu_memory_order _order)
{
    long ret = __mgu_atomic_fetch_xor_explicit_int32(
        (volatile mgu_atomic_long*)_obj, *((long*)&_arg), _order);
    return *((unsigned long*)&ret);
}

MG_CAPI_INLINE long long __mgu_atomic_fetch_xor_explicit_int64(
    volatile mgu_atomic_llong* _obj, long long _arg, mgu_memory_order _order)
{
    long long ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedXor64, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_fetch_xor_explicit_uint64(
    volatile mgu_atomic_ullong* _obj, unsigned long long _arg, mgu_memory_order _order)
{
    long long ret = __mgu_atomic_fetch_xor_explicit_int64(
        (volatile mgu_atomic_llong*)_obj, *((long long*)&_arg), _order);
    return *((unsigned long long*)&ret);
}

#ifdef __cplusplus

MG_CAPI_INLINE char __mgu_atomic_fetch_xor_explicit(
    volatile mgu_atomic_char* _obj, char _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_xor_explicit_int8(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned char __mgu_atomic_fetch_xor_explicit(
    volatile mgu_atomic_uchar* _obj, unsigned char _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_xor_explicit_uint8(_obj, _arg, _order);
}

MG_CAPI_INLINE short __mgu_atomic_fetch_xor_explicit(
    volatile mgu_atomic_short* _obj, short _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_xor_explicit_int16(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned short __mgu_atomic_fetch_xor_explicit(
    volatile mgu_atomic_ushort* _obj, unsigned short _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_xor_explicit_uint16(_obj, _arg, _order);
}

MG_CAPI_INLINE long __mgu_atomic_fetch_xor_explicit(
    volatile mgu_atomic_long* _obj, long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_xor_explicit_int32(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned long __mgu_atomic_fetch_xor_explicit(
    volatile mgu_atomic_ulong* _obj, unsigned long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_xor_explicit_uint32(_obj, _arg, _order);
}

MG_CAPI_INLINE long long __mgu_atomic_fetch_xor_explicit(
    volatile mgu_atomic_llong* _obj, long long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_xor_explicit_int64(_obj, _arg, _order);
}

MG_CAPI_INLINE unsigned long long __mgu_atomic_fetch_xor_explicit(
    volatile mgu_atomic_ullong* _obj, unsigned long long _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_xor_explicit_uint64(_obj, _arg, _order);
}

#elif !defined(__STDC_NO_ATOMICS__) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#define __mgu_atomic_fetch_xor_explicit(obj,arg,order)              \
_Generic((obj),                                                     \
mgu_atomic_char*: __mgu_atomic_fetch_xor_explicit_int8,             \
mgu_atomic_uchar*: __mgu_atomic_fetch_xor_explicit_uint8,           \
mgu_atomic_short*: __mgu_atomic_fetch_xor_explicit_int16,           \
mgu_atomic_ushort*: __mgu_atomic_fetch_xor_explicit_uint16,         \
mgu_atomic_long*: __mgu_atomic_fetch_xor_explicit_int32,            \
mgu_atomic_ulong*: __mgu_atomic_fetch_xor_explicit_uint32,          \
mgu_atomic_llong*: __mgu_atomic_fetch_xor_explicit_int64,           \
mgu_atomic_ullong*: __mgu_atomic_fetch_xor_explicit_uint64          \
)(obj,arg,order)

#endif

#define mgu_atomic_fetch_xor_explicit(obj,arg,order) __mgu_atomic_fetch_xor_explicit(obj,arg,order)
#define mgu_atomic_fetch_xor(obj,arg) __mgu_atomic_fetch_xor_explicit(obj,arg,mgu_memory_order_seq_cst)

MG_CAPI_INLINE bool mgu_atomic_flag_test_and_set_explicit(
    volatile mgu_atomic_flag* _obj, mgu_memory_order _order)
{
    char o = 0;
    return mgu_atomic_compare_exchange_strong_explicit(
        (mgu_atomic_char*)&_obj->value_, &o, 1, _order, mgu_memory_order_relaxed) ? 0 : 1;
}

MG_CAPI_INLINE void mgu_atomic_flag_clear_explicit(
    volatile mgu_atomic_flag* _obj, mgu_memory_order _order)
{
    mgu_atomic_store_explicit((mgu_atomic_char*)&_obj->value_, 0, _order);
}

#define mgu_atomic_flag_test_and_set(obj) mgu_atomic_flag_test_and_set_explicit(obj, mgu_memory_order_seq_cst)
#define mgu_atomic_flag_clear(obj) mgu_atomic_flag_clear_explicit(obj, mgu_memory_order_release)

#else

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

#endif

#endif // !MEGO_UTIL_ATOMIC_H_INCLUDED
