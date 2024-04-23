
#ifndef MEME_RUNE_H_INCLUDED
#define MEME_RUNE_H_INCLUDED

#include "meme/string_fwd.h"
#include "mego/predef/symbol/likely.h"
#include "mego/predef/symbol/inline.h"
#include <meme/utf/u8rune.h>

#include <mego/err/ec.h>

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

MEME_EXTERN_C_SCOPE_START

MEME_API int MEME_STDCALL 
    MemeRuneIndex_isFullWidthDigit(const mmbyte_t* _buf, int _size);

MEME_API int MEME_STDCALL 
    MemeRuneIndex_isChineseDigit(const mmbyte_t* _buf, int _size);

MEME_API int MEME_STDCALL
    MemeRuneIndex_isSpace(const mmbyte_t* _buf, int _size);

MG_CAPI_INLINE int
    MemeRuneIndex_isChPunct(const mmbyte_t* _buf, int _size)
{
    assert(_buf != NULL);

    if (_size < 0)
        _size = mmutf_u8rune_char_size(_buf[0]);

    switch (_size) {
    case 2:
        // is ·
        return (_buf[0] == 0xC2 && _buf[1] == 0xB7) ? 1 : 0;
    case 3: {
        if (_buf[0] == 0xE3 && _buf[1] == 0x80)
        {
            switch (_buf[2]) {
            case 0x81: // is 、
            case 0x82: // is 。
            case 0x88: // is 〈
            case 0x89: // is 〉
            case 0x8A: // is 《
            case 0x8B: // is 》
            case 0x8C: // is 「
            case 0x8D: // is 」
            case 0x8E: // is 『
            case 0x8F: // is 』
            case 0x90: // is 【
            case 0x91: // is 】
            case 0x94: // is 〔
            case 0x95: // is 〕
                return 1;
            default:
                return 0;
            }
        }
        else if (_buf[0] == 0xEF && _buf[1] == 0xBC)
        {
            switch (_buf[2]) {
            case 0x81: // is ！
            case 0x82: // is ＂
            case 0x83: // is ＃
            case 0x84: // is ＄
            case 0x85: // is ％
            case 0x86: // is ＆
            case 0x87: // is ＇
            case 0x88: // is （
            case 0x89: // is ）
            case 0x8A: // is ＊
            case 0x8B: // is ＋
            case 0x8C: // is ，
            case 0x8D: // is －
            case 0x8E: // is ．
            case 0x8F: // is ／
            case 0x9A: // is ：
            case 0x9B: // is ；
            case 0x9C: // is ＜
            case 0x9D: // is ＝
            case 0x9E: // is ＞
            case 0x9F: // is ？
            case 0xA0: // is ＠
            case 0xBB: // is ［
            case 0xBC: // is ＼
            case 0xBD: // is ］
            case 0xBE: // is ＾
            case 0xBF: // is ＿
            case 0xC0: // is ｀
                return 1;
            default:
                return 0;
            }
        }
        else if (_buf[0] == 0xEF && _buf[1] == 0xBD)
        {
            switch (_buf[2]) {
            case 0x9B: // is ｛
            case 0x9C: // is ｜
            case 0x9D: // is ｝
            case 0x9E: // is ～
                return 1;
            default:
                return 0;
            }
        }
        else if (_buf[0] == 0xE2 && _buf[1] == 0x80)
        {
            switch (_buf[2]) {
            case 0x80:
            case 0x98: // is ‘
            case 0x99: // is ’
            case 0x9C: // is “
            case 0x9D: // is ”
            case 0xA6: // is …
                return 1;
            default:
                return 0;
            }
        }
        else if (_buf[0] == 0xEF && _buf[1] == 0xB8)
        {
            switch (_buf[2]) {
            case 0xB5: // is ︵
            case 0xB6: // is ︶
            case 0xB7: // is ︷
            case 0xB8: // is ︸
            case 0xB9: // is ︹
            case 0xBA: // is ︺
            case 0xBB: // is ︻
            case 0xBC: // is ︼
            case 0xBD: // is ︽
            case 0xBE: // is ︾
            case 0xBF: // is ︿
                return 1;
            default:
                return 0;
            }
        }
        else if (_buf[0] == 0xEF && _buf[1] == 0xB9)
        {
            switch (_buf[2]) {
            case 0x80: // is ﹀
            case 0x81: // is ﹁
            case 0x82: // is ﹂
            case 0x83: // is ﹃
            case 0x84: // is ﹄
            case 0x87: // is ﹇
            case 0x88: // is ﹈
                return 1;
            default:
                return 0;
            }
        }
        else {
            // is ─
            return (_buf[0] == 0xE2 && _buf[1] == 0x94 && _buf[2] == 0x80) ? 1 : 0;
        }
    }
    default: return 0;
    }
}

MG_CAPI_INLINE MemeRune_t
    MemeRune_getInitObject()
{
    MemeRune_t w;
    memset(&w, 0, sizeof(w));
    w.attr.capacity = 7;
    return w;
}

MG_CAPI_INLINE MemeInteger_t
    MemeRune_size(const MemeRune_t* _s)
{
    assert(_s != NULL);

    return 7 - _s->attr.capacity;
}

MG_CAPI_INLINE int
    MemeRune_initByOther(MemeRune_t* _out, const MemeRune_t* _other)
{
    assert(_out != NULL && _other != NULL);

    *_out = *_other;
    return 0;
}

MG_CAPI_INLINE int
    MemeRune_initByByte(MemeRune_t* _out, char _ch)
{
    assert(_out != NULL);

    _out->attr.capacity = 6;
    _out->attr.invalid  = 0;
    _out->byte[0] = _ch;
    _out->byte[1] = '\0';
    return 0;
}

MG_CAPI_INLINE int
    MemeRune_initByUtf8Bytes(MemeRune_t* _out, const MemeByte_t* _buf, MemeInteger_t _len)
{
    assert(_out != NULL && _buf != NULL);

    if ((_len > 7))
        return MGEC__INVAL;
    if (_len < 0)
        _len = mmutf_u8rune_char_size(_buf[0]);

    _out->attr.capacity = (mmbyte_t)(7 - _len);
    _out->attr.invalid  = 0;
    memcpy(_out->byte, _buf, _len);
    _out->byte[MemeRune_size(_out)] = '\0';
    return 0;
}

MG_CAPI_INLINE int
    MemeRune_reset(MemeRune_t* _out)
{
    assert(_out != NULL);

    memset(_out, 0, sizeof(*_out));
    _out->attr.capacity = 7;
    return 0;
}

MG_CAPI_INLINE int
    MemeRune_assign(MemeRune_t* _s, const MemeRune_t* _other)
{
    assert(_s != NULL && _other != NULL);

    *_s = *_other;
    return 0;
}

MG_CAPI_INLINE int
    MemeRune_swap(MemeRune_t* _lhs, MemeRune_t* _rhs)
{
    assert(_lhs != NULL && _rhs != NULL);

    MemeRune_t tmp = *_lhs;
    *_lhs = *_rhs;
    *_rhs = tmp;
    return 0;
}

MG_CAPI_INLINE int
    MemeRune_isEmpty(const MemeRune_t* _s)
{
    assert(_s != NULL);

    return _s->attr.capacity == 7;
}

MG_CAPI_INLINE const MemeByte_t*
    MemeRune_data(const MemeRune_t* _s)
{
    return _s->byte;
}

MG_CAPI_INLINE MemeByte_t*
    MemeRune_dataNotConst(const MemeRune_t* _s)
{
    return ((MemeRune_t*)_s)->byte;
}

MG_CAPI_INLINE int
    MemeRune_isValid(const MemeRune_t* _s)
{
    assert(_s != NULL);

    return _s->attr.invalid == 0;
}

MG_CAPI_INLINE int
    MemeRune_isSpace(const MemeRune_t* _s)
{
    assert(_s != NULL);
    
    return MemeRuneIndex_isSpace(MemeRune_data(_s), (int)MemeRune_size(_s)) == 0 ? 0 : 1;
}

//! 使用国家标准GB/T15834-2011出现的标点符号进行判断
//! 
MG_CAPI_INLINE int
    MemeRune_isChPunct(const MemeRune_t* _s)
{
    assert(_s != NULL);
    
    return MemeRuneIndex_isChPunct(MemeRune_data(_s), (int)MemeRune_size(_s)) == 0 ? 0 : 1;
}

MG_CAPI_INLINE int
    MemeRune_resize(MemeRune_t* _s, uint8_t _count)
{
    assert(_s != NULL);

    if ((_count > 7))
        return MGEC__INVAL;

    _s->attr.capacity = 7 - _count;
    _s->byte[MemeRune_size(_s)] = '\0';
    return 0;
}

MG_CAPI_INLINE int
    MemeRune_isMulitChar(const MemeRune_t* _s)
{
    assert(_s != NULL);

    return !!(_s->byte[0] & 0x80);
}

//MG_CAPI_INLINE int
//    MemeRune_byteSize(const MemeRune_t* _s)
//{
//    assert(_s != NULL);
//    if (MemeRune_size(_s) < 1)
//        return 0;
//    
//    return mmutf_u8rune_char_size(_s->byte[0]);
//}


MG_CAPI_INLINE int
    mmruneidx_is_space(const mmbyte_t* _buf, int _size)
{
    assert(_buf != NULL);

    return MemeRuneIndex_isSpace(_buf, _size);
}

MEME_EXTERN_C_SCOPE_ENDED

#endif // MEME_RUNE_H_INCLUDED
