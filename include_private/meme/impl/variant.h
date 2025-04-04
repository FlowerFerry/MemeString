
#ifndef MEME_IMPL_VARIANT_H_INCLUDED
#define MEME_IMPL_VARIANT_H_INCLUDED

#include <mego/predef/symbol/static_assert.h>
#include <meme/common.h>
#include <meme/variant_fwd.h>
#include <meme/string_fwd.h>
#include <meme/buffer_fwd.h>
#include <meme/variable_buffer_fwd.h>

#include <stdint.h>
#include <limits.h>
#include <assert.h>

MEME_EXTERN_C_SCOPE_START

#if INTPTR_MAX == INT32_MAX
#pragma pack(push, 4)
#endif
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
#if INTPTR_MAX == INT32_MAX
#pragma pack(pop)
#endif

typedef struct _MemeVariant_t
{
    mmbyte_t reg_size : 7;
    mmbyte_t non_null : 1;
    uint16_t type;
    //struct {
    //    size_t __res__ : (sizeof(mmint_t) * CHAR_BIT - 25);
    //};
    MemeVariantData_t d;
} MemeVariant_t;

MEGO__STATIC_ASSERT(
    sizeof(MemeVariant_t) == MMVAR__OBJ_SIZE, 
    MemeVariantSizeMismatch,
    "MemeVariant_t size mismatch");
MEGO__STATIC_ASSERT(
    sizeof(MemeVariantData_t) == MMSTR__OBJ_SIZE, 
    MemeVariantDataSizeMismatch,
    "MemeVariantData_t size mismatch");

mmint_t MemeVariantImpl_basicNumberToInt (const MemeVariant_t* _var);
size_t  MemeVariantImpl_basicNumberToUInt(const MemeVariant_t* _var);

int64_t  MemeVariantImpl_basicNumberToInt64 (const MemeVariant_t* _var);
uint64_t MemeVariantImpl_basicNumberToUInt64(const MemeVariant_t* _var);

double  MemeVariantImpl_basicNumberToDouble(const MemeVariant_t* _var);

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_IMPL_VARIANT_H_INCLUDED
