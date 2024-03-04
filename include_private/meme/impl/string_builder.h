
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
    mmstrstk_t str;
} MemeStringBuilder_Part_t, mmsbldr_part_t;

typedef struct _MemeStringBuilder_Format_t
{
    mmstrstk_t fmt;
} MemeStringBuilder_Format_t, mmsbldr_fmt_t;

struct _MemeStringBuilder_t 
{
    cvector_vector_type(mmsbldr_part_t) parts_;
    struct {
        size_t flag_   : (sizeof(size_t) * CHAR_BIT / 2);
        size_t __res__ : (sizeof(size_t) * CHAR_BIT / 2);
    };
    mmsbldr_fmt_t* fmt_;
    mmstr_ptr_t out_;
};

static_assert(sizeof(struct _MemeStringBuilder_t) == MMSBLDR__OBJ_SIZE,
    "MemeStringBuilder_t size is not equal to MMSBUILDER__OBJECT_SIZE");

int 
MemeStringBuilderPart_init(mmsbldr_part_t* _part);

int 
MemeStringBuilderPart_initByOther(mmsbldr_part_t* _part, const mmsbldr_part_t* _other);

void 
MemeStringBuilderPart_unInit(void* _part);

int
MemeStringBuilderParts_checkInit(mmsbldr_part_t** _parts);


int
MemeStringBuilderFormat_init(mmsbldr_fmt_t** _fmt);

int
MemeStringBuilderFormat_initByOther(mmsbldr_fmt_t** _fmt, const mmsbldr_fmt_t* _other);

int
MemeStringBuilderFormat_unInit(mmsbldr_fmt_t** _fmt);

int
MemeStringBuilder_generateWithParts(mmsbldr_cptr_t _builder, mmstr_ptr_t _out);

#endif // MEME_IMPL_STRING_BUILDER_H_INCLUDED
