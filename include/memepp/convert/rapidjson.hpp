
#ifndef MEMEPP_CONVERT_RAPIDJSON_HPP_INCLUDED
#define MEMEPP_CONVERT_RAPIDJSON_HPP_INCLUDED

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "memepp/string.hpp"

namespace memepp {

	inline memepp::string form(const rapidjson::GenericStringRef<char>& _s)
	{
		return memepp::string { _s.s, static_cast<MemeInteger_t>(_s.length) };
	}

	inline memepp::string form(const rapidjson::StringBuffer& _s)
	{
		return memepp::string{ _s.GetString(), static_cast<MemeInteger_t>(_s.GetLength())};
	}

	inline memepp::string form(const rapidjson::Value& _v)
	{
		if (_v.IsString())
			return memepp::string{ _v.GetString(), static_cast<MemeInteger_t>(_v.GetStringLength()) };
		else
			return {};
	}

	inline memepp::string_view view(const rapidjson::GenericStringRef<char>& _s)
	{
		return memepp::string_view { _s.s, static_cast<MemeInteger_t>(_s.length) };
	}

	inline memepp::string_view view(const rapidjson::StringBuffer& _s)
	{
		return memepp::string_view { _s.GetString(), static_cast<MemeInteger_t>(_s.GetLength()) };
	}

	inline memepp::string_view view(const rapidjson::Value& _v)
	{
		if (_v.IsString())
			return memepp::string_view { _v.GetString(), static_cast<MemeInteger_t>(_v.GetStringLength()) };
		else
			return {};
	}

};

#endif // !MEMEPP_CONVERT_RAPIDJSON_HPP_INCLUDED
