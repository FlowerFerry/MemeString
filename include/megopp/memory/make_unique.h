
#ifndef MEGOPP_MEMORY_MAKE_UNIQUE_H_INCLUDED
#define MEGOPP_MEMORY_MAKE_UNIQUE_H_INCLUDED

#include <mego/predef/lang/version.h>

#include <memory>
#include <utility>

namespace mgpp {
namespace mem {

/* std::make_unique is C++14.  Provide the same entry point for older standards
   so call sites do not need conditional compilation. */
#if MG_LANG__CXX14_AVAIL

template<typename _Ty, typename... _Args>
inline std::unique_ptr<_Ty> make_unique(_Args&&... _args)
{
    return std::make_unique<_Ty>(std::forward<_Args>(_args)...);
}

#else

template<typename _Ty, typename... _Args>
inline std::unique_ptr<_Ty> make_unique(_Args&&... _args)
{
    return std::unique_ptr<_Ty>(new _Ty(std::forward<_Args>(_args)...));
}

#endif

}; // namespace mem
}; // namespace mgpp

#endif // !MEGOPP_MEMORY_MAKE_UNIQUE_H_INCLUDED
