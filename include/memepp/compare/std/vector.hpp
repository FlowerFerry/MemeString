
#ifndef MEMEPP_COMPARE_STD_VECTOR_HPP_INCLUDED
#define MEMEPP_COMPARE_STD_VECTOR_HPP_INCLUDED

#include <vector>
#include <memepp/string.hpp>

namespace memepp {

	template<class Allocator>
	inline bool operator==(const memepp::string& _lhs, const std::vector<char, Allocator>& _rhs) noexcept
	{
		int result = 0;
		MemeString_isEqual(
			__to_object__(_lhs.native_handle()), _rhs.data(), _rhs.size(), &result);
		return result;
	}

	template<class Allocator>
	inline bool operator==(const memepp::string& _lhs, const std::vector<uint8_t, Allocator>& _rhs) noexcept
	{
		int result = 0;
		MemeString_isEqual(
			__to_object__(_lhs.native_handle()), static_cast<const char*>(_rhs.data()), _rhs.size(), &result);
		return result;
	}

	template<class Allocator>
	inline bool operator==(const std::vector<char, Allocator>& _lhs, const memepp::string& _rhs) noexcept
	{
		int result = 0;
		MemeString_isEqual(
			__to_object__(_rhs.native_handle()), _lhs.data(), _lhs.size(), &result);
		return result;
	}

	template<class Allocator>
	inline bool operator==(const std::vector<uint8_t, Allocator>& _lhs, const memepp::string& _rhs) noexcept
	{
		int result = 0;
		MemeString_isEqual(
			__to_object__(_rhs.native_handle()), static_cast<const char*>(_lhs.data()), _lhs.size(), &result);
		return result;
	}

	template<class Allocator>
	inline bool operator!=(const memepp::string& _lhs, const std::vector<char, Allocator>& _rhs) noexcept
	{
		return !(_lhs == _rhs);
	}

	template<class Allocator>
	inline bool operator!=(const memepp::string& _lhs, const std::vector<uint8_t, Allocator>& _rhs) noexcept
	{
		return !(_lhs == _rhs);
	}

	template<class Allocator>
	inline bool operator!=(const std::vector<char, Allocator>& _lhs, const memepp::string& _rhs) noexcept
	{
		return !(_lhs == _rhs);
	}

	template<class Allocator>
	inline bool operator!=(const std::vector<uint8_t, Allocator>& _lhs, const memepp::string& _rhs) noexcept
	{
		return !(_lhs == _rhs);
	}

} // namespace memepp

#endif // !MEMEPP_COMPARE_STD_VECTOR_HPP_INCLUDED
