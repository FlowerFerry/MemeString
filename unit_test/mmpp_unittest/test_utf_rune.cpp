#include <catch2/catch.hpp>

#include <memepp/string.hpp>

#include <meme/utf/u8rune.h>
#include <meme/utf/u16rune.h>
#include <meme/utf/u32rune.h>

#include <cstdint>
#include <vector>

// ---------------------------------------------------------------------------
// UTF-16: char_size
// BMP characters occupy one unit; a high surrogate starts a pair (2 units);
// a lone low surrogate is not a lead unit at all (-1).
// ---------------------------------------------------------------------------

TEST_CASE("mmutf_u16rune_char_size", "[utf16][rune]")
{
    SECTION("BMP characters are 1 unit")
    {
        REQUIRE(mmutf_u16rune_char_size(0x0041) == 1);
        REQUIRE(mmutf_u16rune_char_size(0x4E2D) == 1);
        REQUIRE(mmutf_u16rune_char_size(0xFFFF) == 1);
    }

    SECTION("high surrogate starts a pair")
    {
        REQUIRE(mmutf_u16rune_char_size(0xD800) == 2);
        REQUIRE(mmutf_u16rune_char_size(0xDBFF) == 2);
    }

    SECTION("low surrogate is not a lead unit")
    {
        REQUIRE(mmutf_u16rune_char_size(0xDC00) == -1);
        REQUIRE(mmutf_u16rune_char_size(0xDFFF) == -1);
    }
}

// ---------------------------------------------------------------------------
// UTF-16: encode / decode surrogate pairs
// ---------------------------------------------------------------------------

TEST_CASE("mmutf_u16rune_set_u32 / get_u32 roundtrip", "[utf16][rune]")
{
    SECTION("BMP character encodes to one unit")
    {
        uint16_t buf[4] = { 0, 0, 0, 0 };
        int n = mmutf_u16rune_set_u32(buf, 4, 0x4E2D);
        REQUIRE(n == 1);
        REQUIRE(buf[0] == 0x4E2D);

        uint32_t out = 0;
        REQUIRE(mmutf_u16rune_get_u32(buf, n, &out) == 1);
        REQUIRE(out == 0x4E2D);
    }

    SECTION("astral character encodes to a surrogate pair")
    {
        uint16_t buf[4] = { 0, 0, 0, 0 };
        int n = mmutf_u16rune_set_u32(buf, 4, 0x1F600);
        REQUIRE(n == 2);
        REQUIRE(buf[0] == 0xD83D);
        REQUIRE(buf[1] == 0xDE00);

        uint32_t out = 0;
        REQUIRE(mmutf_u16rune_get_u32(buf, n, &out) == 2);
        REQUIRE(out == 0x1F600);
    }

    SECTION("first astral codepoint U+10000")
    {
        uint16_t buf[4] = { 0, 0, 0, 0 };
        int n = mmutf_u16rune_set_u32(buf, 4, 0x10000);
        REQUIRE(n == 2);

        uint32_t out = 0;
        REQUIRE(mmutf_u16rune_get_u32(buf, n, &out) == 2);
        REQUIRE(out == 0x10000);
    }

    SECTION("last astral codepoint U+10FFFF")
    {
        uint16_t buf[4] = { 0, 0, 0, 0 };
        int n = mmutf_u16rune_set_u32(buf, 4, 0x10FFFF);
        REQUIRE(n == 2);
        REQUIRE(buf[0] == 0xDBFF);
        REQUIRE(buf[1] == 0xDFFF);

        uint32_t out = 0;
        REQUIRE(mmutf_u16rune_get_u32(buf, n, &out) == 2);
        REQUIRE(out == 0x10FFFF);
    }

    SECTION("lone surrogate cannot be encoded")
    {
        uint16_t buf[4] = { 0, 0, 0, 0 };
        REQUIRE(mmutf_u16rune_set_u32(buf, 4, 0xD800) == -1);
        REQUIRE(mmutf_u16rune_set_u32(buf, 4, 0xDC00) == -1);
        REQUIRE(mmutf_u16rune_set_u32(buf, 4, 0xDFFF) == -1);
    }

    SECTION("lone surrogate cannot be decoded")
    {
        uint32_t out = 0;
        const uint16_t high[] = { 0xD800 };
        REQUIRE(mmutf_u16rune_get_u32(high, 1, &out) == -1);

        const uint16_t low[] = { 0xDC00 };
        REQUIRE(mmutf_u16rune_get_u32(low, 1, &out) == -1);
    }

    SECTION("truncated pair cannot be decoded")
    {
        uint32_t out = 0;
        const uint16_t pair[] = { 0xD83D };
        REQUIRE(mmutf_u16rune_get_u32(pair, 1, &out) == -1);
    }
}

// ---------------------------------------------------------------------------
// UTF-16: validation
// ---------------------------------------------------------------------------

TEST_CASE("mmutf_u16rune_valid", "[utf16][rune]")
{
    SECTION("single BMP unit is valid")
    {
        const uint16_t buf[] = { 0x4E2D };
        REQUIRE(mmutf_u16rune_valid(buf, 1) == 1);
    }

    SECTION("surrogate pair is valid and reports 2 units")
    {
        const uint16_t buf[] = { 0xD83D, 0xDE00 };
        REQUIRE(mmutf_u16rune_valid(buf, 2) == 2);
    }

    SECTION("lone high surrogate is invalid")
    {
        const uint16_t buf[] = { 0xD800 };
        REQUIRE(mmutf_u16rune_valid(buf, 1) == -1);
    }

    SECTION("lone low surrogate is invalid")
    {
        const uint16_t buf[] = { 0xDC00 };
        REQUIRE(mmutf_u16rune_valid(buf, 1) == -1);
    }
}

// ---------------------------------------------------------------------------
// UTF-32: char_size / valid are unconditional
// UTF-32 stores one codepoint per unit and this layer performs no range
// checking: surrogates, values above U+10FFFF and 0xFFFFFFFF all pass.
// ---------------------------------------------------------------------------

TEST_CASE("mmutf_u32rune_char_size is always 1", "[utf32][rune]")
{
    REQUIRE(mmutf_u32rune_char_size(0x41) == 1);
    REQUIRE(mmutf_u32rune_char_size(0x4E2D) == 1);
    REQUIRE(mmutf_u32rune_char_size(0x1F600) == 1);
    REQUIRE(mmutf_u32rune_char_size(0xD800) == 1);
    REQUIRE(mmutf_u32rune_char_size(0x110000) == 1);
    REQUIRE(mmutf_u32rune_char_size(0xFFFFFFFF) == 1);
}

TEST_CASE("mmutf_u32rune_valid performs no range checking", "[utf32][rune]")
{
    // Documented current behaviour: every single unit validates.
    const uint32_t ok[] = { 0x41 };
    REQUIRE(mmutf_u32rune_valid(ok, 1) == 1);

    const uint32_t surrogate[] = { 0xD800 };
    REQUIRE(mmutf_u32rune_valid(surrogate, 1) == 1);

    const uint32_t too_big[] = { 0x110000 };
    REQUIRE(mmutf_u32rune_valid(too_big, 1) == 1);

    const uint32_t all_ones[] = { 0xFFFFFFFF };
    REQUIRE(mmutf_u32rune_valid(all_ones, 1) == 1);
}

TEST_CASE("mmutf_u32rune_set_u32 / get_u32 roundtrip", "[utf32][rune]")
{
    uint32_t buf[2] = { 0, 0 };
    int n = mmutf_u32rune_set_u32(buf, 2, 0x1F600);
    REQUIRE(n == 1);
    REQUIRE(buf[0] == 0x1F600);

    uint32_t out = 0;
    REQUIRE(mmutf_u32rune_get_u32(buf, n, &out) == 1);
    REQUIRE(out == 0x1F600);
}

// ---------------------------------------------------------------------------
// string::u16char_size — astral characters count as 2 UTF-16 units
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string u16char_size", "[utf16][string]")
{
    SECTION("empty string")
    {
        memepp::string s;
        REQUIRE(s.u16char_size() == 0);
    }

    SECTION("pure ASCII")
    {
        memepp::string s("abc");
        REQUIRE(s.u16char_size() == 3);
    }

    SECTION("BMP CJK counts one unit per rune")
    {
        memepp::string s("\xE4\xB8\xAD\xE6\x96\x87");   // 中文
        REQUIRE(s.u16char_size() == 2);
    }

    SECTION("astral character counts as two units")
    {
        memepp::string s("A\xF0\x9F\x98\x80");          // A + U+1F600
        REQUIRE(s.u16char_size() == 3);                 // 1 + 2
    }

    SECTION("mixed widths")
    {
        // 'A' (1) + U+00E9 (1) + U+4E2D (1) + U+1F600 (2)
        memepp::string s("A\xC3\xA9\xE4\xB8\xAD\xF0\x9F\x98\x80");
        REQUIRE(s.rune_size() == 4);
        REQUIRE(s.u16char_size() == 5);
    }
}

// ---------------------------------------------------------------------------
// cross-width agreement: encode through one width, decode through another
// ---------------------------------------------------------------------------

TEST_CASE("utf8 / utf16 / utf32 codepoint agreement", "[utf16][utf32][rune]")
{
    const uint32_t cps[] = { 0x41, 0x7FF, 0x800, 0x4E2D, 0xFFFF, 0x10000, 0x10FFFF };

    for (uint32_t cp : cps)
    {
        // UTF-8 encode/decode
        uint8_t u8[8] = { 0 };
        int u8_len = mmutf_u8rune_set_u32(u8, 8, cp);
        REQUIRE(u8_len > 0);

        uint32_t back8 = 0;
        REQUIRE(mmutf_u8rune_get_u32(u8, u8_len, &back8) == u8_len);
        REQUIRE(back8 == cp);

        // UTF-16 encode/decode
        uint16_t u16[4] = { 0, 0, 0, 0 };
        int u16_len = mmutf_u16rune_set_u32(u16, 4, cp);
        REQUIRE(u16_len > 0);

        uint32_t back16 = 0;
        REQUIRE(mmutf_u16rune_get_u32(u16, u16_len, &back16) == u16_len);
        REQUIRE(back16 == cp);

        // UTF-32 encode/decode
        uint32_t u32[2] = { 0, 0 };
        int u32_len = mmutf_u32rune_set_u32(u32, 2, cp);
        REQUIRE(u32_len == 1);

        uint32_t back32 = 0;
        REQUIRE(mmutf_u32rune_get_u32(u32, u32_len, &back32) == 1);
        REQUIRE(back32 == cp);
    }
}
