
#ifndef MEGO_UTIL_STD_TIME_H_INCLUDED
#define MEGO_UTIL_STD_TIME_H_INCLUDED

#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <mego/predef/symbol/restrict.h>
#include <mego/predef/os/windows.h>
#include <mego/err/ec.h>

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
    inline int mgu_timespec_get(mgu_timespec_t* _ts, int _base)
    {
        struct timespec ts;
        int ret = timespec_get(&ts, _base);
        _ts->tv_sec = ts.tv_sec; _ts->tv_nsec = ts.tv_nsec;
        return ret;
    }
#else   
    inline int mgu_timespec_get(mgu_timespec_t* _ts, int _base)
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

    inline mgu_timestamp_t mgu_timestamp_get()
    {
        mgu_timespec_t ts;
        mgu_timespec_get(&ts, MGU_TIME_UTC);
        return (mgu_timestamp_t)ts.tv_sec * 1000LL + (mgu_timestamp_t)ts.tv_nsec / 1000000LL;
    }

    inline mgu_timeval_t mgu_timeval_get()
    {
        mgu_timespec_t ts;
        mgu_timespec_get(&ts, MGU_TIME_UTC);
        return (mgu_timeval_t)ts.tv_sec * 1000000LL + (mgu_timeval_t)ts.tv_nsec / 1000LL;
    }
        
    inline mgec_t mgu_asctime_s(char *_buf, size_t _bufsz, const struct tm *_tm)
    {
    #if MG_OS__WIN_AVAIL
        return asctime_s(_buf, _bufsz, _tm);
    #else
        if (!_buf || _tm) return MGEC__INVAL;
        if (_bufsz < 26 || _bufsz > SIZE_MAX) return MGEC__INVAL;

        errno = 0;
        if (!asctime_r(_tm, _buf)) {
            _buf[0] = '\0';
        }
        return errno;
    #endif // COMM_WINDOWS
    }

    inline mgec_t mgu_ctime_s(char *_buf, mgu_time_t _bufsz, const mgu_time_t *_time)
    {
    #if MG_OS__WIN_AVAIL
        return ctime_s(_buf, _bufsz, _time);
    #else
        if (!_buf || _time) return MGEC__INVAL;
        if (_bufsz < 26 || _bufsz > SIZE_MAX) return MGEC__INVAL;

        auto time = static_cast<time_t>(*_time);
        errno = 0;
        if (!ctime_r(&time, _buf)) {
            _buf[0] = '\0';
        }
        return errno;
    #endif // COMM_WINDOWS
    }

    inline struct tm *mgu_gmtime_s(const mgu_time_t * MEGO_SYMBOL__RESTRICT _time, struct tm * MEGO_SYMBOL__RESTRICT _result)
    {
        if (!_time) return NULL;

        time_t time = *_time;
    #if MG_OS__WIN_AVAIL
        return gmtime_s( _result, &time) ? NULL : _result;
    #else
        return gmtime_r(&time, _result);
    #endif

    }

    inline struct tm *mgu_localtime_s(const mgu_time_t * MEGO_SYMBOL__RESTRICT _time, struct tm * MEGO_SYMBOL__RESTRICT _result)
    {	
        if (!_time) return NULL;

        time_t time = *_time;
    #if MG_OS__WIN_AVAIL
        return localtime_s(_result, &time) ? NULL : _result;
    #else
        return localtime_r(&time, _result);
    #endif 
    }

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !MEGO_UTIL_STD_TIME_H_INCLUDED
