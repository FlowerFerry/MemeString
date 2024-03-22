
#include "meme/string_fwd.h"
#include <meme/utf/u8rune_fwd.h>
#include <meme/rune.h>
#include <meme/impl/algorithm.h>
#include <mego/predef/symbol/likely.h>
#include <string.h>

MEME_EXTERN_C_SCOPE_START

MEME_API int MEME_STDCALL
MemeRuneIndex_isSpace(const mmbyte_t* _buf, int _size)
{
    int size = 0;
    assert(_buf != NULL);

    if (MEGO_SYMBOL__UNLIKELY(_size < 1))
        return 0;
    
    size = mmutf_u8rune_char_size(_buf[0]);
    if (MEGO_SYMBOL__UNLIKELY(size < 0))
        return 0;
    size = MemeMath_Min(size, _size);

    switch (size) {
    case 1:
        return isspace(_buf[0]) ? 1 : 0;
    case 2: {
        if (_buf[0] == 0xC2)
        {
            switch (_buf[1]) {
            case 0x85: // 下一行 (NEL)
            case 0xA0: // 不间断空格
                return 1;
            default:
                return 0;
            }
        }
        else {
            return 0;
        }
    }
    case 3: {
        if (_buf[0] == 0xE3 && _buf[1] == 0x80 && _buf[2] == 0x80)
        {
            // 中文全角空格
            return 1;
        }
        else if (_buf[0] == 0xE2 && _buf[1] == 0x80)
        {
            switch (_buf[2]) {
            case 0x80: // EN空隔符
            case 0x81: // EM空隔符
            case 0x82: // EN空格 (nut)
            case 0x83: // EM空格 (mutton)
            case 0x84: // 三分之一EM空格
            case 0x85: // 四分之一EM空格
            case 0x86: // 六分之一EM空格
            case 0x87: // 数字空格
            case 0x88: // 标点空格
            case 0x89: // 窄空格
            case 0x8A: // 发宽空格
            case 0x8B: // 零宽空格
            case 0x8C: // 零宽不连字（Zero Width Non Joiner，简称“ZWNJ”）
            case 0x8D: // 零宽连字（Zero Width Joiner，简称“ZWJ”）
            case 0xA8: // 行分隔符（Line Separator）
            case 0xA9: // 段分隔符（Paragraph Separator）
            case 0xAF: // 窄式不换行空格（Narrow No-Break Space）
                return 1;
            default:
                return 0;
            }
        }
        else if (_buf[0] == 0xE2 && _buf[1] == 0x81)
        {
            switch (_buf[2]) {
            case 0x9F: // 中数学空格（Medium Mathematical Space，简称“MMSP”）
            case 0xA0: // 文字连接符（Word Joiner）
                return 1;
            default:
                return 0;
            }
        }
        else if (_buf[0] == 0xE1)
        {
            if (_buf[1] == 0x9A && _buf[2] == 0x80)
                return 1; // 欧甘空格（Ogham Space Mark）
            else if (_buf[1] == 0xA0 && _buf[2] == 0x8E)
                return 1; // 蒙古语元音分隔符
            else
                return 0;
        }
        else if (_buf[0] == 0xEF && _buf[1] == 0xBB && _buf[2] == 0xBF)
        {
            // UTF-8 BOM
            return 1;
        }
        else {
            return 0;
        }
    }
    default:
        return 0;
    }
}

MEME_API int MEME_STDCALL
MemeRuneIndex_isFullWidthDigit(const mmbyte_t* _buf, int _size)
{
    int size = 0;
    assert(_buf != NULL);

    if (MEGO_SYMBOL__UNLIKELY(_size < 1))
        return 0;

    size = mmutf_u8rune_char_size(_buf[0]);
    if (MEGO_SYMBOL__UNLIKELY(size < 0))
        return 0;
    size = MemeMath_Min(size, _size);

    if (MEGO_SYMBOL__LIKELY(size == 3))
    {
        return _buf[0] == 0xEF && _buf[1] == 0xBC && _buf[2] >= 0x90 && _buf[2] <= 0x99;
    }
    
    return 0;
}

MEME_API int MEME_STDCALL
MemeRuneIndex_isChineseDigit(const mmbyte_t* _buf, int _size)
{
    int size = 0;
    assert(_buf != NULL);

    if (MEGO_SYMBOL__UNLIKELY(_size < 1))
        return 0;

    size = mmutf_u8rune_char_size(_buf[0]);
    if (MEGO_SYMBOL__UNLIKELY(size < 0))
        return 0;
    size = MemeMath_Min(size, _size);

    if (MEGO_SYMBOL__LIKELY(size == 3)) 
    {
        switch (_buf[0]) {
        case 0xE3: {
            // is 〇
            return _buf[1] == 0x80 && _buf[2] == 0x87;
        }
        case 0xE4: {
            switch (_buf[1]) {
            case 0xB8: {
                switch (_buf[2]) {
                case 0x80: // is 一
                case 0x83: // is 七
                case 0x89: // is 三
                    return 1;
                default:
                    return 0;
                }
            }
            case 0xB9: {
                // is 九
                return _buf[2] == 0x9D;
            }
            case 0xBA: {
                // is 二 or 五
                return _buf[2] == 0x8C || _buf[2] == 0x94;
            }
            case 0xBC: {
                // is 伍
                return _buf[2] == 0x8D;
            }
            default:
                return 0;
            }
        case 0xE5: {
            switch (_buf[1]) {
            case 0x85: {
                // is 八 or 六
                return _buf[2] == 0xAB || _buf[2] == 0xAD;
            }
            case 0x8F:
                // is 叁
                return _buf[2] == 0x81;
            case 0x9B:
                // is 四
                return _buf[2] == 0x9B;
            case 0xA3:
                // is 壹
                return _buf[2] == 0xB9;
            default:
                return 0;
            }
        }
        case 0xE6: {
            // is 捌 or 柒
            return (_buf[1] == 0x8D && _buf[2] == 0x8C) || (_buf[1] == 0x9F && _buf[2] == 0x92);
        }
        case 0xE7: {
            // is 玖
            return _buf[1] == 0x8E && _buf[2] == 0x96;
        }
        case 0xE8: {
            // is 肆 or 贰
            return (_buf[1] == 0x82 && _buf[2] == 0x86) || (_buf[1] == 0xB4 && _buf[2] == 0xB0);
        }
        case 0xE9: {
            // is 陆 or 零
            return (_buf[1] == 0x99 && _buf[2] == 0x86) || (_buf[1] == 0x9B && _buf[2] == 0xB6);
        }
        default:
            return 0;
        }
        }
    }

    return 0;
}

int MemeRune_compare(const MemeRune_t* _lhs, const MemeRune_t* _rhs)
{
    mmint_t lsize = MemeRune_size(_lhs);
    mmint_t rsize = MemeRune_size(_rhs);
    return memcmp(_lhs->byte, _rhs->byte, MemeMath_Min(lsize, rsize));
	
	//uint8_t index = 0;
	//while (_lhs->byte[index] && _rhs->byte[index]) 
	//{
	//	if(_lhs->byte[index] != _rhs->byte[index]) 
	//	{
	//		break;
	//	}
	//	++index;
	//}
	//return (int)_lhs->byte[index] - (int)_rhs->byte[index];
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