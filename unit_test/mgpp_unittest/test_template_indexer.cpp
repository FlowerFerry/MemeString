#include <catch2/catch.hpp>
#include <megopp/util/template/indexer.h>

using mgpp::util::template_indexer;

TEST_CASE("template_indexer - index of first type is 0", "[template_indexer]")
{
    constexpr int idx = template_indexer<int, float, double>::index<int>();
    static_assert(idx == 0, "index of int must be 0");
    REQUIRE(idx == 0);
}

TEST_CASE("template_indexer - index of second type is 1", "[template_indexer]")
{
    constexpr int idx = template_indexer<int, float, double>::index<float>();
    static_assert(idx == 1, "index of float must be 1");
    REQUIRE(idx == 1);
}

TEST_CASE("template_indexer - index of third type is 2", "[template_indexer]")
{
    constexpr int idx = template_indexer<int, float, double>::index<double>();
    static_assert(idx == 2, "index of double must be 2");
    REQUIRE(idx == 2);
}

TEST_CASE("template_indexer - unknown type returns -1", "[template_indexer]")
{
    constexpr int idx = template_indexer<int, float, double>::index<char>();
    static_assert(idx == -1, "unknown type must be -1");
    REQUIRE(idx == -1);
}

TEST_CASE("template_indexer - single-type list: found and not-found", "[template_indexer]")
{
    constexpr int idx0 = template_indexer<int>::index<int>();
    constexpr int idx1 = template_indexer<int>::index<float>();
    REQUIRE(idx0 == 0);
    REQUIRE(idx1 == -1);
}

TEST_CASE("template_indexer - duplicate types: first occurrence wins", "[template_indexer]")
{
    // int appears twice; the first match (index 0) should be returned
    constexpr int idx = template_indexer<int, int, double>::index<int>();
    REQUIRE(idx == 0);
}
