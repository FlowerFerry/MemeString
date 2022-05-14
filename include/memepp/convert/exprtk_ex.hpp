
#ifndef MEMEPP_CONVERT_EXPRTK_HPP_INCLUDED
#define MEMEPP_CONVERT_EXPRTK_HPP_INCLUDED

#include "exprtk.hpp"

#include "memepp/string.hpp"
#include "memepp/string_view.hpp"

namespace memepp {
		
	template<typename _Ty>
	inline string from(const exprtk::type_store<_Ty>& _other)
	{
		typename exprtk::type_store<_Ty>::string_view sv(_other);
		return memepp::string{ sv.begin(), sv.size() };
	}

	template<typename _Ty>
	inline string_view view(const exprtk::type_store<_Ty>& _other)
	{
		typename exprtk::type_store<_Ty>::string_view sv(_other);
		return memepp::string_view{ sv.begin(), sv.size() };
	}

};

#endif // !MEMEPP_CONVERT_EXPRTK_HPP_INCLUDED
