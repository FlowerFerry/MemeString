
#ifndef MEGOPP_UTIL_SCOPE_LOCKER_H_INCLUDED
#define MEGOPP_UTIL_SCOPE_LOCKER_H_INCLUDED

#include <mutex>
#include <shared_mutex>

namespace mgpp {
namespace util {

template <typename _Mtx>
class scope_unique_locker {
public:
    explicit scope_unique_locker(std::unique_lock<_Mtx>& _locker) 
        : locker_(_locker), src_owns_lock_(_locker.owns_lock())
    {
        if (!_locker.owns_lock()) 
            _locker.lock();
        
    }

    explicit scope_unique_locker(std::unique_lock<_Mtx>& _locker, std::defer_lock_t) 
        : locker_(_locker), src_owns_lock_(_locker.owns_lock())
    {
        if (_locker.owns_lock()) {
            _locker.unlock();
        }
    }

    ~scope_unique_locker() {
        if (src_owns_lock_ && !locker_.owns_lock()) 
        {
            locker_.lock();
        }
        else if (!src_owns_lock_ && locker_.owns_lock()) 
        {
            locker_.unlock();
        }
    }

    void lock() {
        if (!locker_.owns_lock()) {
            locker_.unlock();
        }
    }

    void unlock() {
        if (locker_.owns_lock()) {
            locker_.unlock();
        }
    }

private:
    std::unique_lock<_Mtx>& locker_;
    bool src_owns_lock_;
};

template <typename _Mtx>
class scope_shared_locker {
public:
    explicit scope_shared_locker(std::shared_lock<_Mtx>& _locker) 
        : locker_(_locker), src_owns_lock_(_locker.owns_lock())
    {
        if (!_locker.owns_lock()) 
            _locker.lock();
        
    }

    explicit scope_shared_locker(std::shared_lock<_Mtx>& _locker, std::defer_lock_t) 
        : locker_(_locker), src_owns_lock_(_locker.owns_lock())
    {
        if (_locker.owns_lock()) {
            _locker.unlock();
        }
    }

    ~scope_shared_locker() {
        if (src_owns_lock_ && !locker_.owns_lock()) 
        {
            locker_.lock();
        }
        else if (!src_owns_lock_ && locker_.owns_lock()) 
        {
            locker_.unlock();
        }
    }

    void lock() {
        if (!locker_.owns_lock()) {
            locker_.unlock();
        }
    }

    void unlock() {
        if (locker_.owns_lock()) {
            locker_.unlock();
        }
    }
};

}
}

#endif // !MEGOPP_UTIL_SCOPE_LOCKER_H_INCLUDED
