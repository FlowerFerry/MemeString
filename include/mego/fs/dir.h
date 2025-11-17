
#ifndef MEGO_FS_DIR_H_INCLUDED
#define MEGO_FS_DIR_H_INCLUDED

#include <mego/err/ec_impl.h>
#include <mego/predef/symbol/likely.h>
#include <mego/predef/os/windows.h>
#include <mego/util/converted_native_string.h>
#include <mego/util/posix/sys/stat.h>
#include <mego/predef/symbol/inline.h>

#include <string.h>
#include <stdbool.h>
#if MG_OS__WIN_AVAIL
#include <direct.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#if MG_OS__WIN_AVAIL

//! @brief Check if the specified path exists and create directories if needed.
//!
//! This function checks whether the given path exists. If the path does not exist and directory creation is needed, it creates the necessary directories.
//!
//! @param[in] _path A pointer to the path string.
//! @param[in] _slen The length of the path string.
//! @param[in] _create_if_needed If non-zero and the path does not exist, the function creates the directories.
//! @param[in] _path_allow_modified If non-zero, the function allows modifications to the input path string.
//! @return Returns 0 on success; if an error occurs, it returns the corresponding error code.
MG_CAPI_INLINE mgec_t mgfs__check_and_create_w_dirs_if_needed(
    const wchar_t *_path, mmint_t _slen, int _create_if_needed, int _path_allow_modified)
{
    mmint_t path_len = 0;
    const wchar_t *path = NULL;
    mgec_t ec = 0;
    if (MEGO_SYMBOL__UNLIKELY(_path == NULL))
        return MGEC__INVAL;
    
    ec = mgu_w__to_cns(
        _path, _slen, &path, &path_len, _create_if_needed && !_path_allow_modified ? 1 : 0);
    if (MEGO_SYMBOL__UNLIKELY(ec != 0))
        return ec;

    if (_create_if_needed) {
        wchar_t *p;
        if (_path_allow_modified)
            p = (wchar_t*)_path;
        else
            p = (wchar_t*)path;
        
        for (mmint_t index = 1; index < path_len; ++index) 
        {
            if (p[index] == L'\\' || p[index] == L'/') 
            {
                wchar_t ch = p[index];
                p[index] = L'\0';
                if (_wmkdir(p) != 0) {
                    if (errno != EEXIST) {
                        p[index] = ch;
                        mgu_w__free_cns(_path, path);
                        return mgec__from_posix_err(errno);
                    }
                }
                p[index] = ch;
            }
        }
        if (_wmkdir(path) != 0) {
            if (errno != EEXIST) {
                mgu_w__free_cns(_path, path);
                return mgec__from_posix_err(errno);
            }
        }

        mgu_w__free_cns(_path, path);
        return 0;
    }
    else {
        struct mgu_stat st;
        int eno = mgu_get_w_stat(path, path_len, &st);
        if (eno != 0) {
            mgu_w__free_cns(_path, path);
            return eno;
        }
        if (!MGU__S_ISDIR(st.st_mode)) {
            mgu_w__free_cns(_path, path);
            return MGEC__NOTDIR;
        }
        mgu_w__free_cns(_path, path);
        return 0;
    }
}

//! @brief Check if the given path exists and is a directory.
//!
//! This function checks whether the specified path exists and whether it is a directory.
//!
//! @param[in] _path A pointer to the path string.
//! @param[in] _slen The length of the path string. A value less than 0 indicates that the string is NULL-terminated.
//! @return Returns 1 if the path exists and is a directory; returns 0 if the path does not exist or is not a directory; returns the corresponding error code if an error occurs.
MG_CAPI_INLINE mgrc_t mgfs__is_exist_w_dir(const wchar_t *_path, mmint_t _slen)
{
    struct mgu_stat st;
    int eno = mgu_get_w_stat(_path, _slen, &st);
    if (eno != 0)
        return eno;
    
    return MGU__S_ISDIR(st.st_mode) ? 1 : 0;
}

MG_CAPI_INLINE bool mgfs__is_w_dir_writable(const wchar_t *_path, mmint_t _slen)
{    
    mmint_t path_len = 0;
    const wchar_t *path = NULL;
    DWORD attr;
    wchar_t temp[MAX_PATH];
    HANDLE hFile;

    mgec_t ec = mgu_w__to_cns(_path, _slen, &path, &path_len, 0);
    if (MEGO_SYMBOL__UNLIKELY(ec != 0))
        return false;
    
    if (path_len >= MAX_PATH) {
        mgu_w__free_cns(_path, path);
        return false;
    }

    attr = GetFileAttributesW(path);
    if (attr == INVALID_FILE_ATTRIBUTES) {
        mgu_w__free_cns(_path, path);
        return false;
    }

    if ((attr & FILE_ATTRIBUTE_DIRECTORY) == 0) 
    {
        mgu_w__free_cns(_path, path);
        return false;
    }
    
    _snwprintf(temp, sizeof(temp) / sizeof(temp[0]), 
        L"%s\\~%s.%d.%s", path, L"test", rand(), L"temp");
    temp[MAX_PATH - 1] = L'\0';
    mgu_w__free_cns(_path, path);
    
    hFile = CreateFileW(
        temp, GENERIC_WRITE, 0, NULL, CREATE_NEW, 
        FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, NULL);
    if (hFile == INVALID_HANDLE_VALUE) 
    {
        return false;
    }

    CloseHandle(hFile);
    return true;

}

#endif

//! @brief Check if the specified path exists and create directories if needed.
//!
//! This function checks whether the given path exists. If the path does not exist and directory creation is required, it creates the necessary directories.
//!
//! @param[in] _path A pointer to the path string.
//! @param[in] _slen The length of the path string.
//! @param[in] _create_if_needed If non-zero and the path does not exist, the function creates the directories.
//! @param[in] _path_allow_modified If non-zero, the function allows modifications to the provided path string.
//! @return Returns 0 on success; if an error occurs, it returns the corresponding error code.
MG_CAPI_INLINE mgec_t mgfs__check_and_create_dirs_if_needed(
    const char *_path, mmint_t _slen, int _create_if_needed, int _path_allow_modified) 
{
    mgec_t ec = 0;
    mmn_char_cptr_t path = NULL;
    mmint_t path_len = 0;

    if (MEGO_SYMBOL__UNLIKELY(_path == NULL))
        return MGEC__INVAL;

    ec = mgu__to_cns(
        _path, _slen, &path, &path_len, _create_if_needed && !_path_allow_modified ? 1 : 0);
    if (MEGO_SYMBOL__UNLIKELY(ec != 0))
        return ec;
    
#if MG_OS__WIN_AVAIL
    ec = mgfs__check_and_create_w_dirs_if_needed(
        (const wchar_t*)path, path_len, _create_if_needed, 1);
    mgu__free_cns(_path, path);
    return ec;
#else
    if (_create_if_needed) {
        char *p;
        if (_path_allow_modified)
            p = (char*)_path;
        else
            p = (char*)path;
        
        for (size_t index = 1; index < path_len; ++index) 
        {
            if (p[index] == '\\' || p[index] == '/') 
            {
                char ch = p[index];
                p[index] = '\0';
                if (mkdir(p, 0755) != 0) {
                    if (errno != EEXIST) {
                        p[index] = ch;
                        mgu__free_cns(_path, path);
                        return mgec__from_posix_err(errno);
                    }
                }
                p[index] = ch;
            }
        }
        if (mkdir(path, 0755) != 0) {
            if (errno != EEXIST) {
                mgu__free_cns(_path, path);
                return mgec__from_posix_err(errno);
            }
        }

        mgu__free_cns(_path, path);
        return 0;
    }
    else {
        struct mgu_stat st;
        int eno = mgu_get_stat(path, path_len, &st);
        if (eno != 0) {
            mgu__free_cns(_path, path);
            return eno;
        }
        if (!MGU__S_ISDIR(st.st_mode)) {
            mgu__free_cns(_path, path);
            return MGEC__NOTDIR;
        }
        mgu__free_cns(_path, path);
        return 0;
    }
#endif 
}

//! @brief Check whether the given path exists and is a directory.
//!
//! This function checks if the specified path exists and determines whether it is a directory.
//!
//! @param[in] _path A pointer to the path string.
//! @param[in] _slen The length of the path string. A value less than 0 indicates that the string is NULL-terminated.
//! @return Returns 1 if the path exists and is a directory; returns 0 if the path does not exist or is not a directory; returns the corresponding error code if an error occurs.
MG_CAPI_INLINE mgrc_t mgfs__is_exist_dir(const char *_path, mmint_t _slen)
{
    struct mgu_stat st;
    int eno = mgu_get_stat(_path, _slen, &st);
    if (eno != 0)
        return eno;
    
    return MGU__S_ISDIR(st.st_mode) ? 1 : 0;
}

MG_CAPI_INLINE bool mgfs__is_dir_writable(const char *_path, mmint_t _slen)
{
#if MG_OS__WIN_AVAIL
    mmn_char_cptr_t path = NULL;
    mmint_t path_len = 0;
    mgec_t ec = 0;
    bool ret;

    ec = mgu__to_cns(
        _path, _slen, &path, &path_len, 0);
    if (MEGO_SYMBOL__UNLIKELY(ec != 0))
        return false;

    ret = mgfs__is_w_dir_writable((const wchar_t*)path, path_len);
    mgu__free_cns(_path, path);
    return ret;
#else
    uid_t uid;
    gid_t gid;
    struct mgu_stat st;
    int eno = mgu_get_stat(_path, _slen, &st);
    if (eno != 0)
        return false;
    
    if (!MGU__S_ISDIR(st.st_mode))
        return false;
    
    uid = getuid();
    gid = getgid();

    if (st.st_uid == uid) 
    {
        if (st.st_mode & mgu_stat_mode_iwusr)
            return true;
    }
    else if (st.st_gid == gid) 
    {
        if (st.st_mode & mgu_stat_mode_iwgrp)
            return true;
    }
    else {
        if (st.st_mode & mgu_stat_mode_iwoth)
            return true;
    }

    return false;
#endif
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !MEGO_FS_DIR_H_INCLUDED
