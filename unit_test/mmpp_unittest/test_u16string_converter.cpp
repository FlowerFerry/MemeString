
#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/convert/std/u16string.hpp>

#include <cstring>
#include <string>

// ============================================================================
// std::u16string <-> memepp::string
//
// The u16 decoder stops at the first unit it cannot pair, and everything from
// there on is dropped (no error signal).  Those cases pin the CURRENT behaviour
// and are marked TODO(impl); see the matching notes in test_wstring_converter.cpp.
// ============================================================================

// ---------------------------------------------------------------------------
// from: u16 -> UTF-8
// ---------------------------------------------------------------------------

TEST_CASE("memepp::from(std::u16string) — empty", "[u16string]")
{
    std::u16string u16;
    auto s = memepp::from(u16);
    REQUIRE(s.empty());
    REQUIRE(s.size() == 0);
    REQUIRE(s.storage_type() == memepp::string_storage_t::small);
}

TEST_CASE("memepp::from(std::u16string) — ASCII", "[u16string]")
{
    std::u16string u16 = u"hello";
    auto s = memepp::from(u16);
    REQUIRE(s == "hello");
    REQUIRE(s.size() == 5);
}

TEST_CASE("memepp::from(std::u16string) — BMP Chinese", "[u16string]")
{
    std::u16string u16 = u"\u4E2D";
    auto s = memepp::from(u16);
    REQUIRE(s.size() == 3);
    const uint8_t* d = reinterpret_cast<const uint8_t*>(s.data());
    REQUIRE(d[0] == 0xE4);
    REQUIRE(d[1] == 0xB8);
    REQUIRE(d[2] == 0xAD);
}

TEST_CASE("memepp::from(std::u16string) — surrogate pair becomes 4-byte UTF-8", "[u16string]")
{
    std::u16string u16;
    u16.push_back(static_cast<char16_t>(0xD83D));   /* high surrogate */
    u16.push_back(static_cast<char16_t>(0xDE00));   /* low surrogate */
    REQUIRE(u16.size() == 2);

    auto s = memepp::from(u16);
    REQUIRE(s.size() == 4);
    const uint8_t* d = reinterpret_cast<const uint8_t*>(s.data());
    REQUIRE(d[0] == 0xF0);
    REQUIRE(d[1] == 0x9F);
    REQUIRE(d[2] == 0x98);
    REQUIRE(d[3] == 0x80);
}

TEST_CASE("memepp::from(std::u16string) — lone surrogates truncate silently", "[u16string]")
{
    // TODO(impl): should substitute U+FFFD instead of dropping data.
    {
        std::u16string hi(1, static_cast<char16_t>(0xD83D));
        REQUIRE(memepp::from(hi).empty());
    }
    {
        std::u16string lo(1, static_cast<char16_t>(0xDE00));
        REQUIRE(memepp::from(lo).empty());
    }
    {
        std::u16string w;
        w.push_back(u'A');
        w.push_back(static_cast<char16_t>(0xD83D));
        w.push_back(u'B');
        // Everything from the unpaired high surrogate onwards is dropped.
        REQUIRE(memepp::from(w) == "A");
    }
}

TEST_CASE("memepp::from(std::u16string) — boundary code points", "[u16string]")
{
    struct row { char16_t unit; const char* utf8; size_t len; };
    const row rows[] = {
        { 0x007F,   "\x7F",         1 },
        { 0x0080,   "\xC2\x80",     2 },
        { 0x07FF,   "\xDF\xBF",     2 },
        { 0x0800,   "\xE0\xA0\x80", 3 },
        { 0xFFFF,   "\xEF\xBF\xBF", 3 },
        { 0xFFFD,   "\xEF\xBF\xBD", 3 },
    };
    for (const auto& r : rows) {
        std::u16string u16(1, r.unit);
        auto s = memepp::from(u16);
        REQUIRE(s.size() == r.len);
        REQUIRE(std::memcmp(s.data(), r.utf8, r.len) == 0);
    }
}

TEST_CASE("memepp::from(std::u16string) — embedded NUL preserved", "[u16string]")
{
    std::u16string u16;
    u16.push_back(u'a');
    u16.push_back(u'\0');
    u16.push_back(u'b');
    u16.push_back(u'\0');
    u16.push_back(u'c');

    auto s = memepp::from(u16);
    REQUIRE(s.size() == 5);
    REQUIRE(s.data()[0] == 'a');
    REQUIRE(s.data()[1] == 0);
    REQUIRE(s.data()[2] == 'b');
    REQUIRE(s.data()[3] == 0);
    REQUIRE(s.data()[4] == 'c');
}

TEST_CASE("memepp::from(std::u16string) — rvalue overload clears the source", "[u16string]")
{
    std::u16string u16 = u"hello";
    auto s = memepp::from(std::move(u16));

    REQUIRE(s == "hello");
    REQUIRE(u16.empty());
    REQUIRE(u16.size() == 0);
}

TEST_CASE("memepp::from(std::u16string) — lvalue keeps the source", "[u16string]")
{
    std::u16string u16 = u"\u4E2D";
    auto s = memepp::from(u16);

    REQUIRE(s.size() == 3);
    REQUIRE(u16.size() == 1);
}

TEST_CASE("memepp::from(std::u16string) — long input reaches large storage", "[u16string]")
{
    std::u16string u16;
    for (int i = 0; i < 300; ++i)
        u16.push_back(static_cast<char16_t>(0x4E2D));

    auto s = memepp::from(u16);
    REQUIRE(s.size() == 900);                    /* 300 * 3 UTF-8 bytes */
    REQUIRE(s.storage_type() == memepp::string_storage_t::large);
}

// ---------------------------------------------------------------------------
// to: UTF-8 -> u16
// ---------------------------------------------------------------------------

TEST_CASE("memepp::to<std::u16string> — empty", "[u16string]")
{
    memepp::string empty;
    REQUIRE(memepp::to<std::u16string>(empty).empty());

    memepp::string_view empty_view;
    REQUIRE(memepp::to<std::u16string>(empty_view).empty());
}

TEST_CASE("memepp::to<std::u16string> — ASCII and BMP", "[u16string]")
{
    memepp::string ascii{ "hello" };
    auto u16a = memepp::to<std::u16string>(ascii);
    REQUIRE(u16a.size() == 5);
    REQUIRE(u16a == u"hello");

    memepp::string bmp{ "\xE4\xB8\xAD", 3 };
    auto u16b = memepp::to<std::u16string>(bmp);
    REQUIRE(u16b.size() == 1);
    REQUIRE(u16b[0] == 0x4E2D);
}

TEST_CASE("memepp::to<std::u16string> — 4-byte UTF-8 becomes a surrogate pair", "[u16string]")
{
    memepp::string s{ "A" "\xF0" "\x9F" "\x98" "\x80" "B", 6 };
    auto u16 = memepp::to<std::u16string>(s);

    REQUIRE(u16.size() == 4);
    REQUIRE(u16[0] == 0x0041);
    REQUIRE(u16[1] == 0xD83D);
    REQUIRE(u16[2] == 0xDE00);
    REQUIRE(u16[3] == 0x0042);

    /* The string_view overload must agree. */
    auto u16v = memepp::to<std::u16string>(memepp::string_view{ s });
    REQUIRE(u16v == u16);
}

TEST_CASE("memepp::to<std::u16string> — boundary code points", "[u16string]")
{
    struct row { const char* utf8; size_t len; char16_t unit; };
    const row rows[] = {
        { "\x7F",         1, 0x007F },
        { "\xC2\x80",     2, 0x0080 },
        { "\xDF\xBF",     2, 0x07FF },
        { "\xE0\xA0\x80", 3, 0x0800 },
        { "\xEF\xBF\xBF", 3, 0xFFFF },
    };
    for (const auto& r : rows) {
        memepp::string s{ r.utf8, static_cast<mmint_t>(r.len) };
        auto u16 = memepp::to<std::u16string>(s);
        REQUIRE(u16.size() == 1);
        REQUIRE(u16[0] == r.unit);
    }
}

TEST_CASE("memepp::to<std::u16string> — U+10FFFF is the last encodable code point", "[u16string]")
{
    memepp::string s{ "\xF4\x8F\xBF\xBF", 4 };
    auto u16 = memepp::to<std::u16string>(s);
    REQUIRE(u16.size() == 2);
    REQUIRE(u16[0] == 0xDBFF);
    REQUIRE(u16[1] == 0xDFFF);
}

TEST_CASE("memepp::to<std::u16string> — embedded NUL preserved", "[u16string]")
{
    const char input[] = { 'a', '\0', 'b', '\0', 'c' };
    memepp::string s{ input, sizeof(input) };
    auto u16 = memepp::to<std::u16string>(s);

    REQUIRE(u16.size() == 5);
    REQUIRE(u16[0] == u'a');
    REQUIRE(u16[1] == 0);
    REQUIRE(u16[2] == u'b');
    REQUIRE(u16[3] == 0);
    REQUIRE(u16[4] == u'c');
}

TEST_CASE("memepp::to<std::u16string> — long input", "[u16string]")
{
    std::string utf8;
    for (int i = 0; i < 300; ++i)
        utf8.append("\xE4\xB8\xAD");
    memepp::string s{ utf8.data(), static_cast<mmint_t>(utf8.size()) };
    REQUIRE(s.storage_type() == memepp::string_storage_t::large);

    auto u16 = memepp::to<std::u16string>(s);
    REQUIRE(u16.size() == 300);
    for (auto u : u16)
        REQUIRE(u == 0x4E2D);
}

TEST_CASE("memepp::to<std::u16string> — roundtrip preserves content", "[u16string]")
{
    memepp::string s{ "A" "\xF0" "\x9F" "\x98" "\x80" "\xE4\xB8\xAD" "B", 10 };
    auto u16 = memepp::to<std::u16string>(s);
    REQUIRE(memepp::from(u16) == s);
}

TEST_CASE("memepp::to<std::u16string> — code points beyond UTF-16 truncate", "[u16string]")
{
    // TODO(impl): these should become U+FFFD and the trailing 'B' must survive.
    struct row { const char* utf8; mmint_t len; size_t want_units; };
    const row rows[] = {
        { "A" "\xF4" "\x90" "\x80" "\x80" "B",    6, 1 },   /* U+110000 */
        { "A" "\xF8" "\x88" "\x80" "\x80" "\x80" "B", 7, 1 }, /* U+200000, 5-byte */
        { "A" "\xED" "\xA0" "\x80" "B",           5, 1 },   /* U+D800 surrogate */
    };
    for (const auto& r : rows) {
        memepp::string s{ r.utf8, r.len };
        auto u16 = memepp::to<std::u16string>(s);
        REQUIRE(u16.size() == r.want_units);
        REQUIRE(u16[0] == 0x0041);
    }
}

TEST_CASE("memepp::to<std::u16string> — invalid UTF-8 truncates", "[u16string]")
{
    struct row { const char* bytes; mmint_t len; size_t want_units; };
    const row rows[] = {
        { "A" "\xFF" "B", 3, 1 },
        { "\xE4\xB8",     2, 0 },
    };
    for (const auto& r : rows) {
        memepp::string s{ r.bytes, r.len };
        auto u16 = memepp::to<std::u16string>(s);
        REQUIRE(u16.size() == r.want_units);
    }
}
