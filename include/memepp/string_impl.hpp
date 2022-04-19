

#ifndef MEMEPP_STRING_IMPL_HPP_INCLUDED
#define MEMEPP_STRING_IMPL_HPP_INCLUDED

#include "meme/string.h"
#include "memepp/string_def.hpp"

#include <string.h>

#ifdef MEMEPP_STRING__IMPL_SEPARATE
#define MEMEPP_STRING__IMPL_SEPARATE_INLINE 
#else
#define MEMEPP_STRING__IMPL_SEPARATE_INLINE inline
#endif

namespace memepp {

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string::string() noexcept
	{
		MemeStringStack_init(&data_, MEME_STRING__OBJECT_SIZE);
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string::string(native_handle_type&& _other)
	{
		MemeStringStack_init(&data_, MEME_STRING__OBJECT_SIZE);
		MemeString_swap(__to_object__(data_), __to_object__(_other));
	}

	//inline string::string(const string& _other, string_storage_type _suggest)
	//{
	//	MemeStringStack_init_with_type(
	//		&data_, MEME_STRING__OBJECT_SIZE, static_cast<MemeString_Storage_t>(_suggest));
	//}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string::string(string && _other)
	{
		MemeStringStack_init(&data_, MEME_STRING__OBJECT_SIZE);
		MemeString_swap(__to_object__(data_), __to_object__(_other.data_));
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string::string(const string & _other)
	{
		MemeStringStack_initByOther(
			&data_, MEME_STRING__OBJECT_SIZE, __to_object__(_other.data_));
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string::string(const char* _utf8)
	{
		MemeStringStack_initByU8bytes(&data_, MEME_STRING__OBJECT_SIZE,
			reinterpret_cast<const uint8_t*>(_utf8), strlen(_utf8));
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string::string(const char* _utf8, size_t _size)
	{
		MemeStringStack_initByU8bytes(&data_, MEME_STRING__OBJECT_SIZE,
			reinterpret_cast<const uint8_t*>(_utf8), _size);
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string::string(const uint8_t* _utf8)
	{
		MemeStringStack_initByU8bytes(&data_, MEME_STRING__OBJECT_SIZE,
			_utf8, strlen(reinterpret_cast<const char*>(_utf8)));
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string::string(const uint8_t* _utf8, size_t _size)
	{
		MemeStringStack_initByU8bytes(&data_, MEME_STRING__OBJECT_SIZE, _utf8, _size);
	}

	//inline string::string(const std::initializer_list<char>& _ilist)
	//{

	//}

	//inline string::string(const std::initializer_list<uint8_t>& _ilist)
	//{
	//}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string::~string()
	{
		MemeStringStack_unInit(&data_, MEME_STRING__OBJECT_SIZE);
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string & string::operator=(string && _other)
	{
		MemeString_swap(__to_object__(data_), __to_object__(_other.data_));
		return *this;
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string & string::operator=(const string & _other)
	{
		MemeStringStack_assign(&data_, __to_object__(_other.data_));
		return *this;
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE string_storage_type string::storage_type() const noexcept
	{
		return static_cast<string_storage_type>(MemeString_storageType(__to_object__(data_)));
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE const char * string::data() const noexcept
	{
		return MemeString_cStr(__to_object__(data_));
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE const char * string::c_str() const noexcept
	{
		return MemeString_cStr(__to_object__(data_));
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE size_t string::size() const noexcept
	{
		return MemeString_byteSize(__to_object__(data_));
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE bool string::empty() const noexcept
	{
		return MemeString_isEmpty(__to_object__(data_)) == 0;
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE size_t string::capacity() const noexcept
	{
		return MemeString_byteCapacity(__to_object__(data_));
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE void string::swap(string& _other) noexcept
	{
		MemeString_swap(__to_object__(data_), __to_object__(_other.data_));
	}

	//string::size_type string::find(string& _other, size_type _pos = 0) const
	//{
	//}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE const string::native_handle_type & string::native_handle() const noexcept
	{
		return data_;
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE bool operator==(const string& _lhs, const string& _rhs)
	{
		int result = 0;
		MemeString_isEqualWithOther(
			__to_object__(_lhs.native_handle()),
			__to_object__(_rhs.native_handle()), &result);
		return result;
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE bool operator==(const char* _lhs, const string& _rhs)
	{
		int result = 0;
		MemeString_isEqual(
			__to_object__(_rhs.native_handle()), _lhs, -1, &result);
		return result;
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE bool operator==(const string& _lhs, const char* _rhs)
	{
		int result = 0;
		MemeString_isEqual(
			__to_object__(_lhs.native_handle()), _rhs, -1, &result);
		return result;
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE bool operator!=(const string& _lhs, const string& _rhs)
	{
		return !(_lhs == _rhs);
	}

	MEMEPP_STRING__IMPL_SEPARATE_INLINE bool operator!=(const char* _lhs, const string& _rhs)
	{
		return !(_lhs == _rhs);
	}
	MEMEPP_STRING__IMPL_SEPARATE_INLINE bool operator!=(const string& _lhs, const char* _rhs)
	{
		return !(_lhs == _rhs);
	}

}; // namespace memepp

	MEMEPP_STRING__IMPL_SEPARATE_INLINE memepp::string operator""_meme(const char* _str, size_t _len)
	{
		return memepp::string{ _str, _len };
	}

#endif // !MEMEPP_STRING_IMPL_HPP_INCLUDED

