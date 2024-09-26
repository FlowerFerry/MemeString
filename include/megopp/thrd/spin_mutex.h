
#ifndef MEGOPP_THRD_SPIN_MUTEX_H_INCLUDED
#define MEGOPP_THRD_SPIN_MUTEX_H_INCLUDED

#include <mego/thrd/spinwait.h>

#include <atomic>

namespace mgpp {
namespace thrd {

struct spin_mutex
{
    spin_mutex()
        : locked(ATOMIC_FLAG_INIT)
    {
    }

    ~spin_mutex()
    {
        unlock();
    }

    inline void lock()
    {
        mgthrd_spinwait_t spinwait;
        mgthrd_spinwait_reset(&spinwait);
        while (locked.test_and_set(std::memory_order_acquire)) 
        {
            mgthrd_spinwait_once(&spinwait);
        }
    }

    inline bool try_lock()
    {
        return !locked.test_and_set(std::memory_order_acquire);
    }

    inline void unlock()
    {
        locked.clear(std::memory_order_release);
    }

private:
    std::atomic_flag locked;
};

}    
}

#endif // !MEGOPP_THRD_SPIN_MUTEX_H_INCLUDED
