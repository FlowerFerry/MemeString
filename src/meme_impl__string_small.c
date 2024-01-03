
#include <meme/impl/string_p__small.h>


int MemeStringSmall_initByU8bytes(
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

int MemeStringSmall_stackReset(MemeStringStack_t* _s)
{
	assert(_s);

	MemeStringSmall_clear((MemeStringSmall_t*)_s);
	return 0;
}

int MemeStringSmall_clear(MemeStringSmall_t* _s)
{
	assert(_s);

	_s->buffer_[0] = 0;
	_s->type_ = MemeString_ImplType_small;
	_s->capacity_ = MMS__GET_SMALL_BUFFER_SIZE;
	return 0;
}

int MemeStringSmall_isEmpty(const MemeStringSmall_t* _s)
{
	assert(_s);
	return MMS__GET_SMALL_BUFFER_SIZE == _s->capacity_;
}

const char* MemeStringSmall_cStr(const MemeStringSmall_t* _s)
{
	assert(_s);
	return (const char*)_s->buffer_;
}

const uint8_t* MemeStringSmall_byteData(const MemeStringSmall_t* _s)
{
	assert(_s);
	return _s->buffer_;
}

MemeInteger_t MemeStringSmall_byteSize(const MemeStringSmall_t* _s)
{
	assert(_s);
	return MMS__GET_SMALL_BUFFER_SIZE - _s->capacity_;
}

MemeInteger_t MemeStringSmall_byteCapacity(const MemeStringSmall_t* _s)
{
	assert(_s);
	return _s->capacity_;
}

int MemeStringSmall_canBeAppendIt(const MemeStringSmall_t* _s, MemeInteger_t _buflen)
{
	assert(_s);
	if (MemeStringSmall_byteCapacity(_s) < _buflen)
		return (MGEC__2BIG);

	return 0;
}

void MemeStringSmall_byteSizeOffsetAndSetZero(MemeStringSmall_t* _s, MemeInteger_t _offset)
{
	_s->capacity_ -= _offset;
	_s->buffer_[MemeStringSmall_byteSize(_s)] = '\0';
}

void MemeStringSmall_shrinkTailZero(MemeStringSmall_t* _s)
{
	MemeInteger_t index = MemeStringSmall_byteSize(_s);
	while (_s->buffer_[index] == 0 && index > 0 && _s->buffer_[index - 1] == 0)
		--index;
	_s->capacity_ += (MemeStringSmall_byteSize(_s) - index);
}

int MemeStringSmall_resizeWithByte(MemeStringSmall_t* _s, MemeInteger_t _size, MemeByte_t _byte)
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

int MemeStringSmall_appendWithByte(MemeStringSmall_t* _s, MemeInteger_t _count, MemeByte_t _byte)
{
	MemeByte_t* pointer = NULL;

	pointer = _s->buffer_ + MemeStringSmall_byteSize(_s);
	MemeStringSmall_byteSizeOffsetAndSetZero(_s, _count);
	for (; 0 <= --_count; ) {
		*pointer++ = _byte;
	}
	return 0;
}

int MemeStringSmall_appendWithBytes(MemeStringSmall_t* _s, const MemeByte_t* _buf, MemeInteger_t _buflen)
{
	assert(_s);
	assert(_buf);

	memcpy(_s->buffer_ + MemeStringSmall_byteSize(_s), _buf, _buflen);
	_s->capacity_ -= (uint8_t)_buflen;
	_s->buffer_[MemeStringSmall_byteSize(_s)] = 0;

	return 0;
}

int MemeStringSmall_insertWithBytes(
	MemeStringSmall_t* _s, MemeInteger_t _pos, const MemeByte_t* _buf, MemeInteger_t _buflen)
{
	assert(_s);
	assert(_buf);

	memmove(_s->buffer_ + _pos + _buflen, _s->buffer_ + _pos, MemeStringSmall_byteSize(_s) - _pos);
	memcpy(_s->buffer_ + _pos, _buf, _buflen);
	_s->capacity_ -= (uint8_t)_buflen;
	_s->buffer_[MemeStringSmall_byteSize(_s)] = 0;

	return 0;
}

MemeInteger_t MemeStringSmall_remove(
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
