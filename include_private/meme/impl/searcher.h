#ifndef MEME_IMPL_SEARCHER_P_H_INCLUDED
#define MEME_IMPL_SEARCHER_P_H_INCLUDED

#include <meme/string_fwd.h>
#include <meme/searcher.h>
#include <mego/err/ec.h>
#include <mego/predef/symbol/inline.h>
#include <mego/predef/symbol/static_assert.h>
#include <stddef.h>

MEME_EXTERN_C_SCOPE_START

/*
 * Algorithm selection for a searcher instance.
 */
typedef mmbyte_t MemeSearcherAlgo_t;
#define MemeSearcherAlgo_None        ((MemeSearcherAlgo_t)0)
#define MemeSearcherAlgo_FastScan    ((MemeSearcherAlgo_t)1)  /* short needle: SIMD first-byte + memcmp */
#define MemeSearcherAlgo_BoyerMoore  ((MemeSearcherAlgo_t)2)  /* long needle: prebuilt BM tables        */

/*
 * Behaviour flags, stored in MemeSearcherImpl_t::flags_.
 *
 * MemeSearcherFlag_DisableDensity
 *   Skip the hit/miss bookkeeping in find().  find() then touches no mutable
 *   state, so the same searcher can be used concurrently from several
 *   threads.  density() keeps reporting whatever it last reached.
 */
typedef mmbyte_t MemeSearcherFlag_t;
#define MemeSearcherFlag_None            ((MemeSearcherFlag_t)0)
#define MemeSearcherFlag_DisableDensity  ((MemeSearcherFlag_t)0x01)

/*
 * Internal layout overlaid on MemeSearcherStack_t.byte[].
 *
 * reg_size_ must be the first field, matching the MemeString stack types: it
 * records the caller's _object_size in units of sizeof(mmint_t) so a later
 * version can tell which layout the object was built with.
 */
typedef struct MemeSearcherImpl
{
    /* MUST be first: object size in units of sizeof(mmint_t) */
    mmbyte_t                   reg_size_;

    /* Result of the last init()/initByOther()/move(), for error_code(). */
    mgec_t                     last_errc;

    mmint_t                    needle_len;
    mmflag_case_sensit_t       cs;
    MemeSearcherAlgo_t         algo;
    mmbyte_t                   b_width;      /* 0: FastScan, 1/2/4/8: BM width */

    /*
     * Behaviour flags; see MemeSearcherFlag_* below.  Placed in a padding
     * byte between b_width and density so the struct size is unchanged.
     */
    mmbyte_t                   flags_;

    MemeSearcherDensity_t      density;
    mmint_t                    call_count;
    mmint_t                    hit_count;

    /* Heap block: the only pointer that is ever freed. */
    void*                      heap_block;
    /* Views into heap_block, or NULL. */
    void*                      heap_b_delta;
    mmint_t*                   heap_g_delta;
    /* Needle bytes: inline_ or heap_block. */
    const mmbyte_t*            needle_ptr;

    /* Inline storage: bad-char table (b_width==1) or the needle itself. */
    mmbyte_t                   inline_[256];
} MemeSearcherImpl_t;

/* Ensure the implementation struct fits in the opaque stack object. */
MEGO__STATIC_ASSERT(
    sizeof(MemeSearcherImpl_t) == MMSEARCHER__OBJ_SIZE,
    MemeSearcherImplSizeMismatch,
    "MemeSearcherImpl_t must equal MMSEARCHER__OBJ_SIZE");

/* Ensure reg_size_ (mmbyte_t, 0-255) can hold the object size in registers. */
MEGO__STATIC_ASSERT(
    (sizeof(MemeSearcherImpl_t) + sizeof(mmint_t) - 1) / sizeof(mmint_t) == MMSEARCHER__OBJ_REG_SIZE,
    MemeSearcherImplRegSizeOverflow,
    "MemeSearcherImpl_t register count must equal MMSEARCHER__OBJ_REG_SIZE");

/* Overlay the opaque stack object onto the implementation struct. */
MG_CAPI_INLINE MemeSearcherImpl_t* mmsrch_impl(mmsrchstk_t* _s)
{
    return (MemeSearcherImpl_t*)(void*)_s;
}

MG_CAPI_INLINE const MemeSearcherImpl_t* mmsrch_implc(const mmsrchstk_t* _s)
{
    return (const MemeSearcherImpl_t*)(const void*)_s;
}

MEME_EXTERN_C_SCOPE_ENDED

#endif /* MEME_IMPL_SEARCHER_P_H_INCLUDED */
