
#ifndef MEMEPP_VARIABLE_BUFFER_IMPL_HPP_INCLUDED
#define MEMEPP_VARIABLE_BUFFER_IMPL_HPP_INCLUDED

#include "meme/variable_buffer.h"
#include "memepp/variable_buffer_def.hpp"
#include "memepp/string_def.hpp"
#include "memepp/buffer_def.hpp"
#include "memepp/string_view_def.hpp"

#include <utility>

#ifndef MEMEPP__IMPL_INLINE
#	ifdef MEMEPP__IMPL_SEPARATE
#		define MEMEPP__IMPL_INLINE 
#	else
#		define MEMEPP__IMPL_INLINE inline
#	endif
#endif 

namespace memepp {

	MEMEPP__IMPL_INLINE variable_buffer::variable_buffer() noexcept
	{
		MemeVariableBufferStack_init(&data_, MEME_STRING__OBJECT_SIZE);
	}

	MEMEPP__IMPL_INLINE variable_buffer::variable_buffer(const_pointer _buf, size_type _size)
	{
		MemeVariableBufferStack_initByBytes(
			&data_, MEME_STRING__OBJECT_SIZE, _buf, _size);
	}

	MEMEPP__IMPL_INLINE variable_buffer::variable_buffer(size_type _count, value_type _value)
	{
		MemeVariableBufferStack_initWithRepeatBytes(&data_, MEME_STRING__OBJECT_SIZE, _count, _value);
	}

	MEMEPP__IMPL_INLINE variable_buffer::variable_buffer(const variable_buffer& _other)
	{
		MemeVariableBufferStack_initByOther(
			&data_, MEME_STRING__OBJECT_SIZE, &_other.data_);
	}

	MEMEPP__IMPL_INLINE variable_buffer::variable_buffer(variable_buffer&& _other)
	{
		MemeVariableBufferStack_init(&data_, MEME_STRING__OBJECT_SIZE);
		MemeVariableBuffer_swap(to_pointer(data_), to_pointer(_other.data_));
	}

	MEMEPP__IMPL_INLINE variable_buffer::~variable_buffer()
	{
		MemeVariableBufferStack_unInit(&data_, MEME_STRING__OBJECT_SIZE);
	}

	MEMEPP__IMPL_INLINE variable_buffer& variable_buffer::operator=(const variable_buffer& _other)
	{
		MemeVariableBufferStack_assign(&data_, MEME_STRING__OBJECT_SIZE, &_other.data_);
		return *this;
	}

	MEMEPP__IMPL_INLINE variable_buffer::const_reference variable_buffer::operator[](size_type _pos) const
	{
		auto p = MemeVariableBuffer_data(to_pointer(data_)) + _pos;
		return *p;
	}

	MEMEPP__IMPL_INLINE variable_buffer::reference variable_buffer::operator[](size_type _pos)
	{
		auto p = MemeVariableBuffer_dataWithNotConst(to_pointer(data_)) + _pos;
		return *p;
	}

	MEMEPP__IMPL_INLINE variable_buffer& variable_buffer::operator=(variable_buffer&& _other)
	{
		MemeVariableBuffer_swap(to_pointer(data_), to_pointer(_other.data_));
		return *this;
	}

	MEMEPP__IMPL_INLINE variable_buffer& variable_buffer::operator+=(const variable_buffer& _other)
	{
		return this->append(_other);
	}

	MEMEPP__IMPL_INLINE variable_buffer& variable_buffer::operator+=(variable_buffer&& _other)
	{
		return this->append(std::move(_other));
	}

	MEMEPP__IMPL_INLINE buffer_storage_type variable_buffer::storage_type() const MEGOPP__NOEXCEPT
	{
		return static_cast<buffer_storage_type>(MemeVariableBuffer_storageType(to_pointer(data_)));
	}

	MEMEPP__IMPL_INLINE variable_buffer::reference variable_buffer::front()
	{
		auto p = MemeVariableBuffer_dataWithNotConst(to_pointer(data_));
		return *p;
	}

	MEMEPP__IMPL_INLINE variable_buffer::const_reference variable_buffer::front() const
	{
		auto p = MemeVariableBuffer_data(to_pointer(data_));
		return *p;
	}

	MEMEPP__IMPL_INLINE variable_buffer::reference variable_buffer::back()
	{
		auto p = MemeVariableBuffer_backItem(to_pointer(data_));
		return *p;
	}

	MEMEPP__IMPL_INLINE variable_buffer::const_reference variable_buffer::back() const
	{
		auto p = MemeVariableBuffer_constBackItem(to_pointer(data_));
		return *p;
	}

	MEMEPP__IMPL_INLINE variable_buffer::const_reference variable_buffer::at(size_type _pos) const
	{
		auto p = MemeVariableBuffer_constAt(to_pointer(data_), _pos);
		if (!p)
			; // TO_DO
		return *p;
	}

	MEMEPP__IMPL_INLINE variable_buffer::reference variable_buffer::at(size_type _pos)
	{
		auto p = MemeVariableBuffer_at(to_pointer(data_), _pos);
		if (!p)
			; // TO_DO
		return *p;
	}

	MEMEPP__IMPL_INLINE variable_buffer::const_pointer variable_buffer::data() const MEGOPP__NOEXCEPT
	{
		return MemeVariableBuffer_data(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE variable_buffer::pointer variable_buffer::data() MEGOPP__NOEXCEPT
	{
		return MemeVariableBuffer_dataWithNotConst(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE variable_buffer::size_type variable_buffer::size() const MEGOPP__NOEXCEPT
	{
		return MemeVariableBuffer_size(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE bool variable_buffer::empty() const MEGOPP__NOEXCEPT
	{
		return MemeVariableBuffer_isEmpty(to_pointer(data_)) == 0;
	}

	MEMEPP__IMPL_INLINE variable_buffer::size_type variable_buffer::max_size() const MEGOPP__NOEXCEPT
	{
		return MemeVariableBuffer_maxByteSize(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE variable_buffer::size_type variable_buffer::capacity() const MEGOPP__NOEXCEPT
	{
		return MemeVariableBuffer_availableByteCapacity(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE void variable_buffer::swap(variable_buffer& _other) MEGOPP__NOEXCEPT
	{
		MemeVariableBuffer_swap(to_pointer(data_), to_pointer(_other.data_));
	}

	MEMEPP__IMPL_INLINE void variable_buffer::clear() MEGOPP__NOEXCEPT
	{
		MemeVariableBuffer_clear(to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE variable_buffer& variable_buffer::push_back(const value_type& value) MEGOPP__NOEXCEPT
	{
		MemeVariableBuffer_appendWithByte(to_pointer(data_), value);
		return *this;
	}

	MEMEPP__IMPL_INLINE variable_buffer& variable_buffer::append(const_pointer _buf, size_type _len)
	{
		MemeVariableBuffer_appendWithBytes(to_pointer(data_), _buf, _len);
		return *this;
	}

	MEMEPP__IMPL_INLINE variable_buffer& variable_buffer::append(const variable_buffer& _other)
	{
		MemeVariableBuffer_appendWithOther(to_pointer(data_), to_pointer(_other.data_));
		return *this;
	}

	MEMEPP__IMPL_INLINE variable_buffer& variable_buffer::append(variable_buffer&& _other)
	{
		MemeVariableBuffer_appendWithOther(to_pointer(data_), to_pointer(_other.data_));
		return *this;
	}

	MEMEPP__IMPL_INLINE variable_buffer& variable_buffer::append(const string& _other)
	{
		MemeVariableBuffer_appendWithBytes(to_pointer(data_), 
			reinterpret_cast<const MemeByte_t*>(_other.data()), _other.size());
		return *this;
	}

	MEMEPP__IMPL_INLINE variable_buffer& variable_buffer::append(const string_view& _other)
	{
		MemeVariableBuffer_appendWithBytes(to_pointer(data_),
			reinterpret_cast<const MemeByte_t*>(_other.data()), _other.size());
		return *this;
	}

	MEMEPP__IMPL_INLINE void variable_buffer::reserve(size_type _new_cap)
	{
		MemeVariableBuffer_reserve(to_pointer(data_), _new_cap);
	}

	MEMEPP__IMPL_INLINE variable_buffer& variable_buffer::resize(size_type _count)
	{
		MemeVariableBuffer_resize(to_pointer(data_), _count);

		return *this;
	}

	MEMEPP__IMPL_INLINE variable_buffer& variable_buffer::resize(size_type _count, value_type _value)
	{
		MemeVariableBuffer_resizeWithByte(to_pointer(data_), _count, _value);

		return *this;
	}

	MEMEPP__IMPL_INLINE MemeInteger_t variable_buffer::release(buffer& _buf)
	{
		MemeBufferStack_t stack;
		auto result = MemeVariableBuffer_releaseToBuffer(to_pointer(data_), &stack, MEME_STRING__OBJECT_SIZE);
		if (result)
			return result;
		_buf = buffer{ std::move(stack) };
		return 0;
	}

	MEMEPP__IMPL_INLINE MemeInteger_t variable_buffer::release(string& _str)
	{
		MemeStringStack_t stack;
		auto result = MemeVariableBuffer_releaseToString(to_pointer(data_), &stack, MEME_STRING__OBJECT_SIZE);
		if (result)
			return result;
		_str = string{ std::move(stack) };
		return 0;
	}

	MEMEPP__IMPL_INLINE const variable_buffer::native_handle_type& variable_buffer::native_handle() const MEGOPP__NOEXCEPT
	{
		return data_;
	}

};

#endif // !MEMEPP_VARIABLE_BUFFER_IMPL_HPP_INCLUDED
