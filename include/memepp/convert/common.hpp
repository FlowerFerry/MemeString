
#ifndef MEMEPP_CONVERT_COMMON_HPP_INCLUDED
#define MEMEPP_CONVERT_COMMON_HPP_INCLUDED

#include "memepp/convert/common_def.hpp"

namespace memepp {
	
	template<typename _Ty>
	inline _Ty to(const memepp::string& _s)
	{
		return _Ty{ _s.data(), static_cast<size_t>(_s.size()) };
	}

	template<typename _Ty>
	inline _Ty to(const memepp::string_view& _sv)
	{
		return _Ty{ _sv.data(), static_cast<size_t>(_sv.size()) };
	}

};

template<typename _Ty>
inline _Ty mm_to(const memepp::string& _s)
{
	return memepp::to<_Ty>(_s);
}

template<typename _Ty>
inline _Ty mm_to(const memepp::string_view& _s)
{
	return memepp::to<_Ty>(_s);
}

template<typename _Ty>
inline memepp::string_view mm_view(const _Ty& _s)
{
	return memepp::view(_s);
}

template<typename _Ty>
inline memepp::string mm_from(const _Ty& _s)
{
	return memepp::from(_s);
}

#endif // !MEMEPP_CONVERT_COMMON_HPP_INCLUDED
