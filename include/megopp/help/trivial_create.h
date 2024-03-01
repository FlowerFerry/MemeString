
#ifndef MEGOPP_HELP_TRIVIAL_CREATE_H_INCLUDED
#define MEGOPP_HELP_TRIVIAL_CREATE_H_INCLUDED

#include <string.h>
#include <type_traits>

namespace mgpp {
namespace help {
    
    template<typename _Object>
    inline typename std::enable_if<!std::is_trivial<_Object>::value, _Object>::type trivial_create()
    {
        return _Object{};
    }

    template<typename _Object>
    inline typename std::enable_if<std::is_trivial<_Object>::value, _Object>::type trivial_create()
    {
        _Object obj;
        memset(&obj, 0, sizeof(_Object));
        return  obj;
    }

}
}

#endif // !MEGOPP_HELP_TRIVIAL_CREATE_H_INCLUDED
