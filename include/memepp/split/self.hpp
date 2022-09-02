
#ifndef MEMEPP_SPLIT_SELF_HPP_INCLUDED
#define MEMEPP_SPLIT_SELF_HPP_INCLUDED

#include "memepp/string_def.hpp"
#include "memepp/string_view_def.hpp"

#include <iterator>

namespace memepp {


	template<template<class> class _List>
	inline MemeInteger_t split(const memepp::string& _source,
		const char* _key, MemeInteger_t _key_len, split_behavior_t _behavior,
		std::back_insert_iterator<_List<memepp::string>> _inserter)
	{
		MemeStringStack_t stacks[4];
		MemeInteger_t stacksCount = 0;
		for (MemeInteger_t index = 0; index != -1;)
		{
			stacksCount = sizeof(stacks) / sizeof(stacks[0]);
			auto result = MemeString_split(to_pointer(_source.native_handle()),
				_key, _key_len,
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

	template<template<class, class...> class _List, class... _Arg>
	inline MemeInteger_t split(const memepp::string& _source,
		const char* _key, MemeInteger_t _key_len, split_behavior_t _behavior,
		std::back_insert_iterator<_List<memepp::string, _Arg...>> _inserter)
	{
		MemeStringStack_t stacks[4];
		MemeInteger_t stacksCount = 0;
		for (MemeInteger_t index = 0; index != -1;)
		{
			stacksCount = sizeof(stacks) / sizeof(stacks[0]);
			auto result = MemeString_split(to_pointer(_source.native_handle()),
				_key, _key_len, 
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

	//template<template<class> class _List>
	//inline MemeInteger_t split(const memepp::string_view& _source,
	//	const char* _key, MemeInteger_t _key_len, split_behavior_t _behavior,
	//	std::back_insert_iterator<_List<memepp::string_view>> _inserter)
	//{
	//	MemeStringStack_t stacks[4];
	//	MemeInteger_t stacksCount = 0;
	//	for (MemeInteger_t index = 0; index != -1;)
	//	{
	//		stacksCount = sizeof(stacks) / sizeof(stacks[0]);
	//		auto result = MemeString_split(to_pointer(_source.native_handle()),
	//			_key, _key_len,
	//			static_cast<MemeFlag_SplitBehavior_t>(_behavior), MemeFlag_AllSensitive,
	//			stacks, &stacksCount, &index);
	//		if (result) {
	//			return result;
	//		}
	//		for (auto i = 0; i < stacksCount; ++i)
	//		{
	//			*_inserter++ = memepp::string_view(std::move(stacks[i]));
	//			MemeStringStack_unInit(stacks + i, MEME_STRING__OBJECT_SIZE);
	//		}
	//	}
	//	return 0;
	//}

	//template<template<class, class...> class _List, class... _Arg>
	//inline MemeInteger_t split(const memepp::string_view& _source,
	//	const char* _key, MemeInteger_t _key_len, split_behavior_t _behavior,
	//	std::back_insert_iterator<_List<memepp::string_view, _Arg...>> _inserter)
	//{
	//	MemeStringStack_t stacks[4];
	//	MemeInteger_t stacksCount = 0;
	//	for (MemeInteger_t index = 0; index != -1;)
	//	{
	//		stacksCount = sizeof(stacks) / sizeof(stacks[0]);
	//		auto result = MemeString_split(to_pointer(_source.native_handle()),
	//			_key, _key_len,
	//			static_cast<MemeFlag_SplitBehavior_t>(_behavior), MemeFlag_AllSensitive,
	//			stacks, &stacksCount, &index);
	//		if (result) {
	//			return result;
	//		}
	//		for (auto i = 0; i < stacksCount; ++i)
	//		{
	//			*_inserter++ = memepp::string_view(std::move(stacks[i]));
	//			MemeStringStack_unInit(stacks + i, MEME_STRING__OBJECT_SIZE);
	//		}
	//	}
	//	return 0;
	//}

	template<template<class> class _List>
	inline MemeInteger_t split(const memepp::string& _source,
		const char* _key, split_behavior_t _behavior,
		std::back_insert_iterator<_List<memepp::string>> _inserter)
	{
		return split(_source, _key, -1, _behavior, _inserter);
	}

	template<template<class, class...> class _List, class... _Arg>
	inline MemeInteger_t split(const memepp::string& _source,
		const char* _key, split_behavior_t _behavior,
		std::back_insert_iterator<_List<memepp::string, _Arg...>> _inserter)
	{
		return split(_source, _key, -1, _behavior, _inserter);
	}

	//template<template<class> class _List>
	//inline MemeInteger_t split(const memepp::string_view& _source,
	//	const char* _key, split_behavior_t _behavior,
	//	std::back_insert_iterator<_List<memepp::string_view>> _inserter)
	//{
	//	return split(_source, _key, -1, _behavior, _inserter);
	//}

	//template<template<class, class...> class _List, class... _Arg>
	//inline MemeInteger_t split(const memepp::string_view& _source,
	//	const char* _key, split_behavior_t _behavior,
	//	std::back_insert_iterator<_List<memepp::string_view, _Arg...>> _inserter)
	//{
	//	return split(_source, _key, -1, _behavior, _inserter);
	//}
};

#endif // !MEMEPP_SPLIT_SELF_HPP_INCLUDED
