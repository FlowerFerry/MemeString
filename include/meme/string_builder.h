
#ifndef MEME_STRING_BUILDER_H_INCLUDED
#define MEME_STRING_BUILDER_H_INCLUDED

#include "string_builder_fwd.h"

MEME_EXTERN_C_SCOPE_START

MEME_API int
	MEME_STDCALL MemeStringBuilderStack_init(
		MemeStringBuilderStack_t* _builder, size_t _builder_size);

MEME_API int
	MEME_STDCALL MemeStringBuilderStack_initAssociated(
		MemeStringBuilderStack_t* _builder, size_t _builder_size, 
		MemeString_Const_t _in, MemeStringStack_t* _out, size_t _object_size);

MEME_API int
	MEME_STDCALL MemeStringBuilderStack_initWithString(
		MemeStringBuilderStack_t* _builder, size_t _builder_size,
		MemeString_Const_t _in, size_t _object_size);

MEME_API int
	MEME_STDCALL MemeStringBuilderStack_initByOther(
		mmsbldrstk_t* _builder, size_t _builder_size, mmsbldr_const_t _other);

MEME_API int
	MEME_STDCALL MemeStringBuilderStack_unInit(
		MemeStringBuilderStack_t* _builder, size_t _builder_size);

MEME_API int
	MEME_STDCALL MemeStringBuilderStack_assignByOther(
		mmsbldrstk_t* _builder, size_t _builder_size, mmsbldr_const_t _other);

MEME_API int
	MEME_STDCALL MemeStringBuilderStack_swap(
		MemeStringBuilderStack_t* _builder, 
		MemeStringBuilderStack_t* _other, size_t _builder_size);

MEME_API int
	MEME_STDCALL MemeStringBuilderStack_release(
		MemeStringBuilderStack_t* _builder, size_t _builder_size, 
		MemeStringStack_t* _out, size_t _object_size);

MEME_API int
	MEME_STDCALL MemeStringBuilderStack_isEqual(
		const MemeStringBuilderStack_t* _builder, size_t _builder_size, 
		const char* _str, MemeInteger_t _len, int* _result);

MEME_API int
	MEME_STDCALL MemeStringBuilderStack_isEqualWithString(
		const MemeStringBuilderStack_t* _builder, size_t _builder_size, 
		MemeString_Const_t _rhs, int* _result);

MEME_API int
MEME_STDCALL MemeStringBuilder_generate(mmsbldr_const_t _builder, mms_t _out);

MEME_API int
MEME_STDCALL MemeStringBuilder_appendArgWithString(mmsbldr_t _builder, mms_const_t _arg);

MEME_API int
MEME_STDCALL MemeStringBuilder_appendArgByOther(mmsbldr_ptr_t _builder, mmsbldr_cptr_t _other);

MEME_API int
MEME_STDCALL MemeStringBuilder_prependArgWithString(mmsbldr_t _builder, mms_const_t _arg);

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_STRING_BUILDER_H_INCLUDED
