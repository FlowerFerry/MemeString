
#ifndef MEMEPP_BUFFER_SPAN_DEF_HPP_INCLUDED
#define MEMEPP_BUFFER_SPAN_DEF_HPP_INCLUDED

#include <mego/predef/lang/version.h>
#include <meme/buffer_fwd.h>

#include "memepp/buffer_span_fwd.hpp"
#include "memepp/buffer_fwd.hpp"
#include "memepp/variable_buffer_fwd.hpp"
#include "memepp/buffer_view_fwd.hpp"
#include "memepp/string_fwd.hpp"
#include "memepp/string_span_fwd.hpp"

#include <megopp/predef/keyword/noexcept.h>

namespace memepp {
inline namespace MMPP_NAMESPACE {

	//! @brief A lightweight non-owning read-only view of a byte sequence.
	//!
	//! Unlike @c buffer_view (which stores a @c MemeBufferStack_t internally,
	//! ~24+ bytes and may share ownership for large/user storage),
	//! @c buffer_span is strictly non-owning — it stores only a pointer and a
	//! length (16 bytes), and the caller must guarantee the underlying data
	//! outlives the span.
	//!
	//! @c buffer_span can be implicitly constructed from @c buffer,
	//! @c variable_buffer, @c buffer_view, @c string_span, and raw pointer+size
	//! pairs, serving as a universal read-only byte-range parameter type for
	//! functions that accept any of these buffer types.
	class buffer_span
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

		buffer_span() MEGOPP__NOEXCEPT;
		buffer_span(const buffer_span&) MEGOPP__NOEXCEPT = default;
		buffer_span(const_pointer _data, size_type _size) MEGOPP__NOEXCEPT;
		buffer_span(const buffer& _other) MEGOPP__NOEXCEPT;
		buffer_span(const variable_buffer& _other) MEGOPP__NOEXCEPT;
		buffer_span(const buffer_view& _other) MEGOPP__NOEXCEPT;
		buffer_span(const string_span& _other) MEGOPP__NOEXCEPT;

		// --- assignment ---

		buffer_span& operator=(const buffer_span&) MEGOPP__NOEXCEPT = default;

		// --- element access ---

		const_reference operator[](size_type _pos) const MEGOPP__NOEXCEPT;
		const_reference at(size_type _pos) const;
		const_reference front() const MEGOPP__NOEXCEPT;
		const_reference back() const MEGOPP__NOEXCEPT;
		const_pointer  data() const MEGOPP__NOEXCEPT;

		// --- capacity ---

		size_type size() const MEGOPP__NOEXCEPT;
		bool      empty() const MEGOPP__NOEXCEPT;

		// --- modifiers ---

		void remove_prefix(size_type _n) MEGOPP__NOEXCEPT;
		void remove_suffix(size_type _n) MEGOPP__NOEXCEPT;
		void swap(buffer_span& _other) MEGOPP__NOEXCEPT;

		// --- slice ---

		buffer_span slice(size_type _pos = 0, size_type _count = npos) const MEGOPP__NOEXCEPT;
		buffer_span first(size_type _count) const MEGOPP__NOEXCEPT;
		buffer_span last(size_type _count) const MEGOPP__NOEXCEPT;

		// --- find ---

		size_type find(buffer_span _v, size_type _pos = 0) const MEGOPP__NOEXCEPT;
		size_type find(value_type _byte, size_type _pos = 0) const MEGOPP__NOEXCEPT;
		size_type find(const_pointer _data, size_type _pos, size_type _count) const MEGOPP__NOEXCEPT;

		size_type rfind(buffer_span _v, size_type _pos = npos) const MEGOPP__NOEXCEPT;
		size_type rfind(value_type _byte, size_type _pos = npos) const MEGOPP__NOEXCEPT;
		size_type rfind(const_pointer _data, size_type _pos, size_type _count) const MEGOPP__NOEXCEPT;

		// --- contains / starts_with / ends_with ---

		bool contains(buffer_span _v) const MEGOPP__NOEXCEPT;
		bool contains(value_type _byte) const MEGOPP__NOEXCEPT;
		bool contains(const_pointer _data, size_type _count) const MEGOPP__NOEXCEPT;

		bool starts_with(buffer_span _v) const MEGOPP__NOEXCEPT;
		bool starts_with(value_type _byte) const MEGOPP__NOEXCEPT;
		bool starts_with(const_pointer _data, size_type _count) const MEGOPP__NOEXCEPT;

		bool ends_with(buffer_span _v) const MEGOPP__NOEXCEPT;
		bool ends_with(value_type _byte) const MEGOPP__NOEXCEPT;
		bool ends_with(const_pointer _data, size_type _count) const MEGOPP__NOEXCEPT;

		// --- conversion ---

		buffer to_buffer() const;
		buffer to_shared_storage() const noexcept;
		string to_string() const;
		variable_buffer to_variable_buffer() const;

		// --- comparison ---

		int compare(buffer_span _v) const MEGOPP__NOEXCEPT;
		int compare(size_type _pos1, size_type _count1, buffer_span _v) const;
		int compare(size_type _pos1, size_type _count1, buffer_span _v,
		            size_type _pos2, size_type _count2) const;

	private:
		const_pointer data_;
		size_type     size_;
	};

	// --- non-member comparison ---

	bool operator==(const buffer_span& _lhs, const buffer_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator!=(const buffer_span& _lhs, const buffer_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator< (const buffer_span& _lhs, const buffer_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator> (const buffer_span& _lhs, const buffer_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator<=(const buffer_span& _lhs, const buffer_span& _rhs) MEGOPP__NOEXCEPT;
	bool operator>=(const buffer_span& _lhs, const buffer_span& _rhs) MEGOPP__NOEXCEPT;

} // namespace MMPP_NAMESPACE
};

// --- free functions ---

memepp::buffer_span mm_bspan(const MemeByte_t* _data, size_t _len) MEGOPP__NOEXCEPT;

#endif // !MEMEPP_BUFFER_SPAN_DEF_HPP_INCLUDED
