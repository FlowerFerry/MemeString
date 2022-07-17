
#ifndef MEMEPP_CONVERT_RAPIDJSON_HPP_INCLUDED
#define MEMEPP_CONVERT_RAPIDJSON_HPP_INCLUDED

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

#include "memepp/convert/common_def.hpp"
#include "memepp/string.hpp"

namespace memepp {

	inline memepp::string from(const rapidjson::GenericStringRef<char>& _s)
	{
		return memepp::string { _s.s, static_cast<MemeInteger_t>(_s.length) };
	}

	inline memepp::string from(const rapidjson::StringBuffer& _s)
	{
		return memepp::string{ _s.GetString(), static_cast<MemeInteger_t>(_s.GetLength())};
	}

	inline memepp::string from(const rapidjson::Value& _v)
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

	template<>
	inline rapidjson::GenericStringRef<char> to< rapidjson::GenericStringRef<char> >(const memepp::string& _s)
	{
		return rapidjson::GenericStringRef<char>{ _s.data(), static_cast<rapidjson::SizeType>(_s.size()) };
	}

	template<>
	inline rapidjson::GenericStringRef<char> to< rapidjson::GenericStringRef<char> >(const memepp::string_view& _sv)
	{
		return rapidjson::GenericStringRef<char>{ _sv.data(), static_cast<rapidjson::SizeType>(_sv.size()) };
	}


};

namespace rapidjson
{
	inline rapidjson::GenericStringRef<char> toStringRef(const memepp::string_view& _s)
	{
		return rapidjson::GenericStringRef<char>{ _s.data(), static_cast<rapidjson::SizeType>(_s.size()) };
	}

};

#endif // !MEMEPP_CONVERT_RAPIDJSON_HPP_INCLUDED
