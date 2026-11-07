/*
 * MemeSearcherStack -- reusable string searcher, stack-allocated value type.
 *
 * Pre-processes a needle once (algorithm selection + table building), then
 * reuses the result across many MemeSearcherStack_find() calls.
 *
 * The object layout lives in include_private/meme/impl/searcher_p.h; the
 * public header only exposes the opaque MMSEARCHER__OBJ_SIZE byte buffer.
 * init() records _object_size in reg_size_, and every later entry point reads
 * the size from the object rather than taking it again.
 */

#include <meme/impl/algorithm.h>
#include <mego/predef/symbol/likely.h>
#include <meme/impl/searcher_p.h>
#include <meme/simd/simd.h>
#include <meme/string_fwd.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

MEME_EXTERN_C_SCOPE_START

/* ------------------------------------------------------------------ */
/* table builders (one per bad-char table width)                        */
/* ------------------------------------------------------------------ */

/* Fold one byte to lower case, matching _BM_CF in meme_impl__algorithm.c. */
MG_CAPI_INLINE MemeByte_t __mmsrch_fold(MemeByte_t _c)
{
    return (_c >= (MemeByte_t)'A' && _c <= (MemeByte_t)'Z')
        ? (MemeByte_t)(_c + 32) : _c;
}

#define __MMSRCH_MAKEBAD(SUFFIX, DELTA_T, FOLD_EXPR, CAST)                    \
static void __mmsrch_makeBadDelta_##SUFFIX(                                   \
    DELTA_T* _delta, const MemeByte_t* _pat, MemeInteger_t _pat_len,          \
    int _ci)                                                                  \
{                                                                             \
    const DELTA_T fill = (DELTA_T)_pat_len;                                   \
    for (int i = 0; i < 256; ++i)                                             \
        _delta[i] = fill;                                                     \
                                                                              \
    for (MemeInteger_t i = 0; i < _pat_len - 1; ++i)                          \
    {                                                                         \
        const MemeByte_t b = (MemeByte_t)(_pat[i]);                           \
        _delta[_ci ? __mmsrch_fold(b) : b] = (DELTA_T)(_pat_len - 1 - i);     \
    }                                                                         \
}

__MMSRCH_MAKEBAD(u8,  uint8_t,  0, 0)
__MMSRCH_MAKEBAD(u16, uint16_t, 0, 0)
__MMSRCH_MAKEBAD(u32, uint32_t, 0, 0)
__MMSRCH_MAKEBAD(u64, uint64_t, 0, 0)

/* ------------------------------------------------------------------ */
/* Boyer-Moore search, one instantiation per bad-char table width       */
/* ------------------------------------------------------------------ */

/*
 * Mirrors MemeImpl_SearchByBoyerMoore: `idx` walks leftwards during the
 * comparison and the shift is applied to the mismatching position, not to
 * the pattern's right end.
 */
#define __MMSRCH_BM_SEARCH(SUFFIX, DELTA_T)                                   \
static MemeInteger_t __mmsrch_bm_##SUFFIX(                                    \
    const MemeByte_t* _haystack, MemeInteger_t _haystack_len,                  \
    const MemeByte_t* _pat, MemeInteger_t _pat_len,                            \
    const DELTA_T* _b_delta, const MemeInteger_t* _g_delta, int _ci)           \
{                                                                             \
    const MemeInteger_t last = _pat_len - 1;                                  \
    MemeInteger_t idx = last;                                                 \
                                                                              \
    while (idx < _haystack_len)                                               \
    {                                                                         \
        MemeInteger_t ni = last;                                              \
                                                                              \
        while (ni >= 0)                                                       \
        {                                                                     \
            const MemeByte_t h = _ci ? __mmsrch_fold(_haystack[idx])          \
                                     : _haystack[idx];                        \
            const MemeByte_t p = _ci ? __mmsrch_fold(_pat[ni])                \
                                     : _pat[ni];                              \
            if (h != p)                                                       \
                break;                                                        \
            --idx;                                                            \
            --ni;                                                             \
        }                                                                     \
                                                                              \
        if (ni < 0)                                                           \
            return idx + 1;                                                   \
                                                                              \
        {                                                                     \
            const MemeByte_t hb = _ci ? __mmsrch_fold(_haystack[idx])         \
                                      : _haystack[idx];                       \
            const MemeInteger_t bad  = (MemeInteger_t)_b_delta[hb];           \
            const MemeInteger_t good = _g_delta[ni];                          \
            MemeInteger_t move = MemeMath_Max(bad, good);                     \
                                                                              \
            if (move <= 0)                                                    \
                move = 1;                                                     \
            if ((MemeInteger_t)((uint64_t)idx + (uint64_t)move) < idx)        \
                return -1;                                                    \
            idx += move;                                                      \
        }                                                                     \
    }                                                                         \
                                                                              \
    return -1;                                                                \
}

__MMSRCH_BM_SEARCH(u8,  uint8_t)
__MMSRCH_BM_SEARCH(u16, uint16_t)
__MMSRCH_BM_SEARCH(u32, uint32_t)
__MMSRCH_BM_SEARCH(u64, uint64_t)

/* ------------------------------------------------------------------ */
/* init / unInit                                                        */
/* ------------------------------------------------------------------ */

MEME_API int MEME_STDCALL MemeSearcherStack_init(
    mmsrchstk_t* _out, size_t _object_size,
    const MemeByte_t* _needle, MemeInteger_t _needle_len,
    MemeFlag_CaseSensitivity_t _cs)
{
    MemeSearcherImpl_t* self;

    if (MEGO_SYMBOL__UNLIKELY(_out == NULL || _needle == NULL || _needle_len <= 0))
        return MGEC__INVAL;

    /* Caller must provide at least the current implementation size. */
    if (MEGO_SYMBOL__UNLIKELY(_object_size < sizeof(MemeSearcherImpl_t)))
        return MGEC__OPNOTSUPP;

    self = mmsrch_impl(_out);
    memset(self, 0, sizeof(MemeSearcherImpl_t));

    self->reg_size_  = (mmbyte_t)(_object_size / sizeof(mmint_t));
    self->needle_len = _needle_len;
    self->cs         = _cs;
    self->density    = MemeSearcherDensity_Unknown;

    /* --- short needle (or case-sensitive): fast scan ------------------- */
    if (_needle_len <= MEME_IMPL_SHORT_NEEDLE_MAX
        || (_cs & MemeFlag_CaseSensitive) != 0)
    {
        self->algo    = MemeSearcherAlgo_FastScan;
        self->b_width = 0;

        if ((size_t)_needle_len <= sizeof(self->inline_))
        {
            memcpy(self->inline_, _needle, (size_t)_needle_len);
            self->needle_ptr = self->inline_;
        }
        else
        {
            self->heap_block = malloc((size_t)_needle_len);
            if (self->heap_block == NULL)
            {
                self->reg_size_ = 0;
                return MGEC__NOMEM;
            }
            memcpy(self->heap_block, _needle, (size_t)_needle_len);
            self->needle_ptr = (const MemeByte_t*)self->heap_block;
        }
        return 0;
    }

    /* --- long needle: Boyer-Moore -------------------------------------- */
    self->algo = MemeSearcherAlgo_BoyerMoore;

    if (_needle_len <= (MemeInteger_t)UINT8_MAX)
        self->b_width = 1;
    else if (_needle_len <= (MemeInteger_t)UINT16_MAX)
        self->b_width = 2;
    else if ((uint64_t)_needle_len <= (uint64_t)UINT32_MAX)
        self->b_width = 4;
    else
        self->b_width = 8;

    {
        const size_t n       = (size_t)_needle_len;
        const size_t b_bytes = (self->b_width == 1)
            ? 0 : ((size_t)self->b_width * 256);
        size_t g_bytes;
        size_t total;

        if (n > SIZE_MAX / sizeof(MemeInteger_t))
        {
            self->reg_size_ = 0;
            return MGEC__NOMEM;
        }
        g_bytes = n * sizeof(MemeInteger_t);

        if (b_bytes > SIZE_MAX - g_bytes || n > SIZE_MAX - (b_bytes + g_bytes))
        {
            self->reg_size_ = 0;
            return MGEC__NOMEM;
        }
        total = b_bytes + g_bytes + n;

        self->heap_block = malloc(total);
        if (self->heap_block == NULL)
        {
            self->reg_size_ = 0;
            return MGEC__NOMEM;
        }

        /* Layout: [g_delta][b_delta when wider than u8][needle] */
        {
            uint8_t* base = (uint8_t*)self->heap_block;
            self->heap_g_delta = (MemeInteger_t*)base;
            self->heap_b_delta = (self->b_width == 1)
                ? NULL : (void*)(base + g_bytes);
            self->needle_ptr = base + g_bytes + b_bytes;
        }

        memcpy((void*)self->needle_ptr, _needle, n);

        switch (self->b_width)
        {
        case 1:
            __mmsrch_makeBadDelta_u8 (self->inline_, _needle, _needle_len, 1);
            break;
        case 2:
            __mmsrch_makeBadDelta_u16((uint16_t*)self->heap_b_delta, _needle, _needle_len, 1);
            break;
        case 4:
            __mmsrch_makeBadDelta_u32((uint32_t*)self->heap_b_delta, _needle, _needle_len, 1);
            break;
        default:
            __mmsrch_makeBadDelta_u64((uint64_t*)self->heap_b_delta, _needle, _needle_len, 1);
            break;
        }

        /* Good-suffix table with the same case folding as the search. */
        __MemeBoyerMooreCI_makeGoodDelta(
            self->heap_g_delta, _needle, _needle_len);
    }

    return 0;
}

MEME_API int MEME_STDCALL MemeSearcherStack_initByOther(
    mmsrchstk_t* _out, size_t _object_size,
    const mmsrchstk_t* _other)
{
    const MemeSearcherImpl_t* src;

    if (MEGO_SYMBOL__UNLIKELY(_out == NULL || _other == NULL))
        return MGEC__INVAL;

    if (MEGO_SYMBOL__UNLIKELY(_object_size < sizeof(MemeSearcherImpl_t)))
        return MGEC__OPNOTSUPP;

    src = mmsrch_implc(_other);

    /* A copy of an uninitialised object is just an empty object. */
    if (src->reg_size_ == 0)
    {
        memset(mmsrch_impl(_out), 0, sizeof(MemeSearcherImpl_t));
        return 0;
    }

    /* Re-run init() over the source needle: that rebuilds every table and
     * repoints needle_ptr at the new object's own storage, so the two
     * objects share nothing and can be released independently. */
    return MemeSearcherStack_init(
        _out, _object_size,
        src->needle_ptr, src->needle_len, src->cs);
}

/*
 * Relocate pointers that refer into the source object's own inline_ storage
 * so they point into the destination's inline_ instead.  Pointers into the
 * heap block are left alone — both objects would agree on them, but the
 * source is cleared right after, so only the destination keeps them.
 */
static void __mmsrch_relocate(MemeSearcherImpl_t* _dst,
                              const MemeSearcherImpl_t* _src)
{
    const MemeByte_t* begin = _src->inline_;
    const MemeByte_t* end   = _src->inline_ + sizeof(_src->inline_);

    /* needle_ptr: may be inline_ (fast-scan / BM u8) or heap_block. */
    if (_src->needle_ptr >= begin && _src->needle_ptr < end)
        _dst->needle_ptr = _dst->inline_ + (_src->needle_ptr - begin);

    /* heap_b_delta: only ever == inline_ when b_width == 1; otherwise it
     * points into heap_block. */
    if ((const MemeByte_t*)_src->heap_b_delta >= begin &&
        (const MemeByte_t*)_src->heap_b_delta <  end)
    {
        _dst->heap_b_delta = _dst->inline_;
    }

    /* heap_block and heap_g_delta point into the heap and need no fixup. */
}

MEME_API int MEME_STDCALL MemeSearcherStack_move(
    mmsrchstk_t* _out, size_t _object_size,
    mmsrchstk_t* _other)
{
    MemeSearcherImpl_t* dst;
    MemeSearcherImpl_t* src;

    if (MEGO_SYMBOL__UNLIKELY(_out == NULL || _other == NULL))
        return MGEC__INVAL;

    if (MEGO_SYMBOL__UNLIKELY(_object_size < sizeof(MemeSearcherImpl_t)))
        return MGEC__OPNOTSUPP;

    if (MEGO_SYMBOL__UNLIKELY(_out == _other))
        return MGEC__INVAL;

    src = mmsrch_impl(_other);

    /* Nothing to move: produce an empty destination. */
    if (src->reg_size_ == 0)
    {
        memset(mmsrch_impl(_out), 0, sizeof(MemeSearcherImpl_t));
        return 0;
    }

    dst = mmsrch_impl(_out);

    /* Byte copy, then fix up the pointers that referred to src->inline_. */
    memcpy(dst, src, sizeof(MemeSearcherImpl_t));
    __mmsrch_relocate(dst, src);

    /* The source no longer owns anything. */
    memset(src, 0, sizeof(MemeSearcherImpl_t));

    return 0;
}

MEME_API int MEME_STDCALL MemeSearcherStack_unInit(mmsrchstk_t* _out)
{
    MemeSearcherImpl_t* self;

    if (MEGO_SYMBOL__UNLIKELY(_out == NULL))
        return MGEC__INVAL;

    self = mmsrch_impl(_out);

    /* Not initialised (or already released): nothing to do. */
    if (self->reg_size_ == 0)
        return 0;

    if (self->heap_block != NULL)
    {
        free(self->heap_block);
        self->heap_block = NULL;
    }

    self->heap_b_delta = NULL;
    self->heap_g_delta = NULL;
    self->needle_ptr   = NULL;
    self->needle_len   = 0;
    self->algo         = MemeSearcherAlgo_None;
    self->b_width      = 0;
    self->call_count   = 0;
    self->hit_count    = 0;
    self->density      = MemeSearcherDensity_Unknown;
    self->reg_size_    = 0;

    return 0;
}

/* ------------------------------------------------------------------ */
/* find                                                                 */
/* ------------------------------------------------------------------ */

MEME_API MemeInteger_t MEME_STDCALL MemeSearcherStack_find(
    const mmsrchstk_t* _out,
    const MemeByte_t* _haystack, MemeInteger_t _haystack_len,
    MemeInteger_t _offset)
{
    const MemeSearcherImpl_t* self;
    MemeInteger_t remaining;
    MemeInteger_t result = -1;

    if (MEGO_SYMBOL__UNLIKELY(_out == NULL || _haystack == NULL))
        return -1;

    self = mmsrch_implc(_out);

    /* Not initialised. */
    if (self->reg_size_ == 0)
        return -1;

    if (_haystack_len < 0 || _offset < 0 || _offset > _haystack_len)
        return -1;

    /* Empty needle matches at the offset. */
    if (self->needle_len <= 0)
        return _offset;

    remaining = _haystack_len - _offset;

    if (self->needle_len <= remaining)
    {
        if (self->algo == MemeSearcherAlgo_FastScan)
        {
            result = __MemeImpl_shortForwardSearch(
                _haystack + _offset, remaining,
                self->needle_ptr, self->needle_len,
                (mmflag_case_sensit_t)self->cs);

            if (result >= 0)
                result += _offset;
        }
        else if (self->algo == MemeSearcherAlgo_BoyerMoore)
        {
            const MemeByte_t* h = _haystack + _offset;
            const void* b_delta = (self->b_width == 1)
                ? (const void*)self->inline_ : self->heap_b_delta;

            switch (self->b_width)
            {
            case 1:
                result = __mmsrch_bm_u8 (h, remaining, self->needle_ptr,
                                         self->needle_len,
                                         (const uint8_t*)b_delta,
                                         self->heap_g_delta, 1);
                break;
            case 2:
                result = __mmsrch_bm_u16(h, remaining, self->needle_ptr,
                                         self->needle_len,
                                         (const uint16_t*)b_delta,
                                         self->heap_g_delta, 1);
                break;
            case 4:
                result = __mmsrch_bm_u32(h, remaining, self->needle_ptr,
                                         self->needle_len,
                                         (const uint32_t*)b_delta,
                                         self->heap_g_delta, 1);
                break;
            case 8:
                result = __mmsrch_bm_u64(h, remaining, self->needle_ptr,
                                         self->needle_len,
                                         (const uint64_t*)b_delta,
                                         self->heap_g_delta, 1);
                break;
            default:
                break;
            }

            if (result >= 0)
                result += _offset;
        }
    }

    /* Update density counters (single-threaded usage per object). */
    {
        MemeSearcherImpl_t* mut = mmsrch_impl((mmsrchstk_t*)(void*)_out);

        if (result >= 0)
            mut->hit_count++;
        mut->call_count++;

        if (mut->call_count >= 4)
        {
            const int pct = (int)((mut->hit_count * 100) / mut->call_count);
            if (pct < 25)
                mut->density = MemeSearcherDensity_Rare;
            else if (pct < 50)
                mut->density = MemeSearcherDensity_Sparse;
            else if (pct < 75)
                mut->density = MemeSearcherDensity_Common;
            else
                mut->density = MemeSearcherDensity_Frequent;
        }
    }

    return result;
}

/* ------------------------------------------------------------------ */
/* density                                                              */
/* ------------------------------------------------------------------ */

MEME_API MemeSearcherDensity_t MEME_STDCALL MemeSearcherStack_density(
    const mmsrchstk_t* _out)
{
    if (MEGO_SYMBOL__UNLIKELY(_out == NULL))
        return MemeSearcherDensity_Unknown;

    return mmsrch_implc(_out)->density;
}

MEME_API int MEME_STDCALL MemeSearcherStack_resetDensity(mmsrchstk_t* _out)
{
    MemeSearcherImpl_t* self;

    if (MEGO_SYMBOL__UNLIKELY(_out == NULL))
        return MGEC__INVAL;

    self = mmsrch_impl(_out);

    if (self->reg_size_ == 0)
        return MGEC__INVAL;

    self->call_count = 0;
    self->hit_count  = 0;
    self->density    = MemeSearcherDensity_Unknown;
    return 0;
}

MEME_EXTERN_C_SCOPE_ENDED
