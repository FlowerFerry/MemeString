
#ifndef MEMEPP_CONVERT_COMMON_DEF_HPP_INCLUDED
#define MEMEPP_CONVERT_COMMON_DEF_HPP_INCLUDED

#include "meme/string.h"
#include "memepp/string.hpp"
#include "memepp/string_view.hpp"

#include <utility>

template<typename _Ty>
inline _Ty mm_to(const memepp::string& _s);

template<typename _Ty>
inline _Ty mm_to(const memepp::string_view& _s);

template<typename _Ty>
inline memepp::string_view mm_view(const _Ty& _s);

namespace memepp {
	
	template<typename _Ty>
	inline _Ty to(const memepp::string& _s);

	template<typename _Ty>
	inline _Ty to(const memepp::string_view& _sv);

	inline memepp::string_view view(const memepp::string_view& _sv);
	inline memepp::string_view view(const memepp::string& _sv);
    
	inline memepp::string from(memepp::string&& _sv);
	inline memepp::string from(const memepp::string& _sv);
	inline memepp::string from(const memepp::string_view& _sv);


	template<typename _Ty, typename _Result>
	inline _Result import_from_dll(_Ty _obj)
	{
		return _Result{};
	}

	template<typename _Ty, typename _Result>
	inline _Result export_into_dll(_Ty _obj)
	{
		return _Result{};
	}

};

#endif // !MEMEPP_CONVERT_COMMON_DEF_HPP_INCLUDED
