#include <catch2/catch.hpp>
#include <megopp/util/template_random.h>

using mgpp::util::template_seed;
using mgpp::util::template_random;

TEST_CASE("template_seed - same string produces same seed", "[template_random]")
{
    constexpr unsigned int s1 = template_seed("hello");
    constexpr unsigned int s2 = template_seed("hello");
    static_assert(s1 == s2, "same seed");
    REQUIRE(s1 == s2);
}

TEST_CASE("template_seed - different strings produce different seeds", "[template_random]")
{
    constexpr unsigned int s1 = template_seed("hello");
    constexpr unsigned int s2 = template_seed("world");
    REQUIRE(s1 != s2);
}

TEST_CASE("template_seed - empty string seed is zero", "[template_random]")
{
    constexpr unsigned int s = template_seed("");
    REQUIRE(s == 0u);
}

TEST_CASE("template_seed - multi-string overload is reproducible", "[template_random]")
{
    constexpr unsigned int s1 = template_seed("foo", "bar");
    constexpr unsigned int s2 = template_seed("foo", "bar");
    REQUIRE(s1 == s2);
}

TEST_CASE("template_seed - multi-string differs from reversed order", "[template_random]")
{
    constexpr unsigned int s1 = template_seed("foo", "bar");
    constexpr unsigned int s2 = template_seed("bar", "foo");
    // The chaining order is different, seeds should differ
    REQUIRE(s1 != s2);
}

TEST_CASE("template_random - value is deterministic for same seed", "[template_random]")
{
    constexpr unsigned int v1 = template_random<42>::value;
    constexpr unsigned int v2 = template_random<42>::value;
    static_assert(v1 == v2, "must be same");
    REQUIRE(v1 == v2);
}

TEST_CASE("template_random - next::value differs from value", "[template_random]")
{
    constexpr unsigned int v  = template_random<42>::value;
    constexpr unsigned int vn = template_random<42>::next::value;
    REQUIRE(v != vn);
}

TEST_CASE("template_random - different seeds produce different values", "[template_random]")
{
    constexpr unsigned int v1 = template_random<1>::value;
    constexpr unsigned int v2 = template_random<2>::value;
    REQUIRE(v1 != v2);
}

TEST_CASE("template_random - seed-based chain is consistent", "[template_random]")
{
    constexpr unsigned int seed = template_seed("unit_test");
    constexpr unsigned int v1  = template_random<seed>::value;
    constexpr unsigned int v2  = template_random<seed>::value;
    REQUIRE(v1 == v2);
}
