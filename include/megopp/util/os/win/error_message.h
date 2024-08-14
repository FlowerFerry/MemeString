
#ifndef MEGOPP_UTIL_WIN_ERROR_MESSAGE_H_INCLUDED
#define MEGOPP_UTIL_WIN_ERROR_MESSAGE_H_INCLUDED

#include <mego/util/os/windows/windows_simplify.h>
#include <megopp/util/scope_cleanup.h>
#include <memepp/string.hpp>
#include <memepp/variable_buffer.hpp>
#include <megopp/predef/namespace_alias.h>


#include <ww898/utf_converters.hpp>
#include <ww898/cp_utf8.hpp>
#include <ww898/cp_utfw.hpp>

namespace mgpp {
namespace os {
namespace win {

	inline memepp::string errcode_as_string(uint32_t _errCode)
	{
#if MEGO_OS__WINDOWS__AVAILABLE
		if (_errCode == 0)
			return memepp::string{}; //No error message has been recorded

		LPWSTR messageBuffer = nullptr;
		size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, _errCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);
		MEGOPP_UTIL__ON_SCOPE_CLEANUP([&messageBuffer] { LocalFree(messageBuffer); });

		if (size == 0)
			return memepp::string{};

		memepp::variable_buffer u8buf;
		
		ww898::utf::conv<ww898::utf::utfw, ww898::utf::utf8>(
			messageBuffer, messageBuffer + size, std::back_inserter(u8buf));

		memepp::string u8msg;
        u8buf.release(u8msg);
		return u8msg;
#else
		return memepp::string{};
#endif
	}

	inline std::wstring errcode_as_u16string(uint32_t _errCode)
	{
#if MEGO_OS__WINDOWS__AVAILABLE
		if (_errCode == 0)
			return std::wstring{}; //No error message has been recorded

		LPWSTR messageBuffer = nullptr;
		size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, _errCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);
		MEGOPP_UTIL__ON_SCOPE_CLEANUP([&messageBuffer] { LocalFree(messageBuffer); });

		if (size == 0)
			return std::wstring{};

		return std::wstring{ messageBuffer, size };
#else
		return std::wstring{};
#endif
	}

}; // namespace win
}; // namespace os
}; // namespace megopp

#endif // MEGOPP_UTIL_WIN_ERROR_MESSAGE_H_INCLUDED

