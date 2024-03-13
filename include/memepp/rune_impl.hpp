
#ifndef MEMEPP_RUNE_IMPL_HPP_INCLUDED
#define MEMEPP_RUNE_IMPL_HPP_INCLUDED

#include "memepp/rune_def.hpp"

namespace memepp {

    MEMEPP__IMPL_INLINE rune::rune()
    {
        data_ = MemeRune_getInitObject();
    }

    MEMEPP__IMPL_INLINE rune::rune(char _ch)
    {
        MemeRune_initByByte(&data_, _ch);
    }

    MEMEPP__IMPL_INLINE rune::rune(const_pointer _u8, size_type _size)
    {
        data_ = MemeRune_getInitObject();
        MemeRune_initByUtf8Bytes(&data_, _u8, _size);
    }

    MEMEPP__IMPL_INLINE rune::rune(const MemeRune_t& _rune)
    {
        data_ = _rune;
    }

    MEMEPP__IMPL_INLINE rune::const_pointer rune::data() const noexcept
    {
        return MemeRune_data(&data_);
    }
    
    MEMEPP__IMPL_INLINE rune::pointer rune::data() noexcept
    {
        return const_cast<value_type*>(MemeRune_data(&data_));
    }

    MEMEPP__IMPL_INLINE rune::size_type rune::size() const noexcept
    {
        return MemeRune_size(&data_);
    }

    MEMEPP__IMPL_INLINE size_t rune::char_size() const noexcept
    {
        return static_cast<size_t>(MemeRune_size(&data_));
    }

    MEMEPP__IMPL_INLINE bool rune::empty() const noexcept
    {
        return MemeRune_isEmpty(&data_);
    }
    
    MEMEPP__IMPL_INLINE bool rune::valid() const noexcept
    {
        return MemeRune_isValid(&data_);
    }

    MEMEPP__IMPL_INLINE const_iterator rune::begin() const noexcept
    {
        return const_iterator(data());
    }
    
    MEMEPP__IMPL_INLINE iterator rune::begin() noexcept
    {
        return iterator(data());
    }

    MEMEPP__IMPL_INLINE const_iterator rune::cbegin() const noexcept
    {
        return const_iterator(data());
    }

    MEMEPP__IMPL_INLINE const_iterator rune::end() const noexcept
    {
        return const_iterator(data() + size());
    }
    
    MEMEPP__IMPL_INLINE iterator rune::end() noexcept
    {
        return iterator(data() + size());
    }

    MEMEPP__IMPL_INLINE const_iterator rune::cend() const noexcept
    {
        return const_iterator(data() + size());
    }

    MEMEPP__IMPL_INLINE bool rune::is_multi() const noexcept 
    { 
        return MemeRune_isMulitChar(&data_); 
    }

    MEMEPP__IMPL_INLINE bool rune::is_ch_punct() const noexcept
    {
        return MemeRune_isChPunct(&data_);
    }
    
    MEMEPP__IMPL_INLINE bool rune::is_en_punct() const noexcept
    {
        if (size() == 1) {
            return ispunct(*data());
        }
        return false;
    }

    MEMEPP__IMPL_INLINE bool rune::is_space() const noexcept
    {
        return MemeRune_isSpace(&data_);
    }

    MEMEPP__IMPL_INLINE rune_index::rune_index(const_pointer _u8, size_type _size)
    {
        data_.data = _u8;
        data_.size = (int8_t)_size;
    }

    MEMEPP__IMPL_INLINE rune_index::const_pointer rune_index::data() const noexcept
    {
        return data_.data;
    }

    MEMEPP__IMPL_INLINE rune_index::size_type rune_index::size() const noexcept
    {
        return data_.size;
    }

};

#endif // !MEMEPP_RUNE_IMPL_HPP_INCLUDED
