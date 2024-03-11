
#ifndef MEMEPP_STRING_DEF_HPP_INCLUDED
#define MEMEPP_STRING_DEF_HPP_INCLUDED

#include "meme/string.h"
#include "meme/unsafe/string_view.h"
#include "memepp/rune_fwd.hpp"
#include "memepp/string_fwd.hpp"
#include "memepp/buffer_fwd.hpp"
#include "memepp/string_view_fwd.hpp"
#include "memepp/string_builder_fwd.hpp"

#include "memepp/iterator.hpp"
#include "memepp/rune_iterator.hpp"

namespace memepp {

    //! \brief A string class.
    //! \details This class is a wrapper of MemeStringStack_t.
	//! \note This class is not Thread-safe.
	class string
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
        
		string() noexcept;
		string(mmstr_const_t _other);
		string(native_handle_type&& _other);
		string(const native_handle_type& _other);

		string(string&& _other) noexcept;
		string(const string& _other);
		//string(const string& _other, size_type _pos);
		//string(const string& _other, size_type _pos, size_type _count);

		string(const char* _utf8);
		string(const char* _utf8, size_type _size);
		string(const char* _utf8, size_type _size, string_storage_t _suggest);
		string(const_pointer _utf8);
		string(const_pointer _utf8, size_type _size);
		string(const_pointer _utf8, size_type _size, string_storage_t _suggest);
		string(const uint16_t* _utf16, size_type _size);
		string(const uint16_t* _utf16, size_type _size, string_storage_t _suggest);

		string(const rune& _ch);
        string(const string_builder& _builder);
		//string(size_type _count, char _ch);

		//template< class InputIt >
		//string(InputIt _first, InputIt _last);

		string(std::nullptr_t) = delete;

		~string();

		string& operator=(string && _other) noexcept;
		string& operator=(const string& _other);
        string& operator=(const string_builder& _builder);

        string operator+(const string& _other) const;
        string operator+(const string_view& _other) const;

		string_storage_t storage_type() const noexcept;

		const_reference at(size_type _pos) const;

		const char* data() const noexcept;
		const char* c_str() const noexcept;
		const_pointer bytes() const noexcept;

		inline size_t length() const noexcept { return static_cast<size_t>(size()); }
		inline size_t char_size() const noexcept { return static_cast<size_t>(size()); }
		size_type size() const noexcept;
		bool empty() const noexcept;
		//size_type max_size() const noexcept;
		size_type capacity() const noexcept;
		size_type rune_size() const noexcept;
        size_type u16char_size() const noexcept;

		const_iterator begin() const noexcept;
		const_iterator cbegin() const noexcept;

		const_iterator end() const noexcept;
		const_iterator cend() const noexcept;

		const_reverse_iterator rbegin() const noexcept;
		const_reverse_iterator crbegin() const noexcept;

		const_reverse_iterator rend() const noexcept;
		const_reverse_iterator crend() const noexcept;

		const_rune_iterator rune_begin() const noexcept;
		const_rune_iterator rune_cbegin() const noexcept;

		const_rune_iterator rune_end() const noexcept;
		const_rune_iterator rune_cend() const noexcept;
		
        const_rune_iterator to_rune_iterator(size_type _pos) const noexcept;
        const_rune_iterator to_rune_iterator(const const_iterator& _it) const noexcept;

        string to_large() const noexcept;

		void swap(string& _other) noexcept;

		//! \brief Find the first occurrence of a character in a string.
		//! \param _other The string to find.
		//! \param _pos The position to start searching from.
		//! \return The position of the first occurrence of the character in the string, or npos if not found.
        size_type find(const string& _other, size_type _pos = 0) const noexcept;
        size_type find(const char* _utf8, size_type _pos = 0) const noexcept;
        size_type find(const char* _utf8, size_type _pos, size_type _count) const noexcept;
        size_type find(const_pointer _utf8, size_type _pos = 0) const noexcept;
        size_type find(const_pointer _utf8, size_type _pos, size_type _count) const noexcept;
		size_type find(char _ch, size_type _pos = 0) const noexcept;
        size_type find(const rune& _ch, size_type _pos = 0) const noexcept;

        //size_type rfind(const string& _other, size_type _pos = npos) const noexcept;
        size_type rfind(const char* _utf8, size_type _pos = npos) const noexcept;
        size_type rfind(const char* _utf8, size_type _pos, size_type _count) const noexcept;
        size_type rfind(const_pointer _utf8, size_type _pos = npos) const noexcept;
        size_type rfind(const_pointer _utf8, size_type _pos, size_type _count) const noexcept;
        //size_type rfind(char _ch, size_type _pos = npos) const noexcept;
        size_type rfind(const rune& _ch, size_type _pos = npos) const noexcept;
        
		size_type index_of(const string& _other,
			case_sensitivity_t _cs = case_sensitivity_t::all_sensitive) const noexcept;

		size_type index_of(const string_view& _other,
			case_sensitivity_t _cs = case_sensitivity_t::all_sensitive) const noexcept;

		size_type index_of(const char* _utf8,
			case_sensitivity_t _cs = case_sensitivity_t::all_sensitive) const noexcept;

		//size_type index_of_with_strlen(
		//	const char* _utf8, size_type _utf8_len,
		//	case_sensitivity_t _cs = case_sensitivity_t::all_sensitive) const noexcept;

		//! @brief Returns true if this string contains the given string.
		//! @param _sv The string to search for.
		//! @returns True if this string contains the given string.
		//! @note This function is case sensitive.
        bool contains(const string_view& _sv) const noexcept;
        bool contains(const char* _utf8) const noexcept;
        bool contains(const char* _utf8, size_type _count) const noexcept;
        bool contains(const_pointer _utf8) const noexcept;
        bool contains(const_pointer _utf8, size_type _count) const noexcept;
        bool contains(char _ch) const noexcept;
        bool contains(const rune& _ch) const noexcept;

        bool contains_only_ascii() const noexcept;

		//! @brief Returns true if this string starts with the given string.
		//! @param _sv The string to search for.
		//! @returns True if this string starts with the given string.
		//! @note This function is case sensitive.
        bool starts_with(const string_view& _sv) const noexcept;

        bool starts_with(const char* _utf8) const noexcept;
        bool starts_with(const char* _utf8, size_type _count) const noexcept;
        bool starts_with(const_pointer _utf8) const noexcept;
        bool starts_with(const_pointer _utf8, size_type _count) const noexcept;
        //bool starts_with(char _ch) const noexcept;
        bool starts_with(const rune& _ch) const noexcept;

		//! @brief Returns true if this string ends with the given string.
		//! @param _sv The string to search for.
		//! @returns True if this string ends with the given string.
		//! @note This function is case sensitive.
        bool ends_with(const string_view& _sv) const noexcept;
        bool ends_with(const char* _utf8) const noexcept;
        bool ends_with(const char* _utf8, size_type _count) const noexcept;
        bool ends_with(const_pointer _utf8) const noexcept;
        bool ends_with(const_pointer _utf8, size_type _count) const noexcept;
        //bool ends_with(char _ch) const noexcept;
        bool ends_with(const rune& _ch) const noexcept;

		string to_en_upper() const noexcept;
		string to_en_lower() const noexcept;
        
        string trim_space() const noexcept;
        string trim_left_space() const noexcept;
        string trim_right_space() const noexcept;

        string left (size_type _count) const noexcept;
        string right(size_type _count) const noexcept;
        
		//! @brief Returns a substring of this string.
		//! @param _pos The position of the first character to copy.
		//! @param _count The number of characters to copy.
		//! @returns A substring of this string.
        string substr(size_type _pos = 0, size_type _count = npos) const noexcept;

        string concat(const string& _other) const noexcept;
        string concat(const string_view& _other) const noexcept;

        string replace(const string_view& _from, const string_view& _to) const noexcept;
		
		template<typename _Func>
		inline string mapping_convert(_Func&& _func) const;

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

		//! \brief Split string with key.
		//! \param _key The key to split.
		//! \param _behavior The behavior of split.
		//! \param _inserter The inserter to insert the result.
		//! \return The error code.
		template<template<class> class _Container>
		inline MemeInteger_t split(string_view _key, split_behavior_t _behavior,
			std::back_insert_iterator<_Container<string>> _inserter) const;

		template<template<class> class _Container>
		inline MemeInteger_t split(string_view _key, 
			std::back_insert_iterator<_Container<string>> _inserter) const;

		//! \brief Split string with key.
		//! \param _key The key to split.
		//! \param _behavior The behavior of split.
		//! \param _inserter The inserter to insert the result.
		//! \return The error code.
		template<template<class> class _Container>
		inline MemeInteger_t split(string_view _key, split_behavior_t _behavior,
			std::back_insert_iterator<_Container<string_view>> _inserter) const;

		template<template<class> class _Container>
		inline MemeInteger_t split(string_view _key, 
			std::back_insert_iterator<_Container<string_view>> _inserter) const;

		//! \brief Split string with key.
		//! \param _key The key to split.
		//! \param _behavior The behavior of split.
		//! \param _inserter The inserter to insert the result.
		//! \return The error code.
		template<template<class, class...> class _Container, class... _Arg>
		inline MemeInteger_t split(
			string_view _key, split_behavior_t _behavior,
			std::back_insert_iterator<_Container<string, _Arg...>> _inserter) const;

		template<template<class, class...> class _Container, class... _Arg>
		inline MemeInteger_t split(
			string_view _key, 
			std::back_insert_iterator<_Container<string, _Arg...>> _inserter) const;

		//! \brief Split string with key.
		//! \param _key The key to split.
		//! \param _behavior The behavior of split.
		//! \param _inserter The inserter to insert the result.
		//! \return The error code.
		template<template<class, class...> class _Container, class... _Arg>
		inline MemeInteger_t split(
			string_view _key, split_behavior_t _behavior,
			std::back_insert_iterator<_Container<string_view, _Arg...>> _inserter) const;
		
		template<template<class, class...> class _Container, class... _Arg>
		inline MemeInteger_t split(
			string_view _key, 
			std::back_insert_iterator<_Container<string_view, _Arg...>> _inserter) const;

		template<typename _Function, template<class> class _Container>
		inline MemeInteger_t split(
			split_behavior_t _behavior,
			std::back_insert_iterator<_Container<string_view>> _inserter, 
			const _Function& _fn) const;
		
		const native_handle_type& native_handle() const noexcept;

	private:
		native_handle_type data_;
	};

	bool operator==(const string& _lhs, const string& _rhs);
	bool operator==(const char* _lhs, const string& _rhs);
	bool operator==(const string& _lhs, const char* _rhs);
    bool operator==(const string& _lhs, string::const_pointer _rhs);
    bool operator==(string::const_pointer _lhs, const string& _rhs);

	bool operator!=(const string& _lhs, const string& _rhs);
	bool operator!=(const char* _lhs, const string& _rhs);
	bool operator!=(const string& _lhs, const char* _rhs);
    bool operator!=(const string& _lhs, string::const_pointer _rhs);
    bool operator!=(string::const_pointer _lhs, const string& _rhs);

	memepp::string from_hexadecimals(const uint8_t* _buf, size_t _len);

	//! \brief Format the string in C style with the specified formatting string
    //! \param _fmt The formatting string
    //! \param _size_limit The formatted string length limit
    //! \param ... The arguments
    //! \return The formatted string
	string c_format(
		size_t _size_limit,
		MEGO_SYMBOL__MSVC_FORMAT_STRING(const char* _fmt),
		...)
		MEGO_SYMBOL__GCC_ATTRIBUTE_FORMAT(printf, 2, 3);
    
}; // namespace memepp


	memepp::string mm_from(const char* _str, size_t _len);
	memepp::string mm_from(const MemeByte_t* _str, size_t _len);

    memepp::string_builder operator+(const char* _lhs, const memepp::string& _rhs);

	memepp::string operator""_meme(const char* _str, size_t _len);


#endif // !MEMEPP_STRING_DEF_HPP_INCLUDED

#include <memepp/string_tmpimpl.hpp>
	