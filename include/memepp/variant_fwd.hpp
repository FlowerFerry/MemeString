
#ifndef MEMEPP_VARIANT_FWD_HPP_INCLUDED
#define MEMEPP_VARIANT_FWD_HPP_INCLUDED

#include <meme/variant_fwd.h>

namespace memepp {

    class variant;

	inline mmvar_ptr_t to_pointer(mmvarstk_t& _s) noexcept
	{
		return reinterpret_cast<mmvar_ptr_t>(&_s);
	}

	inline mmvar_cptr_t to_pointer(const mmvarstk_t& _s) noexcept
	{
		return reinterpret_cast<mmvar_cptr_t>(&_s);
	}

    static_assert(sizeof(mmvarstk_t) == MMVAR__OBJ_SIZE, "mmvarstk_t size mismatch");
}

#endif // !MEMEPP_VARIANT_FWD_HPP_INCLUDED
