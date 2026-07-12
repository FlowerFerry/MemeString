
#ifndef MEMEPP_TO_STRING_SRCIMPL_HPP_INCLUDED
#define MEMEPP_TO_STRING_SRCIMPL_HPP_INCLUDED

#include <meme/string.h>
#include <memepp/string_def.hpp>
#include <memepp/to_string_def.hpp>
#include <utility>

namespace memepp {
inline namespace MMPP_NAMESPACE {

    MEMEPP__IMPL_INLINE string to_string(int16_t _value, int _radix)
    {
        mmstrstk_t stack;
        int result = MemeStringStack_fromInt16(_value, _radix, &stack, sizeof(stack));
        if (result)
            return string{};

        return string{ std::move(stack) };
    }

    MEMEPP__IMPL_INLINE string to_string(int32_t _value, int _radix)
    {
        mmstrstk_t stack;
        int result = MemeStringStack_fromInt32(_value, _radix, &stack, sizeof(stack));
        if (result)
            return string{};

        return string{ std::move(stack) };
    }

    MEMEPP__IMPL_INLINE string to_string(int64_t _value, int _radix)
    {
        mmstrstk_t stack;
        int result = MemeStringStack_fromInt64(_value, _radix, &stack, sizeof(stack));
        if (result)
            return string{};

        return string{ std::move(stack) };
    }

    MEMEPP__IMPL_INLINE string to_string(uint16_t _value, int _radix)
    {
        mmstrstk_t stack;
        int result = MemeStringStack_fromUInt16(_value, _radix, &stack, sizeof(stack));
        if (result)
            return string{};

        return string{ std::move(stack) };
    }

    MEMEPP__IMPL_INLINE string to_string(uint32_t _value, int _radix)
    {
        mmstrstk_t stack;
        int result = MemeStringStack_fromUInt32(_value, _radix, &stack, sizeof(stack));
        if (result)
            return string{};

        return string{ std::move(stack) };
    }

    MEMEPP__IMPL_INLINE string to_string(uint64_t _value, int _radix)
    {
        mmstrstk_t stack;
        int result = MemeStringStack_fromUInt64(_value, _radix, &stack, sizeof(stack));
        if (result)
            return string{};

        return string{ std::move(stack) };
    }

} // namespace MMPP_NAMESPACE
} // namespace memepp

#endif // !MEMEPP_TO_STRING_SRCIMPL_HPP_INCLUDED
