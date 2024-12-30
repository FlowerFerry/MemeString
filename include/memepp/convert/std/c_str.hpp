
#ifndef MEMEPP_CONVERT_STD_C_STR_HPP_INCLUDED
#define MEMEPP_CONVERT_STD_C_STR_HPP_INCLUDED

#include <memepp/string.hpp>
#include <utility>
#include <functional>
#include <tuple>

namespace memepp {
namespace convert {

    struct __cstr_takeover_resource
    {
        template<typename _Fn>
        __cstr_takeover_resource(char* _str, mmint_t _len, _Fn&& _fn)
            : str(_str)
            , len(_len)
            , destruct(std::forward<_Fn>(_fn))
        {}

        ~__cstr_takeover_resource()
        {
            destruct((void*)str);
        }

        char* str;
        mmint_t len;
        std::function<void(void*)> destruct;
    };


    template<typename _Fn>
    inline string takeover(char* _str, mmint_t _len, _Fn&& _fn)
    {
        static const auto destruct_func = [](void* _obj) { 
            delete reinterpret_cast<__cstr_takeover_resource*>(_obj); };
        static const auto data_func = [](const void* _obj)->const char* { 
            return reinterpret_cast<const __cstr_takeover_resource*>(_obj)->str; };
        static const auto size_func = [](const void* _obj)->size_t { 
            return reinterpret_cast<const __cstr_takeover_resource*>(_obj)->len; };

        if (MG_SYM__UNLIKELY(_str == NULL))
            return {};

        if (_len < 0)
            _len = static_cast<mmint_t>(strlen(_str));

        if ((size_t)_len < MemeStringOption_getStorageMediumLimit() + sizeof(__cstr_takeover_resource) + MMSTR__OBJ_SIZE)
        {
            auto out = memepp::string{ _str, _len };
            _fn((void*)_str);
            return out;
        }
        else {
            memepp::string out;
            auto obj = new __cstr_takeover_resource{ _str, _len, std::forward<_Fn>(_fn) };
            auto ret = MemeStringStack_initTakeOverUserObject(
                const_cast<MemeStringStack_t*>(&(out.native_handle())), MMSTR__OBJ_SIZE,
                obj, destruct_func, data_func, size_func);
            if (ret) {
                destruct_func(obj);
                return {};
            }
            return out;
        }
    }

}
}

#endif // !MEMEPP_CONVERT_STD_C_STR_HPP_INCLUDED
