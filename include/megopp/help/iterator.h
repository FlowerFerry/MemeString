
#ifndef MEGOPP_HELP_ITERATOR_H_INCLUDED
#define MEGOPP_HELP_ITERATOR_H_INCLUDED

#include <utility>

namespace mgpp {
namespace help {
    
template <typename T, typename U>
inline U & iter_get_val(std::pair<T, U> & pair) 
{
    return pair.second;
}

template <typename T>
inline T & iter_get_val(T & val) 
{
    return val;
}

}
}; // namespace mgpp

#endif // !MEGOPP_HELP_ITERATOR_H_INCLUDED
