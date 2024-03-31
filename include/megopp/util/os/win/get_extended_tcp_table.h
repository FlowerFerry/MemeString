
#ifndef MEGOPP_UTIL_OS_WIN_GET_EXTENDED_TCP_TABLE_H_INCLUDED
#define MEGOPP_UTIL_OS_WIN_GET_EXTENDED_TCP_TABLE_H_INCLUDED

#include <mego/predef/os/windows.h>
#include <mego/err/ec.h>

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
    struct __mib_tcp_tb_traits
    {
        using row_type = void;
        using table_type = void;
        static constexpr ULONG af = AF_UNSPEC;
    };
    
    template<>
    struct __mib_tcp_tb_traits<MIB_TCPTABLE_OWNER_PID>
    {
        using row_type = MIB_TCPROW_OWNER_PID;
        using table_type = MIB_TCPTABLE_OWNER_PID;
        static constexpr TCP_TABLE_CLASS table_class = TCP_TABLE_OWNER_PID_ALL;
        static constexpr ULONG af = AF_INET;
    };

    template<>
    struct __mib_tcp_tb_traits<MIB_TCPROW_OWNER_PID>
    {
        using row_type = MIB_TCPROW_OWNER_PID;
        using table_type = MIB_TCPTABLE_OWNER_PID;
        static constexpr TCP_TABLE_CLASS table_class = TCP_TABLE_OWNER_PID_ALL;
        static constexpr ULONG af = AF_INET;
    };

    template<>
    struct __mib_tcp_tb_traits<MIB_TCP6TABLE_OWNER_PID>
    {
        using row_type = MIB_TCP6ROW_OWNER_PID;
        using table_type = MIB_TCP6TABLE_OWNER_PID;
        static constexpr TCP_TABLE_CLASS table_class = TCP_TABLE_OWNER_PID_ALL;
        static constexpr ULONG af = AF_INET6;
    };

    template<>
    struct __mib_tcp_tb_traits<MIB_TCP6ROW_OWNER_PID>
    {
        using row_type = MIB_TCP6ROW_OWNER_PID;
        using table_type = MIB_TCP6TABLE_OWNER_PID;
        static constexpr TCP_TABLE_CLASS table_class = TCP_TABLE_OWNER_PID_ALL;
        static constexpr ULONG af = AF_INET6;
    };

    template<>
    struct __mib_tcp_tb_traits<MIB_TCPTABLE_OWNER_MODULE>
    {
        using row_type = MIB_TCPROW_OWNER_MODULE;
        using table_type = MIB_TCPTABLE_OWNER_MODULE;
        static constexpr TCP_TABLE_CLASS table_class = TCP_TABLE_OWNER_MODULE_ALL;
        static constexpr ULONG af = AF_INET;
    };

    template<>
    struct __mib_tcp_tb_traits<MIB_TCPROW_OWNER_MODULE>
    {
        using row_type = MIB_TCPROW_OWNER_MODULE;
        using table_type = MIB_TCPTABLE_OWNER_MODULE;
        static constexpr TCP_TABLE_CLASS table_class = TCP_TABLE_OWNER_MODULE_ALL;
        static constexpr ULONG af = AF_INET;
    };

    template<>
    struct __mib_tcp_tb_traits<MIB_TCP6TABLE_OWNER_MODULE>
    {
        using row_type = MIB_TCP6ROW_OWNER_MODULE;
        using table_type = MIB_TCP6TABLE_OWNER_MODULE;
        static constexpr TCP_TABLE_CLASS table_class = TCP_TABLE_OWNER_MODULE_ALL;
        static constexpr ULONG af = AF_INET6;
    };

    template<>
    struct __mib_tcp_tb_traits<MIB_TCP6ROW_OWNER_MODULE>
    {
        using row_type = MIB_TCP6ROW_OWNER_MODULE;
        using table_type = MIB_TCP6TABLE_OWNER_MODULE;
        static constexpr TCP_TABLE_CLASS table_class = TCP_TABLE_OWNER_MODULE_ALL;
        static constexpr ULONG af = AF_INET6;
    };

    template<>
    struct __mib_tcp_tb_traits<MIB_TCPTABLE>
    {
        using row_type = MIB_TCPROW;
        using table_type = MIB_TCPTABLE;
        static constexpr TCP_TABLE_CLASS table_class = TCP_TABLE_BASIC_ALL;
        static constexpr ULONG af = AF_INET;
    };

    template<>
    struct __mib_tcp_tb_traits<MIB_TCPROW>
    {
        using row_type = MIB_TCPROW;
        using table_type = MIB_TCPTABLE;
        static constexpr TCP_TABLE_CLASS table_class = TCP_TABLE_BASIC_ALL;
        static constexpr ULONG af = AF_INET;
    };

#ifdef _WS2IPDEF_
    template<>
    struct __mib_tcp_tb_traits<MIB_TCP6TABLE>
    {
        using row_type = MIB_TCP6ROW;
        using table_type = MIB_TCP6TABLE;
        static constexpr TCP_TABLE_CLASS table_class = TCP_TABLE_BASIC_ALL;
        static constexpr ULONG af = AF_INET6;
    };
    
    template<>
    struct __mib_tcp_tb_traits<MIB_TCP6ROW>
    {
        using row_type = MIB_TCP6ROW;
        using table_type = MIB_TCP6TABLE;
        static constexpr TCP_TABLE_CLASS table_class = TCP_TABLE_BASIC_ALL;
        static constexpr ULONG af = AF_INET6;
    };

#endif

    template<typename _Ty,
        TCP_TABLE_CLASS _TableClass, 
        typename _Fn,
        typename = std::enable_if_t<
            std::is_invocable_v<_Fn, const typename __mib_tcp_tb_traits<_Ty>::row_type&>
            || !std::is_same_v<typename __mib_tcp_tb_traits<_Ty>::table_type, void>>>
    inline mgec_t enum_extended_tcp_table(BOOL _order, DWORD& _numEntries, _Fn&& _fn)
    {
        DWORD dwResult = 0;
        DWORD dwSize = 0;
        dwResult = GetExtendedTcpTable(nullptr, &dwSize, _order, __mib_tcp_tb_traits<_Ty>::af, _TableClass, 0);
        if (dwResult != ERROR_INSUFFICIENT_BUFFER)
        {
            return MGEC__ERR;
        }

        std::vector<char> buffer(dwSize);
        dwResult = GetExtendedTcpTable(buffer.data(), &dwSize, _order, __mib_tcp_tb_traits<_Ty>::af, _TableClass, 0);
        if (dwResult != NO_ERROR)
        {
            return MGEC__ERR;
        }

        auto table  = reinterpret_cast<const typename __mib_tcp_tb_traits<_Ty>::table_type*>(buffer.data());
        _numEntries = table->dwNumEntries;
        for (DWORD idx = 0; idx < table->dwNumEntries; ++idx)
        {
            if constexpr (std::is_same_v<
                std::invoke_result_t<_Fn, const typename __mib_tcp_tb_traits<_Ty>::row_type&>, bool>)
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
        std::is_invocable_v<_Fn, const typename __mib_tcp_tb_traits<_Ty>::row_type&>
        || !std::is_same_v<typename __mib_tcp_tb_traits<_Ty>::table_type, void>>>
        inline mgec_t enum_extended_tcp_table(BOOL _order, DWORD& _numEntries, _Fn&& _fn)
    {
        return enum_extended_tcp_table<_Ty, __mib_tcp_tb_traits<_Ty>::table_class>(_order, _numEntries, std::forward<_Fn>(_fn));
    }

    template<typename _Ty,
        TCP_TABLE_CLASS _TableClass,
        typename _Fn,
        typename = std::enable_if_t<
        std::is_invocable_v<_Fn, const typename __mib_tcp_tb_traits<_Ty>::row_type&>
        || !std::is_same_v<typename __mib_tcp_tb_traits<_Ty>::table_type, void>>>
        inline mgec_t enum_extended_tcp_table(BOOL _order, _Fn&& _fn)
    {
        DWORD dwNumEntries = 0;
        return enum_extended_tcp_table<_Ty, _TableClass>(_order, dwNumEntries, std::forward<_Fn>(_fn));
    }
    
    template<typename _Ty,
        typename _Fn,
        typename = std::enable_if_t<
        std::is_invocable_v<_Fn, const typename __mib_tcp_tb_traits<_Ty>::row_type&>
        || !std::is_same_v<typename __mib_tcp_tb_traits<_Ty>::table_type, void>>>
        inline mgec_t enum_extended_tcp_table(BOOL _order, _Fn&& _fn)
    {
        return enum_extended_tcp_table<_Ty, __mib_tcp_tb_traits<_Ty>::table_class>(_order, std::forward<_Fn>(_fn));
    }
#endif
}
}
}

#endif // !MEGOPP_UTIL_OS_WIN_GET_EXTENDED_TCP_TABLE_H_INCLUDED
