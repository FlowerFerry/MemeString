
#ifndef MEGOPP_UTIL_OS_LINUX_GETIFADDRS_H_INCLUDED
#define MEGOPP_UTIL_OS_LINUX_GETIFADDRS_H_INCLUDED

#include <mego/predef/os/linux.h>

#include <megopp/util/scope_cleanup.h>

#if MG_OS__LINUX_AVAIL

#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netdb.h>

#endif 

#include <vector>
#include <type_traits>

namespace mgpp {
namespace os {
namespace linux {

#if MG_OS__LINUX_AVAIL

template <typename _Fn>
inline mgpp::err getifaddrs(_Fn&& _fn)
{
    static_assert(std::is_invocable_v<_Fn, const ifaddrs*>, "Invalid function type");

    ifaddrs* ifaddr = nullptr;
    if (getifaddrs(&ifaddr) == -1)
        return mgpp::err{ mgec__from_sys_err(errno) };
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { freeifaddrs(ifaddr); });

    for (auto index = ifaddr; index; index = index->ifa_next)
    {
        if constexpr (std::is_same_v<std::invoke_result_t<_Fn, const ifaddrs*>, bool>)
        {
            if (!_fn(index))
                break;
        }
        else {
            _fn(index);
        }
    }

    return mgpp::err{};
}

#endif

}; // namespace linux
}; // namespace os
}; // namespace mgpp

#endif // !MEGOPP_UTIL_OS_LINUX_GETIFADDRS_H_INCLUDED
