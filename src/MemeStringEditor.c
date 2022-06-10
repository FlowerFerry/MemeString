
#include "meme/string_editor.h"

#include <assert.h>
#include <string.h>

MEME_API int MEME_STDCALL MemeStringEditorStack_swap(
	MemeStringEditorStack_t* _editor, MemeStringEditorStack_t* _other, size_t _editor_size)
{
	assert(_editor	&& MemeStringEditorStack_swap);
	assert(_other	&& MemeStringEditorStack_swap);

	MemeStringEditorStack_t temp;
	memcpy(&temp, _editor, MEME_STRING__OBJECT_SIZE);
	memcpy(_editor, _other, MEME_STRING__OBJECT_SIZE);
	memcpy(_other, &temp, MEME_STRING__OBJECT_SIZE);
	return 0;
}
