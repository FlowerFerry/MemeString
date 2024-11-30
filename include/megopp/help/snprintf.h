
#ifndef MEGOPP_HELP_SNPRINTF_H_INCLUDED
#define MEGOPP_HELP_SNPRINTF_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <mego/mem/cstr_alloc_if_no_end_zero.h>
#include <meme/string_fwd.h>

#include <megopp/util/scope_cleanup.h>

#include <string>
#include <vector>

namespace mgpp {
namespace help {
    
template<size_t _PreSize = 128>    
inline ::std::string vsnprintf(
    mmint_t _limit, const char* _fmt, mmint_t _fmt_len, va_list _args)
{
    if (!_fmt)
        return {};
    
    char sbuf[_PreSize];
    va_list args;
    va_copy(args, _args);
    const char* fmt;
    mgec_t ec = mgmem__cstr_alloc_if_no_end_zero(_fmt, _fmt_len, &fmt, NULL, 0);
    if (MG_SYM__UNLIKELY(ec != 0))
        return {};
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&] { mgmem__free_if_ptr_not_equal(_fmt, (void*)fmt); });

    int len = ::vsnprintf(sbuf, sizeof(sbuf), fmt, args);
    va_end(args);
    
    if (MG_SYM__UNLIKELY(len <= 0))
        return {};

    if (_limit > 0)
        len = (std::min)(len, static_cast<int>(_limit));
    
    if (len < sizeof(sbuf)) {
        return { sbuf, static_cast<size_t>(len) };
    }
    
    std::vector<char> buf(len + 1);
    len = ::vsnprintf(buf.data(), buf.size(), fmt, _args);

    if (MG_SYM__UNLIKELY(len <= 0))
        return {};
    
    return { buf.data(), static_cast<size_t>(len) };
}

template<size_t _PreSize = 128>
inline ::std::string snprintf(
    mmint_t _limit, const char* _fmt, mmint_t _fmt_len, ...)
{
    va_list args;
    va_start(args, _fmt);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&] { va_end(args); });
    return vsnprintf<_PreSize>(_limit, _fmt, _fmt_len, args);
}

}
}

#endif // !MEGOPP_HELP_SNPRINTF_H_INCLUDED
