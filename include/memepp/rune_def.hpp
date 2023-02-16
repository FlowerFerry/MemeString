
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

		MEMEPP__IMPL_INLINE rune();
		MEMEPP__IMPL_INLINE rune(char _ch);

		MEMEPP__IMPL_INLINE pointer data() noexcept;
		MEMEPP__IMPL_INLINE const_pointer data() const noexcept;
		MEMEPP__IMPL_INLINE size_type size() const noexcept;
		MEMEPP__IMPL_INLINE size_t	length() const noexcept;

		MEMEPP__IMPL_INLINE bool empty() const noexcept;
		MEMEPP__IMPL_INLINE bool valid() const noexcept;
        
        MEMEPP__IMPL_INLINE iterator begin() noexcept;
        MEMEPP__IMPL_INLINE const_iterator begin() const noexcept;
        MEMEPP__IMPL_INLINE const_iterator cbegin() const noexcept;
        MEMEPP__IMPL_INLINE iterator end() noexcept;
        MEMEPP__IMPL_INLINE const_iterator end() const noexcept;
        MEMEPP__IMPL_INLINE const_iterator cend() const noexcept;

		MEMEPP__IMPL_INLINE bool is_multi() const noexcept;
		inline bool is_english_char() const noexcept { return !is_multi(); }

        inline operator bool() const noexcept { return valid() && !empty(); }

	private:
		MemeRune_t data_;
	};

};

#endif // !MEMEPP_RUNE_DEF_HPP_INCLUDED
