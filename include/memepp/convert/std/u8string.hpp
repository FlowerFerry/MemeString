
#ifndef MEMEPP_CONVERT_STD_U8STRING_HPP_INCLUDED
#define MEMEPP_CONVERT_STD_U8STRING_HPP_INCLUDED

#include "memepp/string.hpp"
#include "memepp/string_view.hpp"
#include "memepp/convert/common_def.hpp"

#include <string>

/* std::u8string exists only with C++20 char8_t.  Before that the whole file
   is inert so including it stays harmless. */
#if defined(__cpp_char8_t)

namespace memepp  {
namespace convert {

	template<>
	struct from<::std::u8string>
	{
		static memepp::string from_object(const ::std::u8string& _s)
		{
			/* u8string already holds UTF-8 bytes; no transcoding needed. */
			return memepp::string{
				reinterpret_cast<const char*>(_s.data()), static_cast<MemeInteger_t>(_s.size()) };
		}
		static memepp::string from_object(::std::u8string&& _s)
		{
			auto out = memepp::string{
				reinterpret_cast<const char*>(_s.data()), static_cast<MemeInteger_t>(_s.size()) };
			/* Consistent with from(std::wstring&&) / from(std::u16string&&). */
			_s.clear();
			return out;
		}
	};

	template<>
	struct into<::std::u8string>
	{
		static ::std::u8string into_object(const memepp::string& _s)
		{
			return ::std::u8string{
				reinterpret_cast<const char8_t*>(_s.data()),
				static_cast<size_t>(_s.size()) };
		}
		static ::std::u8string into_object(const memepp::string_view& _sv)
		{
			return ::std::u8string{
				reinterpret_cast<const char8_t*>(_sv.data()),
				static_cast<size_t>(_sv.size()) };
		}
	};

}

	inline memepp::string from(const std::u8string& _s)
	{
		return convert::from<::std::u8string>::from_object(_s);
	}

	inline memepp::string from(std::u8string&& _s)
	{
		return convert::from<::std::u8string>::from_object(std::move(_s));
	}

	template<>
	inline std::u8string to<std::u8string>(const memepp::string& _s)
	{
		return convert::into<::std::u8string>::into_object(_s);
	}

	template<>
	inline std::u8string to<std::u8string>(const memepp::string_view& _sv)
	{
		return convert::into<::std::u8string>::into_object(_sv);
	}

}

inline memepp::string mm_from(const std::u8string& _s)
{
	return memepp::from(_s);
}

inline memepp::string mm_from(std::u8string&& _s)
{
	return memepp::from(std::move(_s));
}

#include <memepp/convert/common.hpp>

#endif // __cpp_char8_t

#endif // !MEMEPP_CONVERT_STD_U8STRING_HPP_INCLUDED
