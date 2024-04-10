
#ifndef MEGOPP_STOOL_ENDIAN_NET_H_INCLUDED
#define MEGOPP_STOOL_ENDIAN_NET_H_INCLUDED

#include <stdint.h>

#include "mego/predef/endian.h"
#include "megopp/endian/byte_swap.h"

namespace megopp {
namespace endian {

	template<typename _Ty>
	inline _Ty hton(_Ty _v) noexcept
	{
#if	MEGO_ENDIAN__BIG_BYTE
		return _v;
#elif MEGO_ENDIAN__LITTLE_BYTE
		return byte_swap(_v);
#else
#	error "[megopp::endian::hton] unknown endianness"
#endif
	}

	template<typename _Ty>
	inline _Ty ntoh(_Ty _v) noexcept
	{
#if	MEGO_ENDIAN__BIG_BYTE
		return _v;
#elif MEGO_ENDIAN__LITTLE_BYTE
		return byte_swap(_v);
#else
#	error "[megopp::endian::hton] unknown endianness"
#endif
	}

	template<typename _Ty>
	inline _Ty htot(_Ty _v, endian_t _target) noexcept
	{
		switch (_target)
		{
		case endian_t::big_byte:
		{
#if	MEGO_ENDIAN__BIG_BYTE
			return _v;
#elif MEGO_ENDIAN__LITTLE_BYTE
			return byte_swap(_v);
#else
#	error "[megopp::endian::htot] unknown endianness"
#endif
		} break;
		case endian_t::little_byte:
		{
#if	MEGO_ENDIAN__BIG_BYTE
			return byte_swap(_v);
#elif MEGO_ENDIAN__LITTLE_BYTE
			return _v;
#else
#	error "[megopp::endian::htot] unknown endianness"
#endif
		} break;
		case endian_t::big_word:
		{
#if MEGO_ENDIAN__LITTLE_BYTE || MEGO_ENDIAN__BIG_BYTE
			if constexpr (sizeof(_Ty) >= 4)
			{
				_v = word_swap(_v);
			}
#if MEGO_ENDIAN__LITTLE_BYTE
			return byte_swap(_v);
#endif
#else
#	error "[megopp::endian::htot] unknown endianness"
#endif
		} break;
		case endian_t::little_word:
		{
#if MEGO_ENDIAN__LITTLE_BYTE || MEGO_ENDIAN__BIG_BYTE
			if constexpr (sizeof(_Ty) >= 4)
			{
				_v = word_swap(_v);
			}
#if MEGO_ENDIAN__BIG_BYTE
			return byte_swap(_v);
#endif
#else
#	error "[megopp::endian::htot] unknown endianness"
#endif
		} break;
		default:
			return _v;
		}
		
		return _v;
	}

};
};

#endif // !MEGOPP_STOOL_ENDIAN_NET_H_INCLUDED
