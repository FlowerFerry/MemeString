
#ifndef MEME_RUNE_H_INCLUDED
#define MEME_RUNE_H_INCLUDED

#include "meme/string_fwd.h"
#include "mego/predef/symbol/likely.h"

#include <assert.h>
#include <errno.h>
#include <string.h>

MEME_EXTERN_C_SCOPE_START

static inline MemeRune_t
MemeRune_getInitObject()
{
    MemeRune_t w;
    memset(&w, 0, sizeof(w));
    w.attr.capacity = 7;
    return w;
}

static inline int
MemeRune_initByOther(MemeRune_t* _out, const MemeRune_t* _other)
{
    assert(_out != NULL && _other != NULL);

    *_out = *_other;
    return 0;
}

static inline int
MemeRune_initByByte(MemeRune_t* _out, char _ch)
{
    assert(_out != NULL);

    _out->attr.capacity = 6;
    _out->byte[0] = _ch;
    _out->byte[1] = '\0';
    return 0;
}

static inline int
MemeRune_initByUtf8Bytes(MemeRune_t* _out, const MemeByte_t* _buf, MemeInteger_t _len);

static inline int
MemeRune_reset(MemeRune_t* _out)
{
    assert(_out != NULL);

    memset(_out, 0, sizeof(*_out));
    _out->attr.capacity = 7;
    return 0;
}

static inline int
MemeRune_assign(MemeRune_t* _s, const MemeRune_t* _other)
{
    assert(_s != NULL && _other != NULL);

    *_s = *_other;
    return 0;
}

static inline int
MemeRune_swap(MemeRune_t* _lhs, MemeRune_t* _rhs)
{
    assert(_lhs != NULL && _rhs != NULL);

    MemeRune_t tmp = *_lhs;
    *_lhs = *_rhs;
    *_rhs = tmp;
    return 0;
}

static inline int
MemeRune_isEmpty(const MemeRune_t* _s)
{
    assert(_s != NULL);

    return _s->attr.capacity == 7;
}

static inline const MemeByte_t*
MemeRune_data(const MemeRune_t* _s)
{
    return _s->byte;
}

static inline MemeByte_t*
MemeRune_dataNotConst(const MemeRune_t* _s)
{
    return ((MemeRune_t*)_s)->byte;
}

static inline MemeInteger_t
MemeRune_size(const MemeRune_t* _s)
{
    assert(_s != NULL);

    return 7 - _s->attr.capacity;
}

static inline int
MemeRune_isValid(const MemeRune_t* _s)
{
    assert(_s != NULL);

    return _s->attr.invalid == 0;
}

static inline int
MemeRune_resize(MemeRune_t* _s, uint8_t _count)
{
    assert(_s != NULL);

    if ((_count > 7))
        return MMENO__POSIX_OFFSET(EINVAL);

    _s->attr.capacity = 7 - _count;
    _s->byte[MemeRune_size(_s)] = '\0';
    return 0;
}

static inline int
MemeRune_isMulitChar(const MemeRune_t* _s)
{
    assert(_s != NULL);

    return !!(_s->byte[0] & 0x80);
}

static inline int
MemeRune_initByUtf8Bytes(MemeRune_t* _out, const MemeByte_t* _buf, MemeInteger_t _len)
{
    assert(_out != NULL && _buf != NULL);

    if ((_len < 0 || _len > 7))
        return MMENO__POSIX_OFFSET(EINVAL);

    _out->attr.capacity = 7 - _len;
    memcpy(_out->byte, _buf, _len);
    _out->byte[MemeRune_size(_out)] = '\0';
    return 0;
}

MEME_EXTERN_C_SCOPE_ENDED

#endif // MEME_RUNE_H_INCLUDED
