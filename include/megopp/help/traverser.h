
#ifndef MGPP_HELP_TRAVERSER_H_INCLUDED
#define MGPP_HELP_TRAVERSER_H_INCLUDED

#include <type_traits>
#include <optional>
#include <megopp/util/scope_cleanup.h>
#include <megopp/help/iterator.h>

namespace mgpp {
namespace help {

    template <typename _Container, typename _SharedLocker, typename _Ty, typename _Pred>
    inline void foreach_with_small_step
        (
            _SharedLocker& _locker,
            const _Container& _container, 
            const std::optional<_Ty>& _begin,
            size_t _count,
            std::optional<_Ty>& _out,
            _Pred _pred
        )
    {
        auto has_lock = _locker.owns_lock();
        MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() {
            if (has_lock) {
                if (!_locker.owns_lock())
                    _locker.lock();
                
            }
            else {
                if (_locker.owns_lock())
                    _locker.unlock();
            }
        });

        if (!has_lock)
            _locker.lock();
        typename _Container::const_iterator it;
        if (_begin.has_value()) {
            auto& container = const_cast<_Container&>(_container);
            it = container.find(mgpp::help::iter_key(*_begin));
            if (it == container.cend())
            {
                _out = std::nullopt;
                return;
            }
            
        }
        else {
            it = _container.cbegin();
        }
        for (; it != _container.cend() && _count --> 0; ++it)
        {
            if (!_pred(*it)) {
                if (++it == _container.cend())
                {
                    _out = std::nullopt;
                    return;
                }
                
                if constexpr (std::is_same<_Ty, typename _Container::key_type>::value)
                    _out = it->first;
                else {
                    _out = std::make_pair(it->first, it->second);
                }
                return;
            }
        }

        if (it == _container.cend())
        {
            _out = std::nullopt;
            return;
        }
        
        if constexpr (std::is_same<_Ty, typename _Container::key_type>::value)
            _out = it->first;
        else
            _out = std::make_pair(it->first, it->second);
    }

}
}

#endif // !MGPP_HELP_TRAVERSER_H_INCLUDED
