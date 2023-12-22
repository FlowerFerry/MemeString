
#ifndef MEGO_UTIL_SYS_STAT_H_INCLUDED
#define MEGO_UTIL_SYS_STAT_H_INCLUDED

#include <mego/err/ec.h>
#include <mego/predef/os/linux.h>
#include <mego/util/posix/sys/types.h>
#include <mego/util/conv_to_native_c_str.h>
#include <meme/utf/converter.h>
#include <meme/native.h>

#include <sys/stat.h>
#if MEGO_OS__LINUX__AVAILABLE
#include <sys/types.h>
#endif

#include <string.h>
#include <mego/util/std/time.h>

#if MG_OS__WIN_AVAIL
#  ifndef __MGU__S_ISTYPE
#    define __MGU__S_ISTYPE(mode, mask)  (((mode) & _S_IFMT) == (mask))
#    define MGU__S_ISREG(mode) __MGU__S_ISTYPE((mode), _S_IFREG)
#    define MGU__S_ISDIR(mode) __MGU__S_ISTYPE((mode), _S_IFDIR)
#  endif
#else
#  ifndef MGU__S_ISREG
#    define MGU__S_ISREG(mode)  S_ISREG(mode)
#    define MGU__S_ISDIR(mode)  S_ISDIR(mode)
#  endif
#endif

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

#if MG_OS__WIN_AVAIL
int mgu_get_w_stat(const wchar_t* _path, intptr_t _slen, struct mgu_stat* _buf);
#endif

int mgu_get_stat(const char* _path, intptr_t _slen, struct mgu_stat* _buf);


#if MG_OS__WIN_AVAIL
inline int mgu_get_w_stat(const wchar_t* _path, intptr_t _slen, struct mgu_stat* _buf)
{
	struct _stat64 buffer;
	wchar_t* path = NULL;
	mgec_t ec = mgu_w__conv_to_native_c_str(_path, _slen, &path, NULL, 0);
	if (MEGO_SYMBOL__UNLIKELY(ec != 0))
		return ec;
	
	if (_wstat64(path, &buffer) != 0) {
		mgu_w__free_native_c_str(_path, path);
		return errno;
	}

	mgu_w__free_native_c_str(_path, path);

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
};
#endif

inline int mgu_get_stat(const char* _path, intptr_t _slen, struct mgu_stat* _buf)
{
#if MG_OS__LINUX_AVAIL
	struct stat buffer;
#endif
	mmn_char_cptr_t path = NULL;
	size_t path_len = 0;
	mgec_t ec = mgu__conv_to_native_c_str(_path, _slen, &path, &path_len, 0);
	if (MEGO_SYMBOL__UNLIKELY(ec != 0))
		return ec;
	
#if MG_OS__WIN_AVAIL
	ec = mgu_get_w_stat(path, path_len, _buf);
	mgu__free_native_c_str(_path, path);
	return ec;
#endif 

#if MG_OS__LINUX_AVAIL
	if (stat(path, &buffer) != 0) {
		mgu__free_native_c_str(_path, path);
		return errno;
	}

	mgu__free_native_c_str(_path, path);

	
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
#endif
}

#ifdef __cplusplus
}
#endif

#endif // MEGO_UTIL_SYS_STAT_H_INCLUDED
