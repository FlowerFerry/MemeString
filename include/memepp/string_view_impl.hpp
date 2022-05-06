
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

	MEMEPP__IMPL_INLINE string_view::string_view(const char* _utf8)
	{
		MemeStringViewUnsafeStack_init(&data_, MEME_STRING__OBJECT_SIZE,
			reinterpret_cast<const uint8_t*>(_utf8), -1);
	}

	MEMEPP__IMPL_INLINE string_view::string_view(const char* _utf8, size_t _size)
	{
		MemeStringViewUnsafeStack_init(&data_, MEME_STRING__OBJECT_SIZE, 
			reinterpret_cast<const uint8_t*>(_utf8), _size);
	}

	MEMEPP__IMPL_INLINE string_view::string_view(const uint8_t* _utf8)
	{
		MemeStringViewUnsafeStack_init(&data_, MEME_STRING__OBJECT_SIZE, _utf8, -1);
	}

	MEMEPP__IMPL_INLINE string_view::string_view(const uint8_t* _utf8, size_t _size)
	{
		MemeStringViewUnsafeStack_init(&data_, MEME_STRING__OBJECT_SIZE, _utf8, _size);
	}

	MEMEPP__IMPL_INLINE string_view::string_view(const string& _other)
	{
		MemeStringStack_initByOther(
			&data_, MEME_STRING__OBJECT_SIZE, to_pointer(_other.native_handle()));
	}

	MEMEPP__IMPL_INLINE string_view::string_view(const string_view& _other)
	{
		MemeStringViewUnsafeStack_initByOther(
			&data_, MEME_STRING__OBJECT_SIZE, &_other.data_);
	}

	MEMEPP__IMPL_INLINE string_view::string_view(string&& _other)
	{
		MemeStringStack_init(&data_, MEME_STRING__OBJECT_SIZE);
		MemeString_swap(to_pointer(data_), 
			to_pointer(const_cast<MemeStringStack_t&>(_other.native_handle())));
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
		MemeStringStack_assign(&data_, to_pointer(const_cast<MemeStringStack_t&>(_other.native_handle())));
		return *this;
	}

	MEMEPP__IMPL_INLINE string_view& string_view::operator=(const string_view& _other)
	{
		MemeStringViewUnsafeStack_assignByOther(&data_, MEME_STRING__OBJECT_SIZE, &_other.data_);
		return *this;
	}

	MEMEPP__IMPL_INLINE string_view& string_view::operator=(string&& _other)
	{
		MemeString_swap(to_pointer(data_), to_pointer(const_cast<MemeStringStack_t&>(_other.native_handle())));
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

	MEMEPP__IMPL_INLINE size_t string_view::size() const noexcept
	{
		return static_cast<size_t>(MemeString_byteSize(to_pointer(data_)));
	}

	MEMEPP__IMPL_INLINE string memepp::string_view::to_string() const
	{
		if (MemeString_storageType(to_pointer(data_)) == MemeString_UnsafeStorageType_view)
			return string { data(), size() };

		MemeStringStack_t stack;
		MemeStringStack_initByOther(
			&stack, MEME_STRING__OBJECT_SIZE, to_pointer(data_));
		return string { reinterpret_cast<MemeStringStack_t&&>(stack) };
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

#endif // !MEMEPP_STRING_VIEW_IMPL_HPP_INCLUDED
