
#ifndef MEMEPP_BUFFER_DEF_HPP_INCLUDED
#define MEMEPP_BUFFER_DEF_HPP_INCLUDED

#include "meme/buffer_fwd.h"
#include "memepp/buffer_fwd.hpp"
#include "memepp/string_fwd.hpp"

#include "megopp/predef/keyword/noexcept.h"

#include <memepp/iterator.hpp>

namespace memepp {

	class buffer
	{
	public:
		using value_type = MemeByte_t;
		using size_type = MemeInteger_t;
		using difference_type = ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		using native_handle_type = MemeBufferStack_t;

		static const size_type npos = static_cast<size_type>(-1);

		buffer() MEGOPP__NOEXCEPT;
		buffer(native_handle_type&& _other);

		buffer(buffer&& _other) MEGOPP__NOEXCEPT;
		buffer(const buffer& _other);
		buffer(const buffer& _other, size_type _pos);
		buffer(const buffer& _other, size_type _pos, size_type _count);

		buffer(const_pointer _utf8, size_type _size);
		buffer(const_pointer _utf8, size_type _size, buffer_storage_t _suggest);
		buffer(const_pointer _begin, const_pointer _end);

		buffer(size_type _count, MemeByte_t _byte);

		buffer(std::nullptr_t) = delete;

		~buffer();

		buffer& operator=(buffer&& _other) MEGOPP__NOEXCEPT;
		buffer& operator=(const buffer& _other);

		buffer_storage_t storage_type() const MEGOPP__NOEXCEPT;

		const_reference at(size_type _pos) const;

		const_pointer data() const MEGOPP__NOEXCEPT;

		size_type size() const MEGOPP__NOEXCEPT;
		bool empty() const MEGOPP__NOEXCEPT;
		size_type max_size() const MEGOPP__NOEXCEPT;
		size_type capacity() const MEGOPP__NOEXCEPT;

        const_iterator begin() const MEGOPP__NOEXCEPT;
        const_iterator cbegin() const MEGOPP__NOEXCEPT;
        
        const_iterator end() const MEGOPP__NOEXCEPT;
        const_iterator cend() const MEGOPP__NOEXCEPT;

		void swap(buffer& _other) MEGOPP__NOEXCEPT;
		string to_string(size_type _front_offset = 0) const;
		buffer to_large() const noexcept;
		
		size_type index_of(const buffer& _other) const MEGOPP__NOEXCEPT;
		size_type index_of(const_pointer _utf8, size_type _utf8_len) const MEGOPP__NOEXCEPT;

		bool contains(const buffer& _other) const MEGOPP__NOEXCEPT;
		bool contains(const_pointer _utf8, size_type _count) const MEGOPP__NOEXCEPT;

		const native_handle_type& native_handle() const MEGOPP__NOEXCEPT;

	private:
		native_handle_type data_;
	};

	bool operator==(const buffer& _lhs, const buffer& _rhs);
	bool operator!=(const buffer& _lhs, const buffer& _rhs);

}; // namespace memepp

#include <memepp/buffer_tmpimpl.hpp>

#endif // !MEMEPP_BUFFER_DEF_HPP_INCLUDED
