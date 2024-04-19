
#ifndef MEGOPP_UTIL_C_WRAP_SMART_PTR_H_INCLUDED
#define MEGOPP_UTIL_C_WRAP_SMART_PTR_H_INCLUDED

#include <memory>
#include <mego/predef/symbol/likely.h>

namespace mgpp {
namespace util {

template<typename _Ty, typename _CStruct,
    typename = std::enable_if_t<std::is_trivial_v<_CStruct>>>
struct c_wrap_smart_ptr
{
    c_wrap_smart_ptr()
        : ptr_(nullptr)
        , __reserve__(0)
        , has_ref_(0)
    {
        static_assert(sizeof(_CStruct) == sizeof(*this), "In mgpp::util::c_wrap_smart_ptr, _CStruct size error");
    }

    c_wrap_smart_ptr(const std::shared_ptr<_Ty>& _ptr)
        : ptr_(_ptr)
        , __reserve__(0)
        , has_ref_(_ptr ? 1 : 0)
    {}
    
    inline constexpr bool is_null() const noexcept
    {
        return has_ref_ == 0;
    }

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

    inline _CStruct into_struct() const noexcept
    {
        _CStruct st{};
        *(reinterpret_cast<c_wrap_smart_ptr*>(&st)) = *this;
        return st;
    }

    inline void reset(const std::shared_ptr<_Ty>& _ptr = nullptr)
    {
        ptr_ = _ptr;
        has_ref_ = _ptr ? 1 : 0;
    }

    static inline _CStruct null_struct() noexcept
    {
        return c_wrap_smart_ptr{}.into_struct();
    }

    static inline _CStruct wrap_struct(const std::shared_ptr<_Ty>& _ptr) noexcept
    {
        return c_wrap_smart_ptr{_ptr}.into_struct();
    }

    static inline std::shared_ptr<_Ty> unwrap_struct(const _CStruct* _st) noexcept
    {
        if (MEGO_SYMBOL__UNLIKELY(_st == nullptr))
            return nullptr;

        auto ptr = reinterpret_cast<const c_wrap_smart_ptr*>(_st);
        if (MEGO_SYMBOL__UNLIKELY(ptr->is_null()))
            return nullptr;

        return ptr->get_ptr();
    }

    static inline _CStruct ref_struct(const _CStruct* _st) noexcept
    {
        if (MEGO_SYMBOL__UNLIKELY(_st == nullptr))
            return null_struct();
        
        auto ptr = reinterpret_cast<const c_wrap_smart_ptr*>(_st);
        if (MEGO_SYMBOL__UNLIKELY(ptr->is_null()))
            return null_struct();

        return ptr->into_struct();
    }

    static inline void unref_struct(_CStruct* _st, const std::shared_ptr<_Ty>& _ptr = nullptr) noexcept
    {
        if (MEGO_SYMBOL__UNLIKELY(_st == nullptr))
            return;

        auto ptr = reinterpret_cast<c_wrap_smart_ptr*>(_st);
        ptr->reset(_ptr);
    }

    static inline void reset_struct(_CStruct* _st, const std::shared_ptr<_Ty>& _ptr) noexcept
    {
        if (MEGO_SYMBOL__UNLIKELY(_st == nullptr))
            return;

        auto ptr = reinterpret_cast<c_wrap_smart_ptr*>(_st);
        ptr->reset(_ptr);
    }

    std::shared_ptr<_Ty> ptr_;
    size_t __reserve__ : (sizeof(void*) * 8 - 1);
    size_t has_ref_    : 1;
};

static_assert(sizeof( std::shared_ptr<void>) == sizeof(void*) * 2, 
    "sizeof(std::shared_ptr<void>) != sizeof(void*) * 2");
}
}

#endif // !MEGOPP_UTIL_C_WRAP_SMART_PTR_H_INCLUDED
