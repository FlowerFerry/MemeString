
#ifndef MEMEPP_VARIABLE_BUFFER_FWD_HPP_INCLUDED
#define MEMEPP_VARIABLE_BUFFER_FWD_HPP_INCLUDED

#include "meme/variable_buffer_fwd.h"

#include <memepp/common.hpp>

namespace memepp {
namespace MMPP_NAMESPACE {
	class variable_buffer;
}
	using variable_buffer = MMPP_NAMESPACE::variable_buffer;

	inline MemeVariableBuffer_Const_t to_pointer(const MemeVariableBufferStack_t& _stack)
	{
		return reinterpret_cast<MemeVariableBuffer_Const_t>(&_stack);
	}

	inline MemeVariableBuffer_t to_pointer(MemeVariableBufferStack_t& _stack)
	{
		return reinterpret_cast<MemeVariableBuffer_t>(&_stack);
	}

};

#endif // !MEMEPP_VARIABLE_BUFFER_FWD_HPP_INCLUDED
