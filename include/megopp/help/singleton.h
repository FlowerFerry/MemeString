
#ifndef MEGOPP_HELP_SINGLETON_H_INCLUDED
#define MEGOPP_HELP_SINGLETON_H_INCLUDED

namespace mgpp { namespace help {

template<typename _Object>
struct singleton
{
    using object_t = _Object;

    static object_t& instance()
    {
        static object_t obj;
        return obj;
    }
};

}} // namespace mgpp::help

#endif // !MEGOPP_HELP_SINGLETON_H_INCLUDED
