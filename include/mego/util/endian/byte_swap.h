
#ifndef MEGO_UTIL_ENDIAN_BSWAP_H_INCLUDED
#define MEGO_UTIL_ENDIAN_BSWAP_H_INCLUDED

#include <stdint.h>

inline uint16_t MegoEndian_byteSwap16(uint16_t _v)
{
	return (((_v & 0x00FF) << 8) | ((_v & 0xFF00) >> 8));
}

inline uint32_t MegoEndian_byteSwap32(uint32_t _v)
{
	return (((uint32_t)MegoEndian_byteSwap16(_v & 0x0000FFFF) << 16) | MegoEndian_byteSwap16((_v & 0xFFFF0000) >> 16));
}

inline uint64_t MegoEndian_byteSwap64(uint64_t _v)
{
	return (((uint64_t)MegoEndian_byteSwap32(_v & 0x00000000FFFFFFFF) << 32) | MegoEndian_byteSwap32((_v & 0xFFFFFFFF00000000) >> 32));
}


inline uint32_t MegoEndian_wordSwap32(uint32_t _v)
{
	return (((_v & 0x0000FFFF) << 16) | ((_v & 0xFFFF0000) >> 16));
}

inline uint64_t MegoEndian_wordSwap64(uint64_t _v)
{
	return (((uint64_t)MegoEndian_wordSwap32(_v & 0x00000000FFFFFFFF) << 32) | MegoEndian_wordSwap32((_v & 0xFFFFFFFF00000000) >> 32));
}


#endif // !MEGO_UTIL_ENDIAN_BSWAP_H_INCLUDED
