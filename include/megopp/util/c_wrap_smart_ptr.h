
#ifndef MEGOPP_UTIL_C_WRAP_SMART_PTR_H_INCLUDED
#define MEGOPP_UTIL_C_WRAP_SMART_PTR_H_INCLUDED

#include <memory>

namespace mgpp {
namespace util {

template<typename _Ty, typename _CStruct>
struct c_wrap_smart_ptr
{
    c_wrap_smart_ptr()
        : ptr_(nullptr)
        , __reserve__(0)
        , has_ref_(0)
    {}

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

    inline void reset(const std::shared_ptr<_Ty>& _ptr = nullptr)
    {
        ptr_ = _ptr;
        has_ref_ = _ptr ? 1 : 0;
    }

    std::shared_ptr<_Ty> ptr_;
    size_t __reserve__ : (sizeof(void*) * 8 - 1);
    size_t has_ref_    : 1;
};

template<>
struct c_wrap_smart_ptr<void, void>
{
    std::shared_ptr<void> ptr_;
    size_t __reserve__ : (sizeof(void*) * 8 - 1);
    size_t has_ref_    : 1;
};

static_assert(sizeof( std::shared_ptr<void>) == sizeof(void*) * 2, 
    "sizeof(std::shared_ptr<void>) != sizeof(void*) * 2");
static_assert(sizeof(c_wrap_smart_ptr<void, void>) == sizeof(void*) * 3, 
    "mgpp::util::c_wrap_smart_ptr size error");

}
}

#endif // !MEGOPP_UTIL_C_WRAP_SMART_PTR_H_INCLUDED
