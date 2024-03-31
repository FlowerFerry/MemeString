
#ifndef MEGOPP_UTIL_OS_WIN_ENUM_LOGICAL_DRIVES_H_INCLUDED
#define MEGOPP_UTIL_OS_WIN_ENUM_LOGICAL_DRIVES_H_INCLUDED

#include <mego/predef/os/windows.h>
#include <mego/err/ec_impl.h>
#include <megopp/err/err.h>

#include <vector>
#include <type_traits>

#include <mego/util/os/windows/windows_simplify.h>

namespace mgpp {
namespace os {
namespace win {
    
#if MG_OS__WIN_AVAIL

    template<typename _Fn>
    inline mgec_t enum_logical_drives_u16(_Fn&& _fn)
    {
        DWORD dwSize = GetLogicalDriveStringsW(0, NULL);
        if (dwSize == 0) {
            return mgec__from_sys_err(GetLastError());
        }

        std::vector<wchar_t> drives;
        drives.resize(dwSize + 1, 0);
        dwSize = GetLogicalDriveStringsW(dwSize, drives.data());
        if (dwSize == 0) {
            return mgec__from_sys_err(GetLastError());
        }
        
        for (wchar_t* p = drives.data(); *p != 0; p += wcslen(p) + 1) 
        {
            if constexpr (std::is_same_v<std::invoke_result_t<_Fn, const wchar_t*>, bool>) 
            {
                if (!_fn(p))
                    break;
            }
            else {
                _fn(p);
            }
        }
        
        return 0;
    }
    
    template<typename _Fn>
    inline mgec_t enum_removable_drives_u16(_Fn&& _fn)
    {
        DWORD dwSize = GetLogicalDriveStringsW(0, NULL);
        if (dwSize == 0) {
            return mgec__from_sys_err(GetLastError());
        }

        std::vector<wchar_t> drives;
        drives.resize(dwSize + 1, 0);
        dwSize = GetLogicalDriveStringsW(dwSize, drives.data());
        if (dwSize == 0) {
            return mgec__from_sys_err(GetLastError());
        }
        
        for (wchar_t* p = drives.data(); *p != 0; p += wcslen(p) + 1) 
        {
            UINT type = GetDriveTypeW(p);
            if (type != DRIVE_REMOVABLE)
                continue;
            
            if constexpr (std::is_same_v<std::invoke_result_t<_Fn, const wchar_t*>, bool>) 
            {
                if (!_fn(p))
                    break;
            }
            else {
                _fn(p);
            }
        }
        
        return 0;
    }
    
#endif
}
}
}

#endif // !MEGOPP_UTIL_OS_WIN_ENUM_LOGICAL_DRIVES_H_INCLUDED
