
#ifndef MEME_ATOMIC_FWD_H_INCLUDED
#define MEME_ATOMIC_FWD_H_INCLUDED

#include <meme/string_fwd.h>

#include <mego/util/os/windows/windows_simplify.h>
#include <mego/predef/compiler/visualc.h>

#if MEGO_OS__WINDOWS__AVAILABLE
#	if INTPTR_MAX == INT32_MAX
#		define MEME_WINDOWS__INTER_LOCKED_INCREMENT InterlockedIncrement
#		define MEME_WINDOWS__INTER_LOCKED_DECREMENT InterlockedDecrement
#		define MEME_WINDOWS__INTER_LOCKED_EXCHANGE  InterlockedExchange
#       define MEME_WINDOWS__INTER_LOCKED_LOAD(VALUE) InterlockedCompareExchange((volatile LONG*)(VALUE), 0, 0)
#	else
#		define MEME_WINDOWS__INTER_LOCKED_INCREMENT InterlockedIncrement64
#		define MEME_WINDOWS__INTER_LOCKED_DECREMENT InterlockedDecrement64
#		define MEME_WINDOWS__INTER_LOCKED_EXCHANGE  InterlockedExchange64
#       define MEME_WINDOWS__INTER_LOCKED_LOAD(VALUE) InterlockedCompareExchange64((volatile LONG64*)(VALUE), 0, 0)
#	endif
#elif !defined(__STDC_NO_ATOMICS__)
#	include <stdatomic.h>
#else
#	error "No supported atomic operations found in this compiler"
#endif 

#if MEGO_OS__WINDOWS__AVAILABLE
typedef MemeInteger_t MemeAtomicInteger_t;
#elif !defined(__STDC_NO_ATOMICS__)
typedef atomic_intptr_t MemeAtomicInteger_t;
#endif

#endif // !MEME_ATOMIC_FWD_H_INCLUDED
