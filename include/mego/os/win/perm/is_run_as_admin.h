
#ifndef MEGO_OS_WIN_PERM_IS_RUN_AS_ADMIN_H_INCLUDED
#define MEGO_OS_WIN_PERM_IS_RUN_AS_ADMIN_H_INCLUDED

#include <mego/predef/symbol/inline.h>
#include <mego/util/os/windows/windows_simplify.h>

#include <stdbool.h>

#if MG_OS__WIN_AVAIL
#  pragma comment(lib, "Advapi32.lib")
#endif

MG_CAPI_INLINE bool mg_win_perm_is_run_as_admin() 
{
#if MG_OS__WIN_AVAIL
    BOOL fIsRunAsAdmin = FALSE;
    PSID pAdministratorsGroup = NULL;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if (!AllocateAndInitializeSid(
        &NtAuthority, 2,
        SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0,
        &pAdministratorsGroup)) {
        return false;
    }
    if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin)) {
        fIsRunAsAdmin = FALSE;
    }
    if (pAdministratorsGroup) {
        FreeSid(pAdministratorsGroup);
        pAdministratorsGroup = NULL;
    }
    return fIsRunAsAdmin != FALSE;
#else
    return false;
#endif
}

#endif // !MEGO_OS_WIN_PERM_IS_RUN_AS_ADMIN_H_INCLUDED
