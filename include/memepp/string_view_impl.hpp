
#ifndef MEMEPP_STRING_VIEW_IMPL_HPP_INCLUDED
#define MEMEPP_STRING_VIEW_IMPL_HPP_INCLUDED

#include "meme/string.h"
#include "meme/unsafe/string_view.h"

#include "memepp/string_def.hpp"
#include "memepp/string_view_def.hpp"
#include "memepp/string_builder_def.hpp"

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

	MEMEPP__IMPL_INLINE string_view::string_view() noexcept
	{
		MemeStringStack_init(&data_, MEME_STRING__OBJECT_SIZE);
	}

	MEMEPP__IMPL_INLINE string_view::string_view(mmstr_const_t _other) noexcept
	{
		if (MemeString_isSharedStorageTypes(_other) == 1)
		{
			MemeStringStack_initByOther(&data_, MMSTR__OBJ_SIZE, _other);
		}
		else {
			MemeStringViewUnsafeStack_init(&data_, MMSTR__OBJ_SIZE,
				MemeString_byteData(_other), MemeString_byteSize(_other));
		}
	}

	MEMEPP__IMPL_INLINE string_view::string_view(const MemeStringStack_t& _stack) noexcept
	{
		auto other = to_pointer(_stack);
		if (MemeString_isSharedStorageTypes(other) == 1)
		{
			MemeStringStack_initByOther(&data_, MEME_STRING__OBJECT_SIZE, other);
		}
		else {
			MemeStringViewUnsafeStack_init(&data_, MEME_STRING__OBJECT_SIZE, 
				MemeString_byteData(other), MemeString_byteSize(other));
		}
	}

	MEMEPP__IMPL_INLINE string_view::string_view(const char* _utf8) noexcept
	{
		MemeStringViewUnsafeStack_init(&data_, MEME_STRING__OBJECT_SIZE,
			reinterpret_cast<const uint8_t*>(_utf8), -1);
	}

	MEMEPP__IMPL_INLINE string_view::string_view(const char* _utf8, size_type _size) noexcept
	{
		MemeStringViewUnsafeStack_init(&data_, MEME_STRING__OBJECT_SIZE, 
			reinterpret_cast<const uint8_t*>(_utf8), _size);
	}

	MEMEPP__IMPL_INLINE string_view::string_view(const_pointer _utf8) noexcept
	{
		MemeStringViewUnsafeStack_init(&data_, MEME_STRING__OBJECT_SIZE, _utf8, -1);
	}

	MEMEPP__IMPL_INLINE string_view::string_view(const_pointer _utf8, size_type _size) noexcept
	{
		MemeStringViewUnsafeStack_init(&data_, MEME_STRING__OBJECT_SIZE, _utf8, _size);
	}

	MEMEPP__IMPL_INLINE string_view::string_view(const string& _other) noexcept
	{
		auto other = to_pointer(_other.native_handle());
		if (MemeString_isSharedStorageTypes(other) == 1)
		{
			MemeStringStack_initByOther(&data_, MEME_STRING__OBJECT_SIZE, other);
		}
		else {
			MemeStringViewUnsafeStack_init(&data_, MEME_STRING__OBJECT_SIZE, _other.bytes(), _other.size());
		}
	}

	MEMEPP__IMPL_INLINE string_view::string_view(string&& _other)
	{
		auto other = to_pointer(_other.native_handle());
		if (MemeString_isSharedStorageTypes(other) == 1)
		{
			MemeStringStack_initByOther(&data_, MMS__OBJECT_SIZE, other);
		}
		else {
			MemeStringStack_init(&data_, MMS__OBJECT_SIZE);
            memepp::string s{ _other.data(), _other.size(), memepp::string_storage_t::large };
            *this = memepp::string_view{ s };
		}
	}

	MEMEPP__IMPL_INLINE string_view::string_view(const string_view& _other) noexcept
	{
		MemeStringViewUnsafeStack_initByOther(
			&data_, MEME_STRING__OBJECT_SIZE, &_other.data_);
	}

	MEMEPP__IMPL_INLINE string_view::string_view(string_view&& _other) noexcept
	{
		MemeStringStack_init(&data_, MEME_STRING__OBJECT_SIZE);
		MemeString_swap(to_pointer(data_), to_pointer(_other.data_));
	}

	MEMEPP__IMPL_INLINE memepp::string_view::~string_view()
	{
		MemeStringStack_unInit(&data_, MEME_STRING__OBJECT_SIZE);
	}
	
	MEMEPP__IMPL_INLINE string_storage_t string_view::storage_type() const noexcept
	{
		return static_cast<string_storage_t>(MemeString_storageType(to_pointer(data_)));
	}

	MEMEPP__IMPL_INLINE string_view& string_view::operator=(const string& _other)
	{
		auto other = to_pointer(_other.native_handle());
		if (MemeString_isSharedStorageTypes(other) == 1)
		{
			MemeStringStack_assign(&data_, MEME_STRING__OBJECT_SIZE, to_pointer(const_cast<MemeStringStack_t&>(_other.native_handle())));
		}
		else {
			*this = memepp::string_view { _other.bytes(), _other.size() };
		}
		return *this;
	}

	MEMEPP__IMPL_INLINE string_view& string_view::operator=(const string_view& _other)
	{
		MemeStringViewUnsafeStack_assignByOther(&data_, MEME_STRING__OBJECT_SIZE, &_other.data_);
		return *this;
	}

	MEMEPP__IMPL_INLINE string_view& string_view::operator=(string_view&& _other) noexcept
	{
		MemeString_swap(to_pointer(data_), to_pointer(_other.data_));
		return *this;
	}

	MEMEPP__IMPL_INLINE string_builder string_view::operator+(const string& _other) const
	{
		return string_builder{} + *this + _other;
	}

	MEMEPP__IMPL_INLINE string_builder string_view::operator+(const string_view& _other) const
	{
		return string_builder{} + *this + _other;
	}

	MEMEPP__IMPL_INLINE string_builder string_view::operator+(const char* _other) const
	{
		return string_builder{} + *this + _other;
	}

	MEMEPP__IMPL_INLINE string_view::const_reference string_view::at(size_type _pos) const
	{
		auto p = MemeString_at(memepp::to_pointer(data_), _pos);
#if !MMOPT__EXCEPTION_DISABLED
		if (!p) {
			throw std::out_of_range("string_view::at");
		}
#endif
		return *p;
	}

	MEMEPP__IMPL_INLINE const char* string_view::data() const noexcept
	{
		return MemeString_cStr(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE string_view::size_type string_view::size() const noexcept
	{
		return static_cast<size_t>(MemeString_byteSize(to_pointer(data_)));
	}

	MEMEPP__IMPL_INLINE bool string_view::empty() const noexcept
	{
		return MemeString_isNonempty(to_pointer(data_)) == 0;
	}

    MEMEPP__IMPL_INLINE string_view::const_pointer string_view::bytes() const noexcept
    {
        return MemeString_byteData(to_pointer(data_));
    }

	MEMEPP__IMPL_INLINE string_view::size_type string_view::u16char_size() const noexcept
	{
		return MemeString_u16CharSize(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE const_iterator string_view::begin() const noexcept
	{
		return const_iterator{ bytes() };
	}

    MEMEPP__IMPL_INLINE const_iterator string_view::end() const noexcept
    {
        return const_iterator{ bytes() + size() };
    }

    MEMEPP__IMPL_INLINE const_iterator string_view::cbegin() const noexcept
    {
        return const_iterator{ bytes() };
    }

    MEMEPP__IMPL_INLINE const_iterator string_view::cend() const noexcept
    {
        return const_iterator{ bytes() + size() };
    }

	MEMEPP__IMPL_INLINE const_rune_iterator string_view::rune_begin() const noexcept
	{
		return const_rune_iterator{ bytes(), bytes() + size() };
	}

	MEMEPP__IMPL_INLINE const_rune_iterator string_view::rune_cbegin() const noexcept
	{
		return const_rune_iterator{ bytes(), bytes() + size() };
	}

	MEMEPP__IMPL_INLINE const_rune_iterator string_view::rune_end() const noexcept
	{
		return const_rune_iterator{ bytes() + size(), bytes(), bytes() + size() };
	}

	MEMEPP__IMPL_INLINE const_rune_iterator string_view::rune_cend() const noexcept
	{
		return const_rune_iterator{ bytes() + size(), bytes(), bytes() + size() };
	}
	
	MEMEPP__IMPL_INLINE const_rune_iterator string_view::to_rune_iterator(size_type _pos) const noexcept
	{
		if (_pos > size())
			return rune_end();

		if (_pos < 0)
			return rune_end();

		return const_rune_iterator{ bytes() + _pos, bytes(), bytes() + size() };
	}

	MEMEPP__IMPL_INLINE const_rune_iterator string_view::to_rune_iterator(const const_iterator& _it) const noexcept
	{
		return const_rune_iterator{ _it.operator->(), bytes(), bytes() + size() };
	}

	MEMEPP__IMPL_INLINE string string_view::to_string() const
	{
		if (MemeString_storageType(to_pointer(data_)) == MemeString_UnsafeStorageType_view)
			return string { data(), size() };

		mmsstk_t stack;
		MemeStringStack_initByOther(
			&stack, MMS__OBJECT_SIZE, to_pointer(data_));
		// TO_DO
		return string { reinterpret_cast<MemeStringStack_t&&>(stack) };
	}

	MEMEPP__IMPL_INLINE string string_view::to_large() const noexcept
	{
        return storage_type() == string_storage_t::large ?
            to_string() : string{ data(), size(), string_storage_t::large };
	}
    
	MEMEPP__IMPL_INLINE string_view::size_type string_view::find(
		const string_view& _other, size_type _pos) const noexcept
	{
		return MemeString_indexOfWithOther(
			to_pointer(native_handle()), _pos, to_pointer(_other.native_handle()), 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
	}

    MEMEPP__IMPL_INLINE string_view::size_type string_view::find(const char* _utf8, size_type _pos) const noexcept
    {
        return MemeString_indexOfWithUtf8bytes(
            to_pointer(native_handle()), _pos, reinterpret_cast<const uint8_t*>(_utf8), -1, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

    MEMEPP__IMPL_INLINE string_view::size_type string_view::find(
		const char* _utf8, size_type _pos, size_type _size) const noexcept
    {
        return MemeString_indexOfWithUtf8bytes(
            to_pointer(native_handle()), _pos, reinterpret_cast<const uint8_t*>(_utf8), _size, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

    MEMEPP__IMPL_INLINE string_view::size_type string_view::find(const_pointer _utf8, size_type _pos) const noexcept
    {
        return MemeString_indexOfWithUtf8bytes(
            to_pointer(native_handle()), _pos, _utf8, -1, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

    MEMEPP__IMPL_INLINE string_view::size_type string_view::find(
		const_pointer _utf8, size_type _pos, size_type ) const noexcept
    {
        return MemeString_indexOfWithUtf8bytes(
            to_pointer(native_handle()), _pos, _utf8, -1, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

	MEMEPP__IMPL_INLINE string_view::size_type string_view::find(char _ch, size_type _pos) const noexcept
	{
        return MemeString_indexOfWithByte(
            to_pointer(native_handle()), _pos, _ch, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
	}
    
	MEMEPP__IMPL_INLINE string_view::size_type string_view::find(const rune& _ch, size_type _pos) const noexcept
	{
        return MemeString_indexOfWithUtf8bytes(
            to_pointer(native_handle()), _pos, _ch.data(), _ch.size(), 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
	}

	MEMEPP__IMPL_INLINE string_view::size_type string_view::rfind(const char* _utf8, size_type _pos) const noexcept
	{
        return MemeString_lastIndexOfWithUtf8bytes(
            to_pointer(native_handle()), _pos, reinterpret_cast<const uint8_t*>(_utf8), -1, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
	}

	MEMEPP__IMPL_INLINE string_view::size_type string_view::rfind(const char* _utf8, size_type _pos, size_type _substr_count) const noexcept
	{
        return MemeString_lastIndexOfWithUtf8bytes(
            to_pointer(native_handle()), _pos, reinterpret_cast<const uint8_t*>(_utf8), _substr_count, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
	}

	MEMEPP__IMPL_INLINE string_view::size_type string_view::rfind(const_pointer _utf8, size_type _pos) const noexcept
	{
        return MemeString_lastIndexOfWithUtf8bytes(
            to_pointer(native_handle()), _pos, _utf8, -1, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
	}
	
	MEMEPP__IMPL_INLINE string_view::size_type string_view::rfind(const_pointer _utf8, size_type _pos, size_type _substr_count) const noexcept
	{
        return MemeString_lastIndexOfWithUtf8bytes(
            to_pointer(native_handle()), _pos, _utf8, _substr_count, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
	}
	
	MEMEPP__IMPL_INLINE string_view::size_type string_view::rfind(const rune& _ch, size_type _pos) const noexcept
	{
        return MemeString_lastIndexOfWithUtf8bytes(
            to_pointer(native_handle()), _pos, _ch.data(), _ch.size(), 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
	}

	MEMEPP__IMPL_INLINE string_view::size_type string_view::index_of(const string_view& _other, case_sensitivity_t _cs) const noexcept
	{
        return MemeString_indexOfWithOther(
            to_pointer(native_handle()), 0, to_pointer(_other.native_handle()), 
			static_cast<mmflag_case_sensit_t>(_cs));
	}

	MEMEPP__IMPL_INLINE string_view::size_type string_view::index_of(
		const string_view& _other, bool _full_match, case_sensitivity_t _cs) const noexcept
	{
        return MemeString_indexOfOther(
            to_pointer(native_handle()), 0, -1, 
			to_pointer(_other.native_handle()), -1, (_full_match ? 1 : 0),
            static_cast<mmflag_case_sensit_t>(_cs));
	}

	MEMEPP__IMPL_INLINE string_view::size_type string_view::index_of(const string_view& _other,
		size_type _offset, size_type _limit, bool _full_match, case_sensitivity_t _cs) const noexcept
	{
        return MemeString_indexOfOther(
            to_pointer(native_handle()), _offset, _limit,
            to_pointer(_other.native_handle()), -1, (_full_match ? 1 : 0),
            static_cast<mmflag_case_sensit_t>(_cs));
	}

	MEMEPP__IMPL_INLINE string_view::size_type string_view::index_of(
		const char* _utf8, case_sensitivity_t _cs) const noexcept
	{
		return MemeString_indexOfWithUtf8bytes(
			to_pointer(native_handle()), 0, reinterpret_cast<const uint8_t*>(_utf8), -1, 
			static_cast<mmflag_case_sensit_t>(_cs));
	}

	MEMEPP__IMPL_INLINE string_view::size_type string_view::index_of(
		const char* _utf8, bool _full_match, case_sensitivity_t _cs) const noexcept
	{
        return MemeString_indexOfUtf8bytes(
            to_pointer(native_handle()), 0, -1,
            reinterpret_cast<const uint8_t*>(_utf8), -1, (_full_match ? 1 : 0),
            static_cast<mmflag_case_sensit_t>(_cs));
	}

	MEMEPP__IMPL_INLINE string_view::size_type string_view::last_index_of(const char* _utf8, bool _full_match,
		case_sensitivity_t _cs) const noexcept
	{
        return MemeString_lastIndexOfUtf8bytes(
            to_pointer(native_handle()), 0, -1,
            reinterpret_cast<const uint8_t*>(_utf8), -1, (_full_match ? 1 : 0),
            static_cast<mmflag_case_sensit_t>(_cs));
	}

	MEMEPP__IMPL_INLINE string_view::size_type string_view::last_index_of(const char* _utf8, size_type _u8len,
		size_type _offset, size_type _limit, bool _full_match,
		case_sensitivity_t _cs) const noexcept
	{
        return MemeString_lastIndexOfUtf8bytes(
            to_pointer(native_handle()), _offset, _limit,
            reinterpret_cast<const uint8_t*>(_utf8), _u8len, (_full_match ? 1 : 0),
            static_cast<mmflag_case_sensit_t>(_cs));
	}

	MEMEPP__IMPL_INLINE bool string_view::contains(const string_view& _sv) const noexcept
	{
		return find(_sv) != npos;
	}

    MEMEPP__IMPL_INLINE bool string_view::contains(const char* _utf8) const noexcept
    {
        return find(_utf8) != npos;
    }

    MEMEPP__IMPL_INLINE bool string_view::contains(const char* _utf8, size_type _size) const noexcept
    {
        return find(_utf8, _size) != npos;
    }

    MEMEPP__IMPL_INLINE bool string_view::contains(const_pointer _utf8) const noexcept
    {
        return find(_utf8) != npos;
    }

    MEMEPP__IMPL_INLINE bool string_view::contains(const_pointer _utf8, size_type _size) const noexcept
    {
        return find(_utf8, _size) != npos;
    }

	MEMEPP__IMPL_INLINE bool string_view::contains(char _ch) const noexcept
	{
        return find(_ch) != npos;
	}

	MEMEPP__IMPL_INLINE bool string_view::starts_with(const string_view& _sv) const noexcept
	{
        return MemeString_startsMatchWithOther(
            to_pointer(native_handle()), to_pointer(_sv.native_handle()), 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

    MEMEPP__IMPL_INLINE bool string_view::starts_with(const char* _utf8) const noexcept
    {
        return MemeString_startsMatchWithUtf8bytes(
            to_pointer(native_handle()), 
			reinterpret_cast<const uint8_t*>(_utf8), -1, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

    MEMEPP__IMPL_INLINE bool string_view::starts_with(const char* _utf8, size_type _size) const noexcept
    {
        return MemeString_startsMatchWithUtf8bytes(
            to_pointer(native_handle()), 
			reinterpret_cast<const uint8_t*>(_utf8), _size, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

    MEMEPP__IMPL_INLINE bool string_view::starts_with(const_pointer _utf8) const noexcept
    {
        return MemeString_startsMatchWithUtf8bytes(
            to_pointer(native_handle()), _utf8, -1, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

    MEMEPP__IMPL_INLINE bool string_view::starts_with(const_pointer _utf8, size_type _size) const noexcept
    {
        return MemeString_startsMatchWithUtf8bytes(
            to_pointer(native_handle()), _utf8, _size, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }
    
	MEMEPP__IMPL_INLINE bool string_view::ends_with(const string_view& _sv) const noexcept
	{
        return MemeString_endsMatchWithOther(
            to_pointer(native_handle()), to_pointer(_sv.native_handle()), 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

    MEMEPP__IMPL_INLINE bool string_view::ends_with(const char* _utf8) const noexcept
    {
        return MemeString_endsMatchWithUtf8bytes(
            to_pointer(native_handle()),
            reinterpret_cast<const uint8_t*>(_utf8), -1, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

    MEMEPP__IMPL_INLINE bool string_view::ends_with(const char* _utf8, size_type _size) const noexcept
    {
        return MemeString_endsMatchWithUtf8bytes(
            to_pointer(native_handle()),
            reinterpret_cast<const uint8_t*>(_utf8), _size, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

    MEMEPP__IMPL_INLINE bool string_view::ends_with(const_pointer _utf8) const noexcept
    {
        return MemeString_endsMatchWithUtf8bytes(
            to_pointer(native_handle()), _utf8, -1, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

    MEMEPP__IMPL_INLINE bool string_view::ends_with(const_pointer _utf8, size_type _size) const noexcept
    {
        return MemeString_endsMatchWithUtf8bytes(
            to_pointer(native_handle()), _utf8, _size, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

	MEMEPP__IMPL_INLINE string string_view::to_en_upper() const noexcept
	{
		return MemeStringStack_toEnUpper(&native_handle(), sizeof(data_));
	}

	MEMEPP__IMPL_INLINE string string_view::to_en_lower() const noexcept
	{
		return MemeStringStack_toEnLower(&native_handle(), sizeof(data_));
	}
    
	MEMEPP__IMPL_INLINE string_view string_view::trim_space() const noexcept
	{
        return MemeStringStack_trimSpace(&native_handle(), sizeof(data_));
	}

    MEMEPP__IMPL_INLINE string_view string_view::trim_left_space() const noexcept
    {
        return MemeStringStack_trimLeftSpace(&native_handle(), sizeof(data_));
    }

    MEMEPP__IMPL_INLINE string_view string_view::trim_right_space() const noexcept
    {
        return MemeStringStack_trimRightSpace(&native_handle(), sizeof(data_));
    }

	MEMEPP__IMPL_INLINE string_view string_view::substr(size_type _pos, size_type _count) const noexcept
	{
		if (_count == npos)
            _count = size() - _pos;
		if (_count > size() - _pos)
            _count = size() - _pos;
        return string_view(data() + _pos, _count);
	}

	MEMEPP__IMPL_INLINE string string_view::replace(const string_view& _from, const string_view& _to) const noexcept
	{
		return string{ MemeStringStack_replace(&native_handle(), sizeof(data_),
			_from.data(), _from.size(), _to.data(), _to.size(), -1) };
	}

	MEMEPP__IMPL_INLINE const string_view::native_handle_type& 
		string_view::native_handle() const noexcept
	{
		return data_;
	}

	MEMEPP__IMPL_INLINE bool operator==(const string_view& _lhs, const string_view& _rhs)
	{
		int result = 0;
		MemeString_isEqualWithOther(
			to_pointer(_lhs.native_handle()),
			to_pointer(_rhs.native_handle()), &result);
		return result;
	}

	MEMEPP__IMPL_INLINE bool operator!=(const string_view& _lhs, const string_view& _rhs)
	{
		return !(_lhs == _rhs);
	}

	MEMEPP__IMPL_INLINE bool operator==(const string& _lhs, const string_view& _rhs)
	{
		int result = 0;
		MemeString_isEqualWithOther(
			to_pointer(_lhs.native_handle()),
			to_pointer(_rhs.native_handle()), &result);
		return result;
	}

	MEMEPP__IMPL_INLINE bool operator!=(const string& _lhs, const string_view& _rhs)
	{
		return !(_lhs == _rhs);
	}

	MEMEPP__IMPL_INLINE bool operator==(const string_view& _lhs, const string& _rhs)
	{
		int result = 0;
		MemeString_isEqualWithOther(
			to_pointer(_lhs.native_handle()),
			to_pointer(_rhs.native_handle()), &result);
		return result;
	}

	MEMEPP__IMPL_INLINE bool operator!=(const string_view& _lhs, const string& _rhs)
	{
		return !(_lhs == _rhs);
	}

	MEMEPP__IMPL_INLINE bool operator==(const char* _lhs, const string_view& _rhs)
	{
		int result = 0;
		MemeString_isEqual(
			to_pointer(_rhs.native_handle()), _lhs, -1, &result);
		return result;
	}

	MEMEPP__IMPL_INLINE bool operator!=(const char* _lhs, const string_view& _rhs)
	{
		return !(_lhs == _rhs);
	}

	MEMEPP__IMPL_INLINE bool operator==(const string_view& _lhs, const char* _rhs)
	{
		int result = 0;
		MemeString_isEqual(
			to_pointer(_lhs.native_handle()), _rhs, -1, &result);
		return result;
	}

	MEMEPP__IMPL_INLINE bool operator!=(const string_view& _lhs, const char* _rhs)
	{
		return !(_lhs == _rhs);
	}
};

MEMEPP__IMPL_INLINE memepp::string_view mm_view(const char* _str, size_t _len)
{
	return memepp::string_view{ _str, static_cast<MemeInteger_t>(_len) };
}

MEMEPP__IMPL_INLINE memepp::string_view mm_view(const MemeByte_t* _str, size_t _len)
{
	return memepp::string_view{ _str, static_cast<MemeInteger_t>(_len) };
}

MEMEPP__IMPL_INLINE memepp::string_view operator""_meme_sv(const char* _str, size_t _len)
{
	return memepp::string_view{ _str, static_cast<MemeInteger_t>(_len) };
}

#endif // !MEMEPP_STRING_VIEW_IMPL_HPP_INCLUDED
