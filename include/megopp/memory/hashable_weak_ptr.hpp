
#ifndef MGPP_MEM_HASHABLE_WEAK_PTR_HPP_INCLUDED
#define MGPP_MEM_HASHABLE_WEAK_PTR_HPP_INCLUDED

#include <memory>
#include <functional>

namespace mgpp {
namespace mem {

//! A weak_ptr wrapper that can be used as a key in std::unordered_map / std::unordered_set.
//!
//! Problem with plain std::weak_ptr:
//!   - It provides no default hash or equality, so it cannot be used directly as an
//!     unordered container key.
//!   - Even with a custom hash, comparing via lock() breaks once the managed object is
//!     destroyed: all expired weak_ptrs lock() to nullptr, making them appear equal
//!     while their hashes remain distinct — violating the invariant required by hash
//!     containers (a == b  =>  hash(a) == hash(b)).
//!
//! How this class solves it:
//!   - hash_  is computed once at construction from the shared_ptr (i.e. the control
//!     block address) and never changes afterwards, even after the managed object dies.
//!   - operator== uses owner_before() to compare control-block identity, which is also
//!     stable after expiry.  Together, hash and equality always refer to the same
//!     immutable identity, satisfying the hash-container invariant at all times.
//!   - operator< likewise uses owner_before(), so the type is also usable as a key in
//!     ordered containers (std::map / std::set).
//!
//! Typical usage:
//!   std::unordered_map<mgpp::mem::hashable_weak_ptr<Foo>, Value> map;
//!   auto key = mgpp::mem::hashable_weak_ptr<Foo>(shared_foo);
//!   map[key] = value;
//!   // shared_foo may be destroyed here; the key remains valid for lookup/erase.
template <typename T>
struct hashable_weak_ptr {
    hashable_weak_ptr(): hash_(0) {}
    hashable_weak_ptr(const hashable_weak_ptr<T>& _other): ptr_(_other.ptr_), hash_(_other.hash_) {}
    hashable_weak_ptr(const std::shared_ptr<T>& _ptr): ptr_(_ptr), hash_(std::hash<std::shared_ptr<T>>()(_ptr)) {}
    hashable_weak_ptr(const std::weak_ptr<T>& _other)
        : ptr_(_other)
        , hash_(std::hash<std::shared_ptr<T>>()(_other.lock()))
    {}

    inline constexpr size_t hash_value() const noexcept {
        return hash_;
    }

    inline std::shared_ptr<T> lock() const noexcept {
        return ptr_.lock();
    }

    inline bool expired() const noexcept {
        return ptr_.expired();
    }

    inline std::weak_ptr<T> weak_ptr() const noexcept {
        return ptr_;
    }

    inline void reset() noexcept {
        ptr_.reset();
        hash_ = 0;
    }

    inline void reset(const std::shared_ptr<T>& _ptr) noexcept {
        ptr_ = _ptr;
        hash_ = std::hash<std::shared_ptr<T>>()(_ptr);
    }

    inline hashable_weak_ptr<T>& operator=(const std::shared_ptr<T>& _ptr) noexcept {
        reset(_ptr);
        return *this;
    }

    inline hashable_weak_ptr<T>& operator=(const hashable_weak_ptr<T>& _other) noexcept {
        ptr_  = _other.ptr_;
        hash_ = _other.hash_;
        return *this;
    }

    inline hashable_weak_ptr<T>& operator=(const std::weak_ptr<T>& _other) noexcept {
        ptr_  = _other;
        hash_ = std::hash<std::shared_ptr<T>>()(_other.lock());
        return *this;
    }

    inline bool operator<(const hashable_weak_ptr<T>& _other) const noexcept 
    {
        return ptr_.owner_before(_other.ptr_);
    }

    inline bool operator==(const hashable_weak_ptr<T>& _other) const noexcept 
    {
        // Use owner_before for stable identity comparison — works even after the
        // managed object is destroyed, as the control block outlives it.
        return !ptr_.owner_before(_other.ptr_) && !_other.ptr_.owner_before(ptr_);
    }

    inline bool operator!=(const hashable_weak_ptr<T>& _other) const noexcept 
    {
        return !(*this == _other);
    }

    std::weak_ptr<T> ptr_;
    size_t hash_;
};

}
}

namespace std {
    template <typename T>
    struct hash<mgpp::mem::hashable_weak_ptr<T>> {
        inline size_t operator()(const mgpp::mem::hashable_weak_ptr<T>& _ptr) const noexcept {
            return _ptr.hash_value();
        }
    };
}

#endif // !MGPP_MEM_HASHABLE_WEAK_PTR_HPP_INCLUDED
