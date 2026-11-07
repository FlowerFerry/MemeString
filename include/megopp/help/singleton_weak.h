
#ifndef MEGOPP_HELP_SINGLETON_WEAK_H_INCLUDED
#define MEGOPP_HELP_SINGLETON_WEAK_H_INCLUDED

#include <mutex>
#include <memory>
#include <atomic>
#include <stdexcept>

namespace mgpp { namespace help {

template <typename T>
struct singleton_weak
{
    
    static void install(const std::weak_ptr<T>& obj)
    {
        member& m = get_member();
        std::lock_guard<std::mutex> locker(m.mtx_);
        if (m.ready_.load(std::memory_order_acquire))
        {
            throw std::runtime_error("singleton_weak: instance already installed");
        }
        m.weak_ = obj;
        m.ready_.store(true, std::memory_order_release);
    }

    static std::shared_ptr<T> get_or_throw()
    {
        member& m = get_member();
        if (!m.ready_.load(std::memory_order_acquire))
        {
            throw std::runtime_error("singleton_weak: instance not installed");
        }

        std::shared_ptr<T> sp = m.weak_.lock();
        if (!sp) {
            throw std::runtime_error("singleton_weak: instance expired");
        }
        return sp;
    }

    static std::shared_ptr<T> get()
    {
        member& m = get_member();
        if (!m.ready_.load(std::memory_order_acquire))
        {
            return nullptr;
        }

        return m.weak_.lock();
    }

    static bool is_installed()
    {
        member& m = get_member();
        return m.ready_.load(std::memory_order_acquire);
    }

private:
    struct member
    {
        std::mutex mtx_;
        std::weak_ptr<T> weak_;
        std::atomic_bool ready_{ false };
    };

    static member& get_member()
    {
        static member m;
        return m;
    }
};

}}

#endif // !MEGOPP_HELP_SINGLETON_WEAK_H_INCLUDED
