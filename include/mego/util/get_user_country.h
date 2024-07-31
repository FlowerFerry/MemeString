
#ifndef MEGO_UTIL_GET_USER_COUNTRY_H_INCLUDED
#define MEGO_UTIL_GET_USER_COUNTRY_H_INCLUDED

#include <mego/util/os/windows/windows_simplify.h>
#include <mego/predef/symbol/inline.h>
#include <mego/predef/symbol/likely.h>
#include <mego/predef/os/linux.h>
#include <mego/err/ec.h>
#include <mego/err/ec_impl.h>
#include <mego/util/math.h>

//! @brief 获取当前用户的国家/地区设置。
MG_CAPI_INLINE mgec_t mgu__get_user_country(char* _country, size_t _size) 
{
    if (MEGO_SYMBOL__UNLIKELY(_country == NULL)) 
        return MGEC__INVAL;
    
    if (MEGO_SYMBOL__UNLIKELY(_size < 2)) 
        return MGEC__INVAL;
    
#if MG_OS__WIN_AVAIL
    if (!GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SISO3166CTRYNAME, _country, _size)) 
    {
        return mgec__from_sys_err(GetLastError());
    }
    return 0;
#elif MG_OS__LINUX_AVAIL
    do {
        const char* pos1 = NULL;
        const char* pos2 = NULL;
        char* lang = getenv("LANG");
        if (lang == NULL) 
            return MGEC__ERR;

        pos1 = strchr(lang, '_');
        if (pos1 == NULL) 
            return MGEC__ERR;

        pos2 = strchr(pos1 + 1, '.');
        if (pos2 == NULL) 
            strncpy(_country, pos1 + 1, MGU_MATH__MIN(strlen(pos1 + 1), _size - 1));
        else
            strncpy(_country, pos1 + 1, MGU_MATH__MIN(pos2 - pos1 - 1,  _size - 1));
        _country[_size - 1] = '\0';
    } while (0);
    return 0;
#else
    return MGEC__OPNOTSUPP;
#endif

}

#endif // !MEGO_UTIL_OS_WINDOWS_GET_USER_COUNTRY_H_INCLUDED
