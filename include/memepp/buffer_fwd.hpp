
#ifndef MEMEPP_BUFFER_FWD_HPP_INCLUDED
#define MEMEPP_BUFFER_FWD_HPP_INCLUDED

#include "meme/buffer_fwd.h"

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
	class buffer;
}

	enum class buffer_storage_t {
		small  = MemeBuffer_StorageType_small,
		medium = MemeBuffer_StorageType_medium,
		large  = MemeBuffer_StorageType_large,
		user   = MemeBuffer_StorageType_user,
		view_unsafe = MemeBuffer_UnsafeStorageType_view,
		// Storage reachable from the public constructors vs. from a C handle.
		//
		// The byte constructors that take a `buffer_storage_t _suggest` forward
		// straight to MemeBufferStack_initByU8bytesAndType(), so the hint is
		// honoured by MemeStringImpl_initSuggestType():
		//
		//  * small  — honoured while the payload fits the inline buffer; a
		//             larger payload is promoted to medium.
		//  * medium — honoured unconditionally (even past the default medium
		//             byte limit, which only applies to automatic selection).
		//  * large  — always produces large.
		//  * user   — cannot be produced by a byte construction; it is an
		//             ownership tier, not a size tier.
		//
		// The constructor without a hint calls MemeBufferStack_initByBytes(),
		// which forces large for every non-empty payload. An empty buffer is
		// small.
		//
		// user tier is import-only: MemeBufferStack's own takeover entry point
		// is commented out in meme/buffer.h, but buffer and string share the
		// same 24-byte stack layout, so the live
		// MemeStringStack_initTakeOverUserObject() (or the
		// mmstrstk_init_by_user() wrapper) produces a valid handle that
		// buffer(native_handle_type&&) imports as user tier. Copying such a
		// buffer shares the user block and defers the user destructor callback
		// until the last reference is released; reset()/to_large() detach into
		// an owned tier.
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
