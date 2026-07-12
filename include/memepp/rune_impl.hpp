
#ifndef MEMEPP_RUNE_IMPL_HPP_INCLUDED
#define MEMEPP_RUNE_IMPL_HPP_INCLUDED

#include "memepp/rune_def.hpp"

#include <meme/utf/u8rune.h>

namespace memepp {
inline namespace MMPP_NAMESPACE {

    MEMEPP__IMPL_INLINE rune::rune() noexcept
    {
        data_ = MemeRune_getInitObject();
    }

    MEMEPP__IMPL_INLINE rune::rune(char _ch) noexcept
    {
        MemeRune_initByByte(&data_, _ch);
    }

    MEMEPP__IMPL_INLINE rune::rune(const_pointer _u8, size_type _size) noexcept
    {
        data_ = MemeRune_getInitObject();
        MemeRune_initByUtf8Bytes(&data_, _u8, _size);
    }

    MEMEPP__IMPL_INLINE rune::rune(const MemeRune_t& _rune) noexcept:
        data_(_rune)
    {
    }

    MEMEPP__IMPL_INLINE rune::rune(MemeRune_t&& _rune) noexcept:
        data_(std::move(_rune))
    {
    }

    MEMEPP__IMPL_INLINE rune::rune(const rune& _other) noexcept:
        data_(_other.data_)
    {
    }

    MEMEPP__IMPL_INLINE rune::rune(rune&& _other) noexcept:
        data_(std::move(_other.data_))
    {
    }

    MEMEPP__IMPL_INLINE rune& rune::operator=(char _ch) noexcept
    {
        MemeRune_initByByte(&data_, _ch);
        return *this;
    }
    
    MEMEPP__IMPL_INLINE rune& rune::operator=(const_pointer _u8) noexcept
    {
        MemeRune_initByUtf8Bytes(&data_, _u8, -1);
        return *this;
    }

    MEMEPP__IMPL_INLINE rune& rune::operator=(const MemeRune_t& _rune) noexcept
    {
        data_ = _rune;
        return *this;
    }

    MEMEPP__IMPL_INLINE rune& rune::operator=(MemeRune_t&& _rune) noexcept
    {
        data_ = std::move(_rune);
        return *this;
    }

    MEMEPP__IMPL_INLINE rune& rune::operator=(const rune& _other) noexcept
    {
        if (this != &_other) {
            data_ = _other.data_;
        }
        return *this;
    }

    MEMEPP__IMPL_INLINE rune& rune::operator=(rune&& _other) noexcept
    {
        if (this != &_other) {
            data_ = std::move(_other.data_);
        }
        return *this;
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

    MEMEPP__IMPL_INLINE bool rune::is_ascii() const noexcept
    {
        return size() == 1 && (data_.byte[0] <= 0x7F);
    }

    MEMEPP__IMPL_INLINE bool rune::is_en_lower() const noexcept
    {
        return size() == 1 && islower(data_.byte[0]);
    }

    MEMEPP__IMPL_INLINE bool rune::is_en_upper() const noexcept
    {
        return size() == 1 && isupper(data_.byte[0]);
    }

    MEMEPP__IMPL_INLINE int rune::compare(const rune& _r) const noexcept
    {
        size_type min_sz = (size() < _r.size()) ? size() : _r.size();
        int cmp = memcmp(data(), _r.data(), min_sz);
        if (cmp == 0) {
            if (size() < _r.size()) return -1;
            if (size() > _r.size()) return  1;
            return 0;
        }
        return cmp;
    }

    MEMEPP__IMPL_INLINE void rune::clear() noexcept
    {
        data_ = MemeRune_getInitObject();
    }

    MEMEPP__IMPL_INLINE rune rune::to_en_lower() const noexcept
    {
        if (size() == 1 && isupper(data_.byte[0])) {
            rune r;
            MemeRune_initByByte(&r.data_, static_cast<char>(tolower(data_.byte[0])));
            return r;
        }
        return *this;
    }

    MEMEPP__IMPL_INLINE rune rune::to_en_upper() const noexcept
    {
        if (size() == 1 && islower(data_.byte[0])) {
            rune r;
            MemeRune_initByByte(&r.data_, static_cast<char>(toupper(data_.byte[0])));
            return r;
        }
        return *this;
    }

    MEMEPP__IMPL_INLINE uint32_t rune::codepoint() const noexcept
    {
        if (empty())
            return 0;
        uint32_t value = 0;
        mmutf_u8rune_get_u32(MemeRune_data(&data_), (MemeInteger_t)size(), &value);
        return value;
    }

    MEMEPP__IMPL_INLINE rune rune::from_codepoint(uint32_t _cp) noexcept
    {
        rune r;
        MemeByte_t buf[7];
        int len = mmutf_u8rune_set_u32(buf, sizeof(buf), _cp);
        if (len > 0 && len <= static_cast<int>(MEME_RUNE__MAX_CHAR_SIZE)) {
            MemeRune_initByUtf8Bytes(&r.data_, buf, len);
        }
        return r;
    }

    MEMEPP__IMPL_INLINE rune_index::rune_index(const_pointer _u8, size_type _size)
    {
        data_.data = _u8;
        data_.size = (int8_t)_size;
    }

    MEMEPP__IMPL_INLINE rune_index::rune_index(const MemeRuneIndex_t& other) noexcept : data_(other) {}

    MEMEPP__IMPL_INLINE rune_index::rune_index(MemeRuneIndex_t&& other) noexcept : data_(std::move(other)) {}

    MEMEPP__IMPL_INLINE rune_index::rune_index(const rune_index& other) noexcept : data_(other.data_) {}

    MEMEPP__IMPL_INLINE rune_index::rune_index(rune_index&& other) noexcept : data_(std::move(other.data_)) {}

    MEMEPP__IMPL_INLINE rune_index::const_pointer rune_index::data() const noexcept
    {
        return data_.data;
    }

    MEMEPP__IMPL_INLINE rune_index::size_type rune_index::size() const noexcept
    {
        return data_.size;
    }
    
    MEMEPP__IMPL_INLINE bool rune_index::is_space() const noexcept
    {
        return MemeRuneIndex_isSpace(data(), (int)size());
    }

}; // namespace MMPP_NAMESPACE
};

#endif // !MEMEPP_RUNE_IMPL_HPP_INCLUDED
