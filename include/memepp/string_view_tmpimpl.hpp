
#ifndef MEME_STRING_VIEW_TEMPLATEIMPL_H_INCLUDED
#define MEME_STRING_VIEW_TEMPLATEIMPL_H_INCLUDED

#include <memepp/string_def.hpp>
#include <memepp/string_view_def.hpp>
#include <memepp/help/string_trim_if_helper.hpp>
#include <memepp/errc.hpp>

#include <vector>

namespace memepp {
namespace MMPP_NAMESPACE {

	template<template<class> class _Container, typename>
	inline MemeInteger_t string_view::split(
		string_view _key, split_behavior_t _behavior,
		std::back_insert_iterator<_Container<string>> _inserter) const
	{
		MemeStringStack_t stacks[4];
		MemeInteger_t stacksCount = 0;
		for (MemeInteger_t index = 0; index != -1;)
		{
			stacksCount = sizeof(stacks) / sizeof(stacks[0]);
			auto result = MemeString_split(to_pointer(native_handle()),
				_key.data(), _key.size(),
				static_cast<MemeFlag_SplitBehavior_t>(_behavior), MemeFlag_AllSensitive,
				stacks, &stacksCount, &index);
			if (result) {
				return result;
			}
			for (auto i = 0; i < stacksCount; ++i)
			{
				*_inserter++ = string(std::move(stacks[i]));
				MemeStringStack_unInit(stacks + i, MEME_STRING__OBJECT_SIZE);
			}
		}
		return 0;
	}

	template<template<class, class...> class _Container, class... _Arg, typename>
	inline MemeInteger_t string_view::split(
		string_view _key, split_behavior_t _behavior,
		std::back_insert_iterator<_Container<string, _Arg...>> _inserter) const
	{
		MemeStringStack_t stacks[4];
		MemeInteger_t stacksCount = 0;
		for (MemeInteger_t index = 0; index != -1;)
		{
			stacksCount = sizeof(stacks) / sizeof(stacks[0]);
			auto result = MemeString_split(to_pointer(native_handle()),
				_key.data(), _key.size(),
				static_cast<MemeFlag_SplitBehavior_t>(_behavior), MemeFlag_AllSensitive,
				stacks, &stacksCount, &index);
			if (result) {
				return result;
			}
			for (auto i = 0; i < stacksCount; ++i)
			{
				*_inserter++ = memepp::string(std::move(stacks[i]));
				MemeStringStack_unInit(stacks + i, MEME_STRING__OBJECT_SIZE);
			}
		}
		return 0;
	}

	template<template<class, class...> class _Container, class... _Arg, typename>
	inline mmint_t string_view::split(
		string_view _key, 
		std::back_insert_iterator<_Container<string, _Arg...>> _inserter) const
	{
        return split(_key, split_behav_t::keep_empty_parts, _inserter);
	}

	template<class _Container>
	inline MemeInteger_t string_view::split(string_view _key, split_behavior_t _behavior,
		std::back_insert_iterator<_Container> _inserter) const
	{
		MemeStringStack_t stacks[4];
		MemeInteger_t stacksCount = 0;
		for (MemeInteger_t index = 0; index != -1;)
		{
			stacksCount = sizeof(stacks) / sizeof(stacks[0]);
			auto result = MemeStringViewUnsafe_split(to_pointer(native_handle()),
				_key.data(), _key.size(),
				static_cast<MemeFlag_SplitBehavior_t>(_behavior), MemeFlag_AllSensitive,
				stacks, &stacksCount, &index);
			if (result) {
				return result;
			}
			for (auto i = 0; i < stacksCount; ++i)
			{
				*_inserter++ = typename _Container::value_type {
					MemeString_cStr(to_pointer(stacks[i])),
					static_cast<size_t>(MemeString_byteSize(to_pointer(stacks[i])))
				};
				MemeStringStack_unInit(stacks + i, MEME_STRING__OBJECT_SIZE);
			}
		}
		return 0;
	}

	//template<template<class, class...> class _Container, typename _Ty, class... _Arg>
	//inline MemeInteger_t string_view::split(
	//	string_view _key, split_behavior_t _behavior,
	//	std::back_insert_iterator<_Container<_Ty, _Arg...>> _inserter) const
	//{
	//	MemeStringStack_t stacks[4];
	//	MemeInteger_t stacksCount = 0;
	//	for (MemeInteger_t index = 0; index != -1;)
	//	{
	//		stacksCount = sizeof(stacks) / sizeof(stacks[0]);
	//		auto result = MemeStringViewUnsafe_split(to_pointer(native_handle()),
	//			_key.data(), _key.size(),
	//			static_cast<MemeFlag_SplitBehavior_t>(_behavior), MemeFlag_AllSensitive,
	//			stacks, &stacksCount, &index);
	//		if (result) {
	//			return result;
	//		}
	//		for (auto i = 0; i < stacksCount; ++i)
	//		{
	//			*_inserter++ = _Ty{
	//				MemeString_cStr(to_pointer(stacks[i])),
	//				static_cast<size_t>(MemeString_byteSize(to_pointer(stacks[i])))
	//			};
	//			MemeStringStack_unInit(stacks + i, MEME_STRING__OBJECT_SIZE);
	//		}
	//	}
	//	return 0;
	//}

	template<template<class> class _Container, typename>
	inline MemeInteger_t string_view::split(
		string_view _key, split_behavior_t _behavior,
		std::back_insert_iterator<_Container<string_view>> _inserter) const
	{
		MemeStringStack_t stacks[4];
		MemeInteger_t stacksCount = 0;
		for (MemeInteger_t index = 0; index != -1;)
		{
			stacksCount = sizeof(stacks) / sizeof(stacks[0]);
			auto result = MemeStringViewUnsafe_split(to_pointer(native_handle()),
				_key.data(), _key.size(),
				static_cast<MemeFlag_SplitBehavior_t>(_behavior), MemeFlag_AllSensitive,
				stacks, &stacksCount, &index);
			if (result) {
				return result;
			}
			for (auto i = 0; i < stacksCount; ++i)
			{
				*_inserter++ = memepp::string_view(stacks[i]);
				MemeStringStack_unInit(stacks + i, MEME_STRING__OBJECT_SIZE);
			}
		}
		return 0;
	}

	template<template<class, class...> class _Container, class... _Arg, typename>
	inline MemeInteger_t string_view::split(
		string_view _key, split_behavior_t _behavior,
		std::back_insert_iterator<_Container<string_view, _Arg...>> _inserter) const MEGOPP__NOEXCEPT
	{
		MemeStringStack_t stacks[4];
		MemeInteger_t stacksCount = 0;
		for (MemeInteger_t index = 0; index != -1;)
		{
			stacksCount = sizeof(stacks) / sizeof(stacks[0]);
			auto result = MemeStringViewUnsafe_split(to_pointer(native_handle()),
				_key.data(), _key.size(),
				static_cast<MemeFlag_SplitBehavior_t>(_behavior), MemeFlag_AllSensitive,
				stacks, &stacksCount, &index);
			if (result) {
				return result;
			}
			for (auto i = 0; i < stacksCount; ++i)
			{
				*_inserter++ = memepp::string_view(stacks[i]);
				MemeStringStack_unInit(stacks + i, MEME_STRING__OBJECT_SIZE);
			}
		}
		return 0;
	}
	
	template<template<class, class...> class _Container, class... _Arg, typename>
	inline mmint_t string_view::split(
		string_view _key, 
		std::back_insert_iterator<_Container<string_view, _Arg...>> _inserter) const MEGOPP__NOEXCEPT
	{
		return split(_key, split_behav_t::keep_empty_parts, _inserter);
	}

	// ---- trim_if (byte predicate, returns string_view) ----

	template<typename _Func>
	inline string_view string_view::trim_if(_Func&& _func) const noexcept
	{
		__string_trim_if_byte_helper<_Func> helper{ std::forward<_Func>(_func) };
		mmstrstk_t out;
		mgec_t ec = *errc() = MemeStringStack_trimByCondByteFunc_v2(
			&native_handle(),
			__string_trim_if_byte_helper<_Func>::callback,
			&helper, &out, sizeof(out));
		if (ec) {
			mmstrstk_uninit(&out);
			return string_view{};
		}
		string_view result{ out };
		mmstrstk_uninit(&out);
		return result;
	}

	// ---- trim_if_rune (rune predicate, returns string_view) ----

	template<typename _Func>
	inline string_view string_view::trim_if_rune(_Func&& _func) const noexcept
	{
		__string_trim_if_rune_helper<_Func> helper{ std::forward<_Func>(_func) };
		mmstrstk_t out;
		mgec_t ec = *errc() = MemeStringStack_trimByCondRuneFunc(
			&native_handle(),
			__string_trim_if_rune_helper<_Func>::callback,
			&helper, &out, sizeof(out));
		if (ec) {
			mmstrstk_uninit(&out);
			return string_view{};
		}
		string_view result{ out };
		mmstrstk_uninit(&out);
		return result;
	}

	// ---- join (member method, *this is separator) ----

	namespace details {
		template<typename _Container>
		inline string join_view_sep_impl(const string_view& _sep, const _Container& _items)
		{
			std::vector<mmstrstk_t> raw;
			raw.reserve(_items.size());
			for (const auto& item : _items)
				raw.push_back(item.native_handle());
			mmstrstk_t out;
			mgec_t ec = *errc() = MemeStringStack_join(
				&out, sizeof(out),
				_sep.data(), _sep.size(),
				raw.data(), static_cast<mmint_t>(raw.size()));
			if (ec)
				mmstrstk_uninit(&out);
#if !MMOPT__EXCEPTION_DISABLED
			throw_errc(ec);
#endif
			return ec ? string{} : string{ std::move(out) };
		}
	}

	template<template<class> class _Container, typename>
	inline string string_view::join(const _Container<string>& _items) const
	{
		return details::join_view_sep_impl(*this, _items);
	}

	template<template<class, class...> class _Container, class... _Arg, typename>
	inline string string_view::join(const _Container<string, _Arg...>& _items) const
	{
		return details::join_view_sep_impl(*this, _items);
	}

	template<template<class> class _Container, typename>
	inline string string_view::join(const _Container<string_view>& _items) const
	{
		return details::join_view_sep_impl(*this, _items);
	}

	template<template<class, class...> class _Container, class... _Arg, typename>
	inline string string_view::join(const _Container<string_view, _Arg...>& _items) const
	{
		return details::join_view_sep_impl(*this, _items);
	}
} // namespace MMPP_NAMESPACE
}

#endif // !MEME_STRING_VIEW_TEMPLATEIMPL_H_INCLUDED
