
#ifndef MEGOPP_UTIL_SCOPE_CLEANUP_HPP_INCLUDED
#define MEGOPP_UTIL_SCOPE_CLEANUP_HPP_INCLUDED

#include <mego/predef/helper_macros.h>

#pragma once

#include <utility>

namespace megopp {
namespace util {

	template<typename _Func>
	struct scope_cleanup
	{
		scope_cleanup(_Func && _fn) :
			fn_(std::move(_fn)), canceled_(false)
                {}

        scope_cleanup(scope_cleanup && _other):
            fn_(std::move(_other.fn_)),
            canceled_(_other.canceled_)
                {}

		~scope_cleanup() { if (!canceled_) fn_(); }

		inline constexpr void early_exec() noexcept { if (!canceled_) fn_(); canceled_ = true; }
        inline constexpr void cancel() noexcept { canceled_ = true; }
        inline constexpr bool is_cancel() const noexcept { return canceled_; }

	private:
		scope_cleanup(const scope_cleanup &) = delete;
                scope_cleanup& operator= (const scope_cleanup&) = delete;
                scope_cleanup& operator= (scope_cleanup&&) = delete;

		_Func fn_;
		bool canceled_;
	};

	template<typename _Func>
	inline scope_cleanup<_Func> scope_cleanup__create(_Func && _fn)
	{
		return std::move(_fn);
	}

};
};

//! @def MEGOPP_UTIL__ON_SCOPE_CLEANUP
//! @brief The macro is to let the destructor of a guard object call a user specified cleanup action at the end of a scope.
//! @code
//!  // can be used like :
//!  {
//!	 	int * p = new int;
//!	 	MEGOPP_UTIL__ON_SCOPE_CLEANUP( [&] { delete p; } );
//!  }
//! @endcode
#define MEGOPP_UTIL__ON_SCOPE_CLEANUP(callback) \
	auto MEGO__JOIN(MEGOPP_UTIL__SCOPE_CLEANUP_, __LINE__) = ::megopp::util::scope_cleanup__create(callback)

#endif // !MEGOPP_UTIL_SCOPE_CLEANUP_HPP_INCLUDED
