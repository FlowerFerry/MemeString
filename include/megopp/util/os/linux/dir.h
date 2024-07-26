
#ifndef MEGOPP_OS_LINUX_DIR_H_INCLUDED
#define MEGOPP_OS_LINUX_DIR_H_INCLUDED

#include <mego/predef/os/linux.h>
#include <mego/util/converted_native_string.h>

#include <megopp/err/err.h>
#include <memepp/string_view.hpp>
#include <megopp/util/scope_cleanup.h>

#if MG_OS__LINUX_AVAIL
#include <dirent.h>
#endif

namespace mgpp {
namespace os_linux {

    template <typename _Fn>
    inline mgpp::err readdir(const char* _path, mmint_t _slen, _Fn&& _fn)
    {  
#if MG_OS__LINUX_AVAIL
        static_assert(std::is_invocable_v<_Fn, const dirent*>, "Invalid function type");

        mmn_char_cptr_t path = NULL;
        mmint_t path_len = 0;
        mgec_t ec = mgu__to_cns(_path, _slen, &path, &path_len, 0);
        if (MEGO_SYMBOL__UNLIKELY(ec != 0))
            return { ec };
        MEGOPP_UTIL__ON_SCOPE_CLEANUP([&] { mgu__free_cns(_path, path); });

        struct dirent* entry = NULL;
        DIR* dir = opendir(path);
        if (MEGO_SYMBOL__UNLIKELY(dir == NULL))
            return { MGEC__ERR };
        MEGOPP_UTIL__ON_SCOPE_CLEANUP([&] { closedir(dir); });

        while ((entry = readdir(dir)) != NULL)
        {
            if constexpr (std::is_same_v<std::invoke_result_t<_Fn, const dirent*>, bool>)
            {
                if (!_fn(entry))
                    break;
            }
            else {
                _fn(entry);
            }
        }

        return { 0 };
#endif
        return { MGEC__OPNOTSUPP };
    }

}
}


#endif // !MEGOPP_OS_LINUX_DIR_H_INCLUDED
