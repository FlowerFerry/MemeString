
#ifndef MEGOPP_UTIL_TEMPLET_RANDOM_H_INCLUDED
#define MEGOPP_UTIL_TEMPLET_RANDOM_H_INCLUDED

#include <limits.h>

namespace megopp {
namespace util {

	constexpr unsigned int templet_seed(const char* str,
		const unsigned int initial_result = 0) {
		return *str == 0 ? initial_result :
			templet_seed(str + 1,
			((initial_result << 8ULL) + *str) & UINT_MAX);
	}

	template <typename... RemainingStrings>
	constexpr unsigned int templet_seed(const char* first_str,
		const RemainingStrings&... remaining_strs) {
		return templet_seed(first_str, templet_seed(remaining_strs...));
	}

	template <unsigned int Seed>
	struct templet_random {
		static constexpr unsigned int value = (134775813ULL * Seed + 1ULL) & UINT_MAX;
		using next = templet_random<value>;
	};

};
};

#endif // !MEGOPP_UTIL_TEMPLET_RANDOM_H_INCLUDED

