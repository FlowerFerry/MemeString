
#ifndef MG_OS_LINUX_HARDWARE_I2C_H_INCLUDED
#define MG_OS_LINUX_HARDWARE_I2C_H_INCLUDED

#include <mego/predef/os/linux.h>
#include <mego/predef/symbol/inline.h>
#include <mego/predef/symbol/likely.h>
#include <mego/err/ec.h>
#include <mego/err/ec_impl.h>

#include <stdio.h>
#include <assert.h>

#if MG_OS__LINUX_AVAIL
#  include <fcntl.h>
#  include <unistd.h>
#  include <sys/ioctl.h>
#  include <linux/i2c.h>
#  include <linux/i2c-dev.h>
#endif 

#ifndef MG_I2C__NUM_INVALID
#define MG_I2C__NUM_INVALID (-1)
#endif

MG_CAPI_INLINE mgec_t mg_i2c__read(
    int _num, int _dev_addr, uint8_t _reg_addr, 
    int _timeout_tick, int _retry_count,
    uint8_t* _buf, size_t _len)
{
#if MG_OS__LINUX_AVAIL
    int ret = -1;
    int fd  = -1;
    char path[64] = { 0 };
    struct i2c_rdwr_ioctl_data data;
    uint8_t msg_buf[sizeof(struct i2c_msg) * 2] = { 0 };

    assert(_buf != NULL && "'mg_i2c__read' failed: invalid buffer pointer");
    assert(_len > 0 && "'mg_i2c__read' failed: invalid buffer length");
    assert(_num != MG_I2C__NUM_INVALID && "'mg_i2c__read' failed: invalid I2C number");

    snprintf(path, sizeof(path), "/dev/i2c-%d", _num);
    fd = open(path, O_RDWR);
    if (MEGO_SYMBOL__UNLIKELY(fd < 0))
        return mgec__from_sys_err(errno);

    ioctl(fd, I2C_TIMEOUT, _timeout_tick);
    ioctl(fd, I2C_RETRIES, _retry_count);
    
    data.nmsgs = 2;
    data.msgs  = (struct i2c_msg*)msg_buf;
    
    data.msgs[0].addr  = _dev_addr;
    data.msgs[0].flags = 0;
    data.msgs[0].len   = 1;
    data.msgs[0].buf   = &_reg_addr;
    
    data.msgs[1].addr  = _dev_addr;
    data.msgs[1].flags = 1;
    data.msgs[1].len   = _len;
    data.msgs[1].buf   = _buf;

    ret = ioctl(fd, I2C_RDWR, &data);
    if (MEGO_SYMBOL__UNLIKELY(ret < 0))
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

MG_CAPI_INLINE mgec_t mg_i2c__write(
    int _num, int _dev_addr, uint8_t _reg_addr,
    int _timeout_tick, int _retry_count,
    const uint8_t* _buf, size_t _len)
{
#if MG_OS__LINUX_AVAIL
    int fd = -1;
    int ret = -1;
    char path[64] = { 0 };
    struct i2c_rdwr_ioctl_data data;
    uint8_t* buf = NULL;
    uint8_t msg_buf[sizeof(struct i2c_msg)] = { 0 };
    
    assert(_buf != NULL && "'mg_i2c__write' failed: invalid buffer pointer");
    assert(_len > 0 && "'mg_i2c__write' failed: invalid buffer length");
    assert(_num != MG_I2C__NUM_INVALID && "'mg_i2c__write' failed: invalid I2C number");

    snprintf(path, sizeof(path), "/dev/i2c-%d", _num);
    fd = open(path, O_RDWR);
    if (MEGO_SYMBOL__UNLIKELY(fd < 0))
        return mgec__from_sys_err(errno);

    buf = (uint8_t*)malloc(_len + 1);
    if (buf == NULL) {
        close(fd);
        return MGEC__NOMEM;
    }

    ioctl(fd, I2C_TIMEOUT, _timeout_tick);
    ioctl(fd, I2C_RETRIES, _retry_count);

    buf[0] = _reg_addr;
    memcpy(buf + 1, _buf, _len);

    data.nmsgs = 1;
    data.msgs = (struct i2c_msg*)msg_buf;

    data.msgs[0].addr = _dev_addr;
    data.msgs[0].flags = 0;
    data.msgs[0].len = _len + 1;
    data.msgs[0].buf = buf;

    ret = ioctl(fd, I2C_RDWR, &data);
    if (MEGO_SYMBOL__UNLIKELY(ret < 0))
    {
        free(buf);
        close(fd);
        return mgec__from_sys_err(errno);
    }

    free(buf);
    close(fd);
    return 0;
#else
    return MGEC__OPNOTSUPP;
#endif
}

#endif // !MG_OS_LINUX_HARDWARE_I2C_H_INCLUDED
