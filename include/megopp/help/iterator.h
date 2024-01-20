
#ifndef MEGOPP_HELP_ITERATOR_H_INCLUDED
#define MEGOPP_HELP_ITERATOR_H_INCLUDED

#include <utility>

namespace mgpp {
namespace help {
    
template <typename T, typename U>
inline U & iter_value(std::pair<T, U> & pair) 
{
    return pair.second;
}

template <typename T, typename U>
inline const U & iter_value(const std::pair<T, U> & pair) 
{
    return pair.second;
}

template <typename T>
inline T & iter_value(T & val) 
{
    return val;
}

template <typename T, typename U>
inline U & iter_key(std::pair<T, U> & pair) 
{
    return pair.first;
}

template <typename T, typename U>
inline const T & iter_key(const std::pair<T, U> & pair) 
{
    return pair.first;
}

template <typename T>
inline T & iter_key(T & val) 
{
    return val;
}

}
}; // namespace mgpp

#endif // !MEGOPP_HELP_ITERATOR_H_INCLUDED
