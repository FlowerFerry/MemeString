#include <catch2/catch.hpp>

#include <memepp/rune.hpp>
#include <memepp/string.hpp>

// ---------------------------------------------------------------------------
// rune: equality operators
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune operator== and !=", "[rune]")
{
    memepp::rune a('A');
    memepp::rune b('A');
    memepp::rune c('B');

    REQUIRE(a == b);
    REQUIRE(a != c);
    REQUIRE_FALSE(a != b);
    REQUIRE_FALSE(a == c);
}

TEST_CASE("memepp::rune operator== with const char*", "[rune]")
{
    memepp::rune r('H');
    REQUIRE(r == "H");
    REQUIRE_FALSE(r == "W");
}

TEST_CASE("memepp::rune operator== with char", "[rune]")
{
    memepp::rune r('X');
    REQUIRE(r == 'X');
    REQUIRE_FALSE(r == 'Y');
}

TEST_CASE("memepp::rune operator== with multi-byte rune", "[rune]")
{
    const char zhong[] = { 0xE4, 0xB8, 0xAD, 0 }; // 中
    memepp::rune r((const uint8_t*)zhong, 3);
    REQUIRE(r == zhong);
}

// ---------------------------------------------------------------------------
// rune: is_en_char / is_multi
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune is_multi — Chinese character", "[rune]")
{
    const uint8_t wen[] = { 0xE9, 0x97, 0xAE }; // 问
    memepp::rune r(wen, 3);
    REQUIRE(r.is_multi());
    REQUIRE_FALSE(r.is_en_char());
}

TEST_CASE("memepp::rune is_multi — emoji", "[rune]")
{
    // U+1F600 GRINNING FACE — 4 bytes
    const uint8_t emoji[] = { 0xF0, 0x9F, 0x98, 0x80 };
    memepp::rune r(emoji, 4);
    REQUIRE(r.is_multi());
}

// ---------------------------------------------------------------------------
// rune: operator== with string
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune operator== with memepp::string and !=", "[rune]")
{
    memepp::rune r('H');
    memepp::string s("H");
    REQUIRE(r == s);
    REQUIRE(s == r);

    memepp::string s2("W");
    REQUIRE_FALSE(r == s2);
    REQUIRE_FALSE(s2 == r);

    REQUIRE(r != s2);
    REQUIRE(s2 != r);
}

TEST_CASE("memepp::rune operator== with string_view", "[rune]")
{
    memepp::rune r('H');
    memepp::string_view sv("H");
    REQUIRE(r == sv);
    REQUIRE(sv == r);
}

// ---------------------------------------------------------------------------
// rune: is_ch_punct / is_en_punct / is_space
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune is_space — space chars", "[rune]")
{
    REQUIRE(memepp::rune(' ').is_space());
    REQUIRE(memepp::rune('\t').is_space());
    REQUIRE_FALSE(memepp::rune('A').is_space());
}

TEST_CASE("memepp::rune is_en_punct — ASCII punctuation (extra)", "[rune]")
{
    REQUIRE(memepp::rune('!').is_en_punct());
    REQUIRE(memepp::rune('.').is_en_punct());
    REQUIRE_FALSE(memepp::rune('A').is_en_punct());
}

TEST_CASE("memepp::rune is_ch_punct — Chinese punctuation", "[rune]")
{
    const uint8_t comma[] = { 0xEF, 0xBC, 0x8C }; // ，U+FF0C
    REQUIRE(memepp::rune(comma, 3).is_ch_punct());
    REQUIRE_FALSE(memepp::rune(',').is_ch_punct());
}

// ---------------------------------------------------------------------------
// rune: operator bool
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune operator bool (extra)", "[rune]")
{
    memepp::rune empty;
    REQUIRE(!static_cast<bool>(empty));

    memepp::rune valid('X');
    REQUIRE(static_cast<bool>(valid));
}

// ---------------------------------------------------------------------------
// rune: bytes() const_pointer equality with data()
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune bytes() equals data()", "[rune]")
{
    memepp::rune r('A');
    REQUIRE(r.data() != nullptr);
    REQUIRE(r.size() == 1);
    REQUIRE(r.data()[0] == 'A');
}

// ---------------------------------------------------------------------------
// rune_index: construction and basic properties
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune_index from space byte", "[rune_index]")
{
    const uint8_t space_byte[] = { 0x20 };
    memepp::rune_index ri(space_byte, 1);
    REQUIRE(ri.data() != nullptr);
    REQUIRE(ri.size() == 1);
    REQUIRE(ri.is_space());
}

TEST_CASE("memepp::rune_index from non-space byte", "[rune_index]")
{
    const uint8_t letter[] = { 'A' };
    memepp::rune_index ri(letter, 1);
    REQUIRE(ri.data() != nullptr);
    REQUIRE(ri.size() == 1);
    REQUIRE_FALSE(ri.is_space());
}

TEST_CASE("memepp::rune_index ideographic space", "[rune_index]")
{
    const uint8_t ideo_space[] = { 0xE3, 0x80, 0x80 }; // U+3000 IDEOGRAPHIC SPACE
    memepp::rune_index ri(ideo_space, 3);
    REQUIRE(ri.data() != nullptr);
    REQUIRE(ri.size() == 3);
    REQUIRE(ri.is_space());
}

TEST_CASE("memepp::rune_index copy and move", "[rune_index]")
{
    const uint8_t byte[] = { 'X' };
    memepp::rune_index ri(byte, 1);

    memepp::rune_index ri2(ri);
    REQUIRE(ri2.size() == ri.size());
    REQUIRE(ri2.is_space() == ri.is_space());

    memepp::rune_index ri3(reinterpret_cast<const uint8_t*>("Y"), 1);
    memepp::rune_index ri4(std::move(ri3));
    REQUIRE(ri4.size() == 1);
}
