
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

//! @brief 检查指定路径是否存在并在需要时创建目录。
//!
//! 该函数检查给定路径是否存在，并在路径不存在且需要创建时，创建所需的目录。
//!
//! @param[in] _path 一个指向路径字符串的指针。
//! @param[in] _slen 路径字符串的长度。
//! @param[in] _create_if_needed 如果为非零值且路径不存在，则创建目录。
//! @param[in] _path_allow_modified 如果为非零值，则允许修改传入的路径字符串。
//! @return 成功时返回0；如果发生错误，返回相应的错误码。
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
        
        for (size_t index = 1; index < path_len; ++index) 
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
            return mgec__from_posix_err(eno);
        }
        if (!MGU__S_ISDIR(st.st_mode)) {
            mgu_w__free_cns(_path, path);
            return MGEC__NOTDIR;
        }
        mgu_w__free_cns(_path, path);
        return 0;
    }
}

//! @brief 检查给定路径是否存在且是一个目录。
//!
//! 该函数检查指定路径是否存在，并且是否是一个目录。
//! 
//! @param[in] _path 一个指向路径字符串的指针。
//! @param[in] _slen 路径字符串的长度。小于0表示字符串以NULL结尾。
//! @return 如果路径存在且是目录，返回1；如果路径不存在或不是目录，返回0；如果发生错误，返回相应的错误码。
MG_CAPI_INLINE mgrc_t mgfs__is_exist_w_dir(const wchar_t *_path, mmint_t _slen)
{
    struct mgu_stat st;
    int eno = mgu_get_w_stat(_path, _slen, &st);
    if (eno != 0)
        return mgec__from_posix_err(eno);
    
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
    
    _snwprintf(temp, sizeof(temp), 
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

//! @brief 检查指定路径是否存在并在需要时创建目录。
//!
//! 该函数检查给定路径是否存在，并在路径不存在且需要创建时，创建所需的目录。
//!
//! @param[in] _path 一个指向路径字符串的指针。
//! @param[in] _slen 路径字符串的长度。
//! @param[in] _create_if_needed 如果为非零值且路径不存在，则创建目录。
//! @param[in] _path_allow_modified 如果为非零值，则允许修改传入的路径字符串。
//! @return 成功时返回0；如果发生错误，返回相应的错误码。
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
                if (mkdir(p, 0777) != 0) {
                    if (errno != EEXIST) {
                        p[index] = ch;
                        mgu__free_cns(_path, path);
                        return mgec__from_posix_err(errno);
                    }
                }
                p[index] = ch;
            }
        }
        if (mkdir(path, 0777) != 0) {
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
            return mgec__from_posix_err(eno);
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

//! @brief 检查给定路径是否存在且是一个目录。
//!
//! 该函数检查指定路径是否存在，并且是否是一个目录。
//! 
//! @param[in] _path 一个指向路径字符串的指针。
//! @param[in] _slen 路径字符串的长度。小于0表示字符串以NULL结尾。
//! @return 如果路径存在且是目录，返回1；如果路径不存在或不是目录，返回0；如果发生错误，返回相应的错误码。
MG_CAPI_INLINE mgrc_t mgfs__is_exist_dir(const char *_path, mmint_t _slen)
{
    struct mgu_stat st;
    int eno = mgu_get_stat(_path, _slen, &st);
    if (eno != 0)
        return mgec__from_posix_err(eno);
    
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
