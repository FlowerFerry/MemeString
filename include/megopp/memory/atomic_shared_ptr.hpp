
#ifndef MGMEM_ATOMIC_SHARED_PTR_HPP_INCLUDED
#define MGMEM_ATOMIC_SHARED_PTR_HPP_INCLUDED

#include <memory>
#include <atomic>

namespace mgpp {
namespace mem {

#ifdef __cpp_lib_atomic_shared_ptr
    template<typename _Ty>
    using atomic_shared_ptr = std::atomic<std::shared_ptr<_Ty>>;
#else

//! @brief The partial template specialization of std::atomic for std::shared_ptr<T> allows users to manipulate shared_ptr objects atomically.
//! @see http://zh.cppreference.com/w/cpp/memory/shared_ptr/atomic
//! @see http://zh.cppreference.com/w/cpp/memory/shared_ptr/atomic2
	template<typename _Ty>
	class atomic_shared_ptr
	{
	public:
		typedef atomic_shared_ptr<_Ty> this_type;

		constexpr atomic_shared_ptr() noexcept = default;

		inline atomic_shared_ptr(std::shared_ptr<_Ty> _desired) noexcept
		{
			this->store(_desired);
		}

		atomic_shared_ptr(const atomic_shared_ptr&) = delete;

		void operator=(const atomic_shared_ptr&) = delete;
		inline atomic_shared_ptr& operator=(std::shared_ptr<_Ty> _desired) noexcept
		{
			this->store(_desired);
            return *this;
		}

		inline bool is_lock_free() const noexcept
		{
			return std::atomic_is_lock_free(&p_);
		}

		inline void store(std::shared_ptr<_Ty> _desired,
			std::memory_order _order = std::memory_order_seq_cst) noexcept
		{
			std::atomic_store_explicit(&p_, _desired, _order);
		}

		inline std::shared_ptr<_Ty> load(std::memory_order _order = std::memory_order_seq_cst) const noexcept
		{
			return std::atomic_load_explicit(&p_, _order);
		}

		inline operator std::shared_ptr<_Ty>() const noexcept { return load(); }

		inline std::shared_ptr<_Ty> exchange(std::shared_ptr<_Ty> _desired,
			std::memory_order _order = std::memory_order_seq_cst) noexcept
		{
			return std::atomic_exchange_explicit(&p_, _desired, _order);
		}

		inline bool compare_exchange_strong(std::shared_ptr<_Ty>& _expected, std::shared_ptr<_Ty> _desired,
			std::memory_order _success, std::memory_order _failure) noexcept
		{
			return std::atomic_compare_exchange_strong_explicit(&p_, &_expected, _desired, _success, _failure);
		}

		inline bool compare_exchange_weak(std::shared_ptr<_Ty>& _expected, std::shared_ptr<_Ty> _desired,
			std::memory_order _success, std::memory_order _failure) noexcept
		{
			return std::atomic_compare_exchange_weak_explicit(&p_, &_expected, _desired, _success, _failure);
		}

		inline bool compare_exchange_strong(std::shared_ptr<_Ty>& _expected, std::shared_ptr<_Ty> _desired,
			std::memory_order _order = std::memory_order_seq_cst) noexcept
		{
			return std::atomic_compare_exchange_strong_explicit(&p_, &_expected, _desired, _order, _order);
		}

		inline bool compare_exchange_weak(std::shared_ptr<_Ty>& _expected, std::shared_ptr<_Ty> _desired,
			std::memory_order _order = std::memory_order_seq_cst) noexcept
		{
			return std::atomic_compare_exchange_weak_explicit(&p_, &_expected, _desired, _order, _order);
		}
	private:
		std::shared_ptr<_Ty> p_;
	};
#endif
};
};

#endif // !MGMEM_ATOMIC_SHARED_PTR_HPP_INCLUDED
