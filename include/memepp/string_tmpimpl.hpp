
#ifndef MEME_STRING_TEMPLATEIMPL_H_INCLUDED
#define MEME_STRING_TEMPLATEIMPL_H_INCLUDED

#include <memepp/dll.hpp>
#include <memepp/string_def.hpp>
#include <memepp/string_view_def.hpp>
#include <memepp/rune_impl.hpp>

#include <type_traits>

namespace memepp {

	template<typename _Func, typename = std::enable_if_t<std::is_invocable_v<_Func, memepp::rune&>>>
	struct __string_mapping_convert_helper
	{
		inline __string_mapping_convert_helper(_Func&& _func) : func_(std::forward<_Func>(_func)) {}
		
		inline int call(memepp::rune& _ch)
		{
            if constexpr (std::is_same_v<std::invoke_result_t<_Func, memepp::rune&>, int>)
                return func_(_ch);
			else {
                func_(_ch);
                return 0;
			}
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
        _Func func_;
	};

    template<typename _Func, typename = std::enable_if_t<std::is_invocable_v<_Func, const memepp::rune&>>>
	struct __string_foreach_helper
	{
        inline __string_foreach_helper(_Func&& _func) : func_(std::forward<_Func>(_func)) {}

		inline mmflag_cbproc_t call(const memepp::rune& _ch)
		{
            if constexpr (std::is_same_v<std::invoke_result_t<_Func, const memepp::rune&>, mmflag_cbproc_t>)
                return func_(_ch);
			else {
                func_(_ch);
                return mmflag_cbproc_continue;
			}
		}

        inline static mmflag_cbproc_t callback(const mmrune_t* _ch, void* _user_data)
        {
            auto helper = reinterpret_cast<__string_foreach_helper*>(_user_data);
            auto result = helper->call({ *_ch });
            return result;
        }

	private:
        _Func func_;
	};

	template<typename _Func>
	inline string string::mapping_convert(_Func&& _func) const
	{
		__string_mapping_convert_helper<_Func> helper{std::forward<_Func>(_func)};
        return MemeStringStack_mappingConvert(
			&native_handle(),
            MMS__OBJECT_SIZE, 
			__string_mapping_convert_helper<_Func>::callback,
			&helper);
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

	template<>
	inline memepp::string import_from_dll(const mmstrstk_t& _obj, mmint_t _struct_size)
	{
		mmstrstk_t str;
        auto result = MemeStringStack_initAndConditionalConvert(&str, _struct_size, memepp::to_pointer(_obj));
        if (result)
			return {};
		
        return memepp::string{ std::move(str) };
	}

    template<>
	inline memepp::string import_from_dll(mmstrstk_t&& _obj, mmint_t _struct_size)
    {
		mmstrstk_t str;
		auto result = MemeStringStack_initAndConditionalConvert(&str, _struct_size, memepp::to_pointer(_obj));
		mmstrstk_uninit(&_obj, _struct_size);
		if (result)
			return {};

        return memepp::string{ std::move(str) };
    }

	template<>
	inline mmstrstk_t export_into_dll(const memepp::string& _obj, mmint_t _struct_size)
	{
		mmstrstk_t s;
        mmstrstk_init_by_other(&s, _struct_size, memepp::to_pointer(_obj.native_handle()));
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
