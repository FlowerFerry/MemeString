
#ifndef MEGOPP_AUXILIARY_NULL_TYPETRAITS_HPP_INCLUDED
#define MEGOPP_AUXILIARY_NULL_TYPETRAITS_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <stdint.h>

namespace megopp {

	template<size_t _bits>
	struct type_with_size {};

	template<>
	struct type_with_size<1>
	{
		typedef int8_t  sint;
		typedef uint8_t uint;
	};

	template<>
	struct type_with_size<2>
	{
		typedef int16_t  sint;
		typedef uint16_t uint;
	};

	template<>
	struct type_with_size<4>
	{
		typedef int32_t  sint;
		typedef uint32_t uint;
		typedef float    floating;
	};

	template<>
	struct type_with_size<8>
	{
		typedef int64_t  sint;
		typedef uint64_t uint;
		typedef double   floating;
	};

} // namespace megopp

#endif // MEGOPP_AUXILIARY_NULL_TYPETRAITS_HPP_INCLUDED
