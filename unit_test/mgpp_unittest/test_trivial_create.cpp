#include <catch2/catch.hpp>

#include <megopp/help/trivial_create.h>

#include <string>
#include <cstdint>
#include <type_traits>

struct TrivialPod {
    int    x;
    float  y;
    int    z;
};
static_assert(std::is_trivial<TrivialPod>::value, "TrivialPod must be trivial");

struct NonTrivial {
    std::string s;
    int i = 42;
};
static_assert(!std::is_trivial<NonTrivial>::value, "NonTrivial must not be trivial");

TEST_CASE("mgpp::help::trivial_create - int is zero-initialized", "[trivial_create]")
{
    REQUIRE(mgpp::help::trivial_create<int>() == 0);
}

TEST_CASE("mgpp::help::trivial_create - float is zero-initialized", "[trivial_create]")
{
    REQUIRE(mgpp::help::trivial_create<float>() == 0.0f);
}

TEST_CASE("mgpp::help::trivial_create - uint64_t is zero-initialized", "[trivial_create]")
{
    REQUIRE(mgpp::help::trivial_create<uint64_t>() == 0ULL);
}

TEST_CASE("mgpp::help::trivial_create - trivial struct fields are all zeroed", "[trivial_create]")
{
    TrivialPod pod = mgpp::help::trivial_create<TrivialPod>();
    REQUIRE(pod.x == 0);
    REQUIRE(pod.y == 0.0f);
    REQUIRE(pod.z == 0);
}

TEST_CASE("mgpp::help::trivial_create - non-trivial type falls back to T{}", "[trivial_create]")
{
    NonTrivial obj = mgpp::help::trivial_create<NonTrivial>();
    REQUIRE(obj.s.empty());
}
