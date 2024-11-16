
#ifndef MEGO_UTIL_OS_GET_PROCESS_HANDLE_COUNT_H_INCLUDED
#define MEGO_UTIL_OS_GET_PROCESS_HANDLE_COUNT_H_INCLUDED

#include <mego/predef/symbol/inline.h>
#include <mego/predef/os/linux.h>
#include <mego/util/os/windows/windows_simplify.h>
#include <mego/err/ec.h>
#include <mego/err/ec_impl.h>

#if MG_OS__LINUX_AVAIL
#include <unistd.h>
#include <dirent.h>
#endif

#include <stdio.h>

MG_CAPI_INLINE mgec_t mgu_get_self_handle_count(size_t* _count) 
{
    if (!_count) 
        return MGEC__INVAL;

#if MG_OS__WIN_AVAIL
    DWORD count;
    HANDLE process = GetCurrentProcess();
    if (!GetProcessHandleCount(process, &count)) 
    {
        return mgec__from_sys_err(GetLastError());
    }
    *_count = count;
    return 0;
#else
    pid_t pid = getpid();
    char path[128];
    snprintf(path, sizeof(path), "/proc/%d/fd", pid);
    DIR* dir = opendir(path);
    if (!dir)  {
        return mgec__from_sys_err(errno);
    }
    
    size_t count = 0;
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_LNK) {
            ++count;
        }
    }
    closedir(dir);
    *_count = count;
    return 0;
#endif
}

#endif // !MEGO_UTIL_OS_GET_PROCESS_HANDLE_COUNT_H_INCLUDED
