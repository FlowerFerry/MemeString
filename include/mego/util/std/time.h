
#ifndef MEGO_UTIL_STD_TIME_H_INCLUDED
#define MEGO_UTIL_STD_TIME_H_INCLUDED

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
    
#ifdef TIME_UTC
#define MGU_TIME_UTC TIME_UTC
#else
#define MGU_TIME_UTC 1
#endif

    typedef int64_t mgu_time_t;
    typedef int64_t mgu_timestamp_t;
    typedef int64_t mgu_timeval_t;

    typedef struct mgu_timespec_t
    {
        mgu_time_t tv_sec;  // Seconds - >= 0
        long      tv_nsec;  // Nanoseconds - [0, 999999999]
    } mgu_timespec_t;

#ifdef TIME_UTC
    static inline int mgu_timespec_get(mgu_timespec_t* _ts, int _base)
    {
        struct timespec ts;
        int ret = timespec_get(&ts, _base);
        _ts->tv_sec = ts.tv_sec; _ts->tv_nsec = ts.tv_nsec;
        return ret;
    }
#else   
    static inline int mgu_timespec_get(mgu_timespec_t* _ts, int _base)
    {
        if (!_ts) return 0;

        if (_base != COMM_TIME_UTC) {
            return 0;
        }

#if defined(COMM_WINDOWS)
        struct _timeb tb;
        _ftime_s(&tb);
        _ts->tv_sec = (time_t)tb.time;
        _ts->tv_nsec = 1000000L * (long)tb.millitm;
#elif defined(CLOCK_REALTIME)
        struct timespec ts;
        _base = (clock_gettime(CLOCK_REALTIME, &ts) == 0) ? _base : 0;
        _ts->tv_sec = ts.tv_sec;
        _ts->tv_nsec = ts.tv_nsec;
#else
        struct timeval tv;
        gettimeofday(&tv, NULL);
        _ts->tv_sec = (time_t)tv.tv_sec;
        _ts->tv_nsec = 1000L * (long)tv.tv_usec;
#endif

        return _base;
    }
#endif

    static inline mgu_timestamp_t mgu_timestamp_get()
    {
        mgu_timespec_t ts;
        mgu_timespec_get(&ts, MGU_TIME_UTC);
        return (mgu_timestamp_t)ts.tv_sec * 1000LL + (mgu_timestamp_t)ts.tv_nsec / 1000000LL;
    }

    static inline mgu_timeval_t mgu_timeval_get()
    {
        mgu_timespec_t ts;
        mgu_timespec_get(&ts, MGU_TIME_UTC);
        return (mgu_timeval_t)ts.tv_sec * 1000000LL + (mgu_timeval_t)ts.tv_nsec / 1000LL;
    }
    
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !MEGO_UTIL_STD_TIME_H_INCLUDED
