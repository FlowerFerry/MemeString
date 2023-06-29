
#ifndef MEME_IMPL_STRING_P_SMALL_H_INCLUDED
#define MEME_IMPL_STRING_P_SMALL_H_INCLUDED

#include <meme/impl/string.h>
#include <meme/impl/algorithm.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

inline int 
MemeStringSmall_stackReset(MemeStringStack_t* _s);

inline int 
MemeStringSmall_initByU8bytes(
	MemeStringSmall_t* _s, const MemeByte_t* _utf8, size_t _len);

inline int
MemeStringSmall_resizeWithByte(MemeStringSmall_t* _s, MemeInteger_t _size, MemeByte_t _byte);

inline int 
MemeStringSmall_clear(MemeStringSmall_t* _s);

inline int 
MemeStringSmall_isEmpty(const MemeStringSmall_t* _s);

inline const char* 
MemeStringSmall_cStr(const MemeStringSmall_t* _s);

inline const uint8_t* 
MemeStringSmall_byteData(const MemeStringSmall_t* _s);

inline MemeInteger_t 
MemeStringSmall_byteSize(const MemeStringSmall_t* _s);

inline MemeInteger_t 
MemeStringSmall_byteCapacity(const MemeStringSmall_t* _s);

//inline MemeInteger_t 
//MemeStringSmall_wordSize(const MemeStringSmall_t* _s);

inline MemeRuneIndex_t 
MemeStringSmall_at(const MemeStringSmall_t* _s, size_t _index);

inline int
MemeStringSmall_appendWithByte(MemeStringSmall_t* _s, MemeInteger_t _count, MemeByte_t _byte);

inline int
MemeStringSmall_appendWithBytes(MemeStringSmall_t* _s, const MemeByte_t* _buf, MemeInteger_t _buflen);

inline int
MemeStringSmall_insertWithBytes(MemeStringSmall_t* _s, MemeInteger_t _pos, const MemeByte_t* _buf, MemeInteger_t _buflen);

inline MemeInteger_t
MemeStringSmall_remove(
	MemeStringSmall_t* _s, MemeInteger_t _pos, MemeInteger_t _count);

inline int
MemeStringSmall_canBeAppendIt(const MemeStringSmall_t* _s, MemeInteger_t _buflen);

inline void
MemeStringSmall_byteSizeOffsetAndSetZero(MemeStringSmall_t* _s, MemeInteger_t _offset);

inline void
MemeStringSmall_shrinkTailZero(MemeStringSmall_t* _s);



inline int MemeStringSmall_initByU8bytes(
	MemeStringSmall_t* _s, const MemeByte_t* _utf8, size_t _len)
{
	assert(_s);
	assert(_len <= MMS__GET_SMALL_BUFFER_SIZE);

	if (_utf8) {
		memcpy(_s->buffer_, _utf8, _len);
		_s->buffer_[_len] = '\0';
		_s->type_ = MemeString_ImplType_small;
		_s->capacity_ = (uint8_t)(MMS__GET_SMALL_BUFFER_SIZE - _len);
	}
	else {
		MemeStringSmall_clear(_s);
	}
	return 0;
}

inline int MemeStringSmall_stackReset(MemeStringStack_t * _s)
{
	assert(_s);

	MemeStringSmall_clear((MemeStringSmall_t*)_s);
	return 0;
}

inline int MemeStringSmall_clear(MemeStringSmall_t * _s)
{
	assert(_s);

	_s->buffer_[0] = 0;
	_s->type_ = MemeString_ImplType_small;
	_s->capacity_ = MMS__GET_SMALL_BUFFER_SIZE;
	return 0;
}

inline int MemeStringSmall_isEmpty(const MemeStringSmall_t * _s)
{
	assert(_s);
	return MMS__GET_SMALL_BUFFER_SIZE == _s->capacity_;
}

inline const char * MemeStringSmall_cStr(const MemeStringSmall_t * _s)
{
	assert(_s);
	return (const char*)_s->buffer_;
}

inline const uint8_t * MemeStringSmall_byteData(const MemeStringSmall_t * _s)
{
	assert(_s);
	return _s->buffer_;
}

inline MemeInteger_t MemeStringSmall_byteSize(const MemeStringSmall_t * _s)
{
	assert(_s);
	return MMS__GET_SMALL_BUFFER_SIZE - _s->capacity_;
}

inline MemeInteger_t MemeStringSmall_byteCapacity(const MemeStringSmall_t * _s)
{
	assert(_s);
	return _s->capacity_;
}

inline int MemeStringSmall_canBeAppendIt(const MemeStringSmall_t* _s, MemeInteger_t _buflen)
{
	assert(_s);
	if (MemeStringSmall_byteCapacity(_s) < _buflen)
		return MMENO__POSIX_OFFSET(E2BIG);

	return 0;
}

inline void MemeStringSmall_byteSizeOffsetAndSetZero(MemeStringSmall_t* _s, MemeInteger_t _offset)
{
	_s->capacity_ -= _offset;
	_s->buffer_[MemeStringSmall_byteSize(_s)] = '\0';
}

inline void MemeStringSmall_shrinkTailZero(MemeStringSmall_t* _s)
{
	MemeInteger_t index = MemeStringSmall_byteSize(_s);
	while (_s->buffer_[index] == 0 && index > 0 && _s->buffer_[index - 1] == 0)
		--index;
	_s->capacity_ += (MemeStringSmall_byteSize(_s) - index);
}

inline int MemeStringSmall_resizeWithByte(MemeStringSmall_t* _s, MemeInteger_t _size, MemeByte_t _byte)
{
	if (_size <= MemeStringSmall_byteSize(_s))
	{
		//_s->capacity_ = (uint8_t)(MMS__GET_SMALL_BUFFER_SIZE - _size);
		//_s->buffer_[MemeStringSmall_byteSize(_s)] = 0;

		MemeStringSmall_byteSizeOffsetAndSetZero(_s, _size - (MemeStringSmall_byteSize(_s)));
	}
	else {
		return MemeStringSmall_appendWithByte(_s, _size - MemeStringSmall_byteSize(_s), _byte);
	}
	return 0;
}

inline int MemeStringSmall_appendWithByte(MemeStringSmall_t* _s, MemeInteger_t _count, MemeByte_t _byte)
{
	MemeByte_t* pointer = NULL;

	//if (_count <= 0)
	//	return 0;

	pointer = _s->buffer_ + MemeStringSmall_byteSize(_s);
	MemeStringSmall_byteSizeOffsetAndSetZero(_s, _count);
	for (; 0<= --_count; ) {
		*pointer++ = _byte;
	}
	return 0;
}

inline int MemeStringSmall_appendWithBytes(MemeStringSmall_t* _s, const MemeByte_t* _buf, MemeInteger_t _buflen)
{
	assert(_s);
	assert(_buf);

	//_buflen = MemeMath_Min(_s->capacity_, _buflen);
	//if (_buflen <= 0)
	//	return 0;

	memcpy(_s->buffer_ + MemeStringSmall_byteSize(_s), _buf, _buflen);
	_s->capacity_ -= (uint8_t)_buflen;
	_s->buffer_[MemeStringSmall_byteSize(_s)] = 0;

	return 0;
}

inline int MemeStringSmall_insertWithBytes(
	MemeStringSmall_t* _s, MemeInteger_t _pos, const MemeByte_t* _buf, MemeInteger_t _buflen)
{
    assert(_s);
    assert(_buf);

    //if (_pos < 0 || _pos > MemeStringSmall_byteSize(_s))
    //    return MMENO__POSIX_OFFSET(EINVAL);

    //if (_buflen <= 0)
    //    return 0;

    //if (_buflen > MemeStringSmall_byteCapacity(_s))
    //    return MMENO__POSIX_OFFSET(E2BIG);

    memmove(_s->buffer_ + _pos + _buflen, _s->buffer_ + _pos, MemeStringSmall_byteSize(_s) - _pos);
    memcpy (_s->buffer_ + _pos, _buf, _buflen);
    _s->capacity_ -= (uint8_t)_buflen;
    _s->buffer_[MemeStringSmall_byteSize(_s)] = 0;

    return 0;
}

inline MemeInteger_t MemeStringSmall_remove(
	MemeStringSmall_t* _s, MemeInteger_t _pos, MemeInteger_t _count)
{
    assert(_s);
	
    if (_pos >= MemeStringSmall_byteSize(_s))
        return 0;
	
    if (_count < 0)
        _count = MemeStringSmall_byteSize(_s) - _pos;
	
    if (_pos + _count > MemeStringSmall_byteSize(_s))
        _count = MemeStringSmall_byteSize(_s) - _pos;

    memmove(_s->buffer_ + _pos, _s->buffer_ + _pos + _count, MemeStringSmall_byteSize(_s) - _pos - _count);
    _s->capacity_ += (uint8_t)_count;
    _s->buffer_[MemeStringSmall_byteSize(_s)] = 0;

    return 0;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !MEME_IMPL_STRING_P_SMALL_H_INCLUDED
