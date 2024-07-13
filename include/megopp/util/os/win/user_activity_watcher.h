
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

//! @brief 用户活动监视器结构体，用于检测用户是否处于活动状态。
//!
//! 该结构体提供了一种机制，用于检测用户的活动状态（活动或空闲），并在状态改变时
//! 调用回调函数。适用于 Windows 系统。
struct usr_act_watcher
{
    enum class act_state 
    {
        none,
        act,
        idle
    };

    //! @brief 空闲状态回调函数类型定义。
    //!
    //! @param watcher 用户活动监视器实例的引用。
    //! @param state 当前的用户活动状态。
    //! @param error 错误信息（如果有）。
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

    //! @brief 检查一次用户活动状态。
    //!
    //! 该函数检查用户的活动状态，如果状态发生变化，则调用回调函数。
    void check_once() {
        if (!fn_)
            return;

        LASTINPUTINFO lastInputInfo;
        lastInputInfo.cbSize = sizeof(LASTINPUTINFO);
        if (!GetLastInputInfo(&lastInputInfo)) {
            fn_(*this, act_state::none, { MGEC__ERR, "GetLastInputInfo" });
            return;
        }

        auto cleanup = megopp::util::scope_cleanup__create([&] 
        {
            if (current_state_ != act_state::act)
            {
                current_state_ = act_state::act;
                fn_(*this, act_state::act, {});
            }
        });

        auto idle_time = static_cast<int64_t>(GetTickCount()) -
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

        //PWTS_SESSION_INFOW pSessionInfo = NULL;
        //DWORD sessionCount = 0;
        //DWORD sessionId = 0;
        //// 枚举所有会话
        //if (WTSEnumerateSessionsW(
        //    WTS_CURRENT_SERVER_HANDLE, 0, 1, &pSessionInfo, &sessionCount) == FALSE)
        //{
        //    return;
        //}
        //MEGOPP_UTIL__ON_SCOPE_CLEANUP([&] { WTSFreeMemory(pSessionInfo); });

        //for (DWORD idx = 0; idx < sessionCount; idx++)
        //{
        //    auto& info = pSessionInfo[idx];
        //    if (info.State == WTSActive)
        //    {
        //        info.pWinStationName;
        //        if (wcsstr(info.pWinStationName, L"RDP"))
        //            return;
        //        
        //    }
        //}

        if (current_state_ != act_state::idle)
        {
            current_state_ = act_state::idle;
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
