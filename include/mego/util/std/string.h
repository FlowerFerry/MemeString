
#ifndef MEGO_UTIL_STD_STRING_H_INCLUDED
#define MEGO_UTIL_STD_STRING_H_INCLUDED

#include <mego/predef/os/windows.h>
#include <mego/predef/symbol/inline.h>
#include <mego/util/std/errno.h>
#include <mego/predef/symbol/likely.h>

#ifndef __STDC_WANT_LIB_EXT1__
#define __STDC_WANT_LIB_EXT1__ 1
#define __MEGO_UNDEF__STDC_WANT_LIB_EXT1__ 1
#endif

#include <string.h>

#if MG_OS__WIN_AVAIL || defined(__STDC_LIB_EXT1__)

#define mgu_strncpy_s(dest, destsz, src, count) \
    strncpy_s(dest, destsz, src, count)

#else

MG_CAPI_INLINE errno_t mgu_strncpy_s(
    char* _dest, size_t _destsz, const char* _src, size_t _count) 
{
    if (MG_SYM__UNLIKELY(_dest == NULL || _destsz == 0)) 
        return EINVAL;
    
    if (MG_SYM__UNLIKELY(_src == NULL)) 
    {
        _dest[0] = '\0';
        return EINVAL;
    }

    if (MG_SYM__UNLIKELY(_count >= _destsz)) 
    {
        _dest[0] = '\0';
        return ERANGE;
    }

    strncpy(_dest, _src, _count);
    _dest[_count] = '\0';
    return 0;
}

#endif

#ifdef __MEGO_UNDEF__STDC_WANT_LIB_EXT1__
#undef __STDC_WANT_LIB_EXT1__
#undef __MEGO_UNDEF__STDC_WANT_LIB_EXT1__
#endif

#endif // !MEGO_UTIL_STD_STRING_H_INCLUDED
