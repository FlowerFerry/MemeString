
#ifndef MEMEPP_STRING_FWD_HPP_INCLUDED
#define MEMEPP_STRING_FWD_HPP_INCLUDED

#include "meme/string_fwd.h"
#include "meme/string_builder_fwd.h"

#include <memepp/common.hpp>

#undef small

#ifndef MEMEPP__IMPL_INLINE
#	ifdef MEMEPP__IMPL_SEPARATE
#		define MEMEPP__IMPL_INLINE 
#	else
#		define MEMEPP__IMPL_INLINE inline
#	endif
#endif 

namespace memepp {
inline namespace MMPP_NAMESPACE {
    class string;
}

	enum class string_storage_t {
		small  = MemeString_StorageType_small,
		medium = MemeString_StorageType_medium,
		large  = MemeString_StorageType_large,
		user   = MemeString_StorageType_user,
	};
    using str_strg_t = string_storage_t;

	enum class split_behavior_t {
		keep_empty_parts = MemeFlag_KeepEmptyParts,
		skip_empty_parts = MemeFlag_SkipEmptyParts
	};
    using split_behav_t = split_behavior_t;

	enum class case_sensitivity_t : mmint_t
	{
		all_insensitive = MemeFlag_AllInsensitive,
		case_sensitive  = MemeFlag_CaseSensitive,
	 	simplified_traditional_sensitive = MemeFlag_SimplifiedTraditionalSensitive,
        all_sensitive   = static_cast<int32_t>(MemeFlag_AllSensitive)
	};
    using case_sensit_t = case_sensitivity_t;

    inline constexpr case_sensit_t operator|(case_sensit_t _a, case_sensit_t _b) noexcept
    {
        return static_cast<case_sensit_t>(static_cast<int>(_a) | static_cast<int>(_b));
    }
	
    inline constexpr case_sensit_t operator&(case_sensit_t _a, case_sensit_t _b) noexcept
    {
        return static_cast<case_sensit_t>(static_cast<int>(_a) & static_cast<int>(_b));
    }

    inline constexpr case_sensit_t operator^(case_sensit_t _a, case_sensit_t _b) noexcept
    {
        return static_cast<case_sensit_t>(static_cast<int>(_a) ^ static_cast<int>(_b));
    }

    inline constexpr case_sensit_t operator~(case_sensit_t _a) noexcept
    {
        return static_cast<case_sensit_t>(~static_cast<int>(_a));
    }

    inline case_sensit_t& operator|=(case_sensit_t& _a, case_sensit_t _b) noexcept
    {
        return _a = _a | _b;
    }

    inline case_sensit_t& operator&=(case_sensit_t& _a, case_sensit_t _b) noexcept
    {
        return _a = _a & _b;
    }

    inline case_sensit_t& operator^=(case_sensit_t& _a, case_sensit_t _b) noexcept
    {
        return _a = _a ^ _b;
    }

	inline mmstr_ptr_t to_pointer(mmstrstk_t& _s) noexcept
	{
		return reinterpret_cast<mmstr_ptr_t>(&_s);
	}

	inline mmstr_cptr_t to_pointer(const mmstrstk_t& _s) noexcept
	{
		return reinterpret_cast<mmstr_cptr_t>(&_s);
	}

	inline mmsbldr_ptr_t to_pointer(mmsbldrstk_t& _s) noexcept
	{
		return reinterpret_cast<mmsbldr_ptr_t>(&_s);
	}

    inline mmsbldr_cptr_t to_pointer(const mmsbldrstk_t& _s) noexcept
    {
        return reinterpret_cast<mmsbldr_cptr_t>(&_s);
    }

    static_assert(sizeof(mmstrstk_t) == MMSTR__OBJ_SIZE, "mmstrstk_t size mismatch");
    static_assert(static_cast<mmint_t>(case_sensit_t::all_sensitive) == -1, 
        "case_sensitivity_t all_sensitive value mismatch");
};

#endif // !MEMEPP_STRING_FWD_HPP_INCLUDED