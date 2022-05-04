
#ifndef MEMEPP_COMPARE_STD_STRING_HPP_INCLUDED
#define MEMEPP_COMPARE_STD_STRING_HPP_INCLUDED

#include <string>
#include <memepp/string.hpp>

namespace memepp {

	template<class Traits, class Allocator>
	inline int compare(const std::basic_string<char, Traits, Allocator>& _lhs, const memepp::string& _rhs) noexcept
	{
		return _lhs.compare(_rhs.data());
	}

	template<class Traits, class Allocator>
	inline int compare(const memepp::string& _lhs, const std::basic_string<char, Traits, Allocator>& _rhs) noexcept
	{
		return - memepp::compare(_rhs, _lhs);
	}

	inline int compare(const std::string& _lhs, size_t _pos1, size_t _count1, const memepp::string& _rhs)
	{
		return _lhs.compare(_pos1, _count1, _rhs.data(), _rhs.size());
	}

	template<class Traits, class Allocator>
	inline int compare(const std::basic_string<char, Traits, Allocator>& _lhs, size_t _pos1, size_t _count1,
		const memepp::string& _rhs, size_t _pos2, size_t _count2)
	{
		return _lhs.compare(_pos1, _count1, _rhs.data(), _rhs.size());
	}

	template<class Traits, class Allocator>
	inline bool operator==(const memepp::string& _lhs, const std::basic_string<char, Traits, Allocator>& _rhs) noexcept
	{
		int result = 0;
		MemeString_isEqual(
			to_pointer(_lhs.native_handle()), _rhs.data(), _rhs.size(), &result);
		return result;
	}

	template<class Traits, class Allocator>
	inline bool operator==(const std::basic_string<char, Traits, Allocator>& _lhs, const memepp::string& _rhs) noexcept
	{
		int result = 0;
		MemeString_isEqual(
			to_pointer(_rhs.native_handle()), _lhs.data(), _lhs.size(), &result);
		return result;
	}

	template<class Traits, class Allocator>
	inline bool operator!=(const memepp::string& _lhs, const std::basic_string<char, Traits, Allocator>& _rhs) noexcept
	{
		int result = 0;
		MemeString_isEqual(
			to_pointer(_lhs.native_handle()), _rhs.data(), _rhs.size(), &result);
		return result == 0;
	}

	template<class Traits, class Allocator>
	inline bool operator!=(const std::basic_string<char, Traits, Allocator>& _lhs, const memepp::string& _rhs) noexcept
	{
		int result = 0;
		MemeString_isEqual(
			to_pointer(_rhs.native_handle()), _lhs.data(), _lhs.size(), &result);
		return result == 0;
	}


	//template<class Traits, class Allocator>
	//int string::compare(const std::basic_string<char, Traits, Allocator>& _str) const noexcept
	//{
	//	return - memepp::compare(_str, *this);
	//}

};

#endif // !MEMEPP_COMPARE_STD_STRING_HPP_INCLUDED
