
#ifndef MEGOPP_UTIL_OS_WIN_GET_VOLUME_SERIAL_NUMBER_H_INCLUDED
#define MEGOPP_UTIL_OS_WIN_GET_VOLUME_SERIAL_NUMBER_H_INCLUDED

#include <mego/err/ec_impl.h>
#include <mego/predef/os/windows.h>
#include <mego/util/os/windows/windows_simplify.h>
#include <memepp/native.hpp>
#include <memepp/convert/std/wstring.hpp>
#include <memepp/string.hpp>
#include <megopp/err/err.h>
#include <megopp/util/scope_cleanup.h>

#if MG_OS__WIN_AVAIL
#  include <winioctl.h>
#  include <cfgmgr32.h>
#  pragma comment(lib, "cfgmgr32.lib")
#endif

namespace mgpp {
namespace os {
namespace win {

#if MG_OS__WIN_AVAIL
    memepp::native_string get_volume_serial_number_u16(const memepp::native_string& _drive_letter, mgpp::err& _err)
    {
        auto pos = _drive_letter.find(L":");

        memepp::native_string volumePath = L"\\\\.\\";
        if (pos != _drive_letter.npos && pos + 1 != _drive_letter.size())
            volumePath += _drive_letter.substr(0, pos + 1);
        else
            volumePath += _drive_letter;
        
        HANDLE hVolume = CreateFileW(
            volumePath.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
        if (hVolume == INVALID_HANDLE_VALUE) {
            _err = mgpp::err(mgec__from_sys_err(GetLastError()), "CreateFile");
            return {};
        }
        MEGOPP_UTIL__ON_SCOPE_CLEANUP([&] { CloseHandle(hVolume); });

        DWORD dwBytesReturned = 0;
        STORAGE_PROPERTY_QUERY storagePropertyQuery;
        char  buffer[1024];

        memset(&storagePropertyQuery, 0, sizeof(storagePropertyQuery));
        storagePropertyQuery.PropertyId = StorageDeviceProperty;
        storagePropertyQuery.QueryType  = PropertyStandardQuery;

        if (DeviceIoControl(hVolume, IOCTL_STORAGE_QUERY_PROPERTY, 
            &storagePropertyQuery, sizeof(storagePropertyQuery), &buffer, sizeof(buffer), &dwBytesReturned, NULL))
        {
            STORAGE_DEVICE_DESCRIPTOR* deviceDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)&buffer;
            const DWORD serialNumberOffset = deviceDescriptor->SerialNumberOffset;
            if (serialNumberOffset != 0) {

                memepp::native_string serialNumberStr = mm_to<memepp::native_string>(mm_view(&buffer[serialNumberOffset], -1));
                _err = mgpp::err{};
                return serialNumberStr;
            }
        }

        _err = mgpp::err(mgec__from_sys_err(GetLastError()), "DeviceIoControl");
        return {};
    }
#endif

}
}
}

#endif // !MEGOPP_UTIL_OS_WIN_GET_VOLUME_SERIAL_NUMBER_H_INCLUDED
