
#ifndef MEMEPP_BUFFER_FWD_HPP_INCLUDED
#define MEMEPP_BUFFER_FWD_HPP_INCLUDED

#include "meme/buffer_fwd.h"

#undef small

#ifndef MEMEPP__IMPL_INLINE
#	ifdef MEMEPP__IMPL_SEPARATE
#		define MEMEPP__IMPL_INLINE 
#	else
#		define MEMEPP__IMPL_INLINE inline
#	endif
#endif 

namespace memepp {

	class buffer;

	enum class buffer_storage_type {
		small = MemeBuffer_StorageType_small,
		large = MemeBuffer_StorageType_large,
		user  = MemeBuffer_StorageType_user
	};

	inline MemeBuffer_t to_pointer(MemeBufferStack_t& _s) noexcept
	{
		return reinterpret_cast<MemeBuffer_t>(&_s);
	}

	inline MemeBuffer_Const_t to_pointer(const MemeBufferStack_t& _s) noexcept
	{
		return reinterpret_cast<MemeBuffer_Const_t>(&_s);
	}

};

#endif // !MEMEPP_BUFFER_FWD_HPP_INCLUDED
