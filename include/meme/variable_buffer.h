
#ifndef MEME_VARIABLE_BUFFER_H_INCLUDED
#define MEME_VARIABLE_BUFFER_H_INCLUDED

#include "variable_buffer_fwd.h"

MEME_EXTERN_C_SCOPE_START

MEME_API int
MEME_STDCALL MemeVariableBufferStack_init(MemeVariableBufferStack_t* _out, size_t _object_size);

MEME_API int
MEME_STDCALL MemeVariableBufferStack_initByOther(
	MemeVariableBufferStack_t* _out, size_t _object_size, const MemeVariableBufferStack_t* _other);

MEME_API int
MEME_STDCALL MemeVariableBufferStack_initByBytes(
	MemeVariableBufferStack_t* _out, size_t _object_size, const MemeByte_t* _buf, MemeInteger_t _len);

MEME_API int
MEME_STDCALL MemeVariableBufferStack_initWithRepeatBytes(
	MemeVariableBufferStack_t* _out, size_t _object_size, MemeInteger_t _count, MemeByte_t _byte);

MEME_API int
MEME_STDCALL MemeVariableBufferStack_unInit(MemeVariableBufferStack_t* _out, size_t _object_size);

MEME_API int
MEME_STDCALL MemeVariableBufferStack_reset(MemeVariableBufferStack_t* _out, size_t _object_size);

MEME_API int
MEME_STDCALL MemeVariableBufferStack_assign(
	MemeVariableBufferStack_t* _s, size_t _object_size, const MemeVariableBufferStack_t* _other);

MEME_API MemeVariableBuffer_Storage_t
MEME_STDCALL MemeVariableBuffer_storageType(MemeVariableBuffer_Const_t _s);

MEME_API int
MEME_STDCALL MemeVariableBuffer_swap(MemeVariableBuffer_t _lhs, MemeVariableBuffer_t _rhs);

MEME_API int
MEME_STDCALL MemeVariableBuffer_isNonempty(MemeVariableBuffer_Const_t _s);

MEME_API int
MEME_STDCALL MemeVariableBuffer_isEmpty(MemeVariableBuffer_Const_t _s);

MEME_API const MemeByte_t*
MEME_STDCALL MemeVariableBuffer_data(MemeVariableBuffer_Const_t _s);

MEME_API MemeByte_t*
MEME_STDCALL MemeVariableBuffer_dataWithNotConst(MemeVariableBuffer_t _s);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_size(MemeVariableBuffer_Const_t _s);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_maxByteSize(MemeVariableBuffer_Const_t _s);

MEME_API int
MEME_STDCALL MemeVariableBuffer_isEqual(
	MemeVariableBuffer_Const_t _s, const MemeByte_t* _buf, MemeInteger_t _len, int* _result);

MEME_API int
MEME_STDCALL MemeVariableBuffer_isEqualWithOther(
	MemeVariableBuffer_Const_t _lhs, MemeVariableBuffer_Const_t _rhs, int* _result);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_availableByteCapacity(MemeVariableBuffer_Const_t _s);

MEME_API const MemeByte_t*
MEME_STDCALL MemeVariableBuffer_constBackItem(MemeVariableBuffer_Const_t _s);

MEME_API MemeByte_t*
MEME_STDCALL MemeVariableBuffer_backItem(MemeVariableBuffer_t _s);

MEME_API const MemeByte_t*
MEME_STDCALL MemeVariableBuffer_constAt(MemeVariableBuffer_Const_t _s, MemeInteger_t _pos);

MEME_API MemeByte_t*
MEME_STDCALL MemeVariableBuffer_at(MemeVariableBuffer_t _s, MemeInteger_t _pos);


MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_indexOfWithBytes(
	MemeVariableBuffer_Const_t _s, MemeInteger_t _offset,
	const MemeByte_t* _needle, MemeInteger_t _needle_len);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_indexOfWithOther(
	MemeVariableBuffer_Const_t _s, MemeInteger_t _offset,
	MemeVariableBuffer_Const_t _other);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_indexOfWithByte(
    MemeVariableBuffer_Const_t _s, MemeInteger_t _offset, MemeByte_t _byte);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_appendWithByte(MemeVariableBuffer_t _s, MemeByte_t _byte);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_appendWithOther(
	MemeVariableBuffer_t _s, MemeVariableBuffer_Const_t _other);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_appendWithBytes(
	MemeVariableBuffer_t _s, const MemeByte_t* _buf, MemeInteger_t _len);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_appendWithRepeatBytes(
	MemeVariableBuffer_t _s, MemeInteger_t _count, MemeByte_t _byte);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_insertWithByte(
    MemeVariableBuffer_t _s, MemeInteger_t _pos, MemeByte_t _byte);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_insertWithBytes(
    MemeVariableBuffer_t _s, MemeInteger_t _pos, const MemeByte_t* _buf, MemeInteger_t _len);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_clear(MemeVariableBuffer_t _s);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_resize(MemeVariableBuffer_t _s, MemeInteger_t _size);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_resizeWithByte(MemeVariableBuffer_t _s, MemeInteger_t _size, MemeByte_t _byte);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_remove(
    MemeVariableBuffer_t _s, MemeInteger_t _pos, MemeInteger_t _count);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_releaseToBuffer(
	MemeVariableBuffer_t _s, MemeBufferStack_t* _out, MemeInteger_t _objectSize);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_releaseToString(
	MemeVariableBuffer_t _s, MemeStringStack_t* _out, MemeInteger_t _objectSize);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_split(
	MemeVariableBuffer_Const_t _s,
	const MemeByte_t* _key, MemeInteger_t _key_len,
	MemeFlag_SplitBehavior_t,
	MemeVariableBufferStack_t* _out, MemeInteger_t* _out_count,
	MemeInteger_t* _search_index
);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_reserve(
	MemeVariableBuffer_t _s, MemeInteger_t _size);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_selfChop(
	MemeVariableBuffer_t _s, MemeInteger_t _n);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_capacityCorrectness(
	MemeVariableBuffer_Const_t _s);

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_VARIABLE_BUFFER_H_INCLUDED
