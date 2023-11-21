
#ifndef MEGOPP_ENDIAN_ARITHMETIC_MEMBER_H_INCLUDED
#define MEGOPP_ENDIAN_ARITHMETIC_MEMBER_H_INCLUDED

#include "types.h"
#include "byte_swap.h"

namespace megopp {
namespace endian {
 
    template<typename _Ty, endian_t _Endian>
    struct arithmetic_member_private
    {
        using type = _Ty;

        arithmetic_member_private() noexcept {}
        arithmetic_member_private(type _value) noexcept {}
        //arithmetic_member_private(const arithmetic_member_private&) noexcept {}

        //inline arithmetic_member_private& operator=(const arithmetic_member_private&) noexcept { return *this; }

        inline constexpr type get_value() const noexcept
        {
            return 0;
        }

        inline constexpr void set_value(type _value) noexcept
        {
        }

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

        arithmetic_member_private() noexcept :
            data_(0)
        {
        }

        arithmetic_member_private(const type& _value) noexcept :
            data_()
        {
        }

        //arithmetic_member_private(const arithmetic_member_private& _other) noexcept :
        //    data_(_other.data_)
        //{
        //}

        //inline arithmetic_member_private& operator=(const arithmetic_member_private& _other) noexcept
        //{
        //    data_ = _other.data_;
        //    return *this;
        //}

        inline constexpr type get_value() const noexcept
        {
            return data_;
        }

        inline constexpr void set_value(const type& _value) noexcept
        {
            data_ = _value;
        }

        type data_;
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
        using uint_type = typename types_by_size<sizeof(_Ty)>::uint;

        arithmetic_member_private() noexcept :
            data_(0)
        {
        }
        
        arithmetic_member_private(const type& _value) noexcept :
            data_(byte_swap(_value))
        {
        }

        //arithmetic_member_private(const arithmetic_member_private& _other) noexcept :
        //    data_(_other.data_)
        //{
        //}
        //inline arithmetic_member_private& operator=(const arithmetic_member_private& _other) noexcept
        //{
        //    data_ = _other.data_;
        //    return *this;
        //}
            
        inline constexpr type get_value() const noexcept
        {
            return byte_swap(data_);
        }

        inline constexpr void set_value(const type& _value) noexcept
        {
            data_ = byte_swap(_value);
        }

        type data_;
    };
#endif

    
    template<typename _Ty, endian_t _Endian>
    struct arithmetic_member
    {
        using type = _Ty;
        
        arithmetic_member() noexcept {}
        arithmetic_member(type _value) noexcept :
            private_(_value)
        {
        }
        
        //arithmetic_member(const arithmetic_member& _other) noexcept :
        //    private_(_other.private_)
        //{
        //}

        //inline arithmetic_member& operator=(const arithmetic_member& _other) noexcept
        //{
        //    private_ = _other.private_;
        //    return *this;
        //}
        
        inline explicit operator type() const noexcept
        {
            return private_.get_value();
        }

        inline explicit operator bool() const noexcept
        {
            return private_.get_value() != 0;
        }

        template<typename _Ty>
        inline constexpr _Ty as() const noexcept
        {
            return static_cast<_Ty>(private_.get_value());
        }
        
        inline constexpr type get_value() const noexcept
        {
            return private_.get_value();
        }

        inline constexpr void set_value(const type& _value) noexcept
        {
            private_.set_value(_value);
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
        
        inline arithmetic_member& operator++() noexcept
        {
            set_value(get_value() + 1);
            return *this;
        }

        inline type operator++(int) noexcept
        {
            type temp = get_value();
            set_value(get_value() + 1);
            return temp;
        }

        inline arithmetic_member& operator--() noexcept
        {
            set_value(get_value() - 1);
            return *this;
        }

        inline type operator--(int) noexcept
        {
            type temp = get_value();
            set_value(get_value() - 1);
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
