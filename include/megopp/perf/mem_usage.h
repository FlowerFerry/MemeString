
#ifndef MEGO_PERF_MEM_USAGE_H_INCLUDED
#define MEGO_PERF_MEM_USAGE_H_INCLUDED

#include <mego/predef/symbol/inline.h>
#include <mego/util/os/windows/windows_simplify.h>
#include <mego/err/ec_impl.h>
#include <stdio.h>
#include <stdlib.h>

#if MG_OS__WIN_AVAIL
#  include <Psapi.h>
#  pragma comment(lib, "Psapi.lib")
#endif

#include <megopp/util/os/linux/dir.h>
#include <megopp/util/scope_cleanup.h>
#include <megopp/err/err.h>
#include <vector>
#include <algorithm>
#include <type_traits>

namespace mgpp {
namespace perf {

template<typename _Fn>
inline mgpp::err enum_top_physical_memory_size(int _limit, _Fn&& fn)
{
    static_assert(std::is_invocable_v<_Fn, uint32_t, uint64_t>, "Invalid function type");
    if (_limit <= 0)
        return {};
    
#if MG_OS__WIN_AVAIL

    std::vector<uint32_t> pids{ 1024 };
    do {
        uint32_t cbNeeded;
        if (!EnumProcesses(pids.data(), static_cast<uint32_t>(pids.size() * sizeof(uint32_t)), &cbNeeded))
            return mgpp::err { mgec__from_sys_err(GetLastError()) };
        
        if (cbNeeded < pids.size() * sizeof(uint32_t))
        {
            pids.resize(cbNeeded / sizeof(uint32_t));
            break;
        }
        
        pids.resize(pids.size() * 2);
    } while (true);

    std::vector<std::tuple<uint32_t, uint64_t>> procInfos;

    for (auto pid : pids) {
        if (pid == 0)
            continue;

        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (hProcess == nullptr)
            continue;
        auto hdlCleanup = util::scope_cleanup__create([&] { CloseHandle(hProcess); });

        PROCESS_MEMORY_COUNTERS pmc;
        if (!GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
            continue;

        procInfos.emplace_back(pid, pmc.WorkingSetSize);

    }

    std::sort(procInfos.begin(), procInfos.end(), [](const auto& lhs, const auto& rhs) {
        return std::get<1>(lhs) > std::get<1>(rhs);
    });

    for (auto& info : procInfos) {
        if constexpr (std::is_same_v<std::invoke_result_t<_Fn, uint32_t, uint64_t>, bool>) 
        {
            if (!fn(std::get<0>(info), std::get<1>(info)))
                break;
        }
        else {
            fn(std::get<0>(info), std::get<1>(info));
        }

        if (--_limit == 0)
            break;
    }
#else
    char path[256];
    char line[256];
    std::vector<std::tuple<uint32_t, uint64_t>> procInfos;
    auto e = mgpp::os_linux::readdir("/proc", -1, 
    [&](const dirent* entry) 
    {
        if (entry->d_type != DT_DIR)
            return;
        
        int pid = atoi(entry->d_name);
        if (pid == 0)
            return;
        
        snprintf(path, sizeof(path), "/proc/%d/status", pid);

        FILE* fp = fopen(path, "r");
        if (fp == nullptr)
            return;
        MEGOPP_UTIL__ON_SCOPE_CLEANUP([&] { fclose(fp); });

        char line[256];
        while (fgets(line, sizeof(line), fp) != nullptr)
        {
            if (strncmp(line, "VmRSS:", 6) != 0)
                continue;
            
            uint64_t rss;
            if (sscanf(line, "VmRSS: %llu", &rss) == 1)
            {
                procInfos.emplace_back(pid, rss * 1024);
                break;
            }
        }
    });
    if (e != 0)
        return { e };
    
    std::sort(procInfos.begin(), procInfos.end(), [](const auto& lhs, const auto& rhs) {
        return std::get<1>(lhs) > std::get<1>(rhs);
    });

    for (auto& info : procInfos) {
        if constexpr (std::is_same_v<std::invoke_result_t<_Fn, uint32_t, uint64_t>, bool>) 
        {
            if (!fn(std::get<0>(info), std::get<1>(info)))
                break;
        }
        else {
            fn(std::get<0>(info), std::get<1>(info));
        }

        if (--_limit == 0)
            break;
    }
#endif

    return {};
}

} // namespace perf
} // namespace mgpp

#endif // !MEGO_PERF_MEM_USAGE_H_INCLUDED
