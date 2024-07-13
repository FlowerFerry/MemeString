
#ifndef MEGOPP_HELP_TRIVIAL_CREATE_H_INCLUDED
#define MEGOPP_HELP_TRIVIAL_CREATE_H_INCLUDED

#include <string.h>
#include <type_traits>

namespace mgpp {
namespace help {
    
    template<typename _Object>
    inline _Object trivial_create_impl(std::true_type)
    {
        _Object obj;
        memset(&obj, 0, sizeof(_Object));
        return  obj;
    }

    template<typename _Object>
    inline _Object trivial_create_impl(std::false_type)
    {
        return _Object{};
    }

    template<typename _Object>
    inline _Object trivial_create()
    {
        return trivial_create_impl<_Object>(std::is_trivial<_Object>{});
    }

}
}

#endif // !MEGOPP_HELP_TRIVIAL_CREATE_H_INCLUDED
