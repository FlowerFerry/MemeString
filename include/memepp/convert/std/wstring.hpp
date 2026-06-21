
#ifndef MEMEPP_CONVERT_STD_WSTRING_HPP_INCLUDED
#define MEMEPP_CONVERT_STD_WSTRING_HPP_INCLUDED

#include <mego/predef/lang/version.h>

#include "memepp/string.hpp"
#include "memepp/string_view.hpp"
#include "memepp/convert/common_def.hpp"
#include "meme/string_memory.h"

#include <string>
#if MG_LANG__CXX17_AVAIL
#  include <string_view>
#endif

namespace memepp {
	
	inline memepp::string from(const std::wstring& _s)
	{
#if MG_OS__WIN_AVAIL
		return memepp::string{
			reinterpret_cast<const uint16_t*>(_s.data()), static_cast<mmint_t>(_s.size()) };
#else
		/* Non-Windows: wchar_t is UTF-32. Convert each code unit to UTF-8
		   and build a memepp::string from the result. */
		if (_s.empty())
			return {};
		/* Pre-allocate enough space: worst case 6 bytes per code unit. */
		size_t maxBytes = _s.size() * 6;
		uint8_t* u8buf = (uint8_t*)mmmem_malloc(maxBytes);
		if (!u8buf)
			return {};
		size_t used = 0;
		for (size_t i = 0; i < _s.size() && used + 6 <= maxBytes; ++i) {
			uint32_t ch = static_cast<uint32_t>(_s[i]);
			int bytes = mmutf_u8rune_set_u32(u8buf + used, 6, ch);
			if (bytes > 0)
				used += (size_t)bytes;
		}
		memepp::string result{ u8buf, static_cast<mmint_t>(used) };
		mmmem_free(u8buf);
		return result;
#endif
    }

	inline memepp::string from(std::wstring&& _s)
	{
#if MG_OS__WIN_AVAIL
		return memepp::string{ 
			reinterpret_cast<const uint16_t*>(_s.data()), static_cast<mmint_t>(_s.size())};
#else
		/* Non-Windows: same UTF-32 → UTF-8 path as lvalue variant. */
		return memepp::from(const_cast<const std::wstring&>(_s));
#endif
	}
	
#if MG_LANG__CXX17_AVAIL
	inline memepp::string from(const std::wstring_view& _sv)
	{
#if MG_OS__WIN_AVAIL
		return memepp::string{
			reinterpret_cast<const uint16_t*>(_sv.data()), static_cast<mmint_t>(_sv.size()) };
#else
		return {};
#endif
	}
	
	inline memepp::string from(std::wstring_view&& _sv)
	{
#if MG_OS__WIN_AVAIL
		return memepp::string{
			reinterpret_cast<const uint16_t*>(_sv.data()), static_cast<mmint_t>(_sv.size()) };
#else
		return {};
#endif
	}
#endif

	template<>
	inline std::wstring to<std::wstring>(const memepp::string_view& _sv)
	{
#if MG_OS__WIN_AVAIL
		auto frontSize = _sv.u16char_size();
        std::wstring u16; u16.resize(frontSize);

        auto afterSize = MemeString_writeU16Chars(
            memepp::to_pointer(_sv.native_handle()), (uint16_t*)(u16.data()));

        if (afterSize != frontSize)
			u16.resize(afterSize);
        return u16;
#else
		/* Non-Windows: wchar_t is UTF-32. Convert UTF-8 → UTF-32 code units. */
		if (_sv.empty())
			return {};
		auto cnt = _sv.rune_size();
		std::wstring u32; u32.resize(cnt);
		size_t i = 0;
		for (auto it = _sv.rune_begin(); it != _sv.rune_end() && i < cnt; ++it, ++i) {
			auto idx = it.to_index();
			uint32_t cp;
			mmutf_u8rune_get_u32(idx.data(), idx.size(), &cp);
			u32[i] = static_cast<wchar_t>(cp);
		}
		if (i != cnt)
			u32.resize(i);
		return u32;
#endif
	}

	template<>
	inline std::wstring to<std::wstring>(const memepp::string& _s)
	{
		return to<std::wstring>(memepp::string_view{_s});
	}

}

inline memepp::string mm_from(const std::wstring& _s)
{
    return memepp::from(_s);
}

inline memepp::string mm_from(std::wstring&& _s)
{
    return memepp::from(std::move(_s));
}

#include <memepp/convert/common.hpp>

#endif // !MEMEPP_CONVERT_STD_WSTRING_HPP_INCLUDED
