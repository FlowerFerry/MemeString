
#ifndef MEGO_THRD_NUMERIC_ID_H_INCLUDED
#define MEGO_THRD_NUMERIC_ID_H_INCLUDED

#include <mego/predef/symbol/inline.h>
#include <mego/predef/os/windows.h>
#include <mego/predef/os/linux.h>
#include <stdint.h>
#include <stddef.h>

#include <mego/util/os/windows/windows_simplify.h>

#if !MG_OS__WIN_AVAIL
#  include <fcntl.h>
#  include <unistd.h>
#  include <sys/types.h>
#endif

#if MG_OS__LINUX_AVAIL
#  include <sys/syscall.h>
#elif defined(_AIX)
#  include <pthread.h>
#elif defined(__DragonFly__) || defined(__FreeBSD__)
#  include <pthread_np.h>
#elif defined(__NetBSD__)
#  include <lwp.h>
#elif defined(__sun)
#  include <thread.h>
#endif

#if defined(__APPLE__)
#  include <AvailabilityMacros.h>
#endif

MG_CAPI_INLINE size_t __mgthrd_numeric_id()
{
#if MG_OS__WIN_AVAIL
    return GetCurrentThreadId();
#elif MG_OS__LINUX_AVAIL
  #if __GLIBC__ == 2 && __GLIBC_MINOR__ < 30
  
    #if defined(__ANDROID__) && defined(__ANDROID_API__) && (__ANDROID_API__ < 21)
      #define SYS_gettid __NR_gettid
    #endif

    return (size_t)syscall(SYS_gettid);
  #else
    return (size_t)gettid();
  #endif
#elif defined(_AIX)
    struct __pthrdsinfo buf;
    int reg_size = 0;
    pthread_t pt = pthread_self();
    int retval = pthread_getthrds_np(&pt, PTHRDSINFO_QUERY_TID, &buf, sizeof(buf), NULL, &reg_size);
    int tid = (!retval) ? buf.__pi_tid : 0;
    return (size_t)tid;
#elif defined(__DragonFly__) || defined(__FreeBSD__)
    return (size_t)pthread_getthreadid_np();
#elif defined(__NetBSD__)
    return (size_t)_lwp_self();
#elif defined(__OpenBSD__)
    return (size_t)getthrid();
#elif defined(__sun)
    return (size_t)thr_self();
#elif __APPLE__
    uint64_t tid;
  #ifdef MAC_OS_X_VERSION_MAX_ALLOWED
    do {
        #if (MAC_OS_X_VERSION_MAX_ALLOWED < 1060) || defined(__POWERPC__)
        tid = pthread_mach_thread_np(pthread_self());
        #elif MAC_OS_X_VERSION_MIN_REQUIRED < 1060
        if (&pthread_threadid_np) {
            pthread_threadid_np(nullptr, &tid);
        } else {
            tid = pthread_mach_thread_np(pthread_self());
        }
        #else
        pthread_threadid_np(nullptr, &tid);
        #endif
    } while(0);
  #else
    pthread_threadid_np(NULL, &tid);
  #endif
    return (size_t)tid;
#else
#  error "'mgthrd_numeric_id' is not implemented for this platform"
    return 0;
#endif
}

#endif // !MEGO_THRD_NUMERIC_ID_H_INCLUDED
