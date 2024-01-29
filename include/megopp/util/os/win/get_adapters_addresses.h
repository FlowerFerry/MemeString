
#ifndef MEGOPP_UTIL_OS_WIN_GET_ADAPTERS_ADDRESSES_H_INCLUDED
#define MEGOPP_UTIL_OS_WIN_GET_ADAPTERS_ADDRESSES_H_INCLUDED

#include <mego/predef/os/windows.h>

#include <megopp/err/err.h>
#include <mego/err/ec_impl.h>

#if MG_OS__WIN_AVAIL

#include <iphlpapi.h>
#pragma comment(lib, "Iphlpapi.lib")

#endif

#include <vector>
#include <type_traits>

namespace mgpp {
namespace os {
namespace win {

#if MG_OS__WIN_AVAIL

template <typename _Fn>
inline mgpp::err get_adapters_addresses(_Fn&& _fn)
{
    static_assert(std::is_invocable_v<_Fn, const IP_ADAPTER_ADDRESSES*>, "Invalid function type");

    ULONG len = 1024 * 12;
    std::vector<uint8_t> buf; buf.resize(len);

    DWORD ret = 0;
    int iterations = 0;
    do {
        IP_ADAPTER_ADDRESSES *p = (IP_ADAPTER_ADDRESSES*)buf.data();
        ret = GetAdaptersAddresses(AF_UNSPEC, 0, NULL, p, (ULONG*)&len);
        if (ret == ERROR_BUFFER_OVERFLOW) {
            buf.resize(len);
        }
        else if (ret != ERROR_SUCCESS) {
            return mgpp::err{ mgec__from_sys_err(ret) };
        }
    } while (ret == ERROR_BUFFER_OVERFLOW && iterations++ < 3);

    if (ret != ERROR_SUCCESS)
        return mgpp::err{ mgec__from_sys_err(ret) };

    IP_ADAPTER_ADDRESSES *p = (IP_ADAPTER_ADDRESSES*)buf.data();

    for (auto index = p; index; index = index->Next) 
    {
        if constexpr (std::is_same_v<std::invoke_result_t<_Fn, const IP_ADAPTER_ADDRESSES*>, bool>) 
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

}}}

#endif // !MEGOPP_UTIL_OS_WIN_GET_ADAPTERS_ADDRESSES_H_INCLUDED
