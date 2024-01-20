
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
        _Container::const_iterator it;
        if (_begin.has_value()) {
            auto& container = const_cast<_Container&>(_container);
            it = container.find(mgpp::help::iter_key(*_begin));
            if (it == container.end())
            {
                _out = std::nullopt;
                return;
            }
            
        }
        else {
            it = _container.begin();
        }
        auto end = it;
        std::advance(end, _count);
        for (; it != end; ++it)
        {
            if (!_pred(*it)) {
                if (++it == _container.end())
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

        if (end == _container.end())
        {
            _out = std::nullopt;
            return;
        }
        
        if constexpr (std::is_same<_Ty, typename _Container::key_type>::value)
            _out = end->first;
        else
            _out = std::make_pair(end->first, end->second);
    }

}
}

#endif // !MGPP_HELP_TRAVERSER_H_INCLUDED
