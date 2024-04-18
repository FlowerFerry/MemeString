
#ifndef MEMEPP_STRING_VIEW_DEF_HPP_INCLUDED
#define MEMEPP_STRING_VIEW_DEF_HPP_INCLUDED

#include "meme/string.h"
#include "meme/unsafe/string_view.h"
#include "memepp/string_fwd.hpp"
#include "memepp/string_view_fwd.hpp"
#include "memepp/string_builder_fwd.hpp"

#include "memepp/iterator.hpp"
#include "memepp/rune_iterator.hpp"

#include <type_traits>

namespace memepp {

	class string_view
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

		string_view() noexcept;
		string_view(mmstr_const_t _other) noexcept;
		string_view(const MemeStringStack_t& _stack) noexcept;
		string_view(const char* _utf8) noexcept;
		string_view(const char* _utf8, size_type _size) noexcept;
		string_view(const_pointer _utf8) noexcept;
		string_view(const_pointer _utf8, size_type _size) noexcept;

		string_view(const string& _other) noexcept;
		string_view(string&& _other);
		string_view(const string_view& _other) noexcept;
		string_view(string_view&& _other) noexcept;

		~string_view();

		string_storage_t storage_type() const noexcept;

		string_view& operator=(const string& _other);
		string_view& operator=(const string_view& _other);
		string_view& operator=(string_view&& _other) noexcept;

		string_builder operator+(const string& _other) const;
		string_builder operator+(const string_view& _other) const;
		string_builder operator+(const char* _other) const;

		const_reference at(size_type _pos) const;

		const char* data() const noexcept;
		const_pointer bytes() const noexcept;

		inline size_t length() const noexcept { return static_cast<size_t>(size()); }
		size_type size() const noexcept;

		bool empty() const noexcept;

		size_type rune_size() const noexcept;
		size_type u16char_size() const noexcept;

		const_iterator begin() const noexcept;
		const_iterator cbegin() const noexcept;
        
		const_iterator end() const noexcept;
		const_iterator cend() const noexcept;
		
		const_rune_iterator rune_begin() const noexcept;
		const_rune_iterator rune_cbegin() const noexcept;

		const_rune_iterator rune_end() const noexcept;
		const_rune_iterator rune_cend() const noexcept;
		
		const_rune_iterator to_rune_iterator(size_type _pos) const noexcept;
		const_rune_iterator to_rune_iterator(const const_iterator& _it) const noexcept;

		string to_string() const;
        string to_large () const noexcept;

        size_type find(const string_view& _other, size_type _pos = 0) const noexcept;
        size_type find(const char* _utf8, size_type _pos = 0) const noexcept;
        size_type find(const char* _utf8, size_type _pos, size_type _substr_count) const noexcept;
        size_type find(const_pointer _utf8, size_type _pos = 0) const noexcept;
        size_type find(const_pointer _utf8, size_type _pos, size_type _substr_count) const noexcept;
		size_type find(char _ch, size_type _pos = 0) const noexcept;
		size_type find(const rune& _ch, size_type _pos = 0) const noexcept;

		size_type rfind(const char* _utf8, size_type _pos = npos) const noexcept;
		size_type rfind(const char* _utf8, size_type _pos, size_type _substr_count) const noexcept;
		size_type rfind(const_pointer _utf8, size_type _pos = npos) const noexcept;
		size_type rfind(const_pointer _utf8, size_type _pos, size_type _substr_count) const noexcept;
		size_type rfind(const rune& _ch, size_type _pos = npos) const noexcept;

		size_type index_of(const string_view& _other,
			case_sensitivity_t _cs = case_sensitivity_t::all_sensitive) const noexcept;
		
        size_type index_of(const string_view& _other, bool _full_match,
			case_sensitivity_t _cs = case_sensitivity_t::all_sensitive) const noexcept;
		
        size_type index_of(const string_view& _other, 
			size_type _offset, size_type _limit, bool _full_match,
			case_sensitivity_t _cs = case_sensitivity_t::all_sensitive) const noexcept;

		size_type index_of(const char* _utf8,
			case_sensitivity_t _cs = case_sensitivity_t::all_sensitive) const noexcept;
		
		size_type index_of(const char* _utf8, bool _full_match,
			case_sensitivity_t _cs = case_sensitivity_t::all_sensitive) const noexcept;
		
		size_type index_of(const char* _utf8, size_type _u8len, 
			size_type _offset, bool _full_match,
			case_sensitivity_t _cs = case_sensitivity_t::all_sensitive) const noexcept;

		size_type last_index_of(const string_view& _other,
			case_sensitivity_t _cs = case_sensitivity_t::all_sensitive) const noexcept;

		size_type last_index_of(const string_view& _other,
			size_type _offset, size_type _limit, bool _full_match,
			case_sensitivity_t _cs = case_sensitivity_t::all_sensitive) const noexcept;

		size_type last_index_of(const char* _utf8, 
			case_sensitivity_t _cs = case_sensitivity_t::all_sensitive) const noexcept;

		size_type last_index_of(const char* _utf8, bool _full_match,
			case_sensitivity_t _cs = case_sensitivity_t::all_sensitive) const noexcept;

		size_type last_index_of(const char* _utf8, size_type _u8len,
			size_type _offset, size_type _limit, bool _full_match,
			case_sensitivity_t _cs = case_sensitivity_t::all_sensitive) const noexcept;

		bool contains(const string_view& _sv) const noexcept;
		bool contains(const char* _utf8) const noexcept;
		bool contains(const char* _utf8, size_type _count) const noexcept;
		bool contains(const_pointer _utf8) const noexcept;
		bool contains(const_pointer _utf8, size_type _count) const noexcept;
		bool contains(char _ch) const noexcept;
        bool contains(const rune& _ch) const noexcept;

		bool starts_with(const string_view& _sv) const noexcept;
		bool starts_with(const char* _utf8) const noexcept;
		bool starts_with(const char* _utf8, size_type _count) const noexcept;
		bool starts_with(const_pointer _utf8) const noexcept;
		bool starts_with(const_pointer _utf8, size_type _count) const noexcept;
		//bool starts_with(char _ch) const noexcept;
		//bool starts_with(const word& _ch) const noexcept;

		bool ends_with(const string_view& _sv) const noexcept;
		bool ends_with(const char* _utf8) const noexcept;
		bool ends_with(const char* _utf8, size_type _count) const noexcept;
		bool ends_with(const_pointer _utf8) const noexcept;
		bool ends_with(const_pointer _utf8, size_type _count) const noexcept;
		//bool ends_with(char _ch) const noexcept;
		//bool ends_with(const word& _ch) const noexcept;

		string to_en_upper() const noexcept;
		string to_en_lower() const noexcept;

		string_view trim_space() const noexcept;
		string_view trim_left_space() const noexcept;
		string_view trim_right_space() const noexcept;

        string_view substr(size_type _pos = 0, size_type _count = npos) const noexcept;

		string replace(const string_view& _from, const string_view& _to) const noexcept;

		template<class _Container>
		inline MemeInteger_t split(string_view _key, split_behavior_t _behavior,
			std::back_insert_iterator<_Container> _inserter) const;
		
		//template<class _Container>
		//inline MemeInteger_t split(string_view _key, 
		//	std::back_insert_iterator<_Container> _inserter) const;

		//template<template<class, class...> class _Container, typename _Ty, class... _Arg>
		//inline MemeInteger_t split(
		//	string_view _key, split_behavior_t _behavior,
		//	std::back_insert_iterator<_Container<_Ty, _Arg...>> _inserter) const;
		//
		//template<template<class, class...> class _Container, typename _Ty, class... _Arg>
		//inline MemeInteger_t split(
		//	string_view _key, 
		//	std::back_insert_iterator<_Container<_Ty, _Arg...>> _inserter) const;

		template<template<class> class _Container,
            typename = std::enable_if_t<std::is_same_v<string, typename _Container<string>::value_type>>>
		inline MemeInteger_t split(string_view _key, split_behavior_t _behavior,
			std::back_insert_iterator<_Container<string>> _inserter) const;
		
		//template<template<class> class _Container>
		//inline MemeInteger_t split(string_view _key, 
		//	std::back_insert_iterator<_Container<string>> _inserter) const;

		template<template<class> class _Container,
            typename = std::enable_if_t<std::is_same_v<string, typename _Container<string>::value_type>>>
		inline MemeInteger_t split(string_view _key, split_behavior_t _behavior,
			std::back_insert_iterator<_Container<string_view>> _inserter) const;
		
		//template<template<class> class _Container>
		//inline MemeInteger_t split(string_view _key, 
		//	std::back_insert_iterator<_Container<string_view>> _inserter) const;

		template<template<class, class...> class _Container, class... _Arg,
            typename = std::enable_if_t<std::is_same_v<string, typename _Container<string, _Arg...>::value_type>>>
		inline MemeInteger_t split(
			string_view _key, split_behavior_t _behavior,
			std::back_insert_iterator<_Container<string, _Arg...>> _inserter) const;
		
		//template<template<class, class...> class _Container, class... _Arg>
		//inline MemeInteger_t split(
		//	string_view _key, 
		//	std::back_insert_iterator<_Container<string, _Arg...>> _inserter) const;

		template<template<class, class...> class _Container, class... _Arg,
            typename = std::enable_if_t<std::is_same_v<string_view, typename _Container<string_view, _Arg...>::value_type>>>
		inline MemeInteger_t split(
			string_view _key, split_behavior_t _behavior,
			std::back_insert_iterator<_Container<string_view, _Arg...>> _inserter) const;
		
		//template<template<class, class...> class _Container, class... _Arg>
		//inline MemeInteger_t split(
		//	string_view _key, 
		//	std::back_insert_iterator<_Container<string_view, _Arg...>> _inserter) const;

		const native_handle_type& native_handle() const noexcept;

	private:
		native_handle_type data_;
	};

	bool operator==(const string_view& _lhs, const string_view& _rhs);
	bool operator==(const string& _lhs, const string_view& _rhs);
	bool operator==(const string_view& _lhs, const string& _rhs);
	bool operator==(const char* _lhs, const string_view& _rhs);
	bool operator==(const string_view& _lhs, const char* _rhs);
    
	bool operator!=(const string_view& _lhs, const string_view& _rhs);
	bool operator!=(const string& _lhs, const string_view& _rhs);
	bool operator!=(const string_view& _lhs, const string& _rhs);
	bool operator!=(const char* _lhs, const string_view& _rhs);
	bool operator!=(const string_view& _lhs, const char* _rhs);
    
    //bool operator<(const string_view& _lhs, const string_view& _rhs);
    //bool operator<=(const string_view& _lhs, const string_view& _rhs);
    //bool operator>(const string_view& _lhs, const string_view& _rhs);
    //bool operator>=(const string_view& _lhs, const string_view& _rhs);

    
};

	memepp::string_view mm_view(const char* _str, size_t _len);
	memepp::string_view mm_view(const MemeByte_t* _str, size_t _len);

	memepp::string_view operator""_meme_sv(const char* _str, size_t _len);


#endif // !MEMEPP_STRING_VIEW_DEF_HPP_INCLUDED

#include <memepp/string_view_tmpimpl.hpp>
    