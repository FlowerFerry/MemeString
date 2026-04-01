
#ifndef MEGO_FS_FD_H_INCLUDED
#define MEGO_FS_FD_H_INCLUDED

#include <mego/predef/os/windows.h>
#include <mego/predef/symbol/inline.h>

#include <errno.h>

#if MG_OS__WIN_AVAIL
#include <io.h>
#include <fcntl.h>   // 包含 _O_RDONLY
#else
#include <unistd.h>
#include <fcntl.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//! @brief Check if the maximum number of open file descriptors has been reached.
MG_CAPI_INLINE int mgfs__is_max_files_reached() 
{
#if MG_OS__WIN_AVAIL
    int fd = _open("NUL", _O_RDONLY);
    if (fd < 0) {
        if (errno == EMFILE || errno == ENFILE) 
            return 1;
        return 0;
    }
    
    _close(fd);
    return 0;
#else
    int fd = open("/dev/null", O_RDONLY);
    if (fd < 0) {
        if (errno == EMFILE || errno == ENFILE) 
            return 1;
        return 0;
    }

    close(fd);
    return 0;
#endif
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !MEGO_FS_FD_H_INCLUDED
