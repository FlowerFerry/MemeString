
#ifndef MEMEPP_STRING_SPAN_IMPL_HPP_INCLUDED
#define MEMEPP_STRING_SPAN_IMPL_HPP_INCLUDED

#include <meme/string.h>
#include <meme/unsafe/string_view.h>

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

MEMEPP__IMPL_INLINE string_span::string_span() MEGOPP__NOEXCEPT
	: data_(nullptr), size_(0)
{
}

MEMEPP__IMPL_INLINE string_span::string_span(const char* _utf8) MEGOPP__NOEXCEPT
	: data_(reinterpret_cast<const_pointer>(_utf8))
	, size_(_utf8 ? static_cast<size_type>(std::strlen(_utf8)) : 0)
{
}

MEMEPP__IMPL_INLINE string_span::string_span(const char* _utf8, size_type _count) MEGOPP__NOEXCEPT
	: data_(reinterpret_cast<const_pointer>(_utf8)), size_(_count)
{
}

MEMEPP__IMPL_INLINE string_span::string_span(const_pointer _utf8) MEGOPP__NOEXCEPT
	: data_(_utf8)
	, size_(_utf8 ? static_cast<size_type>(
		std::strlen(reinterpret_cast<const char*>(_utf8))) : 0)
{
}

MEMEPP__IMPL_INLINE string_span::string_span(const_pointer _utf8, size_type _count) MEGOPP__NOEXCEPT
	: data_(_utf8), size_(_count)
{
}

MEMEPP__IMPL_INLINE string_span::string_span(const string& _other) MEGOPP__NOEXCEPT
	: data_(_other.bytes()), size_(_other.size())
{
}

// ============================================================
//  element access
// ============================================================

MEMEPP__IMPL_INLINE string_span::const_reference
string_span::operator[](size_type _pos) const MEGOPP__NOEXCEPT
{
	return data_[_pos];
}

MEMEPP__IMPL_INLINE string_span::const_reference
string_span::at(size_type _pos) const
{
#if !MMOPT__EXCEPTION_DISABLED
	if (_pos < 0 || _pos >= size_)
		throw std::out_of_range("string_span::at");
#endif
	return data_[_pos];
}

MEMEPP__IMPL_INLINE string_span::const_reference
string_span::front() const MEGOPP__NOEXCEPT
{
	return data_[0];
}

MEMEPP__IMPL_INLINE string_span::const_reference
string_span::back() const MEGOPP__NOEXCEPT
{
	return data_[size_ - 1];
}

MEMEPP__IMPL_INLINE const char*
string_span::data() const MEGOPP__NOEXCEPT
{
	return reinterpret_cast<const char*>(data_);
}

MEMEPP__IMPL_INLINE string_span::const_pointer
string_span::bytes() const MEGOPP__NOEXCEPT
{
	return data_;
}

// ============================================================
//  capacity
// ============================================================

MEMEPP__IMPL_INLINE string_span::size_type
string_span::size() const MEGOPP__NOEXCEPT
{
	return size_;
}

MEMEPP__IMPL_INLINE size_t
string_span::length() const MEGOPP__NOEXCEPT
{
	return static_cast<size_t>(size_);
}

MEMEPP__IMPL_INLINE bool
string_span::empty() const MEGOPP__NOEXCEPT
{
	return size_ == 0;
}

// ============================================================
//  modifiers
// ============================================================

MEMEPP__IMPL_INLINE void
string_span::remove_prefix(size_type _n) MEGOPP__NOEXCEPT
{
	data_ += _n;
	size_ -= _n;
}

MEMEPP__IMPL_INLINE void
string_span::remove_suffix(size_type _n) MEGOPP__NOEXCEPT
{
	size_ -= _n;
}

MEMEPP__IMPL_INLINE void
string_span::swap(string_span& _other) MEGOPP__NOEXCEPT
{
	std::swap(data_, _other.data_);
	std::swap(size_, _other.size_);
}

// ============================================================
//  private helper
// ============================================================

MEMEPP__IMPL_INLINE MemeStringStack_t
string_span::to_stack_() const MEGOPP__NOEXCEPT
{
	MemeStringStack_t stack;
	MemeStringViewUnsafeStack_init(
		&stack, MEME_STRING__OBJECT_SIZE, data_, size_);
	return stack;
}

// ============================================================
//  iterators
// ============================================================

MEMEPP__IMPL_INLINE const_iterator
string_span::begin() const MEGOPP__NOEXCEPT
{
	return const_iterator{ data_ };
}

MEMEPP__IMPL_INLINE const_iterator
string_span::cbegin() const MEGOPP__NOEXCEPT
{
	return const_iterator{ data_ };
}

MEMEPP__IMPL_INLINE const_iterator
string_span::end() const MEGOPP__NOEXCEPT
{
	return const_iterator{ data_ + size_ };
}

MEMEPP__IMPL_INLINE const_iterator
string_span::cend() const MEGOPP__NOEXCEPT
{
	return const_iterator{ data_ + size_ };
}

MEMEPP__IMPL_INLINE const_reverse_iterator
string_span::rbegin() const MEGOPP__NOEXCEPT
{
	return const_reverse_iterator{ data_ + size_ - 1 };
}

MEMEPP__IMPL_INLINE const_reverse_iterator
string_span::crbegin() const MEGOPP__NOEXCEPT
{
	return const_reverse_iterator{ data_ + size_ - 1 };
}

MEMEPP__IMPL_INLINE const_reverse_iterator
string_span::rend() const MEGOPP__NOEXCEPT
{
	return const_reverse_iterator{ data_ - 1 };
}

MEMEPP__IMPL_INLINE const_reverse_iterator
string_span::crend() const MEGOPP__NOEXCEPT
{
	return const_reverse_iterator{ data_ - 1 };
}

MEMEPP__IMPL_INLINE const_rune_iterator
string_span::rune_begin() const MEGOPP__NOEXCEPT
{
	return const_rune_iterator{ bytes(), bytes() + size_ };
}

MEMEPP__IMPL_INLINE const_rune_iterator
string_span::rune_cbegin() const MEGOPP__NOEXCEPT
{
	return const_rune_iterator{ bytes(), bytes() + size_ };
}

MEMEPP__IMPL_INLINE const_rune_iterator
string_span::rune_end() const MEGOPP__NOEXCEPT
{
	return const_rune_iterator{ bytes() + size_, bytes(), bytes() + size_ };
}

MEMEPP__IMPL_INLINE const_rune_iterator
string_span::rune_cend() const MEGOPP__NOEXCEPT
{
	return const_rune_iterator{ bytes() + size_, bytes(), bytes() + size_ };
}

MEMEPP__IMPL_INLINE const_rune_iterator
string_span::to_rune_iterator(size_type _pos) const MEGOPP__NOEXCEPT
{
	if (_pos > size_)  return rune_end();
	if (_pos < 0)      return rune_end();
	return const_rune_iterator{ bytes() + _pos, bytes(), bytes() + size_ };
}

// ============================================================
//  string operations
// ============================================================

MEMEPP__IMPL_INLINE string_span::size_type
string_span::rune_size() const MEGOPP__NOEXCEPT
{
	auto stack = to_stack_();
	return MemeString_runeSize(to_pointer(stack));
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::u16char_size() const MEGOPP__NOEXCEPT
{
	auto stack = to_stack_();
	return MemeString_u16CharSize(to_pointer(stack));
}

MEMEPP__IMPL_INLINE rune
string_span::rune_front() const MEGOPP__NOEXCEPT
{
	auto stack = to_stack_();
	return MemeString_runeFront(to_pointer(stack));
}

MEMEPP__IMPL_INLINE rune
string_span::rune_back() const MEGOPP__NOEXCEPT
{
	auto stack = to_stack_();
	return MemeString_runeBack(to_pointer(stack));
}

MEMEPP__IMPL_INLINE string_span
string_span::substr(size_type _pos, size_type _count) const MEGOPP__NOEXCEPT
{
	if (_pos > size_)
		_pos = size_;
	if (_count == npos || _count > size_ - _pos)
		_count = size_ - _pos;
	return string_span(data_ + _pos, _count);
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::copy(value_type* _dest, size_type _count, size_type _pos) const
{
	if (_pos < 0 || _pos > size_)
#if !MMOPT__EXCEPTION_DISABLED
		throw std::out_of_range("string_span::copy");
#else
		return 0;
#endif
	size_type rlen = (_count > size_ - _pos) ? size_ - _pos : _count;
	std::memcpy(_dest, data_ + _pos, rlen);
	return rlen;
}

// ============================================================
//  compare
// ============================================================

MEMEPP__IMPL_INLINE int
string_span::compare(string_span _v) const MEGOPP__NOEXCEPT
{
	auto lstack = to_stack_();
	auto rstack = _v.to_stack_();
	return MemeString_compare(to_pointer(lstack), to_pointer(rstack));
}

MEMEPP__IMPL_INLINE int
string_span::compare(size_type _pos1, size_type _count1, string_span _v) const
{
	return substr(_pos1, _count1).compare(_v);
}

MEMEPP__IMPL_INLINE int
string_span::compare(size_type _pos1, size_type _count1, string_span _v,
                     size_type _pos2, size_type _count2) const
{
	return substr(_pos1, _count1).compare(_v.substr(_pos2, _count2));
}

MEMEPP__IMPL_INLINE int
string_span::compare(const char* _s) const
{
	return compare(string_span(_s));
}

MEMEPP__IMPL_INLINE int
string_span::compare(size_type _pos1, size_type _count1, const char* _s) const
{
	return substr(_pos1, _count1).compare(string_span(_s));
}

MEMEPP__IMPL_INLINE int
string_span::compare(size_type _pos1, size_type _count1,
                     const char* _s, size_type _count2) const
{
	return substr(_pos1, _count1).compare(string_span(_s, _count2));
}

// ============================================================
//  starts_with / ends_with / contains
// ============================================================

MEMEPP__IMPL_INLINE bool
string_span::starts_with(string_span _v) const MEGOPP__NOEXCEPT
{
	if (_v.size_ > size_) return false;
	auto lstack = to_stack_();
	auto rstack = _v.to_stack_();
	return MemeString_startsMatchWithOther(
		to_pointer(lstack), to_pointer(rstack),
		static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
}

MEMEPP__IMPL_INLINE bool
string_span::starts_with(char _c) const MEGOPP__NOEXCEPT
{
	return size_ > 0 && static_cast<char>(data_[0]) == _c;
}

MEMEPP__IMPL_INLINE bool
string_span::starts_with(const char* _s) const
{
	return starts_with(string_span(_s));
}

MEMEPP__IMPL_INLINE bool
string_span::starts_with(const_pointer _s) const MEGOPP__NOEXCEPT
{
	return starts_with(string_span(_s));
}

MEMEPP__IMPL_INLINE bool
string_span::ends_with(string_span _v) const MEGOPP__NOEXCEPT
{
	if (_v.size_ > size_) return false;
	auto lstack = to_stack_();
	auto rstack = _v.to_stack_();
	return MemeString_endsMatchWithOther(
		to_pointer(lstack), to_pointer(rstack),
		static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
}

MEMEPP__IMPL_INLINE bool
string_span::ends_with(char _c) const MEGOPP__NOEXCEPT
{
	return size_ > 0 && static_cast<char>(data_[size_ - 1]) == _c;
}

MEMEPP__IMPL_INLINE bool
string_span::ends_with(const char* _s) const
{
	return ends_with(string_span(_s));
}

MEMEPP__IMPL_INLINE bool
string_span::ends_with(const_pointer _s) const MEGOPP__NOEXCEPT
{
	return ends_with(string_span(_s));
}

MEMEPP__IMPL_INLINE bool
string_span::contains(string_span _v) const MEGOPP__NOEXCEPT
{
	return find(_v) != npos;
}

MEMEPP__IMPL_INLINE bool
string_span::contains(char _c) const MEGOPP__NOEXCEPT
{
	return find(_c) != npos;
}

MEMEPP__IMPL_INLINE bool
string_span::contains(const char* _s) const
{
	return find(_s) != npos;
}

MEMEPP__IMPL_INLINE bool
string_span::contains(const_pointer _s) const MEGOPP__NOEXCEPT
{
	return find(_s) != npos;
}

// ============================================================
//  find
// ============================================================

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find(string_span _v, size_type _pos) const MEGOPP__NOEXCEPT
{
	if (_pos < 0) _pos = 0;
	if (_pos > size_) return npos;
	if (_v.size_ == 0) return _pos;
	auto lstack = to_stack_();
	auto rstack = _v.to_stack_();
	return MemeString_indexOfWithUtf8bytes(
		to_pointer(lstack), _pos, _v.bytes(), _v.size_,
		static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find(char _c, size_type _pos) const MEGOPP__NOEXCEPT
{
	if (_pos < 0) _pos = 0;
	for (size_type i = _pos; i < size_; ++i) {
		if (static_cast<char>(data_[i]) == _c)
			return i;
	}
	return npos;
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find(const char* _s, size_type _pos, size_type _count) const
{
	return find(string_span(_s, _count), _pos);
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find(const char* _s, size_type _pos) const
{
	return find(string_span(_s), _pos);
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find(const_pointer _s, size_type _pos, size_type _count) const MEGOPP__NOEXCEPT
{
	return find(string_span(_s, _count), _pos);
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find(const_pointer _s, size_type _pos) const MEGOPP__NOEXCEPT
{
	return find(string_span(_s), _pos);
}

// ============================================================
//  rfind
// ============================================================

MEMEPP__IMPL_INLINE string_span::size_type
string_span::rfind(string_span _v, size_type _pos) const MEGOPP__NOEXCEPT
{
	if (_v.size_ == 0) {
		if (_pos == npos || _pos >= size_)
			return size_;
		return _pos;
	}
	auto lstack = to_stack_();
	auto rstack = _v.to_stack_();
	return MemeString_lastIndexOfWithUtf8bytes(
		to_pointer(lstack), _pos, _v.bytes(), _v.size_,
		static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::rfind(char _c, size_type _pos) const MEGOPP__NOEXCEPT
{
	if (_pos == npos || _pos >= size_) _pos = size_ - 1;
	if (_pos < 0 || size_ == 0) return npos;
	for (size_type i = _pos; i >= 0; --i) {
		if (static_cast<char>(data_[i]) == _c)
			return i;
	}
	return npos;
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::rfind(const char* _s, size_type _pos, size_type _count) const
{
	return rfind(string_span(_s, _count), _pos);
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::rfind(const char* _s, size_type _pos) const
{
	return rfind(string_span(_s), _pos);
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::rfind(const_pointer _s, size_type _pos, size_type _count) const MEGOPP__NOEXCEPT
{
	return rfind(string_span(_s, _count), _pos);
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::rfind(const_pointer _s, size_type _pos) const MEGOPP__NOEXCEPT
{
	return rfind(string_span(_s), _pos);
}

// ============================================================
//  find_first_of
// ============================================================

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find_first_of(string_span _v, size_type _pos) const MEGOPP__NOEXCEPT
{
	if (_pos < 0) _pos = 0;
	for (size_type i = _pos; i < size_; ++i) {
		if (_v.find(data_[i]) != npos)
			return i;
	}
	return npos;
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find_first_of(char _c, size_type _pos) const MEGOPP__NOEXCEPT
{
	return find(_c, _pos);
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find_first_of(const char* _s, size_type _pos, size_type _count) const
{
	return find_first_of(string_span(_s, _count), _pos);
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find_first_of(const char* _s, size_type _pos) const
{
	return find_first_of(string_span(_s), _pos);
}

// ============================================================
//  find_first_not_of
// ============================================================

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find_first_not_of(string_span _v, size_type _pos) const MEGOPP__NOEXCEPT
{
	if (_pos < 0) _pos = 0;
	for (size_type i = _pos; i < size_; ++i) {
		if (_v.find(data_[i]) == npos)
			return i;
	}
	return npos;
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find_first_not_of(char _c, size_type _pos) const MEGOPP__NOEXCEPT
{
	if (_pos < 0) _pos = 0;
	for (size_type i = _pos; i < size_; ++i) {
		if (static_cast<char>(data_[i]) != _c)
			return i;
	}
	return npos;
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find_first_not_of(const char* _s, size_type _pos, size_type _count) const
{
	return find_first_not_of(string_span(_s, _count), _pos);
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find_first_not_of(const char* _s, size_type _pos) const
{
	return find_first_not_of(string_span(_s), _pos);
}

// ============================================================
//  find_last_of
// ============================================================

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find_last_of(string_span _v, size_type _pos) const MEGOPP__NOEXCEPT
{
	if (_pos == npos || _pos >= size_) _pos = size_ - 1;
	if (_pos < 0 || size_ == 0) return npos;
	for (size_type i = _pos; i >= 0; --i) {
		if (_v.find(data_[i]) != npos)
			return i;
	}
	return npos;
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find_last_of(char _c, size_type _pos) const MEGOPP__NOEXCEPT
{
	return rfind(_c, _pos);
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find_last_of(const char* _s, size_type _pos, size_type _count) const
{
	return find_last_of(string_span(_s, _count), _pos);
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find_last_of(const char* _s, size_type _pos) const
{
	return find_last_of(string_span(_s), _pos);
}

// ============================================================
//  find_last_not_of
// ============================================================

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find_last_not_of(string_span _v, size_type _pos) const MEGOPP__NOEXCEPT
{
	if (_pos == npos || _pos >= size_) _pos = size_ - 1;
	if (_pos < 0 || size_ == 0) return npos;
	for (size_type i = _pos; i >= 0; --i) {
		if (_v.find(data_[i]) == npos)
			return i;
	}
	return npos;
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find_last_not_of(char _c, size_type _pos) const MEGOPP__NOEXCEPT
{
	if (_pos == npos || _pos >= size_) _pos = size_ - 1;
	if (_pos < 0 || size_ == 0) return npos;
	for (size_type i = _pos; i >= 0; --i) {
		if (static_cast<char>(data_[i]) != _c)
			return i;
	}
	return npos;
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find_last_not_of(const char* _s, size_type _pos, size_type _count) const
{
	return find_last_not_of(string_span(_s, _count), _pos);
}

MEMEPP__IMPL_INLINE string_span::size_type
string_span::find_last_not_of(const char* _s, size_type _pos) const
{
	return find_last_not_of(string_span(_s), _pos);
}

// ============================================================
//  conversion
// ============================================================

MEMEPP__IMPL_INLINE string
string_span::to_string() const
{
	return string(data(), size_);
}

// ============================================================
//  comparison — string_span vs string_span
// ============================================================

MEMEPP__IMPL_INLINE bool
operator==(const string_span& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(_rhs) == 0;
}

MEMEPP__IMPL_INLINE bool
operator!=(const string_span& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(_rhs) != 0;
}

MEMEPP__IMPL_INLINE bool
operator<(const string_span& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(_rhs) < 0;
}

MEMEPP__IMPL_INLINE bool
operator>(const string_span& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(_rhs) > 0;
}

MEMEPP__IMPL_INLINE bool
operator<=(const string_span& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(_rhs) <= 0;
}

MEMEPP__IMPL_INLINE bool
operator>=(const string_span& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(_rhs) >= 0;
}

// ============================================================
//  comparison — string vs string_span
// ============================================================

MEMEPP__IMPL_INLINE bool
operator==(const string& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return string_span(_lhs).compare(_rhs) == 0;
}

MEMEPP__IMPL_INLINE bool
operator==(const string_span& _lhs, const string& _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(string_span(_rhs)) == 0;
}

MEMEPP__IMPL_INLINE bool
operator!=(const string& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return !(_lhs == _rhs);
}

MEMEPP__IMPL_INLINE bool
operator!=(const string_span& _lhs, const string& _rhs) MEGOPP__NOEXCEPT
{
	return !(_lhs == _rhs);
}

MEMEPP__IMPL_INLINE bool
operator<(const string& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return string_span(_lhs).compare(_rhs) < 0;
}

MEMEPP__IMPL_INLINE bool
operator<(const string_span& _lhs, const string& _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(string_span(_rhs)) < 0;
}

MEMEPP__IMPL_INLINE bool
operator>(const string& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return string_span(_lhs).compare(_rhs) > 0;
}

MEMEPP__IMPL_INLINE bool
operator>(const string_span& _lhs, const string& _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(string_span(_rhs)) > 0;
}

MEMEPP__IMPL_INLINE bool
operator<=(const string& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return !(_lhs > _rhs);
}

MEMEPP__IMPL_INLINE bool
operator<=(const string_span& _lhs, const string& _rhs) MEGOPP__NOEXCEPT
{
	return !(_lhs > _rhs);
}

MEMEPP__IMPL_INLINE bool
operator>=(const string& _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return !(_lhs < _rhs);
}

MEMEPP__IMPL_INLINE bool
operator>=(const string_span& _lhs, const string& _rhs) MEGOPP__NOEXCEPT
{
	return !(_lhs < _rhs);
}

// ============================================================
//  comparison — const char* vs string_span
// ============================================================

MEMEPP__IMPL_INLINE bool
operator==(const char* _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return string_span(_lhs).compare(_rhs) == 0;
}

MEMEPP__IMPL_INLINE bool
operator==(const string_span& _lhs, const char* _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(string_span(_rhs)) == 0;
}

MEMEPP__IMPL_INLINE bool
operator!=(const char* _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return !(_lhs == _rhs);
}

MEMEPP__IMPL_INLINE bool
operator!=(const string_span& _lhs, const char* _rhs) MEGOPP__NOEXCEPT
{
	return !(_lhs == _rhs);
}

MEMEPP__IMPL_INLINE bool
operator<(const char* _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return string_span(_lhs).compare(_rhs) < 0;
}

MEMEPP__IMPL_INLINE bool
operator<(const string_span& _lhs, const char* _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(string_span(_rhs)) < 0;
}

MEMEPP__IMPL_INLINE bool
operator>(const char* _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return string_span(_lhs).compare(_rhs) > 0;
}

MEMEPP__IMPL_INLINE bool
operator>(const string_span& _lhs, const char* _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(string_span(_rhs)) > 0;
}

MEMEPP__IMPL_INLINE bool
operator<=(const char* _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return !(_lhs > _rhs);
}

MEMEPP__IMPL_INLINE bool
operator<=(const string_span& _lhs, const char* _rhs) MEGOPP__NOEXCEPT
{
	return !(_lhs > _rhs);
}

MEMEPP__IMPL_INLINE bool
operator>=(const char* _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return !(_lhs < _rhs);
}

MEMEPP__IMPL_INLINE bool
operator>=(const string_span& _lhs, const char* _rhs) MEGOPP__NOEXCEPT
{
	return !(_lhs < _rhs);
}

// ============================================================
//  comparison — const_pointer vs string_span
// ============================================================

MEMEPP__IMPL_INLINE bool
operator==(string_span::const_pointer _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return string_span(_lhs).compare(_rhs) == 0;
}

MEMEPP__IMPL_INLINE bool
operator==(const string_span& _lhs, string_span::const_pointer _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(string_span(_rhs)) == 0;
}

MEMEPP__IMPL_INLINE bool
operator!=(string_span::const_pointer _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return !(_lhs == _rhs);
}

MEMEPP__IMPL_INLINE bool
operator!=(const string_span& _lhs, string_span::const_pointer _rhs) MEGOPP__NOEXCEPT
{
	return !(_lhs == _rhs);
}

MEMEPP__IMPL_INLINE bool
operator<(string_span::const_pointer _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return string_span(_lhs).compare(_rhs) < 0;
}

MEMEPP__IMPL_INLINE bool
operator<(const string_span& _lhs, string_span::const_pointer _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(string_span(_rhs)) < 0;
}

MEMEPP__IMPL_INLINE bool
operator>(string_span::const_pointer _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return string_span(_lhs).compare(_rhs) > 0;
}

MEMEPP__IMPL_INLINE bool
operator>(const string_span& _lhs, string_span::const_pointer _rhs) MEGOPP__NOEXCEPT
{
	return _lhs.compare(string_span(_rhs)) > 0;
}

MEMEPP__IMPL_INLINE bool
operator<=(string_span::const_pointer _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return !(_lhs > _rhs);
}

MEMEPP__IMPL_INLINE bool
operator<=(const string_span& _lhs, string_span::const_pointer _rhs) MEGOPP__NOEXCEPT
{
	return !(_lhs > _rhs);
}

MEMEPP__IMPL_INLINE bool
operator>=(string_span::const_pointer _lhs, const string_span& _rhs) MEGOPP__NOEXCEPT
{
	return !(_lhs < _rhs);
}

MEMEPP__IMPL_INLINE bool
operator>=(const string_span& _lhs, string_span::const_pointer _rhs) MEGOPP__NOEXCEPT
{
	return !(_lhs < _rhs);
}

} // namespace MMPP_NAMESPACE
};

// ============================================================
//  free functions
// ============================================================

MEMEPP__IMPL_INLINE memepp::string_span mm_span(const char* _str, size_t _len) MEGOPP__NOEXCEPT
{
	return memepp::string_span(_str, static_cast<MemeInteger_t>(_len));
}

MEMEPP__IMPL_INLINE memepp::string_span mm_span(const MemeByte_t* _str, size_t _len) MEGOPP__NOEXCEPT
{
	return memepp::string_span(_str, static_cast<MemeInteger_t>(_len));
}

MEMEPP__IMPL_INLINE memepp::string_span operator""_meme_span(const char* _str, size_t _len) MEGOPP__NOEXCEPT
{
	return memepp::string_span(_str, static_cast<MemeInteger_t>(_len));
}

#endif // !MEMEPP_STRING_SPAN_IMPL_HPP_INCLUDED
