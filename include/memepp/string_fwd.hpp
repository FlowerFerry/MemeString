
#ifndef MEMEPP_STRING_FWD_HPP_INCLUDED
#define MEMEPP_STRING_FWD_HPP_INCLUDED

#include "meme/string_fwd.h"

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

	enum class string_storage_type {
		small = MemeString_StorageType_small,
		large = MemeString_StorageType_large,
		user  = MemeString_StorageType_user
	};

	enum class split_behavior_t {
		keep_empty_parts = MemeFlag_KeepEmptyParts,
		skip_empty_parts = MemeFlag_SkipEmptyParts
	};

	inline MemeString_t to_pointer(MemeStringStack_t& _s) noexcept
	{
		return reinterpret_cast<MemeString_t>(&_s);
	}

	inline MemeString_Const_t to_pointer(const MemeStringStack_t& _s) noexcept
	{
		return reinterpret_cast<MemeString_Const_t>(&_s);
	}

};

#endif // !MEMEPP_STRING_FWD_HPP_INCLUDED