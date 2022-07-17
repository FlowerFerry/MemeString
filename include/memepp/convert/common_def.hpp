
#ifndef MEMEPP_CONVERT_COMMON_DEF_HPP_INCLUDED
#define MEMEPP_CONVERT_COMMON_DEF_HPP_INCLUDED

#include "meme/string.h"
#include "memepp/string.hpp"
#include "memepp/string_view.hpp"

#include <utility>

template<typename _Ty>
_Ty meme_to(const memepp::string& _s);

namespace memepp {
	
	template<typename _Ty>
	_Ty to(const memepp::string& _s);

	template<typename _Ty>
	_Ty to(const memepp::string_view& _sv);

};

#endif // !MEMEPP_CONVERT_COMMON_DEF_HPP_INCLUDED
