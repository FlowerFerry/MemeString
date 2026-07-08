
#ifndef MEME_STRING_TEMPLATEIMPL_H_INCLUDED
#define MEME_STRING_TEMPLATEIMPL_H_INCLUDED

#include <mego/predef/lang/version.h>

#include <memepp/dll.hpp>
#include <memepp/errc.hpp>
#include <memepp/string_def.hpp>
#include <memepp/string_view_def.hpp>
#include <memepp/help/string_trim_if_helper.hpp>

#include <vector>
#include <type_traits>

namespace memepp {
inline namespace MMPP_NAMESPACE {

#if MG_LANG__CXX17_AVAIL
	template<typename _Func, typename = std::enable_if_t<std::is_invocable_v<_Func, memepp::rune&>>>
#else
    template<typename _Func>
#endif
	struct __string_mapping_convert_helper
	{
		inline __string_mapping_convert_helper(_Func&& _func) : func_(std::forward<_Func>(_func)) {}
		
		inline int call(memepp::rune& _ch)
		{
#if MG_LANG__CXX17_AVAIL
            if constexpr (std::is_same_v<std::invoke_result_t<_Func, memepp::rune&>, int>)
                return func_(_ch);
			else {
                func_(_ch);
                return 0;
			}
#else
			return call_wrap(_ch, 
				typename std::is_integral< std::result_of_t<_Func(memepp::rune&)> >::type{});
#endif
		}

		inline static int callback(MemeRune_t* _ch, void* _user_data)
		{
            auto helper = reinterpret_cast<__string_mapping_convert_helper*>(_user_data);
			memepp::rune r{ *_ch };
			auto result = helper->call(r);
            *_ch = r.native_handle();
            return result;
		}
		
	private:
#if !MG_LANG__CXX17_AVAIL
		inline int call_wrap(memepp::rune& _ch, std::true_type)
		{
            return func_(_ch);
		}
		
		inline int call_wrap(memepp::rune& _ch, std::false_type)
		{
            func_(_ch);
            return 0;
		}
#endif
		
        _Func func_;
	};

#if MG_LANG__CXX17_AVAIL
    template<typename _Func, typename = std::enable_if_t<std::is_invocable_v<_Func, const memepp::rune&>>>
#else
    template<typename _Func>
#endif
	struct __string_foreach_helper
	{
        inline __string_foreach_helper(_Func&& _func) : func_(std::forward<_Func>(_func)) {}

		inline mmflag_cbproc_t call(const memepp::rune& _ch)
		{
#if MG_LANG__CXX17_AVAIL
            if constexpr (std::is_same_v<std::invoke_result_t<_Func, const memepp::rune&>, mmflag_cbproc_t>)
                return func_(_ch);
			else {
                func_(_ch);
                return mmflag_cbproc_continue;
			}
#else
            return call_wrap(_ch,
                typename std::is_same<std::result_of_t<_Func(const memepp::rune&)>, mmflag_cbproc_t>::type{});
#endif
		}

        inline static mmflag_cbproc_t callback(const mmrune_t* _ch, void* _user_data)
        {
            auto helper = reinterpret_cast<__string_foreach_helper*>(_user_data);
            auto result = helper->call({ *_ch });
            return result;
        }

	private:
#if !MG_LANG__CXX17_AVAIL
        inline mmflag_cbproc_t call_wrap(const memepp::rune& _ch, std::true_type)
        {
            return func_(_ch);
        }

        inline mmflag_cbproc_t call_wrap(const memepp::rune& _ch, std::false_type)
        {
            func_(_ch);
            return mmflag_cbproc_continue;
        }
#endif
		
        _Func func_;
	};

	template<typename _Func>
	inline string string::mapping_convert(_Func&& _func) const
	{
		__string_mapping_convert_helper<_Func> helper{std::forward<_Func>(_func)};
		mmstrstk_t out;
		mgec_t ec = *errc() = MemeStringStack_mappingConvert_v2(
			&native_handle(),
			__string_mapping_convert_helper<_Func>::callback,
			&helper,
			&out, sizeof(out));
		if (ec)
			mmstrstk_uninit(&out);
#if !MMOPT__EXCEPTION_DISABLED
		throw_errc(ec);
#endif
		return ec ? string{} : string{ std::move(out) };
	}

	template<typename _Func>
	inline string::size_type string::rune_foreach(_Func&& _func) const noexcept
	{
        __string_foreach_helper<_Func> helper{ std::forward<_Func>(_func) };
        return MemeString_foreach(
			to_pointer(native_handle()), 
			__string_foreach_helper<_Func>::callback, 
			&helper);
	}

	template<class _Container>
	inline MemeInteger_t string::split(string_view _key, split_behavior_t _behavior,
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
				*_inserter++ = typename _Container::value_type{
					MemeString_cStr(to_pointer(stacks[i])),
					static_cast<size_t>(MemeString_byteSize(to_pointer(stacks[i])))
				};
				MemeStringStack_unInit(stacks + i, MEME_STRING__OBJECT_SIZE);
			}
		}
		return 0;
	}

	template<template<class> class _Container, typename>
	inline MemeInteger_t string::split(
		string_view _key, split_behavior_t _behavior,
		std::back_insert_iterator<_Container<string>> _inserter) const
	{
		MemeStringStack_t stacks[4];
		MemeInteger_t stacksCount = 0;
		for (MemeInteger_t index = 0; index != -1;)
		{
			stacksCount = sizeof(stacks) / sizeof(stacks[0]);
			auto result = MemeStringStack_split(&native_handle(),
				_key.data(), _key.size(),
				static_cast<MemeFlag_SplitBehavior_t>(_behavior), MemeFlag_AllSensitive,
				stacks, MEME_STRING__OBJECT_SIZE, &stacksCount, &index);
			if (result) {
				return result;
			}
			for (auto i = 0; i < stacksCount; ++i)
			{
				*_inserter++ = string(std::move(stacks[i]));
				// MemeStringStack_unInit(stacks + i, MEME_STRING__OBJECT_SIZE);
			}
		}
		return 0;
	}

	template<template<class> class _Container, typename>
	inline MemeInteger_t string::split(
		string_view _key, split_behavior_t _behavior,
		std::back_insert_iterator<_Container<string_view>> _inserter) const MEGOPP__NOEXCEPT
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
	inline MemeInteger_t string::split(
		string_view _key, split_behavior_t _behavior,
		std::back_insert_iterator<_Container<string, _Arg...>> _inserter) const
	{
		MemeStringStack_t stacks[4];
		MemeInteger_t stacksCount = 0;
		for (MemeInteger_t index = 0; index != -1;)
		{
			stacksCount = sizeof(stacks) / sizeof(stacks[0]);
			auto result = MemeStringStack_split(&native_handle(),
				_key.data(), _key.size(),
				static_cast<MemeFlag_SplitBehavior_t>(_behavior), MemeFlag_AllSensitive,
				stacks, MEME_STRING__OBJECT_SIZE, &stacksCount, &index);
			if (result) {
				return result;
			}
			for (auto i = 0; i < stacksCount; ++i)
			{
				*_inserter++ = memepp::string(std::move(stacks[i]));
				// MemeStringStack_unInit(stacks + i, MEME_STRING__OBJECT_SIZE);
			}
		}
		return 0;
	}

	template<template<class, class...> class _Container, class... _Arg, typename>
	inline mmint_t string::split(
		string_view _key,
		std::back_insert_iterator<_Container<string, _Arg...>> _inserter) const
	{
        return split(_key, split_behavior_t::keep_empty_parts, _inserter);
	}

	template<template<class, class...> class _Container, class... _Arg, typename>
	inline MemeInteger_t string::split(
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
	inline mmint_t string::split(
		string_view _key,
		std::back_insert_iterator<_Container<string_view, _Arg...>> _inserter) const MEGOPP__NOEXCEPT
	{
        return split(_key, split_behav_t::keep_empty_parts, _inserter);
	}

	// ---- trim_if (byte predicate) ----

	template<typename _Func>
	inline string string::trim_if(_Func&& _func) const
	{
		__string_trim_if_byte_helper<_Func> helper{ std::forward<_Func>(_func) };
		mmstrstk_t out;
		mgec_t ec = *errc() = MemeStringStack_trimByCondByteFunc_v2(
			&native_handle(),
			__string_trim_if_byte_helper<_Func>::callback,
			&helper, &out, sizeof(out));
		// if (ec)
		// 	mmstrstk_uninit(&out);
#if !MMOPT__EXCEPTION_DISABLED
		throw_errc(ec);
#endif
		return ec ? string{} : string{ std::move(out) };
	}

	// ---- trim_if_rune (rune predicate) ----

	template<typename _Func>
	inline string string::trim_if_rune(_Func&& _func) const
	{
		__string_trim_if_rune_helper<_Func> helper{ std::forward<_Func>(_func) };
		mmstrstk_t out;
		mgec_t ec = *errc() = MemeStringStack_trimByCondRuneFunc(
			&native_handle(),
			__string_trim_if_rune_helper<_Func>::callback,
			&helper, &out, sizeof(out));
		// if (ec)
		// 	mmstrstk_uninit(&out);
#if !MMOPT__EXCEPTION_DISABLED
		throw_errc(ec);
#endif
		return ec ? string{} : string{ std::move(out) };
	}

	// ---- join (member method, *this is separator) ----

	namespace details {
		template<typename _Container>
		inline string join_impl(const string& _sep, const _Container& _items)
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

		template<typename _Container>
		inline string join_view_impl(const string& _sep, const _Container& _items)
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
	inline string string::join(const _Container<string>& _items) const
	{
		return details::join_impl(*this, _items);
	}

	template<template<class, class...> class _Container, class... _Arg, typename>
	inline string string::join(const _Container<string, _Arg...>& _items) const
	{
		return details::join_impl(*this, _items);
	}

	template<template<class> class _Container, typename>
	inline string string::join(const _Container<string_view>& _items) const
	{
		return details::join_view_impl(*this, _items);
	}

	template<template<class, class...> class _Container, class... _Arg, typename>
	inline string string::join(const _Container<string_view, _Arg...>& _items) const
	{
		return details::join_view_impl(*this, _items);
	}
} // namespace MMPP_NAMESPACE

	template<>
	inline memepp::string import_from_dll(const mmstrstk_t& _obj, mmint_t _struct_size)
	{
		mmstrstk_t str;
        auto result = MemeStringStack_initByU8bytes(
			&str, _struct_size, MemeString_byteData(memepp::to_pointer(_obj)), MemeString_byteSize(memepp::to_pointer(_obj)));
        if (result)
			return {};
		
        return memepp::string{ std::move(str) };
	}

    template<>
	inline memepp::string import_from_dll(mmstrstk_t&& _obj, mmint_t _struct_size)
    {
		mmstrstk_t str;
		auto result = MemeStringStack_initByU8bytes(
			&str, _struct_size, MemeString_byteData(memepp::to_pointer(_obj)), MemeString_byteSize(memepp::to_pointer(_obj)));
		mmstrstk_uninit_v0(&_obj, _struct_size);
		if (result)
			return {};

        return memepp::string{ std::move(str) };
    }

	template<>
	inline mmstrstk_t export_into_dll(const memepp::string& _obj, mmint_t _struct_size)
	{
		mmstrstk_t s;
        auto result = mmstrstk_init_by_other_v0(&s, _struct_size, memepp::to_pointer(_obj.native_handle()));
		if (result) {
			mmstrstk_init_v0(&s, _struct_size);
		}
        return s;
	}

    template<>
	inline mmstrstk_t export_into_dll(memepp::string&& _obj, mmint_t _struct_size)
	{
		mmstrstk_t s;
		MemeStringStack_init(&s, _struct_size);
        MemeString_swap(memepp::to_pointer(s), const_cast<mmstr_ptr_t>(memepp::to_pointer(_obj.native_handle())));
		return s;
	}

}

#endif // !MEME_STRING_TEMPLATEIMPL_H_INCLUDED
