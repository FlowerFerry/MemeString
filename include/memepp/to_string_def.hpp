
#ifndef MEMEPP_TO_STRING_DEF_HPP_INCLUDED
#define MEMEPP_TO_STRING_DEF_HPP_INCLUDED

#include <memepp/string_fwd.hpp>
#include <memepp/common.hpp>

namespace memepp {
inline namespace MMPP_NAMESPACE {

    //! \brief Create a string from a signed 16-bit integer.
    memepp::string to_string(int16_t _value, int _radix = 10);

    //! \brief Create a string from a signed 32-bit integer.
    memepp::string to_string(int32_t _value, int _radix = 10);

    //! \brief Create a string from a signed 64-bit integer.
    memepp::string to_string(int64_t _value, int _radix = 10);

    //! \brief Create a string from an unsigned 16-bit integer.
    memepp::string to_string(uint16_t _value, int _radix = 10);

    //! \brief Create a string from an unsigned 32-bit integer.
    memepp::string to_string(uint32_t _value, int _radix = 10);

    //! \brief Create a string from an unsigned 64-bit integer.
    memepp::string to_string(uint64_t _value, int _radix = 10);

} // namespace MMPP_NAMESPACE
} // namespace memepp

#endif // !MEMEPP_TO_STRING_DEF_HPP_INCLUDED
