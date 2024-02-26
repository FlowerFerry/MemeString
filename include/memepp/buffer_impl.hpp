
#ifndef MEMEPP_BUFFER_IMPL_HPP_INCLUDED
#define MEMEPP_BUFFER_IMPL_HPP_INCLUDED

#include "meme/buffer.h"
#include "memepp/buffer_def.hpp"
#include "memepp/string_def.hpp"
#include <memepp/errc.hpp>

#include <string.h>

namespace memepp {

	MEMEPP__IMPL_INLINE buffer::buffer() MEGOPP__NOEXCEPT
	{
		MemeBufferStack_init(&data_, MEME_STRING__OBJECT_SIZE);
	}

	MEMEPP__IMPL_INLINE buffer::buffer(native_handle_type&& _other)
	{
		if (MemeBuffer_storageType(to_pointer(_other)) == MemeString_UnsafeStorageType_view)
		{
			MemeBufferStack_initByBytes(&data_, MEME_STRING__OBJECT_SIZE,
				reinterpret_cast<const uint8_t*>(MemeBuffer_data(to_pointer(_other))),
				MemeBuffer_size(to_pointer(_other)));
		}
		else {
			MemeBufferStack_init(&data_, MEME_STRING__OBJECT_SIZE);
			MemeBuffer_swap(to_pointer(data_), to_pointer(_other));
		}
		MemeBufferStack_unInit(&_other, MEME_STRING__OBJECT_SIZE);
	}

	MEMEPP__IMPL_INLINE buffer::buffer(buffer&& _other) MEGOPP__NOEXCEPT
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
		MemeBufferStack_initByBytes(&data_, MEME_STRING__OBJECT_SIZE, _utf8, _size);
	}
	
	MEMEPP__IMPL_INLINE buffer::buffer(const_pointer _utf8, size_type _size, buffer_storage_t _suggest)
	{
        MemeBufferStack_initByU8bytesAndType(&data_, MMS__OBJECT_SIZE, _utf8, _size, 
			static_cast<mmbuf_storage_t>(_suggest));
	}

	MEMEPP__IMPL_INLINE memepp::buffer::buffer(const_pointer _begin, const_pointer _end)
	{
		MemeBufferStack_initByBytes(&data_, MEME_STRING__OBJECT_SIZE, _begin, _end - _begin);
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

	MEMEPP__IMPL_INLINE buffer& buffer::operator=(buffer&& _other) MEGOPP__NOEXCEPT
	{
		MemeBuffer_swap(to_pointer(data_), to_pointer(_other.data_));
		return *this;
	}

	MEMEPP__IMPL_INLINE buffer& buffer::operator=(const buffer& _other)
	{
		MemeBufferStack_assign(&data_, MEME_STRING__OBJECT_SIZE, &_other.data_);
		return *this;
	}

	MEMEPP__IMPL_INLINE buffer_storage_t buffer::storage_type() const MEGOPP__NOEXCEPT
	{
		return static_cast<buffer_storage_t>(MemeBuffer_storageType(to_pointer(data_)));
	}

	MEMEPP__IMPL_INLINE buffer::const_reference buffer::at(size_type _pos) const
	{
		auto p = MemeBuffer_at(memepp::to_pointer(data_), _pos);
		// TO_DO
		return *p;
	}

	MEMEPP__IMPL_INLINE buffer::const_pointer buffer::data() const MEGOPP__NOEXCEPT
	{
		return MemeBuffer_data(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE buffer::size_type buffer::size() const MEGOPP__NOEXCEPT
	{
		return MemeBuffer_size(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE bool buffer::empty() const MEGOPP__NOEXCEPT
	{
		return MemeBuffer_isNonempty(to_pointer(data_)) == 0;
	}

	MEMEPP__IMPL_INLINE const_iterator buffer::begin() const MEGOPP__NOEXCEPT
	{
        return const_iterator(data());
	}
	
	MEMEPP__IMPL_INLINE const_iterator buffer::cbegin() const MEGOPP__NOEXCEPT
	{
        return const_iterator(data());
	}

    MEMEPP__IMPL_INLINE const_iterator buffer::end() const MEGOPP__NOEXCEPT
    {
        return const_iterator(data() + size());
    }

    MEMEPP__IMPL_INLINE const_iterator buffer::cend() const MEGOPP__NOEXCEPT
    {
        return const_iterator(data() + size());
    }

	MEMEPP__IMPL_INLINE void buffer::swap(buffer& _other) MEGOPP__NOEXCEPT
	{
		MemeBuffer_swap(to_pointer(data_), to_pointer(_other.data_));
	}

	MEMEPP__IMPL_INLINE string buffer::to_string(size_type _front_offset) const
	{
		MemeStringStack_t stack;
		auto result = MemeStringStack_initByBuffer(
			&stack, MEME_STRING__OBJECT_SIZE, to_pointer(data_), _front_offset);
		if (result) {
			// TO_DO
			return {};
		}
		return std::move(stack);
	}

	MEMEPP__IMPL_INLINE buffer buffer::to_large() const noexcept
	{
        return storage_type() == buffer_storage_t::large ?
            *this : buffer{ data(), size(), buffer_storage_t::large };
	}
    
	MEMEPP__IMPL_INLINE buffer::size_type buffer::index_of(
		const buffer& _other) const MEGOPP__NOEXCEPT
	{
		return MemeBuffer_indexOfWithOther(
			to_pointer(native_handle()), 0, to_pointer(_other.native_handle()));
	}

	MEMEPP__IMPL_INLINE buffer::size_type buffer::index_of(
		const_pointer _utf8, size_type _utf8_len) const MEGOPP__NOEXCEPT
	{
		return MemeBuffer_indexOfWithBytes(
			to_pointer(native_handle()), 0, reinterpret_cast<const uint8_t*>(_utf8), _utf8_len);
	}

	MEMEPP__IMPL_INLINE bool buffer::contains(const buffer& _other) const MEGOPP__NOEXCEPT
	{
        return index_of(_other) != npos;
	}

	MEMEPP__IMPL_INLINE bool buffer::contains(const_pointer _utf8, size_type _count) const MEGOPP__NOEXCEPT
	{
        return index_of(_utf8, _count) != npos;
	}

	MEMEPP__IMPL_INLINE const buffer::native_handle_type& buffer::native_handle() const MEGOPP__NOEXCEPT
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
