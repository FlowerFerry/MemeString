
#ifndef MEME_IMPL_ALGORITHM_H_INCLUDED
#define MEME_IMPL_ALGORITHM_H_INCLUDED

#include "meme/string_fwd.h"

MemeInteger_t MemeImpl_SearchByViolenceWithSensitivity(
	const uint8_t* _source, MemeInteger_t _source_len, uint8_t _key, 
	MemeFlag_CaseSensitivity_t _cs
);

MemeInteger_t MemeImpl_SearchByBoyerMoore
(
	const uint8_t* _haystack, MemeInteger_t _haystack_len,
	const uint8_t* _needle, MemeInteger_t _needle_len
);

MemeInteger_t MemeImpl_SearchByBoyerMooreWithSensitivity
(
	const uint8_t* _haystack, MemeInteger_t _haystack_len,
	const uint8_t* _needle, MemeInteger_t _needle_len,
	MemeFlag_CaseSensitivity_t _cs
);


#endif // !MEME_IMPL_ALGORITHM_H_INCLUDED
