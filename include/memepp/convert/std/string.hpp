
#ifndef MEMEPP_CONVERT_STD_STRING_HPP_INCLUDED
#define MEMEPP_CONVERT_STD_STRING_HPP_INCLUDED

#include "meme/string.h"
#include "memepp/string_def.hpp"
#include <string>

namespace memepp {

	inline memepp::string from(const std::string& _s);
	inline memepp::string from(std::string&& _s);

}; // namespace memepp


namespace memepp {

	inline memepp::string from(const std::string& _s)
	{
		return memepp::string(_s.data(), _s.size());
	}

	inline memepp::string from(std::string&& _s)
	{
		static const auto destruct_func = [](void* _object) { delete reinterpret_cast<std::string*>(_object); };
		static const auto data_func = [](const void* _object) { return reinterpret_cast<const std::string*>(_object)->data(); };
		static const auto size_func = [](const void* _object) { return reinterpret_cast<const std::string*>(_object)->size(); };
		
		if (_s.empty())
			return {};

		memepp::string out;
		auto obj = new std::string(std::move(_s));
		auto ret = MemeStringStack_initTakeOverUserObject(
			const_cast<MemeStringStack_t*>(&(out.native_handle())), MEME_STRING__OBJECT_SIZE,
			obj, destruct_func, data_func, size_func);
		if (ret) {
			destruct_func(obj);
			return {};
		}
		return out;
	}

}; // namespace memepp

#endif // !MEMEPP_CONVERT_STD_STRING_HPP_INCLUDED