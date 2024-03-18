
#ifndef MEMEPP_CONVERT_STRING_VIEW_HPP_INCLUDED
#define MEMEPP_CONVERT_STRING_VIEW_HPP_INCLUDED

#include "meme/string.h"
#include "memepp/string.hpp"
#include "memepp/string_view.hpp"
#include "memepp/convert/common_def.hpp"

#include <string_view>

namespace memepp {
	
	inline memepp::string from(const std::string_view& _s)
	{
		return memepp::string(_s.data(), static_cast<mmint_t>(_s.size()));
	}
	
	inline memepp::string_view view(const std::string_view& _s)
	{
		return memepp::string_view{ _s.data(), static_cast<mmint_t>(_s.size()) };
	}

	template<>
	inline std::string_view to<std::string_view>(const memepp::string& _s)
	{
		return std::string_view{ _s.data(), static_cast<size_t>(_s.size()) };
	}

	template<>
	inline std::string_view to<std::string_view>(const memepp::string_view& _sv)
	{
		return std::string_view{ _sv.data(), static_cast<size_t>(_sv.size()) };
	}
	
}

inline memepp::string mm_from(const std::string_view& _s)
{
	return memepp::from(_s);
}


#endif // !MEMEPP_CONVERT_STRING_VIEW_HPP_INCLUDED
