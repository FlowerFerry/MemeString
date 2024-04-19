
#ifndef MEGOPP_UTIL_C_WRAP_SMART_PTR_H_INCLUDED
#define MEGOPP_UTIL_C_WRAP_SMART_PTR_H_INCLUDED

#include <memory>
#include <mego/predef/symbol/likely.h>

namespace mgpp {
namespace util {

template<typename _Ty, typename _CStruct,
    typename = std::enable_if_t<std::is_trivial_v<_CStruct>>>
struct c_wrap_shared_ptr
{
    c_wrap_shared_ptr()
        : ptr_(nullptr)
    {
        static_assert(sizeof(_CStruct) == sizeof(*this), "In mgpp::util::c_wrap_shared_ptr, _CStruct size error");
    }

    c_wrap_shared_ptr(const std::shared_ptr<_Ty>& _ptr)
        : ptr_(_ptr)
    {}
    
    inline const _CStruct* get_struct_ptr() const noexcept
    {
        return reinterpret_cast<const _CStruct*>(this);
    }

    inline const _Ty* get_object_ptr() const noexcept
    {
        return ptr_.get();
    }

    inline const std::shared_ptr<_Ty>& get_ptr() const noexcept
    {
        return ptr_;
    }

    explicit inline operator bool() const noexcept
    {
        return static_cast<bool>(ptr_);
    }

    inline _CStruct into_struct() const noexcept
    {
        _CStruct st{};
        *(reinterpret_cast<c_wrap_shared_ptr*>(&st)) = *this;
        return st;
    }

    inline void reset(const std::shared_ptr<_Ty>& _ptr = nullptr)
    {
        ptr_ = _ptr;
    }

    static inline _CStruct null_struct() noexcept
    {
        return c_wrap_shared_ptr{}.into_struct();
    }

    static inline _CStruct wrap_struct(const std::shared_ptr<_Ty>& _ptr) noexcept
    {
        return c_wrap_shared_ptr{_ptr}.into_struct();
    }

    static inline const std::shared_ptr<_Ty>& unwrap_struct(const _CStruct& _st) noexcept
    {
        auto ptr = reinterpret_cast<const c_wrap_shared_ptr*>(&_st);
        return ptr->get_ptr();
    }

    static inline std::shared_ptr<_Ty> unwrap_struct(const _CStruct* _st) noexcept
    {
        if (MEGO_SYMBOL__UNLIKELY(_st == nullptr))
            return nullptr;

        auto ptr = reinterpret_cast<const c_wrap_shared_ptr*>(_st);
        return ptr->get_ptr();
    }

    static inline _CStruct copy_struct(const _CStruct& _st) noexcept
    {
        auto ptr = reinterpret_cast<const c_wrap_shared_ptr*>(&_st);
        if (!ptr)
            return null_struct();

        return ptr->into_struct();
    }

    static inline _CStruct copy_struct(const _CStruct* _st) noexcept
    {
        if (MEGO_SYMBOL__UNLIKELY(_st == nullptr))
            return null_struct();
        
        auto ptr = reinterpret_cast<const c_wrap_shared_ptr*>(_st);
        if (!ptr)
            return null_struct();

        return ptr->into_struct();
    }

    static inline void reset_struct(_CStruct& _st, const std::shared_ptr<_Ty>& _ptr = nullptr) noexcept
    {
        auto ptr = reinterpret_cast<c_wrap_shared_ptr*>(&_st);
        ptr->reset(_ptr);
    }

    static inline void reset_struct(_CStruct* _st, const std::shared_ptr<_Ty>& _ptr = nullptr) noexcept
    {
        if (MEGO_SYMBOL__UNLIKELY(_st == nullptr))
            return;

        auto ptr = reinterpret_cast<c_wrap_shared_ptr*>(_st);
        ptr->reset(_ptr);
    }

private:
    std::shared_ptr<_Ty> ptr_;
};

}
}

#endif // !MEGOPP_UTIL_C_WRAP_SMART_PTR_H_INCLUDED
