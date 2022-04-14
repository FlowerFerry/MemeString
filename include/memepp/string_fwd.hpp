
#ifndef MEMEPP_STRING_FWD_HPP_INCLUDED
#define MEMEPP_STRING_FWD_HPP_INCLUDED

#include "meme/string_fwd.h"

namespace memepp {

	class string;

	enum class string_storage_type {
		small = MemeString_StorageType_small,
		large = MemeString_StorageType_large,
		user = MemeString_StorageType_user
	};

	inline MemeString_t __to_object__(MemeStringStack_t& _s) noexcept
	{
		return reinterpret_cast<MemeString_t>(&_s);
	}
	inline MemeString_Const_t __to_object__(const MemeStringStack_t& _s) noexcept
	{
		return reinterpret_cast<MemeString_Const_t>(&_s);
	}

};

#endif // !MEMEPP_STRING_FWD_HPP_INCLUDED