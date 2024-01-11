
#ifndef MEGOPP_ENDIAN_BIT_FIELD_MEMBER_H_INCLUDED
#define MEGOPP_ENDIAN_BIT_FIELD_MEMBER_H_INCLUDED

#include <stdint.h>

#include <type_traits>
#include "types.h"
#include "megopp/help/type_traits.h"
#include <megopp/endian/byte_swap.h>

namespace megopp {
namespace endian {

	template<size_t _BeginBit, size_t _BitSize, typename _Type, endian_t _Endian>
	struct bit_field_member_private
    {
        using uint_type = typename types_by_size<sizeof(_Type)>::uint;

        static constexpr uint_type total_bit_size   = sizeof(_Type) * CHAR_BIT;
        static constexpr uint_type mask             = ((1 << _BitSize) - 1);

        inline constexpr uint_type get_value() const noexcept
        {
            return 0;
        }

        inline constexpr void set_value(uint_type _value) noexcept
        {
        }

    };

    template<size_t _BeginBit, size_t _BitSize, typename _Type>
    struct bit_field_member_private<_BeginBit, _BitSize, _Type, 
#if MEGO_ENDIAN__LITTLE_BYTE
        endian_t::little_byte
#elif MEGO_ENDIAN__BIG_BYTE
        endian_t::big_byte
#elif MEGO_ENDIAN__LITTLE_WORD
        endian_t::little_word
#elif MEGO_ENDIAN__BIG_WORD
        endian_t::big_word
#endif
    >
    {
        using uint_type = typename types_by_size<sizeof(_Type)>::uint;

        static constexpr uint_type total_bit_size   = sizeof(_Type) * CHAR_BIT;
        static constexpr uint_type mask             = ((1 << _BitSize) - 1);
        static constexpr uint_type begin_bit_index  = _BeginBit;
        static constexpr uint_type end_bit_index    = _BeginBit + _BitSize;
        static constexpr uint_type begin_byte_index = begin_bit_index / CHAR_BIT;
        static constexpr uint_type end_byte_index   = (end_bit_index + CHAR_BIT - 1) / CHAR_BIT;
        static constexpr uint_type byte_size        = end_byte_index - begin_byte_index;

        inline constexpr uint_type get_value() const noexcept
        {
            return (data_ >> begin_bit_index) & mask;
        }

        inline constexpr void set_value(uint_type _value) noexcept
        {
            data_ = (data_ & ~(mask << begin_bit_index)) | ((_value & mask) << begin_bit_index);
        }
        
        uint_type data_;
    };

#if MEGO_ENDIAN__LITTLE_BYTE || MEGO_ENDIAN__BIG_BYTE
    template<size_t _BeginBit, size_t _BitSize, typename _Type>
    struct bit_field_member_private<_BeginBit, _BitSize, _Type,
#if MEGO_ENDIAN__LITTLE_BYTE
        endian_t::big_byte
#else
        endian_t::little_byte
#endif
    >
    {
        using uint_type = typename types_by_size<sizeof(_Type)>::uint;

        static constexpr uint_type total_bit_size   = sizeof(_Type) * CHAR_BIT;
        static constexpr uint_type mask             = ((1 << _BitSize) - 1);
        static constexpr uint_type begin_bit_index  = total_bit_size - _BeginBit - _BitSize;
        static constexpr uint_type end_bit_index    = total_bit_size - _BeginBit;
        static constexpr uint_type begin_byte_index = begin_bit_index / CHAR_BIT;
        static constexpr uint_type end_byte_index   = (end_bit_index + CHAR_BIT - 1) / CHAR_BIT;
        static constexpr uint_type byte_size        = end_byte_index - begin_byte_index;
        static constexpr uint_type rbegin_bit_index = _BeginBit;
        static constexpr uint_type rend_bit_index   = _BeginBit + _BitSize;

        inline constexpr uint_type get_value() const noexcept
        {
            if constexpr ((_BeginBit + _BitSize) / 8 == 0)
            {
                return ((data_ >> (total_bit_size - CHAR_BIT)) >> rbegin_bit_index) & mask;
            }
            else {
                uint_type value = megopp::endian::byte_swap(data_);
                return (value >> rbegin_bit_index) & mask;
            }
        }

        inline constexpr void set_value(uint_type _value) noexcept
        {
            _value = (_value & mask) << rbegin_bit_index;
            _value = megopp::endian::byte_swap(_value);
            data_  = (data_ & ~(mask << begin_bit_index)) | _value;
        }

        uint_type data_;
    };
#else
    #error "not support"
#endif

	template<size_t _BeginBit, size_t _BitSize, endian_t _Endian,
		typename _Type = typename types_by_bit_size<_BeginBit, _BitSize>::uint>
	struct bit_field_member
    {
        using data_type = bit_field_member_private<_BeginBit, _BitSize, _Type, _Endian>;
        using uint_type = typename data_type::uint_type;

        static constexpr uint_type total_bit_size   = data_type::total_bit_size;
        static constexpr uint_type mask             = data_type::mask;
        static constexpr uint_type begin_bit_index  = data_type::begin_bit_index;
        static constexpr uint_type end_bit_index    = data_type::end_bit_index;
        static constexpr uint_type begin_byte_index = data_type::begin_byte_index;
        static constexpr uint_type end_byte_index   = data_type::end_byte_index;
        static constexpr uint_type byte_size        = data_type::byte_size;

        inline constexpr explicit operator _Type() const noexcept
        {
            return static_cast<_Type>(data_.get_value());
        }

        inline constexpr explicit operator bool() const noexcept
        {
            return data_.get_value() != 0;
        }

        inline constexpr _Type get_value() const noexcept
        {
            return data_.get_value();
        }

        inline constexpr void set_value(const _Type& _value) noexcept
        {
            data_.set_value(_value);
        }

        template<typename _Ty>
        inline constexpr bool operator==(_Ty _value) const noexcept
        {
            return data_.get_value() == static_cast<uint_type>(_value);
        }

        template<typename _Ty>
        inline constexpr bool operator!=(_Ty _value) const noexcept
        {
            return !(*this == _value);
        }

        template<size_t _BeginBit2, size_t _BitSize2, endian_t _Endian2, typename _Type2>
        inline constexpr bool operator==(const bit_field_member<_BeginBit2, _BitSize2, _Endian2, _Type2>& _other) const noexcept
        {
            return data_.get_value() == _other.data_.get_value();
        }

        template<size_t _BeginBit2, size_t _BitSize2, endian_t _Endian2, typename _Type2>
        inline constexpr bool operator!=(const bit_field_member<_BeginBit2, _BitSize2, _Endian2, _Type2>& _other) const noexcept
        {
            return data_.get_value() != _other.data_.get_value();
        }

        template<typename _Ty>
        inline constexpr _Ty as() const noexcept
        {
            return static_cast<_Ty>(data_.get_value());
        }

        inline constexpr bit_field_member& operator=(const _Type& _value) noexcept
        {
            data_.set_value(static_cast<uint_type>(_value));
            return *this;
        }

        inline constexpr bit_field_member& operator++() noexcept
        {
            data_.set_value(data_.get_value() + 1);
            return *this;
        }

        inline constexpr bit_field_member operator++(int) noexcept
        {
            bit_field_member tmp = *this;
            ++*this;
            return tmp;
        }

        inline constexpr bit_field_member& operator--() noexcept
        {
            data_.set_value(data_.get_value() - 1);
            return *this;
        }

        inline constexpr bit_field_member operator--(int) noexcept
        {
            bit_field_member tmp = *this;
            --*this;
            return tmp;
        }

        inline constexpr _Type operator+(const _Type& _value) const noexcept
        {
            return data_.get_value() + _value;
        }

        inline constexpr _Type operator-(const _Type& _value) const noexcept
        {
            return data_.get_value() - _value;
        }

        inline constexpr _Type operator*(const _Type& _value) const noexcept
        {
            return data_.get_value() * _value;
        }

        inline constexpr _Type operator/(const _Type& _value) const noexcept
        {
            return data_.get_value() / _value;
        }

        inline constexpr _Type operator%(const _Type& _value) const noexcept
        {
            return data_.get_value() % _value;
        }

        inline constexpr _Type operator&(const _Type& _value) const noexcept
        {
            return data_.get_value() & _value;
        }

        inline constexpr _Type operator|(const _Type& _value) const noexcept
        {
            return data_.get_value() | _value;
        }

        inline constexpr _Type operator^(const _Type& _value) const noexcept
        {
            return data_.get_value() ^ _value;
        }

        inline constexpr _Type operator<<(const _Type& _value) const noexcept
        {
            return data_.get_value() << _value;
        }

        inline constexpr _Type operator>>(const _Type& _value) const noexcept
        {
            return data_.get_value() >> _value;
        }

        inline constexpr _Type operator~() const noexcept
        {
            return ~ data_.get_value();
        }

        inline constexpr bool operator<(const _Type& _value) const noexcept
        {
            return data_.get_value() < _value;
        }

        inline constexpr bool operator>(const _Type& _value) const noexcept
        {
            return data_.get_value() > _value;
        }

        inline constexpr bool operator<=(const _Type& _value) const noexcept
        {
            return data_.get_value() <= _value;
        }

        inline constexpr bool operator>=(const _Type& _value) const noexcept
        {
            return data_.get_value() >= _value;
        }

        inline constexpr bit_field_member& operator+=(const _Type& _value) noexcept
        {
            data_.set_value(data_.get_value() + _value);
            return *this;
        }

        inline constexpr bit_field_member& operator-=(const _Type& _value) noexcept
        {
            data_.set_value(data_.get_value() - _value);
            return *this;
        }

        inline constexpr bit_field_member& operator*=(const _Type& _value) noexcept
        {
            data_.set_value(data_.get_value() * _value);
            return *this;
        }

        inline constexpr bit_field_member& operator/=(const _Type& _value) noexcept
        {
            data_.set_value(data_.get_value() / _value);
            return *this;
        }

        inline constexpr bit_field_member& operator%=(const _Type& _value) noexcept
        {
            data_.set_value(data_.get_value() % _value);
            return *this;
        }

        inline constexpr bit_field_member& operator&=(const _Type& _value) noexcept
        {
            data_.set_value(data_.get_value() & _value);
            return *this;
        }

        inline constexpr bit_field_member& operator|=(const _Type& _value) noexcept
        {
            data_.set_value(data_.get_value() | _value);
            return *this;
        }

        inline constexpr bit_field_member& operator^=(const _Type& _value) noexcept
        {
            data_.set_value(data_.get_value() ^ _value);
            return *this;
        }

        inline constexpr bit_field_member& operator<<=(const _Type& _value) noexcept
        {
            data_.set_value(data_.get_value() << _value);
            return *this;
        }

        inline constexpr bit_field_member& operator>>=(const _Type& _value) noexcept
        {
            data_.set_value(data_.get_value() >> _value);
            return *this;
        }

        inline constexpr void reset(uint_type _value = 0) noexcept
        {
            data_.set_value(_value);
        }

    private:
        data_type data_;
    };

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr _Type operator+(
        const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value + _member.get_value();
    }

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr _Type operator-(
        const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value - _member.get_value();
    }

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr _Type operator*(
        const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value * _member.get_value();
    }

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr _Type operator/(
        const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value / _member.get_value();
    }

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr _Type operator%(
        const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value % _member.get_value();
    }

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr _Type operator&(
        const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value & _member.get_value();
    }

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr _Type operator|(
        const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value | _member.get_value();
    }

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr _Type operator^(
        const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value ^ _member.get_value();
    }

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr _Type operator<<(
        const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value << _member.get_value();
    }

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr _Type operator>>(
        const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value >> _member.get_value();
    }

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr bool operator==(
        const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value == _member.get_value();
    }

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr bool operator!=(
        const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value != _member.get_value();
    }

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr bool operator<(
        const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value < _member.get_value();
    }

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr bool operator>(
        const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value > _member.get_value();
    }

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr bool operator<=(
        const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value <= _member.get_value();
    }

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr bool operator>=(
        const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value >= _member.get_value();
    }

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr bool operator|=(const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value | _member.get_value();
    }

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr bool operator&=(const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value & _member.get_value();
    }

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr bool operator^=(const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value ^ _member.get_value();
    }

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr bool operator<<=(const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value << _member.get_value();
    }

    template<size_t _BeginBit, size_t _BitSize, endian_t _Endian, typename _Type>
    inline constexpr bool operator>>=(const _Type& _value, const bit_field_member<_BeginBit, _BitSize, _Endian, _Type>& _member) noexcept
    {
        return _value >> _member.get_value();
    }

}    
}

#endif // !MEGOPP_ENDIAN_BIT_FIELD_MEMBER_H_INCLUDED
