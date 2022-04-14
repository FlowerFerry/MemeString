
#ifndef MEME_IMPL_ATOMIC_H_INCLUDED
#define MEME_IMPL_ATOMIC_H_INCLUDED

#include "atomic_fwd.h"

void MemeAtomicInteger_init(volatile MemeAtomicInteger_t* _obj, MemeInteger_t _desired);
MemeInteger_t MemeAtomicInteger_increment(volatile MemeAtomicInteger_t* _obj);
MemeInteger_t MemeAtomicInteger_decrement(volatile MemeAtomicInteger_t* _obj);

#endif // !MEME_IMPL_ATOMIC_H_INCLUDED
