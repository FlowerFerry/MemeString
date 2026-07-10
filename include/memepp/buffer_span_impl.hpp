
#ifndef MEMEPP_BUFFER_SPAN_IMPL_HPP_INCLUDED
#define MEMEPP_BUFFER_SPAN_IMPL_HPP_INCLUDED

#include "memepp/buffer_span_def.hpp"
#include "memepp/buffer_def.hpp"
#include "memepp/variable_buffer_def.hpp"
#include "memepp/buffer_view_def.hpp"
#include "memepp/string_span_def.hpp"
#include "memepp/string_def.hpp"

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
buffer_span::operator[](size_type _pos) const MEGOPP__NOEXCEPT
{
	return data_[_pos];
}

MEMEPP__IMPL_INLINE buffer_span::const_reference
buffer_span::at(size_type _pos) const
{
#if !MMOPT__EXCEPTION_DISABLED
	if (_pos < 0 || _pos >= size_)
		throw std::out_of_range("buffer_span::at");
#endif
	return data_[_pos];
}

MEMEPP__IMPL_INLINE buffer_span::const_reference
buffer_span::front() const MEGOPP__NOEXCEPT
{
	return data_[0];
}

MEMEPP__IMPL_INLINE buffer_span::const_reference
buffer_span::back() const MEGOPP__NOEXCEPT
{
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
	return size_ == 0;
}

// ============================================================
//  modifiers
// ============================================================

MEMEPP__IMPL_INLINE void
buffer_span::remove_prefix(size_type _n) MEGOPP__NOEXCEPT
{
	data_ += _n;
	size_ -= _n;
}

MEMEPP__IMPL_INLINE void
buffer_span::remove_suffix(size_type _n) MEGOPP__NOEXCEPT
{
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
	if (_pos > size_)
		_pos = size_;
	if (_count == npos || _count > size_ - _pos)
		_count = size_ - _pos;
	return buffer_span(data_ + _pos, _count);
}

MEMEPP__IMPL_INLINE buffer_span
buffer_span::first(size_type _count) const MEGOPP__NOEXCEPT
{
	if (_count > size_)
		_count = size_;
	return buffer_span(data_, _count);
}

MEMEPP__IMPL_INLINE buffer_span
buffer_span::last(size_type _count) const MEGOPP__NOEXCEPT
{
	if (_count > size_)
		_count = size_;
	return buffer_span(data_ + size_ - _count, _count);
}

// ============================================================
//  find
// ============================================================

MEMEPP__IMPL_INLINE buffer_span::size_type
buffer_span::find(buffer_span _v, size_type _pos) const MEGOPP__NOEXCEPT
{
	if (_pos < 0) _pos = 0;
	if (_pos > size_) return npos;
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
	if (_pos < 0) _pos = 0;
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
