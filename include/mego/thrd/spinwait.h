
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

//! 重置自旋等待结构体的计数器。
//!
//! 该函数用于将自旋等待结构体的计数器重置为0，通常在自旋等待开始时调用。
//! 
//! @param[in,out] _spinwait 指向需要重置的自旋等待结构体。
MG_CAPI_INLINE void mgthrd_spinwait_reset(mgthrd_spinwait_t* _spinwait)
{
    _spinwait->count_ = 0;
}

//! 自旋等待一次，根据计数器选择合适的操作。
//!
//! 该函数用于在多线程环境中进行自旋等待。根据当前计数器的值，
//! 它会选择进行线程让步、短暂休眠或长时间休眠，以避免过度占用CPU。
//!
//! @param[in,out] _spinwait 指向自旋等待结构体。
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
