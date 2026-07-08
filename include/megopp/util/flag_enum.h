#ifndef MEGOPP_UTIL_FLAG_ENUM_H_INCLUDED
#define MEGOPP_UTIL_FLAG_ENUM_H_INCLUDED

#include <type_traits>
#include <bitset>

namespace mgpp {
namespace util {

// ============================================================
// Flag Enum marker trait
// ============================================================

template <typename _Enum>
struct is_flag_enum : std::false_type {};

template <typename _Enum>
inline constexpr bool is_flag_enum_v = is_flag_enum<_Enum>::value;

// ============================================================
// SFINAE helper
// ============================================================

template <typename _Enum>
using enable_if_flag_t = std::enable_if_t<is_flag_enum_v<_Enum>, int>;

// ============================================================
// Bitwise operators (Global Namespace for ADL / Unqualified Lookup)
// ============================================================
} // namespace util
} // namespace mgpp

template <typename _Enum, mgpp::util::enable_if_flag_t<_Enum> = 0>
inline constexpr _Enum operator|(_Enum _lhs, _Enum _rhs) noexcept
{
    using _U = std::underlying_type_t<_Enum>;
    return static_cast<_Enum>(static_cast<_U>(_lhs) | static_cast<_U>(_rhs));
}

template <typename _Enum, mgpp::util::enable_if_flag_t<_Enum> = 0>
inline constexpr _Enum operator&(_Enum _lhs, _Enum _rhs) noexcept
{
    using _U = std::underlying_type_t<_Enum>;
    return static_cast<_Enum>(static_cast<_U>(_lhs) & static_cast<_U>(_rhs));
}

template <typename _Enum, mgpp::util::enable_if_flag_t<_Enum> = 0>
inline constexpr _Enum operator^(_Enum _lhs, _Enum _rhs) noexcept
{
    using _U = std::underlying_type_t<_Enum>;
    return static_cast<_Enum>(static_cast<_U>(_lhs) ^ static_cast<_U>(_rhs));
}

template <typename _Enum, mgpp::util::enable_if_flag_t<_Enum> = 0>
inline constexpr _Enum operator~(_Enum _val) noexcept
{
    using _U = std::underlying_type_t<_Enum>;
    return static_cast<_Enum>(~static_cast<_U>(_val));
}

template <typename _Enum, mgpp::util::enable_if_flag_t<_Enum> = 0>
inline constexpr _Enum& operator|=(_Enum& _lhs, _Enum _rhs) noexcept
{
    _lhs = _lhs | _rhs;
    return _lhs;
}

template <typename _Enum, mgpp::util::enable_if_flag_t<_Enum> = 0>
inline constexpr _Enum& operator&=(_Enum& _lhs, _Enum _rhs) noexcept
{
    _lhs = _lhs & _rhs;
    return _lhs;
}

template <typename _Enum, mgpp::util::enable_if_flag_t<_Enum> = 0>
inline constexpr _Enum& operator^=(_Enum& _lhs, _Enum _rhs) noexcept
{
    _lhs = _lhs ^ _rhs;
    return _lhs;
}

namespace mgpp {
namespace util {

// ============================================================
// Helper query functions
// ============================================================

/// Check if any bit is set (not 0)
template <typename _Enum, enable_if_flag_t<_Enum> = 0>
inline constexpr bool has_any_flag(_Enum _val) noexcept
{
    return static_cast<std::underlying_type_t<_Enum>>(_val) != 0;
}

/// Check if all bits are unset (is 0)
template <typename _Enum, enable_if_flag_t<_Enum> = 0>
inline constexpr bool has_no_flag(_Enum _val) noexcept
{
    return !has_any_flag(_val);
}

/// Check if _val contains all bits from _flags
template <typename _Enum, enable_if_flag_t<_Enum> = 0>
inline constexpr bool has_flag(_Enum _val, _Enum _flags) noexcept
{
    return (_val & _flags) == _flags;
}

/// Check if _val and _flags have any common bits (intersection is not empty)
template <typename _Enum, enable_if_flag_t<_Enum> = 0>
inline constexpr bool has_any_flag(_Enum _val, _Enum _flags) noexcept
{
    return (_val & _flags) != static_cast<_Enum>(0);
}

/// Check if _val and _flags have no common bits (intersection is empty)
template <typename _Enum, enable_if_flag_t<_Enum> = 0>
inline constexpr bool has_no_flag(_Enum _val, _Enum _flags) noexcept
{
    return !has_any_flag(_val, _flags);
}

// ============================================================
// Extended utilities: Value mutators and converters
// ============================================================

/// Convert an enum to its underlying type
template <typename _Enum, enable_if_flag_t<_Enum> = 0>
inline constexpr auto to_underlying(_Enum _val) noexcept
{
    return static_cast<std::underlying_type_t<_Enum>>(_val);
}

/// Set or clear a flag based on a condition
template <typename _Enum, enable_if_flag_t<_Enum> = 0>
inline constexpr _Enum& set_flag_if(_Enum& _lhs, _Enum _rhs, bool _condition) noexcept
{
    return _condition ? (_lhs |= _rhs) : (_lhs &= ~_rhs);
}

/// Toggle a flag
template <typename _Enum, enable_if_flag_t<_Enum> = 0>
inline constexpr _Enum& toggle_flag(_Enum& _lhs, _Enum _rhs) noexcept
{
    return _lhs ^= _rhs;
}

/// Add flags in place
template <typename _Enum, enable_if_flag_t<_Enum> = 0>
inline constexpr _Enum& add_flags(_Enum& _val, _Enum _flags) noexcept
{
    return _val |= _flags;
}

/// Remove flags in place
template <typename _Enum, enable_if_flag_t<_Enum> = 0>
inline constexpr _Enum& remove_flags(_Enum& _val, _Enum _flags) noexcept
{
    return _val &= ~_flags;
}

/// Keep only the specified flags in place
template <typename _Enum, enable_if_flag_t<_Enum> = 0>
inline constexpr _Enum& keep_only_flags(_Enum& _val, _Enum _mask) noexcept
{
    return _val &= _mask;
}

// ============================================================
// Extended utilities: Bit analysis
// ============================================================

/// Extract the lowest set flag
template <typename _Enum, enable_if_flag_t<_Enum> = 0>
inline constexpr _Enum extract_lowest_flag(_Enum _val) noexcept
{
    using _U = std::underlying_type_t<_Enum>;
    _U _v = to_underlying(_val);
    return static_cast<_Enum>(_v & (~_v + 1));
}

/// Check if exactly one flag is set
template <typename _Enum, enable_if_flag_t<_Enum> = 0>
inline constexpr bool has_single_flag(_Enum _val) noexcept
{
    using _U = std::underlying_type_t<_Enum>;
    _U _v = to_underlying(_val);
    return _v != 0 && (_v & (_v - 1)) == 0;
}

/// Count how many flags are set
template <typename _Enum, enable_if_flag_t<_Enum> = 0>
inline constexpr std::size_t count_flags(_Enum _val) noexcept
{
    using _U = std::underlying_type_t<_Enum>;
    return std::bitset<sizeof(_U) * 8>(to_underlying(_val)).count();
}

} // namespace util
} // namespace mgpp

#endif // !MEGOPP_UTIL_FLAG_ENUM_H_INCLUDED