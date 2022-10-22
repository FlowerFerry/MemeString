
#ifndef MEME_WORD_H_INCLUDED
#define MEME_WORD_H_INCLUDED

#include "meme/string_fwd.h"
#include "mego/predef/symbol/likely.h"

#include <assert.h>
#include <errno.h>
#include <string.h>

MEME_EXTERN_C_SCOPE_START

static inline MemeWord_t
MemeWord_getInitObject()
{
    MemeWord_t w;
    memset(&w, 0, sizeof(w));
    w.capacity = 7;
    return w;
}

static inline int
MemeWord_initByOther(MemeWord_t* _out, const MemeWord_t* _other)
{
    assert(_out != NULL && _other != NULL);

    *_out = *_other;
    return 0;
}

static inline int
MemeWord_initByByte(MemeWord_t* _out, char _ch)
{
    assert(_out != NULL);

    _out->capacity = 6;
    _out->byte[0] = _ch;
    _out->byte[1] = '\0';
    return 0;
}

static inline int
MemeWord_initByUtf8Bytes(MemeWord_t* _out, const MemeByte_t* _buf, MemeInteger_t _len);

static inline int
MemeWord_reset(MemeWord_t* _out)
{
    assert(_out != NULL);

    memset(_out, 0, sizeof(*_out));
    _out->capacity = 7;
    return 0;
}

static inline int
MemeWord_assign(MemeWord_t* _s, const MemeWord_t* _other)
{
    assert(_s != NULL && _other != NULL);

    *_s = *_other;
    return 0;
}

static inline int
MemeWord_swap(MemeWord_t* _lhs, MemeWord_t* _rhs)
{
    assert(_lhs != NULL && _rhs != NULL);

    MemeWord_t tmp = *_lhs;
    *_lhs = *_rhs;
    *_rhs = tmp;
    return 0;
}

static inline int
MemeWord_isEmpty(const MemeWord_t* _s)
{
    assert(_s != NULL);

    return _s->capacity == 7;
}

static inline const MemeByte_t*
MemeWord_data(const MemeWord_t* _s)
{
    return _s->byte;
}

static inline MemeByte_t*
MemeWord_dataNotConst(const MemeWord_t* _s)
{
    return ((MemeWord_t*)_s)->byte;
}

static inline MemeInteger_t
MemeWord_size(const MemeWord_t* _s)
{
    assert(_s != NULL);

    return 7 - _s->capacity;
}

static inline int
MemeWord_isValid(const MemeWord_t* _s)
{
    assert(_s != NULL);

    return _s->invalid == 0;
}

static inline int
MemeWord_resize(MemeWord_t* _s, uint8_t _count)
{
    assert(_s != NULL);

    if (MEGO_SYMBOL__UNLIKELY(_count > 7))
        return MEME_ENO__POSIX_OFFSET(EINVAL);

    _s->capacity = 7 - _count;
    _s->byte[MemeWord_size(_s)] = '\0';
    return 0;
}

static inline int
MemeWord_isMulitChar(const MemeWord_t* _s)
{
    assert(_s != NULL);

    return !!(_s->byte[0] & 0x80);
}

static inline int
MemeWord_initByUtf8Bytes(MemeWord_t* _out, const MemeByte_t* _buf, MemeInteger_t _len)
{
    assert(_out != NULL && _buf != NULL);

    if (MEGO_SYMBOL__UNLIKELY(_len < 0 || _len > 7))
        return MEME_ENO__POSIX_OFFSET(EINVAL);

    _out->capacity = 7 - _len;
    memcpy(_out->byte, _buf, _len);
    _out->byte[MemeWord_size(_out)] = '\0';
    return 0;
}

MEME_EXTERN_C_SCOPE_ENDED

#endif // MEME_WORD_H_INCLUDED