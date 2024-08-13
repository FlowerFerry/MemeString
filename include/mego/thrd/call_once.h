
#ifndef MEGO_THRD_CALL_ONCE_H_INCLUDED
#define MEGO_THRD_CALL_ONCE_H_INCLUDED

#include "mego/predef/threads.h"
#include <mego/predef/symbol/inline.h>
#include <stdint.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#if MEGO_THR__WINTHREADS_AVAILABLE
typedef struct {
		long volatile status;
		uint8_t buf[44];
} mgthrd_once_flag;

# define MGTHRD_ONCE_FLAG_INIT { 0, }
// static inline void
// mgthrd_call_once(mgthrd_once_flag* _flag, void(*_func)(void));

MG_CAPI_INLINE void
mgthrd_call_once(mgthrd_once_flag* _flag, void(*_func)(void))
{
	assert(_flag);

	PRTL_CRITICAL_SECTION pCriticalSection = (PRTL_CRITICAL_SECTION)_flag->buf;

	while (_flag->status < 3)
	{
		switch (_flag->status)
		{
		case 0:
			if (InterlockedCompareExchange(&(_flag->status), 1, 0) == 0) {
				InitializeCriticalSection(pCriticalSection);
				EnterCriticalSection(pCriticalSection);
				_flag->status = 2;
				_func();
				_flag->status = 3;
				LeaveCriticalSection(pCriticalSection);
				return;
			}
			break;
		case 1:
			break;
		case 2:
			EnterCriticalSection(pCriticalSection);
			LeaveCriticalSection(pCriticalSection);
			break;
		}
	}
}

#elif MEGO_THR__PTHREADS_AVAILABLE
	
# define mgthrd_once_flag pthread_once_t
# define MGTHRD_ONCE_FLAG_INIT PTHREAD_ONCE_INIT
# define mgthrd_call_once(_flag, _func) pthread_once(_flag, _func)
	
#else
#	error "[mgthrd_call_once] Compilers or platforms not yet supported"
#endif

#ifdef __cplusplus
}
#endif

#endif // !MEGO_THRD_CALL_ONCE_H_INCLUDED
