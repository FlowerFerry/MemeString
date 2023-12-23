
#ifndef MEGO_FS_DIR_H_INCLUDED
#define MEGO_FS_DIR_H_INCLUDED

#include <mego/err/ec_impl.h>
#include <mego/predef/symbol/likely.h>
#include <mego/predef/os/windows.h>
#include <mego/util/converted_native_string.h>
#include <mego/util/posix/sys/stat.h>

#include <string.h>
#include <direct.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#if MG_OS__WIN_AVAIL
mgec_t mgfs__check_and_create_w_dirs_if_needed(
    const wchar_t *_path, size_t _slen, int _create_if_needed, int _path_allow_modified);
#endif

mgec_t mgfs__check_and_create_dirs_if_needed(
    const char *_path, size_t _slen, int _create_if_needed, int _path_allow_modified);

#if MG_OS__WIN_AVAIL
inline mgec_t mgfs__check_and_create_w_dirs_if_needed(
    const wchar_t *_path, size_t _slen, int _create_if_needed, int _path_allow_modified)
{
    size_t path_len = 0;
    wchar_t *path = NULL;
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
#endif

inline mgec_t mgfs__check_and_create_dirs_if_needed(
    const char *_path, size_t _slen, int _create_if_needed, int _path_allow_modified) 
{
    mgec_t ec = 0;
    mmn_char_cptr_t path = NULL;
    size_t path_len = 0;

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
                if (mkdir(p) != 0) {
                    if (errno != EEXIST) {
                        p[index] = ch;
                        mgu__free_cns(_path, path);
                        return mgec__from_posix_err(errno);
                    }
                }
                p[index] = ch;
            }
        }
        if (mkdir(path) != 0) {
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

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !MEGO_FS_DIR_H_INCLUDED
