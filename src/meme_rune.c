
#include "meme/string_fwd.h"
#include <string.h>

MEME_EXTERN_C_SCOPE_START

int MemeRune_compare(const MemeRune_t* _lhs, const MemeRune_t* _rhs)
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

int MemeRune_withinCjkUnifiedIdeographs(const MemeRune_t* _ch)
{
	// CJK Unified Ideographs U+4E00 - U+9FFF
	MemeRune_t begin = { .byte = { 0xE0, 0xB8, 0x80, 0x00 } };
	MemeRune_t end   = { .byte = { 0xE9, 0xBF, 0xBF, 0x00 } };
	return 0 <= MemeRune_compare(_ch, &begin) && MemeRune_compare(_ch, &end) <= 0;
}

int MemeRune_withinCjkUnifiedIdeographsExtensionA(const MemeRune_t* _ch)
{
	// CJK Unified Ideographs Extension A U+3400 - U+4DBF
	MemeRune_t begin = { .byte = { 0xE3, 0x90, 0x80, 0x00 } };
	MemeRune_t end   = { .byte = { 0xE4, 0xB6, 0xBF, 0x00 } };
	return 0 <= MemeRune_compare(_ch, &begin) && MemeRune_compare(_ch, &end) <= 0;
}

int MemeRune_withinCjkUnifiedIdeographsExtensionB(const MemeRune_t* _ch)
{
	return 0;
}

int MemeRune_withinCjkUnifiedIdeographsExtensionC(const MemeRune_t* _ch)
{
	return 0;
}

int MemeRune_withinCjkUnifiedIdeographsExtensionD(const MemeRune_t* _ch)
{
	return 0;
}

int MemeRune_withinCjkUnifiedIdeographsExtensionE(const MemeRune_t* _ch)
{
	return 0;
}

int MemeRune_withinCjkUnifiedIdeographsExtensionF(const MemeRune_t* _ch)
{
	return 0;
}

int MemeRune_withinCjkUnifiedIdeographsExtensionG(const MemeRune_t* _ch)
{
	return 0;
}

int MemeRune_withinCjkCompatibilityIdeographs(const MemeRune_t* _ch)
{
	return 0;
}

int MemeRune_isChineseCharacter(const MemeRune_t* _ch)
{
	return MemeRune_withinCjkUnifiedIdeographs(_ch)
		|| MemeRune_withinCjkUnifiedIdeographsExtensionA(_ch)
		|| MemeRune_withinCjkUnifiedIdeographsExtensionB(_ch)
		|| MemeRune_withinCjkUnifiedIdeographsExtensionC(_ch)
		|| MemeRune_withinCjkUnifiedIdeographsExtensionD(_ch)
		|| MemeRune_withinCjkUnifiedIdeographsExtensionE(_ch)
		|| MemeRune_withinCjkUnifiedIdeographsExtensionF(_ch)
		|| MemeRune_withinCjkUnifiedIdeographsExtensionG(_ch)
		|| MemeRune_withinCjkCompatibilityIdeographs(_ch);
}

MEME_EXTERN_C_SCOPE_ENDED