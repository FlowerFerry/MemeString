
#ifndef MEMEPP_CONVERT_COMMON_HPP_INCLUDED
#define MEMEPP_CONVERT_COMMON_HPP_INCLUDED

#include "memepp/convert/common_def.hpp"

namespace memepp {
	
	template<typename _Ty>
	_Ty to(const memepp::string& _s)
	{
		return _Ty{};
	}

	template<typename _Ty>
	_Ty to(const memepp::string_view& _sv)
	{
		return _Ty{};
	}

};

template<typename _Ty>
inline _Ty meme_to(const memepp::string& _s)
{
	return memepp::to<_Ty>(_s);
}

#endif // !MEMEPP_CONVERT_COMMON_HPP_INCLUDED
