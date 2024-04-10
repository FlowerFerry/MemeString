
#ifndef MEGOPP_NUMERIC_TYPES_H_INCLUDED
#define MEGOPP_NUMERIC_TYPES_H_INCLUDED

#include <stdint.h>
#include <mego/numeric/types.h>

namespace mgpp {

enum class int_neg_mode_t : uint8_t
{
    standard = mg_int_neg_mode__standard,
    sign_bit = mg_int_neg_mode__sign_bit
};

}

#endif // !MEGOPP_NUMERIC_TYPES_H_INCLUDED
