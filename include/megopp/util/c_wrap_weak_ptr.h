
#ifndef MEGOPP_UTIL_C_WRAP_WEAK_PTR_H_INCLUDED
#define MEGOPP_UTIL_C_WRAP_WEAK_PTR_H_INCLUDED

#include <memory>
#include <mego/predef/symbol/likely.h>

namespace mgpp {
namespace util {

template<typename _Ty, typename _CStruct,
    typename = std::enable_if_t<std::is_trivial_v<_CStruct>>>
struct c_wrap_weak_ptr
{
    c_wrap_weak_ptr()
        : w_{}
    {
        static_assert(sizeof(_CStruct) == sizeof(*this), "In mgpp::util::c_wrap_weak_ptr, _CStruct size error");
    }

    c_wrap_weak_ptr(const std::weak_ptr<_Ty>& _ptr)
        : w_{_ptr}
    {}

    c_wrap_weak_ptr(const std::shared_ptr<_Ty>& _ptr)
        : w_{_ptr}
    {}

    inline bool expired() const noexcept
    {
        return w_.expired();
    }

    inline const _CStruct* get_struct_ptr() const noexcept
    {
        return reinterpret_cast<const _CStruct*>(this);
    }

    inline const _Ty* get_object_ptr() const noexcept
    {
        auto ptr = w_.lock();
        return ptr ? ptr.get() : nullptr;
    }

    inline const std::weak_ptr<_Ty>& get_ptr() const noexcept
    {
        return w_;
    }

    inline void reset(const std::weak_ptr<_Ty>& _ptr = {})
    {
        w_ = _ptr;
    }

    inline void reset(const std::shared_ptr<_Ty>& _ptr)
    {
        w_ = _ptr;
    }

    inline _CStruct into_struct() const noexcept
    {
        _CStruct st{};
        *(reinterpret_cast<c_wrap_weak_ptr*>(&st)) = *this;
        return st;
    }

    static inline _CStruct null_struct() noexcept
    {
        _CStruct st{};
        memset(&st, 0, sizeof(st));
        return st;
    }

    static inline _CStruct wrap_struct(const std::weak_ptr<_Ty>& _ptr) noexcept
    {
        return c_wrap_weak_ptr{_ptr}.into_struct();
    }

    static inline std::weak_ptr<_Ty> unwrap_struct(const _CStruct* _st) noexcept
    {
        if (MEGO_SYMBOL__UNLIKELY(_st == nullptr))
            return nullptr;

        return reinterpret_cast<const c_wrap_weak_ptr*>(_st)->get_ptr();
    }

    static inline _CStruct copy_struct(const _CStruct* _st) noexcept
    {
        if (MEGO_SYMBOL__UNLIKELY(_st == nullptr))
            return null_struct();

        auto ptr = reinterpret_cast<const c_wrap_weak_ptr*>(_st);
        if (ptr->expired())
            return null_struct();

        return ptr->into_struct();
    }

    static inline void reset_struct(_CStruct* _st, const std::weak_ptr<_Ty>& _ptr) noexcept
    {
        if (MEGO_SYMBOL__UNLIKELY(_st == nullptr))
            return;

        reinterpret_cast<c_wrap_weak_ptr*>(_st)->reset(_ptr);
    }

private:
    std::weak_ptr<_Ty> w_;
};

} // namespace util
} // namespace mgpp

#endif // MEGOPP_UTIL_C_WRAP_WEAK_PTR_H_INCLUDED
