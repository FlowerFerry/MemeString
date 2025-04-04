
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mego/predef/hdr/unistd.h>
#include <mego/predef/threads.h>

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

}