
#ifndef MEGOPP_UTIL_OS_WIN_GET_KNOWN_FOLDER_PATH_H_INCLUDED
#define MEGOPP_UTIL_OS_WIN_GET_KNOWN_FOLDER_PATH_H_INCLUDED

#include <mego/predef/os/windows.h>
#include <mego/err/ec_impl.h>
#include <megopp/err/err.h>
#include <memepp/native.hpp>
#include <memepp/convert/std/wstring.hpp>
#include <megopp/util/scope_cleanup.h>

#include <type_traits>

#include <mego/util/os/windows/windows_simplify.h>

#if MG_OS__WIN_AVAIL
#include <shlobj.h>
#endif

namespace mgpp {
namespace os {
namespace win {

#if MG_OS__WIN_AVAIL
    
    inline memepp::native_string get_known_folder_path_u16(REFKNOWNFOLDERID rfid, mgec_t& _ec)
    {
        wchar_t* szPath = nullptr;
        auto hr = SHGetKnownFolderPath(rfid, 0, NULL, &szPath);
        if (SUCCEEDED(hr)) {
            MEGOPP_UTIL__ON_SCOPE_CLEANUP([&] {
                CoTaskMemFree(szPath);
            });
            memepp::native_string path = szPath;
            _ec = 0;
            return path;
        }
        else {
            _ec = MGEC__ERR;
            return {};
        }
    }

    inline memepp::string get_known_folder_path(REFKNOWNFOLDERID rfid, mgec_t& _ec)
    {
        return mm_from(get_known_folder_path_u16(rfid, _ec));
    }
    
#endif

}
}
}

#endif // !MEGOPP_UTIL_OS_WIN_GET_KNOWN_FOLDER_PATH_H_INCLUDED
