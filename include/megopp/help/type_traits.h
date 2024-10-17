
#ifndef MEGOPP_AUXILIARY_NULL_TYPETRAITS_HPP_INCLUDED
#define MEGOPP_AUXILIARY_NULL_TYPETRAITS_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <stdint.h>

#include <utility>
#include <type_traits>

#include <megopp/predef/namespace_alias.h>

namespace mgpp {

template <typename T>
struct is_pair : std::false_type {};

template <typename T, typename U>
struct is_pair<std::pair<T, U>> : std::true_type {};

template <typename T>
constexpr bool is_pair_v = is_pair<T>::value;



template<typename _IntTy, _IntTy _Value, typename _EnumTy, _EnumTy _First, _EnumTy... _Rest>
struct is_value_in_enum
{
    static constexpr bool value = 
        static_cast<std::underlying_type_t<_EnumTy> >(_Value) == static_cast<std::underlying_type_t<_EnumTy>>(_First) ||
        is_value_in_enum<_IntTy, _Value, _EnumTy, _Rest...>::value;
};

template<typename _IntTy, _IntTy _Value, typename _EnumTy, _EnumTy _First>
struct is_value_in_enum<_IntTy, _Value, _EnumTy, _First>
{
    static constexpr bool value =
        static_cast<std::underlying_type_t<_EnumTy>>(_Value) == static_cast<std::underlying_type_t<_EnumTy>>(_First);
};

template<typename _IntTy, _IntTy _Value, typename _EnumTy, _EnumTy _First, _EnumTy... _Rest>
constexpr bool is_value_in_enum_v = is_value_in_enum<_IntTy, _Value, _EnumTy, _First, _Rest...>::value;



template<size_t _bytes>
struct type_with_size {};

template<>
struct type_with_size<1>
{
	typedef int8_t  sint;
	typedef uint8_t uint;
};

template<>
struct type_with_size<2>
{
	typedef int16_t  sint;
	typedef uint16_t uint;
};

template<>
struct type_with_size<4>
{
	typedef int32_t  sint;
	typedef uint32_t uint;
	typedef float    floating;
};

template<>
struct type_with_size<8>
{
	typedef int64_t  sint;
	typedef uint64_t uint;
	typedef double   floating;
};

template<size_t _ByteSize>
using type_by_size = type_with_size<_ByteSize>;

template<size_t _BeginBit, size_t _BitSize>
using type_by_bit_size = type_with_size<(_BeginBit + _BitSize + 7) / 8>;

} // namespace megopp

#endif // MEGOPP_AUXILIARY_NULL_TYPETRAITS_HPP_INCLUDED
