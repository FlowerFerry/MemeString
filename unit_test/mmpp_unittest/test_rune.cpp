#include <catch2/catch.hpp>

#include <memepp/rune.hpp>
#include <memepp/string.hpp>
#include <memepp/string_view.hpp>

#include <cstring>

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

// ---------------------------------------------------------------------------
// rune_index — construction, data, size, is_space
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune_index from bytes — space character", "[rune_index]")
{
    const uint8_t space_byte[] = { 0x20 }; // ASCII space
    memepp::rune_index ri(space_byte, 1);

    REQUIRE(ri.data() != nullptr);
    REQUIRE(ri.size() == 1);
    REQUIRE(ri.is_space());
}

TEST_CASE("memepp::rune_index from bytes — non-space character", "[rune_index]")
{
    const uint8_t letter[] = { static_cast<uint8_t>('A') };
    memepp::rune_index ri(letter, 1);

    REQUIRE(ri.data() != nullptr);
    REQUIRE(ri.size() == 1);
    REQUIRE_FALSE(ri.is_space());
}

TEST_CASE("memepp::rune_index from bytes — tab is space", "[rune_index]")
{
    const uint8_t tab_byte[] = { 0x09 }; // horizontal tab
    memepp::rune_index ri(tab_byte, 1);

    REQUIRE(ri.is_space());
}

TEST_CASE("memepp::rune_index copy constructor", "[rune_index]")
{
    const uint8_t byte[] = { static_cast<uint8_t>('X') };
    memepp::rune_index ri(byte, 1);
    memepp::rune_index ri2(ri);

    REQUIRE(ri2.size() == ri.size());
    REQUIRE(ri2.is_space() == ri.is_space());
    REQUIRE(ri2.data() != nullptr);
}

TEST_CASE("memepp::rune_index move constructor", "[rune_index]")
{
    const uint8_t byte[] = { static_cast<uint8_t>('Y') };
    memepp::rune_index ri(byte, 1);
    memepp::rune_index ri2(std::move(ri));

    REQUIRE(ri2.size() == 1);
    REQUIRE_FALSE(ri2.is_space());
    REQUIRE(ri2.data() != nullptr);
}

TEST_CASE("memepp::rune_index from UTF-8 multibyte — ideographic space", "[rune_index]")
{
    // U+3000 IDEOGRAPHIC SPACE — UTF-8: E3 80 80 (3 bytes)
    const uint8_t ideo_space[] = { 0xE3, 0x80, 0x80 };
    memepp::rune_index ri(ideo_space, 3);

    REQUIRE(ri.data() != nullptr);
    REQUIRE(ri.size() == 3);
    REQUIRE(ri.is_space());
}

// ---------------------------------------------------------------------------
// rune: construct from C MemeRune_t (copy and move)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune construct from MemeRune_t (copy)", "[rune]")
{
    MemeRune_t c_rune;
    MemeRune_initByByte(&c_rune, 'Z');

    memepp::rune r(c_rune);
    REQUIRE(r.valid());
    REQUIRE(r == 'Z');
    REQUIRE(r.size() == 1);
}

TEST_CASE("memepp::rune construct from MemeRune_t (move)", "[rune]")
{
    MemeRune_t c_rune;
    MemeRune_initByByte(&c_rune, 'M');

    memepp::rune r(std::move(c_rune));
    REQUIRE(r.valid());
    REQUIRE(r == 'M');
    REQUIRE(r.size() == 1);
}

// ---------------------------------------------------------------------------
// rune: operator= from C MemeRune_t (copy and move)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune operator= from MemeRune_t (copy)", "[rune]")
{
    MemeRune_t c_rune;
    MemeRune_initByByte(&c_rune, 'Q');

    memepp::rune r;
    r = c_rune;
    REQUIRE(r == 'Q');
}

TEST_CASE("memepp::rune operator= from MemeRune_t (move)", "[rune]")
{
    MemeRune_t c_rune;
    MemeRune_initByByte(&c_rune, 'X');

    memepp::rune r;
    r = std::move(c_rune);
    REQUIRE(r == 'X');
}

// ---------------------------------------------------------------------------
// rune: operator= from rune (copy and move) — independent tests with self-assignment
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune operator= from rune (copy)", "[rune]")
{
    memepp::rune src('P');
    memepp::rune dst;

    dst = src;
    REQUIRE(dst == 'P');
    REQUIRE(src == 'P');  // src unchanged
}

TEST_CASE("memepp::rune operator= from rune self-assignment", "[rune]")
{
    memepp::rune r('T');

    r = r;
    REQUIRE(r == 'T');
    REQUIRE(r.size() == 1);
}

TEST_CASE("memepp::rune operator= from rune (move)", "[rune]")
{
    memepp::rune src('W');
    memepp::rune dst;

    dst = std::move(src);
    REQUIRE(dst == 'W');
}

TEST_CASE("memepp::rune operator= from rune self-move-assignment", "[rune]")
{
    memepp::rune r('Y');

    r = std::move(r);
    REQUIRE(r == 'Y');
    REQUIRE(r.size() == 1);
}

// ---------------------------------------------------------------------------
// rune: non-const data(), begin(), end()
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune non-const data returns mutable pointer", "[rune]")
{
    memepp::rune r('A');
    auto* p = r.data();
    REQUIRE(p != nullptr);
    REQUIRE(p[0] == 'A');
}

TEST_CASE("memepp::rune non-const begin and end", "[rune]")
{
    memepp::rune r('K');
    // begin
    auto it = r.begin();
    REQUIRE(it != r.end());
    REQUIRE(*it == static_cast<uint8_t>('K'));
    ++it;
    REQUIRE(it == r.end());

    // end
    REQUIRE(r.begin() != r.end());
}

// ---------------------------------------------------------------------------
// rune: const begin() and end() — direct call (not cbegin/cend)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune begin const and end const", "[rune]")
{
    const memepp::rune r('L');

    auto it = r.begin();
    REQUIRE(it != r.end());
    REQUIRE(*it == static_cast<uint8_t>('L'));
    ++it;
    REQUIRE(it == r.end());
}

// ---------------------------------------------------------------------------
// rune: native_handle() const and non-const
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune native_handle returns reference to MemeRune_t", "[rune]")
{
    memepp::rune r('V');

    auto& nh = r.native_handle();
    REQUIRE(sizeof(nh) == sizeof(MemeRune_t));

    // verify content via C API
    REQUIRE(MemeRune_size(&nh) == 1);
    REQUIRE(MemeRune_data(&nh)[0] == static_cast<uint8_t>('V'));
}

TEST_CASE("memepp::rune native_handle const", "[rune]")
{
    const memepp::rune r('S');

    const auto& nh = r.native_handle();
    REQUIRE(sizeof(nh) == sizeof(MemeRune_t));
    REQUIRE(MemeRune_size(&nh) == 1);
    REQUIRE(MemeRune_data(&nh)[0] == static_cast<uint8_t>('S'));
}

// ---------------------------------------------------------------------------
// cross-module operator!= for rune vs string_view
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune operator!= with string_view", "[rune]")
{
    memepp::rune r('H');
    memepp::string_view sv_h("H");
    memepp::string_view sv_w("W");

    REQUIRE_FALSE(r != sv_h);
    REQUIRE_FALSE(sv_h != r);
    REQUIRE(r != sv_w);
    REQUIRE(sv_w != r);
}

// ---------------------------------------------------------------------------
// rune_index: construct from C MemeRuneIndex_t (copy and move)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune_index construct from MemeRuneIndex_t (copy)", "[rune_index]")
{
    const uint8_t byte[] = { 'C' };
    MemeRuneIndex_t c_idx;
    c_idx.data = byte;
    c_idx.size = 1;

    memepp::rune_index ri(c_idx);
    REQUIRE(ri.data() == byte);
    REQUIRE(ri.size() == 1);
    REQUIRE_FALSE(ri.is_space());
}

TEST_CASE("memepp::rune_index construct from MemeRuneIndex_t (move)", "[rune_index]")
{
    const uint8_t byte[] = { 'D' };
    MemeRuneIndex_t c_idx;
    c_idx.data = byte;
    c_idx.size = 1;

    memepp::rune_index ri(std::move(c_idx));
    REQUIRE(ri.data() == byte);
    REQUIRE(ri.size() == 1);
    REQUIRE_FALSE(ri.is_space());
}

// ---------------------------------------------------------------------------
// rune_index: native_handle() const and non-const
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune_index native_handle returns reference", "[rune_index]")
{
    const uint8_t byte[] = { 0x20 };
    memepp::rune_index ri(byte, 1);

    auto& nh = ri.native_handle();
    REQUIRE(nh.data == byte);
    REQUIRE(nh.size == 1);
    REQUIRE(sizeof(nh) == sizeof(MemeRuneIndex_t));
}

TEST_CASE("memepp::rune_index native_handle const", "[rune_index]")
{
    const uint8_t byte[] = { 'E' };
    const memepp::rune_index ri(byte, 1);

    const auto& nh = ri.native_handle();
    REQUIRE(nh.data == byte);
    REQUIRE(nh.size == 1);
    REQUIRE(sizeof(nh) == sizeof(MemeRuneIndex_t));
}

// ---------------------------------------------------------------------------
// clear
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune clear resets to empty", "[rune]")
{
    memepp::rune r('X');
    REQUIRE(!r.empty());
    REQUIRE(r.valid());

    r.clear();
    REQUIRE(r.empty());
    REQUIRE(r.valid());
    REQUIRE(r.size() == 0);
}

TEST_CASE("memepp::rune clear on empty rune is idempotent", "[rune]")
{
    memepp::rune r;
    REQUIRE(r.empty());

    r.clear();
    REQUIRE(r.empty());
    REQUIRE(r.valid());
}

// ---------------------------------------------------------------------------
// compare and ordering operators
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune compare — equal", "[rune]")
{
    memepp::rune a('A');
    memepp::rune b('A');
    REQUIRE(a.compare(b) == 0);
}

TEST_CASE("memepp::rune compare — less", "[rune]")
{
    memepp::rune a('A');
    memepp::rune b('B');
    REQUIRE(a.compare(b) < 0);
}

TEST_CASE("memepp::rune compare — greater", "[rune]")
{
    memepp::rune a('Z');
    memepp::rune b('A');
    REQUIRE(a.compare(b) > 0);
}

TEST_CASE("memepp::rune compare — shorter prefix is less", "[rune]")
{
    const uint8_t zhong[] = { 0xE4, 0xB8, 0xAD };
    memepp::rune a('A');
    memepp::rune b(zhong, 3);
    REQUIRE(a.compare(b) < 0);
}

TEST_CASE("memepp::rune operator<", "[rune]")
{
    memepp::rune a('A');
    memepp::rune b('B');
    REQUIRE(a < b);
    REQUIRE_FALSE(b < a);
}

TEST_CASE("memepp::rune operator<=", "[rune]")
{
    memepp::rune a('A');
    memepp::rune b('B');
    memepp::rune c('A');
    REQUIRE(a <= b);
    REQUIRE(a <= c);
    REQUIRE_FALSE(b <= a);
}

TEST_CASE("memepp::rune operator>", "[rune]")
{
    memepp::rune a('A');
    memepp::rune b('B');
    REQUIRE(b > a);
    REQUIRE_FALSE(a > b);
}

TEST_CASE("memepp::rune operator>=", "[rune]")
{
    memepp::rune a('A');
    memepp::rune b('B');
    memepp::rune c('A');
    REQUIRE(b >= a);
    REQUIRE(a >= c);
    REQUIRE_FALSE(a >= b);
}

// ---------------------------------------------------------------------------
// is_ascii
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune is_ascii — ASCII letters", "[rune]")
{
    REQUIRE(memepp::rune('A').is_ascii());
    REQUIRE(memepp::rune('z').is_ascii());
    REQUIRE(memepp::rune('0').is_ascii());
}

TEST_CASE("memepp::rune is_ascii — ASCII punctuation", "[rune]")
{
    REQUIRE(memepp::rune('!').is_ascii());
    REQUIRE(memepp::rune(' ').is_ascii());
}

TEST_CASE("memepp::rune is_ascii — multi-byte is not ASCII", "[rune]")
{
    const uint8_t zhong[] = { 0xE4, 0xB8, 0xAD };
    memepp::rune r(zhong, 3);
    REQUIRE_FALSE(r.is_ascii());
}

TEST_CASE("memepp::rune is_ascii — empty rune is not ASCII", "[rune]")
{
    memepp::rune r;
    REQUIRE(r.empty());
    REQUIRE_FALSE(r.is_ascii());
}

// ---------------------------------------------------------------------------
// is_en_lower
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune is_en_lower — lowercase letters", "[rune]")
{
    REQUIRE(memepp::rune('a').is_en_lower());
    REQUIRE(memepp::rune('z').is_en_lower());
}

TEST_CASE("memepp::rune is_en_lower — non-lowercase", "[rune]")
{
    REQUIRE_FALSE(memepp::rune('A').is_en_lower());
    REQUIRE_FALSE(memepp::rune('0').is_en_lower());
    REQUIRE_FALSE(memepp::rune('!').is_en_lower());
}

TEST_CASE("memepp::rune is_en_lower — multibyte is false", "[rune]")
{
    const uint8_t zhong[] = { 0xE4, 0xB8, 0xAD };
    memepp::rune r(zhong, 3);
    REQUIRE_FALSE(r.is_en_lower());
}

// ---------------------------------------------------------------------------
// is_en_upper
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune is_en_upper — uppercase letters", "[rune]")
{
    REQUIRE(memepp::rune('A').is_en_upper());
    REQUIRE(memepp::rune('Z').is_en_upper());
}

TEST_CASE("memepp::rune is_en_upper — non-uppercase", "[rune]")
{
    REQUIRE_FALSE(memepp::rune('a').is_en_upper());
    REQUIRE_FALSE(memepp::rune('0').is_en_upper());
    REQUIRE_FALSE(memepp::rune('!').is_en_upper());
}

TEST_CASE("memepp::rune is_en_upper — multibyte is false", "[rune]")
{
    const uint8_t zhong[] = { 0xE4, 0xB8, 0xAD };
    memepp::rune r(zhong, 3);
    REQUIRE_FALSE(r.is_en_upper());
}

// ---------------------------------------------------------------------------
// to_en_lower
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune to_en_lower — uppercase to lowercase", "[rune]")
{
    memepp::rune r('H');
    auto r2 = r.to_en_lower();
    REQUIRE(r2 == 'h');
    REQUIRE(r == 'H');  // original unchanged
}

TEST_CASE("memepp::rune to_en_lower — already lowercase unchanged", "[rune]")
{
    memepp::rune r('a');
    auto r2 = r.to_en_lower();
    REQUIRE(r2 == 'a');
}

TEST_CASE("memepp::rune to_en_lower — non-letter ASCII unchanged", "[rune]")
{
    memepp::rune r('1');
    auto r2 = r.to_en_lower();
    REQUIRE(r2 == '1');
}

TEST_CASE("memepp::rune to_en_lower — multibyte unchanged", "[rune]")
{
    const uint8_t zhong[] = { 0xE4, 0xB8, 0xAD };
    memepp::rune r(zhong, 3);
    auto r2 = r.to_en_lower();
    REQUIRE(r2 == r);
}

// ---------------------------------------------------------------------------
// to_en_upper
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune to_en_upper — lowercase to uppercase", "[rune]")
{
    memepp::rune r('a');
    auto r2 = r.to_en_upper();
    REQUIRE(r2 == 'A');
    REQUIRE(r == 'a');  // original unchanged
}

TEST_CASE("memepp::rune to_en_upper — already uppercase unchanged", "[rune]")
{
    memepp::rune r('Z');
    auto r2 = r.to_en_upper();
    REQUIRE(r2 == 'Z');
}

TEST_CASE("memepp::rune to_en_upper — non-letter ASCII unchanged", "[rune]")
{
    memepp::rune r('9');
    auto r2 = r.to_en_upper();
    REQUIRE(r2 == '9');
}

TEST_CASE("memepp::rune to_en_upper — multibyte unchanged", "[rune]")
{
    const uint8_t zhong[] = { 0xE4, 0xB8, 0xAD };
    memepp::rune r(zhong, 3);
    auto r2 = r.to_en_upper();
    REQUIRE(r2 == r);
}

// ---------------------------------------------------------------------------
// codepoint
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune codepoint — ASCII", "[rune]")
{
    memepp::rune r('A');
    REQUIRE(r.codepoint() == 0x41);
}

TEST_CASE("memepp::rune codepoint — Chinese character", "[rune]")
{
    const uint8_t zhong[] = { 0xE4, 0xB8, 0xAD };
    memepp::rune r(zhong, 3);
    REQUIRE(r.codepoint() == 0x4E2D);
}

TEST_CASE("memepp::rune codepoint — emoji", "[rune]")
{
    const uint8_t emoji[] = { 0xF0, 0x9F, 0x98, 0x80 };
    memepp::rune r(emoji, 4);
    REQUIRE(r.codepoint() == 0x1F600);
}

TEST_CASE("memepp::rune codepoint — empty rune returns 0", "[rune]")
{
    memepp::rune r;
    REQUIRE(r.codepoint() == 0);
}

// ---------------------------------------------------------------------------
// from_codepoint
// ---------------------------------------------------------------------------

TEST_CASE("memepp::rune from_codepoint — ASCII", "[rune]")
{
    auto r = memepp::rune::from_codepoint(0x41);
    REQUIRE(r.valid());
    REQUIRE(r == 'A');
    REQUIRE(r.size() == 1);
}

TEST_CASE("memepp::rune from_codepoint — Chinese character", "[rune]")
{
    auto r = memepp::rune::from_codepoint(0x4E2D);
    REQUIRE(r.valid());
    REQUIRE(r.size() == 3);
    REQUIRE(r.codepoint() == 0x4E2D);
}

TEST_CASE("memepp::rune from_codepoint — emoji", "[rune]")
{
    auto r = memepp::rune::from_codepoint(0x1F600);
    REQUIRE(r.valid());
    REQUIRE(r.size() == 4);
    REQUIRE(r.codepoint() == 0x1F600);
}

TEST_CASE("memepp::rune from_codepoint — roundtrip", "[rune]")
{
    for (uint32_t cp : { 0x20, 0x41, 0x7E, 0x00E9, 0x4E2D, 0x1F600 }) {
        auto r = memepp::rune::from_codepoint(cp);
        REQUIRE(r.codepoint() == cp);
    }
}

TEST_CASE("memepp::rune from_codepoint — null character", "[rune]")
{
    auto r = memepp::rune::from_codepoint(0x00);
    REQUIRE(r.valid());
    REQUIRE(r.size() == 1);
    REQUIRE(r.data()[0] == 0x00);
}
