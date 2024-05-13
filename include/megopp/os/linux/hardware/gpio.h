
#ifndef MGPP_OS_LINUX_HARDWARE_GPIO_H_INCLUDED
#define MGPP_OS_LINUX_HARDWARE_GPIO_H_INCLUDED

#include <mego/os/linux/hardware/gpio.h>

namespace mgpp {
namespace linux {
namespace hw {

struct gpio
{
    using number_t = mg_gpio__number_t;

    enum direction_e
    {
        in  = mg_gpio__direction_in,
        out = mg_gpio__direction_out
    };

    gpio()
        : num_(MG_GPIO__NUMBER_INVALID)
        , dir_(in)
    {
    }

    gpio(number_t num, direction_e dir)
        : num_(num)
        , dir_(dir)
    {
    }

    gpio(const gpio&) = delete;
    gpio& operator=(const gpio&) = delete;

    gpio(gpio&& other) noexcept
        : num_(other.num_)
        , dir_(other.dir_)
    {
        other.num_ = MG_GPIO__NUMBER_INVALID;
    }

    ~gpio()
    {
        if (MEGO_SYMBOL__UNLIKELY(num_ != MG_GPIO__NUMBER_INVALID))
            uninit();
    }

    inline gpio& operator=(gpio&& other) noexcept
    {
        if (this != &other) {
            num_ = other.num_;
            dir_ = other.dir_;
            other.num_ = MG_GPIO__NUMBER_INVALID;
        }
        return *this;
    }

    inline gpio& operator=(int value) noexcept
    {
        set_value(value);
        return *this;
    }

    inline mgec_t init() const noexcept
    {
        auto rc = mg_gpio__export(num_);
        if (MEGO_SYMBOL__LIKELY(rc == 0))
            rc = mg_gpio__set_direction(num_, (mg_gpio__direction_e)dir_);
        return rc;
    }

    inline mgec_t uninit() const noexcept
    {
        return mg_gpio__unexport(num_);
    }

    inline mgec_t set_value(int value) const noexcept
    {
        return mg_gpio__set_value(num_, value);
    }

    inline int get_value(mgec_t* _ec) const noexcept
    {
        int value = 0;
        auto rc = mg_gpio__get_value(num_, &value);
        if (_ec)
            *_ec = rc;
        return value;
    }

    inline number_t number() const noexcept
    {
        return num_;
    }

private:
    number_t num_;
    direction_e dir_;
};

}
}
}

#endif // !MGPP_OS_LINUX_HARDWARE_GPIO_H_INCLUDED
