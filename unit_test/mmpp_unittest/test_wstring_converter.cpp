#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/convert/std/wstring.hpp>

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

#if !defined(MG_OS__WIN_AVAIL) && !defined(_WIN32)
/* Non-Windows: test the UTF-32 path with supplementary plane characters */
TEST_CASE("memepp::from(std::wstring) — non-Windows: supplementary plane", "[wstring]")
{
    std::wstring ws = L"\U0001F600";  /* U+1F600 grinning face */
    memepp::string s = memepp::from(ws);
    REQUIRE(s.size() == 4);
    const uint8_t* d = reinterpret_cast<const uint8_t*>(s.data());
    REQUIRE(d[0] == 0xF0);
    REQUIRE(d[1] == 0x9F);
    REQUIRE(d[2] == 0x98);
    REQUIRE(d[3] == 0x80);
}
#endif
