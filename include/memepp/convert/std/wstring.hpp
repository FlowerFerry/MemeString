
#ifndef MEMEPP_CONVERT_STD_WSTRING_HPP_INCLUDED
#define MEMEPP_CONVERT_STD_WSTRING_HPP_INCLUDED

#include "memepp/string.hpp"
#include "memepp/string_view.hpp"
#include "memepp/convert/common_def.hpp"

#include <string>

namespace memepp {
	
	inline memepp::string from(const std::wstring& _s)
	{
#if MG_OS__WIN_AVAIL
		return memepp::string{
			reinterpret_cast<const uint16_t*>(_s.data()), static_cast<mmint_t>(_s.size()) };
#else
        return {};
        // return memepp::string{
        //     reinterpret_cast<const uint32_t*>(_s.data()), static_cast<mmint_t>(_s.size()) };
#endif
    }

	inline memepp::string from(std::wstring&& _s)
	{
#if MG_OS__WIN_AVAIL
		return memepp::string{ 
			reinterpret_cast<const uint16_t*>(_s.data()), static_cast<mmint_t>(_s.size())};
#else
        return {};
        // return memepp::string{
        //     reinterpret_cast<const uint32_t*>(_s.data()), static_cast<mmint_t>(_s.size()) };
#endif
	}
	
	template<>
	inline std::wstring to<std::wstring>(const memepp::string& _s)
	{
#if MG_OS__WIN_AVAIL
		auto frontSize = _s.u16char_size();
        std::wstring u16; u16.resize(frontSize);
		
        auto afterSize = MemeString_writeU16Chars(
			memepp::to_pointer(_s.native_handle()), (uint16_t*)(u16.data()));

		if (afterSize != frontSize)
			u16.resize(afterSize);
        return u16;
#else
        return {};
#endif
	}

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
        return {};
#endif
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
