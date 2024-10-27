
#ifndef MEME_IMPL_CONC_STD_ATOMIC_H_INCLUDED
#define MEME_IMPL_CONC_STD_ATOMIC_H_INCLUDED

#include <meme/impl/conc/std_atomic_fwd.h>
#include <mego/predef/symbol/inline.h>

MEME_EXTERN_C_SCOPE_START

MG_CAPI_INLINE enum memory_order mmconc_memory_order_to_std_order(mmconc_memory_order_e _order)
{
    switch (_order)
    {
    case mmconc_memory_order_relaxed: return memory_order_relaxed;
    case mmconc_memory_order_consume: return memory_order_consume;
    case mmconc_memory_order_acquire: return memory_order_acquire;
    case mmconc_memory_order_release: return memory_order_release;
    case mmconc_memory_order_acq_rel: return memory_order_acq_rel;
    case mmconc_memory_order_seq_cst: return memory_order_seq_cst;
    default: return memory_order_seq_cst;
    }
}


MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_IMPL_CONC_ATOMIC_STDWRAP_H_INCLUDED
