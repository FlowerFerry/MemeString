
#ifndef MEMEPP_CONVERT_QCBOR_HPP_INCLUDED
#define MEMEPP_CONVERT_QCBOR_HPP_INCLUDED

#include <qcbor/UsefulBuf.h>

#include "memepp/convert/common_def.hpp"
#include "memepp/string.hpp"
#include <memepp/string_view.hpp>

namespace memepp {

    inline string from(const UsefulBufC& _s)
    {
        return string{ (const char*)_s.ptr, static_cast<mmint_t>(_s.len) };
    }

    inline string_view view(const UsefulBufC& _s)
    {
        return string_view{ (const char*)_s.ptr, static_cast<mmint_t>(_s.len) };
    }

};

inline memepp::string mm_from(const UsefulBufC& _s)
{
    return memepp::from(_s);
}

inline memepp::string_view mm_view(const UsefulBufC& _s)
{
    return memepp::view(_s);
}

#endif // !MEMEPP_CONVERT_QCBOR_HPP_INCLUDED
