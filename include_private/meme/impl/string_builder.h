
#ifndef MEME_IMPL_STRING_BUILDER_H_INCLUDED
#define MEME_IMPL_STRING_BUILDER_H_INCLUDED

#include <meme/string_fwd.h>
#include <meme/string_memory.h>

#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include <cvector.h>
#undef  cvector_clib_malloc
#define cvector_clib_malloc  mmsmem_malloc
#undef  cvector_clib_calloc
#define cvector_clib_calloc  mmsmem_calloc
#undef  cvector_clib_realloc
#define cvector_clib_realloc mmsmem_realloc
#undef  cvector_clib_free
#define cvector_clib_free    mmsmem_free

//typedef MemeByte_t mmsbuilder_merge_type_t;
//enum {
//    mmsbuilder_merge_type__merge,
//};

typedef struct _MemeStringBuilder_Part_t
{
    //mmsbuilder_merge_type_t merge_type;
    mmsstk_t str;
} MemeStringBuilder_Part_t, mmsbuilder_part_t;

typedef struct _MemeStringBuilder_Format_t
{
    mmsstk_t fmt;
} MemeStringBuilder_Format_t, mmsbuilder_format_t;

struct _MemeStringBuilder_t 
{
    cvector_vector_type(mmsbuilder_part_t) parts_;
    struct {
        size_t flag_   : (sizeof(size_t) * CHAR_BIT / 2);
        size_t __res__ : (sizeof(size_t) * CHAR_BIT / 2);
    };
    mmsbuilder_format_t* fmt_;
    mms_t out_;
};

static_assert(sizeof(struct _MemeStringBuilder_t) == MMSBUILDER__OBJECT_SIZE, 
    "MemeStringBuilder_t size is not equal to MMSBUILDER__OBJECT_SIZE");

int 
MemeStringBuilderPart_init(mmsbuilder_part_t* _part);
void 
MemeStringBuilderPart_unInit(void* _part);
int
MemeStringBuilderParts_checkInit(mmsbuilder_part_t** _parts);


int
MemeStringBuilderFormat_destroy(mmsbuilder_format_t** _fmt);

int
MemeStringBuilder_generateWithParts(mmsbldr_const_t _builder, mms_t _out);

#endif // MEME_IMPL_STRING_BUILDER_H_INCLUDED
