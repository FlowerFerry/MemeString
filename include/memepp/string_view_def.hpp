
#ifndef MEMEPP_STRING_VIEW_DEF_HPP_INCLUDED
#define MEMEPP_STRING_VIEW_DEF_HPP_INCLUDED

#include "meme/string.h"
#include "meme/unsafe/string_view.h"
#include "memepp/string_fwd.hpp"
#include "memepp/string_view_fwd.hpp"
#include "memepp/string_builder_fwd.hpp"

#include "memepp/iterator.hpp"

namespace memepp {

	class string_view final
	{
        string_view& operator=(string&& _other) = delete;
        
	public:
		using value_type = MemeByte_t;
		using size_type = MemeInteger_t;
		using difference_type = ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		using native_handle_type = MemeStringStack_t;

		static const size_type npos = static_cast<size_type>(-1);

		MEMEPP__IMPL_INLINE string_view() noexcept;
		MEMEPP__IMPL_INLINE string_view(const MemeStringStack_t& _stack);
		MEMEPP__IMPL_INLINE string_view(const char* _utf8);
		MEMEPP__IMPL_INLINE string_view(const char* _utf8, size_type _size);
		MEMEPP__IMPL_INLINE string_view(const_pointer _utf8);
		MEMEPP__IMPL_INLINE string_view(const_pointer _utf8, size_type _size);

		MEMEPP__IMPL_INLINE string_view(const string& _other);
		MEMEPP__IMPL_INLINE string_view(string&& _other);
		MEMEPP__IMPL_INLINE string_view(const string_view& _other);
		MEMEPP__IMPL_INLINE string_view(string_view&& _other);

		MEMEPP__IMPL_INLINE ~string_view();

		MEMEPP__IMPL_INLINE string_storage_type storage_type() const noexcept;

		string_view& operator=(const string& _other);
		string_view& operator=(const string_view& _other);
		string_view& operator=(string_view&& _other);

		MEMEPP__IMPL_INLINE string_builder operator+(const string& _other) const;
		MEMEPP__IMPL_INLINE string_builder operator+(const string_view& _other) const;
		MEMEPP__IMPL_INLINE string_builder operator+(const char* _other) const;

		MEMEPP__IMPL_INLINE const char* data() const noexcept;
		MEMEPP__IMPL_INLINE size_type size() const noexcept;
		MEMEPP__IMPL_INLINE bool empty() const noexcept;
		inline size_t length() const noexcept { return static_cast<size_t>(size()); }
		MEMEPP__IMPL_INLINE const_pointer bytes() const noexcept;

		MEMEPP__IMPL_INLINE const_iterator begin() const noexcept;
		MEMEPP__IMPL_INLINE const_iterator cbegin() const noexcept;
        
		MEMEPP__IMPL_INLINE const_iterator end() const noexcept;
		MEMEPP__IMPL_INLINE const_iterator cend() const noexcept;

		MEMEPP__IMPL_INLINE string to_string() const;
        MEMEPP__IMPL_INLINE string to_large () const noexcept;

        MEMEPP__IMPL_INLINE size_type find(const string_view& _other, size_type _pos = 0) const noexcept;
        MEMEPP__IMPL_INLINE size_type find(const char* _utf8, size_type _pos = 0) const noexcept;
        MEMEPP__IMPL_INLINE size_type find(const char* _utf8, size_type _pos, size_type _count) const noexcept;
        MEMEPP__IMPL_INLINE size_type find(const_pointer _utf8, size_type _pos = 0) const noexcept;
        MEMEPP__IMPL_INLINE size_type find(const_pointer _utf8, size_type _pos, size_type _count) const noexcept;
		MEMEPP__IMPL_INLINE size_type find(char _ch, size_type _pos = 0) const noexcept;
		//MEMEPP__IMPL_INLINE size_type find(const word& _ch, size_type _pos = 0) const noexcept;

		MEMEPP__IMPL_INLINE size_type index_of(const string_view& _other,
			case_sensitivity_t _cs = case_sensitivity_t::all_sensitive) const noexcept;

		MEMEPP__IMPL_INLINE size_type index_of(const char* _utf8,
			case_sensitivity_t _cs = case_sensitivity_t::all_sensitive) const noexcept;

		MEMEPP__IMPL_INLINE size_type index_of_with_offset(const char* _utf8, size_type _offset,
			case_sensitivity_t _cs = case_sensitivity_t::all_sensitive) const noexcept;

		MEMEPP__IMPL_INLINE bool contains(const string_view& _sv) const noexcept;
		MEMEPP__IMPL_INLINE bool contains(const char* _utf8) const noexcept;
		MEMEPP__IMPL_INLINE bool contains(const char* _utf8, size_type _count) const noexcept;
		MEMEPP__IMPL_INLINE bool contains(const_pointer _utf8) const noexcept;
		MEMEPP__IMPL_INLINE bool contains(const_pointer _utf8, size_type _count) const noexcept;
		MEMEPP__IMPL_INLINE bool contains(char _ch) const noexcept;
		//MEMEPP__IMPL_INLINE bool contains(const word& _ch) const noexcept;

		MEMEPP__IMPL_INLINE bool starts_with(const string_view& _sv) const noexcept;
		MEMEPP__IMPL_INLINE bool starts_with(const char* _utf8) const noexcept;
		MEMEPP__IMPL_INLINE bool starts_with(const char* _utf8, size_type _count) const noexcept;
		MEMEPP__IMPL_INLINE bool starts_with(const_pointer _utf8) const noexcept;
		MEMEPP__IMPL_INLINE bool starts_with(const_pointer _utf8, size_type _count) const noexcept;
		//MEMEPP__IMPL_INLINE bool starts_with(char _ch) const noexcept;
		//MEMEPP__IMPL_INLINE bool starts_with(const word& _ch) const noexcept;

		MEMEPP__IMPL_INLINE bool ends_with(const string_view& _sv) const noexcept;
		MEMEPP__IMPL_INLINE bool ends_with(const char* _utf8) const noexcept;
		MEMEPP__IMPL_INLINE bool ends_with(const char* _utf8, size_type _count) const noexcept;
		MEMEPP__IMPL_INLINE bool ends_with(const_pointer _utf8) const noexcept;
		MEMEPP__IMPL_INLINE bool ends_with(const_pointer _utf8, size_type _count) const noexcept;
		//MEMEPP__IMPL_INLINE bool ends_with(char _ch) const noexcept;
		//MEMEPP__IMPL_INLINE bool ends_with(const word& _ch) const noexcept;

		MEMEPP__IMPL_INLINE string to_en_upper() const noexcept;
		MEMEPP__IMPL_INLINE string to_en_lower() const noexcept;

		MEMEPP__IMPL_INLINE string_view trim_space() const noexcept;
		MEMEPP__IMPL_INLINE string_view trim_left_space() const noexcept;
		MEMEPP__IMPL_INLINE string_view trim_right_space() const noexcept;

        MEMEPP__IMPL_INLINE string_view substr(size_type _pos = 0, size_type _count = npos) const noexcept;


		template<class _Container>
		inline MemeInteger_t split(string_view _key, split_behavior_t _behavior,
			std::back_insert_iterator<_Container> _inserter) const;
		
		template<class _Container>
		inline MemeInteger_t split(string_view _key, 
			std::back_insert_iterator<_Container> _inserter) const;

		//template<template<class, class...> class _Container, typename _Ty, class... _Arg>
		//inline MemeInteger_t split(
		//	string_view _key, split_behavior_t _behavior,
		//	std::back_insert_iterator<_Container<_Ty, _Arg...>> _inserter) const;
		//
		//template<template<class, class...> class _Container, typename _Ty, class... _Arg>
		//inline MemeInteger_t split(
		//	string_view _key, 
		//	std::back_insert_iterator<_Container<_Ty, _Arg...>> _inserter) const;

		template<template<class> class _Container>
		inline MemeInteger_t split(string_view _key, split_behavior_t _behavior,
			std::back_insert_iterator<_Container<string>> _inserter) const;
		
		template<template<class> class _Container>
		inline MemeInteger_t split(string_view _key, 
			std::back_insert_iterator<_Container<string>> _inserter) const;

		template<template<class> class _Container>
		inline MemeInteger_t split(string_view _key, split_behavior_t _behavior,
			std::back_insert_iterator<_Container<string_view>> _inserter) const;
		
		template<template<class> class _Container>
		inline MemeInteger_t split(string_view _key, 
			std::back_insert_iterator<_Container<string_view>> _inserter) const;

		template<template<class, class...> class _Container, class... _Arg>
		inline MemeInteger_t split(
			string_view _key, split_behavior_t _behavior,
			std::back_insert_iterator<_Container<string, _Arg...>> _inserter) const;
		
		template<template<class, class...> class _Container, class... _Arg>
		inline MemeInteger_t split(
			string_view _key, 
			std::back_insert_iterator<_Container<string, _Arg...>> _inserter) const;

		template<template<class, class...> class _Container, class... _Arg>
		inline MemeInteger_t split(
			string_view _key, split_behavior_t _behavior,
			std::back_insert_iterator<_Container<string_view, _Arg...>> _inserter) const;
		
		template<template<class, class...> class _Container, class... _Arg>
		inline MemeInteger_t split(
			string_view _key, 
			std::back_insert_iterator<_Container<string_view, _Arg...>> _inserter) const;

		MEMEPP__IMPL_INLINE const native_handle_type& native_handle() const noexcept;

	private:
		native_handle_type data_;
	};

	MEMEPP__IMPL_INLINE bool operator==(const string_view& _lhs, const string_view& _rhs);
	MEMEPP__IMPL_INLINE bool operator==(const string& _lhs, const string_view& _rhs);
	MEMEPP__IMPL_INLINE bool operator==(const string_view& _lhs, const string& _rhs);
	MEMEPP__IMPL_INLINE bool operator==(const char* _lhs, const string_view& _rhs);
	MEMEPP__IMPL_INLINE bool operator==(const string_view& _lhs, const char* _rhs);
    
	MEMEPP__IMPL_INLINE bool operator!=(const string_view& _lhs, const string_view& _rhs);
	MEMEPP__IMPL_INLINE bool operator!=(const string& _lhs, const string_view& _rhs);
	MEMEPP__IMPL_INLINE bool operator!=(const string_view& _lhs, const string& _rhs);
	MEMEPP__IMPL_INLINE bool operator!=(const char* _lhs, const string_view& _rhs);
	MEMEPP__IMPL_INLINE bool operator!=(const string_view& _lhs, const char* _rhs);
    
    //MEMEPP__IMPL_INLINE bool operator<(const string_view& _lhs, const string_view& _rhs);
    //MEMEPP__IMPL_INLINE bool operator<=(const string_view& _lhs, const string_view& _rhs);
    //MEMEPP__IMPL_INLINE bool operator>(const string_view& _lhs, const string_view& _rhs);
    //MEMEPP__IMPL_INLINE bool operator>=(const string_view& _lhs, const string_view& _rhs);

    
};

	memepp::string_view mm_view(const char* _str, size_t _len);
	memepp::string_view mm_view(const MemeByte_t* _str, size_t _len);

	memepp::string_view operator""_meme_sv(const char* _str, size_t _len);


#endif // !MEMEPP_STRING_VIEW_DEF_HPP_INCLUDED

#include <memepp/string_view_templateimpl.hpp>
    