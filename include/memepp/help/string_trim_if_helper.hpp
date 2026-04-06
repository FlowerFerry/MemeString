#ifndef MEMEPP_STRING_TRIM_IF_HELPER_HPP_INCLUDED
#define MEMEPP_STRING_TRIM_IF_HELPER_HPP_INCLUDED

#include <mego/predef/lang/version.h>
#include <meme/string.h>
#include <memepp/rune_impl.hpp>

namespace memepp {

	// ---- trim_if (byte predicate) ----

#if MG_LANG__CXX17_AVAIL
	template<typename _Func, typename = std::enable_if_t<std::is_invocable_v<_Func, mmbyte_t>>>
#else
	template<typename _Func>
#endif
	struct __string_trim_if_byte_helper
	{
		inline __string_trim_if_byte_helper(_Func&& _func) : func_(std::forward<_Func>(_func)) {}

		inline static int callback(MemeByte_t _ch, void* _user_data)
		{
			auto* h = reinterpret_cast<__string_trim_if_byte_helper*>(_user_data);
			return h->func_(_ch) ? 1 : 0;
		}
		_Func func_;
	};

	// ---- trim_if_rune (rune predicate) ----

#if MG_LANG__CXX17_AVAIL
	template<typename _Func, typename = std::enable_if_t<std::is_invocable_v<_Func, const memepp::rune&>>>
#else
	template<typename _Func>
#endif
	struct __string_trim_if_rune_helper
	{
		inline __string_trim_if_rune_helper(_Func&& _func) : func_(std::forward<_Func>(_func)) {}

		inline static int callback(const MemeRune_t* _ch, void* _user_data)
		{
			auto* h = reinterpret_cast<__string_trim_if_rune_helper*>(_user_data);
			memepp::rune r{ *_ch };
			return h->func_(r) ? 1 : 0;
		}
		_Func func_;
	};

} // namespace memepp

#endif // !MEMEPP_STRING_TRIM_IF_HELPER_HPP_INCLUDED
