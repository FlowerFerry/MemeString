
#ifndef MEMEPP_CONVERT_TOML_EX_HPP_INCLUDED
#define MEMEPP_CONVERT_TOML_EX_HPP_INCLUDED

#include <memepp/string_def.hpp>
#include <memepp/string_view_def.hpp>
#include <memepp/convert/common_def.hpp>
#include <toml/value.hpp>
#include "./std/string.hpp"

#include <string_view>

namespace memepp  {
namespace convert {

    template<>
	struct from<::toml::string>
    {
		static memepp::string from_object(const ::toml::string& _v)
		{
            return mm_from(static_cast<const std::string&>(_v));
		}
		static memepp::string from_object(::toml::string&& _v)
		{
            return mm_from(static_cast<std::string&&>(_v));
		}
	};

	template<>
	struct view<::toml::string>
	{
		static memepp::string_view view_object(const ::toml::string& _v)
		{
            return mm_view(static_cast<const std::string&>(_v));
		}
	};

    template<>
	struct into<::toml::string>
	{
		static ::toml::string into_object(const memepp::string& _v)
		{
            return std::string_view{ _v.data(), static_cast<size_t>(_v.size()) };
		}
		static ::toml::string into_object(const memepp::string_view& _v)
		{
            return std::string_view{ _v.data(), static_cast<size_t>(_v.size()) };
		}
	};
    
}

    inline string from_toml11_table(
        const toml::table& _table, 
        const ::std::string& _key, const string_view& _default)
    {
        auto it = _table.find(_key);
        if (it != _table.end() && it->second.is_string())
            return mm_from(it->second.as_string());
        return _default.to_string();
    }

}

#endif // !MEMEPP_CONVERT_TOML_EX_HPP_INCLUDED
