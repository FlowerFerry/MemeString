
#ifndef MEGOPP_UTIL_OS_WIN_ENUM_USN_RECORD_H_INCLUDED
#define MEGOPP_UTIL_OS_WIN_ENUM_USN_RECORD_H_INCLUDED

#include <mego/predef/os/windows.h>
#include <stdint.h>

#if MG_OS__WIN_AVAIL
#include <winioctl.h>
#endif 

#include <type_traits>

namespace mgpp {
namespace os   {
namespace win  {
#if MG_OS__WIN_AVAIL

template<typename _Fn,
    typename = std::enable_if_t<std::is_invocable_v<_Fn, const USN_RECORD_UNION&>>>
inline void enum_usn_record_buffer(const uint8_t* _buffer, size_t _size, _Fn&& _fn)
{
    auto ptr = _buffer + sizeof(USN);
    auto end = _buffer + _size;
    const USN_RECORD_UNION* record = reinterpret_cast<const USN_RECORD_UNION*>(ptr);
    while (ptr + record->Header.RecordLength < end) 
    {
        record = reinterpret_cast<const USN_RECORD_UNION*>(ptr);

        if constexpr (std::is_same_v<std::invoke_result_t<_Fn, const USN_RECORD_UNION&>, bool>) 
        {
            if (!_fn(*record))
                break;
        }
        else
            _fn(*record);
        
        ptr += record->Header.RecordLength;
    }
}

#endif
}
}
}   

#endif // !MEGOPP_UTIL_OS_WIN_ENUM_USN_RECORD_H_INCLUDED
