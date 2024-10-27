
#ifndef MEME_IMPL_CONC_MSVC_ATOMIC_H_INCLUDED
#define MEME_IMPL_CONC_MSVC_ATOMIC_H_INCLUDED

#include <meme/impl/conc/native_atomic_fwd.h>
#include <meme/impl/conc/msvc_atomic_fwd.h>
#include <mego/predef/compiler/visualc.h>

MEME_EXTERN_C_SCOPE_START
#if MG_COMP__MSVC_AVAIL

#include <mego/predef/helper_macros.h>
#include <mego/predef/symbol/inline.h>
#include <mego/util/os/windows/windows_simplify.h>

#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

#include <crtdbg.h>
#include <intrin.h>


// Interlocked intrinsic mapping for _nf/_acq/_rel
#if defined(_M_CEE_PURE) || defined(_M_IX86) || (defined(_M_X64) && !defined(_M_ARM64EC))
#define __MMCONC_MSVC_ATMC_INTRIN_RELAXED(x) x
#define __MMCONC_MSVC_ATMC_INTRIN_ACQUIRE(x) x
#define __MMCONC_MSVC_ATMC_INTRIN_RELEASE(x) x
#define __MMCONC_MSVC_ATMC_INTRIN_ACQ_REL(x) x
#ifdef _M_CEE_PURE
#define __MMCONC_MSVC_ATMC_YIELD_PROCESSOR()
#else // ^^^ _M_CEE_PURE / !_M_CEE_PURE vvv
#define __MMCONC_MSVC_ATMC_YIELD_PROCESSOR() _mm_pause()
#endif // ^^^ !_M_CEE_PURE ^^^

#elif defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC)

#  define __MMCONC_MSVC_ATMC_INTRIN_RELAXED(x) MEGO__JOIN(x, _nf)
#  define __MMCONC_MSVC_ATMC_INTRIN_ACQUIRE(x) MEGO__JOIN(x, _acq)
#  define __MMCONC_MSVC_ATMC_INTRIN_RELEASE(x) MEGO__JOIN(x, _rel)
// We don't have interlocked intrinsics for acquire-release ordering, even on
// ARM32/ARM64, so fall back to sequentially consistent.
#  define __MMCONC_MSVC_ATMC_INTRIN_ACQ_REL(x) x
#  define __MMCONC_MSVC_ATMC_YIELD_PROCESSOR() __yield()

#else // ^^^ ARM32/ARM64 / unsupported hardware vvv
#  error Unsupported hardware
#endif // hardware


// The following is modified from the _CRT_SECURE_INVALID_PARAMETER macro in
// corecrt.h. We need to do this because this header must be C, not C++, but we
// still want to report invalid parameters in the same way as C++ does. The
// macro in the CRT expands to C++ code because it contains global namespace
// qualification. This can be fixed in the ucrt by using a mechanism that
// defines something like _GLOBAL_NAMESPACE to :: in c++ mode and nothing in C
// mode.
#ifndef __MMCONC_MSVC_ATMC_INVALID_PARAMETER
#ifdef _DEBUG
#define __MMCONC_MSVC_ATMC_INVALID_PARAMETER(expr) _invalid_parameter(_CRT_WIDE(#expr), L"", __FILEW__, __LINE__, 0)
#else
// By default, __MMCONC_MSVC_ATMC_INVALID_PARAMETER in retail invokes
// _invalid_parameter_noinfo_noreturn(), which is marked
// __declspec(noreturn) and does not return control to the application.
// Even if _set_invalid_parameter_handler() is used to set a new invalid
// parameter handler which does return control to the application,
// _invalid_parameter_noinfo_noreturn() will terminate the application
// and invoke Watson. You can overwrite the definition of
// __MMCONC_MSVC_ATMC_INVALID_PARAMETER if you need.
#define __MMCONC_MSVC_ATMC_INVALID_PARAMETER(expr) _invalid_parameter_noinfo_noreturn()
#endif
#endif


// Controls whether ARM64 ldar/ldapr/stlr should be used
#ifndef __MMCONC_MSVC_ATMC_USE_ARM64_LDAR_STLR
#  if defined(_M_ARM64) || defined(_M_ARM64EC)
#    if defined(_HAS_ARM64_LOAD_ACQUIRE) && _HAS_ARM64_LOAD_ACQUIRE == 1 // TRANSITION, VS 2022 17.7 Preview 1
#      define __MMCONC_MSVC_ATMC_USE_ARM64_LDAR_STLR 1
#    else // ^^^ updated intrin0.inl.h is available / workaround vvv
#      define __MMCONC_MSVC_ATMC_USE_ARM64_LDAR_STLR 0
#    endif // ^^^ workaround ^^^
#  else // ^^^ ARM64/ARM64EC / Other architectures vvv
#    define __MMCONC_MSVC_ATMC_USE_ARM64_LDAR_STLR 0
#  endif // ^^^ Other architectures ^^^
#endif // __MMCONC_MSVC_ATMC_USE_ARM64_LDAR_STLR


#ifndef __MMCONC_MSVC_ATMC_INVALID_MEMORY_ORDER
#  ifdef _DEBUG
#define __MMCONC_MSVC_ATMC_INVALID_MEMORY_ORDER                         \
    do {                                                                \
        _RPTF0(_CRT_ASSERT, "Invalid memory order");                    \
        __MMCONC_MSVC_ATMC_INVALID_PARAMETER("Invalid memory order");   \
    } while (0)
#  else // ^^^ defined(_DEBUG) / !defined(_DEBUG) vvv
#    define __MMCONC_MSVC_ATMC_INVALID_MEMORY_ORDER
#  endif // ^^^ !defined(_DEBUG) ^^^
#endif // __MMCONC_MSVC_ATMC_INVALID_MEMORY_ORDER


// this is different from the STL
// we are the MSVC runtime so we need not support clang here
//#define __mmconc_msvc_atmc_compiler_barrier()                                                       \
//    _Pragma("warning(push)") _Pragma("warning(disable : 4996)") /* was declared deprecated */ \
//        _ReadWriteBarrier() _Pragma("warning(pop)")
#define __mmconc_msvc_atmc_compiler_barrier() \
        _ReadWriteBarrier() 

#if defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC)
#  define __mmconc_msvc_atmc_memory_barrier()             __dmb(0xB) // inner shared data memory barrier
#  define __mmconc_msvc_atmc_compiler_or_memory_barrier() __mmconc_msvc_atmc_memory_barrier()
#elif defined(_M_IX86) || defined(_M_X64)
// x86/x64 hardware only emits memory barriers inside _Interlocked intrinsics
#  define __mmconc_msvc_atmc_compiler_or_memory_barrier() __mmconc_msvc_atmc_compiler_barrier()
#else // ^^^ x86/x64 / unsupported hardware vvv
#  error Unsupported hardware
#endif // hardware

MG_CAPI_INLINE void __mmconc_msvc_atmc_check_memory_order(const unsigned int _Order) 
{
    if (_Order > mmconc_memory_order_seq_cst) {
        __MMCONC_MSVC_ATMC_INVALID_MEMORY_ORDER;
    }
}


#if defined(_M_IX86) || (defined(_M_X64) && !defined(_M_ARM64EC))
#define __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_Order, _Result, _Intrinsic, ...)   \
    __mmconc_msvc_atmc_check_memory_order(_Order);                              \
    _Result = _Intrinsic(__VA_ARGS__)
#elif defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC)
#define __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_Order, _Result, _Intrinsic, ...)   \
    switch (_Order) {                                                           \
    case mmconc_memory_order_relaxed:                                           \
        _Result = __MMCONC_MSVC_ATMC_INTRIN_RELAXED(_Intrinsic)(__VA_ARGS__);   \
        break;                                                                  \
    case mmconc_memory_order_consume:                                           \
    case mmconc_memory_order_acquire:                                           \
        _Result = __MMCONC_MSVC_ATMC_INTRIN_ACQUIRE(_Intrinsic)(__VA_ARGS__);   \
        break;                                                                  \
    case mmconc_memory_order_release:                                           \
        _Result = __MMCONC_MSVC_ATMC_INTRIN_RELEASE(_Intrinsic)(__VA_ARGS__);   \
        break;                                                                  \
    default:                                                                    \
        __MMCONC_MSVC_ATMC_INVALID_MEMORY_ORDER;                                \
        /*FALLTHROUGH*/;                                                        \
    case mmconc_memory_order_acq_rel:                                           \
    case mmconc_memory_order_seq_cst:                                           \
        _Result = _Intrinsic(__VA_ARGS__);                                      \
        break;                                                                  \
    }
#endif 


#if __MMCONC_MSVC_ATMC_USE_ARM64_LDAR_STLR == 1

#define __MMCONC_MSVC_ATMC_LOAD_ACQUIRE_ARM64(_Width, _Ptr) \
    __load_acquire##_Width((const volatile unsigned __int##_Width*)(_Ptr))

#define __MMCONC_MSVC_ATMC_LOAD_ARM64_IMPL(_Result, _Width, _Ptr, _Order_var)    \
    switch (_Order_var) {                                                   \
    case mmconc_memory_order_relaxed:                                       \
        _Result = __iso_volatile_load##_Width(_Ptr);                        \
        break;                                                              \
    case mmconc_memory_order_consume:                                       \
    case mmconc_memory_order_acquire:                                       \
    case mmconc_memory_order_seq_cst:                                       \
        _Result = __MMCONC_MSVC_ATMC_LOAD_ACQUIRE_ARM64(_Width, _Ptr);      \
        __mmconc_msvc_atmc_compiler_barrier();                              \
        break;                                                              \
    case mmconc_memory_order_release:                                       \
    case mmconc_memory_order_acq_rel:                                       \
    default:                                                                \
        _Result = __iso_volatile_load##_Width(_Ptr);                        \
        __MMCONC_MSVC_ATMC_INVALID_MEMORY_ORDER;                            \
        break;                                                              \
    }

#endif


#define __MMCONC_MSVC_ATMC_LOAD_IMPL(_Result, _Width, _Ptr, _Order_var)          \
    switch (_Order_var) {                                                   \
    case mmconc_memory_order_relaxed:                                       \
        _Result = __mmconc_msvc_atmc_load_relaxed_##_Width(_Ptr);           \
        break;                                                              \
    case mmconc_memory_order_consume:                                       \
    case mmconc_memory_order_acquire:                                       \
        _Result = __mmconc_msvc_atmc_load_acquire_##_Width(_Ptr);           \
        break;                                                              \
    case mmconc_memory_order_seq_cst:                                       \
        _Result = __mmconc_msvc_atmc_load_seq_cst_##_Width(_Ptr);           \
        break;                                                              \
    case mmconc_memory_order_release:                                       \
    case mmconc_memory_order_acq_rel:                                       \
    default:                                                                \
        __MMCONC_MSVC_ATMC_INVALID_MEMORY_ORDER;                            \
        break;                                                              \
    }

#define __MMCONC_MSVC_ATMC_POST_LOAD_BARRIER_AS_NEEDED(_Order_var)  \
    switch (_Order_var) {                                           \
    case mmconc_memory_order_relaxed:                               \
        break;                                                      \
    case mmconc_memory_order_consume:                               \
    case mmconc_memory_order_acquire:                               \
    case mmconc_memory_order_seq_cst:                               \
        __mmconc_msvc_atmc_compiler_or_memory_barrier();            \
        break;                                                      \
    case mmconc_memory_order_release:                               \
    case mmconc_memory_order_acq_rel:                               \
    default:                                                        \
        __MMCONC_MSVC_ATMC_INVALID_MEMORY_ORDER;                    \
        break;                                                      \
    }


#if __MMCONC_MSVC_ATMC_USE_ARM64_LDAR_STLR == 1

#define __MMCONC_MSVC_ATMC_STORE_RELEASE(_Width, _Ptr, _Desired)       \
    __mmconc_msvc_atmc_compiler_barrier();                             \
    __stlr##_Width((volatile unsigned __int##_Width*)(_Ptr), (_Desired));

#else

#define __MMCONC_MSVC_ATMC_STORE_RELEASE(_Width, _Ptr, _Desired)       \
    __mmconc_msvc_atmc_compiler_or_memory_barrier();                   \
    __iso_volatile_store##_Width((_Ptr), (_Desired));

#endif


#define __MMCONC_MSVC_ATMC_STORE_SWITCH_PREFIX(_Width, _Ptr, _Desired)      \
    case mmconc_memory_order_relaxed:                                       \
        __mmconc_msvc_atmc_store_relaxed_##_Width((_Ptr), (_Desired));      \
        return;                                                             \
    case mmconc_memory_order_release:                                       \
        __mmconc_msvc_atmc_store_release_##_Width((_Ptr), (_Desired));      \
        return;                                                             \
    case mmconc_memory_order_consume:                                       \
    case mmconc_memory_order_acquire:                                       \
    case mmconc_memory_order_acq_rel:                                       \
    default:                                                                \
        __MMCONC_MSVC_ATMC_INVALID_MEMORY_ORDER;                            \
        /*FALLTHROUGH*/;

#define __MMCONC_MSVC_ATMC_STORE_IMPL(_Width, _Ptr, _Desired, _Order_var)   \
    switch (_Order_var) {                                                   \
    case mmconc_memory_order_seq_cst:                                       \
        __mmconc_msvc_atmc_store_seq_cst_##_Width((_Ptr), (_Desired));      \
        return;                                                             \
    __MMCONC_MSVC_ATMC_STORE_SWITCH_PREFIX(_Width, _Ptr, _Desired)          \
    }

#define __MMCONC_MSVC_ATMC_STORE_SEQ_CST_ARM(_Width, _Ptr, _Desired)    \
    __mmconc_msvc_atmc_memory_barrier();                                \
    __iso_volatile_store##_Width((_Ptr), (_Desired));                   \
    __mmconc_msvc_atmc_memory_barrier();

#if __MMCONC_MSVC_ATMC_USE_ARM64_LDAR_STLR == 1
#define __MMCONC_MSVC_ATMC_STORE_SEQ_CST_ARM64(_Width, _Ptr, _Desired)          \
    __mmconc_msvc_atmc_compiler_barrier();                                      \
    __stlr##_Width((volatile unsigned __int##_Width*)(_Ptr), (_Desired));       \
    __mmconc_msvc_atmc_memory_barrier();
#else
#define __MMCONC_MSVC_ATMC_STORE_SEQ_CST_ARM64 _ATOMIC_STORE_SEQ_CST_ARM
#endif

#define __MMCONC_MSVC_ATMC_STORE_SEQ_CST_X86_X64(_Width, _Ptr, _Desired) (void) _InterlockedExchange##_Width((_Ptr), (_Desired));
#define __MMCONC_MSVC_ATMC_STORE_32_SEQ_CST_X86_X64(_Ptr, _Desired) \
    (void) _InterlockedExchange((volatile long*)(_Ptr), (long)(_Desired));

#define __MMCONC_MSVC_ATMC_STORE_64_SEQ_CST_IX86(_Ptr, _Desired) \
    __mmconc_msvc_atmc_compiler_barrier();                              \
    __iso_volatile_store64((_Ptr), (_Desired));                         \
    _Atomic_thread_fence(_Atomic_memory_order_seq_cst);



MG_CAPI_INLINE int8_t __mmconc_msvc_atmc_load_relaxed_8(const volatile int8_t * _source)
{
#if defined(_M_ARM) || defined(_M_ARM64)
    return __iso_volatile_load8((volatile char*)_source);
#else
    return *(_source);
#endif
}

MG_CAPI_INLINE int8_t __mmconc_msvc_atmc_load_seq_cst_8(const volatile int8_t* _source)
{
    int8_t result = __mmconc_msvc_atmc_load_relaxed_8(_source);
    __mmconc_msvc_atmc_compiler_or_memory_barrier();
    return result;
}

MG_CAPI_INLINE int8_t __mmconc_msvc_atmc_load_acquire_8(const volatile int8_t* _source)
{
    return __mmconc_msvc_atmc_load_seq_cst_8(_source);
}

MG_CAPI_INLINE int16_t __mmconc_msvc_atmc_load_relaxed_16(const volatile int16_t* _source)
{
#if defined(_M_ARM) || defined(_M_ARM64)
    return __iso_volatile_load16((volatile short*)_source);
#else
    return *(_source);
#endif
}

MG_CAPI_INLINE int16_t __mmconc_msvc_atmc_load_seq_cst_16(const volatile int16_t* _source)
{
    int16_t result = __mmconc_msvc_atmc_load_relaxed_16(_source);
    __mmconc_msvc_atmc_compiler_or_memory_barrier();
    return result;
}

MG_CAPI_INLINE int16_t __mmconc_msvc_atmc_load_acquire_16(const volatile int16_t* _source)
{
    return __mmconc_msvc_atmc_load_seq_cst_16(_source);
}

MG_CAPI_INLINE int32_t __mmconc_msvc_atmc_load_relaxed_32(const volatile int32_t* _source)
{
#if defined(_M_ARM) || defined(_M_ARM64)
    return __iso_volatile_load32((volatile long*)_source);
#else
    return *(_source);
#endif
}

MG_CAPI_INLINE int32_t __mmconc_msvc_atmc_load_seq_cst_32(const volatile int32_t* _source)
{
    int32_t result = __mmconc_msvc_atmc_load_relaxed_32(_source);
    __mmconc_msvc_atmc_compiler_or_memory_barrier();
    return result;
}

MG_CAPI_INLINE int32_t __mmconc_msvc_atmc_load_acquire_32(const volatile int32_t* _source)
{
    return __mmconc_msvc_atmc_load_seq_cst_32(_source);
}

MG_CAPI_INLINE int64_t __mmconc_msvc_atmc_load_relaxed_64(const volatile int64_t* _source)
{
#if defined(_M_X64)
    return *_source;
#elif defined(_M_ARM)
    return __ldrexd(_source);
#elif defined(_M_ARM64)
    return __iso_volatile_load64(_source);
#else
    return _InterlockedCompareExchange64(_source, 0, 0);
#endif
}

MG_CAPI_INLINE int64_t __mmconc_msvc_atmc_load_seq_cst_64(const volatile int64_t* _source)
{
    int64_t result;
#if defined(_M_X64)
    result = *_source;
    __mmconc_msvc_atmc_compiler_or_memory_barrier();
#elif defined(_M_ARM)
    result = __ldrexd(_source);
    __mmconc_msvc_atmc_compiler_or_memory_barrier();
#elif defined(_M_ARM64)
    result = __iso_volatile_load64(_source);
    __mmconc_msvc_atmc_compiler_or_memory_barrier();
#else
    result = _InterlockedCompareExchange64(_source, 0, 0);
#endif
    return result;
}

MG_CAPI_INLINE int64_t __mmconc_msvc_atmc_load_acquire_64(const volatile int64_t* _source)
{
    return __mmconc_msvc_atmc_load_seq_cst_64(_source);
}



MG_CAPI_INLINE void __mmconc_msvc_atmc_store_relaxed_8(volatile int8_t* _target, int8_t _value)
{
#if defined(_M_ARM) || defined(_M_ARM64)
    __iso_volatile_store8((volatile char*)_target, _value);
#else
    * (_target) = _value;
#endif
}

MG_CAPI_INLINE void __mmconc_msvc_atmc_store_release_8(volatile int8_t* _target, int8_t _value)
{
    __mmconc_msvc_atmc_compiler_or_memory_barrier();
    __mmconc_msvc_atmc_store_relaxed_8(_target, _value);
}

MG_CAPI_INLINE void __mmconc_msvc_atmc_store_seq_cst_8(volatile int8_t* _target, int8_t _value)
{
#if defined(_M_ARM) || defined(_M_ARM64)
    __mmconc_msvc_atmc_compiler_or_memory_barrier();
    __iso_volatile_store8((volatile char*)_target, _value);
    __mmconc_msvc_atmc_compiler_or_memory_barrier();
#else
    _InterlockedExchange8((volatile char*)_target, _value);
#endif
}

MG_CAPI_INLINE void __mmconc_msvc_atmc_store_relaxed_16(volatile int16_t* _target, int16_t _value)
{
#if defined(_M_ARM) || defined(_M_ARM64)
    __iso_volatile_store16((volatile short*)_target, _value);
#else
    * (_target) = _value;
#endif
}

MG_CAPI_INLINE void __mmconc_msvc_atmc_store_release_16(volatile int16_t* _target, int16_t _value)
{
    __mmconc_msvc_atmc_compiler_or_memory_barrier();
    __mmconc_msvc_atmc_store_relaxed_16(_target, _value);
}

MG_CAPI_INLINE void __mmconc_msvc_atmc_store_seq_cst_16(volatile int16_t* _target, int16_t _value)
{
#if defined(_M_ARM) || defined(_M_ARM64)
    __mmconc_msvc_atmc_compiler_or_memory_barrier();
    __iso_volatile_store16((volatile short*)_target, _value);
    __mmconc_msvc_atmc_compiler_or_memory_barrier();
#else
    _InterlockedExchange16((volatile short*)_target, _value);
#endif
}

MG_CAPI_INLINE void __mmconc_msvc_atmc_store_relaxed_32(volatile int32_t* _target, int32_t _value)
{
#if defined(_M_ARM) || defined(_M_ARM64)
    __iso_volatile_store32((volatile long*)_target, _value);
#else
    * (_target) = _value;
#endif
}

MG_CAPI_INLINE void __mmconc_msvc_atmc_store_release_32(volatile int32_t* _target, int32_t _value)
{
    __mmconc_msvc_atmc_compiler_or_memory_barrier();
    __mmconc_msvc_atmc_store_relaxed_32(_target, _value);
}

MG_CAPI_INLINE void __mmconc_msvc_atmc_store_seq_cst_32(volatile int32_t* _target, int32_t _value)
{
#if defined(_M_ARM) || defined(_M_ARM64)
    __mmconc_msvc_atmc_compiler_or_memory_barrier();
    __iso_volatile_store32((volatile long*)_target, _value);
    __mmconc_msvc_atmc_compiler_or_memory_barrier();
#else
    _InterlockedExchange((volatile long*)_target, _value);
#endif
}

MG_CAPI_INLINE void __mmconc_msvc_atmc_store_relaxed_64(volatile int64_t* _target, int64_t _value)
{
#if defined(_M_X64)
    * (_target) = _value;
#elif defined(_M_ARM64)
    __iso_volatile_store64(_target, _value);
#else
    __MMCONC_MSVC_ATMC_INTRIN_RELAXED(_InterlockedExchange64)(_target, _value);
#endif
}

MG_CAPI_INLINE void __mmconc_msvc_atmc_store_release_64(volatile int64_t* _target, int64_t _value)
{
#if defined(_M_X64)
    __mmconc_msvc_atmc_compiler_or_memory_barrier();
    *(_target) = _value;
#elif defined(_M_ARM64)
    __mmconc_msvc_atmc_compiler_or_memory_barrier();
    __iso_volatile_store64(_target, _value);
#else
    __MMCONC_MSVC_ATMC_INTRIN_RELEASE(_InterlockedExchange64)(_target, _value);
#endif
}

MG_CAPI_INLINE void __mmconc_msvc_atmc_store_seq_cst_64(volatile int64_t* _target, int64_t _value)
{
#if defined(_M_ARM64)
    __mmconc_msvc_atmc_compiler_or_memory_barrier();
    __iso_volatile_store64(_target, _value);
    __mmconc_msvc_atmc_compiler_or_memory_barrier();
#else
    _InterlockedExchange64(_target, _value);
#endif
}



MG_CAPI_INLINE bool mmconc_msvc_atmc_bl_is_lock_free (const volatile mmconc_msvc_atmc_bl_t*  _obj) { return true; }
MG_CAPI_INLINE bool mmconc_msvc_atmc_i8_is_lock_free (const volatile mmconc_msvc_atmc_i8_t*  _obj) { return true; }
MG_CAPI_INLINE bool mmconc_msvc_atmc_u8_is_lock_free (const volatile mmconc_msvc_atmc_u8_t*  _obj) { return true; }
MG_CAPI_INLINE bool mmconc_msvc_atmc_i16_is_lock_free(const volatile mmconc_msvc_atmc_i16_t* _obj) { return true; }
MG_CAPI_INLINE bool mmconc_msvc_atmc_u16_is_lock_free(const volatile mmconc_msvc_atmc_u16_t* _obj) { return true; }
MG_CAPI_INLINE bool mmconc_msvc_atmc_i32_is_lock_free(const volatile mmconc_msvc_atmc_i32_t* _obj) { return true; }
MG_CAPI_INLINE bool mmconc_msvc_atmc_u32_is_lock_free(const volatile mmconc_msvc_atmc_u32_t* _obj) { return true; }
MG_CAPI_INLINE bool mmconc_msvc_atmc_i64_is_lock_free(const volatile mmconc_msvc_atmc_i64_t* _obj) { return true; }
MG_CAPI_INLINE bool mmconc_msvc_atmc_u64_is_lock_free(const volatile mmconc_msvc_atmc_u64_t* _obj) { return true; }


MG_CAPI_INLINE void mmconc_msvc_atmc_bl_init (volatile mmconc_msvc_atmc_bl_t*  _obj, bool     _desired) { *_obj = _desired; }
MG_CAPI_INLINE void mmconc_msvc_atmc_i8_init (volatile mmconc_msvc_atmc_i8_t*  _obj, int8_t   _desired) { *_obj = _desired; }
MG_CAPI_INLINE void mmconc_msvc_atmc_u8_init (volatile mmconc_msvc_atmc_u8_t*  _obj, uint8_t  _desired) { *_obj = _desired; }
MG_CAPI_INLINE void mmconc_msvc_atmc_i16_init(volatile mmconc_msvc_atmc_i16_t* _obj, int16_t  _desired) { *_obj = _desired; }
MG_CAPI_INLINE void mmconc_msvc_atmc_u16_init(volatile mmconc_msvc_atmc_u16_t* _obj, uint16_t _desired) { *_obj = _desired; }
MG_CAPI_INLINE void mmconc_msvc_atmc_i32_init(volatile mmconc_msvc_atmc_i32_t* _obj, int32_t  _desired) { *_obj = _desired; }
MG_CAPI_INLINE void mmconc_msvc_atmc_u32_init(volatile mmconc_msvc_atmc_u32_t* _obj, uint32_t _desired) { *_obj = _desired; }
MG_CAPI_INLINE void mmconc_msvc_atmc_i64_init(volatile mmconc_msvc_atmc_i64_t* _obj, int64_t  _desired) { *_obj = _desired; }
MG_CAPI_INLINE void mmconc_msvc_atmc_u64_init(volatile mmconc_msvc_atmc_u64_t* _obj, uint64_t _desired) { *_obj = _desired; }


MG_CAPI_INLINE int8_t mmconc_msvc_atmc_i8_load_explicit(
    const volatile mmconc_msvc_atmc_i8_t* _obj, mmconc_memory_order_e _order)
{
    int8_t result = 0;
#if __MMCONC_MSVC_ATMC_USE_ARM64_LDAR_STLR == 1
    __MMCONC_MSVC_ATMC_LOAD_ARM64_IMPL(result, 8, (const volatile char*)_obj, _order)
#else
    __MMCONC_MSVC_ATMC_LOAD_IMPL(result, 8, _obj, _order)
#endif
    return result;
}

MG_CAPI_INLINE uint8_t mmconc_msvc_atmc_u8_load_explicit(
    const volatile mmconc_msvc_atmc_u8_t* _obj, mmconc_memory_order_e _order)
{
    int8_t result = mmconc_msvc_atmc_i8_load_explicit((volatile mmconc_msvc_atmc_i8_t*)_obj, _order);
    return *((uint8_t*)&result);
}

MG_CAPI_INLINE int16_t mmconc_msvc_atmc_i16_load_explicit(
    const volatile mmconc_msvc_atmc_i16_t* _obj, mmconc_memory_order_e _order)
{
    int16_t result = 0;
#if __MMCONC_MSVC_ATMC_USE_ARM64_LDAR_STLR == 1
    __MMCONC_MSVC_ATMC_LOAD_ARM64_IMPL(result, 16, _obj, _order)
#else
    __MMCONC_MSVC_ATMC_LOAD_IMPL(result, 16, _obj, _order)
#endif
    return result;
}

MG_CAPI_INLINE uint16_t mmconc_msvc_atmc_u16_load_explicit(
    const volatile mmconc_msvc_atmc_u16_t* _obj, mmconc_memory_order_e _order)
{
    int16_t result = mmconc_msvc_atmc_i16_load_explicit((volatile int16_t*)_obj, _order);
    return *((uint16_t*)&result);
}

MG_CAPI_INLINE int32_t mmconc_msvc_atmc_i32_load_explicit(
    const volatile mmconc_msvc_atmc_i32_t* _obj, mmconc_memory_order_e _order)
{
    int32_t result = 0;
#if __MMCONC_MSVC_ATMC_USE_ARM64_LDAR_STLR == 1
    __MMCONC_MSVC_ATMC_LOAD_ARM64_IMPL(result, 32, _obj, _order)
#else
    __MMCONC_MSVC_ATMC_LOAD_IMPL(result, 32, _obj, _order)
#endif
    return result;
}

MG_CAPI_INLINE uint32_t mmconc_msvc_atmc_u32_load_explicit(
    const volatile mmconc_msvc_atmc_u32_t* _obj, mmconc_memory_order_e _order)
{
    int32_t result = mmconc_msvc_atmc_i32_load_explicit((volatile int32_t*)_obj, _order);
    return *((uint32_t*)&result);
}

MG_CAPI_INLINE int64_t mmconc_msvc_atmc_i64_load_explicit(
    const volatile mmconc_msvc_atmc_i64_t* _obj, mmconc_memory_order_e _order)
{
    int64_t result = 0;
#if __MMCONC_MSVC_ATMC_USE_ARM64_LDAR_STLR == 1
    __MMCONC_MSVC_ATMC_LOAD_ARM64_IMPL(result, 64, _obj, _order)
#else
    __MMCONC_MSVC_ATMC_LOAD_IMPL(result, 64, _obj, _order)
#endif
    return result;
}

MG_CAPI_INLINE uint64_t mmconc_msvc_atmc_u64_load_explicit(
    const volatile mmconc_msvc_atmc_u64_t* _obj, mmconc_memory_order_e _order)
{
    int64_t result = mmconc_msvc_atmc_i64_load_explicit((volatile int64_t*)_obj, _order);
    return *((uint64_t*)&result);
}

MG_CAPI_INLINE bool mmconc_msvc_atmc_bl_load_explicit(
    const volatile mmconc_msvc_atmc_bl_t* _obj, mmconc_memory_order_e _order)
{
    return (bool)mmconc_msvc_atmc_i8_load_explicit((volatile mmconc_msvc_atmc_i8_t*)_obj, _order);
}


MG_CAPI_INLINE void mmconc_msvc_atmc_i8_store_explicit(
    volatile mmconc_msvc_atmc_i8_t* _obj, int8_t _desired, mmconc_memory_order_e _order)
{
    __MMCONC_MSVC_ATMC_STORE_IMPL(8, _obj, _desired, _order)
}

MG_CAPI_INLINE void mmconc_msvc_atmc_u8_store_explicit(
    volatile mmconc_msvc_atmc_u8_t* _obj, uint8_t _desired, mmconc_memory_order_e _order)
{
    mmconc_msvc_atmc_i8_store_explicit(
        (volatile mmconc_msvc_atmc_i8_t*)_obj, *((int8_t*)&_desired), _order);
}

MG_CAPI_INLINE void mmconc_msvc_atmc_i16_store_explicit(
    volatile mmconc_msvc_atmc_i16_t* _obj, int16_t _desired, mmconc_memory_order_e _order)
{
    __MMCONC_MSVC_ATMC_STORE_IMPL(16, _obj, _desired, _order)
}

MG_CAPI_INLINE void mmconc_msvc_atmc_u16_store_explicit(
    volatile mmconc_msvc_atmc_u16_t* _obj, uint16_t _desired, mmconc_memory_order_e _order)
{
    mmconc_msvc_atmc_i16_store_explicit((volatile int16_t*)_obj, *((int16_t*)&_desired), _order);
}

MG_CAPI_INLINE void mmconc_msvc_atmc_i32_store_explicit(
    volatile mmconc_msvc_atmc_i32_t* _obj, int32_t _desired, mmconc_memory_order_e _order)
{
    __MMCONC_MSVC_ATMC_STORE_IMPL(32, _obj, _desired, _order)
}

MG_CAPI_INLINE void mmconc_msvc_atmc_u32_store_explicit(
    volatile mmconc_msvc_atmc_u32_t* _obj, uint32_t _desired, mmconc_memory_order_e _order)
{
    mmconc_msvc_atmc_i32_store_explicit((volatile int32_t*)_obj, *((int32_t*)&_desired), _order);
}

MG_CAPI_INLINE void mmconc_msvc_atmc_i64_store_explicit(
    volatile mmconc_msvc_atmc_i64_t* _obj, int64_t _desired, mmconc_memory_order_e _order)
{
    __MMCONC_MSVC_ATMC_STORE_IMPL(64, _obj, _desired, _order)
}

MG_CAPI_INLINE void mmconc_msvc_atmc_u64_store_explicit(
    volatile mmconc_msvc_atmc_u64_t* _obj, uint64_t _desired, mmconc_memory_order_e _order)
{
    mmconc_msvc_atmc_i64_store_explicit((volatile int64_t*)_obj, *((int64_t*)&_desired), _order);
}

MG_CAPI_INLINE void mmconc_msvc_atmc_bl_store_explicit(
    volatile mmconc_msvc_atmc_bl_t* _obj, bool _desired, mmconc_memory_order_e _order)
{
    mmconc_msvc_atmc_i8_store_explicit(
        (volatile mmconc_msvc_atmc_i8_t*)_obj, *((int8_t*)&_desired), _order);
}


MG_CAPI_INLINE int8_t mmconc_msvc_atmc_i8_exchange_explicit(
    volatile mmconc_msvc_atmc_i8_t* _obj, int8_t _desired, mmconc_memory_order_e _order)
{
    int8_t ret;
    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_order, ret, InterlockedExchange8, (volatile char*)_obj, _desired);
    return ret;
}

MG_CAPI_INLINE uint8_t mmconc_msvc_atmc_u8_exchange_explicit(
    volatile mmconc_msvc_atmc_u8_t* _obj, uint8_t _desired, mmconc_memory_order_e _order)
{
    int8_t ret = mmconc_msvc_atmc_i8_exchange_explicit(
        (volatile mmconc_msvc_atmc_i8_t*)_obj, *((int8_t*)&_desired), _order);
    return *((uint8_t*)&ret);
}

MG_CAPI_INLINE int16_t mmconc_msvc_atmc_i16_exchange_explicit(
    volatile mmconc_msvc_atmc_i16_t* _obj, int16_t _desired, mmconc_memory_order_e _order)
{
    int16_t ret;
    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_order, ret, InterlockedExchange16, _obj, _desired);
    return ret;
}

MG_CAPI_INLINE uint16_t mmconc_msvc_atmc_u16_exchange_explicit(
    volatile mmconc_msvc_atmc_u16_t* _obj, uint16_t _desired, mmconc_memory_order_e _order)
{
    int16_t ret = mmconc_msvc_atmc_i16_exchange_explicit(
        (volatile mmconc_msvc_atmc_i16_t*)_obj, *((int16_t*)&_desired), _order);
    return *((uint16_t*)&ret);
}

MG_CAPI_INLINE int32_t mmconc_msvc_atmc_i32_exchange_explicit(
    volatile mmconc_msvc_atmc_i32_t* _obj, int32_t _desired, mmconc_memory_order_e _order)
{
    int32_t ret;
    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_order, ret, InterlockedExchange, (volatile long*)_obj, _desired);
    return ret;
}

MG_CAPI_INLINE uint32_t mmconc_msvc_atmc_u32_exchange_explicit(
    volatile mmconc_msvc_atmc_u32_t* _obj, uint32_t _desired, mmconc_memory_order_e _order)
{
    int32_t ret = mmconc_msvc_atmc_i32_exchange_explicit(
        (volatile mmconc_msvc_atmc_i32_t*)_obj, *((int32_t*)&_desired), _order);
    return *((uint32_t*)&ret);
}

MG_CAPI_INLINE int64_t mmconc_msvc_atmc_i64_exchange_explicit(
    volatile mmconc_msvc_atmc_i64_t* _obj, int64_t _desired, mmconc_memory_order_e _order)
{
    int64_t ret;
    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_order, ret, InterlockedExchange64, _obj, _desired);
    return ret;
}

MG_CAPI_INLINE uint64_t mmconc_msvc_atmc_u64_exchange_explicit(
    volatile mmconc_msvc_atmc_u64_t* _obj, uint64_t _desired, mmconc_memory_order_e _order)
{
    int64_t ret = mmconc_msvc_atmc_i64_exchange_explicit(
        (volatile mmconc_msvc_atmc_i64_t*)_obj, *((int64_t*)&_desired), _order);
    return *((uint64_t*) & ret);
}

MG_CAPI_INLINE bool mmconc_msvc_atmc_bl_exchange_explicit(
    volatile mmconc_msvc_atmc_bl_t* _obj, bool _desired, mmconc_memory_order_e _order)
{
    return (bool)mmconc_msvc_atmc_i8_exchange_explicit(
        (volatile mmconc_msvc_atmc_i8_t*)_obj, (int8_t)_desired, _order);
}


MG_CAPI_INLINE bool mmconc_msvc_atmc_i8_compare_exchange_strong_explicit(
    volatile mmconc_msvc_atmc_i8_t* _obj, int8_t* _expected, int8_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    int8_t expected_value = *_expected;
    int8_t ret;

    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_success, ret, _InterlockedCompareExchange8, (volatile char*)_obj, _desired, expected_value);
    if (ret == expected_value) {
        return true;
    }

    memcpy(_expected, &ret, sizeof(int8_t));
    return false;
}

MG_CAPI_INLINE bool mmconc_msvc_atmc_i8_compare_exchange_weak_explicit(
    volatile mmconc_msvc_atmc_i8_t* _obj, int8_t* _expected, int8_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    return mmconc_msvc_atmc_i8_compare_exchange_strong_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE bool mmconc_msvc_atmc_u8_compare_exchange_strong_explicit(
    volatile mmconc_msvc_atmc_u8_t* _obj, uint8_t* _expected, uint8_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    int8_t expected_value = *((int8_t*)_expected);
    bool ret = mmconc_msvc_atmc_i8_compare_exchange_strong_explicit(
        (volatile mmconc_msvc_atmc_i8_t*)_obj, &expected_value, *((int8_t*)&_desired), _success, _failure);
    if (!ret)
        memcpy(_expected, &expected_value, sizeof(uint8_t));
    return ret;
}

MG_CAPI_INLINE bool mmconc_msvc_atmc_u8_compare_exchange_weak_explicit(
    volatile mmconc_msvc_atmc_u8_t* _obj, uint8_t* _expected, uint8_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    return mmconc_msvc_atmc_u8_compare_exchange_strong_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE bool mmconc_msvc_atmc_i16_compare_exchange_strong_explicit(
    volatile mmconc_msvc_atmc_i16_t* _obj, int16_t* _expected, int16_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    int16_t expected_value = *_expected;
    int16_t ret;

    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_success, ret, InterlockedCompareExchange16, _obj, _desired, expected_value);
    if (ret == expected_value) {
        return true;
    }

    memcpy(_expected, &ret, sizeof(int16_t));
    return false;
}

MG_CAPI_INLINE bool mmconc_msvc_atmc_i16_compare_exchange_weak_explicit(
    volatile mmconc_msvc_atmc_i16_t* _obj, int16_t* _expected, int16_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    return mmconc_msvc_atmc_i16_compare_exchange_strong_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE bool mmconc_msvc_atmc_u16_compare_exchange_strong_explicit(
    volatile mmconc_msvc_atmc_u16_t* _obj, uint16_t* _expected, uint16_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    int16_t expected_value = *((int16_t*)_expected);
    bool ret = mmconc_msvc_atmc_i16_compare_exchange_strong_explicit(
        (volatile mmconc_msvc_atmc_i16_t*)_obj, &expected_value, *((int16_t*)&_desired), _success, _failure);
    if (!ret)
        memcpy(_expected, &expected_value, sizeof(uint16_t));
    return ret;
}

MG_CAPI_INLINE bool mmconc_msvc_atmc_u16_compare_exchange_weak_explicit(
    volatile mmconc_msvc_atmc_u16_t* _obj, uint16_t* _expected, uint16_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    return mmconc_msvc_atmc_u16_compare_exchange_strong_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE bool mmconc_msvc_atmc_i32_compare_exchange_strong_explicit(
    volatile mmconc_msvc_atmc_i32_t* _obj, int32_t* _expected, int32_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    int32_t expected_value = *_expected;
    int32_t ret;

    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_success, ret, _InterlockedCompareExchange, (volatile long*)_obj, _desired, expected_value);
    if (ret == expected_value) {
        return true;
    }

    memcpy(_expected, &ret, sizeof(int32_t));
    return false;
}

MG_CAPI_INLINE bool mmconc_msvc_atmc_i32_compare_exchange_weak_explicit(
    volatile mmconc_msvc_atmc_i32_t* _obj, int32_t* _expected, int32_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    return mmconc_msvc_atmc_i32_compare_exchange_strong_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE bool mmconc_msvc_atmc_u32_compare_exchange_strong_explicit(
    volatile mmconc_msvc_atmc_u32_t* _obj, uint32_t* _expected, uint32_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    int32_t expected_value = *((int32_t*)_expected);
    bool ret = mmconc_msvc_atmc_i32_compare_exchange_strong_explicit(
        (volatile mmconc_msvc_atmc_i32_t*)_obj, &expected_value, *((int32_t*)&_desired), _success, _failure);
    if (!ret)
        memcpy(_expected, &expected_value, sizeof(uint32_t));
    return ret;
}

MG_CAPI_INLINE bool mmconc_msvc_atmc_u32_compare_exchange_weak_explicit(
    volatile mmconc_msvc_atmc_u32_t* _obj, uint32_t* _expected, uint32_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    return mmconc_msvc_atmc_u32_compare_exchange_strong_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE bool mmconc_msvc_atmc_i64_compare_exchange_strong_explicit(
    volatile mmconc_msvc_atmc_i64_t* _obj, int64_t* _expected, int64_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    int64_t expected_value = *_expected;
    int64_t ret;

    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_success, ret, _InterlockedCompareExchange64, _obj, _desired, expected_value);
    if (ret == expected_value) {
        return true;
    }

    memcpy(_expected, &ret, sizeof(int64_t));
    return false;
}

MG_CAPI_INLINE bool mmconc_msvc_atmc_i64_compare_exchange_weak_explicit(
    volatile mmconc_msvc_atmc_i64_t* _obj, int64_t* _expected, int64_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    return mmconc_msvc_atmc_i64_compare_exchange_strong_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE bool mmconc_msvc_atmc_u64_compare_exchange_strong_explicit(
    volatile mmconc_msvc_atmc_u64_t* _obj, uint64_t* _expected, uint64_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    int64_t expected_value = *((int64_t*)_expected);
    bool ret = mmconc_msvc_atmc_i64_compare_exchange_strong_explicit(
        (volatile mmconc_msvc_atmc_i64_t*)_obj, &expected_value, *((int64_t*)&_desired), _success, _failure);
    if (!ret)
        memcpy(_expected, &expected_value, sizeof(uint64_t));
    return ret;
}

MG_CAPI_INLINE bool mmconc_msvc_atmc_u64_compare_exchange_weak_explicit(
    volatile mmconc_msvc_atmc_u64_t* _obj, uint64_t* _expected, uint64_t _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    return mmconc_msvc_atmc_u64_compare_exchange_strong_explicit(_obj, _expected, _desired, _success, _failure);
}

MG_CAPI_INLINE bool mmconc_msvc_atmc_bl_compare_exchange_strong_explicit(
    volatile mmconc_msvc_atmc_bl_t* _obj, bool* _expected, bool _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    return mmconc_msvc_atmc_i8_compare_exchange_strong_explicit(
        (volatile mmconc_msvc_atmc_i8_t*)_obj, (int8_t*)_expected, (int8_t)_desired, _success, _failure);
}

MG_CAPI_INLINE bool mmconc_msvc_atmc_bl_compare_exchange_weak_explicit(
    volatile mmconc_msvc_atmc_bl_t* _obj, bool* _expected, bool _desired, mmconc_memory_order_e _success, mmconc_memory_order_e _failure)
{
    return mmconc_msvc_atmc_i8_compare_exchange_weak_explicit(
        (volatile mmconc_msvc_atmc_i8_t*)_obj, (int8_t*)_expected, (int8_t)_desired, _success, _failure);
}



MG_CAPI_INLINE int8_t mmconc_msvc_atmc_i8_fetch_add_explicit(
    volatile mmconc_msvc_atmc_i8_t* _obj, int8_t _arg, mmconc_memory_order_e _order)
{
    int8_t ret;
    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_order, ret, _InterlockedExchangeAdd8, (volatile char*)_obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint8_t mmconc_msvc_atmc_u8_fetch_add_explicit(
    volatile mmconc_msvc_atmc_u8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order)
{
    int8_t ret = mmconc_msvc_atmc_i8_fetch_add_explicit(
        (volatile mmconc_msvc_atmc_i8_t*)_obj, *((int8_t*)&_arg), _order);
    return *((uint8_t*)&ret);
}

MG_CAPI_INLINE int16_t mmconc_msvc_atmc_i16_fetch_add_explicit(
    volatile mmconc_msvc_atmc_i16_t* _obj, int16_t _arg, mmconc_memory_order_e _order)
{
    int16_t ret;
    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_order, ret, _InterlockedExchangeAdd16, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint16_t mmconc_msvc_atmc_u16_fetch_add_explicit(
    volatile mmconc_msvc_atmc_u16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order)
{
    int16_t ret = mmconc_msvc_atmc_i16_fetch_add_explicit(
        (volatile mmconc_msvc_atmc_i16_t*)_obj, *((int16_t*)&_arg), _order);
    return *((uint16_t*)&ret);
}

MG_CAPI_INLINE int32_t mmconc_msvc_atmc_i32_fetch_add_explicit(
    volatile mmconc_msvc_atmc_i32_t* _obj, int32_t _arg, mmconc_memory_order_e _order)
{
    int32_t ret;
    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_order, ret, _InterlockedExchangeAdd, (volatile long*)_obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint32_t mmconc_msvc_atmc_u32_fetch_add_explicit(
    volatile mmconc_msvc_atmc_u32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order)
{
    int32_t ret = mmconc_msvc_atmc_i32_fetch_add_explicit(
        (volatile mmconc_msvc_atmc_i32_t*)_obj, *((int32_t*)&_arg), _order);
    return *((uint32_t*)&ret);
}

MG_CAPI_INLINE int64_t mmconc_msvc_atmc_i64_fetch_add_explicit(
    volatile mmconc_msvc_atmc_i64_t* _obj, int64_t _arg, mmconc_memory_order_e _order)
{
    int64_t ret;
    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_order, ret, _InterlockedExchangeAdd64, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint64_t mmconc_msvc_atmc_u64_fetch_add_explicit(
    volatile mmconc_msvc_atmc_u64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order)
{
    int64_t ret = mmconc_msvc_atmc_i64_fetch_add_explicit(
        (volatile mmconc_msvc_atmc_i64_t*)_obj, *((int64_t*)&_arg), _order);
    return *((uint64_t*) & ret);
}


MG_CAPI_INLINE int8_t mmconc_msvc_atmc_i8_fetch_sub_explicit(
    volatile mmconc_msvc_atmc_i8_t* _obj, int8_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i8_fetch_add_explicit(_obj, -_arg, _order);
}

MG_CAPI_INLINE uint8_t mmconc_msvc_atmc_u8_fetch_sub_explicit(
    volatile mmconc_msvc_atmc_u8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i8_fetch_add_explicit(
        (volatile mmconc_msvc_atmc_i8_t*)_obj, -(*((int8_t*)&_arg)), _order);
}

MG_CAPI_INLINE int16_t mmconc_msvc_atmc_i16_fetch_sub_explicit(
    volatile mmconc_msvc_atmc_i16_t* _obj, int16_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i16_fetch_add_explicit(_obj, -_arg, _order);
}

MG_CAPI_INLINE uint16_t mmconc_msvc_atmc_u16_fetch_sub_explicit(
    volatile mmconc_msvc_atmc_u16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i16_fetch_add_explicit(
        (volatile mmconc_msvc_atmc_i16_t*)_obj, -(*((int16_t*)&_arg)), _order);
}

MG_CAPI_INLINE int32_t mmconc_msvc_atmc_i32_fetch_sub_explicit(
    volatile mmconc_msvc_atmc_i32_t* _obj, int32_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i32_fetch_add_explicit(_obj, -_arg, _order);
}

MG_CAPI_INLINE uint32_t mmconc_msvc_atmc_u32_fetch_sub_explicit(
    volatile mmconc_msvc_atmc_u32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i32_fetch_add_explicit(
        (volatile mmconc_msvc_atmc_i32_t*)_obj, -(*((int32_t*)&_arg)), _order);
}

MG_CAPI_INLINE int64_t mmconc_msvc_atmc_i64_fetch_sub_explicit(
    volatile mmconc_msvc_atmc_i64_t* _obj, int64_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i64_fetch_add_explicit(_obj, -_arg, _order);
}

MG_CAPI_INLINE uint64_t mmconc_msvc_atmc_u64_fetch_sub_explicit(
    volatile mmconc_msvc_atmc_u64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order)
{
    return mmconc_msvc_atmc_i64_fetch_add_explicit(
        (volatile mmconc_msvc_atmc_i64_t*)_obj, -(*((int64_t*)&_arg)), _order);
}


MG_CAPI_INLINE int8_t mmconc_msvc_atmc_i8_fetch_and_explicit(
    volatile mmconc_msvc_atmc_i8_t* _obj, int8_t _arg, mmconc_memory_order_e _order)
{
    int8_t ret;
    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_order, ret, _InterlockedAnd8, (volatile char*)_obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint8_t mmconc_msvc_atmc_u8_fetch_and_explicit(
    volatile mmconc_msvc_atmc_u8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order)
{
    int8_t ret = mmconc_msvc_atmc_i8_fetch_and_explicit(
        (volatile mmconc_msvc_atmc_i8_t*)_obj, *((int8_t*)&_arg), _order);
    return *((uint8_t*)&ret);
}

MG_CAPI_INLINE int16_t mmconc_msvc_atmc_i16_fetch_and_explicit(
    volatile mmconc_msvc_atmc_i16_t* _obj, int16_t _arg, mmconc_memory_order_e _order)
{
    int16_t ret;
    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_order, ret, _InterlockedAnd16, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint16_t mmconc_msvc_atmc_u16_fetch_and_explicit(
    volatile mmconc_msvc_atmc_u16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order)
{
    int16_t ret = mmconc_msvc_atmc_i16_fetch_and_explicit(
        (volatile mmconc_msvc_atmc_i16_t*)_obj, *((int16_t*)&_arg), _order);
    return *((uint16_t*)&ret);
}

MG_CAPI_INLINE int32_t mmconc_msvc_atmc_i32_fetch_and_explicit(
    volatile mmconc_msvc_atmc_i32_t* _obj, int32_t _arg, mmconc_memory_order_e _order)
{
    int32_t ret;
    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_order, ret, _InterlockedAnd, (volatile long*)_obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint32_t mmconc_msvc_atmc_u32_fetch_and_explicit(
    volatile mmconc_msvc_atmc_u32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order)
{
    int32_t ret = mmconc_msvc_atmc_i32_fetch_and_explicit(
        (volatile mmconc_msvc_atmc_i32_t*)_obj, *((int32_t*)&_arg), _order);
    return *((uint32_t*)&ret);
}

MG_CAPI_INLINE int64_t mmconc_msvc_atmc_i64_fetch_and_explicit(
    volatile mmconc_msvc_atmc_i64_t* _obj, int64_t _arg, mmconc_memory_order_e _order)
{
    int64_t ret;
    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_order, ret, _InterlockedAnd64, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint64_t mmconc_msvc_atmc_u64_fetch_and_explicit(
    volatile mmconc_msvc_atmc_u64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order)
{
    int64_t ret = mmconc_msvc_atmc_i64_fetch_and_explicit(
        (volatile mmconc_msvc_atmc_i64_t*)_obj, *((int64_t*)&_arg), _order);
    return *((uint64_t*) & ret);
}


MG_CAPI_INLINE int8_t mmconc_msvc_atmc_i8_fetch_or_explicit(
    volatile mmconc_msvc_atmc_i8_t* _obj, int8_t _arg, mmconc_memory_order_e _order)
{
    int8_t ret;
    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_order, ret, _InterlockedOr8, (volatile char*)_obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint8_t mmconc_msvc_atmc_u8_fetch_or_explicit(
    volatile mmconc_msvc_atmc_u8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order)
{
    int8_t ret = mmconc_msvc_atmc_i8_fetch_or_explicit(
        (volatile mmconc_msvc_atmc_i8_t*)_obj, *((int8_t*)&_arg), _order);
    return *((uint8_t*)&ret);
}

MG_CAPI_INLINE int16_t mmconc_msvc_atmc_i16_fetch_or_explicit(
    volatile mmconc_msvc_atmc_i16_t* _obj, int16_t _arg, mmconc_memory_order_e _order)
{
    int16_t ret;
    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_order, ret, _InterlockedOr16, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint16_t mmconc_msvc_atmc_u16_fetch_or_explicit(
    volatile mmconc_msvc_atmc_u16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order)
{
    int16_t ret = mmconc_msvc_atmc_i16_fetch_or_explicit(
        (volatile mmconc_msvc_atmc_i16_t*)_obj, *((int16_t*)&_arg), _order);
    return *((uint16_t*)&ret);
}

MG_CAPI_INLINE int32_t mmconc_msvc_atmc_i32_fetch_or_explicit(
    volatile mmconc_msvc_atmc_i32_t* _obj, int32_t _arg, mmconc_memory_order_e _order)
{
    int32_t ret;
    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_order, ret, _InterlockedOr, (volatile long*)_obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint32_t mmconc_msvc_atmc_u32_fetch_or_explicit(
    volatile mmconc_msvc_atmc_u32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order)
{
    int32_t ret = mmconc_msvc_atmc_i32_fetch_or_explicit(
        (volatile mmconc_msvc_atmc_i32_t*)_obj, *((int32_t*)&_arg), _order);
    return *((uint32_t*)&ret);
}

MG_CAPI_INLINE int64_t mmconc_msvc_atmc_i64_fetch_or_explicit(
    volatile mmconc_msvc_atmc_i64_t* _obj, int64_t _arg, mmconc_memory_order_e _order)
{
    int64_t ret;
    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_order, ret, _InterlockedOr64, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint64_t mmconc_msvc_atmc_u64_fetch_or_explicit(
    volatile mmconc_msvc_atmc_u64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order)
{
    int64_t ret = mmconc_msvc_atmc_i64_fetch_or_explicit(
        (volatile mmconc_msvc_atmc_i64_t*)_obj, *((int64_t*)&_arg), _order);
    return *((uint64_t*) & ret);
}


MG_CAPI_INLINE int8_t mmconc_msvc_atmc_i8_fetch_xor_explicit(
    volatile mmconc_msvc_atmc_i8_t* _obj, int8_t _arg, mmconc_memory_order_e _order)
{
    int8_t ret;
    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_order, ret, _InterlockedXor8, (volatile char*)_obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint8_t mmconc_msvc_atmc_u8_fetch_xor_explicit(
    volatile mmconc_msvc_atmc_u8_t* _obj, uint8_t _arg, mmconc_memory_order_e _order)
{
    int8_t ret = mmconc_msvc_atmc_i8_fetch_xor_explicit(
        (volatile mmconc_msvc_atmc_i8_t*)_obj, *((int8_t*)&_arg), _order);
    return *((uint8_t*)&ret);
}

MG_CAPI_INLINE int16_t mmconc_msvc_atmc_i16_fetch_xor_explicit(
    volatile mmconc_msvc_atmc_i16_t* _obj, int16_t _arg, mmconc_memory_order_e _order)
{
    int16_t ret;
    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_order, ret, _InterlockedXor16, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint16_t mmconc_msvc_atmc_u16_fetch_xor_explicit(
    volatile mmconc_msvc_atmc_u16_t* _obj, uint16_t _arg, mmconc_memory_order_e _order)
{
    int16_t ret = mmconc_msvc_atmc_i16_fetch_xor_explicit(
        (volatile mmconc_msvc_atmc_i16_t*)_obj, *((int16_t*)&_arg), _order);
    return *((uint16_t*)&ret);
}

MG_CAPI_INLINE int32_t mmconc_msvc_atmc_i32_fetch_xor_explicit(
    volatile mmconc_msvc_atmc_i32_t* _obj, int32_t _arg, mmconc_memory_order_e _order)
{
    int32_t ret;
    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_order, ret, _InterlockedXor, (volatile long*)_obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint32_t mmconc_msvc_atmc_u32_fetch_xor_explicit(
    volatile mmconc_msvc_atmc_u32_t* _obj, uint32_t _arg, mmconc_memory_order_e _order)
{
    int32_t ret = mmconc_msvc_atmc_i32_fetch_xor_explicit(
        (volatile mmconc_msvc_atmc_i32_t*)_obj, *((int32_t*)&_arg), _order);
    return *((uint32_t*)&ret);
}

MG_CAPI_INLINE int64_t mmconc_msvc_atmc_i64_fetch_xor_explicit(
    volatile mmconc_msvc_atmc_i64_t* _obj, int64_t _arg, mmconc_memory_order_e _order)
{
    int64_t ret;
    __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC(_order, ret, _InterlockedXor64, _obj, _arg);
    return ret;
}

MG_CAPI_INLINE uint64_t mmconc_msvc_atmc_u64_fetch_xor_explicit(
    volatile mmconc_msvc_atmc_u64_t* _obj, uint64_t _arg, mmconc_memory_order_e _order)
{
    int64_t ret = mmconc_msvc_atmc_i64_fetch_xor_explicit(
        (volatile mmconc_msvc_atmc_i64_t*)_obj, *((int64_t*)&_arg), _order);
    return *((uint64_t*) & ret);
}


#undef __MMCONC_MSVC_ATMC_USE_ARM64_LDAR_STLR
#undef __MMCONC_MSVC_ATMC_CHOOSE_INTRINSIC
#undef __MMCONC_MSVC_ATMC_STORE_RELEASE
#undef __MMCONC_MSVC_ATMC_STORE_SWITCH_PREFIX
#undef __MMCONC_MSVC_ATMC_LOAD_IMPL
#undef __MMCONC_MSVC_ATMC_LOAD_ARM64_IMPL
#undef __MMCONC_MSVC_ATMC_LOAD_ACQUIRE_ARM64
#undef __MMCONC_MSVC_ATMC_POST_LOAD_BARRIER_AS_NEEDED

#undef __MMCONC_MSVC_ATMC_STORE_IMPL
#undef __MMCONC_MSVC_ATMC_STORE_SEQ_CST_ARM
#undef __MMCONC_MSVC_ATMC_STORE_SEQ_CST_ARM64
#undef __MMCONC_MSVC_ATMC_STORE_SEQ_CST_X86_X64
#undef __MMCONC_MSVC_ATMC_STORE_32_SEQ_CST_X86_X64
#undef __MMCONC_MSVC_ATMC_STORE_64_SEQ_CST_IX86

#undef __MMCONC_MSVC_ATMC_INVALID_MEMORY_ORDER
#undef __mmconc_msvc_atmc_compiler_or_memory_barrier
#undef __mmconc_msvc_atmc_memory_barrier
#undef __mmconc_msvc_atmc_compiler_barrier

#undef __MMCONC_MSVC_ATMC_INTRIN_RELAXED
#undef __MMCONC_MSVC_ATMC_INTRIN_ACQUIRE
#undef __MMCONC_MSVC_ATMC_INTRIN_RELEASE
#undef __MMCONC_MSVC_ATMC_INTRIN_ACQ_REL
#undef __MMCONC_MSVC_ATMC_YIELD_PROCESSOR

#endif // MG_COMP__MSVC_AVAIL

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_IMPL_CONC_MSVC_ATOMIC_H_INCLUDED
