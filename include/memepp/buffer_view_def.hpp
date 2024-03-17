
#ifndef MEMEPP_BUFFER_VIEW_DEF_HPP_INCLUDED
#define MEMEPP_BUFFER_VIEW_DEF_HPP_INCLUDED

#include "meme/buffer_fwd.h"
#include "memepp/buffer_view_fwd.hpp"

#include "memepp/buffer_fwd.hpp"
#include "memepp/string_fwd.hpp"
#include "memepp/string_view_fwd.hpp"
#include "memepp/variable_buffer_fwd.hpp"
#include "megopp/predef/keyword/noexcept.h"

#include "memepp/iterator.hpp"

namespace memepp {

    class buffer_view
	{
    public:
		using value_type = MemeByte_t;
		using size_type = MemeInteger_t;
		using difference_type = ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		using native_handle_type = mmbufstk_t;

		static const size_type npos = static_cast<size_type>(-1);
        
		buffer_view() MEGOPP__NOEXCEPT;
		buffer_view(const buffer& _other) MEGOPP__NOEXCEPT;
		buffer_view(const string& _other) MEGOPP__NOEXCEPT;
		buffer_view(const variable_buffer& _other) MEGOPP__NOEXCEPT;
		buffer_view(const string_view& _other) MEGOPP__NOEXCEPT;
		buffer_view(const buffer_view& _other) MEGOPP__NOEXCEPT;
		buffer_view(buffer&& _other);
		buffer_view(string&& _other);
		buffer_view(variable_buffer&& _other) = delete;
		buffer_view(string_view&& _other) MEGOPP__NOEXCEPT;
		buffer_view(buffer_view&& _other) MEGOPP__NOEXCEPT;
		buffer_view(const_pointer _data, size_type _size) MEGOPP__NOEXCEPT;
		
		~buffer_view();

		buffer_view& operator=(const buffer& _other) MEGOPP__NOEXCEPT;
		buffer_view& operator=(const string& _other) MEGOPP__NOEXCEPT;
		buffer_view& operator=(const variable_buffer& _other) MEGOPP__NOEXCEPT;
		buffer_view& operator=(const string_view& _other) MEGOPP__NOEXCEPT;
		buffer_view& operator=(const buffer_view& _other) MEGOPP__NOEXCEPT;
		buffer_view& operator=(buffer_view&& _other) MEGOPP__NOEXCEPT;
		
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

		buffer to_buffer() const;
		string to_string(size_type _front_offset = 0) const;

		size_type index_of(const buffer_view& _other) const MEGOPP__NOEXCEPT;
		size_type index_of(const_pointer _utf8, size_type _utf8_len) const MEGOPP__NOEXCEPT;
		
		size_type find(const buffer_view& _other, size_type _pos = 0) const MEGOPP__NOEXCEPT;

		bool contains(const buffer_view& _other) const MEGOPP__NOEXCEPT;
		bool contains(const_pointer _utf8, size_type _count) const MEGOPP__NOEXCEPT;

		bool starts_with(const buffer_view& _other) const MEGOPP__NOEXCEPT;
		bool starts_with(const_pointer _utf8, size_type _count) const MEGOPP__NOEXCEPT;

		bool ends_with(const buffer_view& _other) const MEGOPP__NOEXCEPT;
		bool ends_with(const_pointer _utf8, size_type _count) const MEGOPP__NOEXCEPT;

        buffer_view slice(size_type _pos = 0, size_type _count = npos) const MEGOPP__NOEXCEPT;
		
		const native_handle_type& native_handle() const MEGOPP__NOEXCEPT;

	private:
		native_handle_type data_;
    };

	bool operator==(const buffer_view& _lhs, const buffer_view& _rhs) MEGOPP__NOEXCEPT;
	bool operator!=(const buffer_view& _lhs, const buffer_view& _rhs) MEGOPP__NOEXCEPT;
	
};

#endif // !MEMEPP_BUFFER_VIEW_DEF_HPP_INCLUDED
