
#ifndef MEME_IMPL_STRING_P_MEDIUM_H_INCLUDED
#define MEME_IMPL_STRING_P_MEDIUM_H_INCLUDED

#include <meme/impl/string.h>
#include <meme/impl/string_p__small.h>
#include <meme/string_memory.h>
#include <meme/impl/string_memory.h>
#include <mego/predef/symbol/likely.h>
#include <mego/err/ec.h>
#include <string.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

int 
MemeStringMedium_canBeAppendIt(const MemeStringMedium_t* _s, MemeInteger_t _buflen);

const uint8_t*
MemeStringMedium_constData(const MemeStringMedium_t* _s);

uint8_t*
MemeStringMedium_data(MemeStringMedium_t* _s);

uint8_t*
MemeStringMedium_iteratorEnd(MemeStringMedium_t* _s);

MemeInteger_t
MemeStringMedium_maxByteSize(const MemeStringMedium_t* _s);

MemeInteger_t
MemeStringMedium_realByteSize(const MemeStringMedium_t* _s);

MemeInteger_t
MemeStringMedium_maxByteCapacity(const MemeStringMedium_t* _s);

MemeInteger_t
MemeStringMedium_availableByteCapacity(const MemeStringMedium_t* _s);

mmint_t
MemeStringMedium_usedByteFrontCapacity(const MemeStringMedium_t* _s);

void
MemeStringMedium_byteSizeOffset(MemeStringMedium_t* _s, MemeInteger_t _offset);

void
MemeStringMedium_byteSizeOffsetAndSetZero(MemeStringMedium_t* _s, MemeInteger_t _offset);

int
MemeStringMedium_appendWithByte(
	MemeStringMedium_t* _s, MemeInteger_t _count, MemeByte_t _byte);

int
MemeStringMedium_appendWithBytes(
	MemeStringMedium_t* _s, const MemeByte_t* _buf, MemeInteger_t _buflen);

int 
MemeStringMedium_insertWithBytes(
    MemeStringMedium_t* _s, MemeInteger_t _pos, const MemeByte_t* _buf, MemeInteger_t _buflen);

int
MemeStringMedium_initWithCapacity(
	MemeStringMedium_t* _s, MemeInteger_t _capacity);

int
MemeStringMedium_assign(MemeStringMedium_t* _s, const MemeByte_t* _buf, MemeInteger_t _len);

MemeInteger_t
MemeStringMedium_remove(
	MemeStringMedium_t* _s, MemeInteger_t _pos, MemeInteger_t _count);

mgec_t 
MemeStringMedium_resizeAndOverwrite(MemeStringMedium_t* _s, mmint_t _size);

int
MemeStringMedium_resizeWithByte(MemeStringMedium_t* _s, MemeInteger_t _size, MemeByte_t _byte);

int
MemeStringMedium_capacityExpansion(
	MemeStringMedium_t* _s, MemeInteger_t _minSizeRequest);

int
MemeStringMedium_clear(MemeStringMedium_t* _s);

int
MemeStringMedium_unInit(MemeStringMedium_t* _s);

int
MemeStringMedium_reset(MemeStringMedium_t* _s);

void 
MemeStringMedium_shrinkTailZero(MemeStringMedium_t* _s);

MemeInteger_t
MemeStringMedium_checkHeadTailMemory(const MemeStringMedium_t* _s);




#ifdef __cplusplus
}
#endif

#endif // !MEME_IMPL_STRING_P_MEDIUM_H_INCLUDED
