
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

//! 定义时钟访问类型枚举
typedef enum mghw_clock_access_t {
    mghw_clock_access__isa,        //< ISA访问方式
    mghw_clock_access__rtc_ioctl,  //< RTC IOCTL访问方式
    mghw_clock_access__kd          //< KD访问方式
} mghw_clock_access_e;

//! 获取第一个可用的RTC设备路径，若找到有效路径则将其复制到_path中
//!
//! @param _path 输出参数，用来存储找到的设备路径
//! @param _path_len _path的最大长度
//! @return 成功返回0，失败返回-1
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

//! 确定最适合的时钟访问方式，优先级为RTC > KD > ISA
//!
//! @return 返回最合适的时钟访问类型
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

//! 从x86架构的CMOS寄存器中读取指定的寄存器值
//!
//! @param _reg 要读取的寄存器地址
//! @return 返回寄存器中的值
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

//! 向x86架构的CMOS寄存器写入数据
//!
//! @param _reg 要写入的寄存器地址
//! @param _val 要写入的值
MG_CAPI_INLINE void 
    mghw_clock__x86_write(uint8_t _reg, uint8_t _val) 
{
#if MEGO_ARCH__I386

  outb(_reg & 0x7f, 0x70);
  outb(_val, 0x71);

#endif
}

//! 向x86架构的CMOS寄存器写入BCD编码的时间数据
//!
//! @param _addr 寄存器地址
//! @param _value 要写入的数值，将会被转换为BCD格式并写入
MG_CAPI_INLINE void 
    mghw_clock__x86_write_bcd(int _addr, int _value) 
{
    mghw_clock__x86_write(_addr, ((_value / 10) << 4) + _value % 10);
}

//! 通过ISA方式设置系统时间
//!
//! @param _new 指向结构化时间（struct tm）的指针
//! @return 成功返回0，失败返回-1
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

//! 通过RTC IOCTL调用设置系统时间
//!
//! @param _new 指向结构化时间（struct tm）的指针
//! @return 成功返回0，失败返回-1
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

//! 通过KD方式设置系统时间
//!
//! @param _new 指向结构化时间（struct tm）的指针
//! @return 成功返回0，失败返回-1
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

//! 根据指定的访问方式设置系统时间
//!
//! @param _access 指定的时钟访问方式
//! @param _newtime 时间戳
//! @param _universal 指示是否使用UTC时间
//! @return 成功返回0，失败返回-1
MG_CAPI_INLINE int
    mghw_clock__set_clock(
        const enum mghw_clock_access_e _access, time_t _newtime, int _universal)
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

//! 精确地在指定的参考时间后设置系统时间，期间会阻塞执行
//!
//! @param _settime 目标时间戳
//! @param _reftime 参考时间，用于计算时间偏差
//! @param _access 时钟访问方式
//! @param _universal 指示是否使用UTC时间
//! @return 成功返回0，失败返回-1
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
