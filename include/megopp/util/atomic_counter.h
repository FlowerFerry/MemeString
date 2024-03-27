
#ifndef MEGOPP_UTIL_ATOMIC_COUNTER_H_INCLUDED
#define MEGOPP_UTIL_ATOMIC_COUNTER_H_INCLUDED

#include <atomic>
#include <limits>  

namespace mgpp {
namespace util {

template <typename _Ty>
struct atomic_counter
{
    constexpr atomic_counter(_Ty _minVal = std::numeric_limits<_Ty>::min(), _Ty _maxVal = std::numeric_limits<_Ty>::max()) noexcept
        : val_(0), minVal_(_minVal), maxVal_(_maxVal)
    {}

    constexpr atomic_counter(_Ty _val, _Ty _minVal = std::numeric_limits<_Ty>::min(), _Ty _maxVal = std::numeric_limits<_Ty>::max()) noexcept
        : val_(_val), minVal_(_minVal), maxVal_(_maxVal)
    {
        if (_val < minVal_ || _val > maxVal_)
        {
            val_.store(0, std::memory_order_relaxed);
        }
    }

    inline atomic_counter& operator++() noexcept
    {
        get_and_inc(1);
        return *this;
    }

    inline atomic_counter& operator--() noexcept
    {
        get_and_dec(1);
        return *this;
    }

    inline _Ty operator++(int) noexcept
    {
        return get_and_inc(1);
    }

    inline _Ty operator--(int) noexcept
    {
        return get_and_dec(1);
    }

    inline atomic_counter& operator+=(_Ty _val) noexcept
    {
        get_and_inc(_val);
        return *this;
    }

    inline atomic_counter& operator-=(_Ty _val) noexcept
    {
        get_and_dec(_val);
        return *this;
    }

    inline bool operator==(const atomic_counter& _other) const noexcept
    {
        return val_.load(std::memory_order_relaxed) == _other.val_.load(std::memory_order_relaxed);
    }

    inline bool operator!=(const atomic_counter& _other) const noexcept
    {
        return !(*this == _other);
    }

    inline _Ty operator _Ty() const noexcept
    {
        return val_.load(std::memory_order_relaxed);
    }

    inline _Ty get_and_inc(_Ty _inc) noexcept
    {
        _Ty curr = val_.load(std::memory_order_relaxed);
        _Ty next = (curr + _inc > maxVal_) ? minVal_ : (curr + _inc < minVal_) ? maxVal_ : curr + _inc;
        while (!val_.compare_exchange_weak(curr, next, std::memory_order_release, std::memory_order_relaxed))
        {
            next = (curr + _inc > maxVal_) ? minVal_ : (curr + _inc < minVal_) ? maxVal_ : curr + _inc;
        }
        return curr;
    }

    inline _Ty get_and_dec(_Ty _dec) noexcept
    {
        _Ty curr = val_.load(std::memory_order_relaxed);
        _Ty next = (curr - _dec > maxVal_) ? minVal_ : (curr - _dec < minVal_) ? maxVal_ : curr - _dec;
        while (!val_.compare_exchange_weak(curr, next, std::memory_order_release, std::memory_order_relaxed))
        {
            next = (curr - _dec > maxVal_) ? minVal_ : (curr - _dec < minVal_) ? maxVal_ : curr - _dec;
        }
        return curr;
    }

    inline void reset(_Ty _val = {}) noexcept
    {
        val_.store(_val, std::memory_order_relaxed);
    }

private:

    std::atomic<_Ty> val_;
    const _Ty minVal_;
    const _Ty maxVal_;
};

}
}

#endif // !MEGOPP_UTIL_ATOMIC_COUNTER_H_INCLUDED
