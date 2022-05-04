
#ifndef MEME_IMPL_STRING_P_LARGE_H_INCLUDED
#define MEME_IMPL_STRING_P_LARGE_H_INCLUDED

#include "meme/impl/string.h"
#include "meme/impl/string_p__small.h"
#include "meme/impl/atomic.h"

#include <assert.h>
#include <errno.h>

inline void MemeStringLarge_RefCount_init(volatile MemeStringLarge_RefCounted_t* _refcount);
inline MemeInteger_t MemeStringLarge_RefCount_increment(volatile MemeStringLarge_RefCounted_t* _refcount);
//inline MemeInteger_t MemeStringLarge_RefCount_decrement(volatile MemeStringLarge_RefCounted_t* _refcount);
inline MemeInteger_t MemeStringLarge_RefCount_decrementAndDestruct(
	volatile MemeStringLarge_RefCounted_t* _refcount);

inline int MemeStringLarge_init (
	MemeStringLarge_t* _s, MemeString_MallocFunction_t* _cfn, MemeString_FreeFunction_t* _dfn,
	uint8_t _front_capacity, size_t _capacity);
inline int MemeStringLarge_initByOther(
	MemeStringLarge_t* _s, const MemeStringLarge_t* _other);
inline int MemeStringLarge_initByU8bytes (
	MemeStringLarge_t* _s, const uint8_t* _buf, size_t _len,
	MemeString_MallocFunction_t* _cfn, MemeString_FreeFunction_t* _dfn,
	uint8_t _front_capacity, size_t _capacity);
inline int MemeStringLarge_unInit(MemeStringLarge_t* _s);
inline int MemeStringLarge_reset (MemeStringLarge_t* _s);



inline void MemeStringLarge_RefCount_init(volatile MemeStringLarge_RefCounted_t * _refcount)
{
	assert(_refcount != NULL); 
	MemeAtomicInteger_init(&(_refcount->count_), 1);
}

inline MemeInteger_t MemeStringLarge_RefCount_increment(volatile MemeStringLarge_RefCounted_t * _refcount)
{
	assert(_refcount != NULL);

	return MemeAtomicInteger_increment(&(_refcount->count_));
}
//
//inline MemeInteger_t MemeStringLarge_RefCount_decrement(volatile MemeStringLarge_RefCounted_t * _refcount)
//{
//	MemeString_FreeFunction_t* f_func = NULL;
//	MemeInteger_t result = 0;
//
//	assert(_refcount != NULL);
//
//	result = MemeAtomicInteger_decrement(&(_refcount->count_));
//	if ( result == 0 )
//	{
//		if (MemeStringImpl_default() != _refcount->real_)
//		{
//			if (_refcount->free_fn_)
//				_refcount->free_fn_(_refcount->real_);
//			else {
//				f_func = MemeString_getFreeFunction();
//				f_func(_refcount->real_);
//			}
//		}
//		_refcount->data_ = (uint8_t*)MemeStringImpl_default();
//		_refcount->real_ = (uint8_t*)MemeStringImpl_default();
//	}
//	return result;
//}

inline MemeInteger_t MemeStringLarge_RefCount_decrementAndDestruct(
	volatile MemeStringLarge_RefCounted_t * _refcount)
{
	MemeString_FreeFunction_t* f_func = NULL;
	MemeInteger_t result = 0;

	assert(_refcount != NULL);

	result = MemeAtomicInteger_decrement(&(_refcount->count_));
	if (result == 0)
	{
		if (_refcount->free_fn_)
			f_func = _refcount->free_fn_;
		else {
			f_func = MemeString_getFreeFunction();
		}
		if (MemeStringImpl_default() != _refcount->real_)
		{
			f_func(_refcount->real_);
		}
		f_func((void*)_refcount);
	}
	return result;
}

inline int MemeStringLarge_init (
	MemeStringLarge_t* _s, MemeString_MallocFunction_t* _cfn, MemeString_FreeFunction_t* _dfn,
	uint8_t _front_capacity, size_t _capacity)
{
	MemeString_MallocFunction_t* c_func = NULL;
	MemeString_FreeFunction_t  * d_func = NULL;
	MemeStringLarge_RefCounted_t* ref   = NULL;

	_capacity = 0; // Can not be used, first so deal with

	assert(_s != NULL);
	assert(_front_capacity + _capacity > 0);

	memset(_s, 0, sizeof(MemeStringStack_t));
	_s->type_   = MemeString_ImplType_large;
	_s->offset_ = 0;

	c_func = (_cfn ? _cfn : MemeString_getMallocFunction());
	if (c_func == _cfn) {
		if (_dfn == NULL)
			return -EINVAL;
		d_func = _dfn;
	}
	else {
		d_func = MemeString_getFreeFunction();
	}

	ref = (MemeStringLarge_RefCounted_t*)c_func(sizeof(MemeStringLarge_RefCounted_t));
	if (!ref)
		return -ENOMEM;
	MemeStringLarge_RefCount_init(ref);
	ref->malloc_fn_ = c_func;
	ref->free_fn_   = d_func;
	ref->real_ = (uint8_t*)c_func(_front_capacity + _capacity);
	if (!ref->real_) {
		d_func(ref);
		return -ENOMEM;
	}
	ref->data_ = ref->real_ + _front_capacity;

	if (_s->ref_) {
		d_func(ref->real_);
		d_func(ref);
		return -EPERM;
	}
	_s->ref_ = ref;
	return 0;
}

inline int MemeStringLarge_initByOther(
	MemeStringLarge_t* _s, const MemeStringLarge_t* _other)
{
	assert(_s != NULL		&& MemeStringLarge_initByOther);
	assert(_other != NULL	&& MemeStringLarge_initByOther);

	MemeStringLarge_RefCount_increment(_other->ref_);
	memcpy(_s, _other, MEME_STRING__OBJECT_SIZE);
	return 0;
}

inline int MemeStringLarge_initByU8bytes(
	MemeStringLarge_t* _s, const uint8_t* _buf, size_t _len,
	MemeString_MallocFunction_t* _cfn, MemeString_FreeFunction_t* _dfn, 
	uint8_t _front_capacity, size_t _capacity)
{
	MemeString_MallocFunction_t* c_func = NULL;
	MemeString_FreeFunction_t* d_func   = NULL;
	MemeStringLarge_RefCounted_t* ref   = NULL;

	_capacity = 0; // Can not be used, first so deal with

	size_t total_length = _front_capacity + _capacity + _len;

	assert(_s != NULL);
	assert(_buf != NULL);
	assert(total_length);

	total_length = (total_length / sizeof(void*)) * (sizeof(void*) + 1);

	memset(_s, 0, sizeof(MemeStringStack_t));

	c_func = (_cfn ? _cfn : MemeString_getMallocFunction());
	if (c_func == _cfn) {
		if (_dfn == NULL)
			return -EINVAL;
		d_func = _dfn;
	}
	else {
		d_func = MemeString_getFreeFunction();
	}

	ref = (MemeStringLarge_RefCounted_t*)c_func(sizeof(MemeStringLarge_RefCounted_t));
	if (!ref)
		return -ENOMEM;
	MemeStringLarge_RefCount_init(ref);
	ref->malloc_fn_ = c_func;
	ref->free_fn_ = d_func;
	ref->real_ = (uint8_t*)c_func(total_length);
	if (!ref->real_) {
		d_func(ref);
		return -ENOMEM;
	}
	ref->data_ = ref->real_ + _front_capacity;
	memcpy(ref->data_, _buf, _len);
	ref->data_[_len] = '\0';

	if (_s->ref_) {
		d_func(ref->real_);
		d_func(ref);
		return -EPERM;
	}
	_s->ref_ = ref;
	_s->type_ = MemeString_ImplType_large;
	_s->offset_ = 0;
	_s->size_ = _len;
	return 0;
}

inline int MemeStringLarge_unInit(MemeStringLarge_t* _s)
{
	assert(_s != NULL);

	MemeStringLarge_RefCount_decrementAndDestruct(_s->ref_);
	return 0;
}

inline int MemeStringLarge_reset(MemeStringLarge_t* _s)
{
	assert(_s != NULL);

	MemeStringLarge_RefCount_decrementAndDestruct(_s->ref_);
	MemeStringSmall_stackReset((MemeStringStack_t*)_s);
	return 0;
}

#endif // !MEME_IMPL_STRING_P_LARGE_H_INCLUDED
