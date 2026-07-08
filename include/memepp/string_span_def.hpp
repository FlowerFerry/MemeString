
#ifndef MEMEPP_STRING_SPAN_DEF_HPP_INCLUDED
#define MEMEPP_STRING_SPAN_DEF_HPP_INCLUDED

#include <mego/predef/lang/version.h>
#include <meme/string_fwd.h>

#include "memepp/string_fwd.hpp"
#include "memepp/string_view_fwd.hpp"
#include "memepp/string_span_fwd.hpp"
#include "memepp/iterator.hpp"
#include "memepp/rune_iterator.hpp"

#include <megopp/predef/keyword/noexcept.h>

namespace memepp {
inline namespace MMPP_NAMESPACE {

	//! @brief A lightweight non-owning read-only view of UTF-8 string data.
	//!
	//! Unlike @c string_view (which may share ownership via reference counting for
	//! large/user storage), @c string_span is strictly non-owning — it stores only
	//! a pointer and a length, and the caller must guarantee the underlying data
	//! outlives the span.
	//!
	//! Semantics match @c std::string_view (C++17).
	class string_span
	{
	public:
		using value_type      = MemeByte_t;
		using size_type       = MemeInteger_t;
		using difference_type = ptrdiff_t;
		using const_reference = const value_type&;
		using const_pointer   = const value_type*;

#if MG_LANG__CXX17_AVAIL
		inline static constexpr size_type npos = static_cast<size_type>(-1);
#else
		enum : size_type { npos = static_cast<size_type>(-1) };
#endif

		// --- constructors ---

		string_span() MEGOPP__NOEXCEPT;
		string_span(const string_span&) MEGOPP__NOEXCEPT = default;
		string_span(const char* _utf8) MEGOPP__NOEXCEPT;
		string_span(const char* _utf8, size_type _count) MEGOPP__NOEXCEPT;
		string_span(const_pointer _utf8) MEGOPP__NOEXCEPT;
		string_span(const_pointer _utf8, size_type _count) MEGOPP__NOEXCEPT;
		string_span(const string& _other) MEGOPP__NOEXCEPT;

		// --- assignment ---

		string_span& operator=(const string_span&) MEGOPP__NOEXCEPT = default;

		// --- element access ---

		const_reference operator[](size_type _pos) const MEGOPP__NOEXCEPT;
		const_reference at(size_type _pos) const;
		const_reference front() const MEGOPP__NOEXCEPT;
		const_reference back() const MEGOPP__NOEXCEPT;
		const char*    data() const MEGOPP__NOEXCEPT;
		const_pointer  bytes() const MEGOPP__NOEXCEPT;

		// --- capacity ---

		size_type size() const MEGOPP__NOEXCEPT;
		size_t    length() const MEGOPP__NOEXCEPT;
		bool      empty() const MEGOPP__NOEXCEPT;

		// --- modifiers ---

		void remove_prefix(size_type _n) MEGOPP__NOEXCEPT;
		void remove_suffix(size_type _n) MEGOPP__NOEXCEPT;
		void swap(string_span& _other) MEGOPP__NOEXCEPT;

		// --- iterators ---

		const_iterator         begin()  const MEGOPP__NOEXCEPT;
		const_iterator         cbegin() const MEGOPP__NOEXCEPT;
		const_iterator         end()    const MEGOPP__NOEXCEPT;
		const_iterator         cend()   const MEGOPP__NOEXCEPT;
		const_reverse_iterator rbegin()  const MEGOPP__NOEXCEPT;
		const_reverse_iterator crbegin() const MEGOPP__NOEXCEPT;
		const_reverse_iterator rend()    const MEGOPP__NOEXCEPT;
		const_reverse_iterator crend()   const MEGOPP__NOEXCEPT;

		const_rune_iterator rune_begin()  const MEGOPP__NOEXCEPT;
		const_rune_iterator rune_cbegin() const MEGOPP__NOEXCEPT;
		const_rune_iterator rune_end()    const MEGOPP__NOEXCEPT;
		const_rune_iterator rune_cend()   const MEGOPP__NOEXCEPT;

		// --- string operations ---

		size_type   rune_size() const MEGOPP__NOEXCEPT;
		size_type   u16char_size() const MEGOPP__NOEXCEPT;
		rune        rune_front() const MEGOPP__NOEXCEPT;
		rune        rune_back() const MEGOPP__NOEXCEPT;

		const_rune_iterator to_rune_iterator(size_type _pos) const MEGOPP__NOEXCEPT;

		string_span substr(size_type _pos = 0, size_type _count = npos) const MEGOPP__NOEXCEPT;

		size_type copy(value_type* _dest, size_type _count, size_type _pos = 0) const;

		int compare(string_span _v) const MEGOPP__NOEXCEPT;
		int compare(size_type _pos1, size_type _count1, string_span _v) const;
		int compare(size_type _pos1, size_type _count1, string_span _v,
		            size_type _pos2, size_type _count2) const;
		int compare(const char* _s) const;
		int compare(size_type _pos1, size_type _count1, const char* _s) const;
		int compare(size_type _pos1, size_type _count1, const char* _s, size_type _count2) const;

		bool starts_with(string_span _v) const MEGOPP__NOEXCEPT;
		bool starts_with(char _c) const MEGOPP__NOEXCEPT;
		bool starts_with(const char* _s) const;
		bool starts_with(const_pointer _s) const MEGOPP__NOEXCEPT;
		bool ends_with(string_span _v) const MEGOPP__NOEXCEPT;
		bool ends_with(char _c) const MEGOPP__NOEXCEPT;
		bool ends_with(const char* _s) const;
		bool ends_with(const_pointer _s) const MEGOPP__NOEXCEPT;

		bool contains(string_span _v) const MEGOPP__NOEXCEPT;
		bool contains(char _c) const MEGOPP__NOEXCEPT;
		bool contains(const char* _s) const;
		bool contains(const_pointer _s) const MEGOPP__NOEXCEPT;

		// --- find ---

		size_type find(string_span _v, size_type _pos = 0) const MEGOPP__NOEXCEPT;
		size_type find(char _c, size_type _pos = 0) const MEGOPP__NOEXCEPT;
		size_type find(const char* _s, size_type _pos, size_type _count) const;
		size_type find(const char* _s, size_type _pos = 0) const;
		size_type find(const_pointer _s, size_type _pos, size_type _count) const MEGOPP__NOEXCEPT;
		size_type find(const_pointer _s, size_type _pos = 0) const MEGOPP__NOEXCEPT;

		size_type rfind(string_span _v, size_type _pos = npos) const MEGOPP__NOEXCEPT;
		size_type rfind(char _c, size_type _pos = npos) const MEGOPP__NOEXCEPT;
		size_type rfind(const char* _s, size_type _pos, size_type _count) const;
		size_type rfind(const char* _s, size_type _pos = npos) const;
		size_type rfind(const_pointer _s, size_type _pos, size_type _count) const MEGOPP__NOEXCEPT;
		size_type rfind(const_pointer _s, size_type _pos = npos) const MEGOPP__NOEXCEPT;

		size_type find_first_of(string_span _v, size_type _pos = 0) const MEGOPP__NOEXCEPT;
		size_type find_first_of(char _c, size_type _pos = 0) const MEGOPP__NOEXCEPT;
		size_type find_first_of(const char* _s, size_type _pos, size_type _count) const;
		size_type find_first_of(const char* _s, size_type _pos = 0) const;

		size_type find_first_not_of(string_span _v, size_type _pos = 0) const MEGOPP__NOEXCEPT;
		size_type find_first_not_of(char _c, size_type _pos = 0) const MEGOPP__NOEXCEPT;
		size_type find_first_not_of(const char* _s, size_type _pos, size_type _count) const;
		size_type find_first_not_of(const char* _s, size_type _pos = 0) const;

		size_type find_last_of(string_span _v, size_type _pos = npos) const MEGOPP__NOEXCEPT;
		size_type find_last_of(char _c, size_type _pos = npos) const MEGOPP__NOEXCEPT;
		size_type find_last_of(const char* _s, size_type _pos, size_type _count) const;
		size_type find_last_of(const char* _s, size_type _pos = npos) const;

		size_type find_last_not_of(string_span _v, size_type _pos = npos) const MEGOPP__NOEXCEPT;
		size_type find_last_not_of(char _c, size_type _pos = npos) const MEGOPP__NOEXCEPT;
		size_type find_last_not_of(const char* _s, size_type _pos, size_type _count) const;
		size_type find_last_not_of(const char* _s, size_type _pos = npos) const;

		// --- conversion ---

		string to_string() const;

	private:
		//! Create a temporary non-owning MemeStringStack_t for C API calls.
		MemeStringStack_t to_stack_() const MEGOPP__NOEXCEPT;

		const_pointer data_;
		size_type     size_;
	};

	// --- non-member comparison ---

	bool operator==(const string_span& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator!=(const string_span& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator< (const string_span& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator> (const string_span& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator<=(const string_span& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator>=(const string_span& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;

	// cross-type comparisons with memepp::string
	bool operator==(const string& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator==(const string_span& _lhs, const string& _rhs) MEGOPP__NOEXCEPT;
	bool operator!=(const string& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator!=(const string_span& _lhs, const string& _rhs) MEGOPP__NOEXCEPT;
	bool operator< (const string& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator< (const string_span& _lhs, const string& _rhs) MEGOPP__NOEXCEPT;
	bool operator> (const string& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator> (const string_span& _lhs, const string& _rhs) MEGOPP__NOEXCEPT;
	bool operator<=(const string& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator<=(const string_span& _lhs, const string& _rhs) MEGOPP__NOEXCEPT;
	bool operator>=(const string& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator>=(const string_span& _lhs, const string& _rhs) MEGOPP__NOEXCEPT;

	// cross-type comparisons with const char*
	bool operator==(const char* _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator==(const string_span& _lhs, const char* _rhs) MEGOPP__NOEXCEPT;
	bool operator!=(const char* _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator!=(const string_span& _lhs, const char* _rhs) MEGOPP__NOEXCEPT;
	bool operator< (const char* _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator< (const string_span& _lhs, const char* _rhs) MEGOPP__NOEXCEPT;
	bool operator> (const char* _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator> (const string_span& _lhs, const char* _rhs) MEGOPP__NOEXCEPT;
	bool operator<=(const char* _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator<=(const string_span& _lhs, const char* _rhs) MEGOPP__NOEXCEPT;
	bool operator>=(const char* _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator>=(const string_span& _lhs, const char* _rhs) MEGOPP__NOEXCEPT;

	// cross-type comparisons with const_pointer
	bool operator==(string_span::const_pointer _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator==(const string_span& _lhs, string_span::const_pointer _rhs) MEGOPP__NOEXCEPT;
	bool operator!=(string_span::const_pointer _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator!=(const string_span& _lhs, string_span::const_pointer _rhs) MEGOPP__NOEXCEPT;
	bool operator< (string_span::const_pointer _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator< (const string_span& _lhs, string_span::const_pointer _rhs) MEGOPP__NOEXCEPT;
	bool operator> (string_span::const_pointer _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator> (const string_span& _lhs, string_span::const_pointer _rhs) MEGOPP__NOEXCEPT;
	bool operator<=(string_span::const_pointer _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator<=(const string_span& _lhs, string_span::const_pointer _rhs) MEGOPP__NOEXCEPT;
	bool operator>=(string_span::const_pointer _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator>=(const string_span& _lhs, string_span::const_pointer _rhs) MEGOPP__NOEXCEPT;

} // namespace MMPP_NAMESPACE
};

// --- free functions ---

memepp::string_span mm_span(const char* _str, size_t _len) MEGOPP__NOEXCEPT;
memepp::string_span mm_span(const MemeByte_t* _str, size_t _len) MEGOPP__NOEXCEPT;

memepp::string_span operator""_meme_span(const char* _str, size_t _len) MEGOPP__NOEXCEPT;

#endif // !MEMEPP_STRING_SPAN_DEF_HPP_INCLUDED
