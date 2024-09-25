
#ifndef MEGO_UTIL_DETAIL_MSVC_ATOMIC_H_INCLUDED
#define MEGO_UTIL_DETAIL_MSVC_ATOMIC_H_INCLUDED

#include <mego/util/atomic_fwd.h>
#include <mego/predef/compiler/visualc.h>

#if MG_COMP__MSVC_AVAIL

#include <mego/predef/helper_macros.h>
#include <mego/predef/symbol/inline.h>
#include <mego/util/os/windows/windows_simplify.h>

//! @see vcruntime_c11_atomic_support.h

#include <string.h>
#include <assert.h>
#include <stdint.h>

#include <crtdbg.h>
#include <intrin.h>


// Interlocked intrinsic mapping for _nf/_acq/_rel
#if defined(_M_CEE_PURE) || defined(_M_IX86) || (defined(_M_X64) && !defined(_M_ARM64EC))
#define __MGU_ATOMIC_INTRIN_RELAXED(x) x
#define __MGU_ATOMIC_INTRIN_ACQUIRE(x) x
#define __MGU_ATOMIC_INTRIN_RELEASE(x) x
#define __MGU_ATOMIC_INTRIN_ACQ_REL(x) x
#ifdef _M_CEE_PURE
#define __MGU_ATOMIC_YIELD_PROCESSOR()
#else // ^^^ _M_CEE_PURE / !_M_CEE_PURE vvv
#define __MGU_ATOMIC_YIELD_PROCESSOR() _mm_pause()
#endif // ^^^ !_M_CEE_PURE ^^^

#elif defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC)
#define __MGU_ATOMIC_INTRIN_RELAXED(x) MEGO__JOIN(x, _nf)
#define __MGU_ATOMIC_INTRIN_ACQUIRE(x) MEGO__JOIN(x, _acq)
#define __MGU_ATOMIC_INTRIN_RELEASE(x) MEGO__JOIN(x, _rel)
// We don't have interlocked intrinsics for acquire-release ordering, even on
// ARM32/ARM64, so fall back to sequentially consistent.
#define __MGU_ATOMIC_INTRIN_ACQ_REL(x) x
#define __MGU_ATOMIC_YIELD_PROCESSOR() __yield()

#else // ^^^ ARM32/ARM64 / unsupported hardware vvv
#error Unsupported hardware
#endif // hardware


// The following is modified from the _CRT_SECURE_INVALID_PARAMETER macro in
// corecrt.h. We need to do this because this header must be C, not C++, but we
// still want to report invalid parameters in the same way as C++ does. The
// macro in the CRT expands to C++ code because it contains global namespace
// qualification. This can be fixed in the ucrt by using a mechanism that
// defines something like _GLOBAL_NAMESPACE to :: in c++ mode and nothing in C
// mode.
#ifndef __MGU_ATOMIC_INVALID_PARAMETER
#ifdef _DEBUG
#define __MGU_ATOMIC_INVALID_PARAMETER(expr) _invalid_parameter(_CRT_WIDE(#expr), L"", __FILEW__, __LINE__, 0)
#else
// By default, __MGU_ATOMIC_INVALID_PARAMETER in retail invokes
// _invalid_parameter_noinfo_noreturn(), which is marked
// __declspec(noreturn) and does not return control to the application.
// Even if _set_invalid_parameter_handler() is used to set a new invalid
// parameter handler which does return control to the application,
// _invalid_parameter_noinfo_noreturn() will terminate the application
// and invoke Watson. You can overwrite the definition of
// __MGU_ATOMIC_INVALID_PARAMETER if you need.
#define __MGU_ATOMIC_INVALID_PARAMETER(expr) _invalid_parameter_noinfo_noreturn()
#endif
#endif


// Controls whether ARM64 ldar/ldapr/stlr should be used
#ifndef __MGU_ATOMIC_USE_ARM64_LDAR_STLR
#  if defined(_M_ARM64) || defined(_M_ARM64EC)
#    if defined(_HAS_ARM64_LOAD_ACQUIRE) && _HAS_ARM64_LOAD_ACQUIRE == 1 // TRANSITION, VS 2022 17.7 Preview 1
#      define __MGU_ATOMIC_USE_ARM64_LDAR_STLR 1
#    else // ^^^ updated intrin0.inl.h is available / workaround vvv
#      define __MGU_ATOMIC_USE_ARM64_LDAR_STLR 0
#    endif // ^^^ workaround ^^^
#  else // ^^^ ARM64/ARM64EC / Other architectures vvv
#    define __MGU_ATOMIC_USE_ARM64_LDAR_STLR 0
#  endif // ^^^ Other architectures ^^^
#endif // __MGU_ATOMIC_USE_ARM64_LDAR_STLR


#ifndef __MGU_ATOMIC_INVALID_MEMORY_ORDER
#  ifdef _DEBUG
#define __MGU_ATOMIC_INVALID_MEMORY_ORDER                       \
    do {                                                        \
        _RPTF0(_CRT_ASSERT, "Invalid memory order");            \
        __MGU_ATOMIC_INVALID_PARAMETER("Invalid memory order"); \
    } while (0)
#  else // ^^^ defined(_DEBUG) / !defined(_DEBUG) vvv
#    define __MGU_ATOMIC_INVALID_MEMORY_ORDER
#  endif // ^^^ !defined(_DEBUG) ^^^
#endif // __MGU_ATOMIC_INVALID_MEMORY_ORDER


// this is different from the STL
// we are the MSVC runtime so we need not support clang here
//#define __mgu_atomic_compiler_barrier()                                                       \
//    _Pragma("warning(push)") _Pragma("warning(disable : 4996)") /* was declared deprecated */ \
//        _ReadWriteBarrier() _Pragma("warning(pop)")
#define __mgu_atomic_compiler_barrier() \
        _ReadWriteBarrier() 

#if defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC)
#define __mgu_atomic_memory_barrier()             __dmb(0xB) // inner shared data memory barrier
#define __mgu_atomic_compiler_or_memory_barrier() __mgu_atomic_memory_barrier()
#elif defined(_M_IX86) || defined(_M_X64)
// x86/x64 hardware only emits memory barriers inside _Interlocked intrinsics
#define __mgu_atomic_compiler_or_memory_barrier() __mgu_atomic_compiler_barrier()
#else // ^^^ x86/x64 / unsupported hardware vvv
#error Unsupported hardware
#endif // hardware


MG_CAPI_INLINE void __mgu_atomic_check_memory_order(const unsigned int _Order) {
    if (_Order > mgu_memory_order_seq_cst) {
        __MGU_ATOMIC_INVALID_MEMORY_ORDER;
    }
}


#if defined(_M_IX86) || (defined(_M_X64) && !defined(_M_ARM64EC))
#define __MGU_ATOMIC_CHOOSE_INTRINSIC(_Order, _Result, _Intrinsic, ...) \
    __mgu_atomic_check_memory_order(_Order);                            \
    _Result = _Intrinsic(__VA_ARGS__)
#elif defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC)
#define __MGU_ATOMIC_CHOOSE_INTRINSIC(_Order, _Result, _Intrinsic, ...) \
    switch (_Order) {                                                   \
    case mgu_memory_order_relaxed:                                      \
        _Result = __MGU_ATOMIC_INTRIN_RELAXED(_Intrinsic)(__VA_ARGS__); \
        break;                                                          \
    case mgu_memory_order_consume:                                      \
    case mgu_memory_order_acquire:                                      \
        _Result = __MGU_ATOMIC_INTRIN_ACQUIRE(_Intrinsic)(__VA_ARGS__); \
        break;                                                          \
    case mgu_memory_order_release:                                      \
        _Result = __MGU_ATOMIC_INTRIN_RELEASE(_Intrinsic)(__VA_ARGS__); \
        break;                                                          \
    default:                                                            \
        __MGU_ATOMIC_INVALID_MEMORY_ORDER;                              \
        /*FALLTHROUGH*/;                                                \
    case mgu_memory_order_acq_rel:                                      \
    case mgu_memory_order_seq_cst:                                      \
        _Result = _Intrinsic(__VA_ARGS__);                              \
        break;                                                          \
    }
#endif 


#if __MGU_ATOMIC_USE_ARM64_LDAR_STLR == 1

#define __MGU_ATOMIC_LOAD_ACQUIRE_ARM64(_Width, _Ptr) \
    __load_acquire##_Width((const volatile unsigned __int##_Width*)(_Ptr))

#define __MGU_ATOMIC_LOAD_ARM64(_Result, _Width, _Ptr, _Order_var) \
    switch (_Order_var) {                                          \
    case mgu_memory_order_relaxed:                                 \
        _Result = __iso_volatile_load##_Width(_Ptr);               \
        break;                                                     \
    case mgu_memory_order_consume:                                 \
    case mgu_memory_order_acquire:                                 \
    case mgu_memory_order_seq_cst:                                 \
        _Result = __MGU_ATOMIC_LOAD_ACQUIRE_ARM64(_Width, _Ptr);   \
        __mgu_atomic_compiler_barrier();                           \
        break;                                                     \
    case mgu_memory_order_release:                                 \
    case mgu_memory_order_acq_rel:                                 \
    default:                                                       \
        _Result = __iso_volatile_load##_Width(_Ptr);               \
        __MGU_ATOMIC_INVALID_MEMORY_ORDER;                         \
        break;                                                     \
    }

#endif


#define __MGU_ATOMIC_POST_LOAD_BARRIER_AS_NEEDED(_Order_var) \
    switch (_Order_var) {                                    \
    case mgu_memory_order_relaxed:                           \
        break;                                               \
    case mgu_memory_order_consume:                           \
    case mgu_memory_order_acquire:                           \
    case mgu_memory_order_seq_cst:                           \
        __mgu_atomic_compiler_or_memory_barrier();           \
        break;                                               \
    case mgu_memory_order_release:                           \
    case mgu_memory_order_acq_rel:                           \
    default:                                                 \
        __MGU_ATOMIC_INVALID_MEMORY_ORDER;                   \
        break;                                               \
    }


#if __MGU_ATOMIC_USE_ARM64_LDAR_STLR == 1

#define __MGU_ATOMIC_STORE_RELEASE(_Width, _Ptr, _Desired)       \
    __mgu_atomic_compiler_barrier();                             \
    __stlr##_Width((volatile unsigned __int##_Width*)(_Ptr), (_Desired));

#else

#define __MGU_ATOMIC_STORE_RELEASE(_Width, _Ptr, _Desired)       \
    __mgu_atomic_compiler_or_memory_barrier();                   \
    __iso_volatile_store##_Width((_Ptr), (_Desired));

#endif

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
        /*FALLTHROUGH*/;


#define __MGU_ATOMIC_STORE_SEQ_CST_ARM(_Width, _Ptr, _Desired) \
    __mgu_atomic_memory_barrier();                             \
    __iso_volatile_store##_Width((_Ptr), (_Desired));          \
    __mgu_atomic_memory_barrier();

#if __MGU_ATOMIC_USE_ARM64_LDAR_STLR == 1
#define __MGU_ATOMIC_STORE_SEQ_CST_ARM64(_Width, _Ptr, _Desired)          \
    __mgu_atomic_compiler_barrier();                                      \
    __stlr##_Width((volatile unsigned __int##_Width*)(_Ptr), (_Desired)); \
    __mgu_atomic_memory_barrier();
#else
#define __MGU_ATOMIC_STORE_SEQ_CST_ARM64 _ATOMIC_STORE_SEQ_CST_ARM
#endif

#define __MGU_ATOMIC_STORE_SEQ_CST_X86_X64(_Width, _Ptr, _Desired) (void) _InterlockedExchange##_Width((_Ptr), (_Desired));
#define __MGU_ATOMIC_STORE_32_SEQ_CST_X86_X64(_Ptr, _Desired) \
    (void) _InterlockedExchange((volatile long*)(_Ptr), (long)(_Desired));

#define __MGU_ATOMIC_STORE_64_SEQ_CST_IX86(_Ptr, _Desired) \
    __mgu_atomic_compiler_barrier();                       \
    __iso_volatile_store64((_Ptr), (_Desired));            \
    _Atomic_thread_fence(_Atomic_memory_order_seq_cst);


MG_CAPI_INLINE bool __mgu_atomic_is_lock_free_b  (volatile bool*     _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free_i8 (volatile int8_t*   _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free_u8 (volatile uint8_t*  _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free_i16(volatile int16_t*  _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free_u16(volatile uint16_t* _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free_i32(volatile int32_t*  _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free_u32(volatile uint32_t* _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free_i64(volatile int64_t*  _obj) { return true; }
MG_CAPI_INLINE bool __mgu_atomic_is_lock_free_u64(volatile uint64_t* _obj) { return true; }


MG_CAPI_INLINE void __mgu_atomic_init_b  (volatile bool*     _obj,  bool    _desired) { *_obj = _desired; }
MG_CAPI_INLINE void __mgu_atomic_init_i8 (volatile int8_t*   _obj, int8_t   _desired) { *_obj = _desired; }
MG_CAPI_INLINE void __mgu_atomic_init_u8 (volatile uint8_t*  _obj, uint8_t  _desired) { *_obj = _desired; }
MG_CAPI_INLINE void __mgu_atomic_init_i16(volatile int16_t*  _obj, int16_t  _desired) { *_obj = _desired; }
MG_CAPI_INLINE void __mgu_atomic_init_u16(volatile uint16_t* _obj, uint16_t _desired) { *_obj = _desired; }
MG_CAPI_INLINE void __mgu_atomic_init_i32(volatile int32_t*  _obj, int32_t  _desired) { *_obj = _desired; }
MG_CAPI_INLINE void __mgu_atomic_init_u32(volatile uint32_t* _obj, uint32_t _desired) { *_obj = _desired; }
MG_CAPI_INLINE void __mgu_atomic_init_i64(volatile int64_t*  _obj, int64_t  _desired) { *_obj = _desired; }
MG_CAPI_INLINE void __mgu_atomic_init_u64(volatile uint64_t* _obj, uint64_t _desired) { *_obj = _desired; }


MG_CAPI_INLINE int8_t __mgu_atomic_load_explicit_i8(const volatile int8_t* _obj, mgu_memory_order _order)
{
    int8_t ret;
#if __MGU_ATOMIC_USE_ARM64_LDAR_STLR == 1
    __MGU_ATOMIC_LOAD_ARM64(ret, 8, (const volatile char*)_obj, _order)
#else
    
#if defined(_M_ARM) || defined(_M_ARM64)
    ret = __iso_volatile_load8((const volatile char*)_obj);
#else
    ret = *_obj;
#endif
    __MGU_ATOMIC_POST_LOAD_BARRIER_AS_NEEDED((_order))
#endif
    return ret;
}

MG_CAPI_INLINE uint8_t __mgu_atomic_load_explicit_u8(const volatile uint8_t* _obj, mgu_memory_order _order)
{
    char ret = __mgu_atomic_load_explicit_i8((volatile int8_t*)_obj, _order);
    return *((uint8_t*)&ret);
}

MG_CAPI_INLINE int16_t __mgu_atomic_load_explicit_i16(const volatile int16_t* _obj, mgu_memory_order _order)
{
    int16_t ret;
#if __MGU_ATOMIC_USE_ARM64_LDAR_STLR == 1
    __MGU_ATOMIC_LOAD_ARM64(ret, 16, _obj, _order)
#else
    
#if defined(_M_ARM) || defined(_M_ARM64)
    ret = __iso_volatile_load16(_obj);
#else
    ret = *_obj;
#endif
    
    __MGU_ATOMIC_POST_LOAD_BARRIER_AS_NEEDED((_order))
#endif
    return ret;
}

MG_CAPI_INLINE uint16_t __mgu_atomic_load_explicit_u16(const volatile uint16_t* _obj, mgu_memory_order _order)
{
    int16_t ret = __mgu_atomic_load_explicit_i16((volatile int16_t*)_obj, _order);
    return *((uint16_t*)&ret);
}

MG_CAPI_INLINE int32_t __mgu_atomic_load_explicit_i32(const volatile int32_t* _obj, mgu_memory_order _order)
{
    int32_t ret;
#if __MGU_ATOMIC_USE_ARM64_LDAR_STLR == 1
    __MGU_ATOMIC_LOAD_ARM64(ret, 32, _obj, (_order))
#else
    
#if defined(_M_ARM) || defined(_M_ARM64)
    ret = __iso_volatile_load32(_obj);
#else
    ret = *_obj;
#endif
    
    __MGU_ATOMIC_POST_LOAD_BARRIER_AS_NEEDED((_order))
#endif
    return ret;
}

MG_CAPI_INLINE uint32_t __mgu_atomic_load_explicit_u32(const volatile uint32_t* _obj, mgu_memory_order _order)
{
    int32_t ret = __mgu_atomic_load_explicit_i32((volatile int32_t*)_obj, _order);
    return *((uint32_t*)&ret);
}

MG_CAPI_INLINE int64_t __mgu_atomic_load_explicit_i64(const volatile int64_t* _obj, mgu_memory_order _order)
{
    int64_t ret;
#if __MGU_ATOMIC_USE_ARM64_LDAR_STLR == 1
    __MGU_ATOMIC_LOAD_ARM64(ret, 64, _obj, (_order))
#else

#if defined(_M_X64)
    ret = *_obj;
#elif defined(_M_ARM)
    ret = __ldrexd(_obj);
#elif defined(_M_ARM64)
    ret = __iso_volatile_load64(_obj);
#else
    ret = _InterlockedOr64(_obj, 0);
#endif
    __MGU_ATOMIC_POST_LOAD_BARRIER_AS_NEEDED((_order))
#endif
    return ret;
}

MG_CAPI_INLINE uint64_t __mgu_atomic_load_explicit_u64(const volatile uint64_t* _obj, mgu_memory_order _order)
{
    int64_t ret = __mgu_atomic_load_explicit_i64((volatile int64_t*)_obj, _order);
    return *((uint64_t*)&ret);
}

MG_CAPI_INLINE bool __mgu_atomic_load_explicit_b(const volatile bool* _obj, mgu_memory_order _order)
{
    return (bool)__mgu_atomic_load_explicit_i8((volatile int8_t*)_obj, _order);
}


MG_CAPI_INLINE void __mgu_atomic_store_explicit_i8(
    volatile int8_t* _obj, int8_t _desired, mgu_memory_order _order)
{
    switch (_order) {
        __MGU_ATOMIC_STORE_SWITCH_PREFIX(8, (volatile char*)_obj, _desired)
    case mgu_memory_order_seq_cst:
        InterlockedExchange8((volatile char*)_obj, _desired);
        return;
    }
}

MG_CAPI_INLINE void __mgu_atomic_store_explicit_u8(
    volatile uint8_t* _obj, uint8_t _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_i8((volatile int8_t*)_obj, *((int8_t*)&_desired), _order);
}

MG_CAPI_INLINE void __mgu_atomic_store_explicit_i16(
    volatile int16_t* _obj, int16_t _desired, mgu_memory_order _order)
{
    switch (_order) {
        __MGU_ATOMIC_STORE_SWITCH_PREFIX(16, _obj, _desired)
    case mgu_memory_order_seq_cst:
        InterlockedExchange16(_obj, _desired);
        return;
    }
}

MG_CAPI_INLINE void __mgu_atomic_store_explicit_u16(
    volatile uint16_t* _obj, uint16_t _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_i16((volatile int16_t*)_obj, *((int16_t*)&_desired), _order);
}

MG_CAPI_INLINE void __mgu_atomic_store_explicit_i32(
    volatile int32_t* _obj, int32_t _desired, mgu_memory_order _order)
{
    switch (_order) {
        __MGU_ATOMIC_STORE_SWITCH_PREFIX(32, _obj, _desired)
    case mgu_memory_order_seq_cst:
        InterlockedExchange((volatile long*)_obj, _desired);
        return;
    }
}

MG_CAPI_INLINE void __mgu_atomic_store_explicit_u32(
    volatile uint32_t* _obj, uint32_t _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_i32((volatile int32_t*)_obj, *((int32_t*)&_desired), _order);
}

MG_CAPI_INLINE void __mgu_atomic_store_explicit_i64(
    volatile int64_t* _obj, int64_t _desired, mgu_memory_order _order)
{
    switch (_order) {
        __MGU_ATOMIC_STORE_SWITCH_PREFIX(64, _obj, _desired)
    case mgu_memory_order_seq_cst:
        InterlockedExchange64(_obj, _desired);
        return;
    }
}

MG_CAPI_INLINE void __mgu_atomic_store_explicit_u64(
    volatile uint64_t* _obj, uint64_t _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_i64((volatile int64_t*)_obj, *((int64_t*)&_desired), _order);
}

MG_CAPI_INLINE void __mgu_atomic_store_explicit_b(
    volatile mgu_atomic_bool* _obj, bool _desired, mgu_memory_order _order)
{
    __mgu_atomic_store_explicit_i8((volatile int8_t*)_obj, *((int8_t*)&_desired), _order);
}


MG_CAPI_INLINE int8_t __mgu_atomic_exchange_explicit_i8(
    volatile int8_t* _obj, int8_t _desired, mgu_memory_order _order)
{
    int8_t ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, InterlockedExchange8, (volatile char*)_obj, _desired);
    return ret;
}

MG_CAPI_INLINE uint8_t __mgu_atomic_exchange_explicit_u8(
    volatile uint8_t* _obj, uint8_t _desired, mgu_memory_order _order)
{
    int8_t ret = __mgu_atomic_exchange_explicit_i8((volatile int8_t*)_obj, *((int8_t*)&_desired), _order);
    return *((uint8_t*)&ret);
}

MG_CAPI_INLINE int16_t __mgu_atomic_exchange_explicit_i16(
    volatile int16_t* _obj, int16_t _desired, mgu_memory_order _order)
{
    int16_t ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, InterlockedExchange16, _obj, _desired);
    return ret;
}

MG_CAPI_INLINE uint16_t __mgu_atomic_exchange_explicit_u16(
    volatile uint16_t* _obj, uint16_t _desired, mgu_memory_order _order)
{
    int16_t ret = __mgu_atomic_exchange_explicit_i16((volatile int16_t*)_obj, *((int16_t*)&_desired), _order);
    return *((uint16_t*)&ret);
}

MG_CAPI_INLINE int32_t __mgu_atomic_exchange_explicit_i32(
    volatile int32_t* _obj, int32_t _desired, mgu_memory_order _order)
{
    int32_t ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, InterlockedExchange, (volatile long*)_obj, _desired);
    return ret;
}

MG_CAPI_INLINE uint32_t __mgu_atomic_exchange_explicit_u32(
    volatile uint32_t* _obj, uint32_t _desired, mgu_memory_order _order)
{
    int32_t ret = __mgu_atomic_exchange_explicit_i32((volatile int32_t*)_obj, *((int32_t*)&_desired), _order);
    return *((uint32_t*)&ret);
}

MG_CAPI_INLINE int64_t __mgu_atomic_exchange_explicit_i64(
    volatile int64_t* _obj, int64_t _desired, mgu_memory_order _order)
{
    int64_t ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, InterlockedExchange64, _obj, _desired);
    return ret;
}

MG_CAPI_INLINE uint64_t __mgu_atomic_exchange_explicit_u64(
    volatile uint64_t* _obj, uint64_t _desired, mgu_memory_order _order)
{
    int64_t ret = __mgu_atomic_exchange_explicit_i64((volatile int64_t*)_obj, *((int64_t*)&_desired), _order);
    return *((uint64_t*) & ret);
}

MG_CAPI_INLINE bool __mgu_atomic_exchange_explicit_b(
    volatile bool* _obj, bool _desired, mgu_memory_order _order)
{
    return (bool)__mgu_atomic_exchange_explicit_i8((volatile int8_t*)_obj, (int8_t)_desired, _order);
}


MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit_i8(
    volatile int8_t* _obj, int8_t* _expected, int8_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    int8_t expected_value = *_expected;
    int8_t ret;

    __MGU_ATOMIC_CHOOSE_INTRINSIC(_success, ret, _InterlockedCompareExchange8, (volatile char*)_obj, _desired, expected_value);
    if (ret == expected_value) {
        return true;
    }

    memcpy(_expected, &ret, sizeof(int8_t));
    return false;
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit_i8(
    volatile int8_t* _obj, int8_t* _expected, int8_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_i8(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit_u8(
    volatile uint8_t* _obj, uint8_t* _expected, uint8_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    int8_t expected_value = *((int8_t*)_expected);
    bool ret = __mgu_atomic_compare_exchange_strong_explicit_i8(
        (volatile int8_t*)_obj, &expected_value, *((int8_t*)&_desired), _success, _failure);
    memcpy(_expected, &expected_value, sizeof(uint8_t));
    return ret;
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit_u8(
    volatile uint8_t* _obj, uint8_t* _expected, uint8_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_u8(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit_i16(
    volatile int16_t* _obj, int16_t* _expected, int16_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    int16_t expected_value = *_expected;
    int16_t ret;

    __MGU_ATOMIC_CHOOSE_INTRINSIC(_success, ret, InterlockedCompareExchange16, _obj, _desired, expected_value);
    if (ret == expected_value) {
        return true;
    }

    memcpy(_expected, &ret, sizeof(int16_t));
    return false;
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit_i16(
    volatile int16_t* _obj, int16_t* _expected, int16_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_i16(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit_u16(
    volatile uint16_t* _obj, uint16_t* _expected, uint16_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    short expected_value = *((short*)_expected);
    bool ret = __mgu_atomic_compare_exchange_strong_explicit_i16(
        (volatile int16_t*)_obj, &expected_value, *((int16_t*)&_desired), _success, _failure);
    memcpy(_expected, &expected_value, sizeof(uint16_t));
    return ret;
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit_u16(
    volatile uint16_t* _obj, uint16_t* _expected, uint16_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_u16(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit_i32(
    volatile int32_t* _obj, int32_t* _expected, int32_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    int32_t expected_value = *_expected;
    int32_t ret;

    __MGU_ATOMIC_CHOOSE_INTRINSIC(_success, ret, _InterlockedCompareExchange, (volatile long*)_obj, _desired, expected_value);
    if (ret == expected_value) {
        return true;
    }

    memcpy(_expected, &ret, sizeof(int32_t));
    return false;
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit_i32(
    volatile int32_t* _obj, int32_t* _expected, int32_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_i32(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit_u32(
    volatile uint32_t* _obj, uint32_t* _expected, uint32_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    int32_t expected_value = *((int32_t*)_expected);
    bool ret = __mgu_atomic_compare_exchange_strong_explicit_i32(
        (volatile mgu_atomic_long*)_obj, &expected_value, *((int32_t*)&_desired), _success, _failure);
    memcpy(_expected, &expected_value, sizeof(uint32_t));
    return ret;
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit_u32(
    volatile uint32_t* _obj, uint32_t* _expected, uint32_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_u32(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit_i64(
    volatile int64_t* _obj, int64_t* _expected, int64_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    int64_t expected_value = *_expected;
    int64_t ret;

    __MGU_ATOMIC_CHOOSE_INTRINSIC(_success, ret, _InterlockedCompareExchange64, _obj, _desired, expected_value);
    if (ret == expected_value) {
        return true;
    }

    memcpy(_expected, &ret, sizeof(int64_t));
    return false;
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit_i64(
    volatile int64_t* _obj, int64_t* _expected, int64_t _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_i64(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit_u64(
    volatile mgu_atomic_ullong* _obj, unsigned long long* _expected, unsigned long long _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    long long expected_value = *((long long*)_expected);
    bool ret = __mgu_atomic_compare_exchange_strong_explicit_i64(
        (volatile mgu_atomic_llong*)_obj, &expected_value, *((long long*)&_desired), _success, _failure);
    memcpy(_expected, &expected_value, sizeof(unsigned long long));
    return ret;
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit_u64(
    volatile mgu_atomic_ullong* _obj, unsigned long long* _expected, unsigned long long _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_u64(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_strong_explicit_b(
    volatile mgu_atomic_bool* _obj, bool* _expected, bool _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_strong_explicit_i8(
        (volatile int8_t*)_obj, (int8_t*)_expected, (int8_t)_desired, _success, _failure);
}

MG_CAPI_INLINE bool __mgu_atomic_compare_exchange_weak_explicit_b(
    volatile mgu_atomic_bool* _obj, bool* _expected, bool _desired, mgu_memory_order _success, mgu_memory_order _failure)
{
    return __mgu_atomic_compare_exchange_weak_explicit_i8(
        (volatile int8_t*)_obj, (int8_t*)_expected, (int8_t)_desired, _success, _failure);
}


MG_CAPI_INLINE int8_t __mgu_atomic_fetch_add_explicit_i8(
    volatile int8_t* _obj, int8_t _arg, mgu_memory_order _order)
{
    char ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedExchangeAdd8, (volatile char*)_obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint8_t __mgu_atomic_fetch_add_explicit_u8(
    volatile uint8_t* _obj, uint8_t _arg, mgu_memory_order _order)
{
    int8_t ret = __mgu_atomic_fetch_add_explicit_i8((volatile int8_t*)_obj, *((int8_t*)&_arg), _order);
    return *((uint8_t*)&ret);
}

MG_CAPI_INLINE int16_t __mgu_atomic_fetch_add_explicit_i16(
    volatile int16_t* _obj, int16_t _arg, mgu_memory_order _order)
{
    int16_t ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedExchangeAdd16, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint16_t __mgu_atomic_fetch_add_explicit_u16(
    volatile uint16_t* _obj, uint16_t _arg, mgu_memory_order _order)
{
    int16_t ret = __mgu_atomic_fetch_add_explicit_i16((volatile int16_t*)_obj, *((int16_t*)&_arg), _order);
    return *((uint16_t*)&ret);
}

MG_CAPI_INLINE int32_t __mgu_atomic_fetch_add_explicit_i32(
    volatile int32_t* _obj, int32_t _arg, mgu_memory_order _order)
{
    int32_t ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedExchangeAdd, (volatile long*)_obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint32_t __mgu_atomic_fetch_add_explicit_u32(
    volatile uint32_t* _obj, uint32_t _arg, mgu_memory_order _order)
{
    int32_t ret = __mgu_atomic_fetch_add_explicit_i32((volatile int32_t*)_obj, *((int32_t*)&_arg), _order);
    return *((uint32_t*)&ret);
}

MG_CAPI_INLINE int64_t __mgu_atomic_fetch_add_explicit_i64(
    volatile int64_t* _obj, int64_t _arg, mgu_memory_order _order)
{
    int64_t ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedExchangeAdd64, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint64_t __mgu_atomic_fetch_add_explicit_u64(
    volatile uint64_t* _obj, uint64_t _arg, mgu_memory_order _order)
{
    int64_t ret = __mgu_atomic_fetch_add_explicit_i64((volatile int64_t*)_obj, *((int64_t*)&_arg), _order);
    return *((uint64_t*) & ret);
}


MG_CAPI_INLINE int8_t __mgu_atomic_fetch_sub_explicit_i8(
    volatile int8_t* _obj, int8_t _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_i8(_obj, -_arg, _order);
}

MG_CAPI_INLINE uint8_t __mgu_atomic_fetch_sub_explicit_u8(
    volatile uint8_t* _obj, uint8_t _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_i8((volatile int8_t*)_obj, -(*((int8_t*)&_arg)), _order);
}

MG_CAPI_INLINE int16_t __mgu_atomic_fetch_sub_explicit_i16(
    volatile int16_t* _obj, int16_t _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_i16(_obj, -_arg, _order);
}

MG_CAPI_INLINE uint16_t __mgu_atomic_fetch_sub_explicit_u16(
    volatile uint16_t* _obj, uint16_t _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_i16((volatile int16_t*)_obj, -(*((int16_t*)&_arg)), _order);
}

MG_CAPI_INLINE int32_t __mgu_atomic_fetch_sub_explicit_i32(
    volatile int32_t* _obj, int32_t _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_i32(_obj, -_arg, _order);
}

MG_CAPI_INLINE uint32_t __mgu_atomic_fetch_sub_explicit_u32(
    volatile uint32_t* _obj, uint32_t _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_i32((volatile int32_t*)_obj, -(*((int32_t*)&_arg)), _order);
}

MG_CAPI_INLINE int64_t __mgu_atomic_fetch_sub_explicit_i64(
    volatile int64_t* _obj, int64_t _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_i64(_obj, -_arg, _order);
}

MG_CAPI_INLINE uint64_t __mgu_atomic_fetch_sub_explicit_u64(
    volatile uint64_t* _obj, uint64_t _arg, mgu_memory_order _order)
{
    return __mgu_atomic_fetch_add_explicit_i64((volatile int64_t*)_obj, -(*((int64_t*)&_arg)), _order);
}


MG_CAPI_INLINE int8_t __mgu_atomic_fetch_and_explicit_i8(
    volatile int8_t* _obj, int8_t _arg, mgu_memory_order _order)
{
    int8_t ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedAnd8, (volatile char*)_obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint8_t __mgu_atomic_fetch_and_explicit_u8(
    volatile uint8_t* _obj, uint8_t _arg, mgu_memory_order _order)
{
    int8_t ret = __mgu_atomic_fetch_and_explicit_i8((volatile int8_t*)_obj, *((int8_t*)&_arg), _order);
    return *((uint8_t*)&ret);
}

MG_CAPI_INLINE int16_t __mgu_atomic_fetch_and_explicit_i16(
    volatile int16_t* _obj, int16_t _arg, mgu_memory_order _order)
{
    int16_t ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedAnd16, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint16_t __mgu_atomic_fetch_and_explicit_u16(
    volatile uint16_t* _obj, uint16_t _arg, mgu_memory_order _order)
{
    int16_t ret = __mgu_atomic_fetch_and_explicit_i16((volatile int16_t*)_obj, *((int16_t*)&_arg), _order);
    return *((uint16_t*)&ret);
}

MG_CAPI_INLINE int32_t __mgu_atomic_fetch_and_explicit_i32(
    volatile int32_t* _obj, int32_t _arg, mgu_memory_order _order)
{
    int32_t ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedAnd, (volatile long*)_obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint32_t __mgu_atomic_fetch_and_explicit_u32(
    volatile uint32_t* _obj, uint32_t _arg, mgu_memory_order _order)
{
    int32_t ret = __mgu_atomic_fetch_and_explicit_i32((volatile int32_t*)_obj, *((int32_t*)&_arg), _order);
    return *((uint32_t*)&ret);
}

MG_CAPI_INLINE int64_t __mgu_atomic_fetch_and_explicit_i64(
    volatile int64_t* _obj, int64_t _arg, mgu_memory_order _order)
{
    int64_t ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedAnd64, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint64_t __mgu_atomic_fetch_and_explicit_u64(
    volatile uint64_t* _obj, uint64_t _arg, mgu_memory_order _order)
{
    int64_t ret = __mgu_atomic_fetch_and_explicit_i64((volatile int64_t*)_obj, *((int64_t*)&_arg), _order);
    return *((uint64_t*) & ret);
}


MG_CAPI_INLINE int8_t __mgu_atomic_fetch_or_explicit_i8(
    volatile int8_t* _obj, int8_t _arg, mgu_memory_order _order)
{
    int8_t ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedOr8, (volatile char*)_obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint8_t __mgu_atomic_fetch_or_explicit_u8(
    volatile uint8_t* _obj, uint8_t _arg, mgu_memory_order _order)
{
    int8_t ret = __mgu_atomic_fetch_or_explicit_i8((volatile int8_t*)_obj, *((int8_t*)&_arg), _order);
    return *((uint8_t*)&ret);
}

MG_CAPI_INLINE int16_t __mgu_atomic_fetch_or_explicit_i16(
    volatile int16_t* _obj, int16_t _arg, mgu_memory_order _order)
{
    int16_t ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedOr16, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint16_t __mgu_atomic_fetch_or_explicit_u16(
    volatile uint16_t* _obj, uint16_t _arg, mgu_memory_order _order)
{
    int16_t ret = __mgu_atomic_fetch_or_explicit_i16((volatile int16_t*)_obj, *((int16_t*)&_arg), _order);
    return *((uint16_t*)&ret);
}

MG_CAPI_INLINE int32_t __mgu_atomic_fetch_or_explicit_i32(
    volatile int32_t* _obj, int32_t _arg, mgu_memory_order _order)
{
    int32_t ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedOr, (volatile long*)_obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint32_t __mgu_atomic_fetch_or_explicit_u32(
    volatile uint32_t* _obj, uint32_t _arg, mgu_memory_order _order)
{
    int32_t ret = __mgu_atomic_fetch_or_explicit_i32((volatile int32_t*)_obj, *((int32_t*)&_arg), _order);
    return *((uint32_t*)&ret);
}

MG_CAPI_INLINE int64_t __mgu_atomic_fetch_or_explicit_i64(
    volatile int64_t* _obj, int64_t _arg, mgu_memory_order _order)
{
    int64_t ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedOr64, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint64_t __mgu_atomic_fetch_or_explicit_u64(
    volatile uint64_t* _obj, uint64_t _arg, mgu_memory_order _order)
{
    int64_t ret = __mgu_atomic_fetch_or_explicit_i64((volatile int64_t*)_obj, *((int64_t*)&_arg), _order);
    return *((uint64_t*) & ret);
}


MG_CAPI_INLINE int8_t __mgu_atomic_fetch_xor_explicit_i8(
    volatile int8_t* _obj, int8_t _arg, mgu_memory_order _order)
{
    int8_t ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedXor8, (volatile char*)_obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint8_t __mgu_atomic_fetch_xor_explicit_u8(
    volatile uint8_t* _obj, uint8_t _arg, mgu_memory_order _order)
{
    int8_t ret = __mgu_atomic_fetch_xor_explicit_i8((volatile int8_t*)_obj, *((int8_t*)&_arg), _order);
    return *((uint8_t*)&ret);
}

MG_CAPI_INLINE int16_t __mgu_atomic_fetch_xor_explicit_i16(
    volatile int16_t* _obj, int16_t _arg, mgu_memory_order _order)
{
    int16_t ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedXor16, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint16_t __mgu_atomic_fetch_xor_explicit_u16(
    volatile uint16_t* _obj, uint16_t _arg, mgu_memory_order _order)
{
    int16_t ret = __mgu_atomic_fetch_xor_explicit_i16((volatile int16_t*)_obj, *((int16_t*)&_arg), _order);
    return *((uint16_t*)&ret);
}

MG_CAPI_INLINE int32_t __mgu_atomic_fetch_xor_explicit_i32(
    volatile int32_t* _obj, int32_t _arg, mgu_memory_order _order)
{
    int32_t ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedXor, (volatile long*)_obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint32_t __mgu_atomic_fetch_xor_explicit_u32(
    volatile uint32_t* _obj, uint32_t _arg, mgu_memory_order _order)
{
    int32_t ret = __mgu_atomic_fetch_xor_explicit_i32((volatile int32_t*)_obj, *((int32_t*)&_arg), _order);
    return *((uint32_t*)&ret);
}

MG_CAPI_INLINE int64_t __mgu_atomic_fetch_xor_explicit_i64(
    volatile int64_t* _obj, int64_t _arg, mgu_memory_order _order)
{
    int64_t ret;
    __MGU_ATOMIC_CHOOSE_INTRINSIC(_order, ret, _InterlockedXor64, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint64_t __mgu_atomic_fetch_xor_explicit_u64(
    volatile uint64_t* _obj, uint64_t _arg, mgu_memory_order _order)
{
    int64_t ret = __mgu_atomic_fetch_xor_explicit_i64((volatile int64_t*)_obj, *((int64_t*)&_arg), _order);
    return *((uint64_t*) & ret);
}

#undef __MGU_ATOMIC_USE_ARM64_LDAR_STLR
#undef __MGU_ATOMIC_CHOOSE_INTRINSIC
#undef __MGU_ATOMIC_STORE_RELEASE
#undef __MGU_ATOMIC_STORE_SWITCH_PREFIX
#undef __MGU_ATOMIC_LOAD_ARM64
#undef __MGU_ATOMIC_LOAD_ACQUIRE_ARM64
#undef __MGU_ATOMIC_POST_LOAD_BARRIER_AS_NEEDED

#undef __MGU_ATOMIC_STORE_SEQ_CST_ARM
#undef __MGU_ATOMIC_STORE_SEQ_CST_ARM64
#undef __MGU_ATOMIC_STORE_SEQ_CST_X86_X64
#undef __MGU_ATOMIC_STORE_32_SEQ_CST_X86_X64
#undef __MGU_ATOMIC_STORE_64_SEQ_CST_IX86

#undef __MGU_ATOMIC_INVALID_MEMORY_ORDER
#undef __mgu_atomic_compiler_or_memory_barrier
#undef __mgu_atomic_memory_barrier
#undef __mgu_atomic_compiler_barrier

#undef __MGU_ATOMIC_INTRIN_RELAXED
#undef __MGU_ATOMIC_INTRIN_ACQUIRE
#undef __MGU_ATOMIC_INTRIN_RELEASE
#undef __MGU_ATOMIC_INTRIN_ACQ_REL
#undef __MGU_ATOMIC_YIELD_PROCESSOR

#endif // MG_COMP__MSVC_AVAIL

#endif // !MEGO_UTIL_DETAIL_MSVC_ATOMIC_H_INCLUDED
