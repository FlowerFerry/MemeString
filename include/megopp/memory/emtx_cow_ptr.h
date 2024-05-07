
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
class emtx_cow_ptr 
{
public:

    emtx_cow_ptr() noexcept
        : ptr_()
    {    
    }

    emtx_cow_ptr(std::nullptr_t) noexcept
        : ptr_(nullptr)
    {}

    template <typename _Derived>
    emtx_cow_ptr(_Derived* _ptr)
        : ptr_(_ptr)
    {}

    template <typename _Derived, typename _Deleter>
    emtx_cow_ptr(_Derived* _ptr, _Deleter _deleter)
        : ptr_(_ptr, _deleter)
    {}

    template<typename _Deleter>
    emtx_cow_ptr(std::nullptr_t, _Deleter _deleter)
        : ptr_(nullptr, _deleter)
    {}

    template<typename _Derived, typename _Deleter, typename _Alloc>
    emtx_cow_ptr(_Derived* _ptr, _Deleter _deleter, _Alloc _alloc)
        : ptr_(_ptr, _deleter, _alloc)
    {}

    template<typename _Deleter, typename _Alloc>
    emtx_cow_ptr(std::nullptr_t, _Deleter _deleter, _Alloc _alloc)
        : ptr_(nullptr, _deleter, _alloc)
    {}

    emtx_cow_ptr(const emtx_cow_ptr& _other)
        : ptr_(_other.read())
    {}

    emtx_cow_ptr(emtx_cow_ptr&& _other)
        : ptr_(_other.release())
    {}

    template<typename _Derived>
    emtx_cow_ptr(const std::shared_ptr<_Derived>& _ptr) noexcept
        : ptr_(_ptr)
    {}

    template<typename _Derived>
    emtx_cow_ptr(std::shared_ptr<_Derived>&& _ptr) noexcept
        : ptr_(std::move(_ptr))
    {}

    template<typename _Derived>
    emtx_cow_ptr(const std::weak_ptr<_Derived>& _ptr)
        : ptr_(_ptr)
    {}

    template<typename _Derived, typename _Deleter>
    emtx_cow_ptr(std::unique_ptr<_Derived, _Deleter>&& _ptr)
        : ptr_(std::move(_ptr))
    {}

    template<
        typename = std::enable_if_t<
            std::is_same<_GenrcMutex, megopp::help::null_mutex>::value>>
    inline emtx_cow_ptr& assign(const emtx_cow_ptr& _other)
    {
        if (this != &_other) {
            ptr_ = _other.read();
        }
        return *this;
    }

    inline emtx_cow_ptr& assign(const emtx_cow_ptr& _other, std::unique_lock<_GenrcMutex>& _locker)
    {
        if (this != &_other) {
            util::scope_unique_locker<_GenrcMutex> locker(_locker);
            ptr_ = _other.ptr_;
        }
        return *this;
    }

    template<
        typename = std::enable_if_t<
            std::is_same<_GenrcMutex, megopp::help::null_mutex>::value>>
    inline emtx_cow_ptr& assign(emtx_cow_ptr&& _other)
    {
        if (this != &_other) {
            ptr_ = _other.release();
        }
        return *this;
    }

    inline emtx_cow_ptr& assign(emtx_cow_ptr&& _other, std::unique_lock<_GenrcMutex>& _locker)
    {
        if (this != &_other) {
            util::scope_unique_locker<_GenrcMutex> locker(_locker);
            ptr_ = _other.release(_locker);
        }
        return *this;
    }

    template<typename _Derived>
    inline emtx_cow_ptr& assign(const std::shared_ptr<_Derived>& _ptr, std::unique_lock<_GenrcMutex>& _locker)
    {
        util::scope_unique_locker<_GenrcMutex> locker(_locker);
        ptr_ = _ptr;
        return *this;
    }

    template<typename _Derived>
    inline emtx_cow_ptr& assign(std::shared_ptr<_Derived>&& _ptr, std::unique_lock<_GenrcMutex>& _locker)
    {
        util::scope_unique_locker<_GenrcMutex> locker(_locker);
        ptr_ = std::move(_ptr);
        return *this;
    }

    template<typename _Derived, typename _Deleter>
    inline emtx_cow_ptr& assign(std::unique_ptr<_Derived, _Deleter>&& _ptr, std::unique_lock<_GenrcMutex>& _locker)
    {
        util::scope_unique_locker<_GenrcMutex> locker(_locker);
        ptr_ = std::move(_ptr);
        return *this;
    }

    template<
        typename = std::enable_if_t<
            std::is_same<_GenrcMutex, megopp::help::null_mutex>::value>>
    inline std::shared_ptr<const _Ty> read() const
    {
        return ptr_;
    }

    inline std::shared_ptr<const _Ty> read(std::shared_lock<_GenrcMutex>& _locker) const
    {
        util::scope_shared_locker<_GenrcMutex> locker(_locker);
        return ptr_;
    }

    template<typename _Fn, 
        typename = std::enable_if_t<std::is_invocable_v<_Fn, std::shared_ptr<_Ty>&>>>
    inline std::shared_ptr<const _Ty> write(
        std::unique_lock<_WriteMutex>& _write_locker, 
        util::rw_lock<_GenrcMutex>& _genrc_locker,
        _Fn&& _fn)
    {
        util::scope_unique_locker write_locker(_write_locker);

        _genrc_locker.lock_shared();
        auto old_ptr = ptr_;
        _genrc_locker.unlock();
        auto new_ptr = std::make_shared<_Ty>(*old_ptr);

        if constexpr (std::is_same_v<std::invoke_result_t<_Fn, std::shared_ptr<_Ty>&>, bool>)
        {
            if (!_fn(new_ptr))
                return old_ptr;
        }
        else {
            _fn(new_ptr);
        }

        _genrc_locker.lock();
        ptr_ = new_ptr;
        _genrc_locker.unlock();
        write_locker.unlock();
        return new_ptr;
    }

    template<typename _CondFn, typename _Fn, 
        typename = std::enable_if_t<std::is_invocable_v<_Fn, std::shared_ptr<_Ty>&>>>
    inline std::shared_ptr<const _Ty> write_if(
        std::unique_lock<_WriteMutex>& _write_locker, 
        util::rw_lock<_GenrcMutex>& _genrc_locker,
        _CondFn&& _cond_fn, _Fn&& _fn)
    {
        _genrc_locker.lock_shared();
        auto old_ptr = ptr_;
        _genrc_locker.unlock();

        if constexpr (std::is_same_v<std::invoke_result_t<_CondFn, const std::shared_ptr<const _Ty>&>, bool>)
        {
            if (!_cond_fn(old_ptr))
                return old_ptr;
        }

        util::scope_unique_locker write_locker(_write_locker);

        _genrc_locker.lock_shared();
        old_ptr = ptr_;
        _genrc_locker.unlock();

        auto new_ptr = std::make_shared<_Ty>(*old_ptr);

        if constexpr (std::is_same_v<std::invoke_result_t<_Fn, std::shared_ptr<_Ty>&>, bool>)
        {
            if (!_fn(new_ptr))
                return old_ptr;
        }
        else {
            _fn(new_ptr);
        }

        _genrc_locker.lock();
        ptr_ = new_ptr;
        _genrc_locker.unlock();
        write_locker.unlock();
        return new_ptr;
    }

    template<
        typename = std::enable_if_t<
            std::is_same<_GenrcMutex, megopp::help::null_mutex>::value>>
    inline std::shared_ptr<_Ty> release()
    {
        return std::move(ptr_);
    }

    inline std::shared_ptr<_Ty> release(std::unique_lock<_GenrcMutex>& _locker)
    {
        util::scope_unique_locker<_GenrcMutex> locker(_locker);
        return std::move(ptr_);
    }

private:

    std::shared_ptr<_Ty> ptr_;
};

}
}

#endif // !MEGOPP_MEMORY_COW_PTR_H_INCLUDED
