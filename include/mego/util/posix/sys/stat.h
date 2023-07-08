
#ifndef MEGO_UTIL_SYS_STAT_H_INCLUDED
#define MEGO_UTIL_SYS_STAT_H_INCLUDED

#include <mego/predef/os/linux.h>
#include <mego/util/posix/sys/types.h>

#if MEGO_OS__LINUX__AVAILABLE
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include <string.h>
#include <time.h>

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
	timespec		st_atim;
	timespec		st_mtim;
	timespec		st_ctim;
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

inline int mgu_get_stat(const char* _path, intptr_t _slen, struct mgu_stat* _buf);


inline int mgu_get_stat(const char* _path, intptr_t _slen, struct mgu_stat* _buf)
{
	if (_slen == 0)
		return EINVAL;
    
#if MEGO_OS__LINUX__AVAILABLE
	struct stat buffer;
#else
	struct _stat buffer;
#endif
	if (_slen < 0) {
#if MEGO_OS__LINUX__AVAILABLE
		if (stat(_path, &buffer) != 0) {
#else
		if (_stat(_path, &buffer) != 0) {
#endif
			return errno;
		}
	}
	else {
		char* p = (char*)malloc(_slen + 1);
        if (p == NULL)
            return ENOMEM;
        memcpy(p, _path, _slen);
        p[_slen] = 0;

#if MEGO_OS__LINUX__AVAILABLE
        if (stat(p, &buffer) != 0) {
#else
        if (_stat(p, &buffer) != 0) {
#endif
            free(p);
            return errno;
        }
        free(p);
	}
    
#ifdef st_atime
	_buf->st_atime = buffer.st_atime;
#else
	_buf->st_atim.tv_sec = buffer.st_atime;
	_buf->st_atim.tv_nsec = 0;
#endif

#ifdef st_ctime
	_buf->st_ctime = buffer.st_ctime;
#else
	_buf->st_ctim.tv_sec = buffer.st_ctime;
	_buf->st_ctim.tv_nsec = 0;
#endif

#ifdef st_mtime	
	_buf->st_mtime = buffer.st_mtime;
#else
	_buf->st_mtim.tv_sec = buffer.st_mtime;
	_buf->st_mtim.tv_nsec = 0;
#endif 

	_buf->st_dev = buffer.st_dev;
	_buf->st_gid = buffer.st_gid;
	_buf->st_ino = buffer.st_ino;
	_buf->st_mode = buffer.st_mode;
	_buf->st_nlink = buffer.st_nlink;
	_buf->st_rdev = buffer.st_rdev;
	_buf->st_size = buffer.st_size;
	_buf->st_uid = buffer.st_uid;
	return 0;
}

#ifdef __cplusplus
}
#endif

#endif // MEGO_UTIL_SYS_STAT_H_INCLUDED
