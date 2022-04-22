
#ifndef MEMEPP_CONVERT_STD_INITIALIZER_LIST_HPP_INCLUDED
#define MEMEPP_CONVERT_STD_INITIALIZER_LIST_HPP_INCLUDED

#include "meme/string.h"
#include "memepp/string_def.hpp"
#include <vector>

namespace memepp {

	inline memepp::string from(std::vector<uint8_t>&& _list);

};

namespace memepp {

	inline memepp::string from(std::vector<uint8_t>&& _v)
	{
		static const auto destruct_func = [](void* _object) { 
			delete reinterpret_cast<std::vector<uint8_t>*>(_object);
		};
		static const auto data_func = [](const void* _object) { 
			return reinterpret_cast<const char*>(reinterpret_cast<const std::vector<uint8_t>*>(_object)->data()); 
		};
		static const auto size_func = [](const void* _object) { 
			return reinterpret_cast<const std::vector<uint8_t>*>(_object)->size() - 1; 
		};

		if (_v.empty())
			return {};

		if (_v.capacity() != 0) {
			if (_v.back() != 0)
				_v.push_back(0);

			memepp::string out;
			auto obj = new std::vector<uint8_t>(std::move(_v));
			auto ret = MemeStringStack_initTakeOverUserObject(
				const_cast<MemeStringStack_t*>(&(out.native_handle())), MEME_STRING__OBJECT_SIZE,
				obj, destruct_func, data_func, size_func);
			if (ret) {
				destruct_func(obj);
				return {};
			}
			return out;
		}
		else {
			return { _v.data(), _v.size() };
		}
	}

};

#endif // !MEMEPP_CONVERT_STD_INITIALIZER_LIST_HPP_INCLUDED
