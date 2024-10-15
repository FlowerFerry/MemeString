
#ifndef MEGOPP_THRD_SYNCHRONIZED_H_INCLUDED
#define MEGOPP_THRD_SYNCHRONIZED_H_INCLUDED

#include <mutex>
#include <shared_mutex>

#ifndef MGPP_THRD_SYNCHRONIZED_EXCEPTION_ENABLED
#define MGPP_THRD_SYNCHRONIZED_EXCEPTION_ENABLED (1)
#endif

namespace mgpp {
namespace thrd {

namespace synchronized_details {


    template <
        typename _Ty,
        typename _Mutex,
        typename _SharedLock,
        typename _UniqueLock
    >
    class upgrade_ptr;

    template <
        typename _Ty,
        typename _Mutex,
        typename _SharedLock
    >
    class read_life_ptr;

    template <
        typename _Ty,
        typename _Mutex,
        typename _SharedLock,
        typename _UniqueLock
    >
    class write_life_ptr;

    template <
        typename _Ty,
        typename _Mutex,
        typename _SharedLock
    >
    class read_life_ptr
    {
    protected:
        _Ty const* const data_;
        _SharedLock lock_;

        read_life_ptr(const read_life_ptr&) = delete;
        read_life_ptr& operator=(const read_life_ptr&) = delete;
    public:
        read_life_ptr(_Ty const* _ptr, _SharedLock&& _lock) noexcept
            : data_{ _ptr }, lock_{ std::move(_lock) }
        {
        }

        explicit read_life_ptr(_Ty const* _ptr, _Mutex& _mutex) noexcept
            : data_{ _ptr }, lock_{ _mutex }
        {
        }

        read_life_ptr(read_life_ptr&& _other) noexcept
            : data_{ _other.data_ }, lock_{ std::move(_other.lock_) }
        {
            _other.data_ = nullptr;
        }

        inline read_life_ptr& operator=(read_life_ptr&& _other) noexcept
        {
            if (this != &_other) {
                data_ = _other.data_;
                lock_ = std::move(_other.lock_);
                _other.data_ = nullptr;
            }
            return *this;
        }

        virtual ~read_life_ptr() noexcept
        {}

#if MGPP_THRD_SYNCHRONIZED_EXCEPTION_ENABLED
        inline _Ty const* operator->() const
        {
            if (!owns_lock())
            {
                throw std::logic_error("read_life_ptr::operator->() called while owning lock");
            }
            return data_;
        }
#else
        inline _Ty const* operator->() const noexcept
        {
            return data_;
        }
#endif

#if MGPP_THRD_SYNCHRONIZED_EXCEPTION_ENABLED
        inline _Ty const& operator*() const
        {
            if (!owns_lock())
            {
                throw std::logic_error("read_life_ptr::operator*() called while owning lock");
            }
            if (data_ == nullptr)
            {
                throw std::logic_error("read_life_ptr::operator*() called with nullptr");
            }
            return *data_;
        }
#else
        inline _Ty const& operator*() const noexcept
        {
            return *data_;
        }
#endif

        inline bool owns_lock() const
        {
            return lock_.owns_lock();
        }

        inline void lock()
        {
            if (!owns_lock())
            {
                lock_.lock();
            }
        }

        inline void unlock()
        {
            if (owns_lock())
            {
                lock_.unlock();
            }
        }

    };

    template <
        typename _Ty,
        typename _Mutex,
        typename _SharedLock,
        typename _UniqueLock
    >
    class write_life_ptr
    {
        friend class upgrade_ptr<_Ty, _Mutex, _SharedLock, _UniqueLock>;
    protected:
        _Ty* const data_;
        _UniqueLock lock_;
        upgrade_ptr<_Ty, _Mutex, _SharedLock, _UniqueLock>* upgrade_ptr_;

        write_life_ptr(const write_life_ptr&) = delete;
        write_life_ptr& operator=(const write_life_ptr&) = delete;

        write_life_ptr(upgrade_ptr<_Ty, _Mutex, _SharedLock, _UniqueLock>& _upgrade_ptr, _UniqueLock&& _lock);

    public:
        write_life_ptr(_Ty* _ptr, _UniqueLock&& _lock) noexcept
            : data_{ _ptr }
            , lock_{ std::move(_lock) }
            , upgrade_ptr_{ nullptr }
        {
        }

        explicit write_life_ptr(_Ty* _ptr, _Mutex& _mutex) noexcept
            : data_{ _ptr }
            , lock_{ _mutex }
            , upgrade_ptr_{ nullptr }
        {
        }

        write_life_ptr(write_life_ptr&& _other) noexcept
            : data_{ _other.data_ }
            , lock_{ std::move(_other.lock_) }
            , upgrade_ptr_{ _other.upgrade_ptr_ }
        {
            _other.data_ = nullptr;
            _other.upgrade_ptr_ = nullptr;
        }

        inline write_life_ptr& operator=(write_life_ptr&& _other) noexcept
        {
            if (this != &_other) {
                data_ = _other.data_;
                lock_ = std::move(_other.lock_);
                upgrade_ptr_ = _other.upgrade_ptr_;
                _other.data_ = nullptr;
                _other.upgrade_ptr_ = nullptr;
            }
            return *this;
        }

        ~write_life_ptr();

#if MGPP_THRD_SYNCHRONIZED_EXCEPTION_ENABLED
        inline _Ty* operator->() const
        {      
            if (!owns_lock())
            {
                throw std::logic_error("write_life_ptr::operator->() called while owning lock");
            }
            return data_;
        }
#else
        inline _Ty* operator->() const noexcept
        {
            return data_;
        }
#endif

#if MGPP_THRD_SYNCHRONIZED_EXCEPTION_ENABLED
        inline _Ty& operator*() const
        {
            if constexpr (!std::is_same_v<_SharedLock, _UniqueLock>)
            {
                if (!owns_lock())
                {
                    throw std::logic_error("write_life_ptr::operator*() called while owning lock");
                }
            }
            else {
                if (lock_.mutex() && !owns_lock())
                {
                    throw std::logic_error("write_life_ptr::operator*() called while owning lock");
                }
            }
            if (data_ == nullptr)
            {
                throw std::logic_error("write_life_ptr::operator*() called with nullptr");
            }
            return *data_;
        }
#else
        inline _Ty& operator*() const noexcept
        {
            return *data_;
        }
#endif

        inline bool owns_lock() const
        {
            return lock_.owns_lock();
        }

        inline void lock()
        {
            if (!owns_lock())
            {
                lock_.lock();
            }
        }

        inline void unlock()
        {
            if (owns_lock())
            {
                lock_.unlock();
            }
        }

    };

    template <
        typename _Ty,
        typename _Mutex,
        typename _SharedLock,
        typename _UniqueLock
    >
    class upgrade_ptr : public read_life_ptr<_Ty, _Mutex, _SharedLock>
    {
        friend class write_life_ptr<_Ty, _Mutex, _SharedLock, _UniqueLock>;
    public:

        upgrade_ptr(_Ty const* _ptr, _SharedLock&& _lock)
            : read_life_ptr<_Ty, _Mutex, _SharedLock>{ _ptr, std::move(_lock) }
        {
        }

        explicit upgrade_ptr(_Ty const* _ptr, _Mutex& _mutex)
            : read_life_ptr<_Ty, _Mutex, _SharedLock>{ _ptr, _mutex }
        {
        }

        upgrade_ptr(upgrade_ptr&& _other) noexcept
            : read_life_ptr<_Ty, _Mutex, _SharedLock>{ std::move(_other) }
        {
        }

        inline upgrade_ptr& operator=(upgrade_ptr&& _other) noexcept
        {
            if (this != &_other) {
                read_life_ptr<_Ty, _Mutex, _SharedLock>::operator=(std::move(_other));
            }
            return *this;
        }

        inline write_life_ptr<_Ty, _Mutex, _SharedLock, _UniqueLock> upgrade()
        {
            if constexpr (!std::is_same_v<_SharedLock, _UniqueLock>)
            {
                unlock();
                return write_life_ptr<_Ty, _Mutex, _SharedLock, _UniqueLock>{ 
                    *this, _UniqueLock{ *(lock_.mutex()) } };
            }
            else {
                return write_life_ptr<_Ty, _Mutex, _SharedLock, _UniqueLock>{ 
                    *this, _UniqueLock{} };
            }
        }

    };

    template <
        typename _Ty,
        typename _Mutex,
        typename _SharedLock,
        typename _UniqueLock
    >
    write_life_ptr<_Ty, _Mutex, _SharedLock, _UniqueLock>::
    write_life_ptr(upgrade_ptr<_Ty, _Mutex, _SharedLock, _UniqueLock>& _upgrade_ptr, _UniqueLock&& _lock)
        : data_{ const_cast<_Ty*>(_upgrade_ptr.data_) }
        , lock_{ std::move(_lock) }
        , upgrade_ptr_{ &_upgrade_ptr }
    {
    }
    
    template <
        typename _Ty,
        typename _Mutex,
        typename _SharedLock,
        typename _UniqueLock
    >
    write_life_ptr<_Ty, _Mutex, _SharedLock, _UniqueLock>::~write_life_ptr()
    {
        if constexpr (!std::is_same_v<_SharedLock, _UniqueLock>)
        {
            if (upgrade_ptr_) {
                unlock();
                upgrade_ptr_->lock();
            }
        }
    }

}

template <
    typename _Ty,
    typename _Mutex = std::mutex,
    typename _SharedLock = std::unique_lock<_Mutex>,
    typename _UniqueLock = std::unique_lock<_Mutex>
>
class synchronized_value
{
    mutable _Mutex mutex_;
    _Ty value_;
    
public:
    
    template <typename... _Args>
    synchronized_value(_Args&&... _args)
        : value_{ std::forward<_Args>(_args)... }
    {}
    
    inline synchronized_details::read_life_ptr<_Ty, _Mutex, _SharedLock> 
        rlock() const  noexcept
    {
        return synchronized_details::read_life_ptr<_Ty, _Mutex, _SharedLock>{ &value_, mutex_ };
    }

    inline synchronized_details::write_life_ptr<_Ty, _Mutex, _SharedLock, _UniqueLock>
        wlock() noexcept
    {
        return synchronized_details::write_life_ptr<_Ty, _Mutex, _SharedLock, _UniqueLock>{ &value_, mutex_ };
    }

    inline synchronized_details::upgrade_ptr<_Ty, _Mutex, _SharedLock, _UniqueLock>
        ulock() noexcept
    {
        return synchronized_details::upgrade_ptr<_Ty, _Mutex, _SharedLock, _UniqueLock>{ &value_, mutex_ };
    }

    template <typename _Fn>
    inline auto with_rlock(_Fn&& _fn) const
    {
        _SharedLock lock{ mutex_ };
        return _fn(&value_);
    }
    
    template <typename _Fn>
    inline auto with_wlock(_Fn&& _fn)
    {
        _UniqueLock lock{ mutex_ };
        return _fn(&value_);
    }

    template <typename _Fn>
    inline auto with_ulock(_Fn&& _fn) const
    {
        return _fn(this->ulock());
    }

};

template <typename _Ty>
using shared_synchronized_value =
synchronized_value<_Ty, std::shared_mutex, std::shared_lock<std::shared_mutex>>;

}    
}

#endif // !MEGOPP_THRD_SYNCHRONIZED_H_INCLUDED
