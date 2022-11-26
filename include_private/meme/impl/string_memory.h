
#ifndef MEME_IMPL_STRING_MEMORY_H_INCLUDED
#define MEME_IMPL_STRING_MEMORY_H_INCLUDED

#include "meme/string_memory.h"
#include <meme/string_fwd.h>

#ifndef MMOPT__HEADTAIL_MEMCHECK_ENABLED
#define MMOPT__HEADTAIL_MEMCHECK_ENABLED (0)
#endif

#if MMOPT__HEADTAIL_MEMCHECK_ENABLED
typedef struct _MemeCheckHead_t {
	int64_t count;
	uint8_t byte[6];
	uint16_t crc;
} MemeCheckHead_t;

typedef struct _MemeCheckTail_t {
	uint8_t byte[6];
	uint16_t crc;
} MemeCheckTail_t;

uint16_t 
	MemeCheck_Crc16le(const uint8_t* _buf, size_t _len);
void* 
	MemeCheck_Malloc(size_t _size);
void* 
	MemeCheck_Realloc(void* _block, size_t _size);
void 
	MemeCheck_Free(void* _pointer);

void 
	MemeCheckHead_init(MemeCheckHead_t* _factor);

void 
	MemeCheckTail_init(MemeCheckTail_t* _factor);

MemeInteger_t 
	MemeCheckHead_calibrate(const MemeCheckHead_t* _factor);

MemeInteger_t 
	MemeCheckTail_calibrate(const MemeCheckTail_t* _factor);

const uint8_t*
	MemeCheck_getHeadPointer(const void* _data);
const uint8_t*
	MemeCheck_getTailPointer(const void* _data);
MemeInteger_t
	MemeCheck_calibrate(const void* _data);

#endif

int MemeStringMemory_allocFunctionSwitch(
	MemeString_MallocFunction_t* _in_malloc_fn,
	MemeString_FreeFunction_t* _in_free_fn,
	MemeString_MallocFunction_t** _out_malloc_fn,
	MemeString_FreeFunction_t** _out_free_fn);

#endif // !MEME_IMPL_STRING_MEMORY_H_INCLUDED
