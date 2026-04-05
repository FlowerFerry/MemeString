
#ifndef MEGOPP_UTIL_VALUE_PICKER_H_INCLUDED
#define MMCWPP_UTIL_VALUE_PICKER_H_INCLUDED

#include <mego/err/ec.h>

#include <unordered_set>
#include <type_traits>
#include <algorithm>
#include <vector>
#include <mutex>

#include <megopp/help/null_mutex.h>

namespace mgpp {
namespace util {

template<typename _Ty, typename _Mtx = mgpp::help::null_mutex>
class small_range_value_picker 
{
    static_assert(std::is_integral<_Ty>::value, "small_range_value_picker requires an integral type");
public:
    using dynamic_bitset_t = std::vector<bool>;

    small_range_value_picker(_Ty _first, _Ty _lower, _Ty _upper, _Ty _invalid)
        : lower_(_lower), upper_(_upper), invalid_(_invalid), next_(_first), used_count_(0)
    {
        if (lower_ > upper_) {
            std::swap(lower_, upper_);
        }
        
        used_.resize(__range_size(), false);
        if (lower_ <= invalid_ && invalid_ <= upper_)
        {
            used_[invalid_ - lower_] = true;
            ++used_count_;
        }
        
        next_ = std::clamp(next_, lower_, upper_);
    }

    inline _Ty pick(mgec_t* _result = nullptr) {
        std::lock_guard<_Mtx> lock(mtx_);
        if (used_count_ >= __range_size()) {
            if (_result) {
                *_result = MGEC__RANGE;
            }
            return invalid_;
        }

        for (_Ty index = lower_; index <= upper_; ++index) 
        {
            if (next_ != invalid_ && !used_[next_ - lower_]) {
                auto value = next_;
                next_ = (next_ == upper_) ? lower_ : next_ + 1;
                used_[value - lower_] = true;
                ++used_count_;
                if (_result) {
                    *_result = MGEC__OK;
                }
                return value;
            }
            next_ = (next_ == upper_) ? lower_ : next_ + 1;
        }

        if (_result) {
            *_result = MGEC__RANGE;
        }
        return invalid_;
    }

    inline void release(_Ty _value) {
        std::lock_guard<_Mtx> lock(mtx_);
        if (lower_ <= _value && _value <= upper_ && _value != invalid_)
        {
            used_[_value - lower_] = false;
            --used_count_;
        }
    }


    inline bool is_used(_Ty _value) const {
        std::lock_guard<_Mtx> lock(mtx_);
        if (lower_ <= _value && _value <= upper_) {
            return used_[_value - lower_];
        }
        return false;
    }

    inline bool has_available() const {
        std::lock_guard<_Mtx> lock(mtx_);
        return used_count_ < __range_size();
    }

private:

    inline constexpr _Ty __range_size() const noexcept {
        return upper_ - lower_ + 1;
    }

    mutable _Mtx mtx_;
    _Ty  lower_;
    _Ty  upper_;
    _Ty  invalid_;
    _Ty  next_;
    _Ty  used_count_;
    dynamic_bitset_t used_;
};

template<typename _Ty, typename _Mtx = mgpp::help::null_mutex>
class large_range_value_picker 
{
    static_assert(std::is_integral<_Ty>::value, "large_range_value_picker requires an integral type");

    mutable _Mtx mtx_;
    _Ty  lower_;
    _Ty  upper_;
    _Ty  invalid_;
    _Ty  next_;
    std::unordered_set<_Ty> used_;
    std::unordered_set<_Ty> released_;
    
    inline constexpr _Ty __range_size() const noexcept {
        return upper_ - lower_ + 1;
    }

public:
    large_range_value_picker(_Ty _first, _Ty _lower, _Ty _upper, _Ty _invalid)
        : lower_(_lower), upper_(_upper), invalid_(_invalid), next_(_first) 
    {
        if (lower_ > upper_) {
            std::swap(lower_, upper_);
        }
        
        if (lower_ <= invalid_ && invalid_ <= upper_)
            used_.insert(invalid_);

        next_ = std::clamp(next_, lower_, upper_);
    }

    inline _Ty pick(mgec_t* _result = nullptr) {
        std::lock_guard<_Mtx> lock(mtx_);
        if (used_.size() >= __range_size()) {
            if (_result) {
                *_result = MGEC__RANGE;
            }
            return invalid_;
        }
        
        if (!released_.empty()) {
            auto it = released_.begin();
            auto value = *it;
            released_.erase(it);
            used_.insert(value);
            if (_result) {
                *_result = MGEC__OK;
            }
            return value;
        }

        for (_Ty index = lower_; index <= upper_; ++index) 
        {
            if (next_ != invalid_ && used_.find(next_) == used_.end()) {
                auto value = next_;
                next_ = (next_ == upper_) ? lower_ : next_ + 1;
                used_.insert(value);
                if (_result) {
                    *_result = MGEC__OK;
                }
                return value;
            }
            next_ = (next_ == upper_) ? lower_ : next_ + 1;
        }

        if (_result) {
            *_result = MGEC__RANGE;
        }
        return invalid_;
    }

    inline void release(_Ty _value) {
        std::lock_guard<_Mtx> lock(mtx_);
        if (lower_ <= _value && _value <= upper_ && _value != invalid_) 
        {
            auto it = used_.find(_value);
            if (it != used_.end()) {
                used_.erase(it);
                released_.insert(_value);
            }
        }
    }

    inline bool is_used(_Ty _value) const {
        std::lock_guard<_Mtx> lock(mtx_);
        if (lower_ <= _value && _value <= upper_) {
            return used_.find(_value) != used_.end();
        }
        return false;
    }

    inline bool has_available() const {
        std::lock_guard<_Mtx> lock(mtx_);
        return used_.size() < __range_size();
    }
};

template<typename _Ty, _Ty _Lower, _Ty _Upper, typename _Mtx = mgpp::help::null_mutex>
class value_picker
{
    static_assert(std::is_integral<_Ty>::value, "value_picker requires an integral type");
    
    static constexpr _Ty __lower = (_Lower < _Upper) ? _Lower : _Upper;
    static constexpr _Ty __upper = (_Lower > _Upper) ? _Lower : _Upper;

    static constexpr _Ty __range_size = __upper - __lower + 1;

    using small_t = small_range_value_picker<_Ty, _Mtx>;
    using large_t = large_range_value_picker<_Ty, _Mtx>;

public:
    using impl_t = typename std::conditional<
        __range_size < sizeof(large_t) * 8,
        small_t,
        large_t
    >::type;

    value_picker(_Ty _first, _Ty _invalid)
        : impl_(_first, __lower, __upper, _invalid)
    {
    }

    value_picker(const value_picker&) = delete;
    value_picker& operator=(const value_picker&) = delete;
    value_picker(value_picker&&) = default;
    value_picker& operator=(value_picker&&) = default;

    inline _Ty pick(mgec_t* _result = nullptr) {
        return impl_.pick(_result);
    }

    inline void release(_Ty _value) {
        impl_.release(_value);
    }

    inline bool is_used(_Ty _value) const {
        return impl_.is_used(_value);
    }

    inline bool has_available() const {
        return impl_.has_available();
    }

private:
    impl_t impl_;
};

}
}

#endif // !MMCWPP_UTIL_VALUE_PICKER_H_INCLUDED
