
#ifndef MEGO_UTIL_GET_MODULE_PATH_H_INCLUDED
#define MEGO_UTIL_GET_MODULE_PATH_H_INCLUDED

#include <mego/predef/os/windows.h>

#if MEGO_OS__WINDOWS__AVAILABLE
#   include <mego/util/os/windows/windows_simplify.h>
#endif

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif // __cppplusplus

#if MEGO_OS__WINDOWS__AVAILABLE
    int MegoUtilImpl_GetModulePath(
        HMODULE _module, char* _out, int _capacity, int* _dirname_pos);

    inline int MegoUtilImpl_GetModulePath(
        HMODULE _module, char* _out, int _capacity, int* _dirname_pos)
    {
        wchar_t buf[MAX_PATH] = { 0 };
        int length = -1;
        wchar_t* path = NULL;

        do {
            int w2mb_len = 0;
            int w2mb_eno = 0;
            DWORD result = GetModuleFileNameW(_module, buf, sizeof(buf) / sizeof(buf[0]));
            if (result == 0) {
                break;
            }

            if (result == sizeof(buf) / sizeof(buf[0]))
            {
                DWORD capacity = result;
                while (result == capacity)
                {
                    wchar_t* tmp = (wchar_t*)realloc(path, capacity * 2 * sizeof(wchar_t));
                    if (!tmp)
                    {
                        break;
                    }
                    path = tmp;
                    capacity *= 2;
                    result = GetModuleFileNameW(_module, path, capacity);
                }
                if (result == capacity)
                {
                    break;
                }
            }
            else {
                path = buf;
            }

            w2mb_len = WideCharToMultiByte(CP_UTF8, 0, path, result, _out, _capacity, NULL, NULL);
            w2mb_eno = GetLastError();

            if (w2mb_eno == ERROR_INSUFFICIENT_BUFFER)
            {
                w2mb_len = WideCharToMultiByte(CP_UTF8, 0, path, result, NULL, 0, NULL, NULL);
                if (w2mb_len <= 0) {
                    break;
                }

            }
            else if (w2mb_len <= 0)
            {
                break;
            }

            if (w2mb_len <= _capacity && _dirname_pos)
            {
                int i;

                for (i = w2mb_len - 1; i >= 0; --i)
                {
                    if (_out[i] == '\\')
                    {
                        *_dirname_pos = i;
                        break;
                    }
                }
            }
            
            length = w2mb_len;

        } while (0);
        if (path && path != buf)
            free(path);

        return length;
    }
    
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MEGO_UTIL_GET_MODULE_PATH_H_INCLUDED

