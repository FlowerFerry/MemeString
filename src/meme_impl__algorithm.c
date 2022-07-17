
#include "meme/impl/algorithm.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static void __MemeImpl_makeDelta1(
    MemeInteger_t* _delta1, MemeInteger_t _delta1_len,
    const uint8_t* _pat, MemeInteger_t _pat_len) 
{
    // TO_DO : It can also be optimized
    for (MemeInteger_t index = 0; index < _delta1_len; ++index) 
    {
        _delta1[index] = _pat_len;
    }

    for (MemeInteger_t index = 0; index < _pat_len - 1; ++index)
    {
        _delta1[_pat[index]] = _pat_len - 1 - index;
    }
}

static MemeInteger_t __MemeImpl_isPrefix(const uint8_t* _word, MemeInteger_t _word_len, MemeInteger_t _pos)
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

static MemeInteger_t __MemeImpl_suffixLength(
    const uint8_t* _word, MemeInteger_t _word_len, MemeInteger_t _pos)
{
    MemeInteger_t index;
    for (index = 0; (_word[_pos - index] == _word[_word_len - 1 - index]) && (index < _pos); ++index);
    return index;
}

static void __MemeImpl_makeDelta2(MemeInteger_t* _delta2, const uint8_t* _pat, MemeInteger_t _pat_len)
{
    MemeInteger_t p;
    MemeInteger_t last_prefix_index = 1;

    for (p = _pat_len - 1; p >= 0; --p) 
    {
        if (__MemeImpl_isPrefix(_pat, _pat_len, p + 1)) 
        {
            last_prefix_index = p + 1;
        }
        _delta2[p] = (_pat_len - 1 - p) + last_prefix_index;
    }

    for (p = 0; p < _pat_len - 1; ++p) 
    {
        MemeInteger_t slen = __MemeImpl_suffixLength(_pat, _pat_len, p);
        if (_pat[p - slen] != _pat[_pat_len - 1 - slen]) 
        {
            _delta2[_pat_len - 1 - slen] = _pat_len - 1 - p + slen;
        }
    }
}


MemeInteger_t MemeImpl_SearchByBoyerMoore(
	const uint8_t* _haystack, MemeInteger_t _haystack_len, 
	const uint8_t* _needle, MemeInteger_t _needle_len)
{
    MemeInteger_t delta1[256];
    MemeInteger_t* delta2;
    MemeInteger_t n_shifts;

    assert(_haystack != NULL    && MemeImpl_SearchByBoyerMoore);
    assert(_haystack_len != 0   && MemeImpl_SearchByBoyerMoore);
    assert(_needle != NULL      && MemeImpl_SearchByBoyerMoore);
    assert(_needle_len != 0     && MemeImpl_SearchByBoyerMoore);

    if (_haystack_len != -1 && _haystack_len < _needle_len)
        return -1;

    delta2 = malloc(_needle_len * sizeof(MemeInteger_t));
    n_shifts = 0;
    __MemeImpl_makeDelta1(delta1, sizeof(delta1) / sizeof(delta1[0]), _needle, _needle_len);
    __MemeImpl_makeDelta2(delta2, _needle, _needle_len);

    --_needle_len;
    if (_haystack_len == -1) {

        const uint8_t* index = _haystack + _needle_len;
        while (1) {
            MemeInteger_t needle_index = _needle_len;
            MemeInteger_t move_count;
            while (needle_index >= 0 && (*index == _needle[needle_index]))
            {
                --index;
                --needle_index;
            }
            if (needle_index < 0) {
                free(delta2);
                return index + 1 - _haystack;
            }

            move_count = MemeMath_Max(delta1[*index], delta2[needle_index]);
            while (move_count --> 0)
            {
                if (*(++index) == '\0')
                {
                    free(delta2);
                    return -1;
                }
            }
        }
    }
    else {
        MemeInteger_t index = _needle_len;
        while (index < _haystack_len) {
            MemeInteger_t needle_index = _needle_len;
            while (needle_index >= 0 && (_haystack[index] == _needle[needle_index]))
            {
                --index;
                --needle_index;
            }
            if (needle_index < 0) {
                free(delta2);
                return index + 1;
            }

            index += MemeMath_Max(delta1[_haystack[index]], delta2[needle_index]);
        }
    }
    free(delta2);
    return -1;
}

MemeInteger_t MemeImpl_SearchByBoyerMooreWithSensitivity
(
	const uint8_t* _haystack, MemeInteger_t _haystack_len, 
	const uint8_t* _needle, MemeInteger_t _needle_len, 
	MemeFlag_CaseSensitivity_t _cs
)
{
    if (_haystack == NULL || _needle == NULL)
        return -1;
    if (_haystack_len == 0 || _needle_len == 0)
        return -1;
    if (_needle_len == -1)
        _needle_len = strlen(_needle);
    
    if (_needle_len == 1) {
        return MemeImpl_SearchByViolenceWithSensitivity(_haystack, _haystack_len, *_needle, _cs);
    }

    if (_cs == MemeFlag_AllSensitive)
        return MemeImpl_SearchByBoyerMoore(_haystack, _haystack_len, _needle, _needle_len);

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
            uint8_t key = tolower(_key);
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
            uint8_t key = tolower(_key);
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
