
#ifndef MEME_VARTS_DEF_H_INCLUDED
#define MEME_VARTS_DEF_H_INCLUDED

#include <meme/varts_fwd.h>
#include <mego/predef/compiler/visualc.h>

#include <memepp/varts_fwd.hpp>
#include <memepp/variant_def.hpp>

namespace memepp {
    
    class varts
    {
    public:
        varts() noexcept
        {}
        
        varts(const varts& _other)
            : ts_ud_(_other.ts_ud_), var_(_other.var_)
        {}

        varts(varts&& _other) noexcept
            : ts_ud_(_other.ts_ud_), var_(std::move(_other.var_))
        {}

        varts(const mmvtsstk_t& _vts)
            : var_(_vts.var)
        {
            ts_ud_.ts_ = _vts.ts;
            ts_ud_.ud_ = _vts.userdata;
        }

        varts(mmvtsstk_t&& _vts) noexcept
            : var_(std::move(_vts.var))
        {
            ts_ud_.ts_ = _vts.ts;
            ts_ud_.ud_ = _vts.userdata;
        }

        varts(const variant& _var)
            : var_(_var)
        {}

        varts(variant&& _var) noexcept
            : var_(std::move(_var))
        {}

        varts(const variant& _var, mgu_timestamp_t _ts)
            : var_(_var)
        {
            ts_ud_.ud_ = 0;
            if (_ts == -1)
                ts_ud_.ts_ = MMVTS__TS_INVALID;
            else
                ts_ud_.ts_ = _ts;
        }

        varts(variant&& _var, mgu_timestamp_t _ts)
            : var_(std::move(_var))
        {
            ts_ud_.ud_ = 0;
            if (_ts == -1)
                ts_ud_.ts_ = MMVTS__TS_INVALID;
            else
                ts_ud_.ts_ = _ts;
        }
        
        varts(const variant& _var, mgu_timestamp_t _ts, int8_t _ud)
            : var_(_var)
        {
            ts_ud_.ud_ = _ud;
            if (_ts == -1)
                ts_ud_.ts_ = MMVTS__TS_INVALID;
            else
                ts_ud_.ts_ = _ts;
        }

        varts(variant&& _var, mgu_timestamp_t _ts, int8_t _ud) noexcept
            : var_(std::move(_var))
        {
            ts_ud_.ud_ = _ud;
            if (_ts == -1)
                ts_ud_.ts_ = MMVTS__TS_INVALID;
            else
                ts_ud_.ts_ = _ts;
        }

        varts(mgu_timestamp_t _ts)
        {
            ts_ud_.ud_ = 0;
            if (_ts == -1)
                ts_ud_.ts_ = MMVTS__TS_INVALID;
            else
                ts_ud_.ts_ = _ts;
        }

        varts(mgu_timestamp_t _ts, int8_t _ud)
        {
            ts_ud_.ud_ = _ud;
            if (_ts == -1)
                ts_ud_.ts_ = MMVTS__TS_INVALID;
            else
                ts_ud_.ts_ = _ts;
        }

        varts& operator=(const varts& _other)
        {
            if (this == &_other)
                return *this;
            
            ts_ud_ = _other.ts_ud_;
            var_ = _other.var_;
            return *this;
        }

        varts& operator=(varts&& _other) noexcept
        {
            if (this == &_other)
                return *this;

            ts_ud_ = _other.ts_ud_;
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
            if (ts_ud_.ts_ == MMVTS__TS_INVALID)
                return -1;
            return (mgu_timestamp_t)ts_ud_.ts_;
        }

        mgu_time_t time_part() const noexcept
        {
            if (ts_ud_.ts_ == MMVTS__TS_INVALID)
                return -1;
            return mgu_timestamp_to_time((mgu_timestamp_t)ts_ud_.ts_);
        }
        
        int ms_part() const noexcept
        {
            if (ts_ud_.ts_ == MMVTS__TS_INVALID)
                return -1;
            return mgu_timestamp_get_ms((mgu_timestamp_t)ts_ud_.ts_);
        }

        constexpr int8_t userdata() const noexcept
        {
            return (int8_t)ts_ud_.ud_;
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
            return ts_ud_.ts_ != MMVTS__TS_INVALID;
        }

        void set_timestamp(mgu_timestamp_t _ts) noexcept
        {
            if (_ts == -1)
                ts_ud_.ts_ = MMVTS__TS_INVALID;
            else
                ts_ud_.ts_ = (uint64_t)_ts;
        }

        void set_userdata(int8_t _ud) noexcept
        {
            ts_ud_.ud_ = (uint8_t)_ud;
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
            ts_ud_.ts_ = MMVTS__TS_INVALID;
            ts_ud_.ud_ = 0;
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
#if MEGO_COMP__MSVC__AVAILABLE
            struct TsUd {
#else
            struct {
#endif

                uint64_t ts_ : (CHAR_BIT * 7); // unix timestamp
                uint64_t ud_ : (CHAR_BIT * 1); // user data
            } ts_ud_;
            uint64_t ts_ud_value_;
        };
        variant var_;
    };

}

static_assert(sizeof(memepp::varts) == sizeof(mmvtsstk_t), "memepp::varts size mismatch");

#include <memepp/varts_tmpimpl.hpp>

#endif // !MEME_VARTS_DEF_H_INCLUDED
