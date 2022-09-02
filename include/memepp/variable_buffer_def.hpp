
#ifndef MEMEPP_VARIABLE_BUFFER_DEF_HPP_INCLUDED
#define MEMEPP_VARIABLE_BUFFER_DEF_HPP_INCLUDED

#include "meme/variable_buffer_fwd.h"
#include "memepp/string_fwd.hpp"
#include "memepp/string_view_fwd.hpp"
#include "memepp/variable_buffer_fwd.hpp"
#include "memepp/buffer_fwd.hpp"
#include "megopp/predef/keyword/noexcept.h"
#include "megopp/endian/byte_swap.h"

namespace memepp {

	class variable_buffer
	{
	public:
		using value_type = MemeByte_t;
		using size_type = MemeInteger_t;
		using difference_type = ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		using native_handle_type = MemeVariableBufferStack_t;

		MEMEPP__IMPL_INLINE variable_buffer() noexcept;
		MEMEPP__IMPL_INLINE variable_buffer(const_pointer _buf, size_type _size);
		MEMEPP__IMPL_INLINE variable_buffer(size_type _count, value_type _value);

		MEMEPP__IMPL_INLINE variable_buffer(const variable_buffer& _other);
		MEMEPP__IMPL_INLINE variable_buffer(variable_buffer&& _other);

		MEMEPP__IMPL_INLINE ~variable_buffer();

		MEMEPP__IMPL_INLINE variable_buffer& operator=(const string& _other);
		MEMEPP__IMPL_INLINE variable_buffer& operator=(const string_view& _other);
		MEMEPP__IMPL_INLINE variable_buffer& operator=(const variable_buffer& _other);
		MEMEPP__IMPL_INLINE variable_buffer& operator=(variable_buffer&& _other);

		MEMEPP__IMPL_INLINE const_reference operator[](size_type _pos) const;
		MEMEPP__IMPL_INLINE reference operator[](size_type _pos);

		MEMEPP__IMPL_INLINE variable_buffer& operator+=(const variable_buffer& _other);
		MEMEPP__IMPL_INLINE variable_buffer& operator+=(variable_buffer&& _other);

		MEMEPP__IMPL_INLINE buffer_storage_type storage_type() const MEGOPP__NOEXCEPT;

		MEMEPP__IMPL_INLINE reference front();
		MEMEPP__IMPL_INLINE const_reference front() const;

		MEMEPP__IMPL_INLINE reference back();
		MEMEPP__IMPL_INLINE const_reference back() const;

		MEMEPP__IMPL_INLINE const_reference at(size_type _pos) const;
		MEMEPP__IMPL_INLINE reference at(size_type _pos);

		MEMEPP__IMPL_INLINE const_pointer data() const MEGOPP__NOEXCEPT;
		MEMEPP__IMPL_INLINE pointer data() MEGOPP__NOEXCEPT;
		MEMEPP__IMPL_INLINE size_type size() const MEGOPP__NOEXCEPT;
		MEMEPP__IMPL_INLINE bool empty() const MEGOPP__NOEXCEPT;
		MEMEPP__IMPL_INLINE size_type max_size() const MEGOPP__NOEXCEPT;
		MEMEPP__IMPL_INLINE size_type capacity() const MEGOPP__NOEXCEPT;

		MEMEPP__IMPL_INLINE void swap(variable_buffer& _other) MEGOPP__NOEXCEPT;

		MEMEPP__IMPL_INLINE void clear() MEGOPP__NOEXCEPT;

		MEMEPP__IMPL_INLINE variable_buffer& push_back(const value_type& value) MEGOPP__NOEXCEPT;
		MEMEPP__IMPL_INLINE variable_buffer& pop_back() MEGOPP__NOEXCEPT;

		MEMEPP__IMPL_INLINE variable_buffer& push_front(const value_type& value) MEGOPP__NOEXCEPT;
		MEMEPP__IMPL_INLINE variable_buffer& pop_front() MEGOPP__NOEXCEPT;

		MEMEPP__IMPL_INLINE void reserve(size_type _new_cap);

		MEMEPP__IMPL_INLINE variable_buffer& append(const_pointer _buf, size_type _len);
		MEMEPP__IMPL_INLINE variable_buffer& append(const variable_buffer& _other);
		MEMEPP__IMPL_INLINE variable_buffer& append(variable_buffer&& _other);
		MEMEPP__IMPL_INLINE variable_buffer& append(const string& _other);
		MEMEPP__IMPL_INLINE variable_buffer& append(const string_view& _other);

		template<typename _Ty>
		inline variable_buffer& append(const _Ty& _v, megopp::endian_t _endian);

		MEMEPP__IMPL_INLINE variable_buffer& resize(size_type _count);
		MEMEPP__IMPL_INLINE variable_buffer& resize(size_type _count, value_type _value);

		MEMEPP__IMPL_INLINE MemeInteger_t release(buffer& _buf);
		MEMEPP__IMPL_INLINE MemeInteger_t release(string& _str);

		MEMEPP__IMPL_INLINE const native_handle_type& native_handle() const MEGOPP__NOEXCEPT;

	private:
		native_handle_type data_;
	};

};

#endif // !MEMEPP_VARIABLE_BUFFER_DEF_HPP_INCLUDED
