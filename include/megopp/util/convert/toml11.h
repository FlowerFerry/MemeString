
#ifndef MEGOPP_UTIL_CONVERT_TOML11_H_INCLUDED
#define MEGOPP_UTIL_CONVERT_TOML11_H_INCLUDED

#include <memepp/string_def.hpp>
#include <memepp/string_view_def.hpp>
#include <memepp/convert/toml11.hpp>
#include <toml/value.hpp>
#include <type_traits>

namespace mgpp {
namespace util {

    template <typename _Ty>
    inline _Ty from_toml11_value(
        const toml::value& _value, const _Ty& _default = _Ty{}) 
    {
        static_assert(
            std::is_arithmetic<_Ty>::value || 
            std::is_convertible<_Ty, std::string>::value ||
            std::is_same<_Ty, memepp::string>::value,
            "Type must be arithmetic or convertible to string");

        
        if constexpr (std::is_convertible<_Ty, std::string>::value)
        {
            switch (_value.type())
            {
            case toml::value_t::string:
                return _value.as_string();
            case toml::value_t::integer:
                return std::to_string(_value.as_integer());
            case toml::value_t::floating:
                return std::to_string(_value.as_floating());
            case toml::value_t::boolean:
                return _value.as_boolean() ? "true" : "false";
            default:
                return _default;
            }
        }
        else if constexpr (std::is_same<_Ty, memepp::string>::value)
        {
            switch (_value.type())
            {
            case toml::value_t::string:
                return mm_from(_value.as_string());
            case toml::value_t::integer:
                return mm_from(std::to_string(_value.as_integer()));
            case toml::value_t::floating:
                return mm_from(std::to_string(_value.as_floating()));
            case toml::value_t::boolean:
                return _value.as_boolean() ? "true" : "false";
            default:
                return _default;
            }
        }
        else {
            switch (_value.type())
            {
            case toml::value_t::integer:
                return static_cast<_Ty>(_value.as_integer());
            case toml::value_t::floating:
                return static_cast<_Ty>(_value.as_floating());
            case toml::value_t::boolean:
                return static_cast<_Ty>(_value.as_boolean());
            case toml::value_t::string:
            {
                const char* ptr = static_cast<const std::string&>(_value.as_string()).c_str();
                char *endptr = nullptr;
                auto v = strtod(ptr, &endptr);
                if (endptr == ptr || errno == ERANGE) 
                    return _default;
                return static_cast<_Ty>(v);
            }
            default:
                return _default;
            }
        }
    }

    template <typename _Ty>
    inline _Ty from_toml11_table(
        const toml::table& _table, const std::string& _key, const _Ty& _default = _Ty{}) 
    {
        static_assert(
            std::is_arithmetic<_Ty>::value || 
            std::is_convertible<_Ty, std::string>::value ||
            std::is_same<_Ty, memepp::string>::value ||
            std::is_same<_Ty, memepp::string_view>::value,
            "Type must be arithmetic or convertible to string");

        auto it = _table.find(_key);
        if (it != _table.end()) {
            return from_toml11_value<_Ty>(it->second, _default);
        }
        return _default;
    }

}
}

#endif // !MEGOPP_UTIL_CONVERT_TOML11_H_INCLUDED
