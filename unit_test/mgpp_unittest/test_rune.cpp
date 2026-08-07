#include <catch2/catch.hpp>
#include <megopp/strs/rune.h>
#include <cstring>

TEST_CASE("rune - default construction creates empty rune", "[rune]")
{
    mgpp::strs::rune r;
    REQUIRE(r.empty());
    REQUIRE(r.size() == 0);
    REQUIRE(r.valid());
}

TEST_CASE("rune - construct from single char", "[rune]")
{
    mgpp::strs::rune r('A');
    REQUIRE_FALSE(r.empty());
    REQUIRE(r.size() == 1);
    REQUIRE(r.data()[0] == 'A');
    REQUIRE(r.is_en_char());
}

TEST_CASE("rune - construct from UTF-8 bytes", "[rune]")
{
    const uint8_t data[] = {0xE4, 0xBD, 0xA0}; // U+4F60 '你'
    mgpp::strs::rune r(data, 3);
    REQUIRE_FALSE(r.empty());
    REQUIRE(r.size() == 3);
    REQUIRE(memcmp(r.data(), data, 3) == 0);
}

TEST_CASE("rune - assign from char", "[rune]")
{
    mgpp::strs::rune r;
    r = 'Z';
    REQUIRE(r.size() == 1);
    REQUIRE(r.data()[0] == 'Z');
}

TEST_CASE("rune - assign from const char*", "[rune]")
{
    mgpp::strs::rune r;
    r = "ABC";
    REQUIRE(r.size() == 3);
    REQUIRE(memcmp(r.data(), "ABC", 3) == 0);
}

TEST_CASE("rune - equality comparison", "[rune]")
{
    mgpp::strs::rune a('X');
    mgpp::strs::rune b('X');
    mgpp::strs::rune c('Y');
    REQUIRE(a == b);
    REQUIRE(a != c);
    REQUIRE(a == 'X');
    REQUIRE(a != 'Y');
}

TEST_CASE("rune - comparison with const char*", "[rune]")
{
    mgpp::strs::rune r('A');
    REQUIRE(r == "A");
    REQUIRE_FALSE(r == "B");
}

TEST_CASE("rune - ordering operators", "[rune]")
{
    mgpp::strs::rune a('A');
    mgpp::strs::rune b('B');
    REQUIRE(a < b);
    REQUIRE(a <= b);
    REQUIRE(b > a);
    REQUIRE(b >= a);
}

TEST_CASE("rune - clear resets state", "[rune]")
{
    mgpp::strs::rune r('X');
    REQUIRE_FALSE(r.empty());
    r.clear();
    REQUIRE(r.empty());
    REQUIRE(r.size() == 0);
}

TEST_CASE("rune - copy construction", "[rune]")
{
    mgpp::strs::rune orig('Q');
    mgpp::strs::rune copy(orig);
    REQUIRE(copy == 'Q');
    REQUIRE(copy.size() == orig.size());
}

TEST_CASE("rune - move construction", "[rune]")
{
    mgpp::strs::rune orig('K');
    mgpp::strs::rune moved(std::move(orig));
    REQUIRE(moved == 'K');
}

TEST_CASE("rune - copy assignment", "[rune]")
{
    mgpp::strs::rune a('X');
    mgpp::strs::rune b('Y');
    b = a;
    REQUIRE(b == 'X');
}

TEST_CASE("rune - bool conversion", "[rune]")
{
    mgpp::strs::rune empty;
    mgpp::strs::rune nonempty('A');
    REQUIRE_FALSE(static_cast<bool>(empty));
    REQUIRE(static_cast<bool>(nonempty));
}

TEST_CASE("rune - native_handle access", "[rune]")
{
    mgpp::strs::rune r('A');
    auto& native = r.native_handle();
    REQUIRE(native.byte[0] == 'A');
}
