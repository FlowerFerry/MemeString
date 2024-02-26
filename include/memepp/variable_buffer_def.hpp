
#ifndef MEMEPP_VARIABLE_BUFFER_DEF_HPP_INCLUDED
#define MEMEPP_VARIABLE_BUFFER_DEF_HPP_INCLUDED

#include "meme/variable_buffer_fwd.h"
#include "memepp/string_fwd.hpp"
#include "memepp/string_view_fwd.hpp"
#include "memepp/variable_buffer_fwd.hpp"
#include "memepp/buffer_fwd.hpp"
#include <memepp/buffer_view_fwd.hpp>
#include "megopp/predef/keyword/noexcept.h"
#include "megopp/endian/byte_swap.h"

#include <memepp/iterator.hpp>

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
        
		static const size_type npos = static_cast<size_type>(-1);

		variable_buffer() noexcept;
		variable_buffer(const_pointer _buf, size_type _size);
		variable_buffer(size_type _count, value_type _value);

		variable_buffer(const variable_buffer& _other);
		variable_buffer(variable_buffer&& _other) noexcept;

		~variable_buffer();

		variable_buffer& operator=(const string& _other);
		variable_buffer& operator=(const string_view& _other);
		variable_buffer& operator=(const variable_buffer& _other);
		variable_buffer& operator=(variable_buffer&& _other) noexcept;

		const_reference operator[](size_type _pos) const;
		reference operator[](size_type _pos);

		variable_buffer& operator+=(const variable_buffer& _other);
		variable_buffer& operator+=(variable_buffer&& _other);

		buffer_storage_t storage_type() const MEGOPP__NOEXCEPT;

		reference front();
		const_reference front() const;

		reference back();
		const_reference back() const;

		const_reference at(size_type _pos) const;
		reference at(size_type _pos);

		const_pointer data() const MEGOPP__NOEXCEPT;
		pointer data() MEGOPP__NOEXCEPT;
		size_type size() const MEGOPP__NOEXCEPT;
		bool empty() const MEGOPP__NOEXCEPT;
		size_type max_size() const MEGOPP__NOEXCEPT;
		size_type capacity() const MEGOPP__NOEXCEPT;

		iterator begin() MEGOPP__NOEXCEPT;
		const_iterator begin() const MEGOPP__NOEXCEPT;
		const_iterator cbegin() const MEGOPP__NOEXCEPT;
        
        iterator end() MEGOPP__NOEXCEPT;
        const_iterator end() const MEGOPP__NOEXCEPT;
        const_iterator cend() const MEGOPP__NOEXCEPT;

        size_type find(const string_view& _other, size_type _pos = 0) const MEGOPP__NOEXCEPT;
        size_type find(const variable_buffer& _other, size_type _pos = 0) const MEGOPP__NOEXCEPT;
        size_type find(const buffer& _other, size_type _pos = 0) const MEGOPP__NOEXCEPT;
        size_type find(value_type _value, size_type _pos = 0) const MEGOPP__NOEXCEPT;
        size_type find(const_pointer _buf, size_type _pos, size_type _size) const MEGOPP__NOEXCEPT;


		void swap(variable_buffer& _other) MEGOPP__NOEXCEPT;

		void clear() MEGOPP__NOEXCEPT;

		variable_buffer& push_back(const value_type& value);
		//variable_buffer& pop_back() MEGOPP__NOEXCEPT;
		template<typename _Ty>
		inline variable_buffer& push_back(const _Ty& _v, megopp::endian_t _endian);

		variable_buffer& push_front(const value_type& value);
		//variable_buffer& pop_front() MEGOPP__NOEXCEPT;
		template<typename _Ty>
		inline variable_buffer& push_front(const _Ty& _v, megopp::endian_t _endian);

		void reserve(size_type _new_cap);

		variable_buffer& append(const_pointer _buf, size_type _len);
		variable_buffer& append(const variable_buffer& _other);
		variable_buffer& append(variable_buffer&& _other);
		variable_buffer& append(const string& _other);
		variable_buffer& append(const string_view& _other);
        variable_buffer& append(const buffer& _other);
        variable_buffer& append(const buffer_view& _other);
		template<typename _Ty>
		inline variable_buffer& append(const _Ty& _v, megopp::endian_t _endian);

		//iterator insert(const_iterator _pos, const value_type& _value);
  //      iterator insert(const_iterator _pos, size_type _count, const value_type& _value);
        iterator insert(const_iterator _pos, const_pointer _buf, size_type _count);
        //iterator insert(const_iterator _pos, const variable_buffer& _other);
        //iterator insert(const_iterator _pos, variable_buffer&& _other);
        //iterator insert(const_iterator _pos, const string& _other);
        //iterator insert(const_iterator _pos, const string_view& _other);
		variable_buffer& insert(size_type _pos, const_pointer _buf, size_type _count);
		template<typename _Ty>
		inline variable_buffer& insert(size_type _pos, const _Ty& _v, megopp::endian_t _endian);

        variable_buffer& remove(size_type _pos, size_type _count);

		variable_buffer& resize(size_type _count);
		variable_buffer& resize(size_type _count, value_type _value);

		MemeInteger_t release(buffer& _buf);
		MemeInteger_t release(string& _str);

		const native_handle_type& native_handle() const MEGOPP__NOEXCEPT;

	private:
		native_handle_type data_;
	};

	template<typename _Ty>
	inline variable_buffer& variable_buffer::push_back(const _Ty& _v, megopp::endian_t _endian)
	{
        return append(_v, _endian);
	}
    
	template<typename _Ty>
	inline variable_buffer& variable_buffer::push_front(const _Ty& _v, megopp::endian_t _endian)
	{
        return insert(0, _v, _endian);
	}

	template<typename _Ty>
	inline variable_buffer& variable_buffer::append(const _Ty& _v, megopp::endian_t _endian)
	{
		typename megopp::type_with_size<sizeof(_Ty)>::uint _value = 0;
		memcpy(&_value, &_v, sizeof(_value));

#if MEGO_ENDIAN__LITTLE_BYTE
        if (_endian == megopp::endian_t::big_byte)
        {
            _value = megopp::endian::byte_swap(_value);
        }
#elif MEGO_ENDIAN__BIG_BYTE
        if (_endian == megopp::endian_t::little_byte)
        {
			_value = megopp::endian::byte_swap(_value);
        }
#endif
        return append(reinterpret_cast<const_pointer>(&_value), static_cast<size_type>(sizeof(_value)));
	}
    
	template<typename _Ty>
	inline variable_buffer& variable_buffer::insert(size_type _pos, const _Ty& _v, megopp::endian_t _endian)
	{
        typename megopp::type_with_size<sizeof(_Ty)>::uint _value = 0;
        memcpy(&_value, &_v, sizeof(_value));
        
#if MEGO_ENDIAN__LITTLE_BYTE
        if (_endian == megopp::endian_t::big_byte)
        {
            _value = megopp::endian::byte_swap(_value);
        }
#elif MEGO_ENDIAN__BIG_BYTE
        if (_endian == megopp::endian_t::little_byte)
        {
            _value = megopp::endian::byte_swap(_value);
        }
#endif
        return insert(_pos, reinterpret_cast<const_pointer>(&_value), static_cast<size_type>(sizeof(_value)));
	}

};

#endif // !MEMEPP_VARIABLE_BUFFER_DEF_HPP_INCLUDED
