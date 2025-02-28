
#ifndef MEGOPP_UTIL_OS_WIN_ADD_OR_REMOVE_DLL_DIRECTORY_H_INCLUDED
#define MEGOPP_UTIL_OS_WIN_ADD_OR_REMOVE_DLL_DIRECTORY_H_INCLUDED

#include <mego/util/os/windows/windows_simplify.h>

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/convert/std/wstring.hpp>
#include <memepp/convert/std/string.hpp>
#include <memepp/native.hpp>
#include <memepp/hash/std_hash.hpp>

#include <unordered_map>

namespace mgpp {
namespace os {
namespace win {
struct dll_directory_cookies
{
#if MG_OS__WIN_AVAIL
    typedef DLL_DIRECTORY_COOKIE ( *AddDllDirectoryPtr)(PCWSTR);
    typedef BOOL (*RemoveDllDirectoryPtr)(DLL_DIRECTORY_COOKIE);
#else
    using DLL_DIRECTORY_COOKIE = void*;
#endif

#if MG_OS__WIN_AVAIL
#if defined(_WIN32_WINNT) && _WIN32_WINNT >= _WIN32_WINNT_WIN8
    dll_directory_cookies()
        : add_dll_dir_fn_{ ::AddDllDirectory }
        , remove_dll_dir_fn_{ ::RemoveDllDirectory }
    {}
#else
    dll_directory_cookies()
        : kernel32_{ ::LoadLibraryW(L"kernel32.dll") }
    {
        if (kernel32_ == NULL)
            return;

        add_dll_dir_fn_ = (AddDllDirectoryPtr)::GetProcAddress(kernel32_, "AddDllDirectory");
        remove_dll_dir_fn_ = (RemoveDllDirectoryPtr)::GetProcAddress(kernel32_, "RemoveDllDirectory");
    }
#endif
#else
    dll_directory_cookies()
    {}
#endif

    dll_directory_cookies(const dll_directory_cookies&) = delete;
    dll_directory_cookies& operator= (const dll_directory_cookies&) = delete;
    
    ~dll_directory_cookies()
    {
        remove_all();

#if MG_OS__WIN_AVAIL
#if defined(_WIN32_WINNT) && _WIN32_WINNT < _WIN32_WINNT_WIN8
        if (kernel32_ != NULL)
            ::FreeLibrary(kernel32_);
#endif
#endif
    }

    inline DLL_DIRECTORY_COOKIE add(const memepp::string_view& _path)
    {
#if MG_OS__WIN_AVAIL
        auto path = _path.replace("/", "\\");
        auto it = cookies_.find(path);
        if (it != cookies_.end())
            return it->second;

        auto native = mm_to<memepp::native_string>(path);
        auto cookie = (add_dll_dir_fn_ ? add_dll_dir_fn_(native.data()) : NULL);
        if (cookie == NULL)
            return NULL;

        cookies_[path] = cookie;
        return cookie;
#else
        return NULL;
#endif
    }

    inline bool remove(const memepp::string& _path)
    {
#if MG_OS__WIN_AVAIL
        auto it = cookies_.find(_path);
        if (it == cookies_.end())
            return true;

        auto ret = (remove_dll_dir_fn_ ? remove_dll_dir_fn_(it->second) : false);
        cookies_.erase(it);
        return !!ret;
#else
        return true;
#endif
    }

    inline bool remove(DLL_DIRECTORY_COOKIE _cookie)
    {
#if MG_OS__WIN_AVAIL
        for (auto it = cookies_.begin(); it != cookies_.end(); ++it)
        {
            if (it->second == _cookie)
            {
                auto ret = (remove_dll_dir_fn_ ? remove_dll_dir_fn_(it->second) : false);
                cookies_.erase(it);
                return !!ret;
            }
        }
        return true;
#endif
        return true;
    }

    inline bool remove_all()
    {
#if MG_OS__WIN_AVAIL
        if (remove_dll_dir_fn_) {
            for (auto& it : cookies_)
                remove_dll_dir_fn_(it.second);
        }

        cookies_.clear();
#endif
        return true;
    }

private:
#if MG_OS__WIN_AVAIL
    std::unordered_map<memepp::string, DLL_DIRECTORY_COOKIE> cookies_;
    AddDllDirectoryPtr    add_dll_dir_fn_;
    RemoveDllDirectoryPtr remove_dll_dir_fn_;
#if defined(_WIN32_WINNT) && _WIN32_WINNT < _WIN32_WINNT_WIN8
    HMODULE kernel32_;
#endif
#endif
};


}
}
}

#endif // !MEGOPP_UTIL_OS_WIN_ADD_OR_REMOVE_DLL_DIRECTORY_H_INCLUDED
