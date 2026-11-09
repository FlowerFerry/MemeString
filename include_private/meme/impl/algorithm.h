
#ifndef MEME_IMPL_ALGORITHM_H_INCLUDED
#define MEME_IMPL_ALGORITHM_H_INCLUDED

#include "meme/string_fwd.h"

#define MemeMath_Max(a, b) (((a) > (b)) ? (a) : (b))
#define MemeMath_Min(a, b) (((a) < (b)) ? (a) : (b))

mmint_t MemeImpl_SearchByViolenceWithSensitivity(
	const uint8_t* _source, mmint_t _source_len, uint8_t _key, 
	mmflag_case_sensit_t _cs
);

mmint_t MemeImpl_ReverseSearchByViolenceWithSensitivity(
	const uint8_t* _source, mmint_t _source_len, uint8_t _key,
	mmflag_case_sensit_t _cs
);

mmint_t MemeImpl_SearchByBoyerMoore
(
	const uint8_t* _haystack, mmint_t _haystack_len,
	const uint8_t* _needle, mmint_t _needle_len
);

mmint_t MemeImpl_ReverseSearchByBoyerMoore
(
	const uint8_t* _haystack, mmint_t _haystack_len,
	const uint8_t* _needle, mmint_t _needle_len
);

mmint_t MemeImpl_SearchByBoyerMooreWithSensitivity
(
	const uint8_t* _haystack, mmint_t _haystack_len,
	const uint8_t* _needle, mmint_t _needle_len,
	mmflag_case_sensit_t _cs
);

mmint_t MemeImpl_ReverseSearchByBoyerMooreWithSensitivity
(
	const uint8_t* _haystack, mmint_t _haystack_len,
	const uint8_t* _needle, mmint_t _needle_len,
	mmflag_case_sensit_t _cs
);

mmint_t MemeImpl_SearchByBoyerMoore_CaseInsensitive
(
	const uint8_t* _haystack, mmint_t _haystack_len,
	const uint8_t* _needle, mmint_t _needle_len
);

mmint_t MemeImpl_ReverseSearchByBoyerMoore_CaseInsensitive
(
	const uint8_t* _haystack, mmint_t _haystack_len,
	const uint8_t* _needle, mmint_t _needle_len
);


/* Helpers shared with src/meme_searcher.c (Phase 2 reusable searcher).
 * Promoted from static so they can be reused across translation units
 * instead of duplicating the Boyer-Moore preprocessing. */

/* Short-needle threshold used by the Phase-1 dispatch. */
#ifndef MEME_IMPL_SHORT_NEEDLE_MAX
#define MEME_IMPL_SHORT_NEEDLE_MAX   (8 * sizeof(mmint_t))
#endif
/* Use FastScan when there are at most this many candidate start positions.
 * 512 is the crossover point where Boyer-Moore table construction finally
 * amortizes its cost on this host (MSVC x64, AVX2). */
#ifndef MEME_IMPL_FEW_CANDIDATES
#define MEME_IMPL_FEW_CANDIDATES     (512)
#endif

/* Needle lengths at or below this bound use stack buffers for the
 * good-suffix table and helper needle copies, avoiding malloc/free on the
 * common path.  Sized so the extra stack (128 * 8 + 128 = 1152 bytes)
 * stays small next to the 2 KB bad-character table. */
#ifndef MEME_IMPL_LOCAL_NEEDLE_MAX
#define MEME_IMPL_LOCAL_NEEDLE_MAX   (128)
#endif

void MemeBoyerMoore_makeBadDelta(
    MemeInteger_t* _delta1, MemeInteger_t _delta1_len,
    const uint8_t* _pat, MemeInteger_t _pat_len);

void MemeBoyerMoore_makeGoodDelta(
    MemeInteger_t* _delta2, const uint8_t* _pat, MemeInteger_t _pat_len);

mmint_t MemeImpl_shortForwardSearch(
    const uint8_t* _haystack, mmint_t _haystack_len,
    const uint8_t* _needle, mmint_t _needle_len,
    mmflag_case_sensit_t _cs);


/* Case-insensitive good-suffix table, shared with meme_searcher.c so the
 * reusable searcher applies exactly the same folding as the BM path. */
void MemeBoyerMooreCI_makeGoodDelta(
    MemeInteger_t* _delta2, const uint8_t* _pat, MemeInteger_t _pat_len);
#endif // !MEME_IMPL_ALGORITHM_H_INCLUDED
