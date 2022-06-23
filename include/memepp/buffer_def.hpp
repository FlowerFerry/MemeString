
#ifndef MEMEPP_BUFFER_DEF_HPP_INCLUDED
#define MEMEPP_BUFFER_DEF_HPP_INCLUDED

#include "meme/buffer_fwd.h"
#include "memepp/buffer_fwd.hpp"

#include "megopp/predef/keyword/noexcept.h"

namespace memepp {

	class buffer final
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

		MEMEPP__IMPL_INLINE buffer() MEGOPP__NOEXCEPT;
		MEMEPP__IMPL_INLINE buffer(native_handle_type&& _other);

		MEMEPP__IMPL_INLINE buffer(buffer&& _other);
		MEMEPP__IMPL_INLINE buffer(const buffer& _other);
		MEMEPP__IMPL_INLINE buffer(const buffer& _other, size_type _pos);
		MEMEPP__IMPL_INLINE buffer(const buffer& _other, size_type _pos, size_type _count);

		MEMEPP__IMPL_INLINE buffer(const_pointer _utf8, size_type _size);

		MEMEPP__IMPL_INLINE buffer(size_type _count, MemeByte_t _byte);

		buffer(std::nullptr_t) = delete;

		MEMEPP__IMPL_INLINE ~buffer();

		MEMEPP__IMPL_INLINE buffer& operator=(buffer&& _other);
		MEMEPP__IMPL_INLINE buffer& operator=(const buffer& _other);

		MEMEPP__IMPL_INLINE buffer_storage_type storage_type() const MEGOPP__NOEXCEPT;

		MEMEPP__IMPL_INLINE const_pointer data() const MEGOPP__NOEXCEPT;

		MEMEPP__IMPL_INLINE size_type size() const MEGOPP__NOEXCEPT;
		MEMEPP__IMPL_INLINE bool empty() const MEGOPP__NOEXCEPT;
		MEMEPP__IMPL_INLINE size_type max_size() const MEGOPP__NOEXCEPT;
		MEMEPP__IMPL_INLINE size_type capacity() const MEGOPP__NOEXCEPT;

		MEMEPP__IMPL_INLINE void swap(buffer& _other) MEGOPP__NOEXCEPT;

		MEMEPP__IMPL_INLINE size_type index_of(const buffer& _other) const MEGOPP__NOEXCEPT;

		MEMEPP__IMPL_INLINE size_type index_of_with_strlen(const char* _utf8, size_type _utf8_len) const MEGOPP__NOEXCEPT;

		MEMEPP__IMPL_INLINE const native_handle_type& native_handle() const MEGOPP__NOEXCEPT;

	private:
		native_handle_type data_;
	};

	MEMEPP__IMPL_INLINE bool operator==(const buffer& _lhs, const buffer& _rhs);
	MEMEPP__IMPL_INLINE bool operator!=(const buffer& _lhs, const buffer& _rhs);

}; // namespace memepp


#endif // !MEMEPP_BUFFER_DEF_HPP_INCLUDED
