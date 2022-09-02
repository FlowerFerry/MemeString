
#include "meme/string_fwd.h"
#include <string.h>

MEME_EXTERN_C_SCOPE_START

int MemeWord_compare(const MemeWord_t* _lhs, const MemeWord_t* _rhs)
{
	uint8_t index = 0;
	while (_lhs->byte[index] && _rhs->byte[index]) 
	{
		if(_lhs->byte[index] != _rhs->byte[index]) 
		{
			break;
		}
		++index;
	}
	return (int)_lhs->byte[index] - (int)_rhs->byte[index];
}

int MemeWord_withinCjkUnifiedIdeographs(const MemeWord_t* _ch)
{
	// CJK Unified Ideographs U+4E00 - U+9FFF
	MemeWord_t begin = { .byte = { 0xE0, 0xB8, 0x80, 0x00 } };
	MemeWord_t end   = { .byte = { 0xE9, 0xBF, 0xBF, 0x00 } };
	return 0 <= MemeWord_compare(_ch, &begin) && MemeWord_compare(_ch, &end) <= 0;
}

int MemeWord_withinCjkUnifiedIdeographsExtensionA(const MemeWord_t* _ch)
{
	// CJK Unified Ideographs Extension A U+3400 - U+4DBF
	MemeWord_t begin = { .byte = { 0xE3, 0x90, 0x80, 0x00 } };
	MemeWord_t end   = { .byte = { 0xE4, 0xB6, 0xBF, 0x00 } };
	return 0 <= MemeWord_compare(_ch, &begin) && MemeWord_compare(_ch, &end) <= 0;
}

int MemeWord_withinCjkUnifiedIdeographsExtensionB(const MemeWord_t* _ch)
{
	return 0;
}

int MemeWord_withinCjkUnifiedIdeographsExtensionC(const MemeWord_t* _ch)
{
	return 0;
}

int MemeWord_withinCjkUnifiedIdeographsExtensionD(const MemeWord_t* _ch)
{
	return 0;
}

int MemeWord_withinCjkUnifiedIdeographsExtensionE(const MemeWord_t* _ch)
{
	return 0;
}

int MemeWord_withinCjkUnifiedIdeographsExtensionF(const MemeWord_t* _ch)
{
	return 0;
}

int MemeWord_withinCjkUnifiedIdeographsExtensionG(const MemeWord_t* _ch)
{
	return 0;
}

int MemeWord_withinCjkCompatibilityIdeographs(const MemeWord_t* _ch)
{
	return 0;
}

int MemeWord_isChineseCharacter(const MemeWord_t* _ch)
{
	return MemeWord_withinCjkUnifiedIdeographs(_ch)
		|| MemeWord_withinCjkUnifiedIdeographsExtensionA(_ch)
		|| MemeWord_withinCjkUnifiedIdeographsExtensionB(_ch)
		|| MemeWord_withinCjkUnifiedIdeographsExtensionC(_ch)
		|| MemeWord_withinCjkUnifiedIdeographsExtensionD(_ch)
		|| MemeWord_withinCjkUnifiedIdeographsExtensionE(_ch)
		|| MemeWord_withinCjkUnifiedIdeographsExtensionF(_ch)
		|| MemeWord_withinCjkUnifiedIdeographsExtensionG(_ch)
		|| MemeWord_withinCjkCompatibilityIdeographs(_ch);
}

MEME_EXTERN_C_SCOPE_ENDED