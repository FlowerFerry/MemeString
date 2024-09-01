
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
#include <megopp/predef/namespace_alias.h>

namespace mgpp {

namespace endian {

	template<int _ByteSize>
	struct byte_swap_with_size;

	template<>
	struct byte_swap_with_size<2>
	{
		constexpr static int half_byte_size = 1;
		using uint_t = uint16_t;

		inline constexpr static uint_t convert(uint_t _value) noexcept
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
		inline constexpr static uint8_t convert(uint8_t _value) noexcept
		{
			return _value;
		}
	};

	template<int _ByteSize>
	struct byte_swap_with_size
	{
		constexpr static int half_byte_size = _ByteSize / 2;
		using uint_t = typename type_with_size<_ByteSize>::uint;
		using half_t = byte_swap_with_size<half_byte_size>;

		inline constexpr static uint_t convert(uint_t _value) noexcept
		{
			return
				(static_cast<uint_t>(
					half_t::convert(_value & (::std::numeric_limits<typename half_t::uint_t>::max)())
					) << (half_byte_size * CHAR_BIT))
				| static_cast<uint_t>(half_t::convert(_value >> (half_byte_size * CHAR_BIT)));
		}
	};

	template<int _ByteSize>
	using byte_swap_by_size = byte_swap_with_size<_ByteSize>;

	template<typename _Ty>
	inline _Ty byte_swap(_Ty _value) noexcept
	{
		static_assert(std::is_trivially_copyable<_Ty>::value, "[megopp::endian::byte_swap] only supports arithmetic types");

		typename type_with_size<sizeof(_value)>::uint value = 0;
		memcpy(&value, &_value, sizeof(_value));
		value = byte_swap_with_size<sizeof(_value)>::convert(value);
		memcpy(&_value, &value, sizeof(_value));

		return _value;
	}

	template<size_t _ByteSize>
	struct word_swap_by_size;

	template<>
	struct word_swap_by_size<1>
	{
	};

	template<>
	struct word_swap_by_size<4>
	{
		constexpr static int half_byte_size = 2;
		using uint_t = uint32_t;

		inline static uint_t convert(uint_t _value) noexcept
		{
			return (((_value & 0x0000FFFF) << 16) | ((_value & 0xFFFF0000) >> 16));
		}
	};

	template<size_t _ByteSize>
	struct word_swap_by_size
	{
		constexpr static int half_byte_size = _ByteSize / 2;
		using uint_t = typename type_by_size<_ByteSize>::uint;
		using half_t = word_swap_by_size<half_byte_size>;

		inline static uint_t convert(uint_t _value) noexcept
		{
			return
				(static_cast<uint_t>(
					half_t::convert(_value & (::std::numeric_limits<typename half_t::uint_t>::max)())
					) << (half_byte_size * CHAR_BIT))
				| static_cast<uint_t>(half_t::convert(_value >> (half_byte_size * CHAR_BIT)));
		}
	};

	template<typename _Ty>
	inline _Ty word_swap(_Ty _value) noexcept
	{
		static_assert(std::is_trivially_copyable<_Ty>::value, "[megopp::endian::word_swap] only supports arithmetic types");

		typename type_by_size<sizeof(_value)>::uint value = 0;
		memcpy(&value, &_value, sizeof(_value));
		value = word_swap_by_size<sizeof(_value)>::convert(value);
		memcpy(&_value, &value, sizeof(_value));

		return _value;
	}

};
};

#endif // !MEGOPP_STOOL_ENDIAN_BSWAP_H_INCLUDED
