
#ifndef MEGO_HARDWARE_CPU_COUNT_H_INCLUDED
#define MEGO_HARDWARE_CPU_COUNT_H_INCLUDED

#include <mego/thrd/call_once.h>
#include <mego/predef/symbol/inline.h>
#include <mego/predef/os/linux.h>
#include <mego/util/os/windows/windows_simplify.h>
#include <mego/util/std/file.h>

MG_CAPI_INLINE size_t* __mghw_processor_count_value()
{
    static size_t count = 0;
    return &count;
}

MG_CAPI_INLINE mgthrd_once_flag* __mghw_processor_count_once_flag()
{
    static mgthrd_once_flag flag = MGTHRD_ONCE_FLAG_INIT;
    return &flag;
}

MG_CAPI_INLINE void __mghw_processor_count_init()
{
#if MG_OS__WIN_AVAIL
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    *__mghw_processor_count_value() = sysinfo.dwNumberOfProcessors;
#elif MG_OS__LINUX_AVAIL
    FILE* fp = mgu_fopen("/proc/cpuinfo", "r");
    if (!fp) {
        *__mghw_processor_count_value() = 1;
        return;
    }

    char buf[512];
    size_t count = 0;
    while (fgets(buf, sizeof(buf), fp)) {
        if (strncmp(buf, "processor", 9) == 0) {
            ++count;
        }
    }

    fclose(fp);
    *__mghw_processor_count_value() = count;
#else
    *__mghw_processor_count_value() = 1;
#endif
}

MG_CAPI_INLINE size_t mghw_processor_count()
{
    mgthrd_call_once(__mghw_processor_count_once_flag(), __mghw_processor_count_init);
    return *__mghw_processor_count_value();
}

#endif // !MEGO_HARDWARE_CPU_COUNT_H_INCLUDED
