
#ifndef MGHW_DISK_H_INCLUDED
#define MGHW_DISK_H_INCLUDED

#include <meme/string.h>
#include <meme/unsafe/string_view.h>
#include <mego/predef/os/linux.h>
#include <mego/predef/os/windows.h>
#include <mego/predef/symbol/inline.h>
#include <mego/util/get_exec_path.h>
#include <mego/util/posix/sys/stat.h>
#include <mego/mem/cstr_alloc_if_no_end_zero.h>

#include <stdio.h>

#if MG_OS__WIN_AVAIL
#include <winioctl.h>
#elif MG_OS__LINUX_AVAIL
#include <sys/statvfs.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif // __cppplusplus

typedef struct mghw_harddisk_freespace
{
    uint32_t st_size;  //!< The size of the structure
    uint32_t load;     //!< The disk usage rate (percentage)
    uint64_t total;    //!< Total space
    uint64_t free;     //!< Free space
    uint64_t avail;    //!< Available space (considering permission restrictions)
} mghw_harddisk_freespace_t;

//! Get the mount point of a file based on its file path
//!
//! @param _filepath The file path
//! @param _len The length of the file path; if less than 0, the string must be '\0'-terminated
MG_CAPI_INLINE mgrc_t mghw_get_harddisk_mountpoint_by_path(const char* _filepath, size_t _len, char* _out_buf, size_t _buf_size)
{
#if MG_OS__LINUX_AVAIL

    FILE* fp = NULL;
    char line[PATH_MAX * 2] = { 0 };
    char device[PATH_MAX];
    char mount_point[PATH_MAX];
    char fs_type[PATH_MAX];
    char options[PATH_MAX];
    int dump, pass;
    mgec_t ec;
    
    struct mgu_stat file_stat;
    
    if (_buf_size > 0)
        _out_buf[0] = '\0';

    ec = mgu_get_stat(_filepath, _len, &file_stat);
    if (ec != 0)
        return ec;

    fp = fopen("/proc/mounts", "r");
    if (fp == NULL)
        return 0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (sscanf(line, "%s %s %s %s %d %d",
            device, 
            mount_point, 
            fs_type, 
            options,
            &dump, &pass) != 6)
            continue;
        struct mgu_stat mount_stat;
        if (mgu_get_stat(mount_point, strlen(mount_point), &mount_stat) != 0)
            continue;
        if (file_stat.st_dev == mount_stat.st_dev)
        {
            size_t len = strlen(mount_point);
            if (len > _buf_size - 1)
                len = _buf_size - 1;
            strncpy(_out_buf, mount_point, len);
            _out_buf[len] = '\0';
            fclose(fp);
            return (mgrc_t)len;
        }
    }

    fclose(fp);
    return 0;

#elif MG_OS__WIN_AVAIL
    char* path = NULL;
    char* pos  = NULL;
    mgec_t ec;

    ec = mgmem__cstr_alloc_if_no_end_zero(_filepath, _len, (const char**)&path, NULL, 1);
    if (ec != 0) {
        return ec;
    }

    pos = strchr(path, ':');
    if (pos != NULL) {
        size_t len = pos - path + 1;
        if (len > _buf_size - 1)
            len = _buf_size - 1;

        strncpy(_out_buf, path, len);
        _out_buf[len] = '\0';
        mgmem__free_if_ptr_not_equal(_filepath, path);
        return (int)len;
    }
    
    if (_buf_size > 0)
        _out_buf[0] = '\0';
    mgmem__free_if_ptr_not_equal(_filepath, path);
    return 0;

#else
    return MGEC__OPNOTSUPP;
#endif
}

//! Get the device path of the hard disk where the file is located
//!
//! @param _filepath The file path
//! @param _len The length of the file path; if less than 0, the string must be '\0'-terminated
MG_CAPI_INLINE mgrc_t mghw_get_harddisk_path_by_path(const char* _filepath, size_t _len, char* _out_buf, size_t _buf_size)
{
#if MG_OS__LINUX_AVAIL
    char mountpoint[PATH_MAX];
#elif MG_OS__WIN_AVAIL
    char mountpoint[MAX_PATH];
    char driveName[16] = { 0 };
    char physicalDrivePath[64] = { 0 };
    char* pos = NULL;
#endif
    mgrc_t rc;

    if (_buf_size > 0)
        _out_buf[0] = '\0';

    rc = mghw_get_harddisk_mountpoint_by_path(_filepath, _len, mountpoint, sizeof(mountpoint));
    if (rc < 0)
        return rc;

#if MG_OS__WIN_AVAIL
    pos = strchr(mountpoint, ':');
    if (pos == NULL) {
        return 0;
    }
    *pos = '\0';
    sprintf_s(driveName, sizeof(driveName), "\\\\.\\%s:", mountpoint);

    // Open a handle to the drive
    HANDLE hDrive = CreateFileA(
        driveName, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, 
        NULL, OPEN_EXISTING, 0, NULL);
    if (hDrive == INVALID_HANDLE_VALUE) {
        return mgec__from_sys_err(GetLastError());
    }

    // Get the device number for the drive
    STORAGE_DEVICE_NUMBER deviceNumber;
    DWORD bytesReturned;
    if (!DeviceIoControl(
        hDrive, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, 
        &deviceNumber, sizeof(deviceNumber), &bytesReturned, NULL)) {
        CloseHandle(hDrive);
        return mgec__from_sys_err(GetLastError());
    }
    CloseHandle(hDrive);
    
    // Get the device path for the drive
    sprintf_s(physicalDrivePath, sizeof(physicalDrivePath), 
        "\\\\.\\PhysicalDrive%d", deviceNumber.DeviceNumber);

    size_t len = strlen(physicalDrivePath);
    if (len > _buf_size - 1)
        len = _buf_size - 1;

    strncpy(_out_buf, physicalDrivePath, len);
    _out_buf[len] = '\0';

    return len;

#else
    return rc;
#endif
}

//! Get the free space information of the hard disk where the file is located
//!
//! @param _filepath The file path
//! @param _len The length of the file path; if less than 0, the string must be '\0'-terminated
//! @param _freespace Output parameter to store the hard disk space information
//! @return Returns 0 on success, -1 on failure
//! @note Depends on the libmeme_string library
MG_CAPI_INLINE int mghw_get_harddisk_freespace_by_path(
    const char* _filepath, size_t _len, mghw_harddisk_freespace_t* _freespace)
{
#if MG_OS__LINUX_AVAIL
    char mountpoint[PATH_MAX];
#elif MG_OS__WIN_AVAIL
    char mountpoint[MAX_PATH];
    ULARGE_INTEGER freeBytesAvailable;
    ULARGE_INTEGER totalNumberOfBytes;
    ULARGE_INTEGER totalNumberOfFreeBytes;
#endif
    mghw_get_harddisk_mountpoint_by_path(_filepath, _len, mountpoint, sizeof(mountpoint));
#if MG_OS__LINUX_AVAIL
    if (mountpoint[0] == '\0') {
        return -1;
    }
    
    struct statvfs buf;
    memset(&buf, 0, sizeof(buf));
    if (statvfs(mountpoint, &buf) != 0)
    {
        return -1;
    }
    
    _freespace->total = (uint64_t)buf.f_blocks * (uint64_t)buf.f_frsize;
    _freespace->free  = (uint64_t)buf.f_bfree  * (uint64_t)buf.f_frsize;
    _freespace->avail = (uint64_t)buf.f_bavail * (uint64_t)buf.f_frsize;
    if (_freespace->total > 0)
        _freespace->load  = 100 - (_freespace->free * 100 / _freespace->total);
#elif MG_OS__WIN_AVAIL

    if (GetDiskFreeSpaceExA(
        mountpoint,
        &freeBytesAvailable,
        &totalNumberOfBytes,
        &totalNumberOfFreeBytes) == 0)
    {
        return -1;
    }
    
    _freespace->total = totalNumberOfBytes.QuadPart;
    _freespace->free  = totalNumberOfFreeBytes.QuadPart;
    _freespace->avail = freeBytesAvailable.QuadPart;
    if (_freespace->total > 0)
        _freespace->load  = (uint32_t)(100 - _freespace->free * 100 / _freespace->total);

#endif
    return 0;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !MGHW_DISK_H_INCLUDED
