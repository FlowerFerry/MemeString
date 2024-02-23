
#ifndef MEME_VARIANT_FWD_H_INCLUDED
#define MEME_VARIANT_FWD_H_INCLUDED

#include "common.h"
#include <stdint.h>

MEME_EXTERN_C_SCOPE_START

#define MEME_VARIANT__OBJECT_SIZE (sizeof(void*) * 4)
#define MMVAR__OBJ_SIZE (MEME_VARIANT__OBJECT_SIZE)

typedef int16_t mmmeta_typid_t;
typedef int16_t MemeMetaTypeId_t;

#define MMMETA_TYPID__INVALID     -1
#define MMMETA_TYPID__NULL         0
#define MMMETA_TYPID__BYTE         1
#define MMMETA_TYPID__CHAR         2
#define MMMETA_TYPID__WCHAR        3
#define MMMETA_TYPID__INT64        4
#define MMMETA_TYPID__UINT64       5
#define MMMETA_TYPID__DOUBLE       6
#define MMMETA_TYPID__STRING       7
#define MMMETA_TYPID__BUFFER       8
#define MMMETA_TYPID__VARBUF       9
#define MMMETA_TYPID__RUNE        10
#define MMMETA_TYPID__VOIDPTR     11
#define MMMETA_TYPID__USER      1000

typedef struct _MemeVariantStack_t
{
    uint8_t data[MMVAR__OBJ_SIZE];
} MemeVariantStack_t;
typedef MemeVariantStack_t mmvarstk_t;

typedef struct _MemeVariant_t* MemeVariantPtr_t;
typedef const struct _MemeVariant_t* MemeVariantConstPtr_t;
typedef MemeVariantPtr_t mmvar_ptr_t;
typedef MemeVariantConstPtr_t mmvar_cptr_t;

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_VARIANT_FWD_H_INCLUDED
