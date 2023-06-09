
#ifndef MEGOPP_UTIL_TEMPLET_RANDOM_H_INCLUDED
#define MEGOPP_UTIL_TEMPLET_RANDOM_H_INCLUDED

#include <limits.h>

namespace megopp {
namespace util {

	constexpr unsigned int template_seed(const char* str,
		const unsigned int initial_result = 0) {
		return *str == 0 ? initial_result :
			template_seed(str + 1,
			((initial_result << 8ULL) + *str) & UINT_MAX);
	}

	template <typename... RemainingStrings>
	constexpr unsigned int template_seed(const char* first_str,
		const RemainingStrings&... remaining_strs) {
		return template_seed(first_str, template_seed(remaining_strs...));
	}

	template <unsigned int Seed>
	struct template_random {
		static constexpr unsigned int value = (134775813ULL * Seed + 1ULL) & UINT_MAX;
		using next = template_random<value>;
	};

};
};

#endif // !MEGOPP_UTIL_TEMPLET_RANDOM_H_INCLUDED

