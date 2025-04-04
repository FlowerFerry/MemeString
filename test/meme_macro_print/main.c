
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mego/predef/architecture/arm.h>
#include <mego/predef/architecture/blackfin.h>
#include <mego/predef/architecture/ia64.h>
#include <mego/predef/architecture/m68k.h>
#include <mego/predef/architecture/parisc.h>
#include <mego/predef/architecture/sparc.h>
#include <mego/predef/architecture/sys370.h>
#include <mego/predef/architecture/sys390.h>
#include <mego/predef/architecture/x86.h>
#include <mego/predef/architecture/z.h>

#include <mego/predef/compiler/clang.h>
#include <mego/predef/compiler/gcc.h>
#include <mego/predef/compiler/visualc.h>

#include <mego/predef/hdr/unistd.h>

#include <mego/predef/lang/version.h>

#include <mego/predef/library/c/gnu.h>

#include <mego/predef/os/bsd/bsdi.h>
#include <mego/predef/os/bsd/dragonfly.h>
#include <mego/predef/os/bsd/free.h>
#include <mego/predef/os/bsd/net.h>
#include <mego/predef/os/bsd/open.h>
#include <mego/predef/os/android.h>
#include <mego/predef/os/ios.h>
#include <mego/predef/os/linux.h>
#include <mego/predef/os/macos.h>
#include <mego/predef/os/windows.h>

#include <mego/predef/threads.h>

#if MG_HDR__UNISTD_AVAIL
#include <unistd.h>
#endif

int main(int argc, char *argv[]) 
{
    printf("MEGO_ARCH__ARM: %d\n", MEGO_ARCH__ARM);
    printf("MEGO_ARCH__BLACKFIN: %d\n", MEGO_ARCH__BLACKFIN);
    printf("MEGO_ARCH__IA64: %d\n", MEGO_ARCH__IA64);
    printf("MEGO_ARCH__M68K: %d\n", MEGO_ARCH__M68K);
    printf("MEGO_ARCH__PARISC: %d\n", MEGO_ARCH__PARISC);
    printf("MEGO_ARCH__SPARC: %d\n", MEGO_ARCH__SPARC);
    printf("MEGO_ARCH__SYS370: %d\n", MEGO_ARCH__SYS370);
    printf("MEGO_ARCH__SYS390: %d\n", MEGO_ARCH__SYS390);
    printf("MEGO_ARCH__X86: %d\n", MEGO_ARCH__X86);
    printf("MEGO_ARCH__AMD64: %d\n", MEGO_ARCH__AMD64);
    printf("MEGO_ARCH__Z: %d\n", MEGO_ARCH__Z);

    printf("MG_HDR__UNISTD_AVAIL: %d\n", MG_HDR__UNISTD_AVAIL);
    
    printf("MG_LANG__C_VER: %d\n", MG_LANG__C_VER);
    printf("MG_LANG__C11_AVAIL: %d\n", MG_LANG__C11_AVAIL);
    printf("MG_LANG__C17_AVAIL: %d\n", MG_LANG__C17_AVAIL);
    printf("MG_LANG__C23_AVAIL: %d\n", MG_LANG__C23_AVAIL);

    printf("MG_LANG__CXX_VER: %d\n", MG_LANG__CXX_VER);
    printf("MG_LANG__CXX11_AVAIL: %d\n", MG_LANG__CXX11_AVAIL);
    printf("MG_LANG__CXX14_AVAIL: %d\n", MG_LANG__CXX14_AVAIL);
    printf("MG_LANG__CXX17_AVAIL: %d\n", MG_LANG__CXX17_AVAIL);
    printf("MG_LANG__CXX20_AVAIL: %d\n", MG_LANG__CXX20_AVAIL);
    printf("MG_LANG__CXX23_AVAIL: %d\n", MG_LANG__CXX23_AVAIL);
    printf("MG_LANG__CXX26_AVAIL: %d\n", MG_LANG__CXX26_AVAIL);

    printf("MG_LIB__C_GNU_AVAIL: %d\n", MG_LIB__C_GNU_AVAIL);

    printf("MG_OS__BSD_AVAIL: %d\n", MG_OS__BSD_AVAIL);
    printf("MG_OS__BSD_BSDI_AVAIL: %d\n", MG_OS__BSD_BSDI_AVAIL);
    printf("MG_OS__BSD_DRAGONFLY_AVAIL: %d\n", MG_OS__BSD_DRAGONFLY_AVAIL);
    printf("MG_OS__BSD_FREE_AVAIL: %d\n", MG_OS__BSD_FREE_AVAIL);
    printf("MG_OS__BSD_NET_AVAIL: %d\n", MG_OS__BSD_NET_AVAIL);
    printf("MG_OS__BSD_OPEN_AVAIL: %d\n", MG_OS__BSD_OPEN_AVAIL);
    printf("MG_OS__ANDROID_AVAIL: %d\n", MG_OS__ANDROID_AVAIL);
    printf("MG_OS__IOS_AVAIL: %d\n", MG_OS__IOS_AVAIL);
    printf("MG_OS__LINUX_AVAIL: %d\n", MG_OS__LINUX_AVAIL);
    printf("MG_OS__MACOS_AVAIL: %d\n", MG_OS__MACOS_AVAIL);
    printf("MG_OS__WIN_AVAIL: %d\n", MG_OS__WIN_AVAIL);

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

#ifdef _MSVC_VER
    printf("_MSC_VER: %d\n", _MSC_VER);
#else
    printf("_MSC_VER: not defined\n");
#endif

#ifdef _POSIX_THREADS
    printf("_POSIX_THREADS: %d\n", _POSIX_THREADS);
#else
    printf("_POSIX_THREADS: not defined\n");
#endif
}