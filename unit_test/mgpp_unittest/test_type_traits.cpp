
#include <catch2/catch.hpp>

#include <type_traits>

template<typename _EnumTy, _EnumTy _First, _EnumTy... _Rest>
struct __is_value_in_enum_checker
{
    template<typename _IntTy>
    static constexpr bool check(_IntTy value)
    {
        return static_cast<std::underlying_type_t<_EnumTy>>(value) == static_cast<std::underlying_type_t<_EnumTy>>(_First) ||
            __is_value_in_enum_checker<_EnumTy, _Rest...>::check(value);
    }
};

template<typename _EnumTy, _EnumTy _First>
struct __is_value_in_enum_checker<_EnumTy, _First>
{
    template<typename _IntTy>
    static constexpr bool check(_IntTy value)
    {
        return static_cast<std::underlying_type_t<_EnumTy>>(value) == static_cast<std::underlying_type_t<_EnumTy>>(_First);
    }
};

template<typename _IntTy, _IntTy _Value, typename _EnumTy, _EnumTy _First, _EnumTy... _Rest>
constexpr bool is_value_in_enum_v = __is_value_in_enum_checker<_EnumTy, _First, _Rest...>::check(_Value);


enum class Test01_01 {
    A = 1,
    B = 2,
    C = 3,
    D = 4,
    E = 5,
    F = 6,
    G = 7,
    H = 8,
    I = 9,
    J = 10,
    K = 11,
    L = 12,
    M = 13,
    N = 14,
    O = 15,
    P = 16,
    Q = 17,
    R = 18,
    S = 19,
    T = 20,
    U = 21,
    V = 22,
    W = 23,
    X = 24,
    Y = 25,
    Z = 26
};

enum class Test01_02 {
    A = 0,
    B = 1,
};

template<Test01_01 _Value>
constexpr bool is_enum_value_in_test01_enum_v = is_value_in_enum_v<Test01_01, _Value,
    Test01_01, Test01_01::A, Test01_01::B, Test01_01::C, Test01_01::D, Test01_01::E, Test01_01::F, Test01_01::G, Test01_01::H, Test01_01::I, Test01_01::J, Test01_01::K, Test01_01::L, Test01_01::M, Test01_01::N, Test01_01::O, Test01_01::P, Test01_01::Q, Test01_01::R, Test01_01::S, Test01_01::T, Test01_01::U, Test01_01::V, Test01_01::W, Test01_01::X, Test01_01::Y, Test01_01::Z>;

template<typename _IntTy, _IntTy _Value>
constexpr bool is_value_in_test01_enum_v = is_value_in_enum_v<_IntTy, _Value,
    Test01_01, Test01_01::A, Test01_01::B, Test01_01::C, Test01_01::D, Test01_01::E, Test01_01::F, Test01_01::G, Test01_01::H, Test01_01::I, Test01_01::J, Test01_01::K, Test01_01::L, Test01_01::M, Test01_01::N, Test01_01::O, Test01_01::P, Test01_01::Q, Test01_01::R, Test01_01::S, Test01_01::T, Test01_01::U, Test01_01::V, Test01_01::W, Test01_01::X, Test01_01::Y, Test01_01::Z>;

TEST_CASE("mgpp::type_traits - 01", "is_value_in_enum")
{
    REQUIRE(is_enum_value_in_test01_enum_v<Test01_01::A> == true);
    
    REQUIRE(is_value_in_test01_enum_v<int, 1> == true);
    REQUIRE(is_value_in_test01_enum_v<int, 0> == false);
    
    REQUIRE(is_value_in_test01_enum_v<Test01_02, Test01_02::A> == false);
    REQUIRE(is_value_in_test01_enum_v<Test01_02, Test01_02::B> == true);
}
