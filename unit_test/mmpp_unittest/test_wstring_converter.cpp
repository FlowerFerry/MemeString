#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/errc.hpp>
#include <memepp/convert/std/wstring.hpp>

#include <mego/predef/os/windows.h>

#include <cstring>
#include <string>

/*
 * Test the std::wstring → memepp::string converter (BUG-014 fix).
 *
 * Platform semantics:
 *   Windows:  wchar_t is UTF-16 → simple reinterpret_cast<uint16_t*>
 *   non-Win:  wchar_t is UTF-32 → per-codepoint conversion to UTF-8
 */

TEST_CASE("memepp::from(std::wstring) — empty", "[wstring]")
{
    std::wstring ws;
    memepp::string s = memepp::from(ws);
    REQUIRE(s.empty());
    REQUIRE(s.size() == 0);
    REQUIRE(s.storage_type() == memepp::string_storage_t::small);
}

TEST_CASE("memepp::from(std::wstring) — ASCII only", "[wstring]")
{
    std::wstring ws = L"hello";
    memepp::string s = memepp::from(ws);
    REQUIRE(s == "hello");
    REQUIRE(s.size() == 5);
}

TEST_CASE("memepp::from(std::wstring) — single BMP Chinese character", "[wstring]")
{
    /* U+4E2D (中) is a BMP character. In UTF-16 it is a single code unit.
       In UTF-32 it is a single code unit. Both paths should produce the
       same 3-byte UTF-8 result. */
    std::wstring ws = L"\x4E2D";  /* '中' */
    memepp::string s = memepp::from(ws);
    REQUIRE(s.size() == 3);
    /* Check that the UTF-8 bytes are 0xE4 0xB8 0xAD */
    const uint8_t* d = reinterpret_cast<const uint8_t*>(s.data());
    REQUIRE(d[0] == 0xE4);
    REQUIRE(d[1] == 0xB8);
    REQUIRE(d[2] == 0xAD);
}

TEST_CASE("memepp::from(std::wstring) — roundtrip with std::wstring", "[wstring]")
{
    std::wstring ws = L"test string 123";
    memepp::string s = memepp::from(ws);
    memepp::string expected("test string 123");
    REQUIRE(s == expected);
}

// ---------------------------------------------------------------------------
// Supplementary plane.  Runs unconditionally: the input literal takes a
// different shape per platform (UTF-16 surrogate pair vs. one UTF-32 code
// unit) but the required UTF-8 output is identical.
// ---------------------------------------------------------------------------

TEST_CASE("memepp::from(std::wstring) — supplementary plane U+1F600", "[wstring]")
{
    std::wstring ws = L"\U0001F600";  /* U+1F600 grinning face */

#if MG_OS__WIN_AVAIL
    // UTF-16: a surrogate pair, so two code units.
    REQUIRE(ws.size() == 2);
    REQUIRE(static_cast<uint16_t>(ws[0]) == 0xD83D);
    REQUIRE(static_cast<uint16_t>(ws[1]) == 0xDE00);
#else
    // UTF-32: one code unit holding the code point directly.
    REQUIRE(ws.size() == 1);
#endif

    memepp::string s = memepp::from(ws);
    REQUIRE(s.size() == 4);
    const uint8_t* d = reinterpret_cast<const uint8_t*>(s.data());
    REQUIRE(d[0] == 0xF0);
    REQUIRE(d[1] == 0x9F);
    REQUIRE(d[2] == 0x98);
    REQUIRE(d[3] == 0x80);
}

TEST_CASE("memepp::from(std::wstring) — supplementary plane spliced into text", "[wstring]")
{
    std::wstring ws = L"A\U0001F600B";
    memepp::string s = memepp::from(ws);

    REQUIRE(s.size() == 6);
    const uint8_t* d = reinterpret_cast<const uint8_t*>(s.data());
    REQUIRE(d[0] == 0x41);               /* 'A' */
    REQUIRE(d[1] == 0xF0);
    REQUIRE(d[2] == 0x9F);
    REQUIRE(d[3] == 0x98);
    REQUIRE(d[4] == 0x80);
    REQUIRE(d[5] == 0x42);               /* 'B' */
}

#if MG_OS__WIN_AVAIL
// ---------------------------------------------------------------------------
// Malformed UTF-16 (lone / mis-paired surrogates).
//
// These tests pin the CURRENT behaviour, which is silent truncation: the UTF-16
// decoder stops at the first unit it cannot pair and everything from there on is
// dropped.  MemeStringStack_initByU16bytesAndType pre-sizes the buffer with
// mmutf_char_size_u8from16 and then fills it with mmutf_convert_u16to8; both use
// the same rune parser, so they stop at the same unit, pos == u8len, and the
// function reports success.  memepp::from therefore returns a SHORT string with
// errc() left at 0 — there is no error signal to the caller.
//
// TODO(impl): conversion should report MGEC__INVAL instead of silently
//             returning a truncated string.  Until it does, these assertions
//             describe a defect, not a contract.
// ---------------------------------------------------------------------------

TEST_CASE("memepp::from(std::wstring) — lone high surrogate yields empty", "[wstring]")
{
    std::wstring ws(1, static_cast<wchar_t>(0xD83D));

    memepp::set_errc(MGEC__OK);
    memepp::string s = memepp::from(ws);

    REQUIRE(s.empty());
    REQUIRE(memepp::get_errc() == MGEC__OK);   /* no error reported */
}

TEST_CASE("memepp::from(std::wstring) — lone low surrogate yields empty", "[wstring]")
{
    std::wstring ws(1, static_cast<wchar_t>(0xDE00));

    memepp::set_errc(MGEC__OK);
    memepp::string s = memepp::from(ws);

    REQUIRE(s.empty());
    REQUIRE(memepp::get_errc() == MGEC__OK);
}

TEST_CASE("memepp::from(std::wstring) — high surrogate followed by non-low truncates", "[wstring]")
{
    std::wstring ws;
    ws.push_back(L'A');
    ws.push_back(static_cast<wchar_t>(0xD83D));
    ws.push_back(L'B');

    memepp::set_errc(MGEC__OK);
    memepp::string s = memepp::from(ws);

    // 'A' survives, the unpaired high surrogate and everything after it is
    // dropped — 'B' never reaches the output.
    REQUIRE(s == "A");
    REQUIRE(s.size() == 1);
    REQUIRE(memepp::get_errc() == MGEC__OK);
}

TEST_CASE("memepp::from(std::wstring) — leading lone surrogate truncates the rest", "[wstring]")
{
    std::wstring ws;
    ws.push_back(static_cast<wchar_t>(0xD83D));
    ws.push_back(L'A');

    memepp::set_errc(MGEC__OK);
    memepp::string s = memepp::from(ws);

    // The offending unit is in front, so nothing is emitted at all.
    REQUIRE(s.empty());
    REQUIRE(memepp::get_errc() == MGEC__OK);
}

TEST_CASE("memepp::from(std::wstring) — two high surrogates yield empty", "[wstring]")
{
    std::wstring ws;
    ws.push_back(static_cast<wchar_t>(0xD83D));
    ws.push_back(static_cast<wchar_t>(0xD83D));

    memepp::set_errc(MGEC__OK);
    memepp::string s = memepp::from(ws);

    REQUIRE(s.empty());
    REQUIRE(memepp::get_errc() == MGEC__OK);
}

TEST_CASE("memepp::from(std::wstring) — lone low surrogate before text truncates", "[wstring]")
{
    std::wstring ws;
    ws.push_back(static_cast<wchar_t>(0xDE00));
    ws.push_back(L'A');

    memepp::set_errc(MGEC__OK);
    memepp::string s = memepp::from(ws);

    REQUIRE(s.empty());
    REQUIRE(memepp::get_errc() == MGEC__OK);
}

TEST_CASE("memepp::from(std::wstring) — trailing lone high surrogate keeps prefix", "[wstring]")
{
    std::wstring ws;
    ws.push_back(L'A');
    ws.push_back(static_cast<wchar_t>(0xD83D));

    memepp::set_errc(MGEC__OK);
    memepp::string s = memepp::from(ws);

    REQUIRE(s == "A");
    REQUIRE(s.size() == 1);
    REQUIRE(memepp::get_errc() == MGEC__OK);
}
#endif // MG_OS__WIN_AVAIL

// ---------------------------------------------------------------------------
// from(std::wstring&&) and mm_from(wstring) — rvalue / free-function paths.
// ---------------------------------------------------------------------------

TEST_CASE("memepp::from(std::wstring) — rvalue overload", "[wstring]")
{
    // 4-byte supplementary-plane character in the middle of ASCII text.
    std::wstring ws = L"A\U0001F600B";
    auto s = memepp::from(std::move(ws));

    REQUIRE(s.size() == 6);
    const uint8_t* d = reinterpret_cast<const uint8_t*>(s.data());
    REQUIRE(d[0] == 0x41);               /* 'A' */
    REQUIRE(d[1] == 0xF0);
    REQUIRE(d[2] == 0x9F);
    REQUIRE(d[3] == 0x98);
    REQUIRE(d[4] == 0x80);
    REQUIRE(d[5] == 0x42);               /* 'B' */

    // The rvalue overload now clears the source after moving the data out,
    // consistent with memepp::string(string&&) semantics.
    REQUIRE(ws.empty());
    REQUIRE(ws.size() == 0);
}

TEST_CASE("mm_from(std::wstring) forwards to memepp::from", "[wstring]")
{
    // lvalue
    std::wstring a = L"\x4E2D";
    REQUIRE(mm_from(a) == memepp::from(a));

    // rvalue
    std::wstring b = L"\U0001F600";
    auto s = mm_from(std::move(b));
    REQUIRE(s.size() == 4);
    const uint8_t* d = reinterpret_cast<const uint8_t*>(s.data());
    REQUIRE(d[0] == 0xF0);
    REQUIRE(d[1] == 0x9F);
    REQUIRE(d[2] == 0x98);
    REQUIRE(d[3] == 0x80);
    REQUIRE(b.empty());                   /* source cleared by the rvalue path */
}

// ---------------------------------------------------------------------------
// from(std::wstring_view) — C++17 only.
// ---------------------------------------------------------------------------

#if MG_LANG__CXX17_AVAIL
TEST_CASE("memepp::from(std::wstring_view) — C++17", "[wstring]")
{
    std::wstring backing = L"A\U0001F600B";
    std::wstring_view sv{ backing };

    // wchar_t is UTF-16 on Windows (surrogate pair: A, D83D, DE00, B) and
    // UTF-32 elsewhere (A, U+1F600, B).
#if MG_OS__WIN_AVAIL
    const size_t kBackingUnits = 4;
#else
    const size_t kBackingUnits = 3;
#endif

    auto s1 = memepp::from(sv);                        /* lvalue: source kept */
    REQUIRE(!sv.empty());
    REQUIRE(sv.size() == kBackingUnits);

    std::wstring_view moved{ backing };
    auto s2 = memepp::from(std::move(moved));          /* rvalue: source cleared */
    REQUIRE(moved.empty());
    REQUIRE(moved.size() == 0);

    // Both platforms produce the same UTF-8 output.
    REQUIRE(s1.size() == 6);
    REQUIRE(s2 == s1);
    const uint8_t* d = reinterpret_cast<const uint8_t*>(s1.data());
    REQUIRE(d[0] == 0x41);
    REQUIRE(d[1] == 0xF0);
    REQUIRE(d[5] == 0x42);

    // The view does not own the data, so the backing is untouched.
    REQUIRE(backing.size() == kBackingUnits);
}
#endif // MG_LANG__CXX17_AVAIL

// ---------------------------------------------------------------------------
// to<std::wstring> — UTF-8 → UTF-16/UTF-32, both overloads.
// ---------------------------------------------------------------------------

TEST_CASE("memepp::to<std::wstring> — 4-byte char becomes a surrogate pair", "[wstring]")
{
    // Split hex escapes so the compiler doesn't merge them with trailing chars.
    memepp::string s{ "A" "\xF0" "\x9F" "\x98" "\x80" "B", 6 };

    auto w = memepp::to<std::wstring>(s);
#if MG_OS__WIN_AVAIL
    // UTF-16: A, D83D, DE00, B  -> 4 code units
    REQUIRE(w.size() == 4);
    REQUIRE(static_cast<uint16_t>(w[0]) == 0x0041);
    REQUIRE(static_cast<uint16_t>(w[1]) == 0xD83D);    /* high surrogate */
    REQUIRE(static_cast<uint16_t>(w[2]) == 0xDE00);    /* low surrogate */
    REQUIRE(static_cast<uint32_t>(w[3]) == 0x0042);
#else
    // UTF-32: A, U+1F600, B  -> 3 code units
    REQUIRE(w.size() == 3);
    REQUIRE(static_cast<uint32_t>(w[0]) == 0x0041);
    REQUIRE(static_cast<uint32_t>(w[1]) == 0x1F600);   /* single code unit */
    REQUIRE(static_cast<uint32_t>(w[2]) == 0x0042);
#endif

    // string_view overload produces the same result.
    auto w2 = memepp::to<std::wstring>(memepp::string_view{ s });
    REQUIRE(w2 == w);
}

TEST_CASE("memepp::to<std::wstring> — BMP and empty", "[wstring]")
{
    memepp::string bmp{ "\xE4\xB8\xAD", 3 };
    auto w = memepp::to<std::wstring>(bmp);
    REQUIRE(w.size() == 1);
    REQUIRE(static_cast<uint16_t>(w[0]) == 0x4E2D);

    memepp::string empty;
    REQUIRE(memepp::to<std::wstring>(empty).empty());
}

TEST_CASE("memepp::to<std::wstring> — invalid UTF-8 truncates silently", "[wstring]")
{
    // 固定当前行为：解码遇到无效字节即停止，后面内容全部丢弃，
    // 且 errc 保持 0，调用方无法感知。
    // TODO(impl): 应返回 MGEC__INVAL 而非静默截断。
    //
    // 例外：多字节编码的 U+0000（C0 80 / E0 80 80 / F0 80 80 80 /
    // F8 80 80 80 80 / FC 80 80 80 80 80）不产生 NUL 码元，而是转成
    // U+FFFD，避免消费者按 C 字符串处理后内容被截断。
    struct row { const char* bytes; int len; size_t want_units; uint32_t first_unit; };
    const row rows[] = {
        { "A" "\xFF" "B",  3, 1, 0x0041 },   /* stops at FF, 'B' lost */
        { "\xE4\xB8",      2, 0, 0 },         /* truncated sequence, all lost */
        // Overlong U+0000: one U+FFFD per source byte.  UTF-16 substitutes
        // U+FFFD so the result is not silently truncated by C-string consumers;
        // UTF-32 holds the value directly and reports the same width.
        { "\xC0\x80",      2, 2, 0xFFFD },
    };
    for (const auto& r : rows) {
        memepp::string bad{ r.bytes, r.len };
        memepp::set_errc(MGEC__OK);
        auto w = memepp::to<std::wstring>(bad);
        REQUIRE(w.size() == r.want_units);
        if (r.want_units > 0)
            REQUIRE(static_cast<uint32_t>(w[0]) == r.first_unit);
        REQUIRE(memepp::get_errc() == MGEC__OK);
    }
}

TEST_CASE("memepp::to<std::wstring> — overlong U+0000 becomes one U+FFFD per byte", "[wstring]")
{
    // A multi-byte encoding of U+0000 must not reach the UTF-16 output as a NUL
    // code unit: that would silently truncate any consumer treating the result
    // as a C string.  Every byte of such a sequence becomes its own U+FFFD, so
    // a 2-byte sequence yields two replacement characters and a 6-byte sequence
    // yields six.  Text around it survives untouched.
    struct row { const char* bytes; int len; const char* label; };
    const row rows[] = {
        { "\xC0\x80",                        2, "C0 80" },
        { "\xE0\x80\x80",                    3, "E0 80 80" },
        { "\xF0\x80\x80\x80",                4, "F0 80 80 80" },
        { "\xF8\x80\x80\x80\x80",            5, "F8 80 80 80 80" },
        { "\xFC\x80\x80\x80\x80\x80",        6, "FC 80 80 80 80 80" },
    };

#if MG_OS__WIN_AVAIL
    for (const auto& r : rows) {
        // Standalone: one U+FFFD per source byte.
        memepp::string alone{ r.bytes, static_cast<mmint_t>(r.len) };
        auto w = memepp::to<std::wstring>(alone);
        INFO(r.label);
        REQUIRE(w.size() == static_cast<size_t>(r.len));
        for (int i = 0; i < r.len; ++i)
            REQUIRE(static_cast<uint16_t>(w[static_cast<size_t>(i)]) == 0xFFFD);

        // Spliced between ASCII: the trailing character must not be dropped.
        std::string spliced = "A";
        spliced.append(r.bytes, static_cast<size_t>(r.len));
        spliced.push_back('B');
        memepp::string s{ spliced.data(), static_cast<mmint_t>(spliced.size()) };

        auto ws = memepp::to<std::wstring>(s);
        INFO(r.label);
        REQUIRE(ws.size() == static_cast<size_t>(r.len) + 2);
        REQUIRE(static_cast<uint16_t>(ws[0]) == 0x0041);
        for (int i = 0; i < r.len; ++i)
            REQUIRE(static_cast<uint16_t>(ws[static_cast<size_t>(i) + 1]) == 0xFFFD);
        REQUIRE(static_cast<uint16_t>(ws[static_cast<size_t>(r.len) + 1]) == 0x0042);
    }
#else
    // UTF-32 can hold U+0000 directly, so the substitution does not apply and
    // the decoded code point is preserved byte-count-independently.
    for (const auto& r : rows) {
        memepp::string alone{ r.bytes, static_cast<mmint_t>(r.len) };
        auto w = memepp::to<std::wstring>(alone);
        INFO(r.label);
        REQUIRE(w.size() == static_cast<size_t>(r.len));
        for (int i = 0; i < r.len; ++i)
            REQUIRE(static_cast<uint32_t>(w[static_cast<size_t>(i)]) == 0x0000FFFD);
    }
#endif
}

TEST_CASE("memepp::to<std::wstring> — a real NUL and other overlongs are untouched", "[wstring]")
{
#if MG_OS__WIN_AVAIL
    // A single 0x00 byte is a legitimate U+0000 and must stay a NUL code unit.
    memepp::string real_nul{ "A" "\x00" "B", 3 };
    auto w1 = memepp::to<std::wstring>(real_nul);
    REQUIRE(w1.size() == 3);
    REQUIRE(static_cast<uint16_t>(w1[0]) == 0x0041);
    REQUIRE(static_cast<uint16_t>(w1[1]) == 0x0000);
    REQUIRE(static_cast<uint16_t>(w1[2]) == 0x0042);

    // Overlong encodings of non-zero code points keep their value.
    struct row { const char* bytes; mmint_t len; uint16_t want; };
    const row rows[] = {
        { "\xC1\xBF",     2, 0x007F },   /* overlong U+007F */
        { "\xE0\x9F\xBF", 3, 0x07FF },   /* overlong U+07FF */
    };
    for (const auto& r : rows) {
        memepp::string s{ r.bytes, r.len };
        auto w = memepp::to<std::wstring>(s);
        REQUIRE(w.size() == 1);
        REQUIRE(static_cast<uint16_t>(w[0]) == r.want);
    }
#endif
}

TEST_CASE("memepp::to<std::wstring> — roundtrip preserves supplementary plane", "[wstring]")
{
    memepp::string s{ "A" "\xF0" "\x9F" "\x98" "\x80" "B", 6 };
    auto w = memepp::to<std::wstring>(s);
    REQUIRE(memepp::from(w) == s);
}

