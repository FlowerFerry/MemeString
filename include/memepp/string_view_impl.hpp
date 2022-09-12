
#ifndef MEMEPP_STRING_VIEW_IMPL_HPP_INCLUDED
#define MEMEPP_STRING_VIEW_IMPL_HPP_INCLUDED

#include "meme/string.h"
#include "meme/unsafe/view.h"

#include "memepp/string_def.hpp"
#include "memepp/string_view_def.hpp"

#ifndef MEMEPP__IMPL_INLINE
#	ifdef MEMEPP__IMPL_SEPARATE
#		define MEMEPP__IMPL_INLINE 
#	else
#		define MEMEPP__IMPL_INLINE inline
#	endif
#endif 

namespace memepp {

	MEMEPP__IMPL_INLINE string_view::string_view() noexcept
	{
		MemeStringStack_init(&data_, MEME_STRING__OBJECT_SIZE);
	}

	MEMEPP__IMPL_INLINE string_view::string_view(const MemeStringStack_t* _stack)
	{
		auto other = to_pointer(*_stack);
		if (MemeString_isSharedStorageTypes(other) == 1)
		{
			MemeStringStack_initByOther(&data_, MEME_STRING__OBJECT_SIZE, other);
		}
		else {
			MemeStringViewUnsafeStack_init(&data_, MEME_STRING__OBJECT_SIZE, 
				MemeString_byteData(other), MemeString_byteSize(other));
		}
	}

	MEMEPP__IMPL_INLINE string_view::string_view(const char* _utf8)
	{
		MemeStringViewUnsafeStack_init(&data_, MEME_STRING__OBJECT_SIZE,
			reinterpret_cast<const uint8_t*>(_utf8), -1);
	}

	MEMEPP__IMPL_INLINE string_view::string_view(const char* _utf8, size_type _size)
	{
		MemeStringViewUnsafeStack_init(&data_, MEME_STRING__OBJECT_SIZE, 
			reinterpret_cast<const uint8_t*>(_utf8), _size);
	}

	MEMEPP__IMPL_INLINE string_view::string_view(const_pointer _utf8)
	{
		MemeStringViewUnsafeStack_init(&data_, MEME_STRING__OBJECT_SIZE, _utf8, -1);
	}

	MEMEPP__IMPL_INLINE string_view::string_view(const_pointer _utf8, size_type _size)
	{
		MemeStringViewUnsafeStack_init(&data_, MEME_STRING__OBJECT_SIZE, _utf8, _size);
	}

	MEMEPP__IMPL_INLINE string_view::string_view(const string& _other)
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

	MEMEPP__IMPL_INLINE string_view::string_view(const string_view& _other)
	{
		MemeStringViewUnsafeStack_initByOther(
			&data_, MEME_STRING__OBJECT_SIZE, &_other.data_);
	}

	MEMEPP__IMPL_INLINE string_view::string_view(string_view&& _other)
	{
		MemeStringStack_init(&data_, MEME_STRING__OBJECT_SIZE);
		MemeString_swap(to_pointer(data_), to_pointer(_other.data_));
	}

	MEMEPP__IMPL_INLINE memepp::string_view::~string_view()
	{
		MemeStringStack_unInit(&data_, MEME_STRING__OBJECT_SIZE);
	}

	MEMEPP__IMPL_INLINE string_view& string_view::operator=(const string& _other)
	{
		auto other = to_pointer(_other.native_handle());
		if (MemeString_isSharedStorageTypes(other))
		{
			MemeStringStack_assign_v02(&data_, MEME_STRING__OBJECT_SIZE, to_pointer(const_cast<MemeStringStack_t&>(_other.native_handle())));
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

	MEMEPP__IMPL_INLINE string_view& string_view::operator=(string_view&& _other)
	{
		MemeString_swap(to_pointer(data_), to_pointer(_other.data_));
		return *this;
	}


	MEMEPP__IMPL_INLINE const char* string_view::data() const noexcept
	{
		return MemeString_cStr(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE string_view::size_type string_view::size() const noexcept
	{
		return static_cast<size_t>(MemeString_byteSize(to_pointer(data_)));
	}

	MEMEPP__IMPL_INLINE bool memepp::string_view::empty() const noexcept
	{
		return MemeString_isEmpty_v02(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE string string_view::to_string() const
	{
		if (MemeString_storageType(to_pointer(data_)) == MemeString_UnsafeStorageType_view)
			return string { data(), size() };

		MemeStringStack_t stack;
		MemeStringStack_initByOther(
			&stack, MEME_STRING__OBJECT_SIZE, to_pointer(data_));
		return string { reinterpret_cast<MemeStringStack_t&&>(stack) };
	}

	MEMEPP__IMPL_INLINE string_view::size_type string_view::index_of(
		const char* _utf8, MemeFlag_CaseSensitivity_t _cs) const noexcept
	{
		return MemeString_indexOfWithUtf8bytes(
			to_pointer(native_handle()), 0, reinterpret_cast<const uint8_t*>(_utf8), -1, _cs);
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
