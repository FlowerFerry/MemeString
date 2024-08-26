
#ifndef MEGO_UTIL_SYS_STAT_H_INCLUDED
#define MEGO_UTIL_SYS_STAT_H_INCLUDED

#include <mego/err/ec.h>
#include <mego/err/ec_impl.h>
#include <mego/predef/os/linux.h>
#include <mego/predef/os/windows.h>
#include <mego/util/posix/sys/types.h>
#include <mego/util/converted_native_string.h>
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

//! @struct mgu_stat
//! @brief 文件状态结构体。
//!
//! 该结构体用于存储文件的状态信息，包括设备ID、inode编号、文件模式、硬链接数量、用户ID、组ID、设备ID（如果是特殊文件）、文件大小以及最后访问、修改和状态更改的时间。
struct mgu_stat
{
	mgu_dev_t		st_dev;   ///< 设备ID。
	mgu_ino_t		st_ino;   ///< inode编号。
	uint32_t		st_mode;  ///< 文件模式（权限和文件类型）。
	int64_t			st_nlink; ///< 硬链接数量。
	int64_t			st_uid;   ///< 用户ID。
	int64_t			st_gid;   ///< 组ID。
	mgu_dev_t		st_rdev;  ///< 设备ID（如果是特殊文件）。
	int64_t			st_size;  ///< 文件大小（以字节为单位）。
	mgu_timespec_t	st_atim;  ///< 最后访问时间。
	mgu_timespec_t	st_mtim;  ///< 最后修改时间。
	mgu_timespec_t	st_ctim;  ///< 状态最后更改时间。
};

//! @enum mgu_stat_mode
//! @brief 文件模式枚举。
//!
//! 该枚举定义了文件模式的不同标志，用于表示文件类型、权限以及特殊模式位。
enum mgu_stat_mode {
	mgu_stat_mode_ifmt   = 0xF000, // File type mask
	mgu_stat_mode_ifsock = 0xC000, // socket
	mgu_stat_mode_iflink = 0xA000, // symbolic link
	mgu_stat_mode_ifreg  = 0x8000, // Regular
	mgu_stat_mode_ifblk  = 0x6000, // block device
	mgu_stat_mode_ifdir  = 0x4000, // Directory
	mgu_stat_mode_ifchr  = 0x2000, // Character special
	mgu_stat_mode_iffifo = 0x1000, // Pipe

	mgu_stat_mode_isuid = 0004000, ///< 设置用户ID位
	mgu_stat_mode_isgid = 0002000, ///< 设置组ID位
	mgu_stat_mode_isvtx = 0001000, ///< 粘着位

#if !MG_OS__WIN_AVAIL
	mgu_stat_mode_irwxu = 00700, ///< 用户读、写、执行权限
	mgu_stat_mode_irusr = 00400, ///< 用户读权限
	mgu_stat_mode_iwusr = 00200, ///< 用户写权限
	mgu_stat_mode_ixusr = 00100, ///< 用户执行权限
	mgu_stat_mode_irwxg = 00070, ///< 组读、写、执行权限
	mgu_stat_mode_irgrp = 00040, ///< 组读权限
	mgu_stat_mode_iwgrp = 00020, ///< 组写权限
	mgu_stat_mode_ixgrp = 00010, ///< 组执行权限
	mgu_stat_mode_irwxo = 00007, ///< 其他用户读、写、执行权限
	mgu_stat_mode_iroth = 00004, ///< 其他用户读权限
	mgu_stat_mode_iwoth = 00002, ///< 其他用户写权限
	mgu_stat_mode_ixoth = 00001, ///< 其他用户执行权限
#endif
};

#if MG_OS__WIN_AVAIL

//! @brief 获取文件的状态信息。
//!
//! 该函数获取指定路径的文件状态信息，并将其存储在提供的 `mgu_stat` 结构体中。
//!
//! @param[in] _path 一个指向路径字符串的指针。
//! @param[in] _slen 路径字符串的长度。小于0表示字符串以NULL结尾。
//! @param[out] _buf 用于存储文件状态信息的 `mgu_stat` 结构体指针。
//! @return 成功时返回0；如果发生错误，返回相应的错误码。
MG_CAPI_INLINE mgec_t mgu_get_w_stat(const wchar_t* _path, mmint_t _slen, struct mgu_stat* _buf)
{
	struct _stat64 buffer;
	const wchar_t* path = NULL;
	mgec_t ec = mgu_w__to_cns(_path, _slen, &path, NULL, 0);
	if (MEGO_SYMBOL__UNLIKELY(ec != 0))
		return ec;
	
	if (_wstat64(path, &buffer) != 0) {
		mgu_w__free_cns(_path, path);
		return mgec__from_posix_err(errno);
	}

	mgu_w__free_cns(_path, path);

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

//! @brief 获取文件的状态信息。
//!
//! 该函数获取指定路径的文件状态信息，并将其存储在提供的 `mgu_stat` 结构体中。
//!
//! @param[in] _path 一个指向路径字符串的指针。
//! @param[in] _slen 路径字符串的长度。小于0表示字符串以NULL结尾。
//! @param[out] _buf 用于存储文件状态信息的 `mgu_stat` 结构体指针。
//! @return 成功时返回0；如果发生错误，返回相应的错误码。
MG_CAPI_INLINE mgec_t mgu_get_stat(const char* _path, mmint_t _slen, struct mgu_stat* _buf)
{
#if MG_OS__LINUX_AVAIL
	struct stat buffer;
#endif
	mmn_char_cptr_t path = NULL;
	mmint_t path_len = 0;
	mgec_t ec = mgu__to_cns(_path, _slen, &path, &path_len, 0);
	if (MEGO_SYMBOL__UNLIKELY(ec != 0))
		return ec;
	
#if MG_OS__WIN_AVAIL
	ec = mgu_get_w_stat(path, path_len, _buf);
	mgu__free_cns(_path, path);
	return ec;
#endif 

#if MG_OS__LINUX_AVAIL
	if (stat(path, &buffer) != 0) {
		mgu__free_cns(_path, path);
		return mgec__from_posix_err(errno);
	}

	mgu__free_cns(_path, path);
	
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
