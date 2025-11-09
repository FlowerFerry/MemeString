
#ifndef MEGOPP_UTIL_FUNCTION_WRAPPER_H_INCLUDED
#define MEGOPP_UTIL_FUNCTION_WRAPPER_H_INCLUDED

#include <tuple>
#include <functional>

#include <megopp/help/type_traits.h>

namespace mgpp {
namespace util {

namespace function_wrapper_details {

    template <class T, class Arg, class = void>
    struct has_transform_out : std::false_type {};

    template <class T, class Arg>
    struct has_transform_out<T, Arg, std::void_t<decltype(T::transform_out(std::declval<Arg>()))>> : std::true_type {};

    template<typename T, typename R, bool = false>
    struct final_return_impl {
        using type = R;
    };

    template<typename T, typename R>
    struct final_return_impl<T, R, true> {
        using type = decltype(T::transform_out(std::declval<R>()));
    };
}

template<typename _Func, typename _Transformer>
struct function_wrapper 
{
    using func_type   = std::function<_Func>;
    using trans_type  = _Transformer;
    using return_type = typename mgpp::function_traits<_Func>::result_type;
    using final_return_type = typename function_wrapper_details::final_return_impl<
        trans_type, return_type, 
        (!std::is_void<return_type>::value && 
            function_wrapper_details::has_transform_out<trans_type, return_type>::value)
    >::type;

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
    static final_return_type invoke(void* _user_data, _Args&&... _args) 
    {
        if (!_user_data) {
            if constexpr (std::is_void<final_return_type>::value) {
                return;
            } else if constexpr (std::is_default_constructible_v<final_return_type>) {
                return final_return_type{};
            } else {
                throw std::runtime_error("function_wrapper::invoke: no user data and return type is not default constructible");
            }
        }

        auto* wrapper = static_cast<function_wrapper*>(_user_data);
        if (!wrapper->func_) {
            if constexpr (std::is_void<final_return_type>::value) {
                return;
            } else if constexpr (std::is_default_constructible_v<final_return_type>) {
                return final_return_type{};
            } else {
                throw std::runtime_error("function_wrapper::invoke: function is not set and return type is not default constructible");
            }
        }

        auto tuple_args = trans_type::transform_in(std::forward<_Args>(_args)...);
        if constexpr (!std::is_void<return_type>::value && 
            function_wrapper_details::has_transform_out<trans_type, return_type>::value) 
        {
            auto result = std::apply(wrapper->func_, tuple_args);
            return trans_type::transform_out(result);
        }
        else {
            return std::apply(wrapper->func_, tuple_args);
        } 
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
