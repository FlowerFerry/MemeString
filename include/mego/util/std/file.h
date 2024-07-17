
#ifndef MGU_STD_FILE_H_INCLUDED
#define MGU_STD_FILE_H_INCLUDED

#include <meme/utf/converter.h>
#include <mego/predef/os/linux.h>
#include <mego/predef/os/windows.h>
#include <mego/util/converted_native_string.h>

#include <string.h>
#include <stdio.h>

#if MG_OS__LINUX_AVAIL
#include <unistd.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#if MG_OS__WIN_AVAIL
MG_CAPI_INLINE FILE* mgu_w_fopen(
    const wchar_t* _path, mmint_t _slen, const wchar_t* _mode, mmint_t _mlen)
{
    FILE* fp = NULL;
    mmn_char_cptr_t path = NULL;
    mmn_char_cptr_t mode = NULL;
    mgec_t ec = 0;
    
    ec = mgu_w__to_cns(_path, _slen, &path, NULL, 0);
    if (MEGO_SYMBOL__UNLIKELY(ec != 0))
        return NULL;

    ec = mgu_w__to_cns(_mode, _mlen, &mode, NULL, 0);
    if (MEGO_SYMBOL__UNLIKELY(ec != 0)) {
        mgu_w__free_cns(_path, path);
        return NULL;
    }

    fp = _wfopen(path, mode);
    mgu_w__free_cns(_path, path);
    mgu_w__free_cns(_mode, mode);
    return fp;
}
#endif

//! @brief 打开一个文件，并返回文件指针。
//!
//! 此函数打开指定路径的文件，并以指定的模式进行操作。如果路径或模式的长度
//! 不为零，则会考虑相应的长度参数。
//!
//! @param _path 要打开的文件的路径。
//! @param _slen 路径字符串的长度。如果小于零，则假定路径是以空字符结尾的字符串。
//! @param _mode 打开文件的模式（例如 "r", "w", "a" 等）。
//! @param _mlen 模式字符串的长度。如果小于零，则假定模式是以空字符结尾的字符串。
//! @return 成功时返回指向文件的 `FILE` 指针，失败时返回 `NULL`。
//!
//! @note 此函数主要用于处理路径和模式的长度可变的情况，适用于需要精确控制字符串
//! 长度的场景。
MG_CAPI_INLINE FILE* mgu_fopen(
    const char* _path, mmint_t _slen, const char* _mode, mmint_t _mlen)
{
    FILE* fp = NULL;
    mmn_char_cptr_t path = NULL;
    mmn_char_cptr_t mode = NULL;
    mgec_t ec = 0;
    
    ec = mgu__to_cns(_path, _slen, &path, NULL, 0);
    if (MEGO_SYMBOL__UNLIKELY(ec != 0))
        return NULL;

    ec = mgu__to_cns(_mode, _mlen, &mode, NULL, 0);
    if (MEGO_SYMBOL__UNLIKELY(ec != 0)) {
        mgu__free_cns(_path, path);
        return NULL;
    }

#if MG_OS__WIN_AVAIL
    fp = _wfopen(path, mode);
#else
    fp = fopen(path, mode);
#endif
    mgu__free_cns(_path, path);
    mgu__free_cns(_mode, mode);
    return fp;
}

//! @brief 关闭文件流并可选地进行刷新操作。
//!
//! 此函数关闭给定的文件流。如果 `_flush` 参数为 `true`，则在关闭之前刷新该流。
//! 在 Linux 系统上，如果刷新成功，还会对文件描述符进行同步操作 (`fsync`)。
//!
//! @param _fp 指向要关闭的 `FILE` 对象的指针。
//! @param _flush 一个布尔标志，指示是否在关闭之前刷新流。
//! @return 成功时返回 `0`，失败时返回 `EOF`。如果 `_fp` 为 `NULL`，函数立即返回 `EOF`。
//!
//! @note 此函数使用 `fflush` 来刷新流，而在 Linux 系统上，如果刷新成功，还会使用 `fsync` 
//! 来同步文件描述符。`fsync` 确保所有修改的数据都写入到底层存储设备。
MG_CAPI_INLINE int mgu_fclose(FILE* _fp, bool _flush)
{
    int ret = 0;

    if (MEGO_SYMBOL__UNLIKELY(_fp == NULL))
        return EOF;

    if (_flush) {
#if MG_OS__LINUX_AVAIL
        int fd = fileno(_fp);
#endif
        ret = fflush(_fp);

#if MG_OS__LINUX_AVAIL
        if (ret == 0 && fd != -1)
            ret = fsync(fd);
#endif
    }
    return fclose(_fp);
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !MGU_STD_FILE_H_INCLUDED
