
#ifndef MEGOPP_UTIL_OS_WIN_NET_RESOURCE_H_INCLUDED
#define MEGOPP_UTIL_OS_WIN_NET_RESOURCE_H_INCLUDED

// #include <mego/util/os/windows/windows_simplify.h>
#include <mego/err/ec_impl.h>

#include <megopp/util/scope_cleanup.h>
#include <megopp/err/err.h>
#include <vector>
#include <type_traits>

#include <mego/predef/os/windows.h>
#if MG_OS__WIN_AVAIL
#  include <winnetwk.h>
#  pragma comment(lib, "mpr.lib")
#endif

namespace mgpp {
namespace os {
namespace win {

#if MG_OS__WIN_AVAIL

template <typename _Fn>
inline void enum_net_resources_u16(DWORD _scope, DWORD _type, DWORD _usage, _Fn && _fn, mgpp::err& _err)
{
    HANDLE hEnum = NULL;
    DWORD dwResult = WNetOpenEnumW(_scope, _type, _usage, NULL, &hEnum);
    if (dwResult != NO_ERROR) {
        _err = mgpp::err(mgec__from_sys_err(dwResult), "WNetOpenEnum");
        return;
    }
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&] { WNetCloseEnum(hEnum); });

    DWORD cbBuffer = 16384;
    DWORD cEntries = -1;
    std::vector<uint8_t> resources;
    resources.resize(cbBuffer, 0);
    do {
        ZeroMemory(resources.data(), cbBuffer);
        dwResult = WNetEnumResourceW(hEnum, &cEntries, resources.data(), &cbBuffer);
        if (dwResult != NO_ERROR) {
            break;
        }
        if (cbBuffer > resources.size()) 
        {
            resources.resize(cbBuffer, 0);
            continue;
        }

        const NETRESOURCEW* pnr = reinterpret_cast<NETRESOURCEW*>(resources.data());
        for (DWORD idx = 0; idx < cEntries; ++idx) 
        {        
            if constexpr (std::is_same_v<std::invoke_result_t<_Fn, const NETRESOURCEW*>, bool>) 
            {
                if (!_fn(pnr + idx))
                    break;
            }
            else {
                _fn(pnr + idx);
            }
        }

    } while (dwResult != ERROR_NO_MORE_ITEMS);

    if (dwResult != ERROR_NO_MORE_ITEMS) {
        _err = mgpp::err(mgec__from_sys_err(dwResult), "WNetEnumResource");
        return;
    }

    _err = mgpp::err{};
    return;
}

template <typename _Fn>
inline void enum_net_resources_u16(DWORD _scope, DWORD _type, _Fn&& _fn, mgpp::err& _err)
{
    enum_net_resources_u16(_scope, _type, 0, std::forward<_Fn>(_fn), _err);
}

#endif

}
}
}

#endif // !MEGOPP_UTIL_OS_WIN_NET_RESOURCE_H_INCLUDED
