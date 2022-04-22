
#ifndef MEMEPP_CONVERT_COMMON_HPP_INCLUDED
#define MEMEPP_CONVERT_COMMON_HPP_INCLUDED

#include "meme/string.h"
#include "memepp/string_def.hpp"

#include <utility>

namespace memepp {

	template<typename _Ty>
	inline memepp::string from_object(_Ty _other)
	{
		return memepp::from(std::forward(_other));
	}

};

#endif // !MEMEPP_CONVERT_COMMON_HPP_INCLUDED
