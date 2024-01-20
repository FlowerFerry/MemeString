
#ifndef MGPP_HELP_TRAVERSER_H_INCLUDED
#define MGPP_HELP_TRAVERSER_H_INCLUDED

#include <optional>

namespace mgpp {
namespace help {

    template <typename _Container, typename _SharedLocker, typename _Pred>
    std::optional<typename _Container::key_type> foreach_with_small_step(
        _SharedLocker& _locker,
        const _Container& _container, 
        const std::optional<typename _Container::key_type>& _begin, 
        size_t _count,
        _Pred _pred)
    {
        if (!_begin.has_value()) {
            if (_locker.owns_lock())
                _locker.unlock();
            auto it = _container.find(*_begin);
            if (it == _container.end())
            {
                return std::nullopt;
            }
            else {
                auto end = it;
                std::advance(end, _count);
                for (; it != end; ++it)
                {
                    if (!_pred(*it))
                    {
                        return std::nullopt;
                    }
                }
                return *end;
            }
        }
        else {
            auto it = _container.begin();
            auto end = it;
            std::advance(end, _count);
            for (; it != end; ++it)
            {
                if (!_pred(*it))
                {
                    return std::nullopt;
                }
            }
            return *end;
        }
    }

}
}

#endif // !MGPP_HELP_TRAVERSER_H_INCLUDED
