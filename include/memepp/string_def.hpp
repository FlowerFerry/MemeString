
#ifndef MEMEPP_STRING_DEF_HPP_INCLUDED
#define MEMEPP_STRING_DEF_HPP_INCLUDED

#include "meme/string.h"
#include "memepp/word_fwd.hpp"
#include "memepp/string_fwd.hpp"
#include "memepp/buffer_fwd.hpp"
#include "memepp/string_view_def.hpp"

#include <iterator>

namespace memepp {

	class string final
	{
	public:
		using value_type = MemeByte_t;
		using size_type = MemeInteger_t;
		using difference_type = ptrdiff_t;
		using reference = value_type & ;
		using const_reference = const value_type&;
		using pointer = value_type * ;
		using const_pointer = const value_type*;

		using native_handle_type = MemeStringStack_t;

		static const size_type npos = static_cast<size_type>(-1);

		MEMEPP__IMPL_INLINE string() noexcept;
		MEMEPP__IMPL_INLINE string(native_handle_type&& _other);

		MEMEPP__IMPL_INLINE string(string&& _other);
		MEMEPP__IMPL_INLINE string(const string& _other);
		MEMEPP__IMPL_INLINE string(const string& _other, size_type _pos);
		MEMEPP__IMPL_INLINE string(const string& _other, size_type _pos, size_type _count);

		MEMEPP__IMPL_INLINE string(const char* _utf8);
		MEMEPP__IMPL_INLINE string(const char* _utf8, size_type _size);
		MEMEPP__IMPL_INLINE string(const_pointer _utf8);
		MEMEPP__IMPL_INLINE string(const_pointer _utf8, size_type _size);

		MEMEPP__IMPL_INLINE string(const word& _ch);
		MEMEPP__IMPL_INLINE string(size_type _count, char _ch);

		template< class InputIt >
		MEMEPP__IMPL_INLINE string(InputIt _first, InputIt _last);

		string(std::nullptr_t) = delete;

		MEMEPP__IMPL_INLINE ~string();

		MEMEPP__IMPL_INLINE string& operator=(string && _other);
		MEMEPP__IMPL_INLINE string& operator=(const string& _other);

		MEMEPP__IMPL_INLINE string_storage_type storage_type() const noexcept;

		MEMEPP__IMPL_INLINE const char* data() const noexcept;
		MEMEPP__IMPL_INLINE const char* c_str() const noexcept;
		MEMEPP__IMPL_INLINE const_pointer bytes() const noexcept;

		inline size_t length() const noexcept { return static_cast<size_t>(size()); }
		MEMEPP__IMPL_INLINE size_type size() const noexcept;
		MEMEPP__IMPL_INLINE bool empty() const noexcept;
		MEMEPP__IMPL_INLINE size_type max_size() const noexcept;
		MEMEPP__IMPL_INLINE size_type capacity() const noexcept;

		MEMEPP__IMPL_INLINE void swap(string& _other) noexcept;

		MEMEPP__IMPL_INLINE size_type index_of(const string& _other,
			MemeFlag_CaseSensitivity_t _cs = MemeFlag_AllSensitive) const noexcept;

		MEMEPP__IMPL_INLINE size_type index_of(const string_view& _other,
			MemeFlag_CaseSensitivity_t _cs = MemeFlag_AllSensitive) const noexcept;

		MEMEPP__IMPL_INLINE size_type index_of(const char* _utf8,
			MemeFlag_CaseSensitivity_t _cs = MemeFlag_AllSensitive) const noexcept;

		MEMEPP__IMPL_INLINE size_type index_of_with_strlen(const char* _utf8, size_type _utf8_len,
			MemeFlag_CaseSensitivity_t _cs = MemeFlag_AllSensitive) const noexcept;

		template<template<class> class _Container>
		inline MemeInteger_t split(string_view _key, split_behavior_t _behavior,
			std::back_insert_iterator<_Container<string>> _inserter) const;

		template<template<class> class _Container>
		inline MemeInteger_t split(string_view _key, split_behavior_t _behavior,
			std::back_insert_iterator<_Container<string_view>> _inserter) const;

		template<template<class, class...> class _Container, class... _Arg>
		inline MemeInteger_t split(
			string_view _key, split_behavior_t _behavior,
			std::back_insert_iterator<_Container<string, _Arg...>> _inserter) const;

		template<template<class, class...> class _Container, class... _Arg>
		inline MemeInteger_t split(
			string_view _key, split_behavior_t _behavior,
			std::back_insert_iterator<_Container<string_view, _Arg...>> _inserter) const;

		MEMEPP__IMPL_INLINE const native_handle_type& native_handle() const noexcept;

	private:
		native_handle_type data_;
	};

	MEMEPP__IMPL_INLINE bool operator==(const string& _lhs, const string& _rhs);
	MEMEPP__IMPL_INLINE bool operator==(const char* _lhs, const string& _rhs);
	MEMEPP__IMPL_INLINE bool operator==(const string& _lhs, const char* _rhs);

	MEMEPP__IMPL_INLINE bool operator!=(const string& _lhs, const string& _rhs);
	MEMEPP__IMPL_INLINE bool operator!=(const char* _lhs, const string& _rhs);
	MEMEPP__IMPL_INLINE bool operator!=(const string& _lhs, const char* _rhs);

	MEMEPP__IMPL_INLINE memepp::string from_hexadecimals(const uint8_t* _buf, size_t _len);

	template<template<class> class _Container>
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

	//template<template<class> class _Container>
	//inline MemeInteger_t string::split(
	//	string_view _key, split_behavior_t _behavior,
	//	std::back_insert_iterator<_Container<string_view>> _inserter) const
	//{
	//	MemeStringStack_t stacks[4];
	//	MemeInteger_t stacksCount = 0;
	//	for (MemeInteger_t index = 0; index != -1;)
	//	{
	//		stacksCount = sizeof(stacks) / sizeof(stacks[0]);
	//		auto result = MemeString_split(to_pointer(native_handle()),
	//			_key.data(), _key.size(),
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

	template<template<class, class...> class _Container, class... _Arg>
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

	//template<template<class, class...> class _Container, class... _Arg>
	//inline MemeInteger_t string::split(
	//	string_view _key, split_behavior_t _behavior,
	//	std::back_insert_iterator<_Container<string_view, _Arg...>> _inserter) const
	//{
	//	MemeStringStack_t stacks[4];
	//	MemeInteger_t stacksCount = 0;
	//	for (MemeInteger_t index = 0; index != -1;)
	//	{
	//		stacksCount = sizeof(stacks) / sizeof(stacks[0]);
	//		auto result = MemeString_split(to_pointer(native_handle()),
	//			_key.data(), _key.size(),
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

}; // namespace memepp


	memepp::string mm_from(const char* _str, size_t _len);
	memepp::string mm_from(const MemeByte_t* _str, size_t _len);

	memepp::string operator""_meme(const char* _str, size_t _len);


#endif // !MEMEPP_STRING_DEF_HPP_INCLUDED
