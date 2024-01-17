
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
