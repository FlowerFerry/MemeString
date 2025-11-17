
#ifndef MGHW_CLOCK_MANIPULATE_H_INCLUDED
#define MGHW_CLOCK_MANIPULATE_H_INCLUDED

#include <mego/predef/os/linux.h>
#include <mego/predef/symbol/inline.h>
#include <mego/util/std/time.h>
#include <mego/predef/architecture/x86.h>
#include <mego/util/std/string.h>

#if MG_OS__LINUX_AVAIL
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
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

//! Define the enumeration for clock access types
typedef enum mghw_clock_access_t {
    mghw_clock_access__isa,        //< ISA access method
    mghw_clock_access__rtc_ioctl,  //< RTC IOCTL access method
    mghw_clock_access__kd          //< KD access method
} mghw_clock_access_e;

//! Get the first available RTC device path and copy it to _path if a valid path is found
//!
//! @param _path Output parameter to store the found device path
//! @param _path_len The maximum length of _path
//! @return Returns 0 on success, -1 on failure
MG_CAPI_INLINE int mghw_clock__get_first_rtc_path(
    char* _path, size_t _path_len)
{
#if MG_OS__LINUX_AVAIL
    int ret = -1;
    const char* rtc_paths[] = {
        "/dev/rtc0",
        "/dev/rtc",
        "/dev/rtc/rtc0",
        "/dev/rtc/rtc",
        "/dev/misc/rtc0",
        "/dev/misc/rtc",
        NULL
    };

    size_t index = 0;
    for (; rtc_paths[index]; ++index) {
        struct stat st;
        if (stat(rtc_paths[index], &st) == 0) {
            if (S_ISCHR(st.st_mode)) {
                ret = 0;
                break;
            }
        }
    }
    if (!ret) {
        mgu_strncpy_s(_path, _path_len, rtc_paths[index], _path_len - 1);
    }
    return ret;
#else
    return -1;
#endif
}

//! Determine the most suitable clock access method, with priority RTC > KD > ISA
//!
//! @return Returns the most appropriate clock access type
MG_CAPI_INLINE mghw_clock_access_e
    mghw_clock__determine_access()
{
    mghw_clock_access_e access = mghw_clock_access__isa;
    int is_rtc_avail = 0;
#if MGHW_CLOCK__RTC_AVAIL
    do {
        int fd;
        char path[128];
        if (mghw_clock__get_first_rtc_path(path, sizeof(path)))
        {
            is_rtc_avail = 0;
            break;
        }
        
        fd = open(path, O_RDONLY);
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

//! Read the specified register value from the CMOS registers of the x86 architecture
//!
//! @param _reg The register address to read
//! @return Returns the value in the register
MG_CAPI_INLINE uint8_t
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

//! Write data to the CMOS registers of the x86 architecture
//!
//! @param _reg The register address to write to
//! @param _val The value to write
MG_CAPI_INLINE void 
    mghw_clock__x86_write(uint8_t _reg, uint8_t _val) 
{
#if MEGO_ARCH__I386

  outb(_reg & 0x7f, 0x70);
  outb(_val, 0x71);

#endif
}

//! Write BCD-encoded time data to the CMOS registers of the x86 architecture
//!
//! @param _addr The register address
//! @param _value The value to write, which will be converted to BCD format and written
MG_CAPI_INLINE void 
    mghw_clock__x86_write_bcd(int _addr, int _value) 
{
    mghw_clock__x86_write(_addr, ((_value / 10) << 4) + _value % 10);
}

//! Set the system time using the ISA method
//!
//! @param _new Pointer to the structured time (struct tm)
//! @return Returns 0 on success, -1 on failure
MG_CAPI_INLINE int
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

//! Set the system time using the RTC IOCTL method
//!
//! @param _new Pointer to the structured time (struct tm)
//! @return Returns 0 on success, -1 on failure
MG_CAPI_INLINE int
    mghw_clock__set_clock_by_rtc_ioctl(const struct tm* _new)
{

#if MGHW_CLOCK__RTC_AVAIL
    int rc = 0;
    int fd;
    char path[128];
    if (mghw_clock__get_first_rtc_path(path, sizeof(path)))
        return -1;
    
    fd = open(path, O_RDONLY);
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

//! Set the system time using the KD method
//!
//! @param _new Pointer to the structured time (struct tm)
//! @return Returns 0 on success, -1 on failure
MG_CAPI_INLINE int
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
#endif
    return 0;
}

//! Set the system time using the specified access method
//!
//! @param _access The specified clock access method
//! @param _newtime The timestamp
//! @param _universal Indicates whether to use UTC time
//! @return Returns 0 on success, -1 on failure
MG_CAPI_INLINE int
    mghw_clock__set_clock(
        const mghw_clock_access_e _access, time_t _newtime, int _universal)
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

//! Precisely set the system time after the specified reference time, blocking execution during the process
//!
//! @param _settime The target timestamp
//! @param _reftime The reference time used to calculate the time difference
//! @param _access The clock access method
//! @param _universal Indicates whether to use UTC time
//! @return Returns 0 on success, -1 on failure
MG_CAPI_INLINE int mghw_clock__set_clock_exact_blocked(
    time_t _settime, const struct timeval* _reftime, mghw_clock_access_e _access, int _universal)
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
