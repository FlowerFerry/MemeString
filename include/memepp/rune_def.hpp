
#ifndef MEMEPP_RUNE_DEF_HPP_INCLUDED
#define MEMEPP_RUNE_DEF_HPP_INCLUDED

#include <mego/predef/lang/version.h>
#include <meme/rune.h>

#include <memepp/string_fwd.hpp>
#include <memepp/iterator.hpp>
#include <megopp/strs/rune.h>
#include <megopp/strs/rune_index.h>

namespace memepp {
inline namespace MMPP_NAMESPACE {

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

#if MG_LANG__CXX17_AVAIL
		inline static constexpr size_type npos = static_cast<size_type>(-1);
#else
		enum : size_type { npos = static_cast<size_type>(-1) };
#endif

		rune() noexcept;
		rune(char _ch) noexcept;
        rune(const_pointer _u8, size_type _size) noexcept;
		rune(const MemeRune_t&) noexcept;
		rune(MemeRune_t&&) noexcept;
		rune(const rune&) noexcept;
		rune(rune&&) noexcept;

		rune& operator=(char _ch) noexcept;
		rune& operator=(const_pointer _u8) noexcept;
		rune& operator=(const MemeRune_t&) noexcept;
		rune& operator=(MemeRune_t&&) noexcept;
		rune& operator=(const rune&) noexcept;
		rune& operator=(rune&&) noexcept;

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
		inline bool is_en_char() const noexcept { return !is_multi(); }

        bool is_ch_punct() const noexcept;
        bool is_en_punct() const noexcept;

        bool is_space() const noexcept;

        inline explicit operator bool() const noexcept { return valid() && !empty(); }
		
		inline bool operator==(char _ch) const noexcept
		{
            if (size() != 1) 
				return false;
            return data()[0] == _ch;
		}

        inline bool operator==(const char* _u8) const noexcept
        {
            size_t _size = strlen(_u8);
            if (static_cast<size_t>(size()) != _size)
                return false;
            return memcmp(data(), _u8, _size) == 0;
        }

		inline bool operator==(const rune& _r) const noexcept
		{
            if (size() != _r.size())
                return false;
            return memcmp(data(), _r.data(), size()) == 0;
		}

        inline bool operator!=(char _ch) const noexcept { return !(*this == _ch); }
		inline bool operator!=(const char* _u8) const noexcept { return !(*this == _u8); }
        inline bool operator!=(const rune& _r) const noexcept { return !(*this == _r); }

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
		rune_index(const MemeRuneIndex_t&) noexcept;
		rune_index(MemeRuneIndex_t&&) noexcept;
		rune_index(const rune_index&) noexcept;
		rune_index(rune_index&&) noexcept;
		
		const_pointer data() const noexcept;
		size_type size() const noexcept;
		
		bool is_space() const noexcept;
		
		inline MemeRuneIndex_t& native_handle() noexcept { return data_; }
		inline const MemeRuneIndex_t& native_handle() const noexcept { return data_; }
	private:
		MemeRuneIndex_t data_;
	};

}; // namespace MMPP_NAMESPACE
};

#endif // !MEMEPP_RUNE_DEF_HPP_INCLUDED
