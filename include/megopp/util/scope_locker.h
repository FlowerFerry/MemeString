
#ifndef MEGOPP_UTIL_SCOPE_LOCKER_H_INCLUDED
#define MEGOPP_UTIL_SCOPE_LOCKER_H_INCLUDED

#include <mutex>
#include <shared_mutex>

namespace mgpp {
namespace util {

template <typename _Mtx>
class scope_unique_locker {
public:
    using mutex_type = _Mtx;

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
            locker_.lock();
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
    using mutex_type = _Mtx;

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
            locker_.lock();
        }
    }

    void unlock() {
        if (locker_.owns_lock()) {
            locker_.unlock();
        }
    }

private:
    std::shared_lock<_Mtx>& locker_;
    bool src_owns_lock_;
};

template <typename _Mtx>
class rw_lock {

public:
    using mutex_type = _Mtx;

    explicit rw_lock(_Mtx& _mtx) 
        : mtx_(_mtx), owns_lock_(false), is_shared_(false)
    {
        mtx_.lock();
        owns_lock_ = true;
    }

    explicit rw_lock(_Mtx& _mtx, std::defer_lock_t) 
        : mtx_(_mtx), owns_lock_(false), is_shared_(false)
    {
    }

    explicit rw_lock(_Mtx& _mtx, std::try_to_lock_t) 
        : mtx_(_mtx), owns_lock_(mtx_.try_lock()), is_shared_(false)
    {
    }

    explicit rw_lock(_Mtx& _mtx, std::adopt_lock_t) 
        : mtx_(_mtx), owns_lock_(true), is_shared_(false)
    {
    }

    ~rw_lock() {
        if (owns_lock_) {
            if (is_shared_) {
                mtx_.unlock_shared();
            }
            else {
                mtx_.unlock();
            }
        }
    }

    void lock() {
        if (!owns_lock_) {
            mtx_.lock();
            owns_lock_ = true;
        }
    }

    void unlock() {
        if (owns_lock_) {
            if (is_shared_) {
                mtx_.unlock_shared();
                is_shared_ = false;
            }
            else {
                mtx_.unlock();
            }
            owns_lock_ = false;
        }
    }

    bool try_lock() {
        if (!owns_lock_) {
            owns_lock_ = mtx_.try_lock();
        }
        return owns_lock_;
    }

    void lock_shared() {
        if (!owns_lock_) {
            mtx_.lock_shared();
            is_shared_ = true;
            owns_lock_ = true;
        }
    }

    bool try_lock_shared() {
        if (!owns_lock_) {
            owns_lock_ = mtx_.try_lock_shared();
            if (owns_lock_) {
                is_shared_ = true;
            }
        }
        return owns_lock_;
    }

private:
    _Mtx& mtx_;
    bool owns_lock_;
    bool is_shared_;
};

template <typename _Locker>
class scope_locker {

public:
    using lock_type = _Locker;

    explicit scope_locker(_Locker& _locker) 
        : locker_(_locker), src_owns_lock_(_locker.owns_lock())
    {
        if (!_locker.owns_lock()) 
            _locker.lock();
        
    }

    explicit scope_locker(_Locker& _locker, std::defer_lock_t) 
        : locker_(_locker), src_owns_lock_(_locker.owns_lock())
    {
        if (_locker.owns_lock()) {
            _locker.unlock();
        }
    }

    ~scope_locker() {
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
            locker_.lock();
        }
    }

    void unlock() {
        if (locker_.owns_lock()) {
            locker_.unlock();
        }
    }

private:
    _Locker& locker_;
    bool src_owns_lock_;
};

}
}

#endif // !MEGOPP_UTIL_SCOPE_LOCKER_H_INCLUDED
