
#ifndef MEGOPP_NETIF_PHYSICAL_ADDRESS_H_INCLUDED
#define MEGOPP_NETIF_PHYSICAL_ADDRESS_H_INCLUDED

#include <memepp/string.hpp>
#include <megopp/err/err.h>

namespace mgpp {
namespace netif {

struct physical_address
{
    physical_address()
        : len_(0)
    {
        memset(data_, 0, sizeof(data_));
    }

    inline memepp::string to_string() const
    {
        static const char xdigits[] = "0123456789ABCDEF";

        char buf[sizeof(data_) * 3] = { 0 };
        auto len = (std::min)(len_, sizeof(data_));
        for (uint8_t i = 0; i < len_; ++i)
        {
            if (i != 0)
                buf[i * 3 - 1] = '-';
            
            buf[i * 3 + 0] = xdigits[data_[i] / 16];
            buf[i * 3 + 1] = xdigits[data_[i] % 16];
        }
        return { buf, length_ * 3 };
    }

    inline void assign(const uint8_t* _data, uint8_t _len)
    {
        len_ = (std::min)(_len, sizeof(data_));
        memcpy(data_, _data, len_);
    }

    static physical_address from_string(const memepp::string& _s) noexcept;

    uint8_t data_[8];
    uint8_t len_;
};

using phy_addr = physical_address;

struct phy_addr_info
{

    phy_addr_info()
        : ifindex_(-1)
    {
    }

    inline const memepp::string& ifname() const noexcept
    {
        return ifname_;
    }

    inline const phy_addr& phyaddr() const noexcept
    {
        return phy_addr_;
    }

    inline int ifindex() const noexcept
    {
        return ifindex_;
    }

    int ifindex_;
    memepp::string ifname_;
    phy_addr phy_addr_;
};

}
}

#endif // !MEGOPP_NETIF_PHYSICAL_ADDRESS_H_INCLUDED
