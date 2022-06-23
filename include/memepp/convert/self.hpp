
#ifndef MEMEPP_CONVERT_SELF_HPP_INCLUDED
#define MEMEPP_CONVERT_SELF_HPP_INCLUDED

#include "memepp/string.hpp"

namespace memepp {

	inline memepp::string from(const MemeStringStack_t& _other)
	{
		memepp::string s;
		MemeStringStack_assign_v02(
			const_cast<MemeStringStack_t*>(&(s.native_handle())),
			MEME_STRING__OBJECT_SIZE, 
			memepp::to_pointer(_other));

		return s;
	}

};

#endif // !MEMEPP_CONVERT_SELF_HPP_INCLUDED
