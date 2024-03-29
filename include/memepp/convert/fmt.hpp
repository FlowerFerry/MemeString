
#ifndef MEMEPP_CONVERT_FMT_HPP_INCLUDED
#define MEMEPP_CONVERT_FMT_HPP_INCLUDED

#include "memepp/string_def.hpp"
#include "memepp/string_view_def.hpp"
#include "fmt/format.h"

template <>
struct fmt::formatter<memepp::string> : fmt::formatter<fmt::string_view>
{
	template <typename FormatContext>
	auto format(const memepp::string& _s, FormatContext& ctx)
	{
		return formatter<string_view>::format({ _s.data(), static_cast<size_t>(_s.size()) }, ctx);
	}
};

template <>
struct fmt::formatter<memepp::string_view> : fmt::formatter<fmt::string_view>
{
	template <typename FormatContext>
	auto format(const memepp::string_view& _s, FormatContext& ctx)
	{
		return formatter<string_view>::format({ _s.data(), static_cast<size_t>(_s.size()) }, ctx);
	}
};
#endif // !MEMEPP_CONVERT_FMT_HPP_INCLUDED
