
#ifndef MEGO_THRD_SPINWAIT_H_INCLUDED
#define MEGO_THRD_SPINWAIT_H_INCLUDED

#include <mego/predef/symbol/inline.h>
#include <mego/util/std/stddef.h>
#include <mego/thrd/threads.h>
#include <mego/hardware/cpu/count.h>

#ifndef MGTHRD_SPINWAIT_YIELD_THRESHOLD
#define MGTHRD_SPINWAIT_YIELD_THRESHOLD (10)
#endif

#ifndef MGTHRD_SPINWAIT_SLEEP_EVERY_HOW_MANY_TIMES_0
#define MGTHRD_SPINWAIT_SLEEP_EVERY_HOW_MANY_TIMES_0 (5)
#endif

#ifndef MGTHRD_SPINWAIT_SLEEP_EVERY_HOW_MANY_TIMES_1
#define MGTHRD_SPINWAIT_SLEEP_EVERY_HOW_MANY_TIMES_1 (20)
#endif

typedef struct mgthrd_spinwait {
    rsize_t count_;
} mgthrd_spinwait_t;

//! Reset the counter of a spin-wait structure.
//!
//! This function resets the counter of the spin-wait structure to 0, typically called at the start of a spin-wait operation.
//!
//! @param[in,out] _spinwait A pointer to the spin-wait structure that needs to be reset.
MG_CAPI_INLINE void mgthrd_spinwait_reset(mgthrd_spinwait_t* _spinwait)
{
    _spinwait->count_ = 0;
}

//! Perform one spin-wait operation, choosing the appropriate action based on the counter.
//!
//! This function is used for spin-waiting in a multithreaded environment. Based on the current counter value,
//! it chooses to yield the thread, sleep briefly, or sleep for a longer duration to avoid excessive CPU usage.
//!
//! @param[in,out] _spinwait A pointer to the spin-wait structure.
MG_CAPI_INLINE void mgthrd_spinwait_once(mgthrd_spinwait_t* _spinwait)
{    
    if (_spinwait->count_ > MGTHRD_SPINWAIT_YIELD_THRESHOLD
        || mghw_processor_count() <= 1)
    {
        size_t yields_so_far = _spinwait->count_ - MGTHRD_SPINWAIT_YIELD_THRESHOLD;
        if ((yields_so_far % MGTHRD_SPINWAIT_SLEEP_EVERY_HOW_MANY_TIMES_1) == (MGTHRD_SPINWAIT_SLEEP_EVERY_HOW_MANY_TIMES_1 - 1)) 
        {
            mgu_timespec_t ts = { 0, 1000000 };
            mgthrd_sleep(&ts, NULL);
        } else if ((yields_so_far % MGTHRD_SPINWAIT_SLEEP_EVERY_HOW_MANY_TIMES_0) == (MGTHRD_SPINWAIT_SLEEP_EVERY_HOW_MANY_TIMES_0 - 1)) 
        {
            mgu_timespec_t ts = { 0, 0 };
            mgthrd_sleep(&ts, NULL);
        } else {
            mguthrd_yield();
        }
    }

    if (_spinwait->count_ > RSIZE_MAX) {
        _spinwait->count_ = MGTHRD_SPINWAIT_YIELD_THRESHOLD;
    } else {
        ++_spinwait->count_;
    }
}

#endif // !MEGO_THRD_SPINWAIT_H_INCLUDED
