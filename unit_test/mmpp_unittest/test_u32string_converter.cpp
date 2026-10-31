
#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/convert/std/u32string.hpp>

#include <cstring>
#include <string>

// ============================================================================
// std::u32string <-> memepp::string
//
// char32_t holds the code point directly, so this direction is lossless for any
// value the library's UTF-8 side can encode (up to 6 bytes / 31 bits).  That
// includes code points above U+10FFFF and lone surrogates, which UTF-16 cannot
// represent but UTF-32 can.
//
// The reverse direction (UTF-8 -> UTF-32) still substitutes U+FFFD for an
// overlong encoding of U+0000, one per source byte, to keep the result free of
// embedded NUL code units.
// ============================================================================

// ---------------------------------------------------------------------------
// from: u32 -> UTF-8
// ---------------------------------------------------------------------------

TEST_CASE("memepp::from(std::u32string) — empty", "[u32string]")
{
    std::u32string u32;
    auto s = memepp::from(u32);
    REQUIRE(s.empty());
    REQUIRE(s.size() == 0);
    REQUIRE(s.storage_type() == memepp::string_storage_t::small);
}

TEST_CASE("memepp::from(std::u32string) — ASCII", "[u32string]")
{
    std::u32string u32 = U"hello";
    auto s = memepp::from(u32);
    REQUIRE(s == "hello");
    REQUIRE(s.size() == 5);
}

TEST_CASE("memepp::from(std::u32string) — BMP Chinese", "[u32string]")
{
    std::u32string u32 = U"\u4E2D";
    auto s = memepp::from(u32);
    REQUIRE(s.size() == 3);
    const uint8_t* d = reinterpret_cast<const uint8_t*>(s.data());
    REQUIRE(d[0] == 0xE4);
    REQUIRE(d[1] == 0xB8);
    REQUIRE(d[2] == 0xAD);
}

TEST_CASE("memepp::from(std::u32string) — supplementary plane is one code unit", "[u32string]")
{
    // char32_t holds U+1F600 directly; no surrogate pair involved.
    std::u32string u32;
    u32.push_back(static_cast<char32_t>(0x1F600));
    REQUIRE(u32.size() == 1);

    auto s = memepp::from(u32);
    REQUIRE(s.size() == 4);
    const uint8_t* d = reinterpret_cast<const uint8_t*>(s.data());
    REQUIRE(d[0] == 0xF0);
    REQUIRE(d[1] == 0x9F);
    REQUIRE(d[2] == 0x98);
    REQUIRE(d[3] == 0x80);
}

TEST_CASE("memepp::from(std::u32string) — boundary code points", "[u32string]")
{
    struct row { char32_t unit; const char* utf8; size_t len; };
    const row rows[] = {
        { 0x00007F, "\x7F",                 1 },
        { 0x000080, "\xC2\x80",             2 },
        { 0x0007FF, "\xDF\xBF",             2 },
        { 0x000800, "\xE0\xA0\x80",         3 },
        { 0x00FFFF, "\xEF\xBF\xBF",         3 },
        { 0x010000, "\xF0\x90\x80\x80",     4 },
        { 0x10FFFF, "\xF4\x8F\xBF\xBF",     4 },
    };
    for (const auto& r : rows) {
        std::u32string u32(1, r.unit);
        auto s = memepp::from(u32);
        REQUIRE(s.size() == r.len);
        REQUIRE(std::memcmp(s.data(), r.utf8, r.len) == 0);
    }
}

TEST_CASE("memepp::from(std::u32string) — beyond U+10FFFF is preserved", "[u32string]")
{
    // UTF-32 can hold these; the library's UTF-8 side supports up to 6 bytes
    // (31-bit code points), so nothing is lost or replaced.
    struct row { char32_t unit; const char* utf8; size_t len; };
    const row rows[] = {
        { 0x0110000, "\xF4\x90\x80\x80",          4 },
        { 0x0200000, "\xF8\x88\x80\x80\x80",      5 },
    };
    for (const auto& r : rows) {
        std::u32string u32(1, r.unit);
        auto s = memepp::from(u32);
        REQUIRE(s.size() == r.len);
        REQUIRE(std::memcmp(s.data(), r.utf8, r.len) == 0);
    }
}

TEST_CASE("memepp::from(std::u32string) — lone surrogate is encoded as-is", "[u32string]")
{
    // A surrogate code point is not valid Unicode but char32_t carries it
    // verbatim; the UTF-8 side of this library accepts it (see the project's
    // "long encoding" note in meme_utf_u8rune.c).
    std::u32string u32;
    u32.push_back(static_cast<char32_t>(0xD800));

    auto s = memepp::from(u32);
    REQUIRE(s.size() == 3);
    const uint8_t* d = reinterpret_cast<const uint8_t*>(s.data());
    REQUIRE(d[0] == 0xED);
    REQUIRE(d[1] == 0xA0);
    REQUIRE(d[2] == 0x80);
}

TEST_CASE("memepp::from(std::u32string) — embedded NUL preserved", "[u32string]")
{
    std::u32string u32;
    u32.push_back(U'a');
    u32.push_back(static_cast<char32_t>(0));
    u32.push_back(U'b');

    auto s = memepp::from(u32);
    REQUIRE(s.size() == 3);
    REQUIRE(s.data()[0] == 'a');
    REQUIRE(s.data()[1] == 0);
    REQUIRE(s.data()[2] == 'b');
}

TEST_CASE("memepp::from(std::u32string) — rvalue overload clears the source", "[u32string]")
{
    std::u32string u32 = U"hello";
    auto s = memepp::from(std::move(u32));

    REQUIRE(s == "hello");
    REQUIRE(u32.empty());
    REQUIRE(u32.size() == 0);
}

TEST_CASE("memepp::from(std::u32string) — lvalue keeps the source", "[u32string]")
{
    std::u32string u32 = U"\u4E2D";
    auto s = memepp::from(u32);

    REQUIRE(s.size() == 3);
    REQUIRE(u32.size() == 1);
}

TEST_CASE("memepp::from(std::u32string) — long input reaches large storage", "[u32string]")
{
    std::u32string u32;
    for (int i = 0; i < 300; ++i)
        u32.push_back(static_cast<char32_t>(0x4E2D));

    auto s = memepp::from(u32);
    REQUIRE(s.size() == 900);                    /* 300 * 3 UTF-8 bytes */
    REQUIRE(s.storage_type() == memepp::string_storage_t::large);
}

// ---------------------------------------------------------------------------
// to: UTF-8 -> u32
// ---------------------------------------------------------------------------

TEST_CASE("memepp::to<std::u32string> — empty", "[u32string]")
{
    memepp::string empty;
    REQUIRE(memepp::to<std::u32string>(empty).empty());

    memepp::string_view empty_view;
    REQUIRE(memepp::to<std::u32string>(empty_view).empty());
}

TEST_CASE("memepp::to<std::u32string> — ASCII and BMP", "[u32string]")
{
    memepp::string ascii{ "hello" };
    auto u32a = memepp::to<std::u32string>(ascii);
    REQUIRE(u32a.size() == 5);
    REQUIRE(u32a == U"hello");

    memepp::string bmp{ "\xE4\xB8\xAD", 3 };
    auto u32b = memepp::to<std::u32string>(bmp);
    REQUIRE(u32b.size() == 1);
    REQUIRE(u32b[0] == 0x4E2D);
}

TEST_CASE("memepp::to<std::u32string> — 4-byte UTF-8 is one code unit", "[u32string]")
{
    memepp::string s{ "A" "\xF0" "\x9F" "\x98" "\x80" "B", 6 };
    auto u32 = memepp::to<std::u32string>(s);

    REQUIRE(u32.size() == 3);
    REQUIRE(u32[0] == 0x0041);
    REQUIRE(u32[1] == 0x1F600);        /* no surrogate pair */
    REQUIRE(u32[2] == 0x0042);

    /* The string_view overload must agree. */
    auto u32v = memepp::to<std::u32string>(memepp::string_view{ s });
    REQUIRE(u32v == u32);
}

TEST_CASE("memepp::to<std::u32string> — boundary code points", "[u32string]")
{
    struct row { const char* utf8; size_t len; char32_t unit; };
    const row rows[] = {
        { "\x7F",         1, 0x00007F },
        { "\xC2\x80",     2, 0x000080 },
        { "\xDF\xBF",     2, 0x0007FF },
        { "\xE0\xA0\x80", 3, 0x000800 },
        { "\xEF\xBF\xBF", 3, 0x00FFFF },
        { "\xF0\x90\x80\x80", 4, 0x010000 },
        { "\xF4\x8F\xBF\xBF", 4, 0x10FFFF },
    };
    for (const auto& r : rows) {
        memepp::string s{ r.utf8, static_cast<mmint_t>(r.len) };
        auto u32 = memepp::to<std::u32string>(s);
        REQUIRE(u32.size() == 1);
        REQUIRE(u32[0] == r.unit);
    }
}

TEST_CASE("memepp::to<std::u32string> — code points above U+10FFFF survive", "[u32string]")
{
    // Unlike the UTF-16 conversion, nothing needs replacing: char32_t holds the
    // value and the library's UTF-8 decoder accepts up to 6 bytes.
    struct row { const char* utf8; mmint_t len; char32_t want; };
    const row rows[] = {
        { "\xF4\x90\x80\x80",          4, 0x0110000 },
        { "\xF8\x88\x80\x80\x80",      5, 0x0200000 },
    };
    for (const auto& r : rows) {
        memepp::string s{ r.utf8, r.len };
        auto u32 = memepp::to<std::u32string>(s);
        REQUIRE(u32.size() == 1);
        REQUIRE(u32[0] == r.want);
    }
}

TEST_CASE("memepp::to<std::u32string> — embedded NUL preserved", "[u32string]")
{
    const char input[] = { 'a', '\0', 'b', '\0', 'c' };
    memepp::string s{ input, sizeof(input) };
    auto u32 = memepp::to<std::u32string>(s);

    REQUIRE(u32.size() == 5);
    REQUIRE(u32[0] == U'a');
    REQUIRE(u32[1] == 0);
    REQUIRE(u32[2] == U'b');
    REQUIRE(u32[3] == 0);
    REQUIRE(u32[4] == U'c');
}

TEST_CASE("memepp::to<std::u32string> — overlong U+0000 becomes one U+FFFD per byte", "[u32string]")
{
    // Matches memepp::to<std::wstring>: a multi-byte encoding of U+0000 must not
    // put a NUL code unit into the result.
    struct row { const char* bytes; int len; const char* label; };
    const row rows[] = {
        { "\xC0\x80",                        2, "C0 80" },
        { "\xE0\x80\x80",                    3, "E0 80 80" },
        { "\xF0\x80\x80\x80",                4, "F0 80 80 80" },
        { "\xF8\x80\x80\x80\x80",            5, "F8 80 80 80 80" },
        { "\xFC\x80\x80\x80\x80\x80",        6, "FC 80 80 80 80 80" },
    };
    for (const auto& r : rows) {
        memepp::string alone{ r.bytes, static_cast<mmint_t>(r.len) };
        auto u32 = memepp::to<std::u32string>(alone);
        INFO(r.label);
        REQUIRE(u32.size() == static_cast<size_t>(r.len));
        for (int i = 0; i < r.len; ++i)
            REQUIRE(u32[static_cast<size_t>(i)] == static_cast<char32_t>(0xFFFD));

        // Spliced between ASCII: the trailing character must not be dropped.
        std::string spliced = "A";
        spliced.append(r.bytes, static_cast<size_t>(r.len));
        spliced.push_back('B');
        memepp::string s{ spliced.data(), static_cast<mmint_t>(spliced.size()) };

        auto ws = memepp::to<std::u32string>(s);
        INFO(r.label);
        REQUIRE(ws.size() == static_cast<size_t>(r.len) + 2);
        REQUIRE(ws[0] == 0x0041);
        for (int i = 0; i < r.len; ++i)
            REQUIRE(ws[static_cast<size_t>(i) + 1] == static_cast<char32_t>(0xFFFD));
        REQUIRE(ws[static_cast<size_t>(r.len) + 1] == 0x0042);
    }
}

TEST_CASE("memepp::to<std::u32string> — a real NUL is untouched", "[u32string]")
{
    memepp::string real_nul{ "A" "\x00" "B", 3 };
    auto u32 = memepp::to<std::u32string>(real_nul);
    REQUIRE(u32.size() == 3);
    REQUIRE(u32[0] == 0x0041);
    REQUIRE(u32[1] == 0x0000);
    REQUIRE(u32[2] == 0x0042);
}

TEST_CASE("memepp::to<std::u32string> — invalid UTF-8 truncates", "[u32string]")
{
    struct row { const char* bytes; mmint_t len; size_t want_units; };
    const row rows[] = {
        { "A" "\xFF" "B", 3, 1 },   /* stops at FF, 'B' lost */
        { "\xE4\xB8",     2, 0 },    /* truncated sequence, all lost */
    };
    for (const auto& r : rows) {
        memepp::string s{ r.bytes, r.len };
        auto u32 = memepp::to<std::u32string>(s);
        REQUIRE(u32.size() == r.want_units);
    }
}

TEST_CASE("memepp::to<std::u32string> — long input", "[u32string]")
{
    std::string utf8;
    for (int i = 0; i < 300; ++i)
        utf8.append("\xE4\xB8\xAD");
    memepp::string s{ utf8.data(), static_cast<mmint_t>(utf8.size()) };
    REQUIRE(s.storage_type() == memepp::string_storage_t::large);

    auto u32 = memepp::to<std::u32string>(s);
    REQUIRE(u32.size() == 300);
    for (auto u : u32)
        REQUIRE(u == 0x4E2D);
}

TEST_CASE("memepp::to<std::u32string> — roundtrip preserves content", "[u32string]")
{
    memepp::string s{ "A" "\xF0" "\x9F" "\x98" "\x80" "\xE4\xB8\xAD" "B", 10 };
    auto u32 = memepp::to<std::u32string>(s);
    REQUIRE(memepp::from(u32) == s);
}

TEST_CASE("memepp::from(std::u32string) — roundtrip preserves supplementary plane", "[u32string]")
{
    std::u32string u32;
    u32.push_back(U'A');
    u32.push_back(static_cast<char32_t>(0x1F600));
    u32.push_back(static_cast<char32_t>(0x4E2D));
    u32.push_back(U'B');

    auto s = memepp::from(u32);
    REQUIRE(memepp::to<std::u32string>(s) == u32);
}
