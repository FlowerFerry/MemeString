
#ifndef MEGOPP_ENDIAN_ARITHMETIC_MEMBER_H_INCLUDED
#define MEGOPP_ENDIAN_ARITHMETIC_MEMBER_H_INCLUDED

#include "types.h"
#include "byte_swap.h"

#include <megopp/predef/namespace_alias.h>

#include <cmath>
#include <cstring>
#include <type_traits>

namespace mgpp {
namespace endian {
 
    template<typename _Ty, endian_t _Endian>
    struct arithmetic_member_private
    {
        using type = _Ty;

        inline type get_value() const noexcept
        {
            return 0;
        }

        inline void set_value(type _value) noexcept
        {
        }

        uint8_t data_[sizeof(_Ty)];
    };

    template<typename _Ty>
    struct arithmetic_member_private < _Ty, 
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
        using type = _Ty;
        using uint_type = typename type_by_size<sizeof(type)>::uint;

        inline type get_value() const noexcept
        {
            if constexpr (std::is_integral_v<type> || std::is_enum_v<type>) {
                uint_type val = 0;
#if MEGO_ENDIAN__LITTLE_BYTE || MEGO_ENDIAN__LITTLE_WORD
                for (size_t i = 0; i < sizeof(type); ++i) {
                    val |= static_cast<uint_type>(data_[i]) << (i * 8);
                }
#else
                for (size_t i = 0; i < sizeof(type); ++i) {
                    val |= static_cast<uint_type>(data_[i]) << ((sizeof(type) - 1 - i) * 8);
                }
#endif
                return static_cast<type>(val);
            }
            else {
                type val{};
                std::memcpy(&val, data_, sizeof(type));
                return val;
            }
        }

        inline void set_value(const type& _value) noexcept
        {
            if constexpr (std::is_integral_v<type> || std::is_enum_v<type>) {
                uint_type val = static_cast<uint_type>(_value);
#if MEGO_ENDIAN__LITTLE_BYTE || MEGO_ENDIAN__LITTLE_WORD
                for (size_t i = 0; i < sizeof(type); ++i) {
                    data_[i] = static_cast<uint8_t>((val >> (i * 8)) & 0xFF);
                }
#else
                for (size_t i = 0; i < sizeof(type); ++i) {
                    data_[i] = static_cast<uint8_t>((val >> ((sizeof(type) - 1 - i) * 8)) & 0xFF);
                }
#endif
            }
            else {
                std::memcpy(data_, &_value, sizeof(type));
            }
        }

        //! The implementation has now been switched to use `uint8_t[]`. 
        //! Previously, integer types were used in conjunction with pointer type-casting to attempt to read data from an unaligned address—a practice prone to triggering bus errors.
        uint8_t data_[sizeof(type)];
    };

#if MEGO_ENDIAN__LITTLE_BYTE || MEGO_ENDIAN__BIG_BYTE
    template<typename _Ty>
    struct arithmetic_member_private < _Ty,
#if MEGO_ENDIAN__LITTLE_BYTE
        endian_t::big_byte
#else
        endian_t::little_byte
#endif
    >
    {
        using type = _Ty;
        using uint_type = typename type_by_size<sizeof(type)>::uint;
        
    private:
        inline constexpr type _load() const noexcept {
            if constexpr (std::is_integral_v<type> || std::is_enum_v<type>) {
                uint_type val = 0;
#if MEGO_ENDIAN__LITTLE_BYTE || MEGO_ENDIAN__LITTLE_WORD
                for (size_t i = 0; i < sizeof(type); ++i) {
                    val |= static_cast<uint_type>(data_[i]) << (i * 8);
                }
#else
                for (size_t i = 0; i < sizeof(type); ++i) {
                    val |= static_cast<uint_type>(data_[i]) << ((sizeof(type) - 1 - i) * 8);
                }
#endif
                return static_cast<type>(val);
            }
            else {
                type val{};
                std::memcpy(&val, data_, sizeof(type));
                return val;
            }
        }

        inline constexpr void _store(const type& _value) noexcept {
            if constexpr (std::is_integral_v<type> || std::is_enum_v<type>) {
                uint_type val = static_cast<uint_type>(_value);
#if MEGO_ENDIAN__LITTLE_BYTE || MEGO_ENDIAN__LITTLE_WORD
                for (size_t i = 0; i < sizeof(type); ++i) {
                    data_[i] = static_cast<uint8_t>((val >> (i * 8)) & 0xFF);
                }
#else
                for (size_t i = 0; i < sizeof(type); ++i) {
                    data_[i] = static_cast<uint8_t>((val >> ((sizeof(type) - 1 - i) * 8)) & 0xFF);
                }
#endif
            }
            else {
                std::memcpy(data_, &_value, sizeof(type));
            }
        }

    public:
        inline type get_value() const noexcept
        {
            return byte_swap(_load());
        }

        inline void set_value(const type& _value) noexcept
        {
            _store(byte_swap(_value));
        }

        uint8_t data_[sizeof(type)];
    };
#else
    #error "not support"
#endif

    
    template<typename _Ty, endian_t _Endian,
        typename = typename std::enable_if<
            std::is_arithmetic<_Ty>::value || std::is_enum<_Ty>::value>::type>
    struct arithmetic_member
    {
        using type = _Ty;
        
        // arithmetic_member() noexcept {}
        // arithmetic_member(type _value) noexcept :
        //     private_(_value)
        // {
        // }
        
        //arithmetic_member(const arithmetic_member& _other) noexcept :
        //    private_(_other.private_)
        //{
        //}

        inline explicit operator type() const noexcept
        {
            return private_.get_value();
        }

        inline explicit operator bool() const noexcept
        {
            return private_.get_value() != 0;
        }

        template<typename _Result>
        inline _Result as() const noexcept
        {
            return static_cast<_Result>(private_.get_value());
        }
        
        inline type get_value() const noexcept
        {
            return private_.get_value();
        }

        inline void set_value(const type& _value) noexcept
        {
            private_.set_value(_value);
        }

        inline arithmetic_member& operator=(const type& _value) noexcept
        {
            set_value(_value);
            return *this;
        }

        inline arithmetic_member& operator+=(const type& _value) noexcept
        {
            set_value(get_value() + _value);
            return *this;
        }

        inline arithmetic_member& operator-=(const type& _value) noexcept
        {
            set_value(get_value() - _value);
            return *this;
        }

        inline arithmetic_member& operator*=(const type& _value) noexcept
        {
            set_value(get_value() * _value);
            return *this;
        }

        inline arithmetic_member& operator/=(const type& _value) noexcept
        {
            set_value(get_value() / _value);
            return *this;
        }
        
        template<typename = typename std::enable_if<!std::is_floating_point<type>::value>::type>
        inline arithmetic_member& operator&=(const type& _value) noexcept
        {
            set_value(get_value() & _value);
            return *this;
        }

        template<typename = typename std::enable_if<!std::is_floating_point<type>::value>::type>
        inline arithmetic_member& operator|=(const type& _value) noexcept
        {
            set_value(get_value() | _value);
            return *this;
        }

        template<typename = typename std::enable_if<!std::is_floating_point<type>::value>::type>
        inline arithmetic_member& operator^=(const type& _value) noexcept
        {
            set_value(get_value() ^ _value);
            return *this;
        }

        template<typename = typename std::enable_if<!std::is_floating_point<type>::value>::type>
        inline arithmetic_member& operator<<=(const type& _value) noexcept
        {
            set_value(get_value() << _value);
            return *this;
        }

        template<typename = typename std::enable_if<!std::is_floating_point<type>::value>::type>
        inline arithmetic_member& operator>>=(const type& _value) noexcept
        {
            set_value(get_value() >> _value);
            return *this;
        }

        inline arithmetic_member& operator%=(const type& _value) noexcept
        {
            set_value(fmod(_value, std::is_floating_point<type>()));
            return *this;
        }

        inline arithmetic_member& operator++() noexcept
        {
            set_value(get_value() + 1);
            return *this;
        }

        inline arithmetic_member operator++(int) noexcept
        {
            arithmetic_member temp = *this;
            ++*this;
            return temp;
        }

        inline arithmetic_member& operator--() noexcept
        {
            set_value(get_value() - 1);
            return *this;
        }

        inline arithmetic_member operator--(int) noexcept
        {
            arithmetic_member temp = *this;
            --*this;
            return temp;
        }
        
        inline type operator+(const type& _value) const noexcept
        {
            return get_value() + _value;
        }
        
        inline type operator-(const type& _value) const noexcept
        {
            return get_value() - _value;
        }

        inline type operator*(const type& _value) const noexcept
        {
            return get_value() * _value;
        }

        inline type operator/(const type& _value) const noexcept
        {
            return get_value() / _value;
        }

        inline type operator%(const type& _value) const noexcept
        {
            return fmod(_value, std::is_floating_point<type>());
        }

        template<typename = typename std::enable_if<!std::is_floating_point<type>::value>::type>
        inline type operator&(const type& _value) const noexcept
        {
            return get_value() & _value;
        }

        template<typename = typename std::enable_if<!std::is_floating_point<type>::value>::type>
        inline type operator|(const type& _value) const noexcept
        {
            return get_value() | _value;
        }

        inline type operator^(const type& _value) const noexcept
        {
            return get_value() ^ _value;
        }

        inline type operator~() const noexcept
        {
            return ~get_value();
        }

        inline type operator<<(const type& _value) noexcept
        {
            return get_value() << _value;
        }

        inline type operator>>(const type& _value) noexcept
        {
            return get_value() >> _value;
        }

        inline bool operator==(const type& _value) const noexcept
        {
            return get_value() == _value;
        }

        inline bool operator!=(const type& _value) const noexcept
        {
            return get_value() != _value;
        }

        inline bool operator<(const type& _value) const noexcept
        {
            return get_value() < _value;
        }

        inline bool operator>(const type& _value) const noexcept
        {
            return get_value() > _value;
        }

        inline bool operator<=(const type& _value) const noexcept
        {
            return get_value() <= _value;
        }

        inline bool operator>=(const type& _value) const noexcept
        {
            return get_value() >= _value;
        }

        template<typename _Ty2, endian_t _Endian2>
        inline bool operator==(const arithmetic_member<_Ty2, _Endian2>& _other) const noexcept
        {
            return get_value() == _other.get_value();
        }

        template<typename _Ty2, endian_t _Endian2>
        inline bool operator!=(const arithmetic_member<_Ty2, _Endian2>& _other) const noexcept
        {
            return get_value() != _other.get_value();
        }

    protected:
        inline type fmod(const type& _value, std::false_type) const noexcept
        {
            return get_value() % _value;
        }

        inline type fmod(const type& _value, std::true_type) const noexcept
        {
            return std::fmod(get_value(), _value);
        }

        arithmetic_member_private<_Ty, _Endian> private_;
    };

    template<typename _Ty, endian_t _Endian>
    inline typename arithmetic_member<_Ty, _Endian>::type operator+(const typename arithmetic_member<_Ty, _Endian>::type& _value1, const arithmetic_member<_Ty, _Endian>& _value2) noexcept
    {
        return _value1 + _value2.get_value();
    }

    template<typename _Ty, endian_t _Endian>
    inline typename arithmetic_member<_Ty, _Endian>::type operator-(const typename arithmetic_member<_Ty, _Endian>::type& _value1, const arithmetic_member<_Ty, _Endian>& _value2) noexcept
    {
        return _value1 - _value2.get_value();
    }

    template<typename _Ty, endian_t _Endian>
    inline typename arithmetic_member<_Ty, _Endian>::type operator*(const typename arithmetic_member<_Ty, _Endian>::type& _value1, const arithmetic_member<_Ty, _Endian>& _value2) noexcept
    {
        return _value1 * _value2.get_value();
    }

    template<typename _Ty, endian_t _Endian>
    inline typename arithmetic_member<_Ty, _Endian>::type operator/(const typename arithmetic_member<_Ty, _Endian>::type& _value1, const arithmetic_member<_Ty, _Endian>& _value2) noexcept
    {
        return _value1 / _value2.get_value();
    }

    template<typename _Ty, endian_t _Endian>
    inline typename arithmetic_member<_Ty, _Endian>::type operator%(const typename arithmetic_member<_Ty, _Endian>::type& _value1, const arithmetic_member<_Ty, _Endian>& _value2) noexcept
    {
        return _value1 % _value2.get_value();
    }

    template<typename _Ty, endian_t _Endian>
    inline typename arithmetic_member<_Ty, _Endian>::type operator&(const typename arithmetic_member<_Ty, _Endian>::type& _value1, const arithmetic_member<_Ty, _Endian>& _value2) noexcept
    {
        return _value1 & _value2.get_value();
    }

    template<typename _Ty, endian_t _Endian>
    inline typename arithmetic_member<_Ty, _Endian>::type operator|(const typename arithmetic_member<_Ty, _Endian>::type& _value1, const arithmetic_member<_Ty, _Endian>& _value2) noexcept
    {
        return _value1 | _value2.get_value();
    }

    template<typename _Ty, endian_t _Endian>
    inline typename arithmetic_member<_Ty, _Endian>::type operator^(const typename arithmetic_member<_Ty, _Endian>::type& _value1, const arithmetic_member<_Ty, _Endian>& _value2) noexcept
    {
        return _value1 ^ _value2.get_value();
    }

    template<typename _Ty, endian_t _Endian>
    inline bool operator==(const typename arithmetic_member<_Ty, _Endian>::type& _value1, const arithmetic_member<_Ty, _Endian>& _value2) noexcept
    {
        return _value1 == _value2.get_value();
    }

    template<typename _Ty, endian_t _Endian>
    inline bool operator!=(const typename arithmetic_member<_Ty, _Endian>::type& _value1, const arithmetic_member<_Ty, _Endian>& _value2) noexcept
    {
        return _value1 != _value2.get_value();
    }

    template<typename _Ty, endian_t _Endian>
    inline bool operator<(const typename arithmetic_member<_Ty, _Endian>::type& _value1, const arithmetic_member<_Ty, _Endian>& _value2) noexcept
    {
        return _value1 < _value2.get_value();
    }

    template<typename _Ty, endian_t _Endian>
    inline bool operator>(const typename arithmetic_member<_Ty, _Endian>::type& _value1, const arithmetic_member<_Ty, _Endian>& _value2) noexcept
    {
        return _value1 > _value2.get_value();
    }

    template<typename _Ty, endian_t _Endian>
    inline bool operator<=(const typename arithmetic_member<_Ty, _Endian>::type& _value1, const arithmetic_member<_Ty, _Endian>& _value2) noexcept
    {
        return _value1 <= _value2.get_value();
    }
    
    template<typename _Ty, endian_t _Endian>
    inline bool operator>=(const typename arithmetic_member<_Ty, _Endian>::type& _value1, const arithmetic_member<_Ty, _Endian>& _value2) noexcept
    {
        return _value1 >= _value2.get_value();
    }
    
}
}

#endif // !MEGOPP_ENDIAN_ARITHMETIC_MEMBER_H_INCLUDED
