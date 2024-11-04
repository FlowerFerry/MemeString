
#ifndef MGPP_OS_LINUX_HARDWARE_I2C_H_INCLUDED
#define MGPP_OS_LINUX_HARDWARE_I2C_H_INCLUDED

#include <mego/os/linux/hardware/i2c.h>

namespace mgpp {
namespace os_linux {
namespace hw {
    
    class i2c 
    {
    public:
        i2c(int _num, int _dev_addr) 
            : num_(_num), dev_addr_(_dev_addr)
        {}

        inline void set_timeout_tick(int _timeout_tick)
        {
            timeout_tick_ = _timeout_tick;
        }
        
        inline void set_retry_count(int _retry_count)
        {
            retry_count_ = _retry_count;
        }

        inline mgec_t read(uint8_t _reg_addr, uint8_t* _buf, size_t _len)
        {
            return mg_i2c__read(num_, dev_addr_, _reg_addr, timeout_tick_, retry_count_, _buf, _len);
        }

        inline mgec_t write(uint8_t _reg_addr, uint8_t* _buf, size_t _len)
        {
            return mg_i2c__write(num_, dev_addr_, _reg_addr, timeout_tick_, retry_count_, _buf, _len);
        }

    private:
        int num_ = -1;
        int dev_addr_ = -1;
        int timeout_tick_ = 1;
        int retry_count_ = 2;
    };
    
}
}
}

#endif // !MGPP_OS_LINUX_HARDWARE_I2C_H_INCLUDED
