
#ifndef MEMEPP_BUFFER_SPAN_IMPL_HPP_INCLUDED
#define MEMEPP_BUFFER_SPAN_IMPL_HPP_INCLUDED

#include "memepp/buffer_span_def.hpp"
#include <memepp/buffer.hpp>
#include <memepp/variable_buffer.hpp>
#include <memepp/buffer_view.hpp>
#include <memepp/string_span.hpp>
#include <memepp/string.hpp>
#include <memepp/errc.hpp>

#include <cstring>
#include <algorithm>

#ifndef MEMEPP__IMPL_INLINE
#	ifdef MEMEPP__IMPL_SEPARATE
#		define MEMEPP__IMPL_INLINE
#	else
#		define MEMEPP__IMPL_INLINE inline
#	endif
#endif

#if !MMOPT__EXCEPTION_DISABLED
#	include <stdexcept>
#endif

namespace memepp {
inline namespace MMPP_NAMESPACE {

// ============================================================
//  constructors
// ============================================================

MEMEPP__IMPL_INLINE buffer_span::buffer_span() MEGOPP__NOEXCEPT
	: data_(nullptr), size_(0)
{
}

MEMEPP__IMPL_INLINE buffer_span::buffer_span(const_pointer _data, size_type _size) MEGOPP__NOEXCEPT
	: data_(_data), size_(_size)
{
}

MEMEPP__IMPL_INLINE buffer_span::buffer_span(const buffer& _other) MEGOPP__NOEXCEPT
	: data_(_other.data()), size_(_other.size())
{
}

MEMEPP__IMPL_INLINE buffer_span::buffer_span(const variable_buffer& _other) MEGOPP__NOEXCEPT
	: data_(_other.data()), size_(_other.size())
{
}

MEMEPP__IMPL_INLINE buffer_span::buffer_span(const buffer_view& _other) MEGOPP__NOEXCEPT
	: data_(_other.data()), size_(_other.size())
{
}

MEMEPP__IMPL_INLINE buffer_span::buffer_span(const string_span& _other) MEGOPP__NOEXCEPT
	: data_(_other.bytes()), size_(_other.size())
{
}

// ============================================================
//  element access
// ============================================================

MEMEPP__IMPL_INLINE buffer_span::const_reference
buffer_span::operator[](size_type _pos) const
{
	/*
	 * Mirrors buffer::operator[] and buffer_view::operator[], both of which
	 * forward to at().  An out-of-range index is reported through errc()
	 * (and throws in the exception-enabled build) instead of being
	 * undefined behaviour.
	 */
	return at(_pos);
}

MEMEPP__IMPL_INLINE buffer_span::const_reference
buffer_span::at(size_type _pos) const
{
	if (MEGO_SYMBOL__UNLIKELY(_pos < 0 || _pos >= size_)) {
		*errc() = MGEC__RANGE;
#if !MMOPT__EXCEPTION_DISABLED
		throw_errc(*errc());
#endif
		static const mmbyte_t s_out_of_range = 0;
		return s_out_of_range;
	}
	*errc() = MGEC__OK;
	return data_[_pos];
}

MEMEPP__IMPL_INLINE buffer_span::const_reference
buffer_span::front() const MEGOPP__NOEXCEPT
{
	/* MemeString_front returns NULL for an empty string; buffer_span must
	 * not dereference data_ in that case.  at() reports the range error
	 * instead, but it is not noexcept, so the check is inlined here. */
	if (size_ <= 0)
	{
		static const mmbyte_t s_empty = 0;
		return s_empty;
	}
	return data_[0];
}

MEMEPP__IMPL_INLINE buffer_span::const_reference
buffer_span::back() const MEGOPP__NOEXCEPT
{
	/* size_ - 1 on an empty span would be -1, i.e. data_[-1]. */
	if (size_ <= 0)
	{
		static const mmbyte_t s_empty = 0;
		return s_empty;
	}
	return data_[size_ - 1];
}

MEMEPP__IMPL_INLINE buffer_span::const_pointer
buffer_span::data() const MEGOPP__NOEXCEPT
{
	return data_;
}

// ============================================================
//  capacity
// ============================================================

MEMEPP__IMPL_INLINE buffer_span::size_type
buffer_span::size() const MEGOPP__NOEXCEPT
{
	return size_;
}

MEMEPP__IMPL_INLINE bool
buffer_span::empty() const MEGOPP__NOEXCEPT
{
	/* A construction from (ptr, negative) is out of contract but must not
	 * produce an object that reports empty() == false while front(), back()
	 * and slice() all treat it as empty.  Treat any non-positive size as
	 * empty so every accessor agrees. */
	return size_ <= 0;
}

// ============================================================
//  modifiers
// ============================================================

MEMEPP__IMPL_INLINE void
buffer_span::remove_prefix(size_type _n) MEGOPP__NOEXCEPT
{
	/* Removing more than size() (or a negative amount) would move data_
	 * outside the source array.  Clamp to size(), which yields the empty
	 * span.  Skipping the arithmetic entirely for an empty result avoids
	 * "nullptr + 0", which is still undefined: [expr.add] only defines
	 * pointer arithmetic for pointers into an array object, and a null
	 * pointer points to no object. */
	if (_n < 0 || _n > size_)
		_n = size_;
	if (_n == size_)
	{
		/* Becomes empty: no pointer arithmetic on a possibly-null data_. */
		data_ = (size_ > 0) ? data_ + size_ : data_;
		size_ = 0;
		return;
	}
	data_ += _n;
	size_ -= _n;
}

MEMEPP__IMPL_INLINE void
buffer_span::remove_suffix(size_type _n) MEGOPP__NOEXCEPT
{
	/* A negative amount would increase size_ past the source array. */
	if (_n < 0 || _n > size_)
		_n = size_;
	size_ -= _n;
}

MEMEPP__IMPL_INLINE void
buffer_span::swap(buffer_span& _other) MEGOPP__NOEXCEPT
{
	std::swap(data_, _other.data_);
	std::swap(size_, _other.size_);
}

// ============================================================
//  slice
// ============================================================

MEMEPP__IMPL_INLINE buffer_span
buffer_span::slice(size_type _pos, size_type _count) const MEGOPP__NOEXCEPT
{
	/*
	 * Mirrors MemeBufferStack_slice, which buffer and buffer_view delegate
	 * to:
	 *   _pos < 0 || _pos >= size()  ->  empty span
	 *   _count < 0                  ->  to the end (same as npos)
	 *   _count > size() - _pos      ->  clamped to the remainder
	 * Following the same rules removes the out-of-range pointer arithmetic
	 * that a raw data_ + _pos would perform for a negative position.
	 */
	if (_pos < 0 || _pos >= size_)
		return buffer_span();
	if (_count < 0)
		_count = size_ - _pos;
	else if (_count > size_ - _pos)
		_count = size_ - _pos;
	/* The early return above guarantees _pos < size_, so size_ > 0 and
	 * data_ is not null; the offset below is therefore inside the array. */
	return buffer_span(data_ + _pos, _count);
}

MEMEPP__IMPL_INLINE buffer_span
buffer_span::first(size_type _count) const MEGOPP__NOEXCEPT
{
	/* A negative count is out of contract for first(); clamp it to the
	 * whole span so no out-of-range pointer is ever formed. */
	if (_count < 0 || _count > size_)
		_count = size_;
	return buffer_span(data_, _count);
}

MEMEPP__IMPL_INLINE buffer_span
buffer_span::last(size_type _count) const MEGOPP__NOEXCEPT
{
	/* A negative count would compute data_ + size_ + |count|, past
	 * one-past-the-end.  Clamp it to the whole span instead. */
	if (_count < 0 || _count > size_)
		_count = size_;
	/* On an already-empty span there is nothing to point at; "data_ + 0"
	 * would be arithmetic on a null pointer, which [expr.add] does not
	 * define.  For a non-empty span data_ + size_ is the legal
	 * one-past-the-end pointer, so that case keeps the arithmetic. */
	if (size_ <= 0)
		return buffer_span();
	return buffer_span(data_ + size_ - _count, _count);
}

// ============================================================
//  find
// ============================================================

MEMEPP__IMPL_INLINE buffer_span::size_type
buffer_span::find(buffer_span _v, size_type _pos) const MEGOPP__NOEXCEPT
{
	if (_pos < 0 || _pos > size_) return npos;
	if (_v.size_ == 0) return _pos;
	if (_v.size_ > size_ - _pos) return npos;

	const_pointer last = data_ + size_ - _v.size_ + 1;
	for (const_pointer p = data_ + _pos; p != last; ++p) {
		if (std::memcmp(p, _v.data_, _v.size_) == 0)
			return static_cast<size_type>(p - data_);
	}
	return npos;
}

MEMEPP__IMPL_INLINE buffer_span::size_type
buffer_span::find(value_type _byte, size_type _pos) const MEGOPP__NOEXCEPT
{
	if (_pos < 0) return npos;
	for (size_type i = _pos; i < size_; ++i) {
		if (data_[i] == _byte)
			return i;
	}
	return npos;
}

MEMEPP__IMPL_INLINE buffer_span::size_type
buffer_span::find(const_pointer _data, size_type _pos, size_type _count) const MEGOPP__NOEXCEPT
{
	return find(buffer_span(_data, _count), _pos);
}

// ============================================================
//  rfind
// ============================================================

MEMEPP__IMPL_INLINE buffer_span::size_type
buffer_span::rfind(buffer_span _v, size_type _pos) const MEGOPP__NOEXCEPT
{
	if (_v.size_ == 0) {
		if (_pos == npos || _pos >= size_)
			return size_;
		return _pos;
	}
	if (_v.size_ > size_) return npos;

	if (_pos == npos || _pos >= size_)
		_pos = size_ - _v.size_;
	else if (size_ - _pos < _v.size_)
		_pos = size_ - _v.size_;

	if (_pos < 0) return npos;

	for (size_type i = _pos; ; --i) {
		if (std::memcmp(data_ + i, _v.data_, _v.size_) == 0)
			return i;
		if (i == 0) break;
	}
	return npos;
}

MEMEPP__IMPL_INLINE buffer_span::size_type
buffer_span::rfind(value_type _byte, size_type _pos) const MEGOPP__NOEXCEPT
{
	if (_pos == npos || _pos >= size_) _pos = size_ - 1;
	if (_pos < 0 || size_ == 0) return npos;
	for (size_type i = _pos; ; --i) {
		if (data_[i] == _byte)
			return i;
		if (i == 0) break;
	}
	return npos;
}

MEMEPP__IMPL_INLINE buffer_span::size_type
buffer_span::rfind(const_pointer _data, size_type _pos, size_type _count) const MEGOPP__NOEXCEPT
{
	return rfind(buffer_span(_data, _count), _pos);
}

// ============================================================
//  contains / starts_with / ends_with
// ============================================================

MEMEPP__IMPL_INLINE bool
buffer_span::contains(buffer_span _v) const MEGOPP__NOEXCEPT
{
	return find(_v) != npos;
}

MEMEPP__IMPL_INLINE bool
buffer_span::contains(value_type _byte) const MEGOPP__NOEXCEPT
{
	return find(_byte) != npos;
}

MEMEPP__IMPL_INLINE bool
buffer_span::contains(const_pointer _data, size_type _count) const MEGOPP__NOEXCEPT
{
	return find(_data, 0, _count) != npos;
}

MEMEPP__IMPL_INLINE bool
buffer_span::starts_with(buffer_span _v) const MEGOPP__NOEXCEPT
{
	if (_v.size_ > size_) return false;
	/* An empty needle matches trivially.  Calling memcmp with a null
	 * argument is undefined even for a zero count, so short-circuit it. */
	if (_v.size_ == 0) return true;
	return std::memcmp(data_, _v.data_, _v.size_) == 0;
}

MEMEPP__IMPL_INLINE bool
buffer_span::starts_with(value_type _byte) const MEGOPP__NOEXCEPT
{
	return size_ > 0 && data_[0] == _byte;
}

MEMEPP__IMPL_INLINE bool
buffer_span::starts_with(const_pointer _data, size_type _count) const MEGOPP__NOEXCEPT
{
	return starts_with(buffer_span(_data, _count));
}

MEMEPP__IMPL_INLINE bool
buffer_span::ends_with(buffer_span _v) const MEGOPP__NOEXCEPT
{
	if (_v.size_ > size_) return false;
	/* Empty needle matches trivially; also avoids "data_ + size_ - 0" on an
	 * empty span (null pointer arithmetic) and a null memcmp argument. */
	if (_v.size_ == 0) return true;
	return std::memcmp(data_ + size_ - _v.size_, _v.data_, _v.size_) == 0;
}

MEMEPP__IMPL_INLINE bool
buffer_span::ends_with(value_type _byte) const MEGOPP__NOEXCEPT
{
	return size_ > 0 && data_[size_ - 1] == _byte;
}

MEMEPP__IMPL_INLINE bool
buffer_span::ends_with(const_pointer _data, size_type _count) const MEGOPP__NOEXCEPT
{
	return ends_with(buffer_span(_data, _count));
}

// ============================================================
//  conversion
// ============================================================

MEMEPP__IMPL_INLINE buffer
buffer_span::to_buffer() const
{
	return buffer(data_, size_);
}

MEMEPP__IMPL_INLINE buffer
buffer_span::to_shared_storage() const noexcept
{
    return buffer{ data_, size_, buffer_storage_t::large };
}

MEMEPP__IMPL_INLINE string
buffer_span::to_string() const
{
	return string(reinterpret_cast<const char*>(data_), size_);
}

MEMEPP__IMPL_INLINE variable_buffer
buffer_span::to_variable_buffer() const
{
	return variable_buffer(data_, size_);
}

// ============================================================
//  compare
// ============================================================

MEMEPP__IMPL_INLINE int
buffer_span::compare(buffer_span _v) const MEGOPP__NOEXCEPT
{
	const size_type min_size = (size_ < _v.size_) ? size_ : _v.size_;
	/* Two empty ranges compare equal.  memcmp with a null argument is
	 * undefined even when the count is zero. */
	if (min_size <= 0)
	{
		if (size_ < _v.size_) return -1;
		if (size_ > _v.size_) return 1;
		return 0;
	}
	const int cmp = std::memcmp(data_, _v.data_, min_size);
	if (cmp != 0) return cmp;
	if (size_ < _v.size_) return -1;
	if (size_ > _v.size_) return 1;
	return 0;
}

MEMEPP__IMPL_INLINE int
buffer_span::compare(size_type _pos1, size_type _count1, buffer_span _v) const
{
	return slice(_pos1, _count1).compare(_v);
}

MEMEPP__IMPL_INLINE int
buffer_span::compare(size_type _pos1, size_type _count1, buffer_span _v,
                     size_type _pos2, size_type _count2) const
{
	return slice(_pos1, _count1).compare(_v.slice(_pos2, _count2));
}

// ============================================================
//  comparison — buffer_span vs buffer_span
// ============================================================

MEMEPP__IMPL_INLINE bool
operator==(const buffer_span& _lhs, const buffer_span& _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(_rhs) == 0;
}

MEMEPP__IMPL_INLINE bool
operator!=(const buffer_span& _lhs, const buffer_span& _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(_rhs) != 0;
}

MEMEPP__IMPL_INLINE bool
operator<(const buffer_span& _lhs, const buffer_span& _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(_rhs) < 0;
}

MEMEPP__IMPL_INLINE bool
operator>(const buffer_span& _lhs, const buffer_span& _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(_rhs) > 0;
}

MEMEPP__IMPL_INLINE bool
operator<=(const buffer_span& _lhs, const buffer_span& _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(_rhs) <= 0;
}

MEMEPP__IMPL_INLINE bool
operator>=(const buffer_span& _lhs, const buffer_span& _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(_rhs) >= 0;
}

} // namespace MMPP_NAMESPACE
};

// ============================================================
//  free functions
// ============================================================

MEMEPP__IMPL_INLINE memepp::buffer_span mm_bspan(const MemeByte_t* _data, size_t _len) MEGOPP__NOEXCEPT
{
	return memepp::buffer_span(_data, static_cast<memepp::buffer_span::size_type>(_len));
}

#endif // !MEMEPP_BUFFER_SPAN_IMPL_HPP_INCLUDED
