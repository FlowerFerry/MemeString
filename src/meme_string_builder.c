
#include "meme/string_builder.h"

#include <meme/string.h>
#include <meme/variable_buffer.h>
#include <meme/impl/string_builder.h>
#include <meme/impl/string_memory.h>

#include <mego/err/ec.h>

#include <assert.h>
#include <string.h>
#include <errno.h>

int MemeStringBuilderPart_init(mmsbuilder_part_t* _part)
{
    int result = 0;
    result = mmsstk_init(&(_part->str), MMS__OBJECT_SIZE);
    return result;
}

void MemeStringBuilderPart_unInit(void* _part)
{
    mmsbuilder_part_t* part = (mmsbuilder_part_t*)_part;
    mmsstk_uninit(&part->str, MMS__OBJECT_SIZE);
}

int MemeStringBuilderParts_checkInit(mmsbuilder_part_t** _part)
{
    int result = 0;
    if (*_part == NULL) {
        cvector_reserve(*_part, 2);
        cvector_set_elem_destructor(*_part, MemeStringBuilderPart_unInit);
    }
    if (*_part == NULL) {
        result = (MGEC__NOMEM);
    }
    return result;
}

int 
MemeStringBuilderFormat_destroy(mmsbuilder_format_t** _fmt)
{
    int result = 0;
    
    assert(_fmt != NULL && MemeStringBuilderFormat_destroy);

    if (*_fmt == NULL)
        return 0;

    result = MemeStringStack_unInit(&((*_fmt)->fmt), MMS__OBJECT_SIZE);
    mmsmem_free(*_fmt);
    *_fmt = NULL;
    return result;
}

int
MemeStringBuilder_generateWithParts(mmsbldr_const_t _builder, mms_t _out)
{
    size_t totalSize = 0;
    int result = 0;
    mmvbstk_t vb;
    mmvb_t vbp = (mmvb_t)&vb;
    
    for (size_t index = 0; index < cvector_size(_builder->parts_); ++index)
    {
        mms_t sp = (mms_t)&(_builder->parts_[index].str);
        totalSize += MemeString_byteSize(sp);
    }
    
    totalSize = (totalSize % sizeof(size_t) == 0) ? totalSize : 
        (totalSize + sizeof(size_t) - (totalSize % sizeof(size_t))); 
    
    if (totalSize == 0) {
        return 0;
    }

    MemeVariableBufferStack_init(&vb, sizeof(vb));
    result = (int)MemeVariableBuffer_reserve(vbp, totalSize);
    if (result != 0)
        return result;

    for (size_t index = 0; index < cvector_size(_builder->parts_); ++index)
    {
        mms_t sp = (mms_t) & (_builder->parts_[index].str);
        result = (int)MemeVariableBuffer_appendWithBytes(vbp, MemeString_byteData(sp), MemeString_byteSize(sp));
        if (result != 0) {
            MemeVariableBufferStack_unInit(&vb, sizeof(vb));
            return result;
        }
    }
    result = (int)MemeVariableBuffer_releaseToString(
        vbp, (MemeStringStack_t*)_out, MMS__OBJECT_SIZE);
    if (result) {
        MemeVariableBufferStack_unInit(&vb, sizeof(vb));
    }
    return result;
}

MEME_API int MEME_STDCALL 
MemeStringBuilderStack_init(MemeStringBuilderStack_t* _builder, size_t _builder_size)
{
    mmsbldr_t builder = (mmsbldr_t)_builder;

    assert(_builder != NULL && MemeStringBuilderStack_init);
    assert(_builder_size <= sizeof(MemeStringBuilderStack_t) && MemeStringBuilderStack_init);

    memset(builder, 0 , _builder_size);
    return MemeStringBuilderParts_checkInit(&builder->parts_);

}

MEME_API int MEME_STDCALL 
MemeStringBuilderStack_initByOther(
    mmsbldrstk_t* _builder, size_t _builder_size, mmsbldr_const_t _other)
{
    int result = 0;
    mmsbldr_t builder = (mmsbldr_t)_builder;

    assert(_builder != NULL && MemeStringBuilderStack_initByOther);
    assert(_builder_size <= sizeof(MemeStringBuilderStack_t) && MemeStringBuilderStack_initByOther);
    assert(_other != NULL && MemeStringBuilderStack_initByOther);
    
    result = MemeStringBuilderStack_init(_builder, _builder_size);
    if (result != 0)
        return result;
    
    builder->flag_ = _other->flag_;

    if (_other->out_) {
        result = MemeString_create(&builder->out_);
        if (result != 0)
            return result;
        result = MemeStringStack_assign(
            (mmsstk_t*)builder->out_, MMS__OBJECT_SIZE, _other->out_);
        if (result != 0)
            return result;
    }

    if (_other->fmt_) {
        // TO_DO
    }

    for (size_t index = 0; index < cvector_size(_other->parts_); ++index)
    {
        mmsbuilder_part_t part;
        result = MemeStringBuilderPart_init(&part);
        if (result != 0)
            return result;
        result = MemeStringStack_initByOther(
            &part.str, MMS__OBJECT_SIZE, (mms_const_t)&(_other->parts_[index].str));
        if (result != 0) {
            MemeStringBuilderPart_unInit(&part);
            return result;
        }
        
        cvector_push_back(builder->parts_, part);
    }

    return result;
}

MEME_API int MEME_STDCALL 
MemeStringBuilderStack_unInit(MemeStringBuilderStack_t* _builder, size_t _builder_size)
{
    mmsbldr_t builder = (mmsbldr_t)_builder;
    int result = 0;

    assert(_builder != NULL && MemeStringBuilderStack_unInit);
    assert(_builder_size >= sizeof(MemeStringBuilderStack_t) && MemeStringBuilderStack_unInit);

    if (builder->out_ != NULL) {
        MemeStringBuilder_generate(builder, builder->out_);
        MemeString_destroy(&builder->out_);
    }
    
    if (builder->parts_ != NULL) {
        cvector_free(builder->parts_);
        builder->parts_ = NULL;
    }
    
    MemeStringBuilderFormat_destroy(&(builder->fmt_));
    return result;
}

MEME_API int MEME_STDCALL 
MemeStringBuilderStack_assignByOther(
    mmsbldrstk_t* _builder, size_t _builder_size, mmsbldr_const_t _other)
{
    int result = 0;

    assert(_builder != NULL && MemeStringBuilderStack_assignByOther);
    assert(_builder_size <= sizeof(MemeStringBuilderStack_t) && MemeStringBuilderStack_assignByOther);
    assert(_other != NULL && MemeStringBuilderStack_assignByOther);

    result = MemeStringBuilderStack_unInit(_builder, _builder_size);
    if (result != 0)
        return result;
    result = MemeStringBuilderStack_initByOther(_builder, _builder_size, _other);
    return result;
}

MEME_API int MEME_STDCALL MemeStringBuilderStack_swap(
	MemeStringBuilderStack_t* _editor, MemeStringBuilderStack_t* _other, size_t _editor_size)
{
	assert(_editor	&& MemeStringBuilderStack_swap);
	assert(_other	&& MemeStringBuilderStack_swap);

	MemeStringBuilderStack_t temp;
	memcpy(&temp, _editor,  MMSBUILDER__OBJECT_SIZE);
	memcpy(_editor, _other, MMSBUILDER__OBJECT_SIZE);
	memcpy(_other, &temp,   MMSBUILDER__OBJECT_SIZE);
	return 0;
}

MEME_API int MEME_STDCALL 
MemeStringBuilder_generate(mmsbldr_const_t _builder, mms_t _out)
{
    // TO_DO

    MemeStringStack_reset((MemeStringStack_t*)_out, MMS__OBJECT_SIZE);
    if (_builder->fmt_ == NULL) {
        return MemeStringBuilder_generateWithParts(_builder, _out);
    }
    else {
        return 0;
    }
}

MEME_API int MEME_STDCALL 
MemeStringBuilder_appendArgWithString(mmsbldr_t _builder, mms_const_t _arg)
{
    int result = 0;
    mmsbuilder_part_t part;
    result = MemeStringBuilderPart_init(&part);
    if (result != 0)
        return result;
    result = MemeStringStack_assign(&part.str, MMS__OBJECT_SIZE, _arg);
    if (result != 0) {
        MemeStringBuilderPart_unInit(&part);
        return result;
    }
    cvector_push_back(_builder->parts_, part);
    return result;
}

//MEME_API int MEME_STDCALL 
//MemeStringBuilder_appendArgByOther(mmsbuilder_t _builder, mmsbldr_const_t _other)
//{
//}

MEME_API int MEME_STDCALL 
MemeStringBuilder_prependArgWithString(mmsbldr_t _builder, mms_const_t _arg)
{
    int result = 0;
    mmsbuilder_part_t part;
    result = MemeStringBuilderPart_init(&part);
    if (result != 0)
        return result;
    result = MemeStringStack_assign(&part.str, MMS__OBJECT_SIZE, _arg);
    if (result != 0) {
        MemeStringBuilderPart_unInit(&part);
        return result;
    }
    cvector_insert(_builder->parts_, 0, part);
    return result;
}

