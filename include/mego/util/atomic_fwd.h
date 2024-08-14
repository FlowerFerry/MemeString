
#ifndef MEGO_UTIL_ATOMIC_FWD_H_INCLUDED
#define MEGO_UTIL_ATOMIC_FWD_H_INCLUDED

#include <mego/predef/compiler/visualc.h>
#include <mego/util/os/windows/windows_simplify.h>

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#if MG_COMP__MSVC_AVAIL

#define MGU_ATOMIC_BOOL_LOCK_FREE     2
#define MGU_ATOMIC_CHAR_LOCK_FREE     2
#define MGU_ATOMIC_SHORT_LOCK_FREE    2
#define MGU_ATOMIC_INT_LOCK_FREE      2
#define MGU_ATOMIC_LONG_LOCK_FREE     2
#define MGU_ATOMIC_LLONG_LOCK_FREE    2
#define MGU_ATOMIC_POINTER_LOCK_FREE  2

typedef enum mgu__memory_order {
    mgu_memory_order_relaxed = 0,
    mgu_memory_order_consume = 1,
    mgu_memory_order_acquire = 2,
    mgu_memory_order_release = 3,
    mgu_memory_order_acq_rel = 4,
    mgu_memory_order_seq_cst = 5
} mgu_memory_order;

typedef bool               mgu_atomic_bool;
typedef char               mgu_atomic_char;
typedef unsigned char      mgu_atomic_uchar;
typedef short              mgu_atomic_short;
typedef unsigned short     mgu_atomic_ushort;
typedef long               mgu_atomic_long;
typedef unsigned long      mgu_atomic_ulong;
typedef long long          mgu_atomic_llong;
typedef unsigned long long mgu_atomic_ullong;
typedef intptr_t           mgu_atomic_intptr_t;
typedef uintptr_t          mgu_atomic_uintptr_t;
typedef size_t             mgu_atomic_size_t;
typedef ptrdiff_t          mgu_atomic_ptrdiff_t;
typedef intmax_t           mgu_atomic_intmax_t;
typedef uintmax_t          mgu_atomic_uintmax_t;

typedef struct mgu_atomic_flag { mgu_atomic_bool value_; } mgu_atomic_flag;

#define MGU_ATOMIC_FLAG_INIT { false }

#elif defined(__STDC_NO_ATOMICS__) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#  include <stdatomic.h>

#define MGU_ATOMIC_BOOL_LOCK_FREE     ATOMIC_BOOL_LOCK_FREE
#define MGU_ATOMIC_CHAR_LOCK_FREE     ATOMIC_CHAR_LOCK_FREE
#define MGU_ATOMIC_SHORT_LOCK_FREE    ATOMIC_SHORT_LOCK_FREE
#define MGU_ATOMIC_INT_LOCK_FREE      ATOMIC_INT_LOCK_FREE
#define MGU_ATOMIC_LONG_LOCK_FREE     ATOMIC_LONG_LOCK_FREE
#define MGU_ATOMIC_LLONG_LOCK_FREE    ATOMIC_LLONG_LOCK_FREE
#define MGU_ATOMIC_POINTER_LOCK_FREE  ATOMIC_POINTER_LOCK_FREE

typedef enum mgu_memory_order {
    mgu_memory_order_relaxed = memory_order_relaxed,
    mgu_memory_order_consume = memory_order_consume,
    mgu_memory_order_acquire = memory_order_acquire,
    mgu_memory_order_release = memory_order_release,
    mgu_memory_order_acq_rel = memory_order_acq_rel,
    mgu_memory_order_seq_cst = memory_order_seq_cst
} mgu_memory_order;

#define MGU_ATOMIC_FLAG_INIT ATOMIC_FLAG_INIT

typedef atomic_bool         mgu_atomic_bool;
typedef atomic_char         mgu_atomic_char;
typedef atomic_schar        mgu_atomic_schar;
typedef atomic_short        mgu_atomic_short;
typedef atomic_ushort       mgu_atomic_ushort;
typedef atomic_long         mgu_atomic_long;
typedef atomic_ulong        mgu_atomic_ulong;
typedef atomic_llong        mgu_atomic_llong;
typedef atomic_ullong       mgu_atomic_ullong;
typedef atomic_intptr_t     mgu_atomic_intptr_t;
typedef atomic_uintptr_t    mgu_atomic_uintptr_t;
typedef atomic_size_t       mgu_atomic_size_t;
typedef atomic_ptrdiff_t    mgu_atomic_ptrdiff_t;
typedef atomic_intmax_t     mgu_atomic_intmax_t;
typedef atomic_uintmax_t    mgu_atomic_uintmax_t;

typedef atomic_flag         mgu_atomic_flag;

#else
    
#  error "No atomic support"

#endif

#ifdef __cplusplus
}
#endif

#endif // !MEGO_UTIL_ATOMIC_FWD_H_INCLUDED
