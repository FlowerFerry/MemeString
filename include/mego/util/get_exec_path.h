
#ifndef MEGO_UTIL_GET_EXEC_PATH_H_INCLUDED
#define MEGO_UTIL_GET_EXEC_PATH_H_INCLUDED

#include <mego/util/get_module_path.h>

// Mac OS X : _NSGetExecutablePath() (man 3 dyld)
// Linux : readlink /proc/self/exe
// Solaris : getexecname()
// FreeBSD : sysctl CTL_KERN KERN_PROC KERN_PROC_PATHNAME - 1
// FreeBSD if it has procfs : readlink /proc/curproc/file(FreeBSD doesn't have procfs by default)
// NetBSD: readlink /proc/curproc/exe
// DragonFly BSD : readlink /proc/curproc/file
// Windows : GetModuleFileName() with hModule = NULL

#if MG_OS__WIN_AVAIL
#elif defined(__linux__) || defined(__CYGWIN__) || defined(__sun)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(__linux__)
#include <linux/limits.h>
#else
#include <limits.h>
#endif
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>

#if !defined(__MEGOUTIL__PROC_SELF_EXE_PATH)
#if defined(__sun)
#define __MEGOUTIL__PROC_SELF_EXE_PATH "/proc/self/path/a.out"
#else
#define __MEGOUTIL__PROC_SELF_EXE_PATH "/proc/self/exe"
#endif
#endif

#else

#error unsupported platform

#endif

#ifdef __cplusplus
extern "C" {
#endif // __cppplusplus

#if MG_OS__WIN_AVAIL

    MG_CAPI_INLINE int MegoUtil_GetExecutablePath(char* _out, int _capacity, int* _dirname_pos)
    {
        return MegoUtilImpl_GetModulePath(NULL, _out, _capacity, _dirname_pos);
    }

    MG_CAPI_INLINE int mgu_get_exec_w_path(wchar_t* _out, int _capacity, int* _dirname_pos)
    {
        return mgu_get_module_w_path(NULL, _out, _capacity, _dirname_pos);
    }

#elif defined(__linux__) || defined(__CYGWIN__) || defined(__sun)
    MG_CAPI_INLINE int MegoUtil_GetExecutablePath(char* _out, int _capacity, int* _dirname_pos)
    {
        char buffer[PATH_MAX] = { 0 };
        int  length = -1;

        char* resolved = realpath(__MEGOUTIL__PROC_SELF_EXE_PATH, buffer);
        if (!resolved)
            return -1;
        length = (int)strlen(buffer);
        
        if (length > _capacity) {
            return length;
        }

        memcpy(_out, buffer, length);
        if (_dirname_pos) {
            int i = length - 1;
            for (; i >= 0; --i)
            {
                if (_out[i] == '/')
                {
                    *_dirname_pos = i;
                    break;
                }
            }
        }
        return length;
    }
#else

#error Unsupported platform
    
#endif
    
MG_CAPI_INLINE int mgu_get_executable_path(char* _out, int _capacity, int* _dirname_pos)
{
    return MegoUtil_GetExecutablePath(_out, _capacity, _dirname_pos);
}

MG_CAPI_INLINE int mgu_get_exec_path(char* _out, int _capacity, int* _dirname_pos)
{
    return MegoUtil_GetExecutablePath(_out, _capacity, _dirname_pos);
}

#ifdef __cplusplus
}
#endif // __cplusplus

#undef __MEGOUTIL__PROC_SELF_EXE_PATH

#endif // MEGO_UTIL_GET_EXEC_PATH_H_INCLUDED

