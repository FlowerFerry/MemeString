
#include "meme/impl/algorithm.h"
#include <meme/impl/string.h>
#include <mego/predef/symbol/likely.h>

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static MemeInteger_t __mm_BoyerMoore_isPrefix(
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

static MemeInteger_t __mm_BoyerMoore_suffixLength(
    const uint8_t* _word, MemeInteger_t _word_len, MemeInteger_t _pos)
{
    MemeInteger_t index;
    for (index = 0; (_word[_pos - index] == _word[_word_len - 1 - index]) && (index < _pos); ++index);
    return index;
}

static void __mm_BoyerMoore_makeBadDelta(
    MemeInteger_t* _delta1, MemeInteger_t _delta1_len,
    const uint8_t* _pat, MemeInteger_t _pat_len) 
{
    // TO_DO : It can also be optimized
    for (MemeInteger_t index = 0; index < _delta1_len; ++index) 
        _delta1[index] = _pat_len;
    
    for (MemeInteger_t index = 0; index < _pat_len - 1; ++index)
        _delta1[_pat[index]] = _pat_len - 1 - index;
    
}

static void __mm_BoyerMoore_makeGoodDelta(MemeInteger_t* _delta2, const uint8_t* _pat, MemeInteger_t _pat_len)
{
    MemeInteger_t p;
    MemeInteger_t last_prefix_index = 1;

    for (p = _pat_len - 1; p >= 0; --p) 
    {
        if (__mm_BoyerMoore_isPrefix(_pat, _pat_len, p + 1))
        {
            last_prefix_index = p + 1;
        }
        _delta2[p] = (_pat_len - 1 - p) + last_prefix_index;
    }

    for (p = 0; p < _pat_len - 1; ++p) 
    {
        MemeInteger_t slen = __mm_BoyerMoore_suffixLength(_pat, _pat_len, p);
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

static inline MemeInteger_t __mm_ReverseBoyerMoore_isPrefix(
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

static MemeInteger_t __mm_ReverseBoyerMoore_suffixLength(
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

static inline void __mm_ReverseBoyerMoore_makeBadDelta(
    MemeInteger_t* _delta1, MemeInteger_t _delta1_len, 
    const MemeByte_t* _pat_rbegin, const MemeByte_t* _pat_rend)
{
    MemeInteger_t diff = mbit_rDistance(_pat_rend, _pat_rbegin);
    for (MemeInteger_t index = 0; index < _delta1_len; ++index)
        _delta1[index] = diff;

    for (const MemeByte_t *it = _pat_rbegin, *last = mbit_rPrev(_pat_rend);
        mbit_rDistance(last, it) > 0; it = mbit_rNext(it))
    {
        _delta1[*it] = mbit_rDistance(last, it);
    }
}

static void __mm_ReverseBoyerMoore_makeGoodDelta(
    MemeInteger_t* _delta2, const MemeByte_t* _pat_rbegin, const MemeByte_t* _pat_rend)
{
    MemeInteger_t p;
    MemeInteger_t last_prefix_index = 1;
    MemeInteger_t pat_len = mbit_rDistance(_pat_rend, _pat_rbegin);

    for (p = pat_len - 1; p >= 0; --p)
    {
        if (__mm_ReverseBoyerMoore_isPrefix(_pat_rbegin, _pat_rend, p + 1))
        {
            last_prefix_index = p + 1;
        }
        _delta2[p] = (pat_len - 1 - p) + last_prefix_index;
    }

    for (p = 0; p < pat_len - 1; ++p)
    {
        MemeInteger_t slen = __mm_ReverseBoyerMoore_suffixLength(_pat_rbegin, _pat_rend, p);
        if (*mbit_rAdvance(_pat_rbegin, p - slen) != *mbit_rAdvance(_pat_rbegin, pat_len - 1 - slen))
            _delta2[pat_len - 1 - slen] = pat_len - 1 - p + slen;

    }
    
}

MemeInteger_t MemeImpl_SearchByBoyerMoore(
	const uint8_t* _haystack, MemeInteger_t _haystack_len, 
	const uint8_t* _needle, MemeInteger_t _needle_len)
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

    __mm_BoyerMoore_makeBadDelta (b_delta, sizeof(b_delta) / sizeof(b_delta[0]), _needle, _needle_len);
    __mm_BoyerMoore_makeGoodDelta(g_delta, _needle, _needle_len);
    
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

MemeInteger_t MemeImpl_ReverseSearchByBoyerMoore(
    const uint8_t* _haystack, MemeInteger_t _haystack_len, 
    const uint8_t* _needle, MemeInteger_t _needle_len)
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
    __mm_ReverseBoyerMoore_makeBadDelta (
        b_delta, sizeof(b_delta) / sizeof(b_delta[0]), p_rbegin, p_rend);
    __mm_ReverseBoyerMoore_makeGoodDelta(g_delta, p_rbegin, p_rend);
    
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

MemeInteger_t MemeImpl_SearchByBoyerMooreWithSensitivity
(
	const uint8_t* _haystack, MemeInteger_t _haystack_len, 
	const uint8_t* _needle, MemeInteger_t _needle_len, 
	MemeFlag_CaseSensitivity_t _cs
)
{
    if ((_haystack == NULL || _needle == NULL))
        return -1;
    if ((_haystack_len == 0 || _needle_len == 0))
        return -1;
    if (_needle_len < 0)
        _needle_len = strlen((const char*)_needle);
    
    if (_needle_len == 1) {
        return MemeImpl_SearchByViolenceWithSensitivity(_haystack, _haystack_len, *_needle, _cs);
    }

    if ((enum _MemeFlag_CaseSensitivity_t)_cs == MemeFlag_AllSensitive)
        return MemeImpl_SearchByBoyerMoore(_haystack, _haystack_len, _needle, _needle_len);

	return -1;
}

MemeInteger_t MemeImpl_ReverseSearchByBoyerMooreWithSensitivity(
    const uint8_t* _haystack, MemeInteger_t _haystack_len, 
    const uint8_t* _needle, MemeInteger_t _needle_len, MemeFlag_CaseSensitivity_t _cs)
{
    if ((_haystack == NULL || _needle == NULL))
        return -1;
    if ((_haystack_len == 0 || _needle_len == 0))
        return -1;
    if (_needle_len < 0)
        _needle_len = strlen((const char*)_needle);

    if (_needle_len == 1) {
        return MemeImpl_ReverseSearchByViolenceWithSensitivity(_haystack, _haystack_len, *_needle, _cs);
    }

    if ((enum _MemeFlag_CaseSensitivity_t)_cs == MemeFlag_AllSensitive)
        return MemeImpl_ReverseSearchByBoyerMoore(_haystack, _haystack_len, _needle, _needle_len);

    return -1;
}

MemeInteger_t MemeImpl_SearchByViolenceWithSensitivity(
    const uint8_t* _source, MemeInteger_t _source_len, uint8_t _key, 
    MemeFlag_CaseSensitivity_t _cs)
{
    if (_source_len == -1) {
        const uint8_t* index = _source;
        if ((_cs & MemeFlag_CaseSensitive) == 0)
        {
            uint8_t key = (uint8_t)tolower(_key);
            for (; *index == '\0'; ++index)
                if (tolower(*index) == key)
                    return index - _source;
        }
        else {
            for (; *index == '\0'; ++index)
                if (*index == _key)
                    return index - _source;
        }
    }
    else {
        if ((_cs & MemeFlag_CaseSensitive) == 0)
        {
            uint8_t key = (uint8_t)tolower(_key);
            for (MemeInteger_t index = 0; index < _source_len; ++index)
                if (tolower(_source[index]) == key)
                    return index;
        }
        else {
            for (MemeInteger_t index = 0; index < _source_len; ++index)
                if (_source[index] == _key)
                    return index;
        }
    }
    return -1;
}

MemeInteger_t MemeImpl_ReverseSearchByViolenceWithSensitivity(
    const uint8_t* _source, MemeInteger_t _source_len, uint8_t _key, MemeFlag_CaseSensitivity_t _cs)
{
    if ((_source_len < 0))
        _source_len = strlen((const char*)_source);

    if ((_cs & MemeFlag_CaseSensitive) == 0)
    {
        uint8_t key = (uint8_t)tolower(_key);
        for (MemeInteger_t index = _source_len - 1; index >= 0; --index)
            if (tolower(_source[index]) == key)
                return index;
    }
    else {
        for (MemeInteger_t index = _source_len - 1; index >= 0; --index)
            if (_source[index] == _key)
                return index;
    }

    return -1;
}