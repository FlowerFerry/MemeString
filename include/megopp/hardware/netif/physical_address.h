
#ifndef MGPP_HW_NETIF_PHYSICAL_ADDRESS_H_INCLUDED
#define MGPP_HW_NETIF_PHYSICAL_ADDRESS_H_INCLUDED

#include <mego/predef/os/windows.h>
#include <mego/predef/os/linux.h>

#include <megopp/netif/physical_address.h>
#include <megopp/util/os/win/get_adapters_addresses.h>
#include <megopp/util/os/linux/getifaddrs.h>

#if MG_OS__WIN_AVAIL

#include <iphlpapi.h>
#pragma comment(lib, "Iphlpapi.lib")

#endif 

#if MG_OS__LINUX_AVAIL

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>

#endif

#include <map>
#include <vector>

namespace mgpp {
namespace hw {
namespace netif {


inline mgpp::err get_phy_addr_infos(std::map<int, mgpp::netif::phy_addr_info>& _infos)
{
    _infos.clear();
#if MG_OS__WIN_AVAIL
    auto err = mgpp::os::win::get_adapters_addresses([&](const IP_ADAPTER_ADDRESSES* _addr) 
    {
        mgpp::netif::phy_addr_info info;

        info.ifindex_ = _addr->IfIndex;
        info.ifname_  = _addr->AdapterName;
        info.phy_addr_.assign(_addr->PhysicalAddress, _addr->PhysicalAddressLength);

        _infos[info.ifindex_] = info;
        return true;
    });

    return err;
#elif MG_OS__LINUX_AVAIL
    auto err = mgpp::os::linux::getifaddrs([&](const ifaddrs* _addr)
    {
        if (!_addr->ifa_name)
            return true;

        if (!_addr->ifa_addr)
            return true;

        if (_addr->ifa_addr->sa_family == AF_PACKET)
        {
            mgpp::netif::phy_addr_info info;

            info.ifindex_ = if_nametoindex(_addr->ifa_name);
            info.ifname_  = _addr->ifa_name;
            info.phy_addr_.assign((const uint8_t*)_addr->ifa_addr->sa_data, 6);

            _infos[info.ifindex_] = info;
        }
        return true;
    });

    return err;
#else
#   error "mgpp::hw::netif::get_phy_addr_infos(...) is not implemented for this platform"
    return mgpp::err{ MGEC__ERR };
#endif    
}


}; // namespace netif
}; // namespace hw
}

#endif // !MGPP_HW_NETIF_PHYSICAL_ADDRESS_H_INCLUDED
