
#ifndef MEMEPP_BUFFER_VIEW_IMPL_HPP_INCLUDED
#define MEMEPP_BUFFER_VIEW_IMPL_HPP_INCLUDED

#include "meme/unsafe/buffer_view.h"
#include "meme/buffer.h"

#include "memepp/buffer_view_def.hpp"
#include "memepp/buffer_def.hpp"
#include "memepp/string_def.hpp"
#include "memepp/variable_buffer_def.hpp"

namespace memepp {
	
	MEMEPP__IMPL_INLINE buffer_view::buffer_view() MEGOPP__NOEXCEPT
	{
		MemeBufferStack_init(&data_, MMS__OBJECT_SIZE);
	}
	
	MEMEPP__IMPL_INLINE buffer_view::buffer_view(const buffer& _other) MEGOPP__NOEXCEPT
	{
        MemeBufferViewUnsafeStack_initByOther(&data_, MMS__OBJECT_SIZE, &_other.native_handle());
	}
	
	MEMEPP__IMPL_INLINE buffer_view::buffer_view(const string& _other) MEGOPP__NOEXCEPT
	{
        MemeBufferViewUnsafeStack_initByString(&data_, MMS__OBJECT_SIZE, &_other.native_handle());
	}
	
	MEMEPP__IMPL_INLINE buffer_view::buffer_view(const variable_buffer& _other) MEGOPP__NOEXCEPT
	{
        MemeBufferViewUnsafeStack_init(&data_, MMS__OBJECT_SIZE, _other.data(), _other.size());
	}

	MEMEPP__IMPL_INLINE buffer_view::buffer_view(const string_view& _other) MEGOPP__NOEXCEPT
	{
		MemeBufferViewUnsafeStack_initByString(&data_, MMS__OBJECT_SIZE, &_other.native_handle());
	}
	
	MEMEPP__IMPL_INLINE buffer_view::buffer_view(const buffer_view& _other) MEGOPP__NOEXCEPT
	{
        MemeBufferViewUnsafeStack_initByOther(&data_, MMS__OBJECT_SIZE, &_other.native_handle());
	}
	
	MEMEPP__IMPL_INLINE buffer_view::buffer_view(buffer&& _other) MEGOPP__NOEXCEPT
	{
		auto other = to_pointer(_other.native_handle());
		if (MemeBuffer_isSharedStorageTypes(other) == 1)
		{
			MemeBufferStack_initByOther(&data_, MMS__OBJECT_SIZE, &_other.native_handle());
		}
		else {
			MemeBufferStack_init(&data_, MMS__OBJECT_SIZE);
			memepp::buffer b{ _other.data(), _other.size(), buffer_storage_t::large };
			*this = memepp::buffer_view{ b };
		}
	}
	
	MEMEPP__IMPL_INLINE buffer_view::buffer_view(string&& _other) MEGOPP__NOEXCEPT
	{
        auto other = to_pointer(_other.native_handle());
        if (MemeString_isSharedStorageTypes(other) == 1)
        {
            MemeBufferStack_initByString(&data_, MMS__OBJECT_SIZE, other);
        }
        else {
            MemeBufferStack_init(&data_, MMS__OBJECT_SIZE);
            memepp::string s{ _other.data(), _other.size(), string_storage_t::large };
            *this = memepp::buffer_view{ s };
        }
	}
	
	//MEMEPP__IMPL_INLINE buffer_view::buffer_view(variable_buffer&& _other) MEGOPP__NOEXCEPT
	MEMEPP__IMPL_INLINE buffer_view::buffer_view(string_view&& _other) MEGOPP__NOEXCEPT
	{
        MemeBufferViewUnsafeStack_initByString(
			&data_, MMS__OBJECT_SIZE, &_other.native_handle());
	}
	
	MEMEPP__IMPL_INLINE buffer_view::buffer_view(buffer_view&& _other) MEGOPP__NOEXCEPT
	{
		MemeBufferStack_init(&data_, MMS__OBJECT_SIZE);
        MemeBuffer_swap(
			memepp::to_pointer(data_),
			memepp::to_pointer(const_cast<mmbufstk_t&>(_other.native_handle()))
		);
	}
	
	MEMEPP__IMPL_INLINE buffer_view::buffer_view(const_pointer _data, size_type _size) MEGOPP__NOEXCEPT
	{
        MemeBufferViewUnsafeStack_init(&data_, MMS__OBJECT_SIZE, _data, _size);
	}

	MEMEPP__IMPL_INLINE buffer_view::~buffer_view()
	{
        MemeBufferStack_unInit(&data_, MMS__OBJECT_SIZE);
	}

	MEMEPP__IMPL_INLINE buffer_view& buffer_view::operator=(const buffer& _other) MEGOPP__NOEXCEPT
	{
        MemeBufferViewUnsafeStack_assignByOther(&data_, MMS__OBJECT_SIZE, &_other.native_handle());
        return *this;
	}
	
	MEMEPP__IMPL_INLINE buffer_view& buffer_view::operator=(const string& _other) MEGOPP__NOEXCEPT
	{
        MemeBufferViewUnsafeStack_assignByString(&data_, MMS__OBJECT_SIZE, &_other.native_handle());
        return *this;
	}
	
	MEMEPP__IMPL_INLINE buffer_view& buffer_view::operator=(const variable_buffer& _other) MEGOPP__NOEXCEPT
	{
		MemeBufferViewUnsafeStack_assignByVariableBuffer(&data_, MMS__OBJECT_SIZE, &_other.native_handle());
        return *this;
	}
	
	MEMEPP__IMPL_INLINE buffer_view& buffer_view::operator=(const string_view& _other) MEGOPP__NOEXCEPT
	{
        MemeBufferViewUnsafeStack_assignByString(&data_, MMS__OBJECT_SIZE, &_other.native_handle());
        return *this;
	}
	
	MEMEPP__IMPL_INLINE buffer_view& buffer_view::operator=(const buffer_view& _other) MEGOPP__NOEXCEPT
	{
        MemeBufferViewUnsafeStack_assignByOther(&data_, MMS__OBJECT_SIZE, &_other.native_handle());
        return *this;
	}
	
	MEMEPP__IMPL_INLINE buffer_view& buffer_view::operator=(buffer_view&& _other) MEGOPP__NOEXCEPT
	{
        MemeBuffer_swap(
			memepp::to_pointer(data_), 
            memepp::to_pointer(const_cast<mmbufstk_t&>(_other.native_handle()))
		);
        return *this;
	}

	MEMEPP__IMPL_INLINE buffer_storage_t buffer_view::storage_type() const MEGOPP__NOEXCEPT
	{
        return static_cast<buffer_storage_t>(MemeBuffer_storageType(memepp::to_pointer(data_)));
	}

	MEMEPP__IMPL_INLINE buffer_view::const_reference buffer_view::at(size_type _pos) const
	{
		auto p = MemeBuffer_at(memepp::to_pointer(data_), _pos);
		// TO_DO
		return *p;
	}

	MEMEPP__IMPL_INLINE buffer_view::const_pointer buffer_view::data() const MEGOPP__NOEXCEPT
	{
        return MemeBuffer_data(memepp::to_pointer(data_));
	}

	MEMEPP__IMPL_INLINE buffer_view::size_type buffer_view::size() const MEGOPP__NOEXCEPT
	{
        return MemeBuffer_size(memepp::to_pointer(data_));
	}
	
	MEMEPP__IMPL_INLINE bool buffer_view::empty() const MEGOPP__NOEXCEPT
	{
        return MemeBuffer_isEmpty(memepp::to_pointer(data_));
	}
	
	//MEMEPP__IMPL_INLINE buffer_view::size_type buffer_view::max_size() const MEGOPP__NOEXCEPT
	//MEMEPP__IMPL_INLINE buffer_view::size_type buffer_view::capacity() const MEGOPP__NOEXCEPT

	MEMEPP__IMPL_INLINE const_iterator buffer_view::begin() const MEGOPP__NOEXCEPT
	{
		return const_iterator{ data() };
	}
	
	MEMEPP__IMPL_INLINE const_iterator buffer_view::cbegin() const MEGOPP__NOEXCEPT
	{
        return const_iterator{ data() };
	}

	MEMEPP__IMPL_INLINE const_iterator buffer_view::end() const MEGOPP__NOEXCEPT
	{
        return const_iterator{ data() + size() };
	}
	
	MEMEPP__IMPL_INLINE const_iterator buffer_view::cend() const MEGOPP__NOEXCEPT
	{
        return const_iterator{ data() + size() };
	}
	
	MEMEPP__IMPL_INLINE buffer buffer_view::to_buffer() const
	{
		mmbufstk_t stack;
		MemeBufferStack_initByOther(
            &stack, MMS__OBJECT_SIZE, &data_);
		return stack;
	}
	
	MEMEPP__IMPL_INLINE string buffer_view::to_string(size_type _front_offset) const
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

	MEMEPP__IMPL_INLINE buffer_view::size_type buffer_view::index_of(
		const buffer_view& _other) const MEGOPP__NOEXCEPT
	{
		return MemeBuffer_indexOfWithOther(
			to_pointer(native_handle()), 0, to_pointer(_other.native_handle()));
	}

	MEMEPP__IMPL_INLINE buffer_view::size_type buffer_view::index_of(
		const_pointer _utf8, size_type _utf8_len) const MEGOPP__NOEXCEPT
	{
		return MemeBuffer_indexOfWithBytes(
			to_pointer(native_handle()), 0, reinterpret_cast<const uint8_t*>(_utf8), _utf8_len);
	}

	MEMEPP__IMPL_INLINE buffer_view::size_type buffer_view::find(const buffer_view& _other, size_type _pos) const MEGOPP__NOEXCEPT
	{
		return MemeBuffer_indexOfWithOther(
            to_pointer(native_handle()), _pos, to_pointer(_other.native_handle()));
	}

	MEMEPP__IMPL_INLINE bool buffer_view::contains(const buffer_view& _other) const MEGOPP__NOEXCEPT
	{
        return index_of(_other) != npos;
	}

	MEMEPP__IMPL_INLINE bool buffer_view::contains(const_pointer _utf8, size_type _count) const MEGOPP__NOEXCEPT
	{
        return index_of(_utf8, _count) != npos;
	}

	MEMEPP__IMPL_INLINE const buffer_view::native_handle_type& buffer_view::native_handle() const MEGOPP__NOEXCEPT
	{
        return data_;
	}

	MEMEPP__IMPL_INLINE bool operator==(const buffer_view& _lhs, const buffer_view& _rhs)
	{
		int result = 0;
		MemeBuffer_isEqualWithOther(
			to_pointer(_lhs.native_handle()),
			to_pointer(_rhs.native_handle()), &result);
		return result;
	}
	
	MEMEPP__IMPL_INLINE bool operator!=(const buffer_view& _lhs, const buffer_view& _rhs)
	{
		return !(_lhs == _rhs);
	}

};

#endif // !MEMEPP_BUFFER_VIEW_IMPL_HPP_INCLUDED
