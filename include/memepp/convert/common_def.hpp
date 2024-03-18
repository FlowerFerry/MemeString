
#ifndef MEMEPP_CONVERT_COMMON_DEF_HPP_INCLUDED
#define MEMEPP_CONVERT_COMMON_DEF_HPP_INCLUDED

#include "meme/string_fwd.h"

#include "memepp/string_def.hpp"
#include "memepp/string_view_def.hpp"

#include <utility>

template<typename _Ty>
_Ty mm_to(const memepp::string& _s);

template<typename _Ty>
_Ty mm_to(const memepp::string_view& _s);

template<typename _Ty>
_Ty mm_into(const memepp::string& _s);

template<typename _Ty>
_Ty mm_into(const memepp::string_view& _s);

template<typename _Ty>
memepp::string_view mm_view(const _Ty& _s);

namespace memepp {

	memepp::string_view view(const memepp::string_view& _sv);
	memepp::string_view view(const memepp::string& _sv);

	template<typename _Ty>
	_Ty to(const memepp::string& _s);

	template<typename _Ty>
	_Ty to(const memepp::string_view& _sv);

	memepp::string from(memepp::string&& _sv);
	memepp::string from(const memepp::string& _sv);
	memepp::string from(const memepp::string_view& _sv);


};

#endif // !MEMEPP_CONVERT_COMMON_DEF_HPP_INCLUDED
