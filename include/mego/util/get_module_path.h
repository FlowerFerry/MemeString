
#ifndef MEGO_UTIL_GET_MODULE_PATH_H_INCLUDED
#define MEGO_UTIL_GET_MODULE_PATH_H_INCLUDED

#include <mego/predef/os/windows.h>
#include <mego/predef/symbol/inline.h>

#include <mego/util/os/windows/windows_simplify.h>

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif // __cppplusplus

#if MG_OS__WIN_AVAIL

    MG_CAPI_INLINE int MegoUtilImpl_GetModulePath(
        HMODULE _module, char* _out, int _capacity, int* _dirname_pos)
    {
        wchar_t buf[MAX_PATH] = { 0 };
        int length = -1;
        wchar_t* path = NULL;

        if (!_out)
            _capacity = 0;
        
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
                    if (!tmp) {
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
            else if (w2mb_len <= 0) {
                break;
            }
            
            if (w2mb_len <= _capacity)
            {
                _out[w2mb_len] = '\0';
                
                if (_dirname_pos) {
                    for (int idx = w2mb_len - 1; idx >= 0; --idx)
                    {
                        if (_out[idx] == '\\')
                        {
                            *_dirname_pos = idx;
                            break;
                        }
                    }
                }
            }
            
            length = w2mb_len;

        } while (0);
        if (path && path != buf)
            free(path);

        if (_out && _capacity != 0 && length >= _capacity)
            _out[_capacity - 1] = '\0';
        
        return length;
    }
    
    MG_CAPI_INLINE int mgu_get_module_path(
        HMODULE _module, char* _out, int _capacity, int* _dirname_pos)
    {
        return MegoUtilImpl_GetModulePath(_module, _out, _capacity, _dirname_pos);
    }

    MG_CAPI_INLINE int mgu_get_module_w_path(
        HMODULE _module, wchar_t* _out, int _capacity, int* _dirname_pos)
    {
        int length = -1;
        wchar_t* path = NULL;
        DWORD result = 0;
        
        if (_out) {
            result = GetModuleFileNameW(_module, _out, _capacity);
            
            if (result != 0 && result != _capacity) {
                if (_dirname_pos) {
                    for (int idx = result - 1; idx >= 0; --idx)
                    {
                        if (_out[idx] == L'\\')
                        {
                            *_dirname_pos = idx;
                            break;
                        }
                    }
                }
                return result;
            }
            
            if (_capacity > 0)
                _out[_capacity - 1] = L'\0';
        }
 
        do {
            DWORD capacity = result = (MAX_PATH / 2);
            while (capacity == result)
            {
                wchar_t* tmp = (wchar_t*)realloc(path, capacity * 2 * sizeof(wchar_t));
                if (!tmp) {
                    break;
                }
                path = tmp;
                capacity *= 2;
                result = GetModuleFileNameW(_module, path, capacity);
            }

            if (capacity != result)
                length = result;
        } while (0);

        if (path)
            free(path);
        return length;
    }

#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MEGO_UTIL_GET_MODULE_PATH_H_INCLUDED

