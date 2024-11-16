
#ifndef MEME_THRD_SPINLOCK_H_INCLUDED
#define MEME_THRD_SPINLOCK_H_INCLUDED

#include <meme/conc/atomic.h>
#include <mego/thrd/numeric_id.h>
#include <mego/thrd/spinwait.h>
#include <mego/predef/symbol/inline.h>

typedef struct mmthrd_spinlock {
    mmconc_atomic_size_t locked;
} mmthrd_spinlock_t;

MG_CAPI_INLINE void mmthrd_spinlock_init(mmthrd_spinlock_t* _lock)
{
    mmconc_atomic_size_init(&_lock->locked, SIZE_MAX);
}

MG_CAPI_INLINE bool mmthrd_spinlock_lock(mmthrd_spinlock_t*_lock)
{
    size_t tid = mgthrd_numeric_id();
    size_t expected = SIZE_MAX;
    mgthrd_spinwait_t spinwait;
    mgthrd_spinwait_reset(&spinwait);
    
    if (tid != 0 && 
        mmconc_atomic_size_load_explicit(&_lock->locked, mmconc_memory_order_acquire) == tid)
        return false;

    while (!mmconc_atomic_size_compare_exchange_weak_explicit(
        &_lock->locked, &expected, tid, 
        mmconc_memory_order_acquire, mmconc_memory_order_acquire))
    {
        expected = SIZE_MAX;
        mgthrd_spinwait_once(&spinwait);
    }
    return true;
}

MG_CAPI_INLINE bool mmthrd_spinlock_trylock(mmthrd_spinlock_t*_lock)
{
    size_t tid = mgthrd_numeric_id();
    size_t expected = SIZE_MAX;
    
    if (tid != 0 &&
        mmconc_atomic_size_load_explicit(&_lock->locked, mmconc_memory_order_acquire) == tid)
        return false;

    return mmconc_atomic_size_compare_exchange_strong_explicit(
        &_lock->locked, &expected, tid, 
        mmconc_memory_order_acquire, mmconc_memory_order_acquire);
}

MG_CAPI_INLINE void mmthrd_spinlock_unlock(mmthrd_spinlock_t*_lock)
{
    mmconc_atomic_size_store_explicit(&_lock->locked, SIZE_MAX, mmconc_memory_order_release);
}

#endif // !MEME_THRD_SPINLOCK_H_INCLUDED
