
#include <mego/hardware/disk/disk.h>

#include <stdio.h>

int main(int _argc, char *_argv[])
{
    mmstrstk_t mountPoint;
    mghw_harddisk_freespace_t freeSpace = { sizeof(mghw_harddisk_freespace_t), 0 };
    int result = 0;

    if (_argc < 2) {
        printf("Usage: %s <directory path>\n", _argv[0]);
        return -1;
    }

    mountPoint = mghw_get_harddisk_mountpoint_by_path(_argv[1], -1);
    printf("Mount point: %s\n", MemeString_cStr((mmstr_t)&mountPoint));
    mmstrstk_uninit(&mountPoint);

#if MG_OS__LINUX_AVAIL
    struct statvfs buf;
    memset(&buf, 0, sizeof(buf));
    result = statvfs(_argv[1], &buf);
    if (result != 0) {
        printf("Failed to get file system information.\n");
    }
    else {
        printf("statvfs.f_bsize field size: %zu\n",   sizeof(buf.f_bsize));
        printf("statvfs.f_frsize field size: %zu\n",  sizeof(buf.f_frsize));
        printf("statvfs.f_blocks field size: %zu\n",  sizeof(buf.f_blocks));
        printf("statvfs.f_bfree field size: %zu\n",   sizeof(buf.f_bfree));
        printf("statvfs.f_bavail field size: %zu\n",  sizeof(buf.f_bavail));
        printf("statvfs.f_files field size: %zu\n",   sizeof(buf.f_files));
        printf("statvfs.f_ffree field size: %zu\n",   sizeof(buf.f_ffree));
        printf("statvfs.f_favail field size: %zu\n",  sizeof(buf.f_favail));
        printf("statvfs.f_fsid field size: %zu\n",    sizeof(buf.f_fsid));
        printf("statvfs.f_flag field size: %zu\n",    sizeof(buf.f_flag));
        printf("statvfs.f_namemax field size: %zu\n", sizeof(buf.f_namemax));

        printf("statvfs.f_bsize value: %u\n", buf.f_bsize);
        printf("statvfs.f_frsize value: %u\n", buf.f_frsize);
        printf("statvfs.f_blocks value: %llu\n", (uint64_t)buf.f_blocks);
        printf("statvfs.f_bfree value: %llu\n",  (uint64_t)buf.f_bfree);
        printf("statvfs.f_bavail value: %llu\n", (uint64_t)buf.f_bavail);
        printf("statvfs.f_files value: %llu\n",  (uint64_t)buf.f_files);
        printf("statvfs.f_ffree value: %llu\n",  (uint64_t)buf.f_ffree);
        printf("statvfs.f_favail value: %llu\n", (uint64_t)buf.f_favail);
        printf("statvfs.f_fsid value: %u\n", buf.f_fsid);
        printf("statvfs.f_flag value: %u\n", buf.f_flag);
        printf("statvfs.f_namemax value: %u\n", buf.f_namemax);

    }
#endif

    result = mghw_get_harddisk_freespace_by_path(_argv[1], -1, &freeSpace);
    if (result != 0) {
        printf("Failed to get free space information.\n");
    }
    else {
        printf("Total space: %llu bytes\n", freeSpace.total);
        printf("Free space: %llu bytes\n", freeSpace.free);
        printf("Available space: %llu bytes\n", freeSpace.avail);
        printf("Load: %u%%\n", freeSpace.load);
    }

    return 0;
}