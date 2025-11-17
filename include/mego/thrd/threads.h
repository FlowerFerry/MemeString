
#ifndef MEGO_THRD_THREADS_H_INCLUDED
#define MEGO_THRD_THREADS_H_INCLUDED

#include <mego/predef/os/windows.h>
#include <mego/predef/symbol/inline.h>
#include <mego/predef/threads.h>
#include <mego/util/os/windows/windows_simplify.h>
#include <mego/util/std/time.h>

#include <assert.h>

#if defined(__unix__) || defined(__APPLE__)
#   include <sched.h>
#endif

#if MG_THR__PTHREADS_AVAIL
#   include <pthread.h>
#endif

//! Function return value
enum {
    mgthrd_success  = 0, //!< The requested operation was successful
    mgthrd_nomem    = 1, //!< The requested operation failed due to insufficient memory
    mgthrd_timedout = 2, //!< The specified timeout was reached before the requested resource was acquired
    mgthrd_busy     = 3, //!< The requested operation failed because the resource was already in use
    mgthrd_error    = 4  //!< The requested operation failed
};

#if MG_THR__THREADS_AVAIL
#  if MG_THR__WINTHREADS_AVAIL
typedef void* mgthrd_t;
#  elif MG_THR__PTHREADS_AVAIL
typedef pthread_t mgthrd_t;
#  else
#   error "Unsupported platform"
#  endif
#endif

//! Thread start function.
//! Any thread started through the @ref mgthrd_create() function must be started with a function of this type.
//! @param arg The thread argument (corresponding to the @c arg parameter in the @ref mgthrd_create() call).
//! @return The return value of the thread, which can be obtained by other threads through the @ref mgthrd_join() function.
//!
typedef int (*mgthrd_start_fn_t)(void *arg);

//! Thread-specific storage (TSS) cleanup function
typedef void(*mgthrd_tss_clear_fn_t)();

struct __mgthrd_start_params {
	mgthrd_start_fn_t exec_fn;  
    mgthrd_tss_clear_fn_t clear_fn; 
	void * arg;         
};

#if MG_THR__THREADS_AVAIL

#if MG_THR__WINTHREADS_AVAIL
MG_CAPI_INLINE DWORD WINAPI __mgthrd_wrap_func(LPVOID user_data)
#elif MG_THR__PTHREADS_AVAIL
MG_CAPI_INLINE void* __mgthrd_wrap_func(void * user_data)
#endif
{
	mgthrd_start_fn_t exec_fn;
    mgthrd_tss_clear_fn_t clear_fn;
	void * arg;
	int res;

	struct __mgthrd_start_params* params = (struct __mgthrd_start_params*)user_data;
	exec_fn  = params->exec_fn;
    clear_fn = params->clear_fn;
	arg = params->arg;

	free(params);

	res = exec_fn(arg);

#if MG_THR__WINTHREADS_AVAIL
    if (clear_fn) {
        clear_fn();
    }

	return (DWORD)res;
#elif MG_THR__PTHREADS_AVAIL
	return (void*)(intptr_t)res;
#endif
}

MG_CAPI_INLINE int __mgthrd_create(mgthrd_t* _thr, mgthrd_start_fn_t _exec_fn, mgthrd_tss_clear_fn_t _clear_fn, void* _arg)
{
	assert(_thr && "mgthrd_create: Invalid thread identifier");

	struct __mgthrd_start_params* params = (struct __mgthrd_start_params*)malloc(sizeof(struct __mgthrd_start_params));
	if (params == NULL) {
		return mgthrd_nomem;
	}
	params->exec_fn = _exec_fn;
    params->clear_fn = _clear_fn;
	params->arg = _arg;

#if MG_THR__WINTHREADS_AVAIL
	* _thr = CreateThread(NULL, 0, __mgthrd_wrap_func, params, 0, NULL);
	if (!(*_thr)) {
		free(params);
		return mgthrd_error;
	}
#elif MG_THR__PTHREADS_AVAIL
	if (pthread_create(_thr, NULL, __mgthrd_wrap_func, params) != 0)
	{
		free(params);
		return mgthrd_error;
	}
#endif

	return mgthrd_success;
}


//! Create a new thread.
//! @param _thr The identifier of the newly created thread.
//! @param _func A pointer to the function to be executed by the thread.
//! @param _arg The argument to be passed to the thread function.
//! @return Returns @ref mgthrd_success on success, @ref mgthrd_nomem if memory is insufficient, or @ref mgthrd_error on failure.
//!
MG_CAPI_INLINE int mgthrd_create(mgthrd_t *_thr, mgthrd_start_fn_t _func, void *_arg)
{
    return __mgthrd_create(_thr, _func, NULL, _arg);
}

//! Get the identifier of the current thread.
//! @return The identifier of the current thread.
MG_CAPI_INLINE mgthrd_t mgthrd_current(void)
{
#if MG_THR__WINTHREADS_AVAIL
	return GetCurrentThread();
#elif MG_THR__PTHREADS_AVAIL
	return pthread_self();
#endif
}

MG_CAPI_INLINE int mgthrd_detach(mgthrd_t _thr)
{
#if MG_THR__WINTHREADS_AVAIL
	/* https://stackoverflow.com/questions/12744324/how-to-detach-a-thread-on-windows-c#answer-12746081 */
	return CloseHandle(_thr) != 0 ? mgthrd_success : mgthrd_error;
#elif MG_THR__PTHREADS_AVAIL
	return pthread_detach(_thr) == 0 ? mgthrd_success : mgthrd_error;
#endif
}

//! Compare two thread identifiers.
//! This function determines whether two thread identifiers refer to the same thread.
//! @return Returns 0 if the two thread identifiers refer to different threads; otherwise, returns a non-zero value.
MG_CAPI_INLINE int mgthrd_equal(mgthrd_t _lhs, mgthrd_t _rhs)
{
#if MG_THR__WINTHREADS_AVAIL
	return GetThreadId(_lhs) == GetThreadId(_rhs);
#elif MG_THR__PTHREADS_AVAIL
	return pthread_equal(_lhs, _rhs);
#endif
}

MG_CAPI_INLINE void __mgthrd_exit(int _res, mgthrd_tss_clear_fn_t _clear_fn)
{
#if MG_THR__WINTHREADS_AVAIL
    if (_clear_fn) {
        _clear_fn();
    }

	ExitThread((DWORD)_res);
#elif MG_THR__PTHREADS_AVAIL
	pthread_exit((void*)(intptr_t)_res);
#endif
}


//! Terminate the execution of the calling thread.
//! @param _res The result code of the calling thread.
MG_CAPI_INLINE void mgthrd_exit(int _res)
{
    __mgthrd_exit(_res, NULL);
}

//! Wait for a thread to terminate.
//! This function blocks the calling thread until the specified thread terminates.
//! @param _thr The thread to join.
//! @param _res If this pointer is not NULL, the function stores the thread's return result in the integer pointed to by @c _res.
//! @return Returns @ref mgthrd_success on success, or @ref mgthrd_error on failure.
MG_CAPI_INLINE int mgthrd_join(mgthrd_t _thr, int64_t *_res)
{
#if MG_THR__WINTHREADS_AVAIL
	DWORD dwRes;
	if (WaitForSingleObject(_thr, INFINITE) == WAIT_FAILED) 
	{
		return mgthrd_error;
	}

	if (GetExitCodeThread(_thr, &dwRes) == FALSE)
	{
		return mgthrd_error;
	}
	if (_res != NULL) {
		*_res = dwRes;
	}
	CloseHandle(_thr);
#elif MG_THR__PTHREADS_AVAIL
	void *pres;
	if (pthread_join(_thr, &pres) != 0) 
	{
		return mgthrd_error;
	}
	if (_res != NULL) {
		*_res = (intptr_t)pres;
	}
#endif
	return mgthrd_success;
}

#endif

//! Put the current thread to sleep for a specified duration.
//!
//! This function implements thread sleep functionality based on the operating system. It can optionally return the remaining time if the sleep is interrupted.
//!
//! @param[in] _duration A pointer to an `mgu_timespec_t` structure specifying the sleep duration.
//! @param[out] _remaining If not NULL, stores the remaining sleep time if interrupted.
//! @return Returns 0 on success; -1 if interrupted by a signal; -2 on error.
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

//! Yield the execution of the current thread.
//!
//! This function yields the execution of the current thread in a multithreaded environment, allowing other threads to run.
//! This operation helps improve the responsiveness and performance of multithreaded programs.
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
