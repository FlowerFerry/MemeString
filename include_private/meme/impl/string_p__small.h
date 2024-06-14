
#ifndef MEME_IMPL_STRING_P_SMALL_H_INCLUDED
#define MEME_IMPL_STRING_P_SMALL_H_INCLUDED

#include <meme/impl/string.h>
#include <meme/impl/algorithm.h>
#include <mego/err/ec.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

int 
MemeStringSmall_stackReset(MemeStringStack_t* _s);

int 
MemeStringSmall_initByU8bytes(
	MemeStringSmall_t* _s, const MemeByte_t* _utf8, size_t _len);

mgec_t 
MemeStringSmall_resizeAndOverwrite(MemeStringSmall_t* _s, mmint_t _size);

int
MemeStringSmall_resizeWithByte(MemeStringSmall_t* _s, MemeInteger_t _size, MemeByte_t _byte);

int 
MemeStringSmall_clear(MemeStringSmall_t* _s);

int 
MemeStringSmall_isEmpty(const MemeStringSmall_t* _s);

const char* 
MemeStringSmall_cStr(const MemeStringSmall_t* _s);

const uint8_t* 
MemeStringSmall_byteData(const MemeStringSmall_t* _s);

MemeInteger_t 
MemeStringSmall_byteSize(const MemeStringSmall_t* _s);

MemeInteger_t 
MemeStringSmall_byteCapacity(const MemeStringSmall_t* _s);

//inline MemeInteger_t 
//MemeStringSmall_wordSize(const MemeStringSmall_t* _s);

//MemeRuneIndex_t 
//MemeStringSmall_at(const MemeStringSmall_t* _s, size_t _index);

int
MemeStringSmall_appendWithByte(MemeStringSmall_t* _s, MemeInteger_t _count, MemeByte_t _byte);

int
MemeStringSmall_appendWithBytes(MemeStringSmall_t* _s, const MemeByte_t* _buf, MemeInteger_t _buflen);

int
MemeStringSmall_insertWithBytes(MemeStringSmall_t* _s, MemeInteger_t _pos, const MemeByte_t* _buf, MemeInteger_t _buflen);

MemeInteger_t
MemeStringSmall_remove(
	MemeStringSmall_t* _s, MemeInteger_t _pos, MemeInteger_t _count);

int
MemeStringSmall_canBeAppendIt(const MemeStringSmall_t* _s, MemeInteger_t _buflen);

void
MemeStringSmall_byteSizeOffsetAndSetZero(MemeStringSmall_t* _s, MemeInteger_t _offset);

void
MemeStringSmall_shrinkTailZero(MemeStringSmall_t* _s);



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !MEME_IMPL_STRING_P_SMALL_H_INCLUDED
