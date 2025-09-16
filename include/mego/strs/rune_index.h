
#ifndef MEGO_STRS_RUNE_INDEX_H_INCLUDED
#define MEGO_STRS_RUNE_INDEX_H_INCLUDED

#include <stdint.h>

struct mgs_rune_index_t
{
    const uint8_t * data;
    int8_t size;
    uint8_t __res__[sizeof(size_t) - sizeof(uint8_t)];
};

#endif // !MEGO_STRS_RUNE_INDEX_H_INCLUDED
