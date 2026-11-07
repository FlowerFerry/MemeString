#ifndef MEME_SEARCHER_H_INCLUDED
#define MEME_SEARCHER_H_INCLUDED

#include <meme/string_fwd.h>
#include <mego/err/ec.h>

MEME_EXTERN_C_SCOPE_START

/*
 * MemeSearcherStack_t -- reusable string searcher, stack-allocated value type.
 *
 * Mirrors MemeStringStack_t: the object is an opaque fixed-size byte buffer,
 * manipulated only through the MemeSearcherStack_* functions.
 *
 * Only init() takes _object_size to validate and record the size.
 * Subsequent functions read the size from the object's internal reg_size_ field.
 *
 *   MemeSearcherStack_t s;
 *   if (MemeSearcherStack_init(&s, MMSEARCHER__OBJ_SIZE,
 *                              needle, len, MemeFlag_CaseSensitive) == 0) {
 *       ...
 *       MemeSearcherStack_unInit(&s);
 *   }
 */

/*
 * Object size, in mmint_t registers.
 *
 * MemeSearcherImpl_t holds four pointers plus sizeof(mmint_t) fields, so its
 * size follows the integer/pointer width.  The inline_ buffer is always 256
 * bytes.  Measured:
 *
 *   64-bit: 336 bytes = 42 registers
 *   32-bit: 300 bytes = 75 registers
 */
#ifndef MMSEARCHER__OBJ_REG_SIZE
#include <stdint.h>
#if INTPTR_MAX == INT64_MAX
  #define MMSEARCHER__OBJ_REG_SIZE (42)
#elif INTPTR_MAX == INT32_MAX
  #define MMSEARCHER__OBJ_REG_SIZE (75)
#else
  #error "MMSEARCHER__OBJ_REG_SIZE not defined for this integer width"
#endif
#endif

#ifndef MMSEARCHER__OBJ_SIZE
#define MMSEARCHER__OBJ_SIZE (sizeof(mmint_t) * MMSEARCHER__OBJ_REG_SIZE)
#endif

/* Legacy alias, mirroring MEME_STRING__OBJECT_SIZE. */
#define MEME_SEARCHER__OBJECT_SIZE MMSEARCHER__OBJ_SIZE

/* Opaque stack type, mirrors MemeStringStack_t */
typedef struct _MemeSearcherStack_t {
    MemeByte_t byte[MMSEARCHER__OBJ_SIZE];
} MemeSearcherStack_t;

typedef MemeSearcherStack_t MemeSearcher_t;
typedef MemeSearcherStack_t mmsrchstk_t;

/*
 * Hit-density classification, reported by MemeSearcherStack_density().
 */
typedef mmbyte_t MemeSearcherDensity_t;
#define MemeSearcherDensity_Unknown  ((MemeSearcherDensity_t)0)  /* fewer than 4 calls so far     */
#define MemeSearcherDensity_Rare     ((MemeSearcherDensity_t)1)  /* < 25 % of calls returned hit  */
#define MemeSearcherDensity_Sparse   ((MemeSearcherDensity_t)2)  /* 25-50 %                       */
#define MemeSearcherDensity_Common   ((MemeSearcherDensity_t)3)  /* 50-75 %                       */
#define MemeSearcherDensity_Frequent ((MemeSearcherDensity_t)4)  /* > 75 %                        */

/*
 * Lifecycle state, reported by MemeSearcherStack_state().
 *
 *   empty  - default-constructed or reset(): never held a needle.
 *   ready  - init() succeeded; find() may be called.
 *   failed - init() was attempted and rejected; error_code() says why.
 *
 * A plain integer type (not an enum) for the same ABI reason as
 * MemeSearcherDensity_t.
 */
typedef mmbyte_t MemeSearcherState_t;
#define MemeSearcherState_Empty  ((MemeSearcherState_t)0)
#define MemeSearcherState_Ready  ((MemeSearcherState_t)1)
#define MemeSearcherState_Failed ((MemeSearcherState_t)2)

/*
 * Bring a searcher to the empty state without a needle.
 *
 * Zeroes the object and leaves it ready for a later init()/initByOther()/
 * move().  Equivalent to a default construction in C++: state() reports
 * MemeSearcherState_Empty and errorCode() reports MGEC__OK.
 *
 * @return 0 on success, non-zero mgec_t on failure.
 */
MEME_API int MEME_STDCALL MemeSearcherStack_initEmpty(
    mmsrchstk_t* _out, size_t _object_size);

/*
 * Initialise a searcher.
 * @param _object_size Must be MMSEARCHER__OBJ_SIZE. Used to validate and record size.
 * @return 0 on success, non-zero mgec_t on failure.
 */
MEME_API int MEME_STDCALL MemeSearcherStack_init(
    mmsrchstk_t* _out, size_t _object_size,
    const MemeByte_t* _needle, MemeInteger_t _needle_len,
    MemeFlag_CaseSensitivity_t _cs);

/*
 * Deep-copy an initialised searcher into caller-provided storage.
 *
 * The needle is copied and the algorithm tables are rebuilt, so the result
 * owns everything it needs and both objects can be released independently.
 * Mirrors MemeStringStack_initByOther.
 *
 * @return 0 on success, non-zero mgec_t on failure.
 */
MEME_API int MEME_STDCALL MemeSearcherStack_initByOther(
    mmsrchstk_t* _out, size_t _object_size,
    const mmsrchstk_t* _other);

/*
 * Move an initialised searcher into caller-provided storage.
 *
 * Unlike initByOther(), this does NOT rebuild any tables: the bytes are
 * copied and the internal pointers are relocated to the destination.  That
 * makes it O(1) instead of O(needle_len), at the cost of leaving _other in
 * the empty state (its tables now belong to _out).
 *
 * _out must not already own resources.
 *
 * @return 0 on success, non-zero mgec_t on failure.
 */
MEME_API int MEME_STDCALL MemeSearcherStack_move(
    mmsrchstk_t* _out, size_t _object_size,
    mmsrchstk_t* _other);

/*
 * Release resources. No size needed; reads internal state.
 * @return 0 on success, non-zero mgec_t on failure.
 */
MEME_API int MEME_STDCALL MemeSearcherStack_unInit(
    mmsrchstk_t* _out);

/*
 * Behaviour flags for MemeSearcherStack_setFlags().
 *
 * MemeSearcherFlag_DisableDensity
 *       Stop find() from updating the hit/miss counters.  With this flag set
 *       find() writes nothing to the object, so one searcher may be shared by
 *       several threads searching concurrently.  density() then keeps
 *       reporting the value it last computed.
 *
 *       Without the flag find() bumps call_count/hit_count on every call and
 *       is NOT safe to call concurrently on the same object: the increments
 *       are plain (non-atomic) reads-modify-writes, so counts may be lost and
 *       density() may be wrong -- the returned match position is still
 *       correct in either case.
 */
typedef MemeByte_t MemeSearcherFlag_t;
#define MemeSearcherFlag_None            ((MemeSearcherFlag_t)0)
#define MemeSearcherFlag_DisableDensity  ((MemeSearcherFlag_t)0x01)

/*
 * Replace the searcher's behaviour flags.
 *
 * Call once before sharing the searcher across threads; changing the flags
 * while another thread is inside find() is a data race.
 *
 * @return 0 on success, non-zero mgec_t on failure.
 */
MEME_API int MEME_STDCALL MemeSearcherStack_setFlags(
    mmsrchstk_t* _out, MemeSearcherFlag_t _flags);

/* Current behaviour flags; MemeSearcherFlag_None when not ready. */
MEME_API MemeSearcherFlag_t MEME_STDCALL MemeSearcherStack_flags(
    const mmsrchstk_t* _out);

/*
 * Exchange two searchers.
 *
 * A plain byte swap is wrong: needle_ptr and the b_width==1 view of
 * heap_b_delta may point into the owner's own inline_ array.  This function
 * rebases those pointers after the exchange.
 *
 * @return 0 on success, non-zero mgec_t on failure.
 */
MEME_API int MEME_STDCALL MemeSearcherStack_swap(
    mmsrchstk_t* _a, mmsrchstk_t* _b);

/*
 * Search with offset.
 *
 * _haystack_len < 0 means "null-terminated": the length is recovered with
 * strlen(), matching the convention used by the MemeImpl_Search* family.
 *
 * @return Index of the first match, or -1 if not found.
 */
MEME_API MemeInteger_t MEME_STDCALL MemeSearcherStack_find(
    const mmsrchstk_t* _out,
    const MemeByte_t* _haystack, MemeInteger_t _haystack_len,
    MemeInteger_t _offset);

/* Current hit-density classification. */
MEME_API MemeSearcherDensity_t MEME_STDCALL MemeSearcherStack_density(
    const mmsrchstk_t* _out);

/* Reset the hit/miss counters. */
MEME_API int MEME_STDCALL MemeSearcherStack_resetDensity(
    mmsrchstk_t* _out);

/* The needle length in bytes, or 0 if not initialised. */
MEME_API MemeInteger_t MEME_STDCALL MemeSearcherStack_needleSize(
    const mmsrchstk_t* _out);

/* The case-sensitivity flag the searcher was built with. */
MEME_API MemeFlag_CaseSensitivity_t MEME_STDCALL MemeSearcherStack_caseSensitivity(
    const mmsrchstk_t* _out);

/* Error code of the last init()/move()/swap().  Zero when valid. */
MEME_API int MEME_STDCALL MemeSearcherStack_errorCode(
    const mmsrchstk_t* _out);

/* State of the searcher. */
MEME_API MemeSearcherState_t MEME_STDCALL MemeSearcherStack_state(
    const mmsrchstk_t* _out);

MEME_EXTERN_C_SCOPE_ENDED

#endif /* MEME_SEARCHER_H_INCLUDED */
