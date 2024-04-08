
#ifndef MEGOPP_HELP_ITERATOR_H_INCLUDED
#define MEGOPP_HELP_ITERATOR_H_INCLUDED

#include "type_traits.h"
#include <iterator>

namespace mgpp {
namespace help {
    
template <typename It>
struct iter_traits
{
    using mapped_type = typename std::iterator_traits<It>::value_type;
};

template <typename It, 
    typename = std::enable_if_t<megopp::is_pair_v<typename std::iterator_traits<It>::value_type>>>
struct iter_traits
{
    using mapped_type = typename std::iterator_traits<It>::value_type::second_type;
};

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
