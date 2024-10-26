
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

//! 函数返回值
enum {
    mgthrd_success  = 0, //< 请求的操作成功
    mgthrd_nomem    = 1, //< 请求的操作因无法分配内存而失败
    mgthrd_timedout = 2, //< 调用中的指定时间已到达，但未获取到请求的资源
    mgthrd_busy     = 3, //< 请求的操作失败，因为由测试和返回函数请求的资源已被占用
    mgthrd_error    = 4  //< 请求的操作失败
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

//! 线程启动函数。
//! 任何通过 @ref mgthrd_create() 函数启动的线程必须通过此类型的函数启动。
//! @param arg 线程参数（对应 @ref mgthrd_create() 调用中的 @c arg 参数）。
//! @return 线程的返回值，可以通过 @ref mgthrd_join() 函数由其他线程获取。
//!
typedef int (*mgthrd_start_fn_t)(void *arg);

//! 线程特定存储 (TSS) 清理函数
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


//! 创建一个新线程。
//! @param _thr 新创建的线程标识符。
//! @param _func 线程执行的函数指针。
//! @param _arg 传递给线程函数的参数。
//! @return 成功返回 @ref mgthrd_success，内存不足返回 @ref mgthrd_nomem，失败返回 @ref mgthrd_error。
//!
MG_CAPI_INLINE int mgthrd_create(mgthrd_t *_thr, mgthrd_start_fn_t _func, void *_arg)
{
    return __mgthrd_create(_thr, _func, NULL, _arg);
}

//! 获取当前线程的标识符。
//! @return 当前线程的标识符。
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

//! 比较两个线程标识符。
//! 函数判断两个线程标识符是否引用同一个线程。
//! @return 如果两个线程标识符引用不同的线程，则返回0；否则返回非0值。
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


//! 终止调用线程的执行。
//! @param _res 调用线程的结果代码。
MG_CAPI_INLINE void mgthrd_exit(int _res)
{
    __mgthrd_exit(_res, NULL);
}

//! 等待线程终止。
//! 函数通过阻塞等待线程与当前线程合并，直到目标线程终止。
//! @param _thr 要合并的线程。
//! @param _res 如果该指针不为NULL，函数会将线程的返回结果存入 @c res 指向的整数中。
//! @return 成功返回 @ref mgthrd_success，失败返回 @ref mgthrd_error。
MG_CAPI_INLINE int mgthrd_join(mgthrd_t _thr, int *_res)
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
		*_res = (int)dwRes;
	}
	CloseHandle(_thr);
#elif MG_THR__PTHREADS_AVAIL
	void *pres;
	if (pthread_join(_thr, &pres) != 0) 
	{
		return mgthrd_error;
	}
	if (_res != NULL) {
		*_res = (int)(intptr_t)pres;
	}
#endif
	return mgthrd_success;
}

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
