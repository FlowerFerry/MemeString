
#ifndef MEMEPP_CONVERT_STD_U16STRING_HPP_INCLUDED
#define MEMEPP_CONVERT_STD_U16STRING_HPP_INCLUDED

#include "memepp/string.hpp"
#include "memepp/string_view.hpp"
#include "memepp/convert/common_def.hpp"

#include <string>

namespace memepp  {
namespace convert {

	template<>
	struct from<::std::u16string>
	{
		static memepp::string from_object(const ::std::u16string& _s)
		{
			return memepp::string{
				reinterpret_cast<const uint16_t*>(_s.data()), static_cast<MemeInteger_t>(_s.size()) };
		}
		static memepp::string from_object(::std::u16string&& _s)
		{
			return memepp::string{
				reinterpret_cast<const uint16_t*>(_s.data()), static_cast<MemeInteger_t>(_s.size()) };
		}
	};

	//template<>
	//struct view<::std::u16string>
	//{
	//	static memepp::string_view view_object(const ::std::u16string& _s)
	//	{
	//		// User defined
	//	}
	//};

	template<>
	struct into<::std::u16string>
	{
		static ::std::u16string into_object(const memepp::string& _s)
		{
			auto frontSize = _s.u16char_size();
			std::u16string u16; u16.resize(frontSize);

			auto afterSize = MemeString_writeU16Chars(
				memepp::to_pointer(_s.native_handle()), (uint16_t*)(u16.data()));

			if (afterSize != frontSize)
				u16.resize(afterSize);
			return u16;
		}
		static ::std::u16string into_object(const memepp::string_view& _sv)
		{
			auto frontSize = _sv.u16char_size();
			std::u16string u16; u16.resize(frontSize);

			auto afterSize = MemeString_writeU16Chars(
				memepp::to_pointer(_sv.native_handle()), (uint16_t*)(u16.data()));

			if (afterSize != frontSize)
				u16.resize(afterSize);
			return u16;
		}
	};

}

	//inline memepp::string from(const std::u16string& _s)
	//{
	//	return memepp::string{
	//		reinterpret_cast<const uint16_t*>(_s.data()), static_cast<MemeInteger_t>(_s.size()) };
	//}

	//inline memepp::string from(std::u16string&& _s)
	//{
	//	return memepp::string{ 
	//		reinterpret_cast<const uint16_t*>(_s.data()), static_cast<MemeInteger_t>(_s.size())};
	//}
	//
	//template<>
	//inline std::u16string to<std::u16string>(const memepp::string& _s)
	//{
	//	auto frontSize = _s.u16char_size();
 //       std::u16string u16; u16.resize(frontSize);
	//	
 //       auto afterSize = MemeString_writeU16Chars(
	//		memepp::to_pointer(_s.native_handle()), (uint16_t*)(u16.data()));

	//	if (afterSize != frontSize)
	//		u16.resize(afterSize);
 //       return u16;
	//}

	//template<>
	//inline std::u16string to<std::u16string>(const memepp::string_view& _sv)
	//{
	//	auto frontSize = _sv.u16char_size();
 //       std::u16string u16; u16.resize(frontSize);

 //       auto afterSize = MemeString_writeU16Chars(
 //           memepp::to_pointer(_sv.native_handle()), (uint16_t*)(u16.data()));

 //       if (afterSize != frontSize)
	//		u16.resize(afterSize);
 //       return u16;
	//}

}

//inline memepp::string mm_from(const std::u16string& _s)
//{
//    return memepp::from(_s);
//}
//
//inline memepp::string mm_from(std::u16string&& _s)
//{
//    return memepp::from(std::move(_s));
//}


#endif // !MEMEPP_CONVERT_STD_U16STRING_HPP_INCLUDED
