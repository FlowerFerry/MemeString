
#ifndef MEGO_HARDWARE_CPU_USAGE_H_INCLUDED
#define MEGO_HARDWARE_CPU_USAGE_H_INCLUDED

#include <mego/predef/symbol/inline.h>
#include <mego/util/os/windows/windows_simplify.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#if MG_OS__WIN_AVAIL
MG_CAPI_INLINE double __mghw_cpu_usage_calc(
    FILETIME prevIdleTime, FILETIME prevKernelTime, FILETIME prevUserTime,
    FILETIME currIdleTime, FILETIME currKernelTime, FILETIME currUserTime)
{
    ULARGE_INTEGER prevIdle, prevKernel, prevUser;
    ULARGE_INTEGER currIdle, currKernel, currUser;
    ULONGLONG idleDiff;
    ULONGLONG kernelDiff;
    ULONGLONG userDiff;
    ULONGLONG totalDiff;

    prevIdle.LowPart    = prevIdleTime.dwLowDateTime;
    prevIdle.HighPart   = prevIdleTime.dwHighDateTime;
    prevKernel.LowPart  = prevKernelTime.dwLowDateTime;
    prevKernel.HighPart = prevKernelTime.dwHighDateTime;
    prevUser.LowPart    = prevUserTime.dwLowDateTime;
    prevUser.HighPart   = prevUserTime.dwHighDateTime;

    currIdle.LowPart    = currIdleTime.dwLowDateTime;
    currIdle.HighPart   = currIdleTime.dwHighDateTime;
    currKernel.LowPart  = currKernelTime.dwLowDateTime;
    currKernel.HighPart = currKernelTime.dwHighDateTime;
    currUser.LowPart    = currUserTime.dwLowDateTime;
    currUser.HighPart   = currUserTime.dwHighDateTime;

    idleDiff   = currIdle.QuadPart - prevIdle.QuadPart;
    kernelDiff = currKernel.QuadPart - prevKernel.QuadPart;
    userDiff   = currUser.QuadPart - prevUser.QuadPart;
    totalDiff  = kernelDiff + userDiff;

    if (totalDiff == 0)
        return nan("");

    return 1.0 - (double)idleDiff / totalDiff;
}
#endif

//! @brief 获取当前CPU使用率
MG_CAPI_INLINE double mghw_cpu_usage()
{
#if MG_OS__WIN_AVAIL
    static FILETIME prevIdleTime = { 0, 0 }, prevKernelTime = { 0, 0 }, prevUserTime = { 0, 0 };
    FILETIME currIdleTime, currKernelTime, currUserTime;
    if (GetSystemTimes(&currIdleTime, &currKernelTime, &currUserTime) == FALSE)
        return nan("");
    
    double usage = __mghw_cpu_usage_calc(
        prevIdleTime, prevKernelTime, prevUserTime,
        currIdleTime, currKernelTime, currUserTime);
    
    prevIdleTime   = currIdleTime;
    prevKernelTime = currKernelTime;
    prevUserTime   = currUserTime;

    return usage;
#else
    char buf[512];
    char name[64];
	uint64_t user = 0, nice = 0, system = 0, idle = 0, io_wait = 0, irq = 0, soft_irq = 0;
    uint64_t curr_idle = 0, curr_total = 0;
    uint64_t total_diff = 0;
    static uint64_t prev_idle = 0; prev_total = 0;
    static double usage = nan("");
    FILE* fp = fopen("/proc/stat", "r");
    if (fp == NULL)
        return -1.0f;
    
    fgets(buf, sizeof(buf) - 1, fp);
    buf[sizeof(buf) - 1] = '\0';
    fclose(fp);

    if (sscanf_s(buf, "%s %llu %llu %llu %llu %llu %llu %llu", 
        name, sizeof(name) - 1, &user, &nice, &system, &idle, &io_wait, &irq, &soft_irq) != 8)
        return -1.0f;
    name[sizeof(name) - 1] = '\0';
    
    curr_idle  = idle + io_wait;
    curr_total = user + nice + system + idle + io_wait + irq + soft_irq;

    if (prev_total == 0) {
        prev_total = curr_total;
        prev_idle  = curr_idle;
        return usage;
    }

    total_diff = curr_total - prev_total;
    if (total_diff == 0)
        return usage;

    usage = 1.0 - (double)(curr_idle - prev_idle) / total_diff;
    prev_total = curr_total;
    prev_idle  = curr_idle;
    return usage;
#endif 
}

#endif // !MEGO_HARDWARE_CPU_USAGE_H_INCLUDED
