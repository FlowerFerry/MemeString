
#ifndef MEGOPP_THRD_SPIN_MUTEX_H_INCLUDED
#define MEGOPP_THRD_SPIN_MUTEX_H_INCLUDED

#include <mego/thrd/spinwait.h>
#include <mego/predef/os/linux.h>

#include <megopp/thrd/numeric_id.h>

#include <atomic>
#include <stdexcept>

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
        auto tid = numeric_id();
        // size_t expected = SIZE_MAX;

        mgthrd_spinwait_t spinwait;
        mgthrd_spinwait_reset(&spinwait);
        
        if (tid != 0 && locked.load(std::memory_order_acquire) == tid)
            throw std::logic_error("deadlock detected");
        
        // while (!locked.compare_exchange_weak(
        //     expected, tid, std::memory_order_acquire))
        // {
        //     expected = SIZE_MAX;
        //     mgthrd_spinwait_once(&spinwait);
        // }

        while (true) {
            while (locked.load(std::memory_order_acquire) != SIZE_MAX)
                mgthrd_spinwait_once(&spinwait);
            
            size_t expected = SIZE_MAX;
            if (locked.compare_exchange_weak(expected, tid, std::memory_order_acquire))
                break;
        }
    }

    inline bool try_lock()
    {
        auto tid = numeric_id();
        size_t expected = SIZE_MAX;

        if (tid != 0 && locked.load(std::memory_order_acquire) == tid)
            throw std::logic_error("deadlock detected");
        
        return locked.compare_exchange_strong(
            expected, tid, std::memory_order_acquire);

        // return !locked.test_and_set(std::memory_order_acquire);
    }

    inline void unlock()
    {
        auto tid = numeric_id();
        size_t current = locked.load(std::memory_order_acquire);
        if (tid != 0 && current != tid && current != SIZE_MAX)
            throw std::logic_error("unlocking an unowned mutex");

        locked.store(SIZE_MAX, std::memory_order_release);

        // locked.clear(std::memory_order_release);
    }

private:
    std::atomic<size_t> locked = SIZE_MAX;

    // std::atomic_flag locked = ATOMIC_FLAG_INIT;
};

}    
}

#endif // !MEGOPP_THRD_SPIN_MUTEX_H_INCLUDED
