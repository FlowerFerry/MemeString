
#ifndef MEME_VARTS_FWD_H_INCLUDED
#define MEME_VARTS_FWD_H_INCLUDED

#include <mego/util/std/time.h>
#include "meme/common.h"
#include <meme/variant_fwd.h>
#include <limits.h>

MEME_EXTERN_C_SCOPE_START

#ifndef MMVTS__TS_MAX
#define MMVTS__TS_MAX     (0x7FFFFFFFFFFFFFULL)
#define MMVTS__TS_INVALID (0xFFFFFFFFFFFFFFULL)
#endif // !MMVTS__TS_MAX

typedef struct MemeVariantTimestampStack_t
{
    uint64_t ts       : (CHAR_BIT * 7); // unix timestamp
    uint64_t userdata : (CHAR_BIT * 1);
    mmvarstk_t var;
} MemeVariantTimestampStack_t;
typedef struct MemeVariantTimestampStack_t mmvtsstk_t;

typedef struct MemeVariantTimestamp_t* mmvts_ptr_t;
typedef const struct MemeVariantTimestamp_t* mmvts_cptr_t;

static_assert(sizeof(MemeVariantTimestampStack_t) == 8 + MMVAR__OBJ_SIZE, "MemeVariantTimestampStack_t size mismatch");

#ifndef MMVTS__OBJ_SIZE
#define MMVTS__OBJ_SIZE (8 + MMVAR__OBJ_SIZE)
#endif // !MMVTS__OBJ_SIZE

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_VARTS_FWD_H_INCLUDED
