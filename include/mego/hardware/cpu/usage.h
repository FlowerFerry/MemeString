
#ifndef MEGO_HARDWARE_CPU_USAGE_H_INCLUDED
#define MEGO_HARDWARE_CPU_USAGE_H_INCLUDED

#include <mego/predef/symbol/inline.h>
#include <mego/util/os/windows/windows_simplify.h>
#include <stdio.h>
#include <stdlib.h>

#if MG_OS__WIN_AVAIL
MG_CAPI_INLINE float __mghw_cpu_usage_calc(FILETIME idleTime, FILETIME kernelTime, FILETIME userTime)
{
    ULARGE_INTEGER idle, kernel, user;
    idle.LowPart    = idleTime.dwLowDateTime;
    idle.HighPart   = idleTime.dwHighDateTime;
    kernel.LowPart  = kernelTime.dwLowDateTime;
    kernel.HighPart = kernelTime.dwHighDateTime;
    user.LowPart    = userTime.dwLowDateTime;
    user.HighPart   = userTime.dwHighDateTime;

    return 1.0 - (double)(idle.QuadPart) / (kernel.QuadPart + user.QuadPart);
}
#endif

MG_CAPI_INLINE float mghw_cpu_usage()
{
#if MG_OS__WIN_AVAIL
    FILETIME idleTime, kernelTime, userTime;
    if (GetSystemTimes(&idleTime, &kernelTime, &userTime) == FALSE)
        return -1.0f;
    
    return __mghw_cpu_usage_calc(idleTime, kernelTime, userTime);
#else
    char buf[256] = { 0 };
    char name[64] = { 0 };
	uint32_t user = 0, nice = 0, system = 0, idle = 0;
    FILE* fp = fopen("/proc/stat", "r");
    if (fp == NULL)
        return -1.0f;
    
    fgets(buf, sizeof(buf), fp);
    fclose(fp);

    if (sscanf(buf, "%s %u %u %u %u", name, &user, &nice, &system, &idle) != 5)
        return -1.0f;
    
    return 1.0 - (double)idle / (user + nice + system + idle);
#endif 
}

#endif // !MEGO_HARDWARE_CPU_USAGE_H_INCLUDED
