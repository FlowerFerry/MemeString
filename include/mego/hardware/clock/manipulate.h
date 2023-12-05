
#ifndef MGHW_CLOCK_MANIPULATE_H_INCLUDED
#define MGHW_CLOCK_MANIPULATE_H_INCLUDED

#include <mego/predef/os/linux.h>
#include <mego/util/std/time.h>
#include <mego/predef/architecture/x86.h>

#if MG_OS__LINUX_AVAIL
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <linux/version.h>
#include <linux/rtc.h>

#if MEGO_ARCH__I386
#include <asm/io.h>
#endif

#endif // MG_OS__LINUX_AVAIL

#if LINUX_VERSION_CODE >= 131072
#include <linux/kd.h>
#define MGHW_CLOCK__RTC_AVAIL (1)
#else
#define MGHW_CLOCK__RTC_AVAIL (0)
#endif

#if defined(KDGHWCLK)
#define MGHW_CLOCK__KD_AVAIL (1)
#else
#define MGHW_CLOCK__KD_AVAIL (0)
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum mghw_clock_access_t {
    mghw_clock_access__isa,
    mghw_clock_access__rtc_ioctl,
    mghw_clock_access__kd
};

inline mghw_clock_access_t
mghw_clock__determine_access()
{
    mghw_clock_access_t access = mghw_clock_access__isa;
    int is_rtc_avail = 0;
#if MGHW_CLOCK__RTC_AVAIL
    do {
        int fd = open("/dev/rtc", O_RDONLY);
        if (fd >= 0) {
            is_rtc_avail = 1;
            close(fd);
        }
        else {
            is_rtc_avail = 0;
        }
    } while (0);
#else
    is_rtc_avail = 1;
#endif
    if (is_rtc_avail) {
        access = mghw_clock_access__rtc_ioctl;
    }
#if MGHW_CLOCK__KD_AVAIL
    else {
        struct hwclk_time t;
        int fd = open("/dev/console", O_RDONLY);
        if (fd >= 0) {
            if (ioctl(fd, KDGHWCLK, &t) >= 0) 
            {
                access = mghw_clock_access__kd;
            }
            else {
                if (errno == EINVAL) {
                    access = mghw_clock_access__isa;
                }
                else {
                    access = mghw_clock_access__kd;
                }
            }
            close(fd);
        }
        else {
            access = mghw_clock_access__kd;
        }
    }
#else
    else {
        access = mghw_clock_access__isa;
    }
#endif
    return access;
}

inline uint8_t
mghw_clock__x86_read_reg(uint8_t _reg)
{
#if MEGO_ARCH__I386
    register uint8_t val;
    __asm__ volatile ("cli");

  outb(_reg & 0x7f, 0x70);
  val = inb(0x71);
  __asm__ volatile ("sti");

    return val;
#else
    return 0;
#endif
}

inline void 
mghw_clock__x86_write(uint8_t _reg, uint8_t _val) 
{
#if MEGO_ARCH__I386

  outb(_reg & 0x7f, 0x70);
  outb(_val, 0x71);

#endif
}

inline void 
mghw_clock__x86_write_bcd(int _addr, int _value) 
{
    mghw_clock__x86_write(_addr, ((_value / 10) << 4) + _value % 10);
}

inline int
mghw_clock__set_clock_by_isa(const struct tm* _new)
{
    uint8_t save_control, save_freq_select;
#if MEGO_ARCH__I386
    __asm__ volatile ("cli");
#endif

    save_control = mghw_clock__x86_read_reg(0x0b);
    mghw_clock__x86_write(0x0b, save_control | 0x80);
    save_freq_select = mghw_clock__x86_read_reg(0x0a);
    mghw_clock__x86_write(0x0a, save_freq_select | 0x70);
    mghw_clock__x86_write_bcd(0, _new->tm_sec);
    mghw_clock__x86_write_bcd(2, _new->tm_min);
    mghw_clock__x86_write_bcd(4, _new->tm_hour);
    mghw_clock__x86_write_bcd(6, _new->tm_wday + 3);
    mghw_clock__x86_write_bcd(7, _new->tm_mday);
    mghw_clock__x86_write_bcd(8, _new->tm_mon + 1);
    mghw_clock__x86_write_bcd(9, _new->tm_year % 100);
    mghw_clock__x86_write_bcd(50, (_new->tm_year + 1900) / 100);

    mghw_clock__x86_write(0x0b, save_control);
    mghw_clock__x86_write(0x0a, save_freq_select);

#if MEGO_ARCH__I386
    __asm__ volatile ("sti");
#endif
    return 0;
}

inline int
mghw_clock__set_clock_by_rtc_ioctl(const struct tm* _new)
{

#if MGHW_CLOCK__RTC_AVAIL
    int rc = 0;
    int fd = open("/dev/rtc", O_RDONLY);
    if (fd < 0) {
        return -1;
    }

    rc = ioctl(fd, RTC_SET_TIME, _new);
    if (rc < 0) {
        close(fd);
        return -1;
    }

    close(fd);
#endif
    return 0;
}

inline int
mghw_clock__set_clock_by_kd(const struct tm* _new)
{
#ifdef KDSHWCLK
    int rc = 0;
    struct hwclk_time t;
    int fd = open("/dev/console", O_RDONLY);
    if (fd < 0) {
        return -1;
    }

    t.year  = _new->tm_year;
    t.month = _new->tm_mon;
    t.day   = _new->tm_mday;
    t.hours = _new->tm_hour;
    t.min   = _new->tm_min;
    t.sec   = _new->tm_sec;
    t.wday  = _new->tm_wday;

    rc = ioctl(fd, KDSHWCLK, &t);
    if (rc < 0) {
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
#else
    return 0;
#endif
}

inline int
mghw_clock__set_clock(
    const enum mghw_clock_access_t _access, time_t _newtime, int _universal)
{
    
#if MG_OS__LINUX_AVAIL
    struct tm tm;
    if (_universal) {
        if (gmtime_r(&_newtime, &tm) == NULL) 
        {
            return -1;
        }
    }
    else {
        if (localtime_r(&_newtime, &tm) == NULL) 
        {
            return -1;
        }
    }

    switch (_access)
    {
    case mghw_clock_access__isa:
        return mghw_clock__set_clock_by_isa(&tm);
    case mghw_clock_access__rtc_ioctl:
        return mghw_clock__set_clock_by_rtc_ioctl(&tm);
    case mghw_clock_access__kd:
        return mghw_clock__set_clock_by_kd(&tm);
    default:
        return -1;
    }
#else
    return 0;
#endif
}

int mghw_clock__set_clock_exact_blocked(
    time_t _settime, const struct timeval* _reftime, mghw_clock_access_t _access, int _universal)
{
#if MG_OS__LINUX_AVAIL
    mgu_time_t newtime;
    struct timeval now_tv;
    gettimeofday(&now_tv, NULL);
    newtime = (mgu_time_t)(_settime + mgu_sys_timeval_diff(&now_tv, _reftime) + 1);

    do {
        gettimeofday(&now_tv, NULL);
    }
    while (mgu_sys_timeval_diff(&now_tv, _reftime) < newtime - _settime);

    return mghw_clock__set_clock(_access, _settime, _universal);
#else
    return 0;
#endif

}

#ifdef __cplusplus
}
#endif

#endif // !MGHW_CLOCK_MANIPULATE_H_INCLUDED
