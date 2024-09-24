
#ifndef MEGO_UTIL_OS_GET_PROC_NAME_BY_PID_H_INCLUDED
#define MEGO_UTIL_OS_GET_PROC_NAME_BY_PID_H_INCLUDED

#include <mego/predef/os/windows.h>
#include <mego/predef/symbol/inline.h>
#include <mego/util/os/windows/windows_simplify.h>
#include <mego/err/ec.h>

#if MG_OS__WIN_AVAIL
#  include <Psapi.h>
#  pragma comment(lib, "Psapi.lib")
#endif

#include <stdint.h>
#include <stdlib.h>

MG_CAPI_INLINE mgec_t mgos_get_proc_name_by_pid(uint32_t _pid, char* _buf, size_t _buf_size)
{
#if MG_OS__WIN_AVAIL
    HANDLE hProcess;
    HMODULE hMod;
    DWORD cbNeeded;
    DWORD result;
    wchar_t* namePtr = nullptr;
    int nameLen = 128;
    if (_buf == NULL || _buf_size == 0)
        return MGEC__INVAL;
    _buf[0] = '\0';

    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, _pid);
    if (hProcess == NULL)
        return MGEC__ERR;

    if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
    {
        int w2mbLen;
        do {
            if (namePtr)
                free(namePtr);
            namePtr = (wchar_t*)malloc(nameLen * sizeof(wchar_t));
            if (namePtr == nullptr) {
                CloseHandle(hProcess);
                return MGEC__ERR;
            }

            result = GetModuleBaseNameW(hProcess, hMod, namePtr, nameLen);
            if (result == 0) {
                free(namePtr);
                CloseHandle(hProcess);
                return MGEC__ERR;
            }

            if (result == nameLen) {
                nameLen *= 2;
                continue;
            }

            break;
        } while(true);

        w2mbLen = WideCharToMultiByte(CP_UTF8, 0, namePtr, result, _buf, _buf_size, NULL, NULL);
        if (w2mbLen <= 0) {
            free(namePtr);
            CloseHandle(hProcess);
            return MGEC__ERR;
        }

        if (w2mbLen < _buf_size)
            _buf[w2mbLen] = '\0';
        _buf[_buf_size - 1] = '\0';
    }

    if (namePtr)
        free(namePtr);
    CloseHandle(hProcess);
    return 0;
#else
    char path[256];
    FILE* fp;
    if (_buf == NULL || _buf_size == 0)
        return MGEC__INVAL;
    _buf[0] = '\0';
    snprintf(path, sizeof(path), "/proc/%d/comm", _pid);

    fp = fopen(path, "r");
    if (fp == NULL)
        return MGEC__ERR;
    
    fgets(_buf, _buf_size, fp);
    fclose(fp);

    _buf[strcspn(_buf, "\n")] = '\0';
    return 0;
#endif

}

#endif // !MEGO_UTIL_OS_GET_PROC_NAME_BY_PID_H_INCLUDED
