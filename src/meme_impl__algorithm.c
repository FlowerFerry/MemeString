
#include "meme/impl/algorithm.h"
#include <meme/impl/string.h>
#include <meme/simd/simd.h>
#include <mego/predef/charset.h>
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

void MemeBoyerMoore_makeBadDelta(
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

/* Linear-time suffix table used by the good-suffix construction.
 *
 * suff[i] is the length of the longest suffix of pat[0..i] that is also a
 * suffix of the whole pattern.  Built by the classic Z-algorithm-style scan
 * (Gusfield, "Algorithms on Strings, Trees, and Sequences", section 2.3.2),
 * O(m) time and O(m) extra space. */
/* Allocates _count MemeInteger_t, rejecting sizes whose byte count would wrap
 * size_t.  Without this check a needle length near SIZE_MAX / sizeof would
 * truncate the product to a small number, malloc would succeed, and the
 * caller would then write far past the block.
 * Returns NULL when the request is impossible or the allocation fails. */
static MemeInteger_t* __MemeBoyerMoore_allocTables(MemeInteger_t _count)
{
    if (_count <= 0)
        return NULL;
    if ((size_t)_count > SIZE_MAX / sizeof(MemeInteger_t))
        return NULL;
    return (MemeInteger_t*)malloc((size_t)_count * sizeof(MemeInteger_t));
}

static void __MemeBoyerMoore_computeSuffixes(
    const uint8_t* _pat, MemeInteger_t _pat_len, MemeInteger_t* _suff)
{
    MemeInteger_t f = 0;
    MemeInteger_t g = _pat_len - 1;
    MemeInteger_t i;

    _suff[_pat_len - 1] = _pat_len;

    for (i = _pat_len - 2; i >= 0; --i)
    {
        if (i > g && _suff[i + _pat_len - 1 - f] < i - g)
        {
            _suff[i] = _suff[i + _pat_len - 1 - f];
        }
        else
        {
            if (i < g)
                g = i;
            f = i;
            while (g >= 0 && _pat[g] == _pat[g + _pat_len - 1 - f])
                --g;
            _suff[i] = f - g;
        }
    }
}

void MemeBoyerMoore_makeGoodDelta(MemeInteger_t* _delta2, const uint8_t* _pat, MemeInteger_t _pat_len)
{
    MemeInteger_t  small_suff[MEME_IMPL_LOCAL_NEEDLE_MAX];
    MemeInteger_t  small_bmgs[MEME_IMPL_LOCAL_NEEDLE_MAX];
    MemeInteger_t* suff = small_suff;
    MemeInteger_t* bmgs = small_bmgs;
    MemeInteger_t  i, j;

    if (_pat_len > MEME_IMPL_LOCAL_NEEDLE_MAX)
    {
        suff = __MemeBoyerMoore_allocTables(_pat_len);
        bmgs = __MemeBoyerMoore_allocTables(_pat_len);
        if (suff == NULL || bmgs == NULL)
        {
            /* Allocation failure: fall back to the always-correct naive
             * construction so callers never receive an uninitialised table. */
            free(suff);
            free(bmgs);
            {
                MemeInteger_t p;
                MemeInteger_t last_prefix_index = 1;
                for (p = _pat_len - 1; p >= 0; --p)
                {
                    if (__MemeBoyerMoore_isPrefix(_pat, _pat_len, p + 1))
                        last_prefix_index = p + 1;
                    _delta2[p] = (_pat_len - 1 - p) + last_prefix_index;
                }
                for (p = 0; p < _pat_len - 1; ++p)
                {
                    MemeInteger_t slen = __MemeBoyerMoore_suffixLength(_pat, _pat_len, p);
                    if (slen > p)
                        slen = p;
                    if (_pat[p - slen] != _pat[_pat_len - 1 - slen])
                        _delta2[_pat_len - 1 - slen] = _pat_len - 1 - p + slen;
                }
            }
            return;
        }
    }

    __MemeBoyerMoore_computeSuffixes(_pat, _pat_len, suff);

    /* bmgs[j] is the standard "shift the window by this much when the
     * mismatch is at pattern index j" table. */
    for (i = 0; i < _pat_len; ++i)
        bmgs[i] = _pat_len;

    /* Case 1: a proper suffix of pat[0..i] also occurs earlier in the pattern. */
    j = 0;
    for (i = _pat_len - 1; i >= 0; --i)
        if (suff[i] == i + 1)
            for (; j < _pat_len - 1 - i; ++j)
                if (bmgs[j] == _pat_len)
                    bmgs[j] = _pat_len - 1 - i;

    /* Case 2: the longest suffix of the whole pattern that is also a prefix. */
    for (i = 0; i <= _pat_len - 2; ++i)
        bmgs[_pat_len - 1 - suff[i]] = _pat_len - 1 - i;

    /* The search loops in this file add the shift to the MISMATCHED byte's
     * index, not to the pattern's right end, so convert the window shift into
     * a distance from the mismatch position.  Without this offset the tables
     * would be shifted by (m - 1 - j) and matches would be missed. */
    for (j = 0; j < _pat_len; ++j)
        _delta2[j] = bmgs[j] + (_pat_len - 1 - j);

    if (suff != small_suff) free(suff);
    if (bmgs != small_bmgs) free(bmgs);
}

mmint_t MemeImpl_SearchByBoyerMoore(
	const uint8_t* _haystack, mmint_t _haystack_len,
	const uint8_t* _needle, mmint_t _needle_len)
{
    MemeInteger_t  b_delta[256];
    MemeInteger_t  good_local[MEME_IMPL_LOCAL_NEEDLE_MAX];
    MemeInteger_t* g_delta = good_local;

    assert(_haystack != NULL    && MemeImpl_SearchByBoyerMoore);
    assert(_haystack_len != 0   && MemeImpl_SearchByBoyerMoore);
    assert(_needle != NULL      && MemeImpl_SearchByBoyerMoore);
    assert(_needle_len != 0     && MemeImpl_SearchByBoyerMoore);

    if (_haystack_len >= 0 && _haystack_len < _needle_len)
        return -1;

    if (_needle_len > MEME_IMPL_LOCAL_NEEDLE_MAX)
    {
        g_delta = __MemeBoyerMoore_allocTables(_needle_len);
        if (g_delta == NULL)
            return -1;
    }

    MemeBoyerMoore_makeBadDelta (b_delta, sizeof(b_delta) / sizeof(b_delta[0]), _needle, _needle_len);
    MemeBoyerMoore_makeGoodDelta(g_delta, _needle, _needle_len);

#define MEME_IMPL_BM_RELEASE()                                  \
    do { if (g_delta != good_local) free(g_delta); } while (0)

    
    if ((_haystack_len < 0)) 
    {
        const MemeByte_t* index = _haystack;
        for (MemeInteger_t count = 0; count < _needle_len; ++count)
            if (*index++ == '\0') {
                MEME_IMPL_BM_RELEASE();
                return -1;
            }
        --_needle_len;
        --index;
        
        while (1) {
            MemeInteger_t needle_index = _needle_len;
            MemeInteger_t move_count;
            while (needle_index >= 0 && (*index == _needle[needle_index]))
            {
                /* If we matched the very first byte of the needle at the
                 * very start of the haystack, return 0 immediately.
                 * Avoids underflowing 'index' past _haystack. */
                if (index == _haystack && needle_index == 0) {
                    MEME_IMPL_BM_RELEASE();
                    return 0;
                }
                --index;
                --needle_index;
            }
            if (needle_index < 0) {
                MEME_IMPL_BM_RELEASE();
                return index + 1 - _haystack;
            }

            move_count = MemeMath_Max(b_delta[*index], g_delta[needle_index]);
            /* Safe overflow check: use subtraction to avoid UB. */
            while (move_count --> 0)
            {
                if (*(++index) == '\0')
                {
                    MEME_IMPL_BM_RELEASE();
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
                /* Match at the start of needle: index is the correct result.
                 * Avoid index going below 0 (which would wrap for unsigned
                 * and is undefined for signed arithmetic on decrement). */
                if (needle_index == 0) {
                    MEME_IMPL_BM_RELEASE();
                    return index;
                }
                --index;
                --needle_index;
            }
            if (needle_index < 0) {
                MEME_IMPL_BM_RELEASE();
                return index + 1;
            }

            {
                MemeInteger_t move = MemeMath_Max(b_delta[_haystack[index]], g_delta[needle_index]);
                /* Safe bound check: if move would go past end, stop.
                 * Uses subtraction to avoid signed overflow UB. */
                if (move >= _haystack_len - index)
                    break;
                index += move;
            }
        }
    }
    MEME_IMPL_BM_RELEASE();
    return -1;
}

#undef MEME_IMPL_BM_RELEASE

mmint_t MemeImpl_ReverseSearchByBoyerMoore(
    const uint8_t* _haystack, mmint_t _haystack_len,
    const uint8_t* _needle, mmint_t _needle_len)
{
    MemeInteger_t  b_delta[256];
    MemeInteger_t  good_local[MEME_IMPL_LOCAL_NEEDLE_MAX];
    uint8_t        rev_local[MEME_IMPL_LOCAL_NEEDLE_MAX];
    MemeInteger_t* g_delta = good_local;
    uint8_t*       rev     = rev_local;

    assert(_haystack != NULL  && MemeImpl_ReverseSearchByBoyerMoore);
    assert(_haystack_len != 0 && MemeImpl_ReverseSearchByBoyerMoore);
    assert(_needle != NULL    && MemeImpl_ReverseSearchByBoyerMoore);
    assert(_needle_len > 0    && MemeImpl_ReverseSearchByBoyerMoore);

    /* Negative haystack length means "NUL-terminated", so the assertion above
     * must allow it; strlen() below turns it into a positive length. */
    if (_haystack_len < 0)
        _haystack_len = (mmint_t)strlen((const char*)_haystack);

    if (_haystack_len < _needle_len)
        return -1;

    const MemeInteger_t m = _needle_len;

    if (m > MEME_IMPL_LOCAL_NEEDLE_MAX)
    {
        g_delta = __MemeBoyerMoore_allocTables(m);
        rev     = (uint8_t*)malloc((size_t)m);
        if (g_delta == NULL || rev == NULL)
        {
            free(g_delta);
            free(rev);
            return -1;
        }
    }

    /* Reverse search is expressed as a FORWARD search for the reversed
     * needle inside the reversed haystack.  Building the tables from the
     * reversed needle keeps a single, already-tested preprocessing path and
     * removes the index/table mismatch the old hand-rolled reverse tables
     * had.  Reversed coordinate H maps to original index
     * (_haystack_len - 1 - H). */
    for (MemeInteger_t i = 0; i < m; ++i)
        rev[i] = _needle[m - 1 - i];

    MemeBoyerMoore_makeBadDelta (b_delta, 256, rev, m);
    MemeBoyerMoore_makeGoodDelta(g_delta, rev, m);

    MemeInteger_t index = m - 1;
    while (index < _haystack_len)
    {
        MemeInteger_t needle_index = m - 1;
        while (needle_index >= 0
            && _haystack[_haystack_len - 1 - index] == rev[needle_index])
        {
            if (needle_index == 0)
            {
                const mmint_t hit = (mmint_t)(_haystack_len - index - m);
                if (g_delta != good_local) free(g_delta);
                if (rev     != rev_local)  free(rev);
                return hit;
            }
            --index;
            --needle_index;
        }

        {
            const MemeInteger_t move = MemeMath_Max(
                b_delta[_haystack[_haystack_len - 1 - index]],
                g_delta[needle_index]);
            if (move >= _haystack_len - index)
                break;
            index += move;
        }
    }

    if (g_delta != good_local) free(g_delta);
    if (rev     != rev_local)  free(rev);
    return -1;
}

/* ── case-insensitive Boyer-Moore helpers ─────────────────────────────── */

/*
 * ASCII-only case-folding table, kept as a function-local static so the
 * initialised array lives in the read-only data section but is only visible
 * through the accessor below.  __MemeImpl_foldCase() is the only reader, so the table never
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

#define __MemeImpl_foldCase(c) __MemeImpl_casefoldTable()[(uint8_t)(c)]

/* ── case-insensitive  forward  Boyer-Moore helpers ──────────────────── */

static MemeInteger_t __MemeBoyerMooreCI_isPrefix(
    const uint8_t* _word, MemeInteger_t _word_len, MemeInteger_t _pos)
{
    MemeInteger_t index;
    MemeInteger_t suffixlen = _word_len - _pos;

    for (index = 0; index < suffixlen; ++index)
    {
        if (__MemeImpl_foldCase(_word[index]) != __MemeImpl_foldCase(_word[_pos + index]))
            return 0;
    }
    return 1;
}

static MemeInteger_t __MemeBoyerMooreCI_suffixLength(
    const uint8_t* _word, MemeInteger_t _word_len, MemeInteger_t _pos)
{
    MemeInteger_t index;
    for (index = 0;
        (__MemeImpl_foldCase(_word[_pos - index]) == __MemeImpl_foldCase(_word[_word_len - 1 - index]))
        && (index < _pos); ++index);
    return index;
}

static void __MemeBoyerMooreCI_makeBadDelta(
    MemeInteger_t* _delta1, MemeInteger_t _delta1_len,
    const uint8_t* _pat, MemeInteger_t _pat_len)
{
    mmsimd_iptr_fill(_delta1, _delta1_len, _pat_len);

    for (MemeInteger_t index = 0; index < _pat_len - 1; ++index)
        _delta1[__MemeImpl_foldCase(_pat[index])] = _pat_len - 1 - index;
}

void MemeBoyerMooreCI_makeGoodDelta(
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
        if (__MemeImpl_foldCase(_pat[p - slen]) != __MemeImpl_foldCase(_pat[_pat_len - 1 - slen]))
            _delta2[_pat_len - 1 - slen] = _pat_len - 1 - p + slen;
    }
}

/* ── case-insensitive  reverse  Boyer-Moore helpers ──────────────────── */

mmint_t MemeImpl_SearchByBoyerMoore_CaseInsensitive(
    const uint8_t* _haystack, mmint_t _haystack_len,
    const uint8_t* _needle, mmint_t _needle_len)
{
    MemeInteger_t  b_delta[256];
    MemeInteger_t  good_local[MEME_IMPL_LOCAL_NEEDLE_MAX];
    uint8_t        folded_local[MEME_IMPL_LOCAL_NEEDLE_MAX];
    MemeInteger_t* g_delta       = good_local;
    uint8_t*       needle_folded = folded_local;

    assert(_haystack != NULL    && MemeImpl_SearchByBoyerMoore_CaseInsensitive);
    assert(_haystack_len != 0   && MemeImpl_SearchByBoyerMoore_CaseInsensitive);
    assert(_needle != NULL      && MemeImpl_SearchByBoyerMoore_CaseInsensitive);
    assert(_needle_len != 0     && MemeImpl_SearchByBoyerMoore_CaseInsensitive);

    if (_haystack_len >= 0 && _haystack_len < _needle_len)
        return -1;

    if (_needle_len > MEME_IMPL_LOCAL_NEEDLE_MAX)
    {
        g_delta       = __MemeBoyerMoore_allocTables(_needle_len);
        needle_folded = (uint8_t*)malloc((size_t)_needle_len);
        if (g_delta == NULL || needle_folded == NULL) {
            free(g_delta);
            free(needle_folded);
            return -1;
        }
    }

#define MEME_IMPL_BMCI_RELEASE()                                        \
    do {                                                                \
        if (g_delta != good_local)               free(g_delta);          \
        if (needle_folded != folded_local)       free(needle_folded);    \
    } while (0)

    /* Pre-fold the needle so the hot loop never calls __MemeImpl_foldCase per needle byte. */
    for (MemeInteger_t i = 0; i < _needle_len; ++i)
        needle_folded[i] = __MemeImpl_foldCase(_needle[i]);

    /* Use the already-folded needle for table construction.  The ASCII
     * case-folding is idempotent (fold(fold(c)) == fold(c)), so the
     * resulting tables are identical to passing the original needle to
     * the CI-specific variants, but we save _needle_len fold calls here
     * and many more inside the O(m^2) good-delta construction. */
    MemeBoyerMoore_makeBadDelta(b_delta,
        sizeof(b_delta) / sizeof(b_delta[0]), needle_folded, _needle_len);
    MemeBoyerMoore_makeGoodDelta(g_delta, needle_folded, _needle_len);

    if ((_haystack_len < 0))
    {
        const MemeByte_t* index = _haystack;
        for (MemeInteger_t count = 0; count < _needle_len; ++count)
            if (*index++ == '\0') {
                MEME_IMPL_BMCI_RELEASE();
                return -1;
            }
        --_needle_len;
        --index;

        while (1) {
            MemeInteger_t needle_index = _needle_len;
            MemeInteger_t move_count;
            while (needle_index >= 0
                && __MemeImpl_foldCase(*index) == needle_folded[needle_index])
            {
                /* Match at the very start: return 0 before stepping index
                 * below _haystack (that pointer is not valid to form). */
                if (index == _haystack && needle_index == 0) {
                    MEME_IMPL_BMCI_RELEASE();
                    return 0;
                }
                --index;
                --needle_index;
            }
            if (needle_index < 0) {
                MEME_IMPL_BMCI_RELEASE();
                return index + 1 - _haystack;
            }

            move_count = MemeMath_Max(
                b_delta[__MemeImpl_foldCase(*index)], g_delta[needle_index]);
            while (move_count --> 0)
            {
                if (*(++index) == '\0')
                {
                    MEME_IMPL_BMCI_RELEASE();
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
                && __MemeImpl_foldCase(_haystack[index]) == needle_folded[needle_index])
            {
                --index;
                --needle_index;
            }
            if (needle_index < 0) {
                MEME_IMPL_BMCI_RELEASE();
                return index + 1;
            }

            {
                const MemeInteger_t move = MemeMath_Max(
                    b_delta[__MemeImpl_foldCase(_haystack[index])],
                    g_delta[needle_index]);
                /* Safe bound check using subtraction so a shift near the
                 * signed limit cannot overflow. */
                if (move >= _haystack_len - index)
                    break;
                index += move;
            }
        }
    }
    MEME_IMPL_BMCI_RELEASE();
    return -1;
}

#undef MEME_IMPL_BMCI_RELEASE

/* ── case-insensitive Boyer-Moore  (reverse) ─────────────────────────── */

mmint_t MemeImpl_ReverseSearchByBoyerMoore_CaseInsensitive(
    const uint8_t* _haystack, mmint_t _haystack_len,
    const uint8_t* _needle, mmint_t _needle_len)
{
    MemeInteger_t  b_delta[256];
    MemeInteger_t  good_local[MEME_IMPL_LOCAL_NEEDLE_MAX];
    uint8_t        rev_folded_local[MEME_IMPL_LOCAL_NEEDLE_MAX];
    MemeInteger_t* g_delta    = good_local;
    uint8_t*       rev_folded = rev_folded_local;

    assert(_haystack != NULL
        && MemeImpl_ReverseSearchByBoyerMoore_CaseInsensitive);
    assert(_haystack_len != 0
        && MemeImpl_ReverseSearchByBoyerMoore_CaseInsensitive);
    assert(_needle != NULL
        && MemeImpl_ReverseSearchByBoyerMoore_CaseInsensitive);
    assert(_needle_len != 0
        && MemeImpl_ReverseSearchByBoyerMoore_CaseInsensitive);

    if ((_haystack_len < 0))
        _haystack_len = (mmint_t)strlen((const char*)_haystack);

    if ((_haystack_len < _needle_len))
        return -1;

    const MemeInteger_t m = _needle_len;

    if (m > MEME_IMPL_LOCAL_NEEDLE_MAX)
    {
        g_delta    = __MemeBoyerMoore_allocTables(m);
        rev_folded = (uint8_t*)malloc((size_t)m);
        if (g_delta == NULL || rev_folded == NULL) {
            free(g_delta);
            free(rev_folded);
            return -1;
        }
    }

#define MEME_IMPL_RBMCI_RELEASE()                                       \
    do {                                                                \
        if (g_delta    != good_local)        free(g_delta);             \
        if (rev_folded != rev_folded_local)  free(rev_folded);          \
    } while (0)

    /* Same trick as the case-sensitive reverse path: build the tables from
     * the reversed, already-folded needle and run a forward comparison in
     * reversed coordinates.  Reversed coordinate H maps to the original
     * index (_haystack_len - 1 - H).  No pointer below either base is ever
     * formed, and the CI-specific reverse table builders are no longer
     * needed. */
    for (MemeInteger_t i = 0; i < m; ++i)
        rev_folded[i] = __MemeImpl_foldCase(_needle[m - 1 - i]);

    MemeBoyerMoore_makeBadDelta (b_delta, 256, rev_folded, m);
    MemeBoyerMoore_makeGoodDelta(g_delta, rev_folded, m);

    MemeInteger_t index = m - 1;
    while (index < _haystack_len)
    {
        MemeInteger_t needle_index = m - 1;
        while (needle_index >= 0
            && __MemeImpl_foldCase(_haystack[_haystack_len - 1 - index])
                == rev_folded[needle_index])
        {
            if (needle_index == 0)
            {
                const mmint_t hit = (mmint_t)(_haystack_len - index - m);
                MEME_IMPL_RBMCI_RELEASE();
                return hit;
            }
            --index;
            --needle_index;
        }

        {
            const MemeInteger_t move = MemeMath_Max(
                b_delta[__MemeImpl_foldCase(_haystack[_haystack_len - 1 - index])],
                g_delta[needle_index]);
            if (move >= _haystack_len - index)
                break;
            index += move;
        }
    }

    MEME_IMPL_RBMCI_RELEASE();
    return -1;
}

#undef MEME_IMPL_RBMCI_RELEASE

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

static int __MemeImpl_shouldUseShortPath(mmint_t _haystack_len, mmint_t _needle_len)
{
    if (_needle_len <= MEME_IMPL_SHORT_NEEDLE_MAX)
        return 1;

    /* Unknown haystack length: the candidate count cannot be evaluated yet,
     * so only the needle-length rule applies. */
    if (_haystack_len < 0)
        return 0;

    /* A haystack shorter than the needle can never match; the short helpers
     * return -1 immediately for this case too. */
    if (_haystack_len < _needle_len)
        return 1;

    /* candidate_count = _haystack_len - _needle_len + 1
     *
     * Keep FastScan while candidate_count <= MEME_IMPL_FEW_CANDIDATES:
     *   _haystack_len - _needle_len + 1 <= FEW
     *   _haystack_len - _needle_len     <  FEW
     * Computing it as a strict '<' avoids the +1 (and the overflow it could
     * cause at the extremes). */
    return (_haystack_len - _needle_len) < (mmint_t)MEME_IMPL_FEW_CANDIDATES;
}

/* Folds one byte for case-insensitive comparison, using the same ASCII-only
 * table as the Boyer-Moore path (_bm_casefold_table, defined above) so both
 * paths agree on every input, including 0x80..0xFF which fold to themselves. */
/* fold_case is defined above */

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
        if (__MemeImpl_foldCase(_hay[i]) != __MemeImpl_foldCase(_needle[i]))
            return 0;
    return 1;
}

mmint_t MemeImpl_shortForwardSearch(
    const uint8_t* _haystack, mmint_t _haystack_len,
    const uint8_t* _needle, mmint_t _needle_len,
    mmflag_case_sensit_t _cs)
{
    /* Highest index at which the needle can still fit (inclusive). */
    const mmint_t last_start = _haystack_len - _needle_len;
    const int     fold    = ((_cs & MemeFlag_CaseSensitive) == 0);
    const uint8_t first   = fold ? __MemeImpl_foldCase(_needle[0]) : _needle[0];
    const uint8_t last    = fold ? __MemeImpl_foldCase(_needle[_needle_len - 1])
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

    /* Dual-position prefilter.  A single-byte scan returns one candidate at a
     * time, so a needle whose first byte is common in the haystack ("ab" over
     * "aaaa...") re-enters the SIMD routine for every position.  Filtering on
     * two needle positions inside the vector unit rejects almost all of those
     * candidates before they cost a scalar verification.
     *
     * Only usable when the two positions differ (a 2-byte needle uses
     * offsets 0 and 1) and when neither position needs case folding, because
     * the SIMD compare is exact: a folded first byte would need the two-key
     * (lower/upper) variant, which the dual-position entry point does not
     * provide. */
    const int use_dual = (!fold && _needle_len >= 2);

    while (pos <= last_start)
    {
        mmint_t hit;

        if (use_dual)
        {
            const mmint_t span = _needle_len - 1;
            const mmint_t limit = last_start - pos;   /* relative last start */
            mmint_t rel = mmsimd_u8_find_dual_pos(
                _haystack + pos, limit + 1 + span,
                first, 0, last, span);

            if (rel < 0)
                return -1;
            if (rel > limit)
                return -1;

            hit = pos + rel;
            /* Offsets 0 and span already matched inside the vector filter. */
            if (mid_len < 1
                || __MemeImpl_windowEquals(_haystack + hit + 1,
                                           _needle + 1, mid_len, fold))
            {
                return hit;
            }

            pos = hit + 1;
            continue;
        }

        {
            const mmint_t scan_len = last_start - pos + 1;
            mmint_t rel;

            if (first_is_letter)
            {
                /* Both cases of the first byte in one SIMD pass. */
                rel = mmsimd_u8_find_dual_val(_haystack + pos, scan_len, first, alternate);
            }
            else
            {
                rel = mmsimd_u8_find(_haystack + pos, scan_len, first);
            }

            if (rel < 0)
                return -1;

            hit = pos + rel;
        }

        /* Last byte next: a single compare rejects most candidates.
         * mid_len < 1 covers both the 1-byte needle (mid_len == -1: first
         * and last are the same byte, already compared above) and the
         * 2-byte needle (mid_len == 0: nothing between first and last). */
        {
            const uint8_t hay_last = _haystack[hit + _needle_len - 1];
            if ((fold ? __MemeImpl_foldCase(hay_last) : hay_last) == last
                && (mid_len < 1
                    || __MemeImpl_windowEquals(_haystack + hit + 1,
                                               _needle + 1, mid_len, fold)))
            {
                return hit;
            }

            pos = hit + 1;
        }
    }

    return -1;
}

static mmint_t __MemeImpl_shortReverseSearch(
    const uint8_t* _haystack, mmint_t _haystack_len,
    const uint8_t* _needle, mmint_t _needle_len,
    mmflag_case_sensit_t _cs)
{
    const int     fold    = ((_cs & MemeFlag_CaseSensitive) == 0);
    const uint8_t first   = fold ? __MemeImpl_foldCase(_needle[0]) : _needle[0];
    const uint8_t last    = fold ? __MemeImpl_foldCase(_needle[_needle_len - 1])
                                : _needle[_needle_len - 1];
    const mmint_t mid_len = _needle_len - 2;
    mmint_t       last_start = _haystack_len - _needle_len;

    /* See MemeImpl_shortForwardSearch: only ASCII letters have a case
     * pair, so only they may be searched as a two-key set. */
    const int     first_is_letter =
        fold && first >= 0x61u && first <= 0x7au;
    const uint8_t alternate =
        first_is_letter ? (uint8_t)(first ^ 0x20u) : first;

    while (last_start >= 0)
    {
        const mmint_t scan_len = last_start + 1;
        mmint_t rel;

        /* mmsimd_u8_rfind_dual_val scans 32-byte blocks from the end and takes the
         * highest matching bit, so the whole search stays linear.  Calling
         * rfind twice per candidate (the previous approach) rescanned the
         * remaining prefix each round, which is quadratic: 16 KiB of 'a'
         * with needle "ab" took 31 ms. */
        if (first_is_letter)
        {
            rel = mmsimd_u8_rfind_dual_val(_haystack, scan_len, first, alternate);
        }
        else
        {
            rel = mmsimd_u8_rfind(_haystack, scan_len, first);
        }

        if (rel < 0)
            return -1;

        const uint8_t hay_last = _haystack[rel + _needle_len - 1];
        /* mid_len < 1: see MemeImpl_shortForwardSearch -- a 1-byte needle
         * has nothing between first and last either. */
        if ((fold ? __MemeImpl_foldCase(hay_last) : hay_last) == last
            && (mid_len < 1
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

    /* Record original length semantics before overwriting _needle_len. */
    const int needle_is_cstr     = (_needle_len < 0);
    const int haystack_is_cstr   = (_haystack_len < 0);

    if (needle_is_cstr)
        _needle_len = (mmint_t)strlen((const char*)_needle);
    if ((_haystack_len == 0 || _needle_len == 0))
        return -1;

    /* An explicitly-sized haystack cannot contain a longer needle. */
    if (_haystack_len > 0 && _haystack_len < _needle_len)
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
        if (haystack_is_cstr)
        {
            /* NUL-terminated haystack.  Case-sensitive matching can only
             * safely use strstr() when the needle is ALSO a NUL-terminated
             * C string; otherwise the explicit length might be shorter than
             * the actual bytes, causing strstr() to read past the intended
             * needle. */
            if (needle_is_cstr && (_cs & MemeFlag_CaseSensitive) != 0)
            {
                const char* hit = strstr((const char*)_haystack,
                                         (const char*)_needle);
                return hit ? (mmint_t)(hit - (const char*)_haystack) : -1;
            }

            const mmint_t hay_len = (mmint_t)strlen((const char*)_haystack);
            if (hay_len < _needle_len)
                return -1;

            return MemeImpl_shortForwardSearch(
                _haystack, hay_len, _needle, _needle_len, _cs);
        }

        return MemeImpl_shortForwardSearch(
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

    /* Record original length semantics before overwriting _needle_len. */
    const int needle_is_cstr     = (_needle_len < 0);
    const int haystack_is_cstr   = (_haystack_len < 0);

    if (needle_is_cstr)
        _needle_len = strlen((const char*)_needle);
    if ((_haystack_len == 0 || _needle_len == 0))
        return -1;

    /* An explicitly-sized haystack cannot contain a longer needle. */
    if (_haystack_len > 0 && _haystack_len < _needle_len)
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
        const mmint_t hay_len = haystack_is_cstr
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
                return mmsimd_u8_find_dual_val(
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
            return mmsimd_u8_rfind_dual_val(
                _source, _source_len,
                (uint8_t)folded, (uint8_t)(folded ^ 0x20u));
        }

        /* Non-ASCII letters -- and every non-letter byte such as digits
         * and punctuation -- only match the raw byte. */
        return mmsimd_u8_rfind(_source, _source_len, _key);
    }

    return mmsimd_u8_rfind(_source, _source_len, _key);
}
