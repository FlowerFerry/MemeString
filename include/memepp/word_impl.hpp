
#ifndef MEMEPP_WORD_IMPL_HPP_INCLUDED
#define MEMEPP_WORD_IMPL_HPP_INCLUDED

#include "memepp/word_def.hpp"

namespace memepp {

    MEMEPP__IMPL_INLINE word::word()
    {
        data_ = MemeWord_getInitObject();
    }

    MEMEPP__IMPL_INLINE word::word(char _ch)
    {
        MemeWord_initByByte(&data_, _ch);
    }

    MEMEPP__IMPL_INLINE word::const_pointer word::data() const noexcept
    {
        return MemeWord_data(&data_);
    }

    MEMEPP__IMPL_INLINE word::size_type word::size() const noexcept
    {
        return MemeWord_size(&data_);
    }

    MEMEPP__IMPL_INLINE size_t word::length() const noexcept
    {
        return static_cast<size_t>(MemeWord_size(&data_));
    }

    MEMEPP__IMPL_INLINE bool word::empty() const noexcept
    {
        return MemeWord_isEmpty(&data_);
    }
    
    MEMEPP__IMPL_INLINE bool word::valid() const noexcept
    {
        return MemeWord_isValid(&data_);
    }


    MEMEPP__IMPL_INLINE bool word::is_multi() const noexcept 
    { 
        return MemeWord_isMulitChar(&data_); 
    }
};

#endif // !MEMEPP_WORD_IMPL_HPP_INCLUDED
