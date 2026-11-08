
#ifndef MEGO_PREDEF_CHARSET_H_INCLUDED
#define MEGO_PREDEF_CHARSET_H_INCLUDED

/*
 * Execution character set guard.
 *
 * MemeString is a byte-oriented UTF-8 library: every string operation treats
 * data as a UTF-8 byte sequence, and case-insensitive matching folds ASCII
 * letters with `c | 0x20` (see __MemeImpl_casefoldTable() and __mmsrch_fold()).
 * Both assumptions hold for ASCII-compatible character sets, where the bytes
 * 'A'..'Z' are 0x41..0x5A and 'a'..'z' are 0x61..0x7A.
 *
 * EBCDIC (IBM z/Architecture and System/390 in native mode) violates both:
 * letters are not contiguous and the case bit is 0x40, not 0x20.  The library
 * has no EBCDIC code path, so refuse to build rather than silently produce
 * wrong search results at runtime.
 *
 * Note this checks the EXECUTION character set, not the host architecture:
 * IBM z/Architecture running a UTF-8 locale (the usual z/OS Unix Services
 * setup) still compiles, because its char literals are ASCII.
 */

/* __STDC_VERSION__ >= 201112 gives us _Static_assert-era compilers, but the
 * portable runtime check below works everywhere. */
#include <mego/predef/symbol/static_assert.h>

/*
 * The classic EBCDIC self-test: in ASCII 'A' is 0x41, so comparing the literal
 * against the numeric value detects any execution character set that is not
 * ASCII-compatible.  MEGO__STATIC_ASSERT picks static_assert / _Static_assert
 * where available and falls back to a negative-array typedef otherwise.
 */
MEGO__STATIC_ASSERT(
    'A' == 0x41,
    MemeStringRequiresAsciiExecutionCharacterSet,
    "MemeString requires an ASCII-compatible execution character set "
    "(EBCDIC is not supported)");

#endif /* !MEGO_PREDEF_CHARSET_H_INCLUDED */
