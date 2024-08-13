
#ifndef MEGO_THRD_THREADS_H_INCLUDED
#define MEGO_THRD_THREADS_H_INCLUDED

#include <mego/predef/os/windows.h>
#include <mego/predef/symbol/inline.h>
#include <mego/util/os/windows/windows_simplify.h>
#include <mego/util/std/time.h>

#if defined(__unix__) || defined(__APPLE__)
#   include <sched.h>
#endif

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
