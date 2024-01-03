
#ifndef MEME_IMPL_STRING_P_LARGE_H_INCLUDED
#define MEME_IMPL_STRING_P_LARGE_H_INCLUDED

#include "meme/impl/string.h"
#include "meme/impl/string_p__small.h"
#include "meme/impl/atomic.h"
#include <meme/impl/string_memory.h>
#include "mego/predef/symbol/likely.h"
#include <mego/err/ec.h>

#include <string.h>
#include <assert.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

void MemeStringLarge_RefCount_init(
	volatile MemeStringLarge_RefCounted_t* _refcount);
MemeInteger_t MemeStringLarge_RefCount_increment(
	volatile MemeStringLarge_RefCounted_t* _refcount);
//MemeInteger_t MemeStringLarge_RefCount_decrement(
//	volatile MemeStringLarge_RefCounted_t* _refcount);
MemeInteger_t MemeStringLarge_RefCount_decrementAndDestruct(
	volatile MemeStringLarge_RefCounted_t* _refcount);

int MemeStringLarge_init (
	MemeStringLarge_t* _s, 
	MemeString_MallocFunction_t* _cfn, MemeString_FreeFunction_t* _dfn,
	uint8_t _front_capacity, size_t _capacity);
int MemeStringLarge_initByOther(
	MemeStringLarge_t* _s, const MemeStringLarge_t* _other);
int MemeStringLarge_initByU8bytes (
	MemeStringLarge_t* _s, const uint8_t* _buf, size_t _len,
	MemeString_MallocFunction_t* _cfn, MemeString_FreeFunction_t* _dfn,
	uint8_t _front_capacity, size_t _capacity);
int MemeStringLarge_initAndTakeover(
	MemeStringLarge_t* _s,
	MemeByte_t* _real,
	MemeInteger_t _realSize,
	MemeInteger_t _offset,
	MemeInteger_t _size
);

int MemeStringLarge_unInit(MemeStringLarge_t* _s);
int MemeStringLarge_reset (MemeStringLarge_t* _s);

void
MemeStringLarge_setOffset(MemeStringLarge_t* _s, MemeInteger_t _offset);

void 
MemeStringLarge_shrinkTailZero(MemeStringLarge_t* _s);

const uint8_t* MemeStringLarge_constData(const MemeStringLarge_t* _s);
uint8_t* MemeStringLarge_RefCount_data(volatile MemeStringLarge_RefCounted_t* _refcount);

MemeInteger_t MemeStringLarge_getSharedHeapByteSize (const MemeStringLarge_t* _s);
MemeInteger_t MemeStringLarge_getPrivateHeapByteSize(const MemeStringLarge_t* _s);

MemeInteger_t
MemeStringLarge_checkHeadTailMemory(const MemeStringLarge_t* _s);


#ifdef __cplusplus
}
#endif

#endif // !MEME_IMPL_STRING_P_LARGE_H_INCLUDED
