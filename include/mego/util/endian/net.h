
#ifndef MEGO_UTIL_ENDIAN_NET_H_INCLUDED
#define MEGO_UTIL_ENDIAN_NET_H_INCLUDED

#include <stdint.h>

#include "mego/predef/endian.h"
#include "byte_swap.h"

MG_CAPI_INLINE uint16_t MegoEndian_hton16(uint16_t _v)
{
#if	MEGO_ENDIAN__BIG_BYTE
	return _v;
#elif MEGO_ENDIAN__LITTLE_BYTE
	return MegoEndian_byteSwap16(_v);
#else
#	error "[MegoEndian_hton16] unknown endianness"
#endif
}

MG_CAPI_INLINE uint32_t MegoEndian_hton32(uint32_t _v)
{
#if	MEGO_ENDIAN__BIG_BYTE
	return _v;
#elif MEGO_ENDIAN__LITTLE_BYTE
	return MegoEndian_byteSwap32(_v);
#else
#	error "[MegoEndian_hton32] unknown endianness"
#endif
}

MG_CAPI_INLINE uint64_t MegoEndian_hton64(uint64_t _v)
{
#if	MEGO_ENDIAN__BIG_BYTE
	return _v;
#elif MEGO_ENDIAN__LITTLE_BYTE
	return MegoEndian_byteSwap64(_v);
#else
#	error "[MegoEndian_hton64] unknown endianness"
#endif
}

MG_CAPI_INLINE uint16_t MegoEndian_ntoh16(uint16_t _v)
{
#if	MEGO_ENDIAN__BIG_BYTE
	return _v;
#elif MEGO_ENDIAN__LITTLE_BYTE
	return MegoEndian_byteSwap16(_v);
#else
#	error "[MegoEndian_ntoh16] unknown endianness"
#endif
}

MG_CAPI_INLINE uint32_t MegoEndian_ntoh32(uint32_t _v)
{
#if	MEGO_ENDIAN__BIG_BYTE
	return _v;
#elif MEGO_ENDIAN__LITTLE_BYTE
	return MegoEndian_byteSwap32(_v);
#else
#	error "[MegoEndian_ntoh32] unknown endianness"
#endif
}

MG_CAPI_INLINE uint64_t MegoEndian_ntoh64(uint64_t _v)
{
#if	MEGO_ENDIAN__BIG_BYTE
	return _v;
#elif MEGO_ENDIAN__LITTLE_BYTE
	return MegoEndian_byteSwap64(_v);
#else
#	error "[MegoEndian_ntoh64] unknown endianness"
#endif
}


#endif // !MEGO_UTIL_ENDIAN_NET_H_INCLUDED
