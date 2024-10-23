
#ifndef MEGOPP_THRD_SPIN_MUTEX_H_INCLUDED
#define MEGOPP_THRD_SPIN_MUTEX_H_INCLUDED

#include <mego/thrd/spinwait.h>
#include <mego/predef/os/linux.h>

#include <atomic>
#include <stdexcept>

#if MG_OS__LINUX_AVAIL
#  include <unistd.h>
#endif

namespace mgpp {
namespace thrd {

struct spin_mutex
{
    spin_mutex()
    {
    }

    ~spin_mutex()
    {
        unlock();
    }

    inline void lock()
    {
#if MG_OS__WIN_AVAIL
        auto tid = GetCurrentThreadId();
#else
        auto tid = gettid();
#endif
        int64_t expected = -1;

        mgthrd_spinwait_t spinwait;
        mgthrd_spinwait_reset(&spinwait);
        
        if (locked.load(std::memory_order_acquire) == tid)
            throw std::logic_error("deadlock detected");
        
        while (!locked.compare_exchange_weak(expected, tid, std::memory_order_acquire))
        {
            expected = -1;
            mgthrd_spinwait_once(&spinwait);
        }

        // while (locked.test_and_set(std::memory_order_acquire)) 
        // {
        //     mgthrd_spinwait_once(&spinwait);
        // }
    }

    inline bool try_lock()
    {
#if MG_OS__WIN_AVAIL
        auto tid = GetCurrentThreadId();
#else
        auto tid = gettid();
#endif
        int64_t expected = -1;

        if (locked.load(std::memory_order_acquire) == tid)
            throw std::logic_error("deadlock detected");
        
        return locked.compare_exchange_strong(expected, tid, std::memory_order_acquire);

        // return !locked.test_and_set(std::memory_order_acquire);
    }

    inline void unlock()
    {
        locked.store(-1, std::memory_order_release);

        // locked.clear(std::memory_order_release);
    }

private:
    std::atomic<int64_t> locked = -1;

    // std::atomic_flag locked = ATOMIC_FLAG_INIT;
};

}    
}

#endif // !MEGOPP_THRD_SPIN_MUTEX_H_INCLUDED
