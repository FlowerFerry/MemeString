
#ifndef MEMEPP_STRING_VIEW_IMPL_HPP_INCLUDED
#define MEMEPP_STRING_VIEW_IMPL_HPP_INCLUDED

#include "meme/string.h"
#include "meme/unsafe/view.h"

#include "memepp/string_def.hpp"

#ifndef MEMEPP_STRING__IMPL_SEPARATE_INLINE
#	ifdef MEMEPP_STRING__IMPL_SEPARATE
#		define MEMEPP_STRING__IMPL_SEPARATE_INLINE 
#	else
#		define MEMEPP_STRING__IMPL_SEPARATE_INLINE inline
#	endif
#endif 

namespace memepp {

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string_view::string_view() noexcept
	{
		MemeStringStack_init(&data_, MEME_STRING__OBJECT_SIZE);
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string_view::string_view(const char* _utf8)
	{
		MemeStringViewUnsafeStack_init(&data_, MEME_STRING__OBJECT_SIZE,
			reinterpret_cast<const uint8_t*>(_utf8), -1);
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string_view::string_view(const char* _utf8, size_t _size)
	{
		MemeStringViewUnsafeStack_init(&data_, MEME_STRING__OBJECT_SIZE, 
			reinterpret_cast<const uint8_t*>(_utf8), _size);
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string_view::string_view(const uint8_t* _utf8)
	{
		MemeStringViewUnsafeStack_init(&data_, MEME_STRING__OBJECT_SIZE, _utf8, -1);
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string_view::string_view(const uint8_t* _utf8, size_t _size)
	{
		MemeStringViewUnsafeStack_init(&data_, MEME_STRING__OBJECT_SIZE, _utf8, _size);
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string_view::string_view(const string& _other)
	{
		MemeStringStack_initByOther(
			&data_, MEME_STRING__OBJECT_SIZE, __to_object__(_other.native_handle()));
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string_view::string_view(const string_view& _other)
	{
		MemeStringViewUnsafeStack_initByOther(
			&data_, MEME_STRING__OBJECT_SIZE, &_other.data_);
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string_view::string_view(string&& _other)
	{
		MemeStringStack_init(&data_, MEME_STRING__OBJECT_SIZE);
		MemeString_swap(__to_object__(data_), 
			__to_object__(const_cast<MemeStringStack_t&>(_other.native_handle())));
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string_view::string_view(string_view&& _other)
	{
		MemeStringStack_init(&data_, MEME_STRING__OBJECT_SIZE);
		MemeString_swap(__to_object__(data_), __to_object__(_other.data_));
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE memepp::string_view::~string_view()
	{
		MemeStringStack_unInit(&data_, MEME_STRING__OBJECT_SIZE);
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string_view& string_view::operator=(const string& _other)
	{
		MemeStringStack_assign(&data_, __to_object__(const_cast<MemeStringStack_t&>(_other.native_handle())));
		return *this;
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string_view& string_view::operator=(const string_view& _other)
	{
		MemeStringViewUnsafeStack_assignByOther(&data_, MEME_STRING__OBJECT_SIZE, &_other.data_);
		return *this;
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string_view& string_view::operator=(string&& _other)
	{
		MemeString_swap(__to_object__(data_), __to_object__(const_cast<MemeStringStack_t&>(_other.native_handle())));
		return *this;
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string_view& string_view::operator=(string_view&& _other)
	{
		MemeString_swap(__to_object__(data_), __to_object__(_other.data_));
		return *this;
	}


	MEMEPP_STRING__IMPL_SEPARATE_INLINE const char* string_view::data() const noexcept
	{
		return MemeString_cStr(__to_object__(data_));
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE size_t string_view::size() const noexcept
	{
		return static_cast<size_t>(MemeString_byteSize(__to_object__(data_)));
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string memepp::string_view::to_string() const
	{
		if (MemeString_storageType(__to_object__(data_)) == MemeString_UnsafeStorageType_view)
			return string { data(), size() };

		MemeStringStack_t stack;
		MemeStringStack_initByOther(
			&stack, MEME_STRING__OBJECT_SIZE, __to_object__(data_));
		return string { reinterpret_cast<MemeStringStack_t&&>(stack) };
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE const string_view::native_handle_type& 
		string_view::native_handle() const noexcept
	{
		return data_;
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE bool operator==(const string_view& _lhs, const string_view& _rhs)
	{
		int result = 0;
		MemeString_isEqualWithOther(
			__to_object__(_lhs.native_handle()),
			__to_object__(_rhs.native_handle()), &result);
		return result;
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE bool operator!=(const string_view& _lhs, const string_view& _rhs)
	{
		return !(_lhs == _rhs);
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE bool memepp::operator==(const string& _lhs, const string_view& _rhs)
	{
		int result = 0;
		MemeString_isEqualWithOther(
			__to_object__(_lhs.native_handle()),
			__to_object__(_rhs.native_handle()), &result);
		return result;
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE bool memepp::operator!=(const string& _lhs, const string_view& _rhs)
	{
		return !(_lhs == _rhs);
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE bool memepp::operator==(const string_view& _lhs, const string& _rhs)
	{
		int result = 0;
		MemeString_isEqualWithOther(
			__to_object__(_lhs.native_handle()),
			__to_object__(_rhs.native_handle()), &result);
		return result;
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE bool memepp::operator!=(const string_view& _lhs, const string& _rhs)
	{
		return !(_lhs == _rhs);
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE bool memepp::operator==(const char* _lhs, const string_view& _rhs)
	{
		int result = 0;
		MemeString_isEqual(
			__to_object__(_rhs.native_handle()), _lhs, -1, &result);
		return result;
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE bool memepp::operator!=(const char* _lhs, const string_view& _rhs)
	{
		return !(_lhs == _rhs);
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE bool memepp::operator==(const string_view& _lhs, const char* _rhs)
	{
		int result = 0;
		MemeString_isEqual(
			__to_object__(_lhs.native_handle()), _rhs, -1, &result);
		return result;
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE bool memepp::operator!=(const string_view& _lhs, const char* _rhs)
	{
		return !(_lhs == _rhs);
	}
};

#endif // !MEMEPP_STRING_VIEW_IMPL_HPP_INCLUDED
