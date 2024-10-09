
#ifndef MEGOPP_OS_LINUX_FD_H_INCLUDED
#define MEGOPP_OS_LINUX_FD_H_INCLUDED

#include <mego/predef/os/linux.h>

#if MG_OS__LINUX_AVAIL
#include <unistd.h>
#endif 

namespace mgpp {
namespace os_linux {

class fd 
{
    int fd_;
public:
    constexpr fd() noexcept
        : fd_(-1)
    {}

    constexpr explicit fd(int _fd) noexcept
        : fd_(_fd)
    {}

    fd(const fd&) = delete;
    fd& operator=(const fd&) = delete;

    inline fd(fd&& _other) noexcept
        : fd_(_other.fd_)
    {
        _other.fd_ = -1;
    }

    inline fd& operator=(fd&& _other) noexcept
    {
        if (this != &_other) {
            close();
            fd_ = _other.fd_;
            _other.fd_ = -1;
        }
        return *this;
    }

    ~fd()
    {
        close();
    }

    inline constexpr int raw() const noexcept
    {
        return fd_;
    }

    inline void close() noexcept
    {
        if (fd_ != -1) {
#if MG_OS__LINUX_AVAIL
            ::close(fd_);
#endif
            fd_ = -1;
        }
    }

};

}
}

#endif // !MEGOPP_OS_LINUX_FD_H_INCLUDED
