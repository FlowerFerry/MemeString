
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


#endif // !MEME_IMPL_ALGORITHM_H_INCLUDED
