

#ifndef MEMEPP_STRING_IMPL_HPP_INCLUDED
#define MEMEPP_STRING_IMPL_HPP_INCLUDED

#include "meme/string.h"
#include <memepp/predef/macro_option.hpp>
#include "memepp/rune_def.hpp"
#include "memepp/string_def.hpp"
#include "memepp/string_view.hpp"
#include "memepp/string_builder.hpp"
#include <memepp/errc.hpp>

#include <string.h>

namespace memepp {

	MEMEPP__IMPL_INLINE string::string() noexcept
	{
		MemeStringStack_init(&data_, MEME_STRING__OBJECT_SIZE);
	}

	MEMEPP__IMPL_INLINE string::string(mmstr_const_t _other)
	{
        *errc() = MemeStringStack_initByOther(&data_, MMSTR__OBJ_SIZE, _other);
#if !MMOPT__EXCEPTION_DISABLED
		throw_errc(*errc());
#endif
	}

	MEMEPP__IMPL_INLINE string::string(native_handle_type&& _other)
	{
		auto other = to_pointer(_other);
		if (MemeString_storageType(other) == MemeString_UnsafeStorageType_view)
		{
			*errc() = MemeStringStack_initByU8bytes(&data_, MEME_STRING__OBJECT_SIZE,
				MemeString_byteData(other), MemeString_byteSize(other));
			MemeStringStack_unInit(&_other, MEME_STRING__OBJECT_SIZE);
#if !MMOPT__EXCEPTION_DISABLED
            throw_errc(*errc());
#endif
		}
		else {
			MemeStringStack_init(&data_, MEME_STRING__OBJECT_SIZE);
			MemeString_swap(to_pointer(data_), other);
			MemeStringStack_unInit(&_other, MEME_STRING__OBJECT_SIZE);
		}
	}

	MEMEPP__IMPL_INLINE string::string(const native_handle_type& _other)
	{
		auto other = to_pointer(_other);
		if (MemeString_storageType(other) == MemeString_UnsafeStorageType_view)
		{
			*errc() = MemeStringStack_initByU8bytes(&data_, MEME_STRING__OBJECT_SIZE,
				MemeString_byteData(other), MemeString_byteSize(other));
		}
		else {
			*errc() = MemeStringStack_initByOther(&data_, MEME_STRING__OBJECT_SIZE, other);
		}
#if !MMOPT__EXCEPTION_DISABLED
		throw_errc(*errc());
#endif
	}

	//inline string::string(const string& _other, string_storage_type _suggest)
	//{
	//	MemeStringStack_init_with_type(
	//		&data_, MEME_STRING__OBJECT_SIZE, static_cast<MemeString_Storage_t>(_suggest));
	//}

	MEMEPP__IMPL_INLINE string::string(string && _other) noexcept
	{
		MemeStringStack_init(&data_, MEME_STRING__OBJECT_SIZE);
		MemeString_swap(to_pointer(data_), to_pointer(_other.data_));
	}

	MEMEPP__IMPL_INLINE string::string(const string & _other)
	{
		*errc() = MemeStringStack_initByOther(
			&data_, MEME_STRING__OBJECT_SIZE, to_pointer(_other.data_));
#if !MMOPT__EXCEPTION_DISABLED
		throw_errc(*errc());
#endif
	}
    
	MEMEPP__IMPL_INLINE string::string(const char* _utf8)
	{
		*errc() = MemeStringStack_initByU8bytes(&data_, MEME_STRING__OBJECT_SIZE,
			reinterpret_cast<const uint8_t*>(_utf8), -1);
#if !MMOPT__EXCEPTION_DISABLED
		throw_errc(*errc());
#endif
	}

	MEMEPP__IMPL_INLINE string::string(const char* _utf8, size_type _size)
	{
		*errc() = MemeStringStack_initByU8bytes(&data_, MEME_STRING__OBJECT_SIZE,
			reinterpret_cast<const uint8_t*>(_utf8), _size);
#if !MMOPT__EXCEPTION_DISABLED
		throw_errc(*errc());
#endif
	}

	MEMEPP__IMPL_INLINE string::string(
		const char* _utf8, size_type _size, string_storage_t _suggest)
	{
		*errc() = MemeStringStack_initByU8bytesAndType(&data_, MEME_STRING__OBJECT_SIZE,
            reinterpret_cast<const uint8_t*>(_utf8), _size, static_cast<mmstr_strg_t>(_suggest));
#if !MMOPT__EXCEPTION_DISABLED
		throw_errc(*errc());
#endif
	}

	MEMEPP__IMPL_INLINE string::string(const_pointer _utf8)
	{
		*errc() = MemeStringStack_initByU8bytes(&data_, MEME_STRING__OBJECT_SIZE, _utf8, -1);
#if !MMOPT__EXCEPTION_DISABLED
		throw_errc(*errc());
#endif
	}

	MEMEPP__IMPL_INLINE string::string(const_pointer _utf8, size_type _size)
	{
		*errc() = MemeStringStack_initByU8bytes(&data_, MEME_STRING__OBJECT_SIZE, _utf8, _size);
#if !MMOPT__EXCEPTION_DISABLED
		throw_errc(*errc());
#endif
	}

	MEMEPP__IMPL_INLINE string::string(
		const_pointer _utf8, size_type _size, string_storage_t _suggest)
	{
		*errc() = MemeStringStack_initByU8bytesAndType(&data_, MEME_STRING__OBJECT_SIZE,
            _utf8, _size, static_cast<MemeString_Storage_t>(_suggest));
#if !MMOPT__EXCEPTION_DISABLED
		throw_errc(*errc());
#endif
	}

	MEMEPP__IMPL_INLINE string::string(const uint16_t* _utf16, size_type _size)
	{
        *errc() = MemeStringStack_initByU16bytes(&data_, MMS__OBJECT_SIZE, _utf16, _size);
#if !MMOPT__EXCEPTION_DISABLED
        throw_errc(*errc());
#endif
	}
	
	MEMEPP__IMPL_INLINE string::string(const uint16_t* _utf16, size_type _size, string_storage_t _suggest)
	{
        *errc() = MemeStringStack_initByU16bytesAndType(&data_, MMS__OBJECT_SIZE,
            _utf16, _size, static_cast<MemeString_Storage_t>(_suggest));
#if !MMOPT__EXCEPTION_DISABLED
        throw_errc(*errc());
#endif
	}
	
	MEMEPP__IMPL_INLINE string::string(const rune& _ch) noexcept
	{
		*errc() = MemeStringStack_initByU8bytes(&data_, MEME_STRING__OBJECT_SIZE, _ch.data(), _ch.size());
//#if !MMOPT__EXCEPTION_DISABLED
//		throw_errc(*errc());
//#endif
	}


	MEMEPP__IMPL_INLINE string::string(const string_builder& _builder)
	{
		MemeStringStack_init(&data_, MEME_STRING__OBJECT_SIZE);
		*this = _builder.generate();
	}
    
	MEMEPP__IMPL_INLINE string::~string()
	{
		MemeStringStack_unInit(&data_, MEME_STRING__OBJECT_SIZE);
	}

	MEMEPP__IMPL_INLINE string & string::operator=(string && _other) noexcept
	{
		MemeString_swap(to_pointer(data_), to_pointer(_other.data_));
		return *this;
	}

	MEMEPP__IMPL_INLINE string & string::operator=(const string & _other)
	{
		*errc() = MemeStringStack_assign(&data_, MEME_STRING__OBJECT_SIZE, to_pointer(_other.data_));
#if !MMOPT__EXCEPTION_DISABLED
		throw_errc(*errc());
#endif
		return *this;
	}

	MEMEPP__IMPL_INLINE string& string::operator=(const string_builder& _builder)
	{
        *this = _builder.generate();
        return *this;
	}

	MEMEPP__IMPL_INLINE string string::operator+(const char* _utf8) const
	{
		if (!_utf8)
			return *this;
		
		return concat(memepp::string_view{ _utf8 });
	}

	MEMEPP__IMPL_INLINE string string::operator+(const string& _other) const
	{
        return concat(_other);
	}

	MEMEPP__IMPL_INLINE string string::operator+(const string_view& _other) const
	{
        return concat(_other);
	}
	
	MEMEPP__IMPL_INLINE string_storage_t string::storage_type() const noexcept
	{
		return static_cast<string_storage_t>(MemeString_storageType(to_pointer(data_)));
	}

	MEMEPP__IMPL_INLINE string::const_reference string::at(size_type _pos) const
	{
		auto p = MemeString_at(memepp::to_pointer(data_), _pos);
#if !MMOPT__EXCEPTION_DISABLED
		if (!p) {
            throw std::out_of_range("string::at");
		}
#endif
		return *p;
	}

	MEMEPP__IMPL_INLINE const char * string::data() const noexcept
	{
		return MemeString_cStr(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE const char * string::c_str() const noexcept
	{
		return MemeString_cStr(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE string::const_pointer string::bytes() const noexcept
	{
		return MemeString_byteData(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE string::size_type string::size() const noexcept
	{
		return MemeString_byteSize(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE bool string::empty() const noexcept
	{
		return MemeString_isNonempty(to_pointer(data_)) == 0;
	}

	MEMEPP__IMPL_INLINE string::size_type string::capacity() const noexcept
	{
		return MemeString_availableByteCapacity(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE string::size_type string::rune_size() const noexcept
	{
        return MemeString_runeSize(to_pointer(data_));
	}
	
	MEMEPP__IMPL_INLINE string::size_type string::u16char_size() const noexcept
	{
        return MemeString_u16CharSize(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE const_iterator string::begin() const noexcept
	{
		return const_iterator(bytes());
	}
	
	MEMEPP__IMPL_INLINE const_iterator string::cbegin() const noexcept
	{
		return const_iterator(bytes());
	}

	MEMEPP__IMPL_INLINE const_iterator string::end() const noexcept
	{
        return const_iterator(bytes() + size());
	}

    MEMEPP__IMPL_INLINE const_iterator string::cend() const noexcept
    {
        return const_iterator(bytes() + size());
    }

	MEMEPP__IMPL_INLINE const_reverse_iterator string::rbegin() const noexcept
	{
        return const_reverse_iterator(bytes() + size() - 1);
	}
	
	MEMEPP__IMPL_INLINE const_reverse_iterator string::crbegin() const noexcept
	{
        return const_reverse_iterator(bytes() + size() - 1);
	}

	MEMEPP__IMPL_INLINE const_reverse_iterator string::rend() const noexcept
	{
        return const_reverse_iterator(bytes() - 1);
	}
	
	MEMEPP__IMPL_INLINE const_reverse_iterator string::crend() const noexcept
	{
        return const_reverse_iterator(bytes() - 1);
	}

	MEMEPP__IMPL_INLINE const_rune_iterator string::rune_begin() const noexcept
	{
		return const_rune_iterator{ bytes(), bytes() + size() };
	}
	
	MEMEPP__IMPL_INLINE const_rune_iterator string::rune_cbegin() const noexcept
	{
		return const_rune_iterator{ bytes(), bytes() + size() };
	}

	MEMEPP__IMPL_INLINE const_rune_iterator string::rune_end() const noexcept
	{
		return const_rune_iterator{ bytes() + size(), bytes(), bytes() + size() };
	}
	
	MEMEPP__IMPL_INLINE const_rune_iterator string::rune_cend() const noexcept
	{
		return const_rune_iterator{ bytes() + size(), bytes(), bytes() + size() };
	}
	
	MEMEPP__IMPL_INLINE const_rune_iterator string::to_rune_iterator(size_type _pos) const noexcept
	{
        if (_pos > size())
            return rune_end();
		
        if (_pos < 0)
            return rune_end();
		
        return const_rune_iterator{ bytes() + _pos, bytes(), bytes() + size() };
	}

	MEMEPP__IMPL_INLINE const_rune_iterator string::to_rune_iterator(const const_iterator& _it) const noexcept
	{
        return const_rune_iterator{ _it.operator->(), bytes(), bytes() + size()};
	}

	MEMEPP__IMPL_INLINE string string::to_large() const noexcept
	{
        return storage_type() == string_storage_t::large ?
            *this : string{ data(), size(), string_storage_t::large };
	}
    
	MEMEPP__IMPL_INLINE void string::swap(string& _other) noexcept
	{
		MemeString_swap(to_pointer(data_), to_pointer(_other.data_));
	}

	MEMEPP__IMPL_INLINE string::size_type string::find(const string& _other, size_type _pos) const noexcept
	{
		return MemeString_indexOfWithOther(
			to_pointer(native_handle()), _pos, to_pointer(_other.native_handle()), 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
	}

    MEMEPP__IMPL_INLINE string::size_type string::find(const char* _utf8, size_type _pos) const noexcept
    {
        return MemeString_indexOfWithUtf8bytes(
            to_pointer(native_handle()), _pos, reinterpret_cast<const uint8_t*>(_utf8), -1, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

    MEMEPP__IMPL_INLINE string::size_type string::find(const char* _utf8, size_type _pos, size_type _count) const noexcept
    {
        return MemeString_indexOfWithUtf8bytes(
            to_pointer(native_handle()), _pos, reinterpret_cast<const uint8_t*>(_utf8), _count, 
			static_cast<mmflag_case_sensit_t>(case_sensitivity_t::all_sensitive));
    }
    
    MEMEPP__IMPL_INLINE string::size_type string::find(const_pointer _utf8, size_type _pos) const noexcept
    {
        return MemeString_indexOfWithUtf8bytes(
            to_pointer(native_handle()), _pos, _utf8, -1, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

    MEMEPP__IMPL_INLINE string::size_type string::find(const_pointer _utf8, size_type _pos, size_type _count) const noexcept
    {
        return MemeString_indexOfWithUtf8bytes(
            to_pointer(native_handle()), _pos, _utf8, _count, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }
    
	MEMEPP__IMPL_INLINE string::size_type string::find(char _ch, size_type _pos) const noexcept
	{
        return MemeString_indexOfWithByte(
            to_pointer(native_handle()), _pos, _ch, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
	}

    MEMEPP__IMPL_INLINE string::size_type string::find(const rune& _ch, size_type _pos) const noexcept
    {
        return MemeString_indexOfWithUtf8bytes(
            to_pointer(native_handle()), _pos, _ch.data(), _ch.size(), 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

	MEMEPP__IMPL_INLINE string::size_type string::rfind(const char* _utf8, size_type _pos) const noexcept
	{
        return MemeString_lastIndexOfWithUtf8bytes(
            to_pointer(native_handle()), _pos, reinterpret_cast<const uint8_t*>(_utf8), -1, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
	}

    MEMEPP__IMPL_INLINE string::size_type string::rfind(const char* _utf8, size_type _pos, size_type _count) const noexcept
    {
        return MemeString_lastIndexOfWithUtf8bytes(
            to_pointer(native_handle()), _pos, reinterpret_cast<const uint8_t*>(_utf8), _count, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

    MEMEPP__IMPL_INLINE string::size_type string::rfind(const_pointer _utf8, size_type _pos) const noexcept
    {
        return MemeString_lastIndexOfWithUtf8bytes(
            to_pointer(native_handle()), _pos, _utf8, -1, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

    MEMEPP__IMPL_INLINE string::size_type string::rfind(const_pointer _utf8, size_type _pos, size_type _count) const noexcept
    {
        return MemeString_lastIndexOfWithUtf8bytes(
            to_pointer(native_handle()), _pos, _utf8, _count, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }
    
    MEMEPP__IMPL_INLINE string::size_type string::rfind(const rune& _ch, size_type _pos) const noexcept
    {
        return MemeString_lastIndexOfWithUtf8bytes(
            to_pointer(native_handle()), _pos, _ch.data(), _ch.size(), 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

	MEMEPP__IMPL_INLINE string::size_type string::index_of(
		const string& _other, case_sensitivity_t _cs) const noexcept
	{
		return MemeString_indexOfWithOther(
			to_pointer(native_handle()), 0, to_pointer(_other.native_handle()), 
			static_cast<mmflag_case_sensit_t>(_cs));
	}

	MEMEPP__IMPL_INLINE string::size_type string::index_of(
		const string_view& _other, case_sensitivity_t _cs) const noexcept
	{
		return MemeString_indexOfWithOther(
			to_pointer(native_handle()), 0, to_pointer(_other.native_handle()), 
			static_cast<mmflag_case_sensit_t>(_cs));
	}

	MEMEPP__IMPL_INLINE string::size_type string::index_of(
		const string_view& _other, bool _full_match, case_sensitivity_t _cs) const noexcept
	{
		return MemeString_indexOfOther(
			to_pointer(native_handle()), 0, -1,
			to_pointer(_other.native_handle()), -1, (_full_match ? 1 : 0),
			static_cast<mmflag_case_sensit_t>(_cs));
	}

	MEMEPP__IMPL_INLINE string::size_type string::index_of(const string_view& _other,
		size_type _offset, size_type _limit, bool _full_match, case_sensitivity_t _cs) const noexcept
	{
		return MemeString_indexOfOther(
			to_pointer(native_handle()), _offset, _limit,
			to_pointer(_other.native_handle()), -1, (_full_match ? 1 : 0),
			static_cast<mmflag_case_sensit_t>(_cs));
	}

	MEMEPP__IMPL_INLINE string::size_type string::index_of(
		const char* _utf8, case_sensitivity_t _cs) const noexcept
	{
		return MemeString_indexOfWithUtf8bytes(
			to_pointer(native_handle()), 0, reinterpret_cast<const uint8_t*>(_utf8), -1, 
			static_cast<mmflag_case_sensit_t>(_cs));
	}

	MEMEPP__IMPL_INLINE string::size_type string::index_of(const char* _utf8, bool _full_match,
		case_sensitivity_t _cs) const noexcept
	{
		return MemeString_indexOfUtf8bytes(
			to_pointer(native_handle()), 0, -1,
			reinterpret_cast<const uint8_t*>(_utf8), -1, (_full_match ? 1 : 0),
			static_cast<mmflag_case_sensit_t>(_cs));
	}
	
	MEMEPP__IMPL_INLINE string::size_type string::index_of(
		const char* _utf8, size_type _u8len, size_type _offset, bool _full_match, case_sensitivity_t _cs) const noexcept
	{
		return MemeString_indexOfUtf8bytes(
			to_pointer(native_handle()), _offset, -1,
			reinterpret_cast<const uint8_t*>(_utf8), _u8len, (_full_match ? 1 : 0),
			static_cast<mmflag_case_sensit_t>(_cs));
	}

	MEMEPP__IMPL_INLINE string::size_type string::last_index_of(const char* _utf8, bool _full_match,
		case_sensitivity_t _cs) const noexcept
	{
        return MemeString_lastIndexOfUtf8bytes(
            to_pointer(native_handle()), 0, -1,
            reinterpret_cast<const uint8_t*>(_utf8), -1, (_full_match ? 1 : 0),
            static_cast<mmflag_case_sensit_t>(_cs));
	}

	MEMEPP__IMPL_INLINE string::size_type string::last_index_of(const char* _utf8, size_type _u8len,
		size_type _offset, size_type _limit, bool _full_match,
		case_sensitivity_t _cs) const noexcept
	{
        return MemeString_lastIndexOfUtf8bytes(
            to_pointer(native_handle()), _offset, _limit,
            reinterpret_cast<const uint8_t*>(_utf8), _u8len, (_full_match ? 1 : 0),
            static_cast<mmflag_case_sensit_t>(_cs));
	}

	MEMEPP__IMPL_INLINE bool string::contains(const string_view& _sv) const noexcept
	{
		return find(_sv.data(), 0, _sv.size()) != npos;
	}
    
    MEMEPP__IMPL_INLINE bool string::contains(const char* _utf8) const noexcept
    {
        return find(_utf8) != npos;
    }
    
	MEMEPP__IMPL_INLINE bool string::contains(const char* _utf8, size_type _count) const noexcept
	{
         return find(_utf8, 0, _count) != npos;
	}

    MEMEPP__IMPL_INLINE bool string::contains(const_pointer _utf8) const noexcept
    {
        return find(_utf8) != npos;
    }

	MEMEPP__IMPL_INLINE bool string::contains(const_pointer _utf8, size_type _count) const noexcept
	{
        return find(_utf8, 0, _count) != npos;
	}
    
	MEMEPP__IMPL_INLINE bool string::contains(char _ch) const noexcept
	{
        return find(_ch) != npos;
	}

    MEMEPP__IMPL_INLINE bool string::contains(const rune& _ch) const noexcept
    {
        return find(_ch) != npos;
    }

	MEMEPP__IMPL_INLINE bool string::is_only_ascii() const noexcept
	{
        int result = 0;
		MemeString_isOnlyAscii(to_pointer(native_handle()), &result);
        return result != 0;
	}

	MEMEPP__IMPL_INLINE bool string::starts_with(const string_view& _sv) const noexcept
	{
        return MemeString_startsMatchWithOther(
            to_pointer(native_handle()), to_pointer(_sv.native_handle()), 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
	}

    MEMEPP__IMPL_INLINE bool string::starts_with(const char* _utf8) const noexcept
    {
        return starts_with(_utf8, -1);
    }

    MEMEPP__IMPL_INLINE bool string::starts_with(const char* _utf8, size_type _count) const noexcept
    {
        return MemeString_startsMatchWithUtf8bytes(
            to_pointer(native_handle()), reinterpret_cast<const uint8_t*>(_utf8), _count, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

    MEMEPP__IMPL_INLINE bool string::starts_with(const_pointer _utf8) const noexcept
    {
        return starts_with(_utf8, -1);
    }

    MEMEPP__IMPL_INLINE bool string::starts_with(const_pointer _utf8, size_type _count) const noexcept
    {
        return MemeString_startsMatchWithUtf8bytes(
            to_pointer(native_handle()), _utf8, _count, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

    //MEMEPP__IMPL_INLINE bool string::starts_with(char _ch) const noexcept
    //{
    //    return MemeString_startsMatchWithByte(
    //        to_pointer(native_handle()), _ch, case_sensitivity_t::all_sensitive);
    //}

    MEMEPP__IMPL_INLINE bool string::starts_with(const rune& _ch) const noexcept
    {
        return MemeString_startsMatchWithUtf8bytes(
            to_pointer(native_handle()), _ch.data(), _ch.size(), 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

	MEMEPP__IMPL_INLINE bool string::ends_with(const string_view& _sv) const noexcept
	{
        return MemeString_endsMatchWithOther(
            to_pointer(native_handle()), to_pointer(_sv.native_handle()), 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

    MEMEPP__IMPL_INLINE bool string::ends_with(const char* _utf8) const noexcept
    {
        return ends_with(_utf8, -1);
    }

    MEMEPP__IMPL_INLINE bool string::ends_with(const char* _utf8, size_type _count) const noexcept
    {
        return MemeString_endsMatchWithUtf8bytes(
            to_pointer(native_handle()), reinterpret_cast<const uint8_t*>(_utf8), _count, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

    MEMEPP__IMPL_INLINE bool string::ends_with(const_pointer _utf8) const noexcept
    {
        return ends_with(_utf8, -1);
    }

    MEMEPP__IMPL_INLINE bool string::ends_with(const_pointer _utf8, size_type _count) const noexcept
    {
        return MemeString_endsMatchWithUtf8bytes(
            to_pointer(native_handle()), _utf8, _count, 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }

    //MEMEPP__IMPL_INLINE bool string::ends_with(char _ch) const noexcept
    //{
    //    return MemeString_endsMatchWithByte(
    //        to_pointer(native_handle()), _ch, case_sensitivity_t::all_sensitive);
    //}

    MEMEPP__IMPL_INLINE bool string::ends_with(const rune& _ch) const noexcept
    {
        return MemeString_endsMatchWithUtf8bytes(
            to_pointer(native_handle()), _ch.data(), _ch.size(), 
			static_cast<mmflag_case_sensit_t>(case_sensit_t::all_sensitive));
    }
    
    MEMEPP__IMPL_INLINE string string::left (size_type _count) const noexcept
    {
        return substr(0, _count);
    }

    MEMEPP__IMPL_INLINE string string::right(size_type _count) const noexcept
    {
        return substr(size() - _count, _count);
    }
    
    MEMEPP__IMPL_INLINE string string::trim_space() const noexcept
    {
        return string(MemeStringStack_trimSpace(&native_handle(), sizeof(data_)));
    }

    MEMEPP__IMPL_INLINE string string::trim_left_space() const noexcept
    {
        return string(MemeStringStack_trimLeftSpace(&native_handle(), sizeof(data_)));
    }

    MEMEPP__IMPL_INLINE string string::trim_right_space() const noexcept
    {
        return string(MemeStringStack_trimRightSpace(&native_handle(), sizeof(data_)));
    }
    
	MEMEPP__IMPL_INLINE string string::substr(size_type _pos, size_type _count) const noexcept
	{
        return string(MemeStringStack_mid(&native_handle(), sizeof(data_), _pos, _count));
	}

	MEMEPP__IMPL_INLINE string string::concat(const string& _other) const noexcept
	{
		return string{ MemeStringStack_concat(&native_handle(), sizeof(data_), &_other.native_handle()) };
	}

	MEMEPP__IMPL_INLINE string string::concat(const string_view& _other) const noexcept
	{
        return string{ MemeStringStack_concat(&native_handle(), sizeof(data_), &_other.native_handle()) };
	}

	MEMEPP__IMPL_INLINE string string::replace(const string_view& _from, const string_view& _to) const noexcept
	{
        return string{ MemeStringStack_replace(&native_handle(), sizeof(data_), 
            _from.data(), _from.size(), _to.data(), _to.size(), -1) };
	}

	MEMEPP__IMPL_INLINE string string::to_en_upper() const noexcept
	{
		return MemeStringStack_toEnUpper(&native_handle(), sizeof(data_));
	}

	MEMEPP__IMPL_INLINE string string::to_en_lower() const noexcept
	{
		return MemeStringStack_toEnLower(&native_handle(), sizeof(data_));
	}

	MEMEPP__IMPL_INLINE const string::native_handle_type & string::native_handle() const noexcept
	{
		return data_;
	}

	MEMEPP__IMPL_INLINE bool operator==(const string& _lhs, const string& _rhs)
	{
		int result = 0;
		MemeString_isEqualWithOther(
			to_pointer(_lhs.native_handle()),
			to_pointer(_rhs.native_handle()), &result);
		return result;
	}

	MEMEPP__IMPL_INLINE bool operator==(const char* _lhs, const string& _rhs)
	{
		int result = 0;
		MemeString_isEqual(
			to_pointer(_rhs.native_handle()), _lhs, -1, &result);
		return result;
	}

	MEMEPP__IMPL_INLINE bool operator==(const string& _lhs, const char* _rhs)
	{
		int result = 0;
		MemeString_isEqual(
			to_pointer(_lhs.native_handle()), _rhs, -1, &result);
		return result;
	}
	
	MEMEPP__IMPL_INLINE bool operator==(const string& _lhs, string::const_pointer _rhs)
	{
        int result = 0;
        MemeString_isEqual(
            to_pointer(_lhs.native_handle()), reinterpret_cast<const char*>(_rhs), -1, &result);
        return result;
	}

	MEMEPP__IMPL_INLINE bool operator==(string::const_pointer _lhs, const string& _rhs)
	{
        int result = 0;
        MemeString_isEqual(
            to_pointer(_rhs.native_handle()), reinterpret_cast<const char*>(_lhs), -1, &result);
        return result;
	}

	MEMEPP__IMPL_INLINE bool operator!=(const string& _lhs, const string& _rhs)
	{
		return !(_lhs == _rhs);
	}

	MEMEPP__IMPL_INLINE bool operator!=(const char* _lhs, const string& _rhs)
	{
		return !(_lhs == _rhs);
	}

	MEMEPP__IMPL_INLINE bool operator!=(const string& _lhs, const char* _rhs)
	{
		return !(_lhs == _rhs);
	}
    
    MEMEPP__IMPL_INLINE bool operator!=(const string& _lhs, string::const_pointer _rhs)
    {
        return !(_lhs == _rhs);
    }

    MEMEPP__IMPL_INLINE bool operator!=(string::const_pointer _lhs, const string& _rhs)
    {
        return !(_lhs == _rhs);
    }
	
    //MEMEPP__IMPL_INLINE bool operator<(const string& _lhs, const string& _rhs)
    //{
    //    int result = 0;
    //    MemeString_isLessWithOther(
    //        to_pointer(_lhs.native_handle()),
    //        to_pointer(_rhs.native_handle()), &result);
    //    return result;
    //}

    //MEMEPP__IMPL_INLINE bool operator<(const char* _lhs, const string& _rhs)
    //{
    //    int result = 0;
    //    MemeString_isLess(
    //        to_pointer(_rhs.native_handle()), _lhs, -1, &result);
    //    return result;
    //}

    //MEMEPP__IMPL_INLINE bool operator<(const string& _lhs, const char* _rhs)
    //{
    //    int result = 0;
    //    MemeString_isLess(
    //        to_pointer(_lhs.native_handle()), _rhs, -1, &result);
    //    return result;
    //}

    //MEMEPP__IMPL_INLINE bool operator<=(const string& _lhs, const string& _rhs)
    //{
    //    return !(_rhs < _lhs);
    //}

    //MEMEPP__IMPL_INLINE bool operator<=(const char* _lhs, const string& _rhs)
    //{
    //    return !(_rhs < _lhs);
    //}

    //MEMEPP__IMPL_INLINE bool operator<=(const string& _lhs, const char* _rhs)
    //{
    //    return !(_rhs < _lhs);
    //}

    //MEMEPP__IMPL_INLINE bool operator>(const string& _lhs, const string& _rhs)
    //{
    //    return _rhs < _lhs;
    //}

    //MEMEPP__IMPL_INLINE bool operator>(const char* _lhs, const string& _rhs)
    //{
    //    return _rhs < _lhs;
    //}

    //MEMEPP__IMPL_INLINE bool operator>(const string& _lhs, const char* _rhs)
    //{
    //    return _rhs < _lhs;
    //}

    //MEMEPP__IMPL_INLINE bool operator>=(const string& _lhs, const string& _rhs)
    //{
    //    return !(_lhs < _rhs);
    //}

    //MEMEPP__IMPL_INLINE bool operator>=(const char* _lhs, const string& _rhs)
    //{
    //    return !(_lhs < _rhs);
    //}

    //MEMEPP__IMPL_INLINE bool operator>=(const string& _lhs, const char* _rhs)
    //{
    //    return !(_lhs < _rhs);
    //}

	MEMEPP__IMPL_INLINE string from_hexadecimals(const uint8_t* _buf, size_t _len)
	{
		MemeStringStack_t stack;
		int result = MemeStringStack_initWithHexadecimals(&stack, sizeof(stack), NULL, 0, _buf, _len);
		if (result)
			return string{};

		return string{ std::move(stack) };
	}

	MEMEPP__IMPL_INLINE string c_format(size_t _size_limit, MEGO_SYMBOL__MSVC_FORMAT_STRING(const char* _fmt),  ...)
	{
        va_list args;
        va_start(args, _fmt);
        string result = MemeStringStack_vformatInCstyle_v2(
			MEME_STRING__OBJECT_SIZE, _size_limit, _fmt, args);
        va_end(args);
        return result;
	}

}; // namespace memepp

	MEMEPP__IMPL_INLINE memepp::string mm_from(const char* _str, size_t _len)
	{
		return memepp::string{ _str, static_cast<MemeInteger_t>(_len) };
	}

	MEMEPP__IMPL_INLINE memepp::string mm_from(const MemeByte_t* _str, size_t _len)
	{
		return memepp::string{ _str, static_cast<MemeInteger_t>(_len) };
	}

	MEMEPP__IMPL_INLINE memepp::string_builder operator+(const char* _lhs, const memepp::string& _rhs)
	{
		memepp::string_builder builder;
        (builder += _lhs) += _rhs;
        return builder;
	}

	MEMEPP__IMPL_INLINE memepp::string operator""_meme(const char* _str, size_t _len)
	{
		return memepp::string{ _str, static_cast<MemeInteger_t>(_len) };
	}

#endif // !MEMEPP_STRING_IMPL_HPP_INCLUDED

