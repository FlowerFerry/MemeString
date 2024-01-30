
#ifndef MEGOPP_UTIL_OS_WIN_ADD_OR_REMOVE_DLL_DIRECTORY_H_INCLUDED
#define MEGOPP_UTIL_OS_WIN_ADD_OR_REMOVE_DLL_DIRECTORY_H_INCLUDED

#include <mego/util/os/windows/windows_simplify.h>

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/convert/std/wstring.hpp>
#include <memepp/convert/std/string.hpp>
#include <memepp/native.hpp>

#include <unordered_map>

namespace mgpp {
namespace os {
namespace win {

struct dll_directory_cookies
{

    dll_directory_cookies() = default;
    dll_directory_cookies(const dll_directory_cookies&) = delete;
    dll_directory_cookies& operator= (const dll_directory_cookies&) = delete;
    
    ~dll_directory_cookies()
    {
#if MG_OS__WIN_AVAIL
        for (auto& it : cookies_)
            ::RemoveDllDirectory(it.second);
#endif
    }

    inline DLL_DIRECTORY_COOKIE add(const memepp::string_view& _path)
    {
#if MG_OS__WIN_AVAIL
        auto it = cookies_.find(_path);
        if (it != cookies_.end())
            return it->second;

        auto path = mm_to<memepp::native_string>(_path);
        auto cookie = ::AddDllDirectory(path.data());
        if (cookie == NULL)
            return NULL;

        cookies_[_path] = cookie;
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

        auto ret = ::RemoveDllDirectory(it->second);
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
                auto ret = ::RemoveDllDirectory(it->second);
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
        for (auto& it : cookies_)
            ::RemoveDllDirectory(it.second);

        cookies_.clear();
#endif
        return true;
    }

private:
#if MG_OS__WIN_AVAIL
    std::unordered_map<memepp::string, DLL_DIRECTORY_COOKIE> cookies_;
#endif
};


}
}
}

#endif // !MEGOPP_UTIL_OS_WIN_ADD_OR_REMOVE_DLL_DIRECTORY_H_INCLUDED
