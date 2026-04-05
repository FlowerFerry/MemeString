
#include <catch2/catch.hpp>

#include <megopp/help/type_traits.h>
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

TEST_CASE("mgpp::type_traits is_value_in_enum", "[type_traits]")
{
    REQUIRE(is_enum_value_in_test01_enum_v<Test01_01::A> == true);
    
    REQUIRE(is_value_in_test01_enum_v<int, 1> == true);
    REQUIRE(is_value_in_test01_enum_v<int, 0> == false);
    
    REQUIRE(is_value_in_test01_enum_v<Test01_02, Test01_02::A> == false);
    REQUIRE(is_value_in_test01_enum_v<Test01_02, Test01_02::B> == true);
}

TEST_CASE("mgpp::is_pair - non-pair types return false", "[type_traits]")
{
    REQUIRE(mgpp::is_pair<int>::value == false);
    REQUIRE(mgpp::is_pair<std::string>::value == false);
    REQUIRE(mgpp::is_pair<double>::value == false);
    REQUIRE(mgpp::is_pair_v<float> == false);
}

TEST_CASE("mgpp::is_pair - std::pair specializations return true", "[type_traits]")
{
    REQUIRE(mgpp::is_pair<std::pair<int, int>>::value == true);
    REQUIRE(mgpp::is_pair<std::pair<std::string, double>>::value == true);
    REQUIRE(mgpp::is_pair_v<std::pair<int, float>> == true);
}

TEST_CASE("mgpp::type_with_size - 1-byte types", "[type_traits]")
{
    REQUIRE((std::is_same<mgpp::type_with_size<1>::sint,  int8_t>::value));
    REQUIRE((std::is_same<mgpp::type_with_size<1>::uint,  uint8_t>::value));
}

TEST_CASE("mgpp::type_with_size - 2-byte types", "[type_traits]")
{
    REQUIRE((std::is_same<mgpp::type_with_size<2>::sint,  int16_t>::value));
    REQUIRE((std::is_same<mgpp::type_with_size<2>::uint,  uint16_t>::value));
}

TEST_CASE("mgpp::type_with_size - 4-byte types", "[type_traits]")
{
    REQUIRE((std::is_same<mgpp::type_with_size<4>::sint,     int32_t>::value));
    REQUIRE((std::is_same<mgpp::type_with_size<4>::uint,     uint32_t>::value));
    REQUIRE((std::is_same<mgpp::type_with_size<4>::floating, float>::value));
}

TEST_CASE("mgpp::type_with_size - 8-byte types", "[type_traits]")
{
    REQUIRE((std::is_same<mgpp::type_with_size<8>::sint,     int64_t>::value));
    REQUIRE((std::is_same<mgpp::type_with_size<8>::uint,     uint64_t>::value));
    REQUIRE((std::is_same<mgpp::type_with_size<8>::floating, double>::value));
}

TEST_CASE("mgpp::function_traits - arity and result_type", "[type_traits]")
{
    using Traits1 = mgpp::function_traits<int(float, double)>;
    REQUIRE(Traits1::arity == 2);
    REQUIRE((std::is_same<Traits1::result_type, int>::value));

    using Traits2 = mgpp::function_traits<void()>;
    REQUIRE(Traits2::arity == 0);
    REQUIRE((std::is_same<Traits2::result_type, void>::value));
}

TEST_CASE("mgpp::function_traits - individual argument types", "[type_traits]")
{
    using Traits = mgpp::function_traits<double(int, float, char)>;
    REQUIRE((std::is_same<Traits::arg<0>::type, int>::value));
    REQUIRE((std::is_same<Traits::arg<1>::type, float>::value));
    REQUIRE((std::is_same<Traits::arg<2>::type, char>::value));
}
