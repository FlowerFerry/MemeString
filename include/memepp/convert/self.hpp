
#ifndef MEMEPP_CONVERT_SELF_HPP_INCLUDED
#define MEMEPP_CONVERT_SELF_HPP_INCLUDED

#include "meme/string.h"

#include "memepp/string_def.hpp"
#include "memepp/string_view_def.hpp"

#include <utility>

namespace memepp {

	inline memepp::string from(const MemeStringStack_t& _other)
	{
		memepp::string s;
		MemeStringStack_assign(
			const_cast<MemeStringStack_t*>(&(s.native_handle())),
			MEME_STRING__OBJECT_SIZE, 
			memepp::to_pointer(_other));

		return s;
	}

	inline memepp::string from(memepp::string&& _other)
	{
		return std::move(_other);
	}

	inline memepp::string from(const memepp::string& _other)
	{
		return _other;
	}

	inline memepp::string from(const memepp::string_view& _other)
	{
		return _other.to_string();
	}

	inline memepp::string_view view(const MemeStringStack_t& _other)
	{
		memepp::string_view sv;
		MemeStringStack_assign(
			const_cast<MemeStringStack_t*>(&(sv.native_handle())),
			MEME_STRING__OBJECT_SIZE,
			memepp::to_pointer(_other));

		return sv;
	}

	inline memepp::string_view view(const memepp::string& _other)
	{
		return memepp::string_view{ _other };
	}

	inline memepp::string_view view(memepp::string_view&& _other)
	{
		return memepp::string_view{ _other };
	}

	inline memepp::string_view view(const memepp::string_view& _other)
	{
		return memepp::string_view{ _other };
	}

};

inline memepp::string mm_from(const MemeStringStack_t& _s)
{
    return memepp::from(_s);
}

inline memepp::string mm_from(memepp::string&& _s)
{
    return std::move(_s);
}

inline memepp::string mm_from(const memepp::string& _s)
{
    return _s;
}

inline memepp::string mm_from(const memepp::string_view& _s)
{
    return _s.to_string();
}

#include <memepp/convert/common.hpp>
#endif // !MEMEPP_CONVERT_SELF_HPP_INCLUDED
