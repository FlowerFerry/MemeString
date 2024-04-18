
#ifndef MEME_STRING_VIEW_TEMPLATEIMPL_H_INCLUDED
#define MEME_STRING_VIEW_TEMPLATEIMPL_H_INCLUDED

#include <memepp/string_def.hpp>
#include <memepp/string_view_def.hpp>

namespace memepp {


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
		std::back_insert_iterator<_Container<string_view, _Arg...>> _inserter) const
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
}

#endif // !MEME_STRING_VIEW_TEMPLATEIMPL_H_INCLUDED
