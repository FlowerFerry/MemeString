
#ifndef MGHW_DISK_H_INCLUDED
#define MGHW_DISK_H_INCLUDED

#include <meme/string.h>
#include <meme/unsafe/string_view.h>
#include <mego/predef/os/linux.h>
#include <mego/predef/os/windows.h>
#include <mego/predef/symbol/inline.h>
#include <mego/util/get_exec_path.h>
#include <mego/util/posix/sys/stat.h>

#include <stdio.h>

#if MG_OS__WIN_AVAIL
#include <winioctl.h>
#elif MG_OS__LINUX_AVAIL
#include <sys/statvfs.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif // __cppplusplus

struct mghw_harddisk_freespace
{
    uint32_t st_size;
    uint32_t load;
    uint64_t total;
    uint64_t free;
    uint64_t avail;
};

// mmstrstk_t mghw_get_harddisk_path_by_path(const char* _filepath, size_t _len);

// mmstrstk_t mghw_get_harddisk_mountpoint_by_path(const char* _filepath, size_t _len);

// int mghw_get_harddisk_freespace_by_path(
//     const char* _filepath, size_t _len, mghw_harddisk_freespace* _freespace);


MG_CAPI_INLINE mmstrstk_t mghw_get_harddisk_path_by_path(const char* _filepath, size_t _len)
{
    mmstrstk_t s;
#if MEGO_OS__LINUX__AVAILABLE
    struct mgu_stat file_stat;

    mmstrstk_init(&s, MMSTR__OBJ_SIZE);

    if (mgu_get_stat(_filepath, _len, &file_stat) != 0)
        return s;
    
    FILE* fp = fopen("/proc/mounts", "r");
    if (fp == NULL)
        return s;

    char line[PATH_MAX * 2] = { 0 };
    char device[PATH_MAX];
    char mount_point[PATH_MAX];
    char fs_type[PATH_MAX];
    char options[PATH_MAX];
    int dump, pass;
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
            mmstr_assign_by_utf8((mmstr_t)&s, 
                (const uint8_t*)device, strlen(device));
            break;
        }
    }
    fclose(fp);

    return s;
#elif MEGO_OS__WINDOWS__AVAILABLE
    mmstrstk_t path;
    mmint_t pos = -1;
    // The format for the drive name is "\\.\X:"
    char driveName[16] = { 0 };
    char physicalDrivePath[64] = { 0 };
    mmstrstk_init(&s, MMSTR__OBJ_SIZE);
    MemeStringViewUnsafeStack_init(&path, MMSTR__OBJ_SIZE, (const uint8_t*)_filepath, _len);
    pos = MemeString_indexOfWithUtf8bytes(
        (mmstr_t)&path, 0, (const uint8_t*)":", -1, MemeFlag_AllSensitive);
    if (pos == -1) {
        mmstrstk_uninit(&path, MMSTR__OBJ_SIZE);
        return s;
    }
    else {
        char temp[8] = { 0 };
        strncpy_s(temp, sizeof(temp), _filepath, pos);
        sprintf_s(driveName, sizeof(driveName), "\\\\.\\%s:", temp);
    }

    // Open a handle to the drive
    HANDLE hDrive = CreateFileA(
        driveName, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, 
        NULL, OPEN_EXISTING, 0, NULL);
    if (hDrive == INVALID_HANDLE_VALUE) {
        mmstrstk_uninit(&path, MMSTR__OBJ_SIZE);
        return s;
    }

    // Get the device number for the drive
    STORAGE_DEVICE_NUMBER deviceNumber;
    DWORD bytesReturned;
    if (!DeviceIoControl(
        hDrive, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, 
        &deviceNumber, sizeof(deviceNumber), &bytesReturned, NULL)) {
        CloseHandle(hDrive);
        mmstrstk_uninit(&path, MMSTR__OBJ_SIZE);
        return s;
    }
    
    // Get the device path for the drive
    sprintf_s(physicalDrivePath, sizeof(physicalDrivePath), 
        "\\\\.\\PhysicalDrive%d", deviceNumber.DeviceNumber);
    CloseHandle(hDrive);

    mmstr_assign_by_utf8((mmstr_t)&s, (const uint8_t*)physicalDrivePath, strlen(physicalDrivePath));

    mmstrstk_uninit(&path, MMSTR__OBJ_SIZE);
    return s;
#else // MEGO_OS__Linux__AVAILABLE
    mmstrstk_init(&s, MMSTR__OBJ_SIZE);
    return s;
#endif // MEGO_OS__Linux__AVAILABLE
}

MG_CAPI_INLINE mmstrstk_t mghw_get_harddisk_mountpoint_by_path(const char* _filepath, size_t _len)
{
    mmstrstk_t s;
#if MEGO_OS__LINUX__AVAILABLE
    struct mgu_stat file_stat;

    mmstrstk_init(&s, MMSTR__OBJ_SIZE);

    if (mgu_get_stat(_filepath, _len, &file_stat) != 0)
        return s;

    FILE* fp = fopen("/proc/mounts", "r");
    if (fp == NULL)
        return s;

    char line[PATH_MAX * 2] = { 0 };
    char device[PATH_MAX];
    char mount_point[PATH_MAX];
    char fs_type[PATH_MAX];
    char options[PATH_MAX];
    int dump, pass;
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
            mmstr_assign_by_utf8((mmstr_t)&s, 
                (const uint8_t*)mount_point, strlen(mount_point));
            break;
        }
    }
    fclose(fp);

    return s;
#elif MEGO_OS__WINDOWS__AVAILABLE
    mmstrstk_t path;
    MemeInteger_t pos = -1;
    // The format for the drive name is "\\.\X:"
    char driveName[16] = { 0 };
    char physicalDrivePath[64] = { 0 };
    mmstrstk_init(&s, MMSTR__OBJ_SIZE);
    MemeStringViewUnsafeStack_init(&path, MMSTR__OBJ_SIZE, (const uint8_t*)_filepath, _len);
    pos = MemeString_indexOfWithUtf8bytes(
        (mmstr_t)&path, 0, (const uint8_t*)":", -1, MemeFlag_AllSensitive);
    if (pos == -1) {
    }
    else {
        mmstr_assign_by_utf8((mmstr_t)&s, (const uint8_t*)_filepath, pos + 1);
    }

    mmstrstk_uninit(&path, MMSTR__OBJ_SIZE);
    return s;
#else // MEGO_OS__Linux__AVAILABLE
    mmstrstk_init(&s, MMSTR__OBJ_SIZE);
    return s;
#endif // MEGO_OS__Linux__AVAILABLE
}

MG_CAPI_INLINE int mghw_get_harddisk_freespace_by_path(
    const char* _filepath, size_t _len, mghw_harddisk_freespace* _freespace)
{
    mmstrstk_t mountpoint = mghw_get_harddisk_mountpoint_by_path(_filepath, _len);
#if MEGO_OS__LINUX__AVAILABLE
    if (MemeString_isEmpty((mmstr_t)&mountpoint))
    {
        mmstrstk_uninit(&mountpoint, MMSTR__OBJ_SIZE);
        return -1;
    }
    
    struct statvfs buf;
    if (statvfs(MemeString_cStr((mmstr_t)&mountpoint), &buf) != 0)
    {
        mmstrstk_uninit(&mountpoint, MMSTR__OBJ_SIZE);
        return -1;
    }
    
    _freespace->total = buf.f_blocks * buf.f_frsize;
    _freespace->free  = buf.f_bfree * buf.f_frsize;
    _freespace->avail = buf.f_bavail * buf.f_frsize;
    _freespace->load  = _freespace->free * 100 / _freespace->total;
#elif MEGO_OS__WINDOWS__AVAILABLE
    ULARGE_INTEGER freeBytesAvailable;
    ULARGE_INTEGER totalNumberOfBytes;
    ULARGE_INTEGER totalNumberOfFreeBytes;
    if (GetDiskFreeSpaceExA(
        MemeString_cStr((mmstr_t)&mountpoint),
        &freeBytesAvailable,
        &totalNumberOfBytes,
        &totalNumberOfFreeBytes) == 0)
    {
        mmstrstk_uninit(&mountpoint, MMSTR__OBJ_SIZE);
        return -1;
    }
    
    _freespace->total = totalNumberOfBytes.QuadPart;
    _freespace->free  = totalNumberOfFreeBytes.QuadPart;
    _freespace->avail = freeBytesAvailable.QuadPart;
    _freespace->load  = (uint32_t)(_freespace->free * 100 / _freespace->total);

#endif
    mmstrstk_uninit(&mountpoint, MMSTR__OBJ_SIZE);
    return 0;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !MGHW_DISK_H_INCLUDED
