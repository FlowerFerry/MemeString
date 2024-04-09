
#ifndef MEGOPP_MEMORY_COW_PTR_H_INCLUDED
#define MEGOPP_MEMORY_COW_PTR_H_INCLUDED

#include <megopp/help/null_mutex.h>
#include <megopp/util/scope_locker.h>

#include <memory>
#include <mutex>
#include <shared_mutex>

namespace mgpp {
namespace mem {

template <
    typename _Ty, 
    typename _WriteMutex = std::mutex, 
    typename _GenrcMutex = std::shared_mutex>
class cow_ptr 
{
public:

    cow_ptr() noexcept
        : ptr_()
    {    
    }

    cow_ptr(std::nullptr_t) noexcept
        : ptr_(nullptr)
    {}

    template <typename _Derived>
    cow_ptr(_Derived* _ptr)
        : ptr_(_ptr)
    {}

    template <typename _Derived, typename _Deleter>
    cow_ptr(_Derived* _ptr, _Deleter _deleter)
        : ptr_(_ptr, _deleter)
    {}

    template<typename _Deleter>
    cow_ptr(std::nullptr_t, _Deleter _deleter)
        : ptr_(nullptr, _deleter)
    {}

    template<typename _Derived, typename _Deleter, typename _Alloc>
    cow_ptr(_Derived* _ptr, _Deleter _deleter, _Alloc _alloc)
        : ptr_(_ptr, _deleter, _alloc)
    {}

    template<typename _Deleter, typename _Alloc>
    cow_ptr(std::nullptr_t, _Deleter _deleter, _Alloc _alloc)
        : ptr_(nullptr, _deleter, _alloc)
    {}

    cow_ptr(const cow_ptr& _other)
        : ptr_(_other.read())
    {}

    cow_ptr(cow_ptr&& _other)
        : ptr_(_other.release())
    {}

    template<typename _Derived>
    cow_ptr(const std::shared_ptr<_Derived>& _ptr) noexcept
        : ptr_(_ptr)
    {}

    template<typename _Derived>
    cow_ptr(std::shared_ptr<_Derived>&& _ptr) noexcept
        : ptr_(std::move(_ptr))
    {}

    template<typename _Derived>
    cow_ptr(const std::weak_ptr<_Derived>& _ptr)
        : ptr_(_ptr)
    {}

    template<typename _Derived, typename _Deleter>
    cow_ptr(std::unique_ptr<_Derived, _Deleter>&& _ptr)
        : ptr_(std::move(_ptr))
    {}

    inline cow_ptr& operator=(const cow_ptr& _other)
    {
        if (this != &_other) {
            std::unique_lock<_GenrcMutex> locker(genrc_mutex_);
            ptr_ = _other.read();
        }
        return *this;
    }

    inline cow_ptr& operator=(cow_ptr&& _other)
    {
        if (this != &_other) {
            std::unique_lock<_GenrcMutex> locker(genrc_mutex_);
            ptr_ = _other.release();
        }
        return *this;
    }

    template<typename _Derived>
    inline cow_ptr& operator=(const std::shared_ptr<_Derived>& _ptr)
    {
        std::unique_lock<_GenrcMutex> locker(genrc_mutex_);
        ptr_ = _ptr;
        return *this;
    }

    template<typename _Derived>
    inline cow_ptr& operator=(std::shared_ptr<_Derived>&& _ptr)
    {
        std::unique_lock<_GenrcMutex> locker(genrc_mutex_);
        ptr_ = std::move(_ptr);
        return *this;
    }

    template<typename _Derived, typename _Deleter>
    inline cow_ptr& operator=(std::unique_ptr<_Derived, _Deleter>&& _ptr)
    {
        std::unique_lock<_GenrcMutex> locker(genrc_mutex_);
        ptr_ = std::move(_ptr);
        return *this;
    }

    inline std::shared_ptr<const _Ty> read() const
    {
        std::shared_lock<_GenrcMutex> locker(genrc_mutex_);
        return ptr_;
    }

    template<typename _Mutex, typename _Fn, 
        typename = std::enable_if_t<std::is_invocable_v<_Fn, std::shared_ptr<_Ty>&>>>
    inline std::shared_ptr<const _Ty> write(std::unique_lock<_Mutex>& _locker, _Fn&& _fn)
    {
        util::scope_unique_locker write_locker(_locker);
        auto old_ptr = read();
        auto new_ptr = std::make_shared<_Ty>(*old_ptr);

        if constexpr (std::is_same_v<std::invoke_result_t<_Fn, std::shared_ptr<_Ty>&>, bool>)
        {
            if (!_fn(new_ptr))
                return old_ptr;
        }
        else {
            _fn(new_ptr);
        }

        std::unique_lock<_GenrcMutex> genrc_locker(genrc_mutex_);
        ptr_ = new_ptr;
        genrc_locker.unlock();
        write_locker.unlock();
        return new_ptr;
    }

    template<typename _Fn, 
        typename = std::enable_if_t<std::is_invocable_v<_Fn, std::shared_ptr<_Ty>&>>>
    inline std::shared_ptr<const _Ty> write(_Fn&& _fn)
    {
        std::uniue_lock<_WriteMutex> locker(write_mutex_, std::defer_lock);
        return write(locker, std::forward<_Fn>(_fn));
    }

    template<typename _Mutex, typename _CondFn, typename _Fn, 
        typename = std::enable_if_t<std::is_invocable_v<_Fn, std::shared_ptr<_Ty>&>>>
    inline std::shared_ptr<const _Ty> write_if(std::unique_lock<_Mutex>& _locker, _CondFn&& _cond_fn, _Fn&& _fn)
    {
        auto old_ptr = read();

        if constexpr (std::is_same_v<std::invoke_result_t<_CondFn, const std::shared_ptr<_Ty>&>, bool>)
        {
            if (!_cond_fn(old_ptr))
                return old_ptr;
        }

        util::scope_unique_locker write_locker(_locker);
        old_ptr = read();
        auto new_ptr = std::make_shared<_Ty>(*old_ptr);

        if constexpr (std::is_same_v<std::invoke_result_t<_Fn, std::shared_ptr<_Ty>&>, bool>)
        {
            if (!_fn(new_ptr))
                return old_ptr;
        }
        else {
            _fn(new_ptr);
        }

        std::unique_lock<_GenrcMutex> genrc_locker(genrc_mutex_);
        ptr_ = new_ptr;
        genrc_locker.unlock();
        write_locker.unlock();
        return new_ptr;
    }

    template<typename _CondFn, typename _Fn, 
        typename = std::enable_if_t<std::is_invocable_v<_Fn, std::shared_ptr<_Ty>&>>>
    inline std::shared_ptr<const _Ty> write_if(_CondFn&& _cond_fn, _Fn&& _fn)
    {
        std::unique_lock<_WriteMutex> locker(write_mutex_, std::defer_lock);
        return write_if(locker, std::forward<_CondFn>(_cond_fn), std::forward<_Fn>(_fn));
    }

    inline std::shared_ptr<_Ty> release()
    {
        std::unique_lock<_GenrcMutex> locker(genrc_mutex_);
        return std::move(ptr_);
    }

private:
    std::shared_ptr<_Ty> ptr_;
    _GenrcMutex genrc_mutex_;
    _WriteMutex write_mutex_;
};

}
}

#endif // !MEGOPP_MEMORY_COW_PTR_H_INCLUDED