
#ifndef MEGO_STRS_RUNE_H_INCLUDED
#define MEGO_STRS_RUNE_H_INCLUDED

#include <stdint.h>
#include <limits.h>

#define MGS_RUNE__MAX_CHAR_SIZE 7

struct mgs_rune_t
{
	uint8_t byte[7];
	struct {
		uint8_t capacity : (3);
		uint8_t invalid  : (CHAR_BIT - 3);
	} attr;
};

#endif // !MEGO_STRS_RUNE_H_INCLUDED
