
#ifndef MEGOPP_THRD_NUMERIC_ID_H_INCLUDED
#define MEGOPP_THRD_NUMERIC_ID_H_INCLUDED

#include <mego/thrd/numeric_id.h>

namespace mgpp {
namespace thrd {

inline size_t numeric_id() noexcept
{
    static thread_local const size_t tid = __mgthrd_numeric_id();
    return tid;
}

}
}

#endif // !MEGOPP_THRD_NUMERIC_ID_H_INCLUDED
