
#ifndef MEMEPP_WORD_DEF_HPP_INCLUDED
#define MEMEPP_WORD_DEF_HPP_INCLUDED

#include "meme/string_fwd.h"

#include "memepp/string_fwd.hpp"

namespace memepp {

	class word
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

		MEMEPP__IMPL_INLINE word();
		MEMEPP__IMPL_INLINE word(char _ch);

		MEMEPP__IMPL_INLINE const_pointer data() const;
		MEMEPP__IMPL_INLINE size_type size() const;
		MEMEPP__IMPL_INLINE size_t	length() const;

		constexpr inline bool is_multi() const noexcept { return !!(data_.byte[0] & 0x80); }
		constexpr inline bool is_english_char() const noexcept { return !is_multi(); }

	private:
		MemeWord_t data_;
	};

};

#endif // !MEMEPP_WORD_DEF_HPP_INCLUDED
