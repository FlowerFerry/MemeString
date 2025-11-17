
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
    mgec_t  err = 0;
    errno_t eno = 0;
    
    err = mgu_w__to_cns(_path, _slen, &path, NULL, 0);
    if (MEGO_SYMBOL__UNLIKELY(err != 0))
        return NULL;

    err = mgu_w__to_cns(_mode, _mlen, &mode, NULL, 0);
    if (MEGO_SYMBOL__UNLIKELY(err != 0)) {
        mgu_w__free_cns(_path, path);
        return NULL;
    }

    eno = _wfopen_s(&fp, path, mode);
    mgu_w__free_cns(_path, path);
    mgu_w__free_cns(_mode, mode);

    if (MEGO_SYMBOL__UNLIKELY(eno != 0))
    {
        errno = eno;
        return NULL;
    }    
    return fp;
}
#endif

//! @brief Open a file and return a file pointer.
//!
//! This function opens a file at the specified path and operates on it in the specified mode. If the length of the path or mode is not zero, the corresponding length parameters will be considered.
//!
//! @param _path The path of the file to be opened.
//! @param _slen The length of the path string. If less than zero, the path is assumed to be a null-terminated string.
//! @param _mode The mode in which the file is to be opened (e.g., "r", "w", "a", etc.).
//! @param _mlen The length of the mode string. If less than zero, the mode is assumed to be a null-terminated string.
//! @return On success, returns a `FILE` pointer to the file. On failure, returns `NULL`.
//!
//! @note This function is primarily designed to handle cases where the lengths of the path and mode strings are variable. It is suitable for scenarios requiring precise control over string lengths.
MG_CAPI_INLINE FILE* mgu_fopen(
    const char* _path, mmint_t _slen, const char* _mode, mmint_t _mlen)
{
    FILE* fp = NULL;
    mmn_char_cptr_t path = NULL;
    mmn_char_cptr_t mode = NULL;
    mgec_t  err = 0;
#if MG_OS__WIN_AVAIL
    errno_t eno = 0;
#endif
    
    err = mgu__to_cns(_path, _slen, &path, NULL, 0);
    if (MEGO_SYMBOL__UNLIKELY(err != 0))
        return NULL;

    err = mgu__to_cns(_mode, _mlen, &mode, NULL, 0);
    if (MEGO_SYMBOL__UNLIKELY(err != 0)) {
        mgu__free_cns(_path, path);
        return NULL;
    }

#if MG_OS__WIN_AVAIL
    eno = _wfopen_s(&fp, path, mode);
#else
    fp = fopen(path, mode);
#endif
    mgu__free_cns(_path, path);
    mgu__free_cns(_mode, mode);
#if MG_OS__WIN_AVAIL
    if (MEGO_SYMBOL__UNLIKELY(eno != 0))
    {
        errno = eno;
        return NULL;
    }
#endif
    return fp;
}

//! @brief Close a file stream with an optional flush operation.
//!
//! This function closes the given file stream. If the `_flush` parameter is `true`, it flushes the stream before closing.
//! On Linux systems, if the flush is successful, it also synchronizes the file descriptor using `fsync`.
//!
//! @param _fp A pointer to the `FILE` object to be closed.
//! @param _flush A boolean flag indicating whether to flush the stream before closing.
//! @return Returns `0` on success, `EOF` on failure. If `_fp` is `NULL`, the function returns `EOF` immediately.
//!
//! @note This function uses `fflush` to flush the stream, and on Linux systems, if the flush is successful, it also uses `fsync` 
//! to synchronize the file descriptor. `fsync` ensures that all modified data is written to the underlying storage device.
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
        if (fd != -1)
            ret = fsync(fd);
#endif
    }
    return fclose(_fp);
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !MGU_STD_FILE_H_INCLUDED
