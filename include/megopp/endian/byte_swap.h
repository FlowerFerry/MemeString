
#ifndef MEGOPP_STOOL_ENDIAN_BSWAP_H_INCLUDED
#define MEGOPP_STOOL_ENDIAN_BSWAP_H_INCLUDED

#include <stdint.h>
#include <limits.h>
#include <string.h>

#include <mego/predef/endian.h>

#include <limits>
#include <type_traits>

#include "megopp/help/type_traits.h"
#include "types.h"

namespace megopp {

namespace endian {

	template<int _ByteSize>
	struct byte_swap_with_size;

	template<>
	struct byte_swap_with_size<2>
	{
		constexpr static int halfByteSize = 1;

		inline constexpr static uint16_t convert(uint16_t _value) noexcept
		{
			return (((_value & 0x00FF) << 8) | ((_value & 0xFF00) >> 8));
		}
	};

	template<>
	struct byte_swap_with_size<-1>
	{
	};
	template<>
	struct byte_swap_with_size<1>
	{
	};

	template<int _ByteSize>
	struct byte_swap_with_size
	{
		constexpr static int halfByteSize = _ByteSize / 2;
		using self_half_t = byte_swap_with_size<halfByteSize>;

		using full_t = typename type_with_size<_ByteSize>::uint;
		using half_t = typename type_with_size<halfByteSize>::uint;

		inline constexpr static full_t convert(full_t _value) noexcept
		{
			return
				(static_cast<full_t>(
					self_half_t::convert(_value & std::numeric_limits<half_t>::max())
					) << (halfByteSize * CHAR_BIT))
				| static_cast<full_t>(self_half_t::convert(_value >> (halfByteSize * CHAR_BIT)));
		}
	};

	template<typename _Ty>
	inline _Ty byte_swap(_Ty _value) noexcept
	{
		static_assert(std::is_arithmetic<_Ty>::value, "[megopp::endian::byte_swap] only supports arithmetic types");

		typename type_with_size<sizeof(_value)>::uint value = 0;
		memcpy(&value, &_value, sizeof(_value));
		value = byte_swap_with_size<sizeof(_value)>::convert(value);
		memcpy(&_value, &value, sizeof(_value));

		return _value;
	}

};
};

#endif // !MEGOPP_STOOL_ENDIAN_BSWAP_H_INCLUDED
