
#ifndef MEMEPP_CONVERT_COMMON_DEF_HPP_INCLUDED
#define MEMEPP_CONVERT_COMMON_DEF_HPP_INCLUDED

#include "meme/string_fwd.h"

#include "memepp/string_fwd.hpp"
#include "memepp/string_view_fwd.hpp"

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


};

#endif // !MEMEPP_CONVERT_COMMON_DEF_HPP_INCLUDED
