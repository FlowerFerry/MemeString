
#ifndef MEMEPP_STRING_FWD_HPP_INCLUDED
#define MEMEPP_STRING_FWD_HPP_INCLUDED

#include "meme/string_fwd.h"
#include "meme/string_builder_fwd.h"

#undef small

#ifndef MEMEPP__IMPL_INLINE
#	ifdef MEMEPP__IMPL_SEPARATE
#		define MEMEPP__IMPL_INLINE 
#	else
#		define MEMEPP__IMPL_INLINE inline
#	endif
#endif 

namespace memepp {

	class string;

	enum class string_storage_t {
		small  = MemeString_StorageType_small,
		medium = MemeString_StorageType_medium,
		large  = MemeString_StorageType_large,
		user   = MemeString_StorageType_user,
	};

	enum class split_behavior_t {
		keep_empty_parts = MemeFlag_KeepEmptyParts,
		skip_empty_parts = MemeFlag_SkipEmptyParts
	};

	enum case_sensitivity_t
	{
		all_insensitive = MemeFlag_AllInsensitive,
		case_sensitive  = MemeFlag_CaseSensitive,
	 	simplified_traditional_sensitive = MemeFlag_SimplifiedTraditionalSensitive,
		all_sensitive   = MemeFlag_AllSensitive
	};

	inline MemeString_t to_pointer(MemeStringStack_t& _s) noexcept
	{
		return reinterpret_cast<MemeString_t>(&_s);
	}

	inline MemeString_Const_t to_pointer(const MemeStringStack_t& _s) noexcept
	{
		return reinterpret_cast<MemeString_Const_t>(&_s);
	}

	inline mmsbldr_t to_pointer(mmsbldrstk_t& _s) noexcept
	{
		return reinterpret_cast<mmsbldr_t>(&_s);
	}

    inline mmsbldr_const_t to_pointer(const mmsbldrstk_t& _s) noexcept
    {
        return reinterpret_cast<mmsbldr_const_t>(&_s);
    }

};

#endif // !MEMEPP_STRING_FWD_HPP_INCLUDED