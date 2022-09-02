
#include "meme/string_builder.h"

#include <assert.h>
#include <string.h>

MEME_API int MEME_STDCALL MemeStringBuilderStack_swap(
	MemeStringBuilderStack_t* _editor, MemeStringBuilderStack_t* _other, size_t _editor_size)
{
	assert(_editor	&& MemeStringBuilderStack_swap);
	assert(_other	&& MemeStringBuilderStack_swap);

	MemeStringBuilderStack_t temp;
	memcpy(&temp, _editor, MEME_STRING__OBJECT_SIZE);
	memcpy(_editor, _other, MEME_STRING__OBJECT_SIZE);
	memcpy(_other, &temp, MEME_STRING__OBJECT_SIZE);
	return 0;
}
