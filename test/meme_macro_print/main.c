
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mego/predef/hdr/unistd.h>
#include <mego/predef/threads.h>

#if MG_HDR__UNISTD_AVAIL
#include <unistd.h>
#endif

int main(int argc, char *argv[]) 
{
    printf("MG_OS__LINUX_AVAIL: %d\n", MG_OS__LINUX_AVAIL);
    printf("MG_OS__WIN_AVAIL: %d\n", MG_OS__WIN_AVAIL);
    printf("MG_HDR__UNISTD_AVAIL: %d\n", MG_HDR__UNISTD_AVAIL);
    printf("MG_THR__THREADS_AVAIL: %d\n", MG_THR__THREADS_AVAIL);
    printf("MG_THR__PTHREADS_AVAIL: %d\n", MG_THR__PTHREADS_AVAIL);
    printf("MG_THR__WINTHREADS_AVAIL: %d\n", MG_THR__WINTHREADS_AVAIL);
    printf("MG_COMP__CLANG_AVAIL: %d\n", MG_COMP__CLANG_AVAIL);
    printf("MG_COMP__GCC_AVAIL: %d\n", MG_COMP__GCC_AVAIL);
    printf("MG_COMP__MSVC_AVAIL: %d\n", MG_COMP__MSVC_AVAIL);

#ifdef __GLIBC__
    printf("__GLIBC__: %d\n", __GLIBC__);
#else
    printf("__GLIBC__: not defined\n");
#endif

#ifdef __GLIBC_MINOR__
    printf("__GLIBC_MINOR__: %d\n", __GLIBC_MINOR__);
#else
    printf("__GLIBC_MINOR__: not defined\n");
#endif

#ifdef __UCLIBC__
    printf("__UCLIBC__: %d\n", __UCLIBC__);
#else
    printf("__UCLIBC__: not defined\n");
#endif

#ifdef __UCLIBC_MAJOR__
    printf("__UCLIBC_MAJOR__: %d\n", __UCLIBC_MAJOR__);
#else
    printf("__UCLIBC_MAJOR__: not defined\n");
#endif

#ifdef __UCLIBC_MINOR__
    printf("__UCLIBC_MINOR__: %d\n", __UCLIBC_MINOR__);
#else
    printf("__UCLIBC_MINOR__: not defined\n");
#endif

#ifdef __UCLIBC_SUBLEVEL__
    printf("__UCLIBC_SUBLEVEL__: %d\n", __UCLIBC_SUBLEVEL__);
#else
    printf("__UCLIBC_SUBLEVEL__: not defined\n");
#endif

#ifdef _POSIX_THREADS
    printf("_POSIX_THREADS: %d\n", _POSIX_THREADS);
#else
    printf("_POSIX_THREADS: not defined\n");
#endif
}