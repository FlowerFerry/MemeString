
#include <meme/impl/string_p__large.h>
// #include <mego/mem/pool.h>

void MemeStringLarge_RefCount_init(volatile MemeStringLarge_RefCounted_t* _refcount)
{
	assert(_refcount != NULL);
	MemeAtomicInteger_init(&(_refcount->count_), 1);
}

MemeInteger_t MemeStringLarge_RefCount_increment(volatile MemeStringLarge_RefCounted_t* _refcount)
{
	assert(_refcount != NULL);

	return MemeAtomicInteger_increment(&(_refcount->count_));
}

MemeInteger_t MemeStringLarge_RefCount_decrementAndDestruct(
	volatile MemeStringLarge_RefCounted_t* _refcount)
{
	MemeString_FreeFunction_t* f_func = NULL;
	MemeInteger_t result = 0;

	assert(_refcount != NULL);

	result = MemeAtomicInteger_decrement(&(_refcount->count_));
	if (result == 0)
	{
		//if (_refcount->free_fn_)
		//	f_func = _refcount->free_fn_;
		//else {
		//	f_func = MemeString_getFreeFunction();
		//}
		f_func = mmsmem_get_free_func();

		if (MemeStringImpl_default() != _refcount->real_)
		{
			f_func(_refcount->real_);
		}
		f_func((void*)_refcount);
	}
	return result;
}

int MemeStringLarge_init(
	MemeStringLarge_t* _s, MemeString_MallocFunction_t* _cfn, MemeString_FreeFunction_t* _dfn,
	uint8_t _front_capacity, size_t _capacity)
{
	MemeString_MallocFunction_t* c_func = NULL;
	MemeString_FreeFunction_t* d_func = NULL;
	MemeStringLarge_RefCounted_t* refCount = NULL;

	_capacity = 0; // Can not be used, first so deal with

	assert(_s != NULL);
	assert(_front_capacity + _capacity > 0);

	memset(_s, 0, sizeof(MemeStringStack_t));
	_s->type_ = MemeString_ImplType_large;
	_s->offset_ = 0;

	c_func = (_cfn ? _cfn : mmsmem_get_malloc_func());
	if (c_func == _cfn) {
		if (_dfn == NULL)
			return MGEC__INVAL;
		d_func = _dfn;
	}
	else {
		d_func = mmsmem_get_free_func();
	}

	refCount = (MemeStringLarge_RefCounted_t*)c_func(sizeof(MemeStringLarge_RefCounted_t));
	if (!refCount)
		return MGEC__NOMEM;
	MemeStringLarge_RefCount_init(refCount);
	//ref->malloc_fn_ = c_func;
	//ref->free_fn_   = d_func;
	refCount->real_ = (uint8_t*)c_func(_front_capacity + _capacity);
	if (!refCount->real_) {
		d_func(refCount);
		return MGEC__NOMEM;
	}

	if (_s->ref_) {
		d_func(refCount->real_);
		d_func(refCount);
		return MGEC__PERM;
	}
	_s->ref_ = refCount;
	return 0;
}

int MemeStringLarge_initByOther(
	MemeStringLarge_t* _s, const MemeStringLarge_t* _other)
{
	assert(_s != NULL && MemeStringLarge_initByOther);
	assert(_other != NULL && MemeStringLarge_initByOther);

	MemeStringLarge_RefCount_increment(_other->ref_);
	memcpy(_s, _other, MEME_STRING__OBJECT_SIZE);
	return 0;
}

int MemeStringLarge_initByU8bytes(
	MemeStringLarge_t* _s, const uint8_t* _buf, size_t _len,
	MemeString_MallocFunction_t* _cfn, MemeString_FreeFunction_t* _dfn,
	uint8_t _front_capacity, size_t _capacity)
{
	MemeString_MallocFunction_t* c_func = NULL;
	MemeString_FreeFunction_t* d_func = NULL;
	MemeStringLarge_RefCounted_t* refCount = NULL;

	_capacity = 0; // Can not be used, first so deal with

	size_t total_length = _front_capacity + _capacity + _len;

	assert(_s != NULL);
	assert(_buf != NULL);
	assert(total_length);

	total_length = ((total_length / sizeof(void*)) + 1) * (sizeof(void*));

	memset(_s, 0, sizeof(MemeStringStack_t));

	if (_cfn) {
		if ((_dfn == NULL))
			return MGEC__INVAL;
		c_func = _cfn;
		d_func = _dfn;
	}
	else {
		c_func = mmsmem_get_malloc_func();
		d_func = mmsmem_get_free_func();
	}

	//c_func = (_cfn ? _cfn : MemeString_getMallocFunction());
	//if (c_func == _cfn) {
	//	if (_dfn == NULL)
	//		return -EINVAL;
	//	d_func = _dfn;
	//}
	//else {
	//	d_func = MemeString_getFreeFunction();
	//}

	refCount = (MemeStringLarge_RefCounted_t*)c_func(sizeof(MemeStringLarge_RefCounted_t));
	if (!refCount)
		return (MGEC__NOMEM);
	MemeStringLarge_RefCount_init(refCount);
	//ref->malloc_fn_ = c_func;
	//ref->free_fn_ = d_func;
	refCount->real_ = (uint8_t*)c_func(total_length);
	if (!refCount->real_) {
		d_func(refCount);
		return (MGEC__NOMEM);
	}


	//ref->data_ = ref->real_ + _front_capacity;
	//memcpy(ref->data_, _buf, _len);
	//ref->data_[_len] = '\0';

	memcpy(MemeStringLarge_RefCount_data(refCount), _buf, _len);
	MemeStringLarge_RefCount_data(refCount)[_len] = '\0';


	//if (_s->ref_) {
	//	d_func(refCount->real_);
	//	d_func(refCount);
	//	return -EPERM;
	//}

	_s->ref_ = refCount;
	_s->type_ = MemeString_ImplType_large;
	_s->offset_ = 0;
	_s->size_ = _len;
	return 0;
}

int MemeStringLarge_unInit(MemeStringLarge_t* _s)
{
	assert(_s != NULL);

	MemeStringLarge_RefCount_decrementAndDestruct(_s->ref_);
	return 0;
}

int MemeStringLarge_reset(MemeStringLarge_t* _s)
{
	assert(_s != NULL);

	MemeStringLarge_RefCount_decrementAndDestruct(_s->ref_);
	MemeStringSmall_stackReset((MemeStringStack_t*)_s);
	return 0;
}

int MemeStringLarge_initAndTakeover(
	MemeStringLarge_t* _s,
	MemeByte_t* _real, MemeInteger_t _realSize,
	MemeInteger_t _offset, MemeInteger_t _size
)
{
	//MemeString_MallocFunction_t* c_func = MemeString_getMallocFunction();
	//MemeString_FreeFunction_t* d_func = MemeString_getFreeFunction();
	MemeStringLarge_RefCounted_t* refCount = NULL;

	refCount = (MemeStringLarge_RefCounted_t*)mmsmem_malloc(sizeof(MemeStringLarge_RefCounted_t));
	if (!refCount)
		return (MGEC__NOMEM);
	MemeStringLarge_RefCount_init(refCount);
	refCount->real_ = _real;

	_s->ref_ = refCount;
	_s->type_ = MemeString_ImplType_large;
	_s->offset_ = _offset;
	_s->size_ = _size;

	return 0;
}

//void MemeStringLarge_setOffset(MemeStringLarge_t* _s, MemeInteger_t _offset)
//{
//	_s->offset_ += _offset;
//	_s->size_ -= _offset;
//}

void MemeStringLarge_shrinkTailZero(MemeStringLarge_t* _s)
{
	while (MemeStringLarge_constData(_s)[_s->size_] == 0 && _s->size_ > 0
		&& MemeStringLarge_constData(_s)[_s->size_ - 1] == 0)
		--(_s->size_);
}

//const uint8_t* MemeStringLarge_constData(const MemeStringLarge_t* _s)
//{
//	return _s->ref_->real_ + _s->offset_;
//}

uint8_t* MemeStringLarge_RefCount_data(volatile MemeStringLarge_RefCounted_t* _refcount)
{
	return _refcount->real_;
}

//uint8_t* MemeStringLarge_data(MemeStringLarge_t* _s)
//{
//	return _s->ref_->real_;
//}


MemeInteger_t MemeStringLarge_getSharedHeapByteSize(const MemeStringLarge_t* _s)
{
	return sizeof(MemeStringLarge_RefCounted_t) + _s->offset_ + _s->size_;
}

MemeInteger_t MemeStringLarge_getPrivateHeapByteSize(const MemeStringLarge_t* _s)
{
	return 0;
}

MemeInteger_t MemeStringLarge_checkHeadTailMemory(const MemeStringLarge_t* _s)
{
#if !(MMOPT__HEADTAIL_MEMCHECK_ENABLED)
	return 1;
#else
	if (!(_s->ref_))
		return 1;

	if (MemeCheck_calibrate(_s->ref_) == 0)
		return 0;
	return MemeCheck_calibrate(_s->ref_->real_);
#endif
}
