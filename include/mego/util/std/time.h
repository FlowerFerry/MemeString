
#ifndef MEGO_UTIL_STD_TIME_H_INCLUDED
#define MEGO_UTIL_STD_TIME_H_INCLUDED

#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <mego/predef/symbol/restrict.h>
#include <mego/predef/symbol/inline.h>
#include <mego/predef/os/windows.h>
#include <mego/predef/os/linux.h>
#include <mego/predef/os/macos.h>
#include <mego/err/ec.h>
#include <errno.h>
#include <limits.h>

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

    typedef struct mgu_timestamp_pair_t
    {
        mgu_timestamp_t begin;
        mgu_timestamp_t end;
    } mgu_timestamp_pair_t;

    typedef struct mgu_timespec_t
    {
        mgu_time_t tv_sec;  // Seconds - >= 0
        long      tv_nsec;  // Nanoseconds - [0, 999999999]
    } mgu_timespec_t;

    typedef enum mgu_round_t
    {
        mgu_round_down = 0,
        mgu_round_up = 1
    } mgu_round_t;

#ifdef TIME_UTC
    MG_CAPI_INLINE int mgu_timespec_get(mgu_timespec_t* _ts, int _base)
    {
        struct timespec ts;
        int ret = timespec_get(&ts, _base);
        _ts->tv_sec = ts.tv_sec; _ts->tv_nsec = ts.tv_nsec;
        return ret;
    }
#else   
    MG_CAPI_INLINE int mgu_timespec_get(mgu_timespec_t* _ts, int _base)
    {
#if MG_OS__WIN_AVAIL
        struct _timeb tb;
#elif defined(CLOCK_REALTIME)
        struct timespec ts = { 0 };
#else
        struct timeval tv;
#endif

        if (!_ts) {
            errno = EINVAL;
            return 0;
        }

        if (_base != MGU_TIME_UTC) {
            errno = EINVAL;
            return 0;
        }

#if MG_OS__WIN_AVAIL
        _ftime_s(&tb);
        _ts->tv_sec = (time_t)tb.time;
        _ts->tv_nsec = 1000000L * (long)tb.millitm;
#elif defined(CLOCK_REALTIME)
        _base = (clock_gettime(CLOCK_REALTIME, &ts) == 0) ? _base : 0;
        _ts->tv_sec = ts.tv_sec;
        _ts->tv_nsec = ts.tv_nsec;
#else
        gettimeofday(&tv, NULL);
        _ts->tv_sec = (time_t)tv.tv_sec;
        _ts->tv_nsec = 1000L * (long)tv.tv_usec;
#endif

        return _base;
    }
#endif

    MG_CAPI_INLINE mgu_timestamp_t mgu_timestamp_get()
    {
        mgu_timespec_t ts;
        mgu_timespec_get(&ts, MGU_TIME_UTC);
        return (mgu_timestamp_t)ts.tv_sec * 1000LL + (mgu_timestamp_t)ts.tv_nsec / 1000000LL;
    }

    MG_CAPI_INLINE mgu_timeval_t mgu_timeval_get()
    {
        mgu_timespec_t ts;
        mgu_timespec_get(&ts, MGU_TIME_UTC);
        return (mgu_timeval_t)ts.tv_sec * 1000000LL + (mgu_timeval_t)ts.tv_nsec / 1000LL;
    }
        
    MG_CAPI_INLINE mgec_t mgu_asctime_s(char *_buf, size_t _bufsz, const struct tm *_tm)
    {
    #if MG_OS__WIN_AVAIL
        return asctime_s(_buf, _bufsz, _tm);
    #else
        if (!_buf || !_tm) return MGEC__INVAL;
        if ( _bufsz < 26 ) return MGEC__INVAL;

        errno = 0;
        if (!asctime_r(_tm, _buf)) {
            _buf[0] = '\0';
        }
        return errno;
    #endif
    }

    MG_CAPI_INLINE mgec_t mgu_ctime_s(char *_buf, size_t _bufsz, const mgu_time_t *_time)
    {
    #if MG_OS__WIN_AVAIL
        return ctime_s(_buf, _bufsz, _time);
    #else
        time_t time;
        if (!_buf || !_time) return MGEC__INVAL;
        if (_bufsz < 26) return MGEC__INVAL;

        time = (time_t)(*_time);
        errno = 0;
        if (!ctime_r(&time, _buf)) {
            _buf[0] = '\0';
        }
        return errno;
    #endif
    }

    MG_CAPI_INLINE struct tm *mgu_gmtime_s(const mgu_time_t * MEGO_SYMBOL__RESTRICT _time, struct tm * MEGO_SYMBOL__RESTRICT _result)
    {
        if (!_time) return NULL;

        time_t time = *_time;
    #if MG_OS__WIN_AVAIL
        return gmtime_s( _result, &time) ? NULL : _result;
    #else
        return gmtime_r(&time, _result);
    #endif

    }

    MG_CAPI_INLINE struct tm *mgu_localtime_s(const mgu_time_t * MEGO_SYMBOL__RESTRICT _time, struct tm * MEGO_SYMBOL__RESTRICT _result)
    {	
        if (!_time) return NULL;

        time_t time = *_time;
    #if MG_OS__WIN_AVAIL
        return localtime_s(_result, &time) ? NULL : _result;
    #else
        return localtime_r(&time, _result);
    #endif 
    }

    MG_CAPI_INLINE int mgu_minute_timezone()
    {
        struct tm gtm;
        mgu_time_t t_now = time(NULL);
        mgu_time_t t_g2l = -1;
        if (t_now == (mgu_time_t)-1)
            return INT_MIN;

        if (mgu_gmtime_s(&t_now, &gtm) == NULL)
            return INT_MIN;

        gtm.tm_isdst = -1;
        t_g2l = mktime(&gtm);
        if (t_g2l == (mgu_time_t)-1)
            return INT_MIN;

        return (int)((t_now - t_g2l) / 60);
    }

    MG_CAPI_INLINE int mgu_timezone_minute()
    {
        return mgu_minute_timezone();
    }

    MG_CAPI_INLINE int mgu_hour_timezone()
    {
        int tz = mgu_minute_timezone();
        if (tz == INT_MIN)
            return INT_MIN;
        return tz / 60;
    }

    MG_CAPI_INLINE int mgu_timezone_hour()
    {
        return mgu_hour_timezone();
    }

    MG_CAPI_INLINE mgu_time_t mgu_mkgmtime_fallback(struct tm * _tm)
    {
        struct tm ttm = *_tm;
        mgu_time_t tv = -1;
        int tz = mgu_minute_timezone() * 60; // convert to seconds
        ttm.tm_isdst = -1;

        if (tz == INT_MIN)
            return -1;

        tv = mktime(&ttm);
        if (tv == -1)
            return -1;
        
        return tv + tz;
    }

    MG_CAPI_INLINE mgu_time_t mgu_mkgmtime(struct tm * _tm)
    {
#if MG_OS__WIN_AVAIL
        return _mkgmtime(_tm);
#elif MG_OS__LINUX_AVAIL || MG_OS__MACOS_AVAIL
        return timegm(_tm);
#else
        return mgu_mkgmtime_fallback(_tm);
#endif
    }

    // MG_CAPI_INLINE mgu_time_t mgu_mktime_utc(struct tm * _tm)
    // {
    //     return mgu_mkgmtime(_tm);
    // }

    MG_CAPI_INLINE mgu_timestamp_t mgu_timestamp_round_to_minute(
        mgu_timestamp_t _ts, int _min_interval, mgu_round_t _round) 
    {
        int remainder = 0;
        struct tm gtm;
        mgu_time_t tv = (mgu_time_t)(_ts / 1000);
        if (mgu_gmtime_s(&tv, &gtm) == NULL)
            return _ts;
        remainder = gtm.tm_min % _min_interval;
    
        switch (_round) {
            case mgu_round_down:
                gtm.tm_min -= remainder;
                break;
            case mgu_round_up:
                if (remainder > 0) 
                {
                    gtm.tm_min += (_min_interval - remainder);
                    if (gtm.tm_min >= 60) {
                        gtm.tm_min -= 60;
                        gtm.tm_hour += 1;
                    }
                }
                break;
        }
    
        gtm.tm_sec = 0;
        tv = mgu_mkgmtime(&gtm);
        if (tv == -1)
            return _ts;
        return tv * 1000;
    }

    MG_CAPI_INLINE mgu_timestamp_t mgu_timestamp_round_to_hour(
        mgu_timestamp_t _ts, int _interval, mgu_round_t _round)
    {
        int remainder = 0;
        struct tm gtm;
        mgu_time_t tv = (mgu_time_t)(_ts / 1000);
        if (mgu_gmtime_s(&tv, &gtm) == NULL)
            return _ts;
        remainder = gtm.tm_hour % _interval;
    
        switch (_round) {
            case mgu_round_down:
                gtm.tm_hour -= remainder;
                break;
            case mgu_round_up:
                if (remainder > 0) 
                {
                    gtm.tm_hour += (_interval - remainder);
                    if (gtm.tm_hour >= 24) {
                        gtm.tm_hour -= 24;
                        gtm.tm_mday += 1;
                    }
                }
                break;
        }
    
        gtm.tm_min = 0;
        gtm.tm_sec = 0;
        tv = mgu_mkgmtime(&gtm);
        if (tv == -1)
            return _ts;
        return tv * 1000;
    }

    MG_CAPI_INLINE mgu_timestamp_t mgu_timestamp_round_to_day(
        mgu_timestamp_t _ts, mgu_round_t _round)
    {
        struct tm gtm;
        mgu_time_t tv = (mgu_time_t)(_ts / 1000);
        if (mgu_gmtime_s(&tv, &gtm) == NULL)
            return _ts;
    
        gtm.tm_hour = 0;
        gtm.tm_min  = 0;
        gtm.tm_sec  = 0;
        
        tv = mgu_mkgmtime(&gtm);
        if (tv == -1)
            return _ts;

        if (_round == mgu_round_up) {
            tv += 24 * 3600;
        }
        
        return tv * 1000;
    }

    MG_CAPI_INLINE mgu_time_t mgu_timestamp_to_time(mgu_timestamp_t _ts)
    {
        return (mgu_time_t)(_ts / 1000);
    }

    MG_CAPI_INLINE mgu_timestamp_t mgu_time_to_ts(mgu_time_t _time)
    {
        return (mgu_timestamp_t)(_time * 1000);
    }

    MG_CAPI_INLINE int mgu_timestamp_get_ms(mgu_timestamp_t _ts)
    {
        return (int)(_ts % 1000);
    }

    MG_CAPI_INLINE struct tm *mgu_gmtime_from_timestamp(mgu_timestamp_t _ts, struct tm * _result, int* _ms)
    {
        mgu_time_t tv = (mgu_time_t)(_ts / 1000);
        if (mgu_gmtime_s(&tv, _result) == NULL)
            return NULL;
        if (_ms) *_ms = (int)(_ts % 1000);
        return _result;
    }

    MG_CAPI_INLINE struct tm *mgu_gmtime_from_ts(mgu_timestamp_t _ts, struct tm * _result, int* _ms)
    {
        return mgu_gmtime_from_timestamp(_ts, _result, _ms);
    }

    MG_CAPI_INLINE struct tm *mgu_localtime_from_timestamp(mgu_timestamp_t _ts, struct tm * _result, int* _ms)
    {
        mgu_time_t tv = (mgu_time_t)(_ts / 1000);
        if (mgu_localtime_s(&tv, _result) == NULL)
            return NULL;
        if (_ms) *_ms = (int)(_ts % 1000);
        return _result;
    }

    MG_CAPI_INLINE struct tm *mgu_localtime_from_ts(mgu_timestamp_t _ts, struct tm * _result, int* _ms)
    {
        return mgu_localtime_from_timestamp(_ts, _result, _ms);
    }

    MG_CAPI_INLINE mgu_timestamp_t mgu_timestamp_from_gmtime(struct tm * _tm, int _ms)
    {
        mgu_time_t tv = mgu_mkgmtime(_tm);
        if (tv == -1)
            return -1;
        return (mgu_timestamp_t)tv * 1000 + _ms % 1000;
    }

    MG_CAPI_INLINE mgu_timestamp_t mgu_timestamp_from_localtime(struct tm * _tm, int _ms)
    {
        mgu_time_t tv = mktime(_tm);
        if (tv == -1)
            return -1;
        return (mgu_timestamp_t)tv * 1000 + _ms % 1000;
    }

#if MG_OS__LINUX_AVAIL
    MG_CAPI_INLINE double 
    mgu_sys_timeval_diff(
        const struct timeval* _subtrahend, const struct timeval* _subtractor) 
    {
        return( (_subtrahend->tv_sec - _subtractor->tv_sec)
            + (_subtrahend->tv_usec - _subtractor->tv_usec) / 1E6 );
    }
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !MEGO_UTIL_STD_TIME_H_INCLUDED
