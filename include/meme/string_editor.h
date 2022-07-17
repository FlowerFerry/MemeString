
#ifndef MEME_STRING_EDITOR_H_INCLUDED
#define MEME_STRING_EDITOR_H_INCLUDED

#include "string_editor_fwd.h"

MEME_EXTERN_C_SCOPE_START

MEME_API int
	MEME_STDCALL MemeStringEditorStack_init(
		MemeStringEditorStack_t* _editor, size_t _editor_size);

MEME_API int
	MEME_STDCALL MemeStringEditorStack_initAssociated(
		MemeStringEditorStack_t* _editor, size_t _editor_size, 
		MemeString_Const_t _in, MemeStringStack_t* _out, size_t _object_size);

MEME_API int
	MEME_STDCALL MemeStringEditorStack_initWithString(
		MemeStringEditorStack_t* _editor, size_t _editor_size,
		MemeString_Const_t _in, size_t _object_size);

MEME_API int
	MEME_STDCALL MemeStringEditorStack_unInit(
		MemeStringEditorStack_t* _editor, size_t _editor_size);

MEME_API int
	MEME_STDCALL MemeStringEditorStack_swap(
		MemeStringEditorStack_t* _editor, MemeStringEditorStack_t* _other, size_t _editor_size);

MEME_API int
	MEME_STDCALL MemeStringEditorStack_release(
		MemeStringEditorStack_t* _editor, size_t _editor_size, MemeStringStack_t* _out, size_t _object_size);

MEME_API int
	MEME_STDCALL MemeStringEditorStack_isEqual(
		const MemeStringEditorStack_t* _editor, size_t _editor_size, const char* _str, MemeInteger_t _len, int* _result);

MEME_API int
	MEME_STDCALL MemeStringEditorStack_isEqualWithString(
		const MemeStringEditorStack_t* _editor, size_t _editor_size, MemeString_Const_t _rhs, int* _result);



MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_STRING_EDITOR_H_INCLUDED
