
#ifndef MEGOPP_ENDIAN_TYPES_H_INCLUDED
#define MEGOPP_ENDIAN_TYPES_H_INCLUDED

#include <mego/predef/endian.h>
#include <megopp/predef/namespace_alias.h>


namespace mgpp {

	enum class endian_t
	{
		little_byte = 0,
		big_byte = 1,
		little_word = 2,
		big_word = 3,
#if   MEGO_ENDIAN__BIG_BYTE
		native = big_byte
#elif MEGO_ENDIAN__LITTLE_BYTE
		native = little_byte
#elif MEGO_ENDIAN__LITTLE_WORD
		native = little_word
#elif MEGO_ENDIAN__BIG_WORD
		native = big_word
#endif 
	};

}

#endif // !MEGOPP_ENDIAN_TYPES_H_INCLUDED
