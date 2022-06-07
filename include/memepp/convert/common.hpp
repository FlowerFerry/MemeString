
#ifndef MEMEPP_CONVERT_COMMON_HPP_INCLUDED
#define MEMEPP_CONVERT_COMMON_HPP_INCLUDED

#include "meme/string.h"
#include "memepp/string.hpp"
#include "memepp/string_view.hpp"

#include <utility>

template<typename _Ty>
inline memepp::string meme_from(_Ty _other)
{
	return memepp::from(std::forward(_other));
}

template<typename _Ty>
inline memepp::string_view meme_view(_Ty _other)
{
	return memepp::view(std::forward(_other));
}

#endif // !MEMEPP_CONVERT_COMMON_HPP_INCLUDED
