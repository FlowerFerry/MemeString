
#ifndef MEME_IMPL_STRING_P_USER_H_INCLUDED
#define MEME_IMPL_STRING_P_USER_H_INCLUDED

#include <meme/impl/atomic.h>
#include <meme/impl/string.h>
#include <meme/impl/string_p__small.h>
#include <assert.h>

inline void MemeStringUser_RefCount_init(volatile MemeStringUser_RefCounted_t* _refcount);
inline MemeInteger_t MemeStringUser_RefCount_increment(volatile MemeStringUser_RefCounted_t* _refcount);
inline MemeInteger_t MemeStringUser_RefCount_decrementAndDestruct(
	volatile MemeStringUser_RefCounted_t* _refcount);

inline int MemeStringUser_initTakeOver(MemeStringUser_t* _s,
	MemeString_MallocFunction_t* _cfn, MemeString_FreeFunction_t* _dfn,
	void* _user_data,
	size_t _strlen,
	MemeString_UserObjectDestruct_t* _destruct_fn,
	MemeString_UserObjectData_t* _data_fn);
inline int MemeStringUser_unInit(MemeStringUser_t* _s);
inline int MemeStringUser_reset (MemeStringUser_t* _s);
inline const char* MemeStringUser_cStr(MemeStringUser_t* _s);


inline int MemeStringUser_initTakeOver(MemeStringUser_t* _s,
	MemeString_MallocFunction_t* _cfn, MemeString_FreeFunction_t* _dfn,
	void* _user_data,
	size_t _strlen,
	MemeString_UserObjectDestruct_t* _destruct_fn,
	MemeString_UserObjectData_t* _data_fn)
{
	MemeString_MallocFunction_t* c_func = NULL;
	MemeString_FreeFunction_t* d_func = NULL;
	MemeStringUser_RefCounted_t* refCount = NULL;
	int result = 0;

	assert(_s != NULL			&& "function is" && MemeStringUser_initTakeOver);
	assert(_user_data != NULL	&& "function is" && MemeStringUser_initTakeOver);
	assert(_destruct_fn != NULL && "function is" && MemeStringUser_initTakeOver);
	assert(_data_fn != NULL		&& "function is" && MemeStringUser_initTakeOver);

	result = MemeStringMemory_allocFunctionSwitch(_cfn, _dfn, &c_func, &d_func);
	if (result)
		return result;

	memset(_s, 0, sizeof(MemeStringStack_t));

	refCount = (MemeStringUser_RefCounted_t*)c_func(sizeof(MemeStringUser_RefCounted_t));
	if (!refCount)
		return -ENOMEM;
	MemeStringUser_RefCount_init(refCount);
	refCount->malloc_fn_ = c_func;
	refCount->free_fn_   = d_func;
	refCount->data_fn_   = _data_fn;
	refCount->destruct_fn_ = _destruct_fn;
	refCount->user_data_ = _user_data;
	
	if (_s->ref_) {
		d_func(refCount);
		return -EPERM;
	}
	_s->ref_  = refCount;
	_s->size_ = _strlen;
	_s->type_ = MemeString_ImplType_user;

	return 0;
}

inline int MemeStringUser_unInit(MemeStringUser_t* _s)
{
	return MemeStringUser_RefCount_decrementAndDestruct(_s->ref_);
}

inline int MemeStringUser_reset(MemeStringUser_t* _s)
{
	int result = MemeStringUser_RefCount_decrementAndDestruct(_s->ref_);
	if (result)
		return result;
	MemeStringSmall_stackReset((MemeStringStack_t*)_s);
	return 0;
}

inline const char* MemeStringUser_cStr(MemeStringUser_t* _s)
{
	const char* p = _s->ref_->data_fn_(_s->ref_->user_data_);
	return p ? p : (const char*)MemeStringImpl_default();
}

inline void MemeStringUser_RefCount_init(volatile MemeStringUser_RefCounted_t* _refcount)
{
	assert(_refcount != NULL && MemeStringUser_RefCount_init);
	MemeAtomicInteger_init(&(_refcount->count_), 1);
}

inline MemeInteger_t MemeStringUser_RefCount_increment(volatile MemeStringUser_RefCounted_t* _refcount)
{
	assert(_refcount != NULL && MemeStringUser_RefCount_increment);

	return MemeAtomicInteger_increment(&(_refcount->count_));
}

inline MemeInteger_t MemeStringUser_RefCount_decrementAndDestruct(volatile MemeStringUser_RefCounted_t* _refcount)
{
	MemeString_FreeFunction_t* f_func = NULL;
	MemeInteger_t result = 0;

	assert(_refcount != NULL);

	result = MemeAtomicInteger_decrement(&(_refcount->count_));
	if (result == 0)
	{
		_refcount->destruct_fn_(_refcount->user_data_);

		if (_refcount->free_fn_)
			f_func = _refcount->free_fn_;
		else {
			f_func = MemeString_getFreeFunction();
		}
		f_func((void*)_refcount);
	}
	return result;
}

#endif // !MEME_IMPL_STRING_P_SMALL_H_INCLUDED