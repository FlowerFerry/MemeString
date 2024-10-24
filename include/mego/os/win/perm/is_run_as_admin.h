
#ifndef MEGO_OS_WIN_PERM_IS_RUN_AS_ADMIN_H_INCLUDED
#define MEGO_OS_WIN_PERM_IS_RUN_AS_ADMIN_H_INCLUDED

#include <mego/predef/symbol/inline.h>
#include <mego/util/os/windows/windows_simplify.h>

#include <stdbool.h>

#if MG_OS__WIN_AVAIL
#  pragma comment(lib, "Advapi32.lib")
#endif

//! 检查当前进程是否以管理员权限运行。
//! 
//! @note 此函数仅在Windows操作系统上可用，使用Windows API来检查当前用户令牌是否属于管理员组。
//! 
//! @return 如果当前进程以管理员权限运行则返回true，否则返回false。
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
