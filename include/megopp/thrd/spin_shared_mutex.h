
#ifndef MEGOPP_THRD_SPIN_SHARED_MUTEX_H_INCLUDED
#define MEGOPP_THRD_SPIN_SHARED_MUTEX_H_INCLUDED

#include <mego/thrd/spinwait.h>
#include <atomic>

namespace mgpp {
namespace thrd {

struct spin_shared_mutex
{
    spin_shared_mutex()
        : state(0)
    {
    }

    // 共享锁（读锁）
    void lock_shared() {
        int expected;
        do {
            mgthrd_spinwait_t spinwait;
            mgthrd_spinwait_reset(&spinwait);
            // 等待写者释放锁
            expected = state.load(std::memory_order_acquire);
            while (expected < 0) {
                expected = state.load(std::memory_order_acquire);
                mgthrd_spinwait_once(&spinwait);
            }
        } while (!state.compare_exchange_weak(
            expected, expected + 1, std::memory_order_acq_rel, std::memory_order_acquire));
    }

    void unlock_shared() {
        state.fetch_sub(1, std::memory_order_release);
    }

    // 独占锁（写锁）
    void lock() {
        int expected = 0;
        // 等待所有读者和写者释放锁
        mgthrd_spinwait_t spinwait;
        mgthrd_spinwait_reset(&spinwait);
        while (!state.compare_exchange_weak(
            expected, -1, std::memory_order_acq_rel, std::memory_order_acquire)) 
        {
            expected = 0;
            mgthrd_spinwait_once(&spinwait);
        }
    }

    void unlock() {
        state.store(0, std::memory_order_release);
    }

private:
    std::atomic<int> state; // 正数表示读者数量，-1表示写者持有锁
};

}
}

#endif // !MEGOPP_THRD_SPIN_SHARED_MUTEX_H_INCLUDED
