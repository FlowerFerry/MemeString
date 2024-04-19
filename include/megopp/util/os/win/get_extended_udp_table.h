
#ifndef MEGOPP_UTIL_OS_WIN_GET_EXTENDED_UDP_TABLE_H_INCLUDED
#define MEGOPP_UTIL_OS_WIN_GET_EXTENDED_UDP_TABLE_H_INCLUDED

#include <mego/predef/os/windows.h>
#include <mego/err/ec.h>

#include <megopp/help/objbuf.h>

#include <vector>

#if MG_OS__WIN_AVAIL

#  include <winsock2.h>
#  include <ws2tcpip.h>
#  include <iphlpapi.h>
#  pragma comment( lib, "Iphlpapi.lib" )

#endif


namespace mgpp {
namespace os   {
namespace win  {
#if MG_OS__WIN_AVAIL
    template<typename _Table>
    struct __mib_udp_tb_traits
    {
        using row_type = void;
        using table_type = void;
        static constexpr ULONG af = AF_UNSPEC;
    };
    
    //struct mib_udptable_owner_pid
    //{
    //    inline const MIB_UDPTABLE_OWNER_PID* get() const noexcept 
    //    { 
    //        return reinterpret_cast<const MIB_UDPTABLE_OWNER_PID*>(buf.data()); 
    //    }

    //    std::vector<uint8_t> buf;
    //};

    //struct mib_udp6table_owner_pid
    //{
    //    inline const MIB_UDP6TABLE_OWNER_PID* get() const noexcept
    //    {
    //        return reinterpret_cast<const MIB_UDP6TABLE_OWNER_PID*>(buf.data());
    //    }

    //    std::vector<uint8_t> buf;
    //};

    //struct mib_udptable_owner_module
    //{
    //    inline const MIB_UDPTABLE_OWNER_MODULE* get() const noexcept
    //    {
    //        return reinterpret_cast<const MIB_UDPTABLE_OWNER_MODULE*>(buf.data());
    //    }

    //    std::vector<uint8_t> buf;
    //};

    //struct mib_udp6table_owner_module
    //{
    //    inline const MIB_UDP6TABLE_OWNER_MODULE* get() const noexcept
    //    {
    //        return reinterpret_cast<const MIB_UDP6TABLE_OWNER_MODULE*>(buf.data());
    //    }

    //    std::vector<uint8_t> buf;
    //};

    //struct mib_udptable
    //{
    //    inline const MIB_UDPTABLE* get() const noexcept
    //    {
    //        return reinterpret_cast<const MIB_UDPTABLE*>(buf.data());
    //    }

    //    std::vector<uint8_t> buf;
    //};

    //struct mib_udp6table
    //{
    //    inline const MIB_UDP6TABLE* get() const noexcept
    //    {
    //        return reinterpret_cast<const MIB_UDP6TABLE*>(buf.data());
    //    }

    //    std::vector<uint8_t> buf;
    //};
    
    template<>
    struct __mib_udp_tb_traits<MIB_UDPTABLE_OWNER_PID>
    {
        using row_type = MIB_UDPROW_OWNER_PID;
        using table_type = MIB_UDPTABLE_OWNER_PID;
        static constexpr UDP_TABLE_CLASS table_class = UDP_TABLE_OWNER_PID;
        static constexpr ULONG af = AF_INET;
    };

    template<>
    struct __mib_udp_tb_traits<MIB_UDPROW_OWNER_PID>
    {
        using row_type = MIB_UDPROW_OWNER_PID;
        using table_type = MIB_UDPTABLE_OWNER_PID;
        static constexpr UDP_TABLE_CLASS table_class = UDP_TABLE_OWNER_PID;
        static constexpr ULONG af = AF_INET;
    };

    template<>
    struct __mib_udp_tb_traits<MIB_UDP6TABLE_OWNER_PID>
    {
        using row_type = MIB_UDP6ROW_OWNER_PID;
        using table_type = MIB_UDP6TABLE_OWNER_PID;
        static constexpr UDP_TABLE_CLASS table_class = UDP_TABLE_OWNER_PID;
        static constexpr ULONG af = AF_INET6;
    };

    template<>
    struct __mib_udp_tb_traits<MIB_UDP6ROW_OWNER_PID>
    {
        using row_type = MIB_UDP6ROW_OWNER_PID;
        using table_type = MIB_UDP6TABLE_OWNER_PID;
        static constexpr UDP_TABLE_CLASS table_class = UDP_TABLE_OWNER_PID;
        static constexpr ULONG af = AF_INET6;
    };

    template<>
    struct __mib_udp_tb_traits<MIB_UDPTABLE_OWNER_MODULE>
    {
        using row_type = MIB_UDPROW_OWNER_MODULE;
        using table_type = MIB_UDPTABLE_OWNER_MODULE;
        static constexpr UDP_TABLE_CLASS table_class = UDP_TABLE_OWNER_MODULE;
        static constexpr ULONG af = AF_INET;
    };

    template<>
    struct __mib_udp_tb_traits<MIB_UDPROW_OWNER_MODULE>
    {
        using row_type = MIB_UDPROW_OWNER_MODULE;
        using table_type = MIB_UDPTABLE_OWNER_MODULE;
        static constexpr UDP_TABLE_CLASS table_class = UDP_TABLE_OWNER_MODULE;
        static constexpr ULONG af = AF_INET;
    };

    template<>
    struct __mib_udp_tb_traits<MIB_UDP6TABLE_OWNER_MODULE>
    {
        using row_type = MIB_UDP6ROW_OWNER_MODULE;
        using table_type = MIB_UDP6TABLE_OWNER_MODULE;
        static constexpr UDP_TABLE_CLASS table_class = UDP_TABLE_OWNER_MODULE;
        static constexpr ULONG af = AF_INET6;
    };

    template<>
    struct __mib_udp_tb_traits<MIB_UDP6ROW_OWNER_MODULE>
    {
        using row_type = MIB_UDP6ROW_OWNER_MODULE;
        using table_type = MIB_UDP6TABLE_OWNER_MODULE;
        static constexpr UDP_TABLE_CLASS table_class = UDP_TABLE_OWNER_MODULE;
        static constexpr ULONG af = AF_INET6;
    };

    template<>
    struct __mib_udp_tb_traits<MIB_UDPTABLE>
    {
        using row_type = MIB_UDPROW;
        using table_type = MIB_UDPTABLE;
        static constexpr UDP_TABLE_CLASS table_class = UDP_TABLE_BASIC;
        static constexpr ULONG af = AF_INET;
    };

    template<>
    struct __mib_udp_tb_traits<MIB_UDPROW>
    {
        using row_type = MIB_UDPROW;
        using table_type = MIB_UDPTABLE;
        static constexpr UDP_TABLE_CLASS table_class = UDP_TABLE_BASIC;
        static constexpr ULONG af = AF_INET;
    };

#ifdef _WS2IPDEF_
    template<>
    struct __mib_udp_tb_traits<MIB_UDP6TABLE>
    {
        using row_type = MIB_UDP6ROW;
        using table_type = MIB_UDP6TABLE;
        static constexpr UDP_TABLE_CLASS table_class = UDP_TABLE_BASIC;
        static constexpr ULONG af = AF_INET6;
    };
    
    template<>
    struct __mib_udp_tb_traits<MIB_UDP6ROW>
    {
        using row_type = MIB_UDP6ROW;
        using table_type = MIB_UDP6TABLE;
        static constexpr UDP_TABLE_CLASS table_class = UDP_TABLE_BASIC;
        static constexpr ULONG af = AF_INET6;
    };

#endif

    template<typename _Ty,
        typename = std::enable_if_t<
            !std::is_same_v<typename __mib_udp_tb_traits<_Ty>::table_type, void>>>
    inline help::objbuf<typename __mib_udp_tb_traits<_Ty>::table_type>
    get_extended_udp_table(BOOL _order, mgec_t* _ec = nullptr)
    {
        using result_t = help::objbuf<typename __mib_udp_tb_traits<_Ty>::table_type, std::allocator<uint8_t>>;

        DWORD dwSize = 0;
        DWORD dwResult = GetExtendedUdpTable(
            nullptr, &dwSize, _order, __mib_udp_tb_traits<_Ty>::af, __mib_udp_tb_traits<_Ty>::table_class, 0);
        if (dwResult != ERROR_INSUFFICIENT_BUFFER)
        {
            if (_ec)
                *_ec = mgec__from_sys_err(dwResult);
            return {};
        }

        std::vector<uint8_t> buffer(dwSize);
        dwResult = GetExtendedUdpTable(
            buffer.data(), &dwSize, _order, __mib_udp_tb_traits<_Ty>::af,
            __mib_udp_tb_traits<_Ty>::table_class, 0);
        if (dwResult != NO_ERROR)
        {
            if (_ec)
                *_ec = mgec__from_sys_err(dwResult);
            return {};
        }

        if (_ec)
            *_ec = 0;
        return result_t{ std::move(buffer) };
    }

    template<typename _Ty,
        typename _Fn,
        typename = std::enable_if_t<
            std::is_invocable_v<_Fn, const typename __mib_udp_tb_traits<_Ty>::row_type&>
            || !std::is_same_v<typename __mib_udp_tb_traits<_Ty>::table_type, void>>>
    inline mgec_t enum_extended_udp_table(BOOL _order, DWORD& _numEntries, _Fn&& _fn)
    {
        DWORD dwResult = 0;
        DWORD dwSize = 0;
        dwResult = GetExtendedUdpTable(nullptr, &dwSize, _order, 
            __mib_udp_tb_traits<_Ty>::af, __mib_udp_tb_traits<_Ty>::table_class, 0);
        if (dwResult != ERROR_INSUFFICIENT_BUFFER)
        {
            return MGEC__ERR;
        }

        std::vector<uint8_t> buffer(dwSize);
        dwResult = GetExtendedUdpTable(buffer.data(), &dwSize, _order, 
            __mib_udp_tb_traits<_Ty>::af, __mib_udp_tb_traits<_Ty>::table_class, 0);
        if (dwResult != NO_ERROR)
        {
            return MGEC__ERR;
        }

        auto table  = reinterpret_cast<const typename __mib_udp_tb_traits<_Ty>::table_type*>(buffer.data());
        _numEntries = table->dwNumEntries;
        for (DWORD idx = 0; idx < table->dwNumEntries; ++idx)
        {
            if constexpr (std::is_same_v<
                std::invoke_result_t<_Fn, const typename __mib_udp_tb_traits<_Ty>::row_type&>, bool>)
            {
                if (!_fn(table->table[idx]))
                    break;
            }
            else {
                _fn(table->table[idx]);
            }
        }

        return MGEC__OK;
    }  

    template<typename _Ty,
        typename _Fn,
        typename = std::enable_if_t<
        std::is_invocable_v<_Fn, const typename __mib_udp_tb_traits<_Ty>::row_type&>
        || !std::is_same_v<typename __mib_udp_tb_traits<_Ty>::table_type, void>>>
        inline mgec_t enum_extended_udp_table(BOOL _order, _Fn&& _fn)
    {
        DWORD numEntries = 0;
        return enum_extended_udp_table<_Ty>(_order, numEntries, std::forward<_Fn>(_fn));
    }
#endif
}
}
}

#endif // !MEGOPP_UTIL_OS_WIN_GET_EXTENDED_UDP_TABLE_H_INCLUDED
