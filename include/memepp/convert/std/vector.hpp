
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
		
		if (_v.size() < MemeStringOption_getStorageMediumLimit() + MMSTR__OBJ_SIZE)
		{
			return memepp::string{ _v.data(), static_cast<mmint_t>(_v.size()) };
		}
		else {
			if (_v.back() != 0)
                _v.push_back(0);

			memepp::string out;
			auto obj = new std::vector<uint8_t>{ std::move(_v) };
			auto ret = MemeStringStack_initTakeOverUserObject(
				const_cast<MemeStringStack_t*>(&(out.native_handle())), MMSTR__OBJ_SIZE,
				obj, destruct_func, data_func, size_func);
			if (ret) {
				destruct_func(obj);
				return {};
			}
			return out;
		}
	}

    //! @brief Convert std::vector<uint8_t> to memepp::string.
	//! 
    //! One of the reasons why this function is not safe is that when it is used in a child dynamic library,
    //! the allocated memepp::string is passed to the parent process and its life cycle is longer than the life cycle of the dynamic library, which will cause problems.
    //! @param _v std::vector<uint8_t> to convert
    //! @return memepp::string
	//inline memepp::string fromUnsafe(std::vector<uint8_t>&& _v)
	//{
	//	static const auto destruct_func = [](void* _object) { 
	//		delete reinterpret_cast<std::vector<uint8_t>*>(_object);
	//	};
	//	static const auto data_func = [](const void* _object) { 
	//		return reinterpret_cast<const char*>(reinterpret_cast<const std::vector<uint8_t>*>(_object)->data()); 
	//	};
	//	static const auto size_func = [](const void* _object) { 
	//		return reinterpret_cast<const std::vector<uint8_t>*>(_object)->size() - 1; 
	//	};

	//	if (_v.empty())
	//		return {};

	//	if (static_cast<MemeInteger_t>(_v.size()) < MemeStringOption_getStorageMediumLimit())
	//	{
	//		//if (_v.back() != 0)
	//		_v.push_back(0);

	//		memepp::string out;
	//		auto obj = new std::vector<uint8_t>(std::move(_v));
	//		auto ret = MemeStringStack_initTakeOverUserObject(
	//			const_cast<MemeStringStack_t*>(&(out.native_handle())), MEME_STRING__OBJECT_SIZE,
	//			obj, destruct_func, data_func, size_func);
	//		if (ret) {
	//			destruct_func(obj);
	//			return {};
	//		}
	//		return out;
	//	}
	//	else {
	//		return { _v.data(), static_cast<MemeInteger_t>(_v.size()) };
	//	}
	//}

};

inline memepp::string mm_from(std::vector<uint8_t>&& _list)
{
    return memepp::from(std::move(_list));
}


#endif // !MEMEPP_CONVERT_STD_INITIALIZER_LIST_HPP_INCLUDED
