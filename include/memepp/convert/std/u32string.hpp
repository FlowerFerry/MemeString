
#ifndef MEMEPP_CONVERT_STD_U32STRING_HPP_INCLUDED
#define MEMEPP_CONVERT_STD_U32STRING_HPP_INCLUDED

#include "memepp/string.hpp"
#include "memepp/string_view.hpp"
#include "memepp/convert/common_def.hpp"
#include "meme/string_memory.h"

#include <meme/utf/u32rune.h>
#include <meme/utf/u8rune.h>

#include <string>

namespace memepp  {
namespace convert {

	template<>
	struct from<::std::u32string>
	{
		static memepp::string from_object(const ::std::u32string& _s)
		{
			/* wchar_t/char32_t is UTF-32 here: convert each code point to UTF-8.
			   The library's UTF-8 side accepts up to 6 bytes (31-bit code points),
			   so no value loss occurs for any char32_t input. */
			if (_s.empty())
				return {};

			/* Worst case 6 UTF-8 bytes per code unit. */
			const size_t maxBytes = _s.size() * 6;
			uint8_t* buf = static_cast<uint8_t*>(mmmem_malloc(maxBytes));
			if (!buf)
				return {};

			size_t used = 0;
			for (size_t i = 0; i < _s.size() && used + 6 <= maxBytes; ++i) {
				const uint32_t cp = static_cast<uint32_t>(_s[i]);
				const int n = mmutf_u8rune_set_u32(buf + used, 6, cp);
				if (n > 0)
					used += static_cast<size_t>(n);
			}

			memepp::string out{ buf, static_cast<MemeInteger_t>(used) };
			mmmem_free(buf);
			return out;
		}

		static memepp::string from_object(::std::u32string&& _s)
		{
			auto out = from_object(static_cast<const ::std::u32string&>(_s));
			/* Consistent with from(std::wstring&&) / from(std::u16string&&). */
			_s.clear();
			return out;
		}
	};

	template<>
	struct into<::std::u32string>
	{
		static ::std::u32string into_object(const memepp::string& _s)
		{
			return into_object(memepp::string_view{ _s });
		}

		static ::std::u32string into_object(const memepp::string_view& _sv)
		{
			/* Walk the bytes with the strict decoder rather than
			   rune_begin/rune_end: rune_size() applies loose validation (lead
			   byte only) and would size the buffer wrong for malformed input.

			   An overlong encoding of U+0000 (C0 80, E0 80 80, F0 80 80 80,
			   F8 80 80 80 80, FC 80 80 80 80 80) becomes one U+FFFD per source
			   byte, matching mmutf_default_convert_u8to16 and what
			   to<std::wstring> produces. */
			const MemeByte_t* p   = reinterpret_cast<const MemeByte_t*>(_sv.data());
			MemeInteger_t     len = static_cast<MemeInteger_t>(_sv.size());

			::std::u32string out;
			while (len > 0) {
				uint32_t cp = 0;
				const int srcSize = mmutf_u8rune_get_u32(p, len, &cp);
				if (srcSize == -1)
					break;                  /* undecodable byte: truncate */

				if (cp == 0 && srcSize > 1) {
					for (int k = 0; k < srcSize; ++k)
						out.push_back(static_cast<char32_t>(0xFFFD));
				}
				else {
					out.push_back(static_cast<char32_t>(cp));
				}

				p   += srcSize;
				len -= srcSize;
			}
			return out;
		}
	};

}

	inline memepp::string from(const std::u32string& _s)
	{
		return convert::from<::std::u32string>::from_object(_s);
	}

	inline memepp::string from(std::u32string&& _s)
	{
		return convert::from<::std::u32string>::from_object(std::move(_s));
	}

	template<>
	inline std::u32string to<std::u32string>(const memepp::string& _s)
	{
		return convert::into<::std::u32string>::into_object(_s);
	}

	template<>
	inline std::u32string to<std::u32string>(const memepp::string_view& _sv)
	{
		return convert::into<::std::u32string>::into_object(_sv);
	}

}

inline memepp::string mm_from(const std::u32string& _s)
{
	return memepp::from(_s);
}

inline memepp::string mm_from(std::u32string&& _s)
{
	return memepp::from(std::move(_s));
}

#include <memepp/convert/common.hpp>

#endif // !MEMEPP_CONVERT_STD_U32STRING_HPP_INCLUDED
