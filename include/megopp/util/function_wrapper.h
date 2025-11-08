
#ifndef MEGOPP_UTIL_FUNCTION_WRAPPER_H_INCLUDED
#define MEGOPP_UTIL_FUNCTION_WRAPPER_H_INCLUDED

#include <tuple>
#include <functional>

#include <megopp/help/type_traits.h>

namespace mgpp {
namespace util {

template<typename _Func, typename _Transformer>
struct function_wrapper 
{
    using func_type   = std::function<_Func>;
    using trans_type  = _Transformer;
    using return_type = typename mgpp::function_traits<_Func>::result_type;

    explicit function_wrapper(const func_type& _fn)
        : func_(_fn)
    {}

    explicit function_wrapper(func_type&& _fn)
        : func_(std::move(_fn))
    {}

    template<typename _Fn>
    explicit function_wrapper(_Fn&& _fn)
        : func_(std::forward<_Fn>(_fn))
    {}

    function_wrapper* to_heap_object() &&
    {
        return new function_wrapper(std::move(*this));
    }

    template<typename... _Args>
    static return_type invoke(void* _user_data, _Args&&... _args) 
    {
        if (!_user_data) {
            if constexpr (std::is_void_v<return_type>) {
                return;
            } else if constexpr (std::is_default_constructible_v<return_type>) {
                return return_type{};
            } else {
                throw std::runtime_error("function_wrapper::invoke: no user data and return type is not default constructible");
            }
        }

        auto* wrapper = static_cast<function_wrapper*>(_user_data);
        if (!wrapper->func_) {
            if constexpr (std::is_void_v<return_type>) {
                return;
            } else if constexpr (std::is_default_constructible_v<return_type>) {
                return return_type{};
            } else {
                throw std::runtime_error("function_wrapper::invoke: function is not set and return type is not default constructible");
            }
        }

        auto tuple_args = trans_type::transform(std::forward<_Args>(_args)...);
        return std::apply(wrapper->func_, tuple_args);
    }

    static void destroy(void* _wrapper)
    {
        if (!_wrapper) {
            return;
        }
        delete reinterpret_cast<function_wrapper*>(_wrapper);
    }

    func_type  func_;
};
    
};
};

#endif // !MEGOPP_UTIL_FUNCTION_WRAPPER_H_INCLUDED
