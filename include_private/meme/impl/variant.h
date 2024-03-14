
#ifndef MEME_IMPL_VARIANT_H_INCLUDED
#define MEME_IMPL_VARIANT_H_INCLUDED

#include <meme/common.h>
#include <meme/variant_fwd.h>

#include <stdint.h>
#include <limits.h>

MEME_EXTERN_C_SCOPE_START

typedef union _MemeVariantData_t 
{
    mmbyte_t    b;
    char        c;
    wchar_t     wc;
    int64_t     i64;
    uint64_t    u64;
    double      d;
    mmstrstk_t  str;
    mmbufstk_t  buf;
    mmvbstk_t   vb;
    mmrune_t    r;
    void*       p;
} MemeVariantData_t;

typedef struct _MemeVariant_t
{
    MemeVariantData_t d;
    struct {
        mmint_t type: 16;
        mmint_t non_null: 1;
        mmint_t __res__: (sizeof(mmint_t) * CHAR_BIT - 17);
    };
} MemeVariant_t;

static_assert(sizeof(MemeVariant_t) == MMVAR__OBJ_SIZE, "MemeVariant_t size mismatch");

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_IMPL_VARIANT_H_INCLUDED
