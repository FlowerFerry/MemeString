
#ifndef MEGOPP_UTIL_OS_WIN_USER_ACTIVITY_WATCHER_H_INCLUDED
#define MEGOPP_UTIL_OS_WIN_USER_ACTIVITY_WATCHER_H_INCLUDED

#include <meme/string_fwd.h>
#include <mego/util/os/windows/windows_simplify.h>
#include <mego/predef/os/windows.h>
#include <megopp/err/err.h>

#include <megopp/util/scope_cleanup.h>

#include <functional>
#include <string>

#include <yyjson.h>

#if MG_OS__WIN_AVAIL
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include <shellapi.h>
#include <WtsApi32.h>
#pragma comment(lib, "Wtsapi32.lib")

#include <XInput.h>
#pragma comment(lib, "XInput.lib")
#endif

namespace mgpp {
namespace os   {
namespace win  {

namespace details {

// Serialize idle_ms into JSON: {"idle_ms":N}
// Returns bytes written (>0), or -1 on failure. buf is NOT null-terminated.
inline int usr_act_encode_packet(char* buf, size_t cap, uint64_t idle_ms)
{
    yyjson_mut_doc* doc = yyjson_mut_doc_new(nullptr);
    if (!doc) return -1;

    yyjson_mut_val* root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);
    yyjson_mut_obj_add_uint(doc, root, "idle_ms", idle_ms);

    size_t wlen = 0;
    char* json = yyjson_mut_write(doc, YYJSON_WRITE_NOFLAG, &wlen);
    yyjson_mut_doc_free(doc);

    if (!json || wlen == 0 || wlen >= cap) {
        if (json) free(json);
        return -1;
    }
    memcpy(buf, json, wlen);
    free(json);
    return static_cast<int>(wlen);
}

// Deserialize a packet produced by usr_act_encode_packet.
// Returns true and sets out_idle_ms on success; false on any parse error.
// Tolerant of extra fields — only "idle_ms" (integer) is required.
inline bool usr_act_decode_packet(char* buf, int len, uint64_t& out_idle_ms)
{
    if (!buf || len <= 0) return false;

    yyjson_doc* doc = yyjson_read_opts(
        buf, static_cast<size_t>(len),
        YYJSON_READ_NOFLAG, nullptr, nullptr);
    if (!doc) return false;

    yyjson_val* root = yyjson_doc_get_root(doc);
    yyjson_val* val  = yyjson_obj_get(root, "idle_ms");
    if (!val || !yyjson_is_int(val)) {
        yyjson_doc_free(doc);
        return false;
    }
    out_idle_ms = yyjson_get_uint(val);
    yyjson_doc_free(doc);
    return true;
}

} // namespace details

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

    inline void set_udp_port(uint16_t _port) noexcept
    {
        udp_port_ = _port;
    }

    //! @brief Bind a non-blocking UDP socket on 127.0.0.1:udp_port_ to receive reports from usr_act_reporter.
    //! Must be called before check_once() is used from Session 0.
    mgpp::err init_udp_server()
    {
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
            return { MGEC__ERR, "WSAStartup" };

        SOCKET s = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (s == INVALID_SOCKET) {
            WSACleanup();
            return { MGEC__ERR, "socket" };
        }

        // Set non-blocking so check_once() never waits
        u_long nb = 1;
        if (ioctlsocket(s, FIONBIO, &nb) == SOCKET_ERROR) {
            ::closesocket(s);
            WSACleanup();
            return { MGEC__ERR, "ioctlsocket(FIONBIO)" };
        }

        sockaddr_in addr{};
        addr.sin_family      = AF_INET;
        addr.sin_port        = htons(udp_port_);
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

        if (::bind(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
            ::closesocket(s);
            WSACleanup();
            return { MGEC__ERR, "bind" };
        }

        udp_socket_ = s;
        return {};
    }

    void cleanup_udp_server() noexcept
    {
        if (udp_socket_ != INVALID_SOCKET) {
            ::closesocket(udp_socket_);
            udp_socket_ = INVALID_SOCKET;
            WSACleanup();
        }
        udp_last_idle_ms_   = -1;
        udp_last_recv_tick_ = 0;
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
            if (udp_socket_ == INVALID_SOCKET) {
                return;
            }

            // Drain the UDP receive buffer (non-blocking: FIONBIO was set in init_udp_server)
            for (;;) {
                char buf[256];
                int n = ::recv(udp_socket_, buf, static_cast<int>(sizeof(buf) - 1), 0);
                if (n == SOCKET_ERROR)
                    break;
                uint64_t pkt = 0;
                if (details::usr_act_decode_packet(buf, n, pkt)) {
                    udp_last_idle_ms_   = static_cast<int64_t>(pkt);
                    udp_last_recv_tick_ = GetTickCount64();
                }
            }

            // No data yet, or data is stale (reporter is gone) — treat as active
            if (udp_last_idle_ms_ < 0 ||
                (GetTickCount64() - udp_last_recv_tick_) >
                    static_cast<ULONGLONG>(idle_timeout_sec_) * 2000ULL)
            {
                return;
            }

            if (udp_last_idle_ms_ < static_cast<int64_t>(idle_timeout_sec_) * 1000) {
                return;
            }

            if (current_state_ != act_state::idle) {
                current_state_  = act_state::idle;
                fn_(*this, act_state::idle, {});
            }
            cleanup.cancel();
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
            static_cast<int64_t>(GetTickCount()) -
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
    int       idle_timeout_sec_ = 30;
    uint16_t  udp_port_         = 54320;
    SOCKET    udp_socket_       = INVALID_SOCKET;
    int64_t   udp_last_idle_ms_ = -1;   // -1 = no data received yet
    ULONGLONG udp_last_recv_tick_ = 0;  // GetTickCount64() at last recv
    std::function<idle_fn_t> fn_;
};

//! @brief User input reporter that sends idle-time data to usr_act_watcher via UDP.
//!
//! Run this in an interactive user session alongside the service that hosts usr_act_watcher.
//! Call init() once to set up the socket, then call report_once() periodically (e.g. every second).
//! Call cleanup() when done.
struct usr_act_reporter
{
    inline void set_udp_port(uint16_t _port) noexcept { udp_port_ = _port; }
    inline void set_udp_host(const std::string& _host) { udp_host_ = _host; }

    //! @brief Create UDP socket and connect to the watcher (127.0.0.1:udp_port_ by default).
    mgpp::err init()
    {
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
            return { MGEC__ERR, "WSAStartup" };

        SOCKET s = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (s == INVALID_SOCKET) {
            WSACleanup();
            return { MGEC__ERR, "socket" };
        }

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port   = htons(udp_port_);
        if (inet_pton(AF_INET, udp_host_.c_str(), &addr.sin_addr) != 1) {
            ::closesocket(s);
            WSACleanup();
            return { MGEC__ERR, "inet_pton" };
        }

        if (::connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
            ::closesocket(s);
            WSACleanup();
            return { MGEC__ERR, "connect" };
        }

        udp_socket_ = s;
        return {};
    }

    void cleanup() noexcept
    {
        if (udp_socket_ != INVALID_SOCKET) {
            ::closesocket(udp_socket_);
            udp_socket_ = INVALID_SOCKET;
            WSACleanup();
        }
    }

    //! @brief Sample the current idle time and send it to the watcher.
    //! Non-blocking: a single non-waiting send() call.
    void report_once() noexcept
    {
        if (udp_socket_ == INVALID_SOCKET)
            return;

        LASTINPUTINFO li;
        li.cbSize = sizeof(LASTINPUTINFO);
        if (!GetLastInputInfo(&li))
            return;

        uint64_t idle_ms =
            static_cast<uint64_t>(GetTickCount64()) -
            static_cast<uint64_t>(li.dwTime);

        char buf[256];
        int len = details::usr_act_encode_packet(buf, sizeof(buf), idle_ms);
        if (len > 0)
            ::send(udp_socket_, buf, len, 0);
    }

private:
    uint16_t    udp_port_   = 54320;
    std::string udp_host_   = "127.0.0.1";
    SOCKET      udp_socket_ = INVALID_SOCKET;
};

#endif

}
}
}

#endif // !MEGOPP_UTIL_OS_WIN_USER_ACTIVITY_WATCHER_H_INCLUDED
