
#ifndef MEGO_PREDEF_ENDIAN_H_INCLUDED
#define MEGO_PREDEF_ENDIAN_H_INCLUDED

// @https://en.wikipedia.org/wiki/Endianness

// example:
//     storage of a 32-bit word (hexadecimal 0A0B0C0D) 
//         increasing addresses -->
//         on BIG_BYTE    : 0A 0B 0C 0D
//         on BIG_WORD    : 0C 0D 0A 0B
//         on LITTLE_BYTE : 0D 0C 0B 0A
//         on LITTLE_WORD : 0B 0A 0D 0C

#define _MEGO_ENDIAN__BIG_BYTE_STRING "Byte-Swapped Big-Endian"
#define _MEGO_ENDIAN__BIG_WORD_STRING "Word-Swapped Big-Endian"			// Honeywell 316
#define _MEGO_ENDIAN__LITTLE_BYTE_STRING "Byte-Swapped Little-Endian"
#define _MEGO_ENDIAN__LITTLE_WORD_STRING "Word-Swapped Little-Endian"	    // DEC PDP-11

#if !defined(MEGO_ENDIAN__STRING)

#include "mego/predef/library/c/gnu.h"

#include "mego/predef/os/macos.h"
#include "mego/predef/os/bsd.h"
#include "mego/predef/os/bsd/open.h"

/* GNU libc provides a header defining __BYTE_ORDER, or _BYTE_ORDER.
* And some OSs provide some for of endian header also.
*/
#	if MEGO_LIB__C_GNU__AVAILABLE
#		include <endian.h>
#	endif

#	if MEGO_OS__MACOS__AVAILABLE
#		include <machine/endian.h>
#	endif

#	if MEGO_OS__BSD__AVAILABLE
#		if MEGO_OS__BSD_OPEN__AVAILABLE
#			include <machine/endian.h>
#		else
#			include <sys/endian.h>
#		endif
#	endif

#	if defined(__BYTE_ORDER)

#		if defined(__BIG_ENDIAN) && (__BYTE_ORDER == __BIG_ENDIAN)
#			define MEGO_ENDIAN__BIG_BYTE (1)
#			define MEGO_ENDIAN__STRING _MEGO_ENDIAN__BIG_BYTE_STRING
#		elif defined(__LITTLE_ENDIAN) && (__BYTE_ORDER == __LITTLE_ENDIAN)
#			define MEGO_ENDIAN__LITTLE_BYTE (1)
#			define MEGO_ENDIAN__STRING _MEGO_ENDIAN__LITTLE_BYTE_STRING
#		elif defined(__PDP_ENDIAN) && (__BYTE_ORDER == __PDP_ENDIAN)
#			define MEGO_ENDIAN__LITTLE_WORD (1)
#			define MEGO_ENDIAN__STRING _MEGO_ENDIAN__LITTLE_WORD_STRING
#		endif

#	elif defined(_BYTE_ORDER)

#		if defined(_BIG_ENDIAN) && (_BYTE_ORDER == _BIG_ENDIAN)
#			define MEGO_ENDIAN__BIG_BYTE (1)
#			define MEGO_ENDIAN__STRING _MEGO_ENDIAN__BIG_BYTE_STRING
#		elif defined(_LITTLE_ENDIAN) && (_BYTE_ORDER == _LITTLE_ENDIAN)
#			define MEGO_ENDIAN__LITTLE_BYTE (1)
#			define MEGO_ENDIAN__STRING _MEGO_ENDIAN__LITTLE_BYTE_STRING
#		elif defined(_PDP_ENDIAN) && (_BYTE_ORDER == _PDP_ENDIAN)
#			define MEGO_ENDIAN__LITTLE_WORD (1)
#			define MEGO_ENDIAN__STRING _MEGO_ENDIAN__LITTLE_WORD_STRING
#		endif

#	endif

#endif // !MEGO_ENDIAN__STRING

#if !defined(MEGO_ENDIAN__STRING)

#   if (defined(__BIG_ENDIAN__) && !defined(__LITTLE_ENDIAN__)) || \
       (defined(_BIG_ENDIAN) && !defined(_LITTLE_ENDIAN)) || \
        defined(__ARMEB__) || \
        defined(__THUMBEB__) || \
        defined(__AARCH64EB__) || \
        defined(_MIPSEB) || \
        defined(__MIPSEB) || \
        defined(__MIPSEB__)

#		define MEGO_ENDIAN__BIG_BYTE (1)
#		define MEGO_ENDIAN__STRING _MEGO_ENDIAN__BIG_BYTE_STRING
#	endif
#endif // !MEGO_ENDIAN__STRING

#if !defined(MEGO_ENDIAN__STRING)

#   if (defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)) || \
       (defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN)) || \
        defined(__ARMEL__) || \
        defined(__THUMBEL__) || \
        defined(__AARCH64EL__) || \
        defined(_MIPSEL) || \
        defined(__MIPSEL) || \
        defined(__MIPSEL__)

#		define MEGO_ENDIAN__LITTLE_BYTE (1)
#		define MEGO_ENDIAN__STRING _MEGO_ENDIAN__LITTLE_BYTE_STRING
#	endif
#endif // !MEGO_ENDIAN__STRING

#if !defined(MEGO_ENDIAN__STRING)

#	include "mego/predef/architecture/arm.h"
#	include "mego/predef/architecture/x86.h"

#   if defined(MEGO_ARCH__M68K) || \
       defined(MEGO_ARCH__PARISC) || \
       defined(MEGO_ARCH__SPARC) || \
       defined(MEGO_ARCH__SYS370) || \
       defined(MEGO_ARCH__SYS390) || \
       defined(MEGO_ARCH__Z)

#		define MEGO_ENDIAN__BIG_BYTE (1)
#		define MEGO_ENDIAN__STRING _MEGO_ENDIAN__BIG_BYTE_STRING
#   endif
#   if defined(MEGO_ARCH__AMD64) || \
       defined(MEGO_ARCH__IA64) || \
       defined(MEGO_ARCH__I386) || \
       defined(MEGO_ARCH__BLACKFIN)

#		define MEGO_ENDIAN__LITTLE_BYTE (1)
#		define MEGO_ENDIAN__STRING _MEGO_ENDIAN__LITTLE_BYTE_STRING
#   endif

#endif // !NCM_ENDIAN_STRING

#if !defined(MEGO_ENDIAN__STRING)

#	if MEGO_ARCH__ARM && MEGO_OS__WINDOWS__AVAILABLE
#		define MEGO_ENDIAN__LITTLE_BYTE (1)
#		define MEGO_ENDIAN__STRING _MEGO_ENDIAN__LITTLE_BYTE_STRING
#	endif

#endif // !MEGO_ENDIAN__STRING


#ifndef MEGO_ENDIAN__BIG_BYTE
#define MEGO_ENDIAN__BIG_BYTE (0)
#endif

#ifndef MEGO_ENDIAN__LITTLE_BYTE
#define MEGO_ENDIAN__LITTLE_BYTE (0)
#endif 

#ifndef MEGO_ENDIAN__LITTLE_WORD
#define MEGO_ENDIAN__LITTLE_WORD (0)
#endif 

#ifndef MEGO_ENDIAN__BIG_WORD
#define MEGO_ENDIAN__BIG_WORD (0)
#endif 

#endif // !MEGO_PREDEF_ENDIAN_H_INCLUDED
