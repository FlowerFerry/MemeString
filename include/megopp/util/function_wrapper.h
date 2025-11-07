
#ifndef MEGOPP_UTIL_FUNCTION_WRAPPER_H_INCLUDED
#define MEGOPP_UTIL_FUNCTION_WRAPPER_H_INCLUDED

#include <tuple>
#include <functional>

namespace mgpp {
namespace util {

template<typename _Func, typename _Transformer>
struct function_wrapper 
{
    using func_type  = std::function<_Func>;
    using trans_type = _Transformer;

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
    static void invoke(void* _user_data, _Args&&... _args) 
    {
        if (!_user_data) {
            return;
        }

        auto* wrapper = static_cast<function_wrapper*>(_user_data);
        if (!wrapper->func_) {
            return;
        }

        auto tuple_args = trans_type::transform(std::forward<_Args>(_args)...);
        std::apply(wrapper->func_, tuple_args);
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
