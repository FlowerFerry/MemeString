
#ifndef MEGO_THRD_THREADS_H_INCLUDED
#define MEGO_THRD_THREADS_H_INCLUDED

#include <mego/predef/os/windows.h>
#include <mego/predef/symbol/inline.h>
#include <mego/util/os/windows/windows_simplify.h>
#include <mego/util/std/time.h>

#if defined(__unix__) || defined(__APPLE__)
#   include <sched.h>
#endif

//! 使当前线程休眠指定的时间。
//!
//! 此函数根据操作系统实现线程的休眠功能。它可以选择性地返回剩余时间，如果休眠被中断。
//!
//! @param[in] _duration 指向指定休眠时间的`mgu_timespec_t`结构体。
//! @param[out] _remaining 如果不为NULL，存储被中断时剩余的休眠时间。
//! @return 成功时返回0；如果被信号中断返回-1；如果发生错误返回-2。
MG_CAPI_INLINE int mgthrd_sleep(const struct mgu_timespec_t *_duration, struct mgu_timespec_t *_remaining)
{
#if MG_OS__WIN_AVAIL
	struct mgu_timespec_t start;
	DWORD t;

    if (!_duration) 
        return -2;

	mgu_timespec_get(&start, TIME_UTC);

	t = SleepEx((DWORD)(_duration->tv_sec * 1000 +
		_duration->tv_nsec / 1000000 +
		(((_duration->tv_nsec % 1000000) == 0) ? 0 : 1)),
		TRUE);

	if (t == 0) {
		return 0;
	}
	else {
		if (_remaining != NULL) {
			mgu_timespec_get(_remaining, TIME_UTC);
			_remaining->tv_sec -= start.tv_sec;
			_remaining->tv_nsec -= start.tv_nsec;
			if (_remaining->tv_nsec < 0)
			{
				_remaining->tv_nsec += 1000000000;
				_remaining->tv_sec -= 1;
			}
		}

		return (t == WAIT_IO_COMPLETION) ? -1 : -2;
	}
#else
	struct timespec d;
	int res = 0;
    
    if (!_duration) 
        return -2;

    d.tv_sec  = _duration->tv_sec;
    d.tv_nsec = _duration->tv_nsec;

	if (!_remaining)
		res = nanosleep(&d, NULL);
	else {
        struct timespec r;
        res = nanosleep(&d, &r);
        if (res != 0 && errno == EINTR) {
            _remaining->tv_sec = r.tv_sec;
            _remaining->tv_nsec = r.tv_nsec;
        } else {
            _remaining->tv_sec  = 0;
            _remaining->tv_nsec = 0;
        }
	}
	if (res == 0) {
		return 0;
	}
	else if (errno == EINTR) {
		return -1;
	}
	else {
		return -2;
	}
#endif
}

//! 让出当前线程的执行。
//!
//! 此函数用于在多线程环境中让出当前线程的执行权限，以便其他线程可以运行。
//! 这个操作有助于提高多线程程序的响应性和性能。
MG_CAPI_INLINE void mguthrd_yield(void)
{
#if MG_OS__WIN_AVAIL
    SwitchToThread();
#elif defined(__unix__) || defined(__APPLE__)
    sched_yield();
#else
#   error "Unsupported platform"
#endif 
}

#endif // !MEGO_THRD_THREADS_H_INCLUDED
