
#ifndef MEME_VARTS_DEF_H_INCLUDED
#define MEME_VARTS_DEF_H_INCLUDED

#include <meme/varts_fwd.h>

#include <memepp/varts_fwd.hpp>
#include <memepp/variant_def.hpp>

namespace memepp {
    
    class varts
    {
    public:
        varts() noexcept
            : ts_(MMVTS__TS_INVALID), ud_(0)
        {}
        
        varts(const varts& _other)
            : ts_(_other.ts_), ud_(_other.ud_), var_(_other.var_)
        {}

        varts(varts&& _other) noexcept
            : ts_(_other.ts_), ud_(_other.ud_), var_(std::move(_other.var_))
        {}

        varts(const mmvtsstk_t& _vts)
            : ts_(_vts.ts), ud_(_vts.userdata), var_(_vts.var)
        {}

        varts(mmvtsstk_t&& _vts) noexcept
            : ts_(_vts.ts), ud_(_vts.userdata), var_(std::move(_vts.var))
        {}

        varts(const variant& _var)
            : ts_(MMVTS__TS_INVALID), ud_(0), var_(_var)
        {}

        varts(variant&& _var) noexcept
            : ts_(MMVTS__TS_INVALID), ud_(0), var_(std::move(_var))
        {}

        varts(const variant& _var, mgu_timestamp_t _ts)
            : ts_((uint64_t)_ts), ud_(0), var_(_var)
        {
            if (_ts == -1)
                ts_ = MMVTS__TS_INVALID;
        }

        varts(variant&& _var, mgu_timestamp_t _ts)
            : ts_((uint64_t)_ts), ud_(0), var_(std::move(_var))
        {
            if (_ts == -1)
                ts_ = MMVTS__TS_INVALID;
        }
        
        varts(const variant& _var, mgu_timestamp_t _ts, int8_t _ud)
            : ts_((uint64_t)_ts), ud_((uint8_t)_ud), var_(_var)
        {
            if (_ts == -1)
                ts_ = MMVTS__TS_INVALID;
        }

        varts(variant&& _var, mgu_timestamp_t _ts, int8_t _ud) noexcept
            : ts_((uint64_t)_ts), ud_((uint8_t)_ud), var_(std::move(_var))
        {
            if (_ts == -1)
                ts_ = MMVTS__TS_INVALID;
        }

        varts(mgu_timestamp_t _ts)
            : ts_((uint64_t)_ts), ud_(0)
        {
            if (_ts == -1)
                ts_ = MMVTS__TS_INVALID;
        }

        varts(mgu_timestamp_t _ts, int8_t _ud)
            : ts_((uint64_t)_ts), ud_((uint8_t)_ud)
        {
            if (_ts == -1)
                ts_ = MMVTS__TS_INVALID;
        }

        varts& operator=(const varts& _other)
        {
            if (this == &_other)
                return *this;
            
            ts_   = _other.ts_;
            ud_   = _other.ud_;
            var_ = _other.var_;
            return *this;
        }

        varts& operator=(varts&& _other) noexcept
        {
            if (this == &_other)
                return *this;

            ts_   = _other.ts_;
            ud_   = _other.ud_;
            var_ = std::move(_other.var_);
            return *this;
        }

        varts& operator=(const variant& _var)
        {
            var_ = _var;
            return *this;
        }

        varts& operator=(variant&& _var) noexcept
        {
            var_ = std::move(_var);
            return *this;
        }

        ~varts()
        {}

        constexpr mgu_timestamp_t timestamp() const noexcept
        {
            if (ts_ == MMVTS__TS_INVALID)
                return -1;
            return (mgu_timestamp_t)ts_;
        }

        mgu_time_t time_part() const noexcept
        {
            if (ts_ == MMVTS__TS_INVALID)
                return -1;
            return mgu_timestamp_to_time((mgu_timestamp_t)ts_);
        }
        
        int ms_part() const noexcept
        {
            if (ts_ == MMVTS__TS_INVALID)
                return -1;
            return mgu_timestamp_get_ms((mgu_timestamp_t)ts_);
        }

        constexpr int8_t userdata() const noexcept
        {
            return (int8_t)ud_;
        }

        constexpr const variant& var() const noexcept
        {
            return var_;
        }

        constexpr variant& var() noexcept
        {
            return var_;
        }

        constexpr bool is_ts_valid() const noexcept
        {
            return ts_ != MMVTS__TS_INVALID;
        }

        void set_timestamp(mgu_timestamp_t _ts) noexcept
        {
            if (_ts == -1)
                ts_ = MMVTS__TS_INVALID;
            else
                ts_ = (uint64_t)_ts;
        }

        void set_userdata(int8_t _ud) noexcept
        {
            ud_ = (uint8_t)_ud;
        }

        void set_var(const variant& _var)
        {
            var_ = _var;
        }

        void set_var(variant&& _var)
        {
            var_ = std::move(_var);
        }

        void reset() noexcept
        {
            ts_ = MMVTS__TS_INVALID;
            ud_ = 0;
            var_.reset();
        }

        void swap(varts& _other) noexcept
        {
            std::swap(ts_ud_value_, _other.ts_ud_value_);
            var_.swap(_other.var_);
        }

        static varts make_now()
        {
            return varts{ mgu_timestamp_get() };
        }

        static varts make_now(int8_t _ud)
        {
            return varts{ mgu_timestamp_get(), _ud };
        }

        static varts make_now(const variant& _var)
        {
            return varts{ _var, mgu_timestamp_get() };
        }

        static varts make_now(variant&& _var)
        {
            return varts{ std::move(_var), mgu_timestamp_get() };
        }

        static varts make_now(const variant& _var, int8_t _ud)
        {
            return varts{ _var, mgu_timestamp_get(), _ud };
        }

        static varts make_now(variant&& _var, int8_t _ud)
        {
            return varts{ std::move(_var), mgu_timestamp_get(), _ud };
        }

    private:
        union {
            struct {
                uint64_t ts_ : (CHAR_BIT * 7); // unix timestamp
                uint64_t ud_ : (CHAR_BIT * 1); // user data
            };
            uint64_t ts_ud_value_;
        };
        variant var_;
    };

}

static_assert(sizeof(memepp::varts) == sizeof(mmvtsstk_t), "memepp::varts size mismatch");

#include <memepp/varts_tmpimpl.hpp>

#endif // !MEME_VARTS_DEF_H_INCLUDED
