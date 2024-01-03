
#include <meme/impl/string_p__medium.h>


int
MemeStringMedium_canBeAppendIt(const MemeStringMedium_t* _s, MemeInteger_t _buflen)
{
	assert(_s);

	if (MemeStringMedium_availableByteCapacity(_s) < _buflen)
		return (MGEC__2BIG);

	return 0;
}

const uint8_t*
MemeStringMedium_constData(const MemeStringMedium_t* _s)
{
	return _s->real_ + _s->front_capacity_;
}

uint8_t* MemeStringMedium_data(MemeStringMedium_t* _s)
{
	return _s->real_ + _s->front_capacity_;
}

uint8_t* MemeStringMedium_iteratorEnd(MemeStringMedium_t* _s)
{
	return MemeStringMedium_data(_s) + _s->size_;
}

MemeInteger_t MemeStringMedium_maxByteSize(const MemeStringMedium_t* _s)
{
	return SIZE_MAX >> sizeof(size_t);
}

MemeInteger_t MemeStringMedium_realByteSize(const MemeStringMedium_t* _s)
{
	return _s->front_capacity_ + _s->size_ + _s->capacity_;
}

MemeInteger_t
MemeStringMedium_maxByteCapacity(const MemeStringMedium_t* _s)
{
	return _s->size_ + MemeStringMedium_availableByteCapacity(_s);
}

MemeInteger_t MemeStringMedium_availableByteCapacity(const MemeStringMedium_t* _s)
{
	return _s->capacity_ - 1;
}

mmint_t MemeStringMedium_usedByteFrontCapacity(const MemeStringMedium_t* _s)
{
	return MMS__GET_MEDIUM_FRONT_CAPACITY_MAX_VALUE - _s->front_capacity_;
}

void MemeStringMedium_byteSizeOffset(MemeStringMedium_t* _s, MemeInteger_t _offset)
{
	_s->size_ += _offset;
	_s->capacity_ -= _offset;
}

void MemeStringMedium_byteSizeOffsetAndSetZero(MemeStringMedium_t* _s, MemeInteger_t _offset)
{
	MemeStringMedium_byteSizeOffset(_s, _offset);
	*(MemeStringMedium_iteratorEnd(_s)) = '\0';
}

int MemeStringMedium_appendWithByte(MemeStringMedium_t* _s, MemeInteger_t _count, MemeByte_t _byte)
{
	MemeByte_t* pointer = NULL;
	if (MemeStringMedium_availableByteCapacity(_s) < _count)
	{
		int result = MemeStringImpl_capacityExpansionWithModifiable(
			(MemeStringStack_t*)_s, _s->size_ + _count);
		if (result)
			return result;
	}

	pointer = MemeStringMedium_iteratorEnd(_s);
	MemeStringMedium_byteSizeOffset(_s, _count);
	for (; 0 <= --_count; ) {
		*pointer++ = _byte;
	}
	*(MemeStringMedium_iteratorEnd(_s)) = '\0';
	return 0;
}

int
MemeStringMedium_appendWithBytes(
	MemeStringMedium_t* _s, const MemeByte_t* _buf, MemeInteger_t _buflen)
{
	if (MemeStringMedium_availableByteCapacity(_s) < _buflen)
	{
		int result = MemeStringImpl_capacityExpansionWithModifiable(
			(MemeStringStack_t*)_s, _s->size_ + _buflen);
		if (result)
			return result;
	}

	memcpy(MemeStringMedium_iteratorEnd(_s), _buf, _buflen);
	MemeStringMedium_byteSizeOffsetAndSetZero(_s, _buflen);
	return 0;
}

int
MemeStringMedium_insertWithBytes(
	MemeStringMedium_t* _s, MemeInteger_t _pos, const MemeByte_t* _buf, MemeInteger_t _buflen)
{
	if (_pos + _buflen <= (MemeInteger_t)_s->front_capacity_)
	{
		if (_pos != 0)
			memmove(MemeStringMedium_data(_s) - _buflen, MemeStringMedium_data(_s), _buflen);
		memcpy(MemeStringMedium_data(_s) + _pos - _buflen, _buf, _buflen);
		_s->front_capacity_ -= _buflen;
		_s->size_ += _buflen;
		return 0;
	}

	if (MemeStringMedium_availableByteCapacity(_s) < _buflen)
	{
		int result = MemeStringImpl_capacityExpansionWithModifiable(
			(MemeStringStack_t*)_s, _s->size_ + _buflen);
		if ((result != 0))
			return result;
	}

	memmove(MemeStringMedium_data(_s) + _pos + _buflen, MemeStringMedium_data(_s) + _pos, _s->size_ - _pos);
	memcpy(MemeStringMedium_data(_s) + _pos, _buf, _buflen);
	MemeStringMedium_byteSizeOffsetAndSetZero(_s, _buflen);
	return 0;
}

int
MemeStringMedium_initWithCapacity(
	MemeStringMedium_t* _s, MemeInteger_t _capacity)
{
	MemeInteger_t front_capacity = 0;
	//MemeString_MallocFunction_t* c_func = MemeString_getMallocFunction();

	++_capacity;
	front_capacity = (_capacity / (sizeof(size_t) * 10)) * sizeof(size_t);

	_capacity = (_capacity % sizeof(size_t)) == 0 ?
		(_capacity ? _capacity : sizeof(size_t)) : ((_capacity / sizeof(size_t) + 1) * sizeof(size_t));

	if (front_capacity > MMS__GET_MEDIUM_FRONT_CAPACITY_MAX_VALUE)
		front_capacity = MMS__GET_MEDIUM_FRONT_CAPACITY_MAX_VALUE;

	_s->real_ = mmsmem_malloc(front_capacity + _capacity);
	if (!(_s->real_))
		return (MGEC__NOMEM);

	_s->size_ = 0;
	_s->type_ = MemeString_StorageType_medium;
	_s->front_capacity_ = front_capacity;
	_s->capacity_ = _capacity;
	MemeStringMedium_data(_s)[0] = '\0';

	return 0;
}

int
MemeStringMedium_assign(MemeStringMedium_t* _s, const MemeByte_t* _buf, MemeInteger_t _len)
{
	if (MemeStringMedium_maxByteCapacity(_s) < _len)
	{
		int result = MemeStringImpl_capacityExpansionWithModifiable(
			(MemeStringStack_t*)_s, _len);
		if (result)
			return result;
	}

	MemeStringMedium_clear(_s);

	if (_len == 0) {
		return 0;
	}

	MemeStringMedium_data(_s)[_len] = '\0';
	memcpy(MemeStringMedium_data(_s), _buf, _len);
	MemeStringMedium_byteSizeOffset(_s, _len);
	return 0;
}

MemeInteger_t
MemeStringMedium_remove(
	MemeStringMedium_t* _s, MemeInteger_t _pos, MemeInteger_t _count)
{
	if (_pos >= (MemeInteger_t)_s->size_)
		return 0;

	if (_count < 0)
		_count = _s->size_ - _pos;

	if (_pos + _count > (MemeInteger_t)_s->size_)
		_count = _s->size_ - _pos;

	if (_pos == 0) {
		if (_count > MemeStringMedium_usedByteFrontCapacity(_s))
		{
			memmove(MemeStringMedium_data(_s), MemeStringMedium_data(_s) + _count, _s->size_ - _count);
			MemeStringMedium_byteSizeOffsetAndSetZero(_s, -_count);
		}
		else {
			_s->front_capacity_ += _count;
			_s->size_ -= _count;
		}
	}
	else {

		memmove(MemeStringMedium_data(_s) + _pos, MemeStringMedium_data(_s) + _pos + _count, _s->size_ - _pos - _count);
		MemeStringMedium_byteSizeOffsetAndSetZero(_s, -_count);
	}

	return 0;
}

int MemeStringMedium_resizeWithByte(MemeStringMedium_t* _s, MemeInteger_t _size, MemeByte_t _byte)
{
	if (_size < 0)
		return 0;

	if ((size_t)_size <= _s->size_) {
		//_s->capacity_ += _s->size_ - _size;
		//_s->size_ = _size;
		MemeStringMedium_byteSizeOffsetAndSetZero(_s, _size - _s->size_);
	}
	else {
		if (MemeStringMedium_maxByteCapacity(_s) < _size)
		{
			int result = MemeStringImpl_capacityExpansionWithModifiable((MemeStringStack_t*)_s, _size);
			if (result)
				return result;
		}

		return MemeStringMedium_appendWithByte(_s, _size - _s->size_, _byte);
	}
	return 0;
}

int MemeStringMedium_capacityExpansion(MemeStringMedium_t* _s, MemeInteger_t _minSizeRequest)
{
	MemeInteger_t dstlen = 0;
	//MemeString_ReallocFunction_t* realloc_func = MemeString_getReallocFunction();
	MemeByte_t* new_pointer = NULL;

	dstlen = (MemeInteger_t)(MemeStringMedium_maxByteCapacity(_s) * 1.667);
	if (dstlen < _minSizeRequest) {
		dstlen = (MemeInteger_t)(_minSizeRequest * 1.667);
	}
	dstlen = dstlen + _s->front_capacity_ + 1;
	dstlen = (dstlen % sizeof(size_t)) == 0 ?
		dstlen : ((dstlen / sizeof(size_t) + 1) * sizeof(size_t));

	new_pointer = mmsmem_realloc(_s->real_, dstlen);
	if (new_pointer == NULL)
		return (MGEC__NOMEM);

	_s->real_ = new_pointer;
	_s->capacity_ = dstlen - _s->front_capacity_ - _s->size_;
	return 0;
}

int MemeStringMedium_clear(MemeStringMedium_t* _s)
{
	_s->capacity_ += _s->size_;
	_s->size_ = 0;
	MemeStringMedium_data(_s)[0] = '\0';
	return 0;
}

int
MemeStringMedium_unInit(MemeStringMedium_t* _s)
{
	//MemeString_FreeFunction_t* d_func = MemeString_getFreeFunction();

	if (_s->real_) {
		mmsmem_free(_s->real_);
		_s->real_ = NULL;
	}

	return 0;
}

int MemeStringMedium_reset(MemeStringMedium_t* _s)
{
	MemeStringMedium_unInit(_s);
	MemeStringSmall_stackReset((MemeStringStack_t*)_s);
	return 0;
}

void MemeStringMedium_shrinkTailZero(MemeStringMedium_t* _s)
{
	while (MemeStringMedium_constData(_s)[_s->size_] == 0 && _s->size_ > 0
		&& MemeStringMedium_constData(_s)[_s->size_ - 1] == 0)
		MemeStringMedium_byteSizeOffset(_s, -1);
}

MemeInteger_t MemeStringMedium_checkHeadTailMemory(const MemeStringMedium_t* _s)
{
#if !(MMOPT__HEADTAIL_MEMCHECK_ENABLED)
	return 1;
#else
	return MemeCheck_calibrate(_s->real_);
#endif
}
