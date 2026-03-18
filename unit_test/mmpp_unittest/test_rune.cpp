#include <catch2/catch.hpp>

#include <memepp/rune.hpp>
#include <memepp/string.hpp>

// ---------------------------------------------------------------------------
// constructors / basic properties
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune default constructor is empty and invalid", "[rune]")
{
    memepp::rune r;
    REQUIRE(r.empty());
    REQUIRE(r.valid());
    REQUIRE(r.size() == 0);
}

TEST_CASE("memepp::rune from ASCII char", "[rune]")
{
    memepp::rune r('A');
    REQUIRE(!r.empty());
    REQUIRE(r.valid());
    REQUIRE(r.size() == 1);
    REQUIRE(r.char_size() == 1);
    REQUIRE(r == 'A');
    REQUIRE(!r.is_multi());
    REQUIRE(r.is_en_char());
}

TEST_CASE("memepp::rune from UTF-8 bytes (Chinese character)", "[rune]")
{
    // U+4E2D '中' — UTF-8: E4 B8 AD
    const uint8_t utf8_zhong[] = { 0xE4, 0xB8, 0xAD };
    memepp::rune r(utf8_zhong, 3);
    REQUIRE(!r.empty());
    REQUIRE(r.valid());
    REQUIRE(r.size() == 3);
    REQUIRE(r.char_size() == 3);
    REQUIRE(r.is_multi());
}

TEST_CASE("memepp::rune from 4-byte UTF-8 emoji", "[rune]")
{
    // U+1F600 GRINNING FACE — UTF-8: F0 9F 98 80
    const uint8_t emoji[] = { 0xF0, 0x9F, 0x98, 0x80 };
    memepp::rune r(emoji, 4);
    REQUIRE(r.valid());
    REQUIRE(r.size() == 4);
    REQUIRE(r.is_multi());
}

TEST_CASE("memepp::rune copy and move constructors", "[rune]")
{
    memepp::rune src('Z');
    memepp::rune copy(src);
    REQUIRE(copy == 'Z');
    REQUIRE(copy.valid());

    memepp::rune moved(std::move(src));
    REQUIRE(moved == 'Z');
}

// ---------------------------------------------------------------------------
// assignment operators
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune operator= from char", "[rune]")
{
    memepp::rune r;
    r = 'K';
    REQUIRE(r == 'K');
    REQUIRE(r.size() == 1);
}

TEST_CASE("memepp::rune operator= from const_pointer", "[rune]")
{
    memepp::rune r;
    const uint8_t* u8ptr = reinterpret_cast<const uint8_t*>("B");
    r = reinterpret_cast<const memepp::rune::const_pointer>(u8ptr);
    REQUIRE(r == 'B');
}

// ---------------------------------------------------------------------------
// is_ch_punct — Chinese / fullwidth punctuation
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune is_ch_punct — Chinese punctuation marks", "[rune]")
{
    // 、U+3001 — 3 bytes: E3 80 81
    const uint8_t zh_enum_comma[]   = { 0xE3, 0x80, 0x81 };
    // 。U+3002 — 3 bytes: E3 80 82
    const uint8_t zh_period[]       = { 0xE3, 0x80, 0x82 };
    // 《U+300A — 3 bytes: E3 80 8A
    const uint8_t left_book_mark[]  = { 0xE3, 0x80, 0x8A };
    // 》U+300B — 3 bytes: E3 80 8B
    const uint8_t right_book_mark[] = { 0xE3, 0x80, 0x8B };
    // ！U+FF01 — 3 bytes: EF BC 81
    const uint8_t fullwidth_bang[]  = { 0xEF, 0xBC, 0x81 };
    // ，U+FF0C — 3 bytes: EF BC 8C
    const uint8_t fullwidth_comma[] = { 0xEF, 0xBC, 0x8C };
    // ？U+FF1F — 3 bytes: EF BC 9F
    const uint8_t fullwidth_qmark[] = { 0xEF, 0xBC, 0x9F };

    REQUIRE(memepp::rune(zh_enum_comma,   3).is_ch_punct());
    REQUIRE(memepp::rune(zh_period,       3).is_ch_punct());
    REQUIRE(memepp::rune(left_book_mark,  3).is_ch_punct());
    REQUIRE(memepp::rune(right_book_mark, 3).is_ch_punct());
    REQUIRE(memepp::rune(fullwidth_bang,  3).is_ch_punct());
    REQUIRE(memepp::rune(fullwidth_comma, 3).is_ch_punct());
    REQUIRE(memepp::rune(fullwidth_qmark, 3).is_ch_punct());
}

TEST_CASE("memepp::rune is_ch_punct — ASCII chars are not ch_punct", "[rune]")
{
    REQUIRE(!memepp::rune('!').is_ch_punct());
    REQUIRE(!memepp::rune(',').is_ch_punct());
    REQUIRE(!memepp::rune('?').is_ch_punct());
    REQUIRE(!memepp::rune('A').is_ch_punct());
}

// ---------------------------------------------------------------------------
// is_en_punct — ASCII punctuation
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune is_en_punct — ASCII punctuation", "[rune]")
{
    REQUIRE(memepp::rune('!').is_en_punct());
    REQUIRE(memepp::rune('.').is_en_punct());
    REQUIRE(memepp::rune(',').is_en_punct());
    REQUIRE(memepp::rune('?').is_en_punct());
}

TEST_CASE("memepp::rune is_en_punct — letters are not en_punct", "[rune]")
{
    REQUIRE(!memepp::rune('A').is_en_punct());
    REQUIRE(!memepp::rune('z').is_en_punct());
    REQUIRE(!memepp::rune('0').is_en_punct());
}

// ---------------------------------------------------------------------------
// is_space
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune is_space — space characters", "[rune]")
{
    REQUIRE(memepp::rune(' ').is_space());
    REQUIRE(memepp::rune('\t').is_space());
    REQUIRE(memepp::rune('\n').is_space());
    REQUIRE(memepp::rune('\r').is_space());
}

TEST_CASE("memepp::rune is_space — non-space characters", "[rune]")
{
    REQUIRE(!memepp::rune('A').is_space());
    REQUIRE(!memepp::rune('1').is_space());
}

// ---------------------------------------------------------------------------
// is_multi / is_en_char
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune is_multi vs is_en_char", "[rune]")
{
    memepp::rune ascii('Q');
    REQUIRE(!ascii.is_multi());
    REQUIRE(ascii.is_en_char());

    // U+4E2D '中' UTF-8: E4 B8 AD
    const uint8_t zhong[] = { 0xE4, 0xB8, 0xAD };
    memepp::rune zh(zhong, 3);
    REQUIRE(zh.is_multi());
    REQUIRE(!zh.is_en_char());
}

// ---------------------------------------------------------------------------
// bool conversion (operator bool)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune operator bool", "[rune]")
{
    memepp::rune empty_r;
    REQUIRE(!static_cast<bool>(empty_r));

    memepp::rune valid_r('X');
    REQUIRE(static_cast<bool>(valid_r));
}

// ---------------------------------------------------------------------------
// iterators
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune iterators produce correct bytes", "[rune]")
{
    // ASCII 'H' — single byte
    memepp::rune r('H');
    int count = 0;
    for (auto it = r.cbegin(); it != r.cend(); ++it)
        ++count;
    REQUIRE(count == 1);
    REQUIRE(*r.cbegin() == static_cast<uint8_t>('H'));

    // '中' — three bytes
    const uint8_t zhong[] = { 0xE4, 0xB8, 0xAD };
    memepp::rune rz(zhong, 3);
    count = 0;
    for (auto it = rz.cbegin(); it != rz.cend(); ++it)
        ++count;
    REQUIRE(count == 3);
}
