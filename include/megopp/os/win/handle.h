
#ifndef MEGOPP_OS_WIN_HANDLE_H_INCLUDED
#define MEGOPP_OS_WIN_HANDLE_H_INCLUDED

#include <mego/util/os/windows/windows_simplify.h>

namespace mgpp {
namespace os {
namespace win {

#if MG_OS__WIN_AVAIL
class handle
{
    HANDLE handle_;
public:
    constexpr handle() noexcept
        : handle_(INVALID_HANDLE_VALUE)
    {}

    constexpr explicit handle(HANDLE handle) noexcept
        : handle_(handle)
    {}

    handle(const handle&) = delete;
    handle& operator=(const handle&) = delete;

    inline handle(handle&& other) noexcept
        : handle_(other.handle_)
    {
        other.handle_ = INVALID_HANDLE_VALUE;
    }

    inline handle& operator=(handle&& other) noexcept
    {
        if (this != &other) {
            close();
            handle_ = other.handle_;
            other.handle_ = INVALID_HANDLE_VALUE;
        }
        return *this;
    }

    ~handle()
    {
        close();
    }

    inline constexpr HANDLE raw() const noexcept
    {
        return handle_;
    }

    inline void close() noexcept
    {
        if (handle_ != INVALID_HANDLE_VALUE) {
            ::CloseHandle(handle_);
            handle_ = INVALID_HANDLE_VALUE;
        }
    }

};
#endif

}
}
}

#endif // !MEGOPP_OS_WIN_HANDLE_H_INCLUDED
