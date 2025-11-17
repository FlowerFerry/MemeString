
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
//! @brief File status structure.
//!
//! This structure is used to store the status information of a file, including the device ID, inode number, file mode, number of hard links, user ID, group ID, device ID (if it is a special file), file size, and the times of last access, modification, and status change.
struct mgu_stat
{
	mgu_dev_t		st_dev;   ///< Device ID.
	mgu_ino_t		st_ino;   ///< Inode number.
	uint32_t		st_mode;  ///< File mode (permissions and file type).
	int64_t			st_nlink; ///< Number of hard links.
	int64_t			st_uid;   ///< User ID.
	int64_t			st_gid;   ///< Group ID.
	mgu_dev_t		st_rdev;  ///< Device ID (if it is a special file).
	int64_t			st_size;  ///< File size (in bytes).
	mgu_timespec_t	st_atim;  ///< Last access time.
	mgu_timespec_t	st_mtim;  ///< Last modification time.
	mgu_timespec_t	st_ctim;  ///< Last status change time.
};

//! @enum mgu_stat_mode
//! @brief File mode enumeration.
//!
//! This enumeration defines various flags for file modes, used to represent file types, permissions, and special mode bits.
enum mgu_stat_mode {
	mgu_stat_mode_ifmt   = 0xF000, //!< File type mask
	mgu_stat_mode_ifsock = 0xC000, //!< socket
	mgu_stat_mode_iflink = 0xA000, //!< symbolic link
	mgu_stat_mode_ifreg  = 0x8000, //!< Regular
	mgu_stat_mode_ifblk  = 0x6000, //!< block device
	mgu_stat_mode_ifdir  = 0x4000, //!< Directory
	mgu_stat_mode_ifchr  = 0x2000, //!< Character special
	mgu_stat_mode_iffifo = 0x1000, //!< Pipe

	mgu_stat_mode_isuid = 0004000, //!< Set user ID bit
	mgu_stat_mode_isgid = 0002000, //!< Set group ID bit
	mgu_stat_mode_isvtx = 0001000, //!< Sticky bit

#if !MG_OS__WIN_AVAIL
	mgu_stat_mode_irwxu = 00700, //!< User read, write, execute permissions
	mgu_stat_mode_irusr = 00400, //!< User read permission
	mgu_stat_mode_iwusr = 00200, //!< User write permission
	mgu_stat_mode_ixusr = 00100, //!< User execute permission
	mgu_stat_mode_irwxg = 00070, //!< Group read, write, execute permissions
	mgu_stat_mode_irgrp = 00040, //!< Group read permission
	mgu_stat_mode_iwgrp = 00020, //!< Group write permission
	mgu_stat_mode_ixgrp = 00010, //!< Group execute permission
	mgu_stat_mode_irwxo = 00007, //!< Other users read, write, execute permissions
	mgu_stat_mode_iroth = 00004, //!< Other users read permission
	mgu_stat_mode_iwoth = 00002, //!< Other users write permission
	mgu_stat_mode_ixoth = 00001, //!< Other users execute permission
#endif
};

#if MG_OS__WIN_AVAIL

//! @brief Retrieve the status information of a file.
//!
//! This function retrieves the status information of the file at the specified path and stores it in the provided `mgu_stat` structure.
//!
//! @param[in] _path A pointer to the path string.
//! @param[in] _slen The length of the path string. A value less than 0 indicates that the string is NULL-terminated.
//! @param[out] _buf A pointer to the `mgu_stat` structure used to store the file's status information.
//! @return Returns 0 on success; if an error occurs, returns the corresponding error code.
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

//! @brief Retrieve the status information of a file.
//!
//! This function retrieves the status information of the file at the specified path and stores it in the provided `mgu_stat` structure.
//!
//! @param[in] _path A pointer to the path string.
//! @param[in] _slen The length of the path string. A value less than 0 indicates that the string is NULL-terminated.
//! @param[out] _buf A pointer to the `mgu_stat` structure used to store the file's status information.
//! @return Returns 0 on success; if an error occurs, returns the corresponding error code.
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
