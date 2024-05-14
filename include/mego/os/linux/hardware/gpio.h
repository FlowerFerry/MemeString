
#ifndef MG_OS_LINUX_HARDWARE_GPIO_H_INCLUDED
#define MG_OS_LINUX_HARDWARE_GPIO_H_INCLUDED

#include <mego/predef/os/linux.h>
#include <mego/predef/symbol/inline.h>
#include <mego/predef/symbol/likely.h>
#include <mego/err/ec.h>
#include <mego/err/ec_impl.h>
#include <mego/util/posix/sys/stat.h>

#include <stdio.h>

typedef enum mg_gpio__direction 
{
    mg_gpio__direction_in  = 0,
    mg_gpio__direction_out = 1
} mg_gpio__direction_e;

typedef int mg_gpio__number_t;

#ifndef MG_GPIO__NUMBER_INVALID
#define MG_GPIO__NUMBER_INVALID (-1)
#endif

MG_CAPI_INLINE mgec_t mg_gpio__export(mg_gpio__number_t _num)
{
#if MG_OS__LINUX_AVAIL
    mgu_stat stat;
    int fd;
    char buf[128];
    ssize_t len;
    if (MEGO_SYMBOL__UNLIKELY(_num == MG_GPIO__NUMBER_INVALID))
        return MGEC__INVAL;
    
    snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d", _num);
    if (mgu_get_stat(buf, -1, &stat) == 0)
        return 0;

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (MEGO_SYMBOL__UNLIKELY(fd < 0))
        return mgec__from_sys_err(errno);
    
    len = snprintf(buf, sizeof(buf), "%d", _num);
    if (write(fd, buf, len) < 0)
    {
        close(fd);
        return mgec__from_sys_err(errno);
    }

    close(fd);

    snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d", _num);
    if (mgu_get_stat(buf, -1, &stat) != 0)
        return mgec__from_sys_err(errno);
    
    return 0;
#else
    return MGEC__OPNOTSUPP;
#endif
}

MG_CAPI_INLINE mgec_t mg_gpio__unexport(mg_gpio__number_t _num)
{
#if MG_OS__LINUX_AVAIL
    int fd;
    char buf[64];
    ssize_t len;
    if (MEGO_SYMBOL__UNLIKELY(_num == MG_GPIO__NUMBER_INVALID))
        return MGEC__INVAL;
    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (MEGO_SYMBOL__UNLIKELY(fd < 0))
        return mgec__from_sys_err(errno);
    
    len = snprintf(buf, sizeof(buf), "%d", _num);
    if (write(fd, buf, len) < 0)
    {
        close(fd);
        return mgec__from_sys_err(errno);
    }

    close(fd);
    return 0;
#else
    return MGEC__OPNOTSUPP;
#endif
}

MG_CAPI_INLINE mgec_t mg_gpio__set_direction(
    mg_gpio__number_t _num, mg_gpio__direction_e _dir)
{
#if MG_OS__LINUX_AVAIL
    int fd;
    char buf[128];
    ssize_t len;
    if (MEGO_SYMBOL__UNLIKELY(_num == MG_GPIO__NUMBER_INVALID))
        return MGEC__INVAL;

    snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/direction", _num);
    fd = open(buf, O_WRONLY);
    if (MEGO_SYMBOL__UNLIKELY(fd < 0))
        return mgec__from_sys_err(errno);
    
    if (write(fd, 
        _dir == mg_gpio__direction_in ? "in" : "out", 
        _dir == mg_gpio__direction_in ? 2 : 3) < 0)
    {
        close(fd);
        return mgec__from_sys_err(errno);
    }
    
    close(fd);
    return 0;
#else
    return MGEC__OPNOTSUPP;
#endif
}

MG_CAPI_INLINE mgec_t mg_gpio__set_value(mg_gpio__number_t _num, int _value)
{
#if MG_OS__LINUX_AVAIL
    int fd;
    char buf[128];
    ssize_t len;
    if (MEGO_SYMBOL__UNLIKELY(_num == MG_GPIO__NUMBER_INVALID))
        return MGEC__INVAL;

    snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", _num);
    fd = open(buf, O_WRONLY);
    if (MEGO_SYMBOL__UNLIKELY(fd < 0))
        return mgec__from_sys_err(errno);
    
    if (write(fd, _value ? "1" : "0", 1) < 0)
    {
        close(fd);
        return mgec__from_sys_err(errno);
    }

    close(fd);
    return 0;
#else
    return MGEC__OPNOTSUPP;
#endif
}

MG_CAPI_INLINE mgec_t mg_gpio__get_value(mg_gpio__number_t _num, int* _value)
{
#if MG_OS__LINUX_AVAIL
    int fd;
    char buf[128];
    ssize_t len;
    if (MEGO_SYMBOL__UNLIKELY(_num == MG_GPIO__NUMBER_INVALID))
        return MGEC__INVAL;

    snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", _num);
    fd = open(buf, O_RDONLY);
    if (MEGO_SYMBOL__UNLIKELY(fd < 0))
        return mgec__from_sys_err(errno);

    char c;
    if (read(fd, &c, 1) < 0) {
        close(fd);
        return mgec__from_sys_err(errno);
    }

    close(fd);
    *_value = (c == '1');
    return 0;
#else
    return MGEC__OPNOTSUPP;
#endif
}

#endif // !MG_OS_LINUX_HARDWARE_GPIO_H_INCLUDED
