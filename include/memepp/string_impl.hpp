

#ifndef MEMEPP_STRING_IMPL_HPP_INCLUDED
#define MEMEPP_STRING_IMPL_HPP_INCLUDED

#include "meme/string.h"
#include "memepp/string_def.hpp"

#include <string.h>

namespace memepp {

	MEMEPP__IMPL_INLINE string::string() noexcept
	{
		MemeStringStack_init(&data_, MEME_STRING__OBJECT_SIZE);
	}

	MEMEPP__IMPL_INLINE string::string(native_handle_type&& _other)
	{
		if (MemeString_storageType(to_pointer(_other)) == MemeString_UnsafeStorageType_view)
		{
			MemeStringStack_initByU8bytes(&data_, MEME_STRING__OBJECT_SIZE, 
				reinterpret_cast<const uint8_t*>(MemeString_cStr(to_pointer(_other))), 
				MemeString_byteSize(to_pointer(_other)));
		}
		else {
			MemeStringStack_init(&data_, MEME_STRING__OBJECT_SIZE);
			MemeString_swap(to_pointer(data_), to_pointer(_other));
		}
		MemeStringStack_unInit(&_other, MEME_STRING__OBJECT_SIZE);
	}

	//inline string::string(const string& _other, string_storage_type _suggest)
	//{
	//	MemeStringStack_init_with_type(
	//		&data_, MEME_STRING__OBJECT_SIZE, static_cast<MemeString_Storage_t>(_suggest));
	//}

	MEMEPP__IMPL_INLINE string::string(string && _other)
	{
		MemeStringStack_init(&data_, MEME_STRING__OBJECT_SIZE);
		MemeString_swap(to_pointer(data_), to_pointer(_other.data_));
	}

	MEMEPP__IMPL_INLINE string::string(const string & _other)
	{
		MemeStringStack_initByOther(
			&data_, MEME_STRING__OBJECT_SIZE, to_pointer(_other.data_));
	}

	MEMEPP__IMPL_INLINE string::string(const char* _utf8)
	{
		MemeStringStack_initByU8bytes(&data_, MEME_STRING__OBJECT_SIZE,
			reinterpret_cast<const uint8_t*>(_utf8), -1);
	}

	MEMEPP__IMPL_INLINE string::string(const char* _utf8, size_t _size)
	{
		MemeStringStack_initByU8bytes(&data_, MEME_STRING__OBJECT_SIZE,
			reinterpret_cast<const uint8_t*>(_utf8), _size);
	}

	MEMEPP__IMPL_INLINE string::string(const uint8_t* _utf8)
	{
		MemeStringStack_initByU8bytes(&data_, MEME_STRING__OBJECT_SIZE, _utf8, -1);
	}

	MEMEPP__IMPL_INLINE string::string(const uint8_t* _utf8, size_t _size)
	{
		MemeStringStack_initByU8bytes(&data_, MEME_STRING__OBJECT_SIZE, _utf8, _size);
	}

	//inline string::string(const std::initializer_list<char>& _ilist)
	//{

	//}

	//inline string::string(const std::initializer_list<uint8_t>& _ilist)
	//{
	//}

	MEMEPP__IMPL_INLINE string::~string()
	{
		MemeStringStack_unInit(&data_, MEME_STRING__OBJECT_SIZE);
	}

	MEMEPP__IMPL_INLINE string & string::operator=(string && _other)
	{
		MemeString_swap(to_pointer(data_), to_pointer(_other.data_));
		return *this;
	}

	MEMEPP__IMPL_INLINE string & string::operator=(const string & _other)
	{
		MemeStringStack_assign(&data_, to_pointer(_other.data_));
		return *this;
	}

	MEMEPP__IMPL_INLINE string_storage_type string::storage_type() const noexcept
	{
		return static_cast<string_storage_type>(MemeString_storageType(to_pointer(data_)));
	}

	MEMEPP__IMPL_INLINE const char * string::data() const noexcept
	{
		return MemeString_cStr(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE const char * string::c_str() const noexcept
	{
		return MemeString_cStr(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE size_t string::size() const noexcept
	{
		return MemeString_byteSize(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE bool string::empty() const noexcept
	{
		return MemeString_isEmpty(to_pointer(data_)) == 0;
	}

	MEMEPP__IMPL_INLINE size_t string::capacity() const noexcept
	{
		return MemeString_byteCapacity(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE void string::swap(string& _other) noexcept
	{
		MemeString_swap(to_pointer(data_), to_pointer(_other.data_));
	}

	//string::size_type string::find(string& _other, size_type _pos = 0) const
	//{
	//}

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

}; // namespace memepp

	MEMEPP__IMPL_INLINE memepp::string operator""_meme(const char* _str, size_t _len)
	{
		return memepp::string{ _str, _len };
	}

#endif // !MEMEPP_STRING_IMPL_HPP_INCLUDED

