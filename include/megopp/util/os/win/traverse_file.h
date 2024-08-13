
#ifndef MEGOPP_UTIL_OS_WIN_TRAVERSE_FILE_H_INCLUDED
#define MEGOPP_UTIL_OS_WIN_TRAVERSE_FILE_H_INCLUDED

#include <mego/predef/os/windows.h>
#include <mego/err/ec_impl.h>
#include <megopp/err/err.h>
#include <memepp/native.hpp>
#include <megopp/util/scope_cleanup.h>

#include <deque>
#include <type_traits>

#include <mego/util/os/windows/windows_simplify.h>

namespace mgpp {
namespace os {
namespace win {

#if MG_OS__WIN_AVAIL
    template<typename _Fn>
    inline void traverse_files_and_directorys_u16(
        const memepp::native_string& _dir, const memepp::native_string& _wildcard, _Fn&& _fn, mgpp::err& _err)
    {
        memepp::native_string path = _dir + _wildcard;
        WIN32_FIND_DATAW findFileData;
        HANDLE hFind = FindFirstFileW(path.data(), &findFileData);
        if (hFind == INVALID_HANDLE_VALUE) {
            _err = mgpp::err(mgec__from_sys_err(GetLastError()), "FindFirstFileW");
            return;
        }
        MEGOPP_UTIL__ON_SCOPE_CLEANUP([&] { FindClose(hFind); });
        
        do {
            if (wcscmp(findFileData.cFileName, L".") == 0)
            {
                continue;
            }
            if (wcscmp(findFileData.cFileName, L"..") == 0)
            {
                continue;
            }

            if constexpr (std::is_same_v<std::invoke_result_t<_Fn, const WIN32_FIND_DATAW&>, bool>)
            {
                if (!_fn(findFileData))
                    break;
            }
            else {
                _fn(findFileData);
            }
        } while (FindNextFileW(hFind, &findFileData) != 0);

        DWORD dwError = GetLastError();
        if (dwError != ERROR_NO_MORE_FILES) {
            _err = mgpp::err(mgec__from_sys_err(dwError), "FindNextFileW");
            return;
        }
        
        _err = mgpp::err{};
        return;
    }
    
    template<typename _Ty>
    inline void recursive_directorys_u16(
        const memepp::native_string& _dir,
        const memepp::native_string& _subDir,
        const memepp::native_string& _wildcard,
        std::back_insert_iterator<_Ty> _subDirIt,
        mgpp::err& _err)
    {
        auto path = _dir + MMN_PATH_SEP_CH + _subDir 
            + memepp::native_string{ _subDir.empty() ? L"" : MMN_PATH_SEP_STR }
            + memepp::native_string{ _wildcard.empty() ? L"*" : _wildcard };
        WIN32_FIND_DATAW findFileData;
        HANDLE hFind = FindFirstFileW(path.data(), &findFileData);
        if (hFind == INVALID_HANDLE_VALUE) {
            _err = mgpp::err(mgec__from_sys_err(GetLastError()), "FindFirstFileW");
            return;
        }
        MEGOPP_UTIL__ON_SCOPE_CLEANUP([&] { FindClose(hFind); });
        
        do {
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
            {
                if (wcscmp(findFileData.cFileName, L".") == 0)
                {
                    continue;
                }
                if (wcscmp(findFileData.cFileName, L"..") == 0) 
                {
                    continue;
                }

                *_subDirIt++ = path = (_subDir + memepp::native_string{ _subDir.empty() ? L"" : MMN_PATH_SEP_STR } + findFileData.cFileName);
                recursive_directorys_u16(
                    _dir, path, _wildcard, _subDirIt, _err);
                if (_err) {
                    return;
                }
            }
            
        } while (FindNextFileW(hFind, &findFileData) != 0);
        
        DWORD dwError = GetLastError();
        if (dwError != ERROR_NO_MORE_FILES) {
            _err = mgpp::err(mgec__from_sys_err(dwError), "FindNextFileW");
            return;
        }
        
        _err = mgpp::err{};
        return;
    }

    template<typename _Ty>
    inline void recursive_directorys_u16(
        const memepp::native_string& _dir,
        const memepp::native_string& _wildcard,
        std::back_insert_iterator<_Ty> _subDirIt,
        mgpp::err& _err)
    {
        recursive_directorys_u16(_dir, {}, _wildcard, _subDirIt, _err);
    }

    template<typename _Fn>
    inline void list_files_by_recursive_directory_u16(
        const memepp::native_string& _dir, 
        const memepp::native_string& _wildcard, _Fn&& _fn, mgpp::err& _err)
    {
        std::deque<memepp::native_string> _subDirs;
        auto subDirIt = std::back_inserter(_subDirs);
        recursive_directorys_u16(_dir, L"*", subDirIt, _err);
        if (_err) {
            return;
        }

        WIN32_FIND_DATAW findFileData;
        for (const auto& subDir : _subDirs) {
            memepp::native_string path = _dir + MMN_PATH_SEP_CH + subDir + MMN_PATH_SEP_CH
                + memepp::native_string{ _wildcard.empty() ?  L"*"  : _wildcard };
            HANDLE hFind = FindFirstFileW(path.data(), &findFileData);
            if (hFind == INVALID_HANDLE_VALUE) {
                _err = mgpp::err(mgec__from_sys_err(GetLastError()), "FindFirstFileW");
                return;
            }
            MEGOPP_UTIL__ON_SCOPE_CLEANUP([&] { FindClose(hFind); });

            do {
                if (wcscmp(findFileData.cFileName, L".") == 0)
                {
                    continue;
                }
                if (wcscmp(findFileData.cFileName, L"..") == 0)
                {
                    continue;
                }

                if constexpr (std::is_same_v<std::invoke_result_t<_Fn, const memepp::native_string&, const WIN32_FIND_DATAW&>, bool>)
                {
                    if (!_fn(subDir, findFileData))
                        break;
                }
                else {
                    _fn(subDir, findFileData);
                }
            } while (FindNextFileW(hFind, &findFileData) != 0);

            DWORD dwError = GetLastError();
            if (dwError != ERROR_NO_MORE_FILES) {
                _err = mgpp::err(mgec__from_sys_err(dwError), "FindNextFileW");
                return;
            }
        }

        _err = mgpp::err{};
        return;
    }

#endif
}
}
}

#endif // !MEGOPP_UTIL_OS_WIN_TRAVERSE_FILE_H_INCLUDED
