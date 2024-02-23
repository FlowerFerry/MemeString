
#ifndef MEMEPP_METATYPE_FWD_HPP_INCLUDED
#define MEMEPP_METATYPE_FWD_HPP_INCLUDED

#include <meme/variant_fwd.h>

namespace memepp {
namespace meta {
    
    enum class typid : mmmeta_typid_t
    {
        invalid    = MMMETA_TYPID__INVALID,
        null       = MMMETA_TYPID__NULL,
        byte       = MMMETA_TYPID__BYTE,
        char_t     = MMMETA_TYPID__CHAR,
        wchar      = MMMETA_TYPID__WCHAR,
        int64      = MMMETA_TYPID__INT64,
        uint64     = MMMETA_TYPID__UINT64,
        double_t   = MMMETA_TYPID__DOUBLE,
        string     = MMMETA_TYPID__STRING,
        buffer     = MMMETA_TYPID__BUFFER,
        varbuf     = MMMETA_TYPID__VARBUF,
        rune       = MMMETA_TYPID__RUNE,
        voidptr    = MMMETA_TYPID__VOIDPTR,
        user       = MMMETA_TYPID__USER
    };

}
}

#endif // !MEMEPP_METATYPE_FWD_HPP_INCLUDED
