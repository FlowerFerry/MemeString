
#ifndef MGPP_MEM_HASHABLE_WEAK_PTR_HPP_INCLUDED
#define MGPP_MEM_HASHABLE_WEAK_PTR_HPP_INCLUDED

#include <memory>

namespace mgpp {
namespace mem {

template <typename T>
struct hashable_weak_ptr {
    hashable_weak_ptr(): hash_(0) {}
    hashable_weak_ptr(const std::shared_ptr<T>& _ptr): ptr_(_ptr), hash_(std::hash<std::shared_ptr<T>>()(_ptr)) {}

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
        ptr_.reset(_ptr);
        hash_ = std::hash<std::shared_ptr<T>>()(_ptr);
    }

    inline hashable_weak_ptr<T>& operator=(const std::shared_ptr<T>& _ptr) noexcept {
        reset(_ptr);
        return *this;
    }

    inline hashable_weak_ptr<T>& operator=(const hashable_weak_ptr<T>& _other) noexcept {
        reset(_other.lock());
        return *this;
    }

    inline hashable_weak_ptr<T>& operator=(const std::weak_ptr<T>& _other) noexcept {
        reset(_other.lock());
        return *this;
    }

    inline bool operator<(const hashable_weak_ptr<T>& _other) const noexcept 
    {
        return ptr_.owner_before(_other.ptr_);
    }

    inline bool operator==(const hashable_weak_ptr<T>& _other) const noexcept 
    {
        return ptr_.lock() == _other.ptr_.lock();
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
