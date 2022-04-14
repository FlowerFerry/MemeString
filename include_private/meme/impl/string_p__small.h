
#ifndef MEME_IMPL_STRING_P_SMALL_H_INCLUDED
#define MEME_IMPL_STRING_P_SMALL_H_INCLUDED

#include <meme/impl/string.h>
#include <assert.h>

inline int MemeStringSmall_stackReset(MemeStringStack_t* _s);

inline int MemeStringSmall_initByU8bytes(
	MemeStringSmall_t* _s, const MemeByte_t* _utf8, size_t _len);
inline int MemeStringSmall_clear(MemeStringSmall_t* _s);

inline int MemeStringSmall_isEmpty(const MemeStringSmall_t* _s);
inline const char* MemeStringSmall_cStr(const MemeStringSmall_t* _s);
inline const uint8_t* MemeStringSmall_byteData(const MemeStringSmall_t* _s);
inline MemeInteger_t MemeStringSmall_byteSize(const MemeStringSmall_t* _s);
inline MemeInteger_t MemeStringSmall_byteCapacity(const MemeStringSmall_t* _s);

inline MemeInteger_t MemeStringSmall_wordSize(const MemeStringSmall_t* _s);
inline MemeWordIndex_t MemeStringSmall_at(const MemeStringSmall_t* _s, size_t _index);



inline int MemeStringSmall_initByU8bytes(
	MemeStringSmall_t* _s, const MemeByte_t* _utf8, size_t _len)
{
	assert(_s);
	assert(_len <= MEME_STRING__GET_SMALL_BUFFER_SIZE);

	if (_utf8) {
		memcpy(_s->buffer_, _utf8, _len);
		_s->buffer_[_len] = '\0';
		_s->type_ = MemeString_ImplType_small;
		_s->capacity_ = (uint8_t)(MEME_STRING__GET_SMALL_BUFFER_SIZE - _len);
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
	_s->capacity_ = MEME_STRING__GET_SMALL_BUFFER_SIZE;
	return 0;
}

inline int MemeStringSmall_isEmpty(const MemeStringSmall_t * _s)
{
	assert(_s);
	return MEME_STRING__GET_SMALL_BUFFER_SIZE == _s->capacity_;
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
	return MEME_STRING__GET_SMALL_BUFFER_SIZE - _s->capacity_;
}

inline MemeInteger_t MemeStringSmall_byteCapacity(const MemeStringSmall_t * _s)
{
	assert(_s);
	return _s->capacity_;
}

#endif // !MEME_IMPL_STRING_P_SMALL_H_INCLUDED
