
#ifndef MEME_IMPL_STRING_P_USER_H_INCLUDED
#define MEME_IMPL_STRING_P_USER_H_INCLUDED

#include <meme/impl/atomic.h>
#include <meme/impl/string.h>
#include <meme/impl/string_p__small.h>
#include <meme/impl/string_memory.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

void MemeStringUser_RefCount_init(volatile MemeStringUser_RefCounted_t* _refcount);
MemeInteger_t MemeStringUser_RefCount_increment(volatile MemeStringUser_RefCounted_t* _refcount);
MemeInteger_t MemeStringUser_RefCount_decrementAndDestruct(
	volatile MemeStringUser_RefCounted_t* _refcount);

int MemeStringUser_initTakeOver(MemeStringUser_t* _s,
	MemeString_MallocFunction_t* _cfn, MemeString_FreeFunction_t* _dfn,
	void* _user_data,
	size_t _strlen,
	MemeString_UserObjectDestruct_t* _destruct_fn,
	MemeString_UserObjectData_t* _data_fn);
int MemeStringUser_initByOther(MemeStringUser_t* _s, const MemeStringUser_t* _other);
int MemeStringUser_unInit(MemeStringUser_t* _s);
int MemeStringUser_reset (MemeStringUser_t* _s);
const MemeByte_t* MemeStringUser_constData(const MemeStringUser_t* _s);
void MemeStringUser_setOffset(MemeStringUser_t* _s, MemeInteger_t _offset);
void MemeStringUser_shrinkTailZero(MemeStringUser_t* _s);

MemeInteger_t MemeStringUser_getSharedHeapByteSize (const MemeStringUser_t* _s);
MemeInteger_t MemeStringUser_getPrivateHeapByteSize(const MemeStringUser_t* _s);

MemeInteger_t
MemeStringUser_checkHeadTailMemory(const MemeStringUser_t* _s);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !MEME_IMPL_STRING_P_SMALL_H_INCLUDED