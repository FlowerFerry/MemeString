
#ifndef MEMEPP_BUFFER_IMPL_HPP_INCLUDED
#define MEMEPP_BUFFER_IMPL_HPP_INCLUDED

#include "meme/buffer.h"
#include "memepp/buffer_def.hpp"

#include <string.h>

namespace memepp {

	MEMEPP__IMPL_INLINE buffer::buffer() noexcept
	{
		MemeBufferStack_init(&data_, MEME_STRING__OBJECT_SIZE);
	}

	MEMEPP__IMPL_INLINE buffer::buffer(native_handle_type&& _other)
	{
		if (MemeBuffer_storageType(to_pointer(_other)) == MemeString_UnsafeStorageType_view)
		{
			MemeBufferStack_initByU8bytes(&data_, MEME_STRING__OBJECT_SIZE,
				reinterpret_cast<const uint8_t*>(MemeBuffer_data(to_pointer(_other))),
				MemeBuffer_size(to_pointer(_other)));
		}
		else {
			MemeBufferStack_init(&data_, MEME_STRING__OBJECT_SIZE);
			MemeBuffer_swap(to_pointer(data_), to_pointer(_other));
		}
		MemeBufferStack_unInit(&_other, MEME_STRING__OBJECT_SIZE);
	}

	MEMEPP__IMPL_INLINE buffer::buffer(buffer&& _other)
	{
		MemeBufferStack_init(&data_, MEME_STRING__OBJECT_SIZE);
		MemeBuffer_swap(to_pointer(data_), to_pointer(_other.data_));
	}

	MEMEPP__IMPL_INLINE buffer::buffer(const buffer& _other)
	{
		MemeBufferStack_initByOther(
			&data_, MEME_STRING__OBJECT_SIZE, &_other.data_);
	}

	MEMEPP__IMPL_INLINE buffer::buffer(const_pointer _utf8, size_type _size)
	{
		MemeBufferStack_initByU8bytes(&data_, MEME_STRING__OBJECT_SIZE, _utf8, _size);
	}

	//inline buffer::buffer(const std::initializer_list<char>& _ilist)
	//{

	//}

	//inline buffer::buffer(const std::initializer_list<uint8_t>& _ilist)
	//{
	//}

	MEMEPP__IMPL_INLINE buffer::~buffer()
	{
		MemeBufferStack_unInit(&data_, MEME_STRING__OBJECT_SIZE);
	}

	MEMEPP__IMPL_INLINE buffer& buffer::operator=(buffer&& _other)
	{
		MemeBuffer_swap(to_pointer(data_), to_pointer(_other.data_));
		return *this;
	}

	MEMEPP__IMPL_INLINE buffer& buffer::operator=(const buffer& _other)
	{
		MemeBufferStack_assign(&data_, MEME_STRING__OBJECT_SIZE, &_other.data_);
		return *this;
	}

	MEMEPP__IMPL_INLINE buffer_storage_type buffer::storage_type() const noexcept
	{
		return static_cast<buffer_storage_type>(MemeBuffer_storageType(to_pointer(data_)));
	}

	MEMEPP__IMPL_INLINE buffer::const_pointer buffer::data() const noexcept
	{
		return MemeBuffer_data(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE buffer::size_type buffer::size() const noexcept
	{
		return MemeBuffer_size(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE bool buffer::empty() const noexcept
	{
		return MemeBuffer_isEmpty(to_pointer(data_)) == 0;
	}

	MEMEPP__IMPL_INLINE void buffer::swap(buffer& _other) noexcept
	{
		MemeBuffer_swap(to_pointer(data_), to_pointer(_other.data_));
	}

	MEMEPP__IMPL_INLINE buffer::size_type buffer::index_of(
		const buffer& _other) const noexcept
	{
		return MemeBuffer_indexOfWithOther(
			to_pointer(native_handle()), 0, to_pointer(_other.native_handle()));
	}

	MEMEPP__IMPL_INLINE buffer::size_type buffer::index_of_with_strlen(
		const char* _utf8, size_type _utf8_len) const noexcept
	{
		return MemeBuffer_indexOfWithUtf8bytes(
			to_pointer(native_handle()), 0, reinterpret_cast<const uint8_t*>(_utf8), _utf8_len);
	}

	MEMEPP__IMPL_INLINE const buffer::native_handle_type& buffer::native_handle() const noexcept
	{
		return data_;
	}

	MEMEPP__IMPL_INLINE bool operator==(const buffer& _lhs, const buffer& _rhs)
	{
		int result = 0;
		MemeBuffer_isEqualWithOther(
			to_pointer(_lhs.native_handle()),
			to_pointer(_rhs.native_handle()), &result);
		return result;
	}

	MEMEPP__IMPL_INLINE bool operator!=(const buffer& _lhs, const buffer& _rhs)
	{
		return !(_lhs == _rhs);
	}

}; // namespace memepp

#endif // !MEMEPP_BUFFER_IMPL_HPP_INCLUDED
