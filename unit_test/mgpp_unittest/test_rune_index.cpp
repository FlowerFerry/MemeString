#include <catch2/catch.hpp>
#include <megopp/strs/rune_index.h>
#include <cstring>

TEST_CASE("rune_index - construct from ptr and size", "[rune_index]")
{
    const uint8_t data[] = {0xE4, 0xBD, 0xA0};
    mgpp::strs::rune_index ri(data, 3);
    REQUIRE(ri.data() == data);
    REQUIRE(ri.size() == 3);
}

TEST_CASE("rune_index - copy construction", "[rune_index]")
{
    const uint8_t data[] = {0xE4, 0xBD, 0xA0};
    mgpp::strs::rune_index ri1(data, 3);
    mgpp::strs::rune_index ri2(ri1);
    REQUIRE(ri2.data() == data);
    REQUIRE(ri2.size() == 3);
}

TEST_CASE("rune_index - move construction", "[rune_index]")
{
    const uint8_t data[] = {0xE4, 0xBD, 0xA0};
    mgpp::strs::rune_index ri1(data, 3);
    mgpp::strs::rune_index ri2(std::move(ri1));
    REQUIRE(ri2.data() == data);
    REQUIRE(ri2.size() == 3);
}

TEST_CASE("rune_index - native_handle access", "[rune_index]")
{
    const uint8_t data[] = {'A'};
    mgpp::strs::rune_index ri(data, 1);
    auto& native = ri.native_handle();
    REQUIRE(native.data == data);
    REQUIRE(native.size == 1);
}

TEST_CASE("rune_index - zero size", "[rune_index]")
{
    mgpp::strs::rune_index ri(nullptr, 0);
    REQUIRE(ri.size() == 0);
}
