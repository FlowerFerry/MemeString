
#ifndef MEGO_UTIL_SYS_STAT_H_INCLUDED
#define MEGO_UTIL_SYS_STAT_H_INCLUDED

#include <mego/err/ec.h>
#include <mego/predef/os/linux.h>
#include <mego/util/posix/sys/types.h>
#include <meme/utf/converter.h>

#include <sys/stat.h>
#if MEGO_OS__LINUX__AVAILABLE
#include <sys/types.h>
#endif

#include <string.h>
#include <mego/util/std/time.h>

#ifdef __cplusplus
extern "C" {
#endif

struct mgu_stat
{
	mgu_dev_t		st_dev;
	mgu_ino_t		st_ino;
	uint32_t		st_mode;
	int64_t			st_nlink;
	int64_t			st_uid;
	int64_t			st_gid;
	mgu_dev_t		st_rdev;
	int64_t			st_size;
	mgu_timespec_t	st_atim;
	mgu_timespec_t	st_mtim;
	mgu_timespec_t	st_ctim;
};

enum mgu_stat_mode {
	mgu_stat_mode_ifmt   = 0xF000, // File type mask
	mgu_stat_mode_ifsock = 0xC000, // socket
	mgu_stat_mode_iflink = 0xA000, // symbolic link
	mgu_stat_mode_ifreg  = 0x8000, // Regular
	mgu_stat_mode_ifblk  = 0x6000, // block device
	mgu_stat_mode_ifdir  = 0x4000, // Directory
	mgu_stat_mode_ifchr  = 0x2000, // Character special
	mgu_stat_mode_iffifo = 0x1000, // Pipe

	mgu_stat_mode_isuid = 0004000,
	mgu_stat_mode_isgid = 0002000,
	mgu_stat_mode_isvtx = 0001000,

	mgu_stat_mode_irwxu = 00700,
	mgu_stat_mode_irusr = 00400,
	mgu_stat_mode_iwusr = 00200,
	mgu_stat_mode_ixusr = 00100,
	mgu_stat_mode_irwxg = 00070,
	mgu_stat_mode_irgrp = 00040,
	mgu_stat_mode_iwgrp = 00020,
	mgu_stat_mode_ixgrp = 00010,
	mgu_stat_mode_irwxo = 00007,
	mgu_stat_mode_iroth = 00004,
	mgu_stat_mode_iwoth = 00002,
	mgu_stat_mode_ixoth = 00001,
};

int mgu_get_stat(const char* _path, intptr_t _slen, struct mgu_stat* _buf);


inline int mgu_get_stat(const char* _path, intptr_t _slen, struct mgu_stat* _buf)
{
#if MG_OS__LINUX_AVAIL
    char* path = NULL;
	struct stat buffer;
#else
    mmint_t u16len = 0;
	wchar_t* path = NULL;
	struct _stat64 buffer;
#endif

	if (_slen < 0)
		_slen = strlen(_path);
    
#if MG_OS__LINUX_AVAIL
	if (_path[_slen] == '\0')
		path = (char*)_path;
	else {
		path = (char*)malloc(sizeof(char) * (_slen + 1));
		if (path == NULL)
			return ENOMEM;
		memcpy(path, _path, _slen);
		path[_slen] = '\0';
	}
	if (stat(path, &buffer) != 0) {
		if (path != _path)
			free(path);
		return errno;
	}

	if (path != _path)
		free(path);
#else
    u16len = mmutf_char_size_u16from8((const uint8_t*)_path, _slen);
    if (u16len < 1)
        return EINVAL;
		
    path = (wchar_t*)malloc(sizeof(wchar_t) * (u16len + 1));
    if (!path)
        return ENOMEM;
	if (mmutf_convert_u8to16((const uint8_t*)_path, _slen, (uint16_t*)path) == 0)
    {
        free(path);
        return EINVAL;
    }
    path[u16len] = L'\0';

	if (_wstat64(path, &buffer) != 0) {
		free(path);
		return errno;
	}

	free(path);
#endif

#ifdef st_atime
	_buf->st_atime = buffer.st_atime;
#else
	_buf->st_atim.tv_sec  = buffer.st_atime;
	_buf->st_atim.tv_nsec = 0;
#endif

#ifdef st_ctime
	_buf->st_ctime = buffer.st_ctime;
#else
	_buf->st_ctim.tv_sec  = buffer.st_ctime;
	_buf->st_ctim.tv_nsec = 0;
#endif

#ifdef st_mtime	
	_buf->st_mtime = buffer.st_mtime;
#else
	_buf->st_mtim.tv_sec  = buffer.st_mtime;
	_buf->st_mtim.tv_nsec = 0;
#endif 

	_buf->st_dev   = buffer.st_dev;
	_buf->st_gid   = buffer.st_gid;
	_buf->st_ino   = buffer.st_ino;
	_buf->st_mode  = buffer.st_mode;
	_buf->st_nlink = buffer.st_nlink;
	_buf->st_rdev  = buffer.st_rdev;
	_buf->st_size  = buffer.st_size;
	_buf->st_uid   = buffer.st_uid;
	return 0;
}

#ifdef __cplusplus
}
#endif

#endif // MEGO_UTIL_SYS_STAT_H_INCLUDED
