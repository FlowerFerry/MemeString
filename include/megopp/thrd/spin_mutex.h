
#ifndef MEGOPP_THRD_SPIN_MUTEX_H_INCLUDED
#define MEGOPP_THRD_SPIN_MUTEX_H_INCLUDED

#include <mego/thrd/spinlock.h>

namespace mgpp {
namespace thrd {

struct spin_mutex : mgthrd_spinlock_t
{
    spin_mutex()
    {
        mgthrd_spinlock_init(this);
    }

    ~spin_mutex()
    {
        unlock();
    }

    inline void lock()
    {
        mgthrd_spinlock_lock(this);
    }

    inline bool try_lock()
    {
        return mgthrd_spinlock_trylock(this);
    }

    inline void unlock()
    {
        mgthrd_spinlock_unlock(this);
    }

};

}    
}

#endif // !MEGOPP_THRD_SPIN_MUTEX_H_INCLUDED
