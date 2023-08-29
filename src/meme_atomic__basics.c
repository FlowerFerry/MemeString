
#include <meme/impl/atomic.h>

void MemeAtomicInteger_init(volatile MemeAtomicInteger_t * _obj, MemeInteger_t _desired)
{
#if MEGO_OS__WINDOWS__AVAILABLE
	MEME_WINDOWS__INTER_LOCKED_EXCHANGE(_obj, _desired);
#elif !defined(__STDC_NO_ATOMICS__)
	atomic_exchange(_obj, _desired);
#else
#	error "There is no suitable implementation of the 'MemeAtomicInteger_init' function"
#endif
}

MemeInteger_t MemeAtomicInteger_increment(volatile MemeAtomicInteger_t * _obj)
{
	// memory_order_relaxed
#if MEGO_OS__WINDOWS__AVAILABLE
	return MEME_WINDOWS__INTER_LOCKED_INCREMENT(_obj);
#elif !defined(__STDC_NO_ATOMICS__)
	return atomic_fetch_add(_obj, 1) + 1;
#else
#	error "There is no suitable implementation of the 'MemeAtomicInteger_increment' function"
#endif
}

MemeInteger_t MemeAtomicInteger_decrement(volatile MemeAtomicInteger_t * _obj)
{
	// memory_order_acq_rel
#if MEGO_OS__WINDOWS__AVAILABLE
	return MEME_WINDOWS__INTER_LOCKED_DECREMENT(_obj);
#elif !defined(__STDC_NO_ATOMICS__)
	return atomic_fetch_sub(_obj, 1) - 1;
#else
#	error "There is no suitable implementation of the 'MemeAtomicInteger_decrement' function"
#endif
}


MemeInteger_t MemeAtomicInteger_load(volatile MemeAtomicInteger_t* _obj)
{
#if MEGO_OS__WINDOWS__AVAILABLE
    return MEME_WINDOWS__INTER_LOCKED_LOAD(_obj);
#elif !defined(__STDC_NO_ATOMICS__)
    return atomic_load(_obj);
#else
#	error "There is no suitable implementation of the 'MemeAtomicInteger_load' function"
#endif
}

void MemeAtomicInteger_store(volatile MemeAtomicInteger_t* _obj, MemeInteger_t _desired)
{
#if MEGO_OS__WINDOWS__AVAILABLE
    MEME_WINDOWS__INTER_LOCKED_EXCHANGE(_obj, _desired);
#elif !defined(__STDC_NO_ATOMICS__)
    atomic_store(_obj, _desired);
#else
#	error "There is no suitable implementation of the 'MemeAtomicInteger_store' function"
#endif
}
