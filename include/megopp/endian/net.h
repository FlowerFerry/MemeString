
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

};
};

#endif // !MEGOPP_STOOL_ENDIAN_NET_H_INCLUDED
