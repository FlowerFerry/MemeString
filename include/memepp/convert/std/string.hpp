
#ifndef MEMEPP_CONVERT_STD_STRING_HPP_INCLUDED
#define MEMEPP_CONVERT_STD_STRING_HPP_INCLUDED

#include "meme/string.h"
#include "memepp/string.hpp"
#include "memepp/string_view.hpp"
#include "memepp/string_builder.hpp"
#include "memepp/convert/common_def.hpp"

#include <string>

namespace memepp {

	//inline memepp::string from(const std::string& _s);
	//inline memepp::string from(std::string&& _s);

}; // namespace memepp

namespace memepp  {
namespace convert {
	
	template<>
	struct from<::std::string>
	{
		static memepp::string from_object(const ::std::string& _s)
		{
			return memepp::string{ _s.data(), static_cast<mmint_t>(_s.size()) };
		}
		
		static memepp::string from_object(::std::string&& _s)
		{
			static const auto destruct_func = [](void* _object) { delete reinterpret_cast<::std::string*>(_object); };
			static const auto data_func = [](const void* _object) { return reinterpret_cast<const ::std::string*>(_object)->data(); };
			static const auto size_func = [](const void* _object) { return reinterpret_cast<const ::std::string*>(_object)->size(); };

			if (_s.size() < MemeStringOption_getStorageMediumLimit() + MMSTR__OBJ_SIZE)
			{
				return memepp::string{ _s.data(), static_cast<mmint_t>(_s.size()) };
			}
			else {
				memepp::string out;
				auto obj = new ::std::string{ std::move(_s) };
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
	};

	template<>
	struct view<::std::string>
	{
		static memepp::string_view view_object(const ::std::string& _s)
		{
			return memepp::string_view{ _s.data(), static_cast<mmint_t>(_s.size()) };
		}
	};

	template<>
	struct into<::std::string>
	{
		static ::std::string into_object(const memepp::string& _s)
		{
			return ::std::string{ _s.data(), static_cast<size_t>(_s.size()) };
		}
		static ::std::string into_object(const memepp::string_view& _s)
		{
			return ::std::string{ _s.data(), static_cast<size_t>(_s.size()) };
		}
	};

}

	//inline memepp::string from(const std::string& _s)
	//{
	//	return memepp::string(_s.data(), static_cast<MemeInteger_t>(_s.size()));
	//}
	//
	//inline memepp::string from(std::string&& _s)
	//{
	//	static const auto destruct_func = [](void* _object) { delete reinterpret_cast<std::string*>(_object); };
	//	static const auto data_func = [](const void* _object) { return reinterpret_cast<const std::string*>(_object)->data(); };
	//	static const auto size_func = [](const void* _object) { return reinterpret_cast<const std::string*>(_object)->size(); };
	//
	//	if (_s.size() < MemeStringOption_getStorageMediumLimit() + MMSTR__OBJ_SIZE)
 //       {
	//		return memepp::string{ _s.data(), static_cast<mmint_t>(_s.size()) };
 //       }
 //       else {
	//		memepp::string out;
	//		auto obj = new std::string{ std::move(_s) };
	//		auto ret = MemeStringStack_initTakeOverUserObject(
	//			const_cast<MemeStringStack_t*>(&(out.native_handle())), MMSTR__OBJ_SIZE,
	//			obj, destruct_func, data_func, size_func);
	//		if (ret) {
	//			destruct_func(obj);
	//			return {};
	//		}
	//		return out;
 //       }
	//}

    //! @brief Convert memepp::string to std::string.
	//! 
	//! One of the reasons why this function is not safe is that when it is used in a child dynamic library, 
	//! the allocated memepp::string is passed to the parent process and its life cycle is longer than the life cycle of the dynamic library, which will cause problems.
    //! @param _s std::string to convert
    //! @return memepp::string
	//inline memepp::string fromUnsafe(std::string&& _s)
	//{
	//	static const auto destruct_func = [](void* _object) { delete reinterpret_cast<std::string*>(_object); };
	//	static const auto data_func = [](const void* _object) { return reinterpret_cast<const std::string*>(_object)->data(); };
	//	static const auto size_func = [](const void* _object) { return reinterpret_cast<const std::string*>(_object)->size(); };
	//	
	//	if (_s.empty())
	//		return {};
	//	else if (_s.size() < MEME_STRING__OBJECT_SIZE)
	//	{
	//		return memepp::string(_s.data(), static_cast<MemeInteger_t>(_s.size()));
	//	}
 //       else if (_s.size() < 16 * sizeof(intptr_t))
 //       {
 //           return memepp::string(_s.data(), static_cast<MemeInteger_t>(_s.size()), 
	//			memepp::string_storage_t::medium);
 //       }

	//	memepp::string out;
	//	auto obj = new std::string(std::move(_s));
	//	auto ret = MemeStringStack_initTakeOverUserObject(
	//		const_cast<MemeStringStack_t*>(&(out.native_handle())), MEME_STRING__OBJECT_SIZE,
	//		obj, destruct_func, data_func, size_func);
	//	if (ret) {
	//		destruct_func(obj);
	//		return {};
	//	}
	//	return out;
	//}

	//inline memepp::string_view view(const std::string& _s)
	//{
	//	return memepp::string_view { _s.data(), static_cast<MemeInteger_t>(_s.size()) };
	//}

	//template<>
	//inline std::string to<std::string>(const memepp::string& _s)
	//{
	//	return std::string { _s.data(), static_cast<size_t>(_s.size()) };
	//}

	//template<>
	//inline std::string to<std::string>(const memepp::string_view& _sv)
	//{
	//	return std::string { _sv.data(), static_cast<size_t>(_sv.size()) };
	//}
    
}; // namespace memepp

//inline memepp::string mm_from(const std::string& _s)
//{
//	return memepp::from(_s);
//}
//
//inline memepp::string mm_from(std::string&& _s)
//{
//	return memepp::from(std::move(_s));
//}


#endif // !MEMEPP_CONVERT_STD_STRING_HPP_INCLUDED