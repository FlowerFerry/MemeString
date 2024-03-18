
#ifndef MEMEPP_CONVERT_EXPRTK_HPP_INCLUDED
#define MEMEPP_CONVERT_EXPRTK_HPP_INCLUDED

#include "exprtk.hpp"

#include "memepp/string.hpp"
#include "memepp/string_view.hpp"

#include <memepp/convert/common_def.hpp>

namespace memepp {
	
	inline string from(const exprtk::type_store<float>& _other)
	{
		typename exprtk::type_store<float>::string_view sv(_other);
		return memepp::string{ sv.begin(), static_cast<MemeInteger_t>(sv.size()) };
	}
	
	inline string from(const exprtk::type_store<double>& _other)
	{
		typename exprtk::type_store<double>::string_view sv(_other);
		return memepp::string{ sv.begin(), static_cast<MemeInteger_t>(sv.size()) };
	}
	
	inline string from(const exprtk::type_store<long double>& _other)
	{
		typename exprtk::type_store<long double>::string_view sv(_other);
		return memepp::string{ sv.begin(), static_cast<MemeInteger_t>(sv.size()) };
	}
	
	inline string_view view(const exprtk::type_store<float>& _other)
	{
		typename exprtk::type_store<float>::string_view sv(_other);
		return memepp::string_view{ sv.begin(), static_cast<MemeInteger_t>(sv.size()) };
	}
	
	inline string_view view(const exprtk::type_store<double>& _other)
	{
		typename exprtk::type_store<double>::string_view sv(_other);
		return memepp::string_view{ sv.begin(), static_cast<MemeInteger_t>(sv.size()) };
	}
	
	inline string_view view(const exprtk::type_store<long double>& _other)
	{
		typename exprtk::type_store<long double>::string_view sv(_other);
		return memepp::string_view{ sv.begin(), static_cast<MemeInteger_t>(sv.size()) };
	}

	inline string_view view(const exprtk::type_store<float>::string_view& _other)
	{
		return memepp::string_view{ _other.begin(), static_cast<MemeInteger_t>(_other.size()) };
	}

	inline string_view view(const exprtk::type_store<double>::string_view& _other)
	{
		return memepp::string_view{ _other.begin(), static_cast<MemeInteger_t>(_other.size()) };
	}

	inline string_view view(const exprtk::type_store<long double>::string_view& _other)
	{
		return memepp::string_view{ _other.begin(), static_cast<MemeInteger_t>(_other.size()) };
	}
	
};

template<typename _Ty>
inline memepp::string mm_from(const exprtk::type_store<_Ty>& _other)
{
    return memepp::from(_other);
}

#endif // !MEMEPP_CONVERT_EXPRTK_HPP_INCLUDED
