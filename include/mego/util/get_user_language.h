
#ifndef MEGO_UTIL_GET_USER_LANGUAGE_H_INCLUDED
#define MEGO_UTIL_GET_USER_LANGUAGE_H_INCLUDED

#include <mego/util/os/windows/windows_simplify.h>
#include <mego/predef/symbol/inline.h>
#include <mego/predef/symbol/likely.h>
#include <mego/predef/os/linux.h>
#include <mego/err/ec.h>
#include <mego/err/ec_impl.h>
#include <mego/util/math.h>

//! @brief 获取当前用户的语言设置。
MG_CAPI_INLINE mgec_t mgu__get_user_language(char* _language, size_t _size) 
{
    if (MEGO_SYMBOL__UNLIKELY(_language == NULL)) 
        return MGEC__INVAL;
    
    if (MEGO_SYMBOL__UNLIKELY(_size < 2)) 
        return MGEC__INVAL;

#if MG_OS__WIN_AVAIL
    if (!GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SISO639LANGNAME, _language, _size)) 
    {
        return mgec__from_sys_err(GetLastError());
    }
    return 0;
#elif MG_OS__LINUX_AVAIL
    do {
        const char* pos = NULL;
        char* lang = getenv("LANG");
        if (lang == NULL) 
            return MGEC__ERR;
        
        pos = strchr(lang, '_');
        if (pos == NULL)
            return MGEC__ERR;

        strncpy(_language, lang, MGU_MATH__MIN(lang - pos, _size - 1));
        _language[_size - 1] = '\0';
    } while (0);
    return 0;
#else
    return MGEC__OPNOTSUPP;
#endif
    
}

#endif // !MEGO_UTIL_GET_USER_LANGUAGE_H_INCLUDED
