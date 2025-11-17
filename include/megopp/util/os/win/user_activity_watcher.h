
#ifndef MEGOPP_UTIL_OS_WIN_USER_ACTIVITY_WATCHER_H_INCLUDED
#define MEGOPP_UTIL_OS_WIN_USER_ACTIVITY_WATCHER_H_INCLUDED

#include <meme/string_fwd.h>
#include <mego/util/os/windows/windows_simplify.h>
#include <mego/predef/os/windows.h>
#include <megopp/err/err.h>

#include <megopp/util/scope_cleanup.h>

#include <functional>

#if MG_OS__WIN_AVAIL
#include <shellapi.h>
#include <WtsApi32.h>
#pragma comment(lib, "Wtsapi32.lib")

#include <XInput.h>
#pragma comment(lib, "XInput.lib")
#endif

namespace mgpp {
namespace os   {
namespace win  {

#if MG_OS__WIN_AVAIL

//! @brief User activity monitor structure for detecting user activity status.
//!
//! This structure provides a mechanism to detect the user's activity status (active or idle) 
//! and invoke a callback function when the status changes. Applicable to Windows systems.
struct usr_act_watcher
{
    enum class act_state 
    {
        none,
        act,
        idle
    };

    //! @brief Idle state callback function type definition.
    //!
    //! @param watcher Reference to the user activity watcher instance.
    //! @param state Current user activity state.
    //! @param error Error information (if any).
    typedef void(idle_fn_t)(usr_act_watcher&, act_state, const mgpp::err&);

    inline constexpr act_state current_state() const noexcept { return current_state_; }

    inline constexpr void set_idle_timeout_sec(int _sec) noexcept 
    {
        if (_sec < 1)
            _sec = 1;
        idle_timeout_sec_ = _sec;
    }

    inline void set_idle_callback(const std::function<idle_fn_t>& _fn)
    {
        fn_ = _fn;
    }

    //! @brief Check the user activity status once.
    //!
    //! This function checks the user's activity status and invokes the callback function if the status changes.
    void check_once() {
        if (!fn_)
            return;

        auto cleanup = megopp::util::scope_cleanup__create([&] 
        {
            if (current_state_ != act_state::act) {
                current_state_  = act_state::act;
                fn_(*this, act_state::act, {});
            }
        });

        DWORD sessionId = 0xFFFFFFFF;
        if (!ProcessIdToSessionId(GetCurrentProcessId(), &sessionId)) {
            fn_(*this, act_state::none, { MGEC__ERR, "ProcessIdToSessionId" });
            cleanup.cancel();
            return;
        }

        if (sessionId == 0) {
            // TO_DO
            return;
        }

        LASTINPUTINFO lastInputInfo;
        lastInputInfo.cbSize = sizeof(LASTINPUTINFO);
        if (!GetLastInputInfo(&lastInputInfo)) {
            fn_(*this, act_state::none, { MGEC__ERR, "GetLastInputInfo" });
            cleanup.cancel();
            return;
        }

        auto idle_time = 
            static_cast<int64_t>(GetTickCount64()) -
            static_cast<int64_t>(lastInputInfo.dwTime);
        if (idle_time < idle_timeout_sec_ * 1000)
        {
            return;
        }

        QUERY_USER_NOTIFICATION_STATE state_flag;
        HRESULT hr = SHQueryUserNotificationState(&state_flag);
        if (FAILED(hr)) {
            fn_(*this, act_state::none, { MGEC__ERR, "SHQueryUserNotificationState" });
            cleanup.cancel();
            return;
        }

        if (state_flag != QUNS_NOT_PRESENT
            && state_flag != QUNS_ACCEPTS_NOTIFICATIONS)
        {
            return;
        }
        
        for (int idx = 0; idx < XUSER_MAX_COUNT; ++idx)
        {
            XINPUT_STATE state;
            ZeroMemory(&state, sizeof(XINPUT_STATE));
            DWORD result = XInputGetState(idx, &state);
            if (result != ERROR_DEVICE_NOT_CONNECTED)
            {
                return;
            }
        }

        if (current_state_ != act_state::idle) {
            current_state_  = act_state::idle;
            fn_(*this, act_state::idle, {});
        }
        cleanup.cancel();
        
    }

private:
    act_state current_state_ = act_state::none;
    int idle_timeout_sec_ = 30;
    std::function<idle_fn_t> fn_;
};
#endif

}
}
}

#endif // !MEGOPP_UTIL_OS_WIN_USER_ACTIVITY_WATCHER_H_INCLUDED
