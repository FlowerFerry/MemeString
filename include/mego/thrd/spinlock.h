
#ifndef MEGO_THRD_SPINLOCK_H_INCLUDED
#define MEGO_THRD_SPINLOCK_H_INCLUDED

#include <mego/util/atomic.h>
#include <mego/thrd/threads.h>
#include <mego/thrd/spinwait.h>
#include <mego/predef/symbol/inline.h>

typedef struct mgthrd_spinlock {
    mgu_atomic_flag locked;
} mgthrd_spinlock_t;

MG_CAPI_INLINE void mgthrd_spinlock_init(mgthrd_spinlock_t* _lock)
{
    mgu_atomic_flag_clear(&_lock->locked);
}

MG_CAPI_INLINE void mgthrd_spinlock_lock(mgthrd_spinlock_t*_lock)
{
    mgthrd_spinwait_t spinwait;
    mgthrd_spinwait_reset(&spinwait);
    while (mgu_atomic_flag_test_and_set(&_lock->locked)) 
    {
        mgthrd_spinwait_once(&spinwait);
    }
}

MG_CAPI_INLINE bool mgthrd_spinlock_trylock(mgthrd_spinlock_t*_lock)
{
    return !mgu_atomic_flag_test_and_set(&_lock->locked);
}

MG_CAPI_INLINE void mgthrd_spinlock_unlock(mgthrd_spinlock_t*_lock)
{
    mgu_atomic_flag_clear(&_lock->locked);
}

#endif // !MEGO_THRD_SPINLOCK_H_INCLUDED
