
#ifndef MEMEPP_CONVERT_ASIO_HPP_INCLUDED
#define MEMEPP_CONVERT_ASIO_HPP_INCLUDED

#include <asio/buffer.hpp> 
#include <memepp/variable_buffer_def.hpp>

namespace memepp {

ASIO_NODISCARD inline asio::ASIO_MUTABLE_BUFFER
    to_asio_buffer(memepp::variable_buffer& _buf) ASIO_NOEXCEPT 
{
    return asio::ASIO_MUTABLE_BUFFER(
        _buf.size() ? _buf.data() : nullptr,
        _buf.size()
#if defined(ASIO_ENABLE_BUFFER_DEBUGGING)
    //   , asio::detail::buffer_debug_check<
    //       memepp::variable_buffer::iterator
    //     >(_buf.begin())
#endif
    );
}

ASIO_NODISCARD inline asio::ASIO_CONST_BUFFER
    to_asio_buffer(const memepp::variable_buffer& _buf) ASIO_NOEXCEPT 
{
    return asio::ASIO_CONST_BUFFER(
        _buf.size() ? _buf.data() : nullptr,
        _buf.size()
#if defined(ASIO_ENABLE_BUFFER_DEBUGGING)
    //   , asio::detail::buffer_debug_check<
    //       memepp::variable_buffer::const_iterator
    //     >(_buf.begin())
#endif
    );
}

ASIO_NODISCARD inline asio::ASIO_CONST_BUFFER
    to_asio_buffer(
        memepp::variable_buffer& _buf, 
        mmint_t max_size_in_bytes) ASIO_NOEXCEPT
{
    return asio::ASIO_CONST_BUFFER(
        _buf.size() ? _buf.data() : nullptr,
        (std::min)(_buf.size(), max_size_in_bytes)
#if defined(ASIO_ENABLE_BUFFER_DEBUGGING)
    //   , asio::detail::buffer_debug_check<
    //       memepp::variable_buffer::const_iterator
    //     >(_buf.begin())
#endif
    );
}

ASIO_NODISCARD inline asio::ASIO_CONST_BUFFER
    to_asio_buffer(
        const memepp::variable_buffer& _buf, 
        mmint_t max_size_in_bytes) ASIO_NOEXCEPT
{
    return asio::ASIO_CONST_BUFFER(
        _buf.size() ? _buf.data() : nullptr,
        (std::min)(_buf.size(), max_size_in_bytes)
#if defined(ASIO_ENABLE_BUFFER_DEBUGGING)
    //   , asio::detail::buffer_debug_check<
    //       memepp::variable_buffer::const_iterator
    //     >(_buf.begin())
#endif
    );
}

}

#endif // !MEMEPP_CONVERT_ASIO_HPP_INCLUDED
