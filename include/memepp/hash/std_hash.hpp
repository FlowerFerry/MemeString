
#ifndef MEMEPP_HASH_STD_HPP_INCLUDED
#define MEMEPP_HASH_STD_HPP_INCLUDED

#include "memepp/string_def.hpp"
#include "memepp/string_view_def.hpp"

#include <type_traits>

namespace memepp {
namespace hash {
namespace details {

	inline size_t hash_value_with_bytes(const uint8_t* _first, size_t _count) noexcept
	{
#if (INTPTR_MAX == INT64_MAX)

		static_assert(sizeof(size_t) == 8, 
			"In memepp::hash::details::hash_value_with_bytes, this code is for 64-bit size_t");

		size_t val = 14695981039346656037ULL;
		static constexpr size_t fnv_prime = 1099511628211ULL;

#elif (INTPTR_MAX == INT32_MAX)

		static_assert(sizeof(size_t) == 4, 
			"In memepp::hash::details::hash_value_with_bytes, this code is for 32-bit size_t");

		size_t val = 2166136261U;
		static constexpr size_t fnv_prime = 16777619U;
#else
#	error "In memepp::hash::details::hash_value_with_bytes, unsupported size_t types"
#endif

		for (const uint8_t* _last = _first + _count; _first != _last; ++_first)
		{
			val ^= static_cast<size_t>(*_first);
			val *= fnv_prime;
		}

		return (val);
	}

}; // namespace details
}; // namespace hash

}

namespace std {

	template<typename _Ty>
	struct hash;

	template<>
	struct hash<memepp::string>
	{
		inline size_t operator()(const memepp::string& _s) const
		{
			return memepp::hash::details::hash_value_with_bytes(
				reinterpret_cast<const uint8_t*>(_s.data()), _s.size());
		}
	};

	template<>
	struct hash<memepp::string_view>
	{
		inline size_t operator()(const memepp::string_view& _s) const
		{
			return memepp::hash::details::hash_value_with_bytes(
				reinterpret_cast<const uint8_t*>(_s.data()), _s.size());
		}
	};
};

#endif // !MEMEPP_HASH_STD_HPP_INCLUDED
