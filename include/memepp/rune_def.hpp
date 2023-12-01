
#ifndef MEMEPP_RUNE_DEF_HPP_INCLUDED
#define MEMEPP_RUNE_DEF_HPP_INCLUDED

#include "meme/rune.h"
#include "memepp/string_fwd.hpp"

#include <memepp/iterator.hpp>

namespace memepp {

	class rune
	{
	public:
		using value_type = MemeByte_t;
		using size_type = MemeInteger_t;
		using difference_type = ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		static const size_type npos = static_cast<size_type>(-1);

		rune();
		rune(char _ch);
        rune(const_pointer _u8, size_type _size);
		rune(const MemeRune_t&);

		pointer data() noexcept;
		const_pointer data() const noexcept;
		size_type size() const noexcept;
		size_t	char_size() const noexcept;

		bool empty() const noexcept;
		bool valid() const noexcept;
        
        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        const_iterator cbegin() const noexcept;
        iterator end() noexcept;
        const_iterator end() const noexcept;
        const_iterator cend() const noexcept;

		bool is_multi() const noexcept;
		inline bool is_english_char() const noexcept { return !is_multi(); }

        inline operator bool() const noexcept { return valid() && !empty(); }

        inline MemeRune_t& native_handle() noexcept { return data_; }
        inline const MemeRune_t& native_handle() const noexcept { return data_; }
	private:
		MemeRune_t data_;
	};

	class rune_index
	{
	public:
		using value_type = MemeByte_t;
		using size_type = MemeInteger_t;
		using difference_type = ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		
		rune_index(const_pointer _u8, size_type _size);

		const_pointer data() const noexcept;
		size_type size() const noexcept;
		
	private:
		MemeRuneIndex_t data_;
	};
};

#endif // !MEMEPP_RUNE_DEF_HPP_INCLUDED
