
#include "meme/impl/algorithm.h"
#include <meme/impl/string.h>
#include <meme/simd/simd.h>
#include <mego/predef/symbol/likely.h>

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static MemeInteger_t __MemeBoyerMoore_isPrefix(
    const uint8_t* _word, MemeInteger_t _word_len, MemeInteger_t _pos)
{
    MemeInteger_t index;
    MemeInteger_t suffixlen = _word_len - _pos;

    for (index = 0; index < suffixlen; ++index)
    {
        if (_word[index] != _word[_pos + index])
        {
            return 0;
        }
    }
    return 1;
}

static MemeInteger_t __MemeBoyerMoore_suffixLength(
    const uint8_t* _word, MemeInteger_t _word_len, MemeInteger_t _pos)
{
    MemeInteger_t index;
    for (index = 0; (_word[_pos - index] == _word[_word_len - 1 - index]) && (index < _pos); ++index);
    return index;
}

static void __MemeBoyerMoore_makeBadDelta(
    MemeInteger_t* _delta1, MemeInteger_t _delta1_len,
    const uint8_t* _pat, MemeInteger_t _pat_len) 
{
    mmsimd_iptr_fill(_delta1, _delta1_len, _pat_len);

    // TO_DO : It can also be optimized
    //for (MemeInteger_t index = 0; index < _delta1_len; ++index) 
    //    _delta1[index] = _pat_len;
    
    for (MemeInteger_t index = 0; index < _pat_len - 1; ++index)
        _delta1[_pat[index]] = _pat_len - 1 - index;
    
}

static void __MemeBoyerMoore_makeGoodDelta(MemeInteger_t* _delta2, const uint8_t* _pat, MemeInteger_t _pat_len)
{
    MemeInteger_t p;
    MemeInteger_t last_prefix_index = 1;

    for (p = _pat_len - 1; p >= 0; --p) 
    {
        if (__MemeBoyerMoore_isPrefix(_pat, _pat_len, p + 1))
        {
            last_prefix_index = p + 1;
        }
        _delta2[p] = (_pat_len - 1 - p) + last_prefix_index;
    }

    for (p = 0; p < _pat_len - 1; ++p) 
    {
        MemeInteger_t slen = __MemeBoyerMoore_suffixLength(_pat, _pat_len, p);
        if (_pat[p - slen] != _pat[_pat_len - 1 - slen]) 
        {
            _delta2[_pat_len - 1 - slen] = _pat_len - 1 - p + slen;
        }
    }
}

static inline MemeInteger_t 
    mbit_distance(const MemeByte_t* _last, const MemeByte_t* _first)
{
    return _last - _first;
}
static inline const MemeByte_t* 
    mbit_advance(const MemeByte_t* _it, MemeInteger_t _n)
{
    return _it + _n;
}
static inline const MemeByte_t* 
    mbit_next(const MemeByte_t* _it)
{
    return _it + 1;
}
static inline const MemeByte_t* 
    mbit_prev(const MemeByte_t * _it)
{
    return _it - 1;
}
static inline MemeInteger_t 
    mbit_rDistance(const MemeByte_t* _last, const MemeByte_t* _first)
{
    return _first - _last;
}
static inline const MemeByte_t* 
    mbit_rAdvance(const MemeByte_t* _rit, MemeInteger_t _n)
{
    return _rit - _n;
}
static inline const MemeByte_t* 
    mbit_rNext(const MemeByte_t* _rit)
{
    return _rit - 1;
}
static inline const MemeByte_t* 
    mbit_rPrev(const MemeByte_t* _rit)
{
    return _rit + 1;
}

static inline MemeInteger_t __MemeReverseBoyerMoore_isPrefix(
    const MemeByte_t* _pat_rbegin, const MemeByte_t* _pat_rend, MemeInteger_t _pos)
{
    MemeInteger_t suffixlen = mbit_rDistance(_pat_rend, _pat_rbegin) - _pos;
    for (MemeInteger_t index = 0; index < suffixlen; ++index)
    {
        if (*mbit_rAdvance(_pat_rbegin, index) != *mbit_rAdvance(_pat_rbegin, _pos + index))
            return 0;
    }
    return 1;
    
}

static MemeInteger_t __MemeReverseBoyerMoore_suffixLength(
    const MemeByte_t* _pat_rbegin, const MemeByte_t* _pat_rend, MemeInteger_t _pos)
{
    MemeInteger_t len = mbit_rDistance(_pat_rend, _pat_rbegin);
    MemeInteger_t index;
    for (index = 0; 
        (*mbit_rAdvance(_pat_rbegin, _pos - index) 
            == *mbit_rAdvance(_pat_rbegin, len - 1 - index))
        && (index < _pos); ++index)
        continue;
    return index;
    
}

static inline void __MemeReverseBoyerMoore_makeBadDelta(
    MemeInteger_t* _delta1, MemeInteger_t _delta1_len, 
    const MemeByte_t* _pat_rbegin, const MemeByte_t* _pat_rend)
{
    MemeInteger_t diff = mbit_rDistance(_pat_rend, _pat_rbegin);
    //for (MemeInteger_t index = 0; index < _delta1_len; ++index)
    //    _delta1[index] = diff;
    mmsimd_iptr_fill(_delta1, _delta1_len, diff);

    for (const MemeByte_t *it = _pat_rbegin, *last = mbit_rPrev(_pat_rend);
        mbit_rDistance(last, it) > 0; it = mbit_rNext(it))
    {
        _delta1[*it] = mbit_rDistance(last, it);
    }
}

static void __MemeReverseBoyerMoore_makeGoodDelta(
    MemeInteger_t* _delta2, const MemeByte_t* _pat_rbegin, const MemeByte_t* _pat_rend)
{
    MemeInteger_t p;
    MemeInteger_t last_prefix_index = 1;
    MemeInteger_t pat_len = mbit_rDistance(_pat_rend, _pat_rbegin);

    for (p = pat_len - 1; p >= 0; --p)
    {
        if (__MemeReverseBoyerMoore_isPrefix(_pat_rbegin, _pat_rend, p + 1))
        {
            last_prefix_index = p + 1;
        }
        _delta2[p] = (pat_len - 1 - p) + last_prefix_index;
    }

    for (p = 0; p < pat_len - 1; ++p)
    {
        MemeInteger_t slen = __MemeReverseBoyerMoore_suffixLength(_pat_rbegin, _pat_rend, p);
        if (*mbit_rAdvance(_pat_rbegin, p - slen) != *mbit_rAdvance(_pat_rbegin, pat_len - 1 - slen))
            _delta2[pat_len - 1 - slen] = pat_len - 1 - p + slen;

    }
    
}

mmint_t MemeImpl_SearchByBoyerMoore(
	const uint8_t* _haystack, mmint_t _haystack_len,
	const uint8_t* _needle, mmint_t _needle_len)
{
    MemeInteger_t  b_delta[256];
    MemeInteger_t* g_delta;

    assert(_haystack != NULL    && MemeImpl_SearchByBoyerMoore);
    assert(_haystack_len != 0   && MemeImpl_SearchByBoyerMoore);
    assert(_needle != NULL      && MemeImpl_SearchByBoyerMoore);
    assert(_needle_len != 0     && MemeImpl_SearchByBoyerMoore);

    if (_haystack_len >= 0 && _haystack_len < _needle_len)
        return -1;

    g_delta = malloc(_needle_len * sizeof(MemeInteger_t));

    __MemeBoyerMoore_makeBadDelta (b_delta, sizeof(b_delta) / sizeof(b_delta[0]), _needle, _needle_len);
    __MemeBoyerMoore_makeGoodDelta(g_delta, _needle, _needle_len);
    
    if ((_haystack_len < 0)) 
    {
        const MemeByte_t* index = _haystack;
        for (MemeInteger_t count = 0; count < _needle_len; ++count)
            if (*index++ == '\0') {
                free(g_delta);
                return -1;
            }
        --_needle_len;
        --index;
        
        while (1) {
            MemeInteger_t needle_index = _needle_len;
            MemeInteger_t move_count;
            while (needle_index >= 0 && (*index == _needle[needle_index]))
            {
                --index;
                --needle_index;
            }
            if (needle_index < 0) {
                free(g_delta);
                return index + 1 - _haystack;
            }

            move_count = MemeMath_Max(b_delta[*index], g_delta[needle_index]);
            while (move_count --> 0)
            {
                if (*(++index) == '\0')
                {
                    free(g_delta);
                    return -1;
                }
            }
        }
    }
    else {
        --_needle_len;
        MemeInteger_t index = _needle_len;
        while (index < _haystack_len) {
            MemeInteger_t needle_index = _needle_len;
            while (needle_index >= 0 && (_haystack[index] == _needle[needle_index]))
            {
                --index;
                --needle_index;
            }
            if (needle_index < 0) {
                free(g_delta);
                return index + 1;
            }

            index += MemeMath_Max(b_delta[_haystack[index]], g_delta[needle_index]);
        }
    }
    free(g_delta);
    return -1;
}

mmint_t MemeImpl_ReverseSearchByBoyerMoore(
    const uint8_t* _haystack, mmint_t _haystack_len,
    const uint8_t* _needle, mmint_t _needle_len)
{    
    MemeInteger_t  b_delta[256];
    MemeInteger_t* g_delta;
    const MemeByte_t* s_rbegin;
    const MemeByte_t* s_rend;
    const MemeByte_t* p_rbegin;
    const MemeByte_t* p_rend;
    MemeInteger_t index;
    
    assert(_haystack != NULL  && MemeImpl_ReverseSearchByBoyerMoore);
    assert(_haystack_len != 0 && MemeImpl_ReverseSearchByBoyerMoore);
    assert(_needle != NULL    && MemeImpl_ReverseSearchByBoyerMoore);
    assert(_needle_len != 0   && MemeImpl_ReverseSearchByBoyerMoore);
    
    if ((_haystack_len < 0))
        _haystack_len = strlen((const char*)_haystack);
    
    if ((_haystack_len < _needle_len))
        return -1;

    s_rbegin = _haystack + _haystack_len - 1;
    s_rend   = _haystack - 1;
    p_rbegin = _needle + _needle_len - 1;
    p_rend   = _needle - 1;
    g_delta  = malloc(_needle_len * sizeof(MemeInteger_t));
    __MemeReverseBoyerMoore_makeBadDelta (
        b_delta, sizeof(b_delta) / sizeof(b_delta[0]), p_rbegin, p_rend);
    __MemeReverseBoyerMoore_makeGoodDelta(g_delta, p_rbegin, p_rend);
    
    --_needle_len;
    index = _needle_len;
    while (index < _haystack_len)
    {
        MemeInteger_t needle_index = _needle_len;
        while (needle_index >= 0 
            && *mbit_rAdvance(s_rbegin, index) == *mbit_rAdvance(p_rbegin, needle_index))
        {
            --index;
            --needle_index;
        }
        if (needle_index < 0)
        {
            free(g_delta);
            return _haystack_len - (index + 1) - (_needle_len + 1);
        }
        index += MemeMath_Max(b_delta[*mbit_rAdvance(s_rbegin, index)], g_delta[needle_index]);
    }

    free(g_delta);
    return -1;
}

/* ── case-insensitive Boyer-Moore helpers ─────────────────────────────── */

/*
 * ASCII-only case-folding table, kept as a function-local static so the
 * initialised array lives in the read-only data section but is only visible
 * through the accessor below.  _BM_CF() is the only reader, so the table never
 * needs to be named anywhere else in this file.
 */
static const uint8_t* __MemeImpl_casefoldTable(void)
{
    static const uint8_t table[256] = {
        0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
        0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
        0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
        0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
        0x40,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
        0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x5B,0x5C,0x5D,0x5E,0x5F,
        0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
        0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
        0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
        0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
        0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
        0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
        0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
        0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
        0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
        0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF
    };
    return table;
}

#define _BM_CF(c) __MemeImpl_casefoldTable()[(uint8_t)(c)]

/* ── case-insensitive  forward  Boyer-Moore helpers ──────────────────── */

static MemeInteger_t __MemeBoyerMooreCI_isPrefix(
    const uint8_t* _word, MemeInteger_t _word_len, MemeInteger_t _pos)
{
    MemeInteger_t index;
    MemeInteger_t suffixlen = _word_len - _pos;

    for (index = 0; index < suffixlen; ++index)
    {
        if (_BM_CF(_word[index]) != _BM_CF(_word[_pos + index]))
            return 0;
    }
    return 1;
}

static MemeInteger_t __MemeBoyerMooreCI_suffixLength(
    const uint8_t* _word, MemeInteger_t _word_len, MemeInteger_t _pos)
{
    MemeInteger_t index;
    for (index = 0;
        (_BM_CF(_word[_pos - index]) == _BM_CF(_word[_word_len - 1 - index]))
        && (index < _pos); ++index);
    return index;
}

static void __MemeBoyerMooreCI_makeBadDelta(
    MemeInteger_t* _delta1, MemeInteger_t _delta1_len,
    const uint8_t* _pat, MemeInteger_t _pat_len)
{
    mmsimd_iptr_fill(_delta1, _delta1_len, _pat_len);

    for (MemeInteger_t index = 0; index < _pat_len - 1; ++index)
        _delta1[_BM_CF(_pat[index])] = _pat_len - 1 - index;
}

static void __MemeBoyerMooreCI_makeGoodDelta(
    MemeInteger_t* _delta2, const uint8_t* _pat, MemeInteger_t _pat_len)
{
    MemeInteger_t p;
    MemeInteger_t last_prefix_index = 1;

    for (p = _pat_len - 1; p >= 0; --p)
    {
        if (__MemeBoyerMooreCI_isPrefix(_pat, _pat_len, p + 1))
            last_prefix_index = p + 1;
        _delta2[p] = (_pat_len - 1 - p) + last_prefix_index;
    }

    for (p = 0; p < _pat_len - 1; ++p)
    {
        MemeInteger_t slen = __MemeBoyerMooreCI_suffixLength(_pat, _pat_len, p);
        if (_BM_CF(_pat[p - slen]) != _BM_CF(_pat[_pat_len - 1 - slen]))
            _delta2[_pat_len - 1 - slen] = _pat_len - 1 - p + slen;
    }
}

/* ── case-insensitive  reverse  Boyer-Moore helpers ──────────────────── */

static inline MemeInteger_t __MemeReverseBoyerMooreCI_isPrefix(
    const MemeByte_t* _pat_rbegin, const MemeByte_t* _pat_rend, MemeInteger_t _pos)
{
    MemeInteger_t suffixlen = mbit_rDistance(_pat_rend, _pat_rbegin) - _pos;
    for (MemeInteger_t index = 0; index < suffixlen; ++index)
    {
        if (_BM_CF(*mbit_rAdvance(_pat_rbegin, index))
            != _BM_CF(*mbit_rAdvance(_pat_rbegin, _pos + index)))
            return 0;
    }
    return 1;
}

static MemeInteger_t __MemeReverseBoyerMooreCI_suffixLength(
    const MemeByte_t* _pat_rbegin, const MemeByte_t* _pat_rend, MemeInteger_t _pos)
{
    MemeInteger_t len = mbit_rDistance(_pat_rend, _pat_rbegin);
    MemeInteger_t index;
    for (index = 0;
        (_BM_CF(*mbit_rAdvance(_pat_rbegin, _pos - index))
            == _BM_CF(*mbit_rAdvance(_pat_rbegin, len - 1 - index)))
        && (index < _pos); ++index)
        continue;
    return index;
}

static inline void __MemeReverseBoyerMooreCI_makeBadDelta(
    MemeInteger_t* _delta1, MemeInteger_t _delta1_len,
    const MemeByte_t* _pat_rbegin, const MemeByte_t* _pat_rend)
{
    MemeInteger_t diff = mbit_rDistance(_pat_rend, _pat_rbegin);
    mmsimd_iptr_fill(_delta1, _delta1_len, diff);

    for (const MemeByte_t *it = _pat_rbegin, *last = mbit_rPrev(_pat_rend);
        mbit_rDistance(last, it) > 0; it = mbit_rNext(it))
    {
        _delta1[_BM_CF(*it)] = mbit_rDistance(last, it);
    }
}

static void __MemeReverseBoyerMooreCI_makeGoodDelta(
    MemeInteger_t* _delta2, const MemeByte_t* _pat_rbegin, const MemeByte_t* _pat_rend)
{
    MemeInteger_t p;
    MemeInteger_t last_prefix_index = 1;
    MemeInteger_t pat_len = mbit_rDistance(_pat_rend, _pat_rbegin);

    for (p = pat_len - 1; p >= 0; --p)
    {
        if (__MemeReverseBoyerMooreCI_isPrefix(_pat_rbegin, _pat_rend, p + 1))
            last_prefix_index = p + 1;
        _delta2[p] = (pat_len - 1 - p) + last_prefix_index;
    }

    for (p = 0; p < pat_len - 1; ++p)
    {
        MemeInteger_t slen =
            __MemeReverseBoyerMooreCI_suffixLength(_pat_rbegin, _pat_rend, p);
        if (_BM_CF(*mbit_rAdvance(_pat_rbegin, p - slen))
            != _BM_CF(*mbit_rAdvance(_pat_rbegin, pat_len - 1 - slen)))
            _delta2[pat_len - 1 - slen] = pat_len - 1 - p + slen;
    }
}

/* ── case-insensitive Boyer-Moore  (forward) ─────────────────────────── */

mmint_t MemeImpl_SearchByBoyerMoore_CaseInsensitive(
    const uint8_t* _haystack, mmint_t _haystack_len,
    const uint8_t* _needle, mmint_t _needle_len)
{
    MemeInteger_t  b_delta[256];
    MemeInteger_t* g_delta;
    uint8_t*       needle_folded;

    assert(_haystack != NULL    && MemeImpl_SearchByBoyerMoore_CaseInsensitive);
    assert(_haystack_len != 0   && MemeImpl_SearchByBoyerMoore_CaseInsensitive);
    assert(_needle != NULL      && MemeImpl_SearchByBoyerMoore_CaseInsensitive);
    assert(_needle_len != 0     && MemeImpl_SearchByBoyerMoore_CaseInsensitive);

    if (_haystack_len >= 0 && _haystack_len < _needle_len)
        return -1;

    g_delta       = malloc(_needle_len * sizeof(MemeInteger_t));
    needle_folded = malloc((size_t)_needle_len);
    if (g_delta == NULL || needle_folded == NULL) {
        free(g_delta);
        free(needle_folded);
        return -1;
    }

    /* Pre-fold the needle so the hot loop never calls _BM_CF per needle byte. */
    for (MemeInteger_t i = 0; i < _needle_len; ++i)
        needle_folded[i] = _BM_CF(_needle[i]);

    __MemeBoyerMooreCI_makeBadDelta(b_delta,
        sizeof(b_delta) / sizeof(b_delta[0]), _needle, _needle_len);
    __MemeBoyerMooreCI_makeGoodDelta(g_delta, _needle, _needle_len);

    if ((_haystack_len < 0))
    {
        const MemeByte_t* index = _haystack;
        for (MemeInteger_t count = 0; count < _needle_len; ++count)
            if (*index++ == '\0') {
                free(needle_folded); free(g_delta);
                return -1;
            }
        --_needle_len;
        --index;

        while (1) {
            MemeInteger_t needle_index = _needle_len;
            MemeInteger_t move_count;
            while (needle_index >= 0
                && _BM_CF(*index) == needle_folded[needle_index])
            {
                --index;
                --needle_index;
            }
            if (needle_index < 0) {
                free(needle_folded); free(g_delta);
                return index + 1 - _haystack;
            }

            move_count = MemeMath_Max(
                b_delta[_BM_CF(*index)], g_delta[needle_index]);
            while (move_count --> 0)
            {
                if (*(++index) == '\0')
                {
                    free(needle_folded); free(g_delta);
                    return -1;
                }
            }
        }
    }
    else
    {
        --_needle_len;
        MemeInteger_t index = _needle_len;
        while (index < _haystack_len) {
            MemeInteger_t needle_index = _needle_len;
            while (needle_index >= 0
                && _BM_CF(_haystack[index]) == needle_folded[needle_index])
            {
                --index;
                --needle_index;
            }
            if (needle_index < 0) {
                free(needle_folded); free(g_delta);
                return index + 1;
            }

            index += MemeMath_Max(
                b_delta[_BM_CF(_haystack[index])], g_delta[needle_index]);
        }
    }
    free(needle_folded);
    free(g_delta);
    return -1;
}

/* ── case-insensitive Boyer-Moore  (reverse) ─────────────────────────── */

mmint_t MemeImpl_ReverseSearchByBoyerMoore_CaseInsensitive(
    const uint8_t* _haystack, mmint_t _haystack_len,
    const uint8_t* _needle, mmint_t _needle_len)
{
    MemeInteger_t  b_delta[256];
    MemeInteger_t* g_delta;
    const MemeByte_t* s_rbegin;
    const MemeByte_t* s_rend;
    const MemeByte_t* p_rbegin;
    const MemeByte_t* p_rend;
    MemeInteger_t index;
    uint8_t*       needle_folded;

    assert(_haystack != NULL
        && MemeImpl_ReverseSearchByBoyerMoore_CaseInsensitive);
    assert(_haystack_len != 0
        && MemeImpl_ReverseSearchByBoyerMoore_CaseInsensitive);
    assert(_needle != NULL
        && MemeImpl_ReverseSearchByBoyerMoore_CaseInsensitive);
    assert(_needle_len != 0
        && MemeImpl_ReverseSearchByBoyerMoore_CaseInsensitive);

    if ((_haystack_len < 0))
        _haystack_len = strlen((const char*)_haystack);

    if ((_haystack_len < _needle_len))
        return -1;

    s_rbegin       = _haystack + _haystack_len - 1;
    s_rend         = _haystack - 1;
    p_rbegin       = _needle + _needle_len - 1;
    p_rend         = _needle - 1;
    g_delta        = malloc(_needle_len * sizeof(MemeInteger_t));
    needle_folded  = malloc((size_t)_needle_len);
    if (g_delta == NULL || needle_folded == NULL) {
        free(g_delta);
        free(needle_folded);
        return -1;
    }

    for (MemeInteger_t i = 0; i < _needle_len; ++i)
        needle_folded[i] = _BM_CF(_needle[i]);

    __MemeReverseBoyerMooreCI_makeBadDelta(
        b_delta, sizeof(b_delta) / sizeof(b_delta[0]), p_rbegin, p_rend);
    __MemeReverseBoyerMooreCI_makeGoodDelta(g_delta, p_rbegin, p_rend);

    --_needle_len;
    index = _needle_len;
    while (index < _haystack_len)
    {
        MemeInteger_t needle_index = _needle_len;
        while (needle_index >= 0
            && _BM_CF(*mbit_rAdvance(s_rbegin, index))
                == needle_folded[_needle_len - needle_index])
        {
            --index;
            --needle_index;
        }
        if (needle_index < 0)
        {
            free(needle_folded); free(g_delta);
            return _haystack_len - (index + 1) - (_needle_len + 1);
        }
        index += MemeMath_Max(
            b_delta[_BM_CF(*mbit_rAdvance(s_rbegin, index))],
            g_delta[needle_index]);
    }

    free(needle_folded);
    free(g_delta);
    return -1;
}

/* ── short-needle fast paths ──────────────────────────────────────────── */

/*
 * Boyer-Moore pays a fixed preprocessing cost (a 256-entry bad-character
 * table plus a heap-allocated good-suffix table) before it examines a single
 * byte.  For a short needle that cost dominates: measured on this host, a
 * 3-byte needle in a 91-byte haystack took 103 ns through BM versus 5 ns
 * through strstr.
 *
 * These helpers therefore avoid BM for the cases where it cannot pay for
 * itself:
 *   - needle_len <= MEME_IMPL_SHORT_NEEDLE_MAX, or
 *   - at most MEME_IMPL_FEW_CANDIDATES possible start positions
 *
 * Strategy: locate the first needle byte with the SIMD dispatcher (which
 * already picks AVX2 / NEON / scalar at runtime), verify the LAST byte, then
 * memcmp the middle.  Checking the last byte before the middle is what makes
 * this cheap: real text usually differs at the end of a candidate rather than
 * in the interior, so most candidates are rejected by a single compare.
 *
 * Both helpers require a non-negative _haystack_len and 2 <= _needle_len.
 */
#define MEME_IMPL_SHORT_NEEDLE_MAX   (16)
#define MEME_IMPL_FEW_CANDIDATES     (8)

static int __MemeImpl_shouldUseShortPath(mmint_t _haystack_len, mmint_t _needle_len)
{
    if (_needle_len <= MEME_IMPL_SHORT_NEEDLE_MAX)
        return 1;

    /* Unknown haystack length: the candidate count cannot be evaluated yet,
     * so only the needle-length rule applies. */
    if (_haystack_len < 0)
        return 0;

    return (_haystack_len - _needle_len) <= MEME_IMPL_FEW_CANDIDATES;
}

/* Folds one byte for case-insensitive comparison, using the same ASCII-only
 * table as the Boyer-Moore path (_bm_casefold_table, defined above) so both
 * paths agree on every input, including 0x80..0xFF which fold to themselves. */
#define _SHORT_CF(c) _BM_CF(c)

/* memcmp equivalent that optionally folds both operands.  Kept as a tiny
 * loop rather than two call sites so the case-sensitive case still compiles
 * down to a real memcmp on MSVC. */
static int __MemeImpl_windowEquals(
    const uint8_t* _hay, const uint8_t* _needle,
    mmint_t _len, int _fold)
{
    if (!_fold)
        return memcmp(_hay, _needle, (size_t)_len) == 0;

    for (mmint_t i = 0; i < _len; ++i)
        if (_SHORT_CF(_hay[i]) != _SHORT_CF(_needle[i]))
            return 0;
    return 1;
}

static mmint_t __MemeImpl_shortForwardSearch(
    const uint8_t* _haystack, mmint_t _haystack_len,
    const uint8_t* _needle, mmint_t _needle_len,
    mmflag_case_sensit_t _cs)
{
    /* Highest index at which the needle can still fit (inclusive). */
    const mmint_t last_start = _haystack_len - _needle_len;
    const int     fold    = ((_cs & MemeFlag_CaseSensitive) == 0);
    const uint8_t first   = fold ? _SHORT_CF(_needle[0]) : _needle[0];
    const uint8_t last    = fold ? _SHORT_CF(_needle[_needle_len - 1])
                                : _needle[_needle_len - 1];
    const mmint_t mid_len = _needle_len - 2;
    mmint_t       pos = 0;

    /* Only ASCII letters have a case pair.  Folding '@' (0x40) gives '`'
     * (0x60), '[' (0x5B) gives '{' (0x7B), 0x80 gives 0xA0 -- none of those
     * are case variants, so pairing them would produce false matches.
     * first_is_letter gates the two-key scan for exactly this reason. */
    const int     first_is_letter =
        fold && first >= 0x61u && first <= 0x7au;
    const uint8_t alternate =
        first_is_letter ? (uint8_t)(first ^ 0x20u) : first;

    while (pos <= last_start)
    {
        const mmint_t scan_len = last_start - pos + 1;
        mmint_t rel;

        if (first_is_letter)
        {
            /* Both cases of the first byte in one SIMD pass. */
            rel = mmsimd_u8_find2(_haystack + pos, scan_len, first, alternate);
        }
        else
        {
            rel = mmsimd_u8_find(_haystack + pos, scan_len, first);
        }

        if (rel < 0)
            return -1;

        const mmint_t hit = pos + rel;

        /* Last byte next: a single compare rejects most candidates. */
        const uint8_t hay_last = _haystack[hit + _needle_len - 1];
        if ((fold ? _SHORT_CF(hay_last) : hay_last) == last
            && (mid_len == 0
                || __MemeImpl_windowEquals(_haystack + hit + 1,
                                           _needle + 1, mid_len, fold)))
        {
            return hit;
        }

        pos = hit + 1;
    }

    return -1;
}

static mmint_t __MemeImpl_shortReverseSearch(
    const uint8_t* _haystack, mmint_t _haystack_len,
    const uint8_t* _needle, mmint_t _needle_len,
    mmflag_case_sensit_t _cs)
{
    const int     fold    = ((_cs & MemeFlag_CaseSensitive) == 0);
    const uint8_t first   = fold ? _SHORT_CF(_needle[0]) : _needle[0];
    const uint8_t last    = fold ? _SHORT_CF(_needle[_needle_len - 1])
                                : _needle[_needle_len - 1];
    const mmint_t mid_len = _needle_len - 2;
    mmint_t       last_start = _haystack_len - _needle_len;

    /* See __MemeImpl_shortForwardSearch: only ASCII letters have a case
     * pair, so only they may be searched as a two-key set. */
    const int     first_is_letter =
        fold && first >= 0x61u && first <= 0x7au;
    const uint8_t alternate =
        first_is_letter ? (uint8_t)(first ^ 0x20u) : first;

    while (last_start >= 0)
    {
        const mmint_t scan_len = last_start + 1;
        mmint_t rel;

        /* mmsimd_u8_rfind2 scans 32-byte blocks from the end and takes the
         * highest matching bit, so the whole search stays linear.  Calling
         * rfind twice per candidate (the previous approach) rescanned the
         * remaining prefix each round, which is quadratic: 16 KiB of 'a'
         * with needle "ab" took 31 ms. */
        if (first_is_letter)
        {
            rel = mmsimd_u8_rfind2(_haystack, scan_len, first, alternate);
        }
        else
        {
            rel = mmsimd_u8_rfind(_haystack, scan_len, first);
        }

        if (rel < 0)
            return -1;

        const uint8_t hay_last = _haystack[rel + _needle_len - 1];
        if ((fold ? _SHORT_CF(hay_last) : hay_last) == last
            && (mid_len == 0
                || __MemeImpl_windowEquals(_haystack + rel + 1,
                                           _needle + 1, mid_len, fold)))
        {
            return rel;
        }

        /* Next candidate must start strictly before this one. */
        last_start = rel - 1;
    }

    return -1;
}

/* ── sensitivity-dispatching wrappers ────────────────────────────────── */

mmint_t MemeImpl_SearchByBoyerMooreWithSensitivity
(
	const uint8_t* _haystack, mmint_t _haystack_len,
	const uint8_t* _needle, mmint_t _needle_len,
    mmflag_case_sensit_t _cs
)
{
    if ((_haystack == NULL || _needle == NULL))
        return -1;
    if (_needle_len < 0)
        _needle_len = strlen((const char*)_needle);
    if ((_haystack_len == 0 || _needle_len == 0))
        return -1;

    if (_needle_len == 1) {
        return MemeImpl_SearchByViolenceWithSensitivity(
            _haystack, _haystack_len, *_needle, _cs);
    }

    /* Short needle, or too few candidate positions for BM preprocessing to
     * ever pay off: scan with SIMD instead of building the delta tables.
     * Handles both case-sensitive and case-insensitive matching; the latter
     * folds through _bm_casefold_table so it agrees with the BM path. */
    if (__MemeImpl_shouldUseShortPath(_haystack_len, _needle_len))
    {
        if (_haystack_len < 0)
        {
            /* NUL-terminated haystack.  Case-sensitive matching needs no
             * length at all: strstr already handles it and is SIMD-vectorised
             * in this CRT, so measuring the haystack first would add a full
             * pass for nothing.  Only the folding path needs a bounded buffer,
             * because no standard function does case-insensitive search. */
            if ((_cs & MemeFlag_CaseSensitive) != 0)
            {
                const char* hit = strstr((const char*)_haystack,
                                         (const char*)_needle);
                return hit ? (mmint_t)(hit - (const char*)_haystack) : -1;
            }

            const mmint_t hay_len = (mmint_t)strlen((const char*)_haystack);
            if (hay_len < _needle_len)
                return -1;

            return __MemeImpl_shortForwardSearch(
                _haystack, hay_len, _needle, _needle_len, _cs);
        }

        return __MemeImpl_shortForwardSearch(
            _haystack, _haystack_len, _needle, _needle_len, _cs);
    }

    if ((_cs & MemeFlag_CaseSensitive) != 0)
        return MemeImpl_SearchByBoyerMoore(
            _haystack, _haystack_len, _needle, _needle_len);
    else
        return MemeImpl_SearchByBoyerMoore_CaseInsensitive(
            _haystack, _haystack_len, _needle, _needle_len);
}

mmint_t MemeImpl_ReverseSearchByBoyerMooreWithSensitivity(
    const uint8_t* _haystack, mmint_t _haystack_len,
    const uint8_t* _needle, mmint_t _needle_len, mmflag_case_sensit_t _cs)
{
    if ((_haystack == NULL || _needle == NULL))
        return -1;
    if (_needle_len < 0)
        _needle_len = strlen((const char*)_needle);
    if ((_haystack_len == 0 || _needle_len == 0))
        return -1;

    if (_needle_len == 1) {
         return MemeImpl_ReverseSearchByViolenceWithSensitivity(_haystack, _haystack_len, *_needle, _cs);
     }

    /* Same short-needle bypass as the forward entry, scanning backwards and
     * with the same case-folding support.  ISO C has no strrstr, so unlike the
     * forward case a NUL-terminated haystack still has to be measured before
     * the reverse scan can be bounded. */
    if (__MemeImpl_shouldUseShortPath(_haystack_len, _needle_len))
    {
        const mmint_t hay_len = (_haystack_len < 0)
            ? (mmint_t)strlen((const char*)_haystack)
            : _haystack_len;

        if (hay_len < _needle_len)
            return -1;

        return __MemeImpl_shortReverseSearch(
            _haystack, hay_len, _needle, _needle_len, _cs);
    }

     if ((_cs & MemeFlag_CaseSensitive) != 0)
         return MemeImpl_ReverseSearchByBoyerMoore(_haystack, _haystack_len, _needle, _needle_len);
     else
         return MemeImpl_ReverseSearchByBoyerMoore_CaseInsensitive(_haystack, _haystack_len, _needle, _needle_len);
}

mmint_t MemeImpl_SearchByViolenceWithSensitivity(
    const uint8_t* _source, mmint_t _source_len, uint8_t _key,
    mmflag_case_sensit_t _cs)
{
    if (_source_len < 0) {
        /* NUL-terminated path.  Use the CRT strchr instead of a byte loop:
         * the MSVC strchr is SIMD-vectorised, whereas strpbrk is a scalar
         * loop.  Measured on this host over a 64 KiB NUL-terminated buffer
         * with an absent key: strchr 2.4 us, strpbrk 38.7 us, byte loop
         * 183.6 us.  Two strchr calls therefore beat one strpbrk by ~8x.
         *
         * _key == 0 is rejected first because strchr(s, 0) returns a pointer
         * to the terminator rather than NULL, which would turn "not found"
         * into "found at end of string" and change the previous behaviour. */
        const char* p_lo;
        const char* p_hi;

        if (_key == 0)
            return -1;

        if ((_cs & MemeFlag_CaseSensitive) == 0)
        {
            /* ASCII-only case folding, matching _bm_casefold_table
             * (src/meme_impl__algorithm.c:319).  Bit arithmetic instead of
             * tolower/toupper so the result does not depend on the CRT locale
             * (tr_TR maps 'I' to U+0131). */
            const unsigned folded = (unsigned)_key | 0x20u;

            if (folded >= 0x61u && folded <= 0x7au) {
                p_lo = strchr((const char*)_source, (char)folded);
                p_hi = strchr((const char*)_source, (char)(folded ^ 0x20u));

                if (p_lo == NULL)
                    return p_hi ? (mmint_t)(p_hi - (const char*)_source) : -1;
                if (p_hi == NULL)
                    return (mmint_t)(p_lo - (const char*)_source);
                return (mmint_t)((p_lo < p_hi ? p_lo : p_hi) - (const char*)_source);
            }

            /* Non-ASCII letters -- and every non-letter byte such as digits
             * and punctuation -- only match the raw byte. */
            p_lo = strchr((const char*)_source, (char)_key);
            return p_lo ? (mmint_t)(p_lo - (const char*)_source) : -1;
        }

        p_lo = strchr((const char*)_source, (char)_key);
        return p_lo ? (mmint_t)(p_lo - (const char*)_source) : -1;
    }
    else {
        if ((_cs & MemeFlag_CaseSensitive) == 0)
        {
            /* ASCII-only case folding, matching _bm_casefold_table
             * (src/meme_impl__algorithm.c:319).  Bit arithmetic instead of
             * tolower/toupper so the result does not depend on the CRT locale
             * (tr_TR maps 'I' to U+0131, which would fold to a non-adjacent
             * byte pair here). */
            const unsigned folded = (unsigned)_key | 0x20u;

            if (folded >= 0x61u && folded <= 0x7au) {
                return mmsimd_u8_find2(
                    _source, _source_len,
                    (uint8_t)folded, (uint8_t)(folded ^ 0x20u));
            }

            /* Non-ASCII letters -- and every non-letter byte such as digits
             * and punctuation -- only match the raw byte. */
            return mmsimd_u8_find(_source, _source_len, _key);
        }
        else {
            return mmsimd_u8_find(_source, _source_len, _key);
        }
    }
    return -1;
}

mmint_t MemeImpl_ReverseSearchByViolenceWithSensitivity(
    const uint8_t* _source, mmint_t _source_len, uint8_t _key, mmflag_case_sensit_t _cs)
{
    if (_source_len < 0) {
        /* NUL-terminated reverse search.  strrchr needs no length, so this
         * avoids the strlen pass the old code performed before its scalar
         * tolower loop.  MSVC strrchr is SIMD-vectorised; the loop was not.
         *
         * Reverse search takes the LARGEST matching offset (unlike the forward
         * counterpart, which takes the smallest).
         *
         * _key == 0 is rejected first because strrchr(s, 0) returns a pointer
         * to the terminator rather than NULL, which would report "found at end"
         * instead of "not found". */
        const char* p_lo;
        const char* p_hi;

        if (_key == 0)
            return -1;

        if ((_cs & MemeFlag_CaseSensitive) == 0)
        {
            /* ASCII-only case folding, matching _bm_casefold_table
             * (src/meme_impl__algorithm.c:319).  Bit arithmetic instead of
             * tolower/toupper so the result does not depend on the CRT locale
             * (tr_TR maps 'I' to U+0131). */
            const unsigned folded = (unsigned)_key | 0x20u;

            if (folded >= 0x61u && folded <= 0x7au) {
                p_lo = strrchr((const char*)_source, (char)folded);
                p_hi = strrchr((const char*)_source, (char)(folded ^ 0x20u));

                if (p_lo == NULL)
                    return p_hi ? (mmint_t)(p_hi - (const char*)_source) : -1;
                if (p_hi == NULL)
                    return (mmint_t)(p_lo - (const char*)_source);
                return (mmint_t)((p_lo > p_hi ? p_lo : p_hi) - (const char*)_source);
            }

            /* Non-ASCII letters -- and every non-letter byte such as digits
             * and punctuation -- only match the raw byte. */
            p_lo = strrchr((const char*)_source, (char)_key);
            return p_lo ? (mmint_t)(p_lo - (const char*)_source) : -1;
        }

        p_lo = strrchr((const char*)_source, (char)_key);
        return p_lo ? (mmint_t)(p_lo - (const char*)_source) : -1;
    }

    /* Fixed-length buffer.  strrchr cannot be used here because the buffer
     * may have no terminator; the bounded SIMD primitive is the right tool.
     * */
    if ((_cs & MemeFlag_CaseSensitive) == 0)
    {
        /* ASCII-only case folding, matching _bm_casefold_table.
         * Bit arithmetic instead of tolower so the result does not depend on
         * the CRT locale (tr_TR maps 'I' to U+0131). */
        const unsigned folded = (unsigned)_key | 0x20u;

        if (folded >= 0x61u && folded <= 0x7au) {
            /* Both cases of the key in one reverse SIMD pass. */
            return mmsimd_u8_rfind2(
                _source, _source_len,
                (uint8_t)folded, (uint8_t)(folded ^ 0x20u));
        }

        /* Non-ASCII letters -- and every non-letter byte such as digits
         * and punctuation -- only match the raw byte. */
        return mmsimd_u8_rfind(_source, _source_len, _key);
    }

    return mmsimd_u8_rfind(_source, _source_len, _key);
}
