
#ifndef MEMEPP_CONVERT_STD_UNORDERED_MAP_HPP_INCLUDED
#define MEMEPP_CONVERT_STD_UNORDERED_MAP_HPP_INCLUDED


#include <memepp/string_def.hpp>
#include <memepp/convert/common_def.hpp>
#include <unordered_map>

namespace memepp {
namespace convert {
}

    template<template<class, class, class...> class _Map, 
    typename _Key, typename _Value, typename _KeyParameter, typename _Fn, typename... _MapArgs>
    inline memepp::string from_map(
        const _Map<_Key, _Value, _MapArgs...>& _map, 
        const _KeyParameter& _key, const memepp::string_view& _default, _Fn&& _fn)
    {
        static_assert(std::is_invocable_v<_Fn, const _Value&>, 
            "The function must be invocable with the value type of the map");
        static_assert(std::is_convertible_v<_KeyParameter, _Key>, 
            "The key parameter must be convertible to the key type of the map");

        auto it = _map.find(_key);
        if (it == _map.end())
            return _default.to_string();
        return _fn(it->second);
    }

    template<template<class, class, class...> class _Map, 
    typename _Key, typename _Value, typename _KeyParameter, typename... _MapArgs>
    inline memepp::string from_map(
        const _Map<_Key, _Value, _MapArgs...>& _map, 
        const _KeyParameter& _key, const memepp::string_view& _default)
    {
        static_assert(std::is_same_v<_Value, memepp::string>, 
            "The value type of the map must be memepp::string");
        static_assert(std::is_convertible_v<_KeyParameter, _Key>, 
            "The key parameter must be convertible to the key type of the map");

        auto it = _map.find(_key);
        if (it == _map.end())
            return _default.to_string();
        return it->second;
    }

}

#endif // !MEMEPP_CONVERT_STD_UNORDERED_MAP_HPP_INCLUDED
