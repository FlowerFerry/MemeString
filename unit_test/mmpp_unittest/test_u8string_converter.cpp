
#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/convert/std/u8string.hpp>

#include <cstring>
#include <string>

#if defined(__cpp_char8_t)

// ============================================================================
// std::u8string <-> memepp::string
//
// u8string already holds UTF-8 bytes, so both directions are byte-for-byte
// copies with no transcoding.  The whole file is inert before C++20.
// ============================================================================

TEST_CASE("memepp::from(std::u8string) — empty", "[u8string]")
{
    std::u8string u8;
    auto s = memepp::from(u8);
    REQUIRE(s.empty());
    REQUIRE(s.size() == 0);
    REQUIRE(s.storage_type() == memepp::string_storage_t::small);
}

TEST_CASE("memepp::from(std::u8string) — ASCII", "[u8string]")
{
    std::u8string u8 = u8"hello";
    auto s = memepp::from(u8);
    REQUIRE(s == "hello");
    REQUIRE(s.size() == 5);
}

TEST_CASE("memepp::from(std::u8string) — BMP Chinese", "[u8string]")
{
    std::u8string u8 = u8"\u4E2D";
    auto s = memepp::from(u8);
    REQUIRE(s.size() == 3);
    const uint8_t* d = reinterpret_cast<const uint8_t*>(s.data());
    REQUIRE(d[0] == 0xE4);
    REQUIRE(d[1] == 0xB8);
    REQUIRE(d[2] == 0xAD);
}

TEST_CASE("memepp::from(std::u8string) — supplementary plane", "[u8string]")
{
    std::u8string u8 = u8"\U0001F600";
    auto s = memepp::from(u8);
    REQUIRE(s.size() == 4);
    const uint8_t* d = reinterpret_cast<const uint8_t*>(s.data());
    REQUIRE(d[0] == 0xF0);
    REQUIRE(d[1] == 0x9F);
    REQUIRE(d[2] == 0x98);
    REQUIRE(d[3] == 0x80);
}

TEST_CASE("memepp::from(std::u8string) — bytes are copied, not shared", "[u8string]")
{
    std::u8string u8 = u8"\u4E2D";
    auto s = memepp::from(u8);

    // Mutating the source must not reach the copy.
    u8.assign(u8"X");
    REQUIRE(s.size() == 3);
    REQUIRE(s == memepp::string{ "\xE4\xB8\xAD", 3 });
}

TEST_CASE("memepp::from(std::u8string) — rvalue overload clears the source", "[u8string]")
{
    std::u8string u8 = u8"hello";
    auto s = memepp::from(std::move(u8));

    REQUIRE(s == "hello");
    REQUIRE(u8.empty());
    REQUIRE(u8.size() == 0);
}

TEST_CASE("memepp::from(std::u8string) — lvalue keeps the source", "[u8string]")
{
    std::u8string u8 = u8"\u4E2D";
    auto s = memepp::from(u8);

    REQUIRE(s.size() == 3);
    REQUIRE(u8.size() == 3);
}

TEST_CASE("memepp::from(std::u8string) — embedded NUL preserved", "[u8string]")
{
    std::u8string u8;
    u8.push_back(u8'a');
    u8.push_back(static_cast<char8_t>(0));
    u8.push_back(u8'b');

    auto s = memepp::from(u8);
    REQUIRE(s.size() == 3);
    REQUIRE(s.data()[0] == 'a');
    REQUIRE(s.data()[1] == 0);
    REQUIRE(s.data()[2] == 'b');
}

TEST_CASE("memepp::from(std::u8string) — long input reaches large storage", "[u8string]")
{
    std::u8string u8;
    for (int i = 0; i < 300; ++i)
        u8.append(u8"\u4E2D");

    auto s = memepp::from(u8);
    REQUIRE(s.size() == 900);
    REQUIRE(s.storage_type() == memepp::string_storage_t::large);
}

// ---------------------------------------------------------------------------
// to: UTF-8 string -> u8string
// ---------------------------------------------------------------------------

TEST_CASE("memepp::to<std::u8string> — empty", "[u8string]")
{
    memepp::string empty;
    REQUIRE(memepp::to<std::u8string>(empty).empty());

    memepp::string_view empty_view;
    REQUIRE(memepp::to<std::u8string>(empty_view).empty());
}

TEST_CASE("memepp::to<std::u8string> — ASCII and BMP", "[u8string]")
{
    memepp::string ascii{ "hello" };
    auto u8a = memepp::to<std::u8string>(ascii);
    REQUIRE(u8a.size() == 5);
    REQUIRE(u8a == u8"hello");

    memepp::string bmp{ "\xE4\xB8\xAD", 3 };
    auto u8b = memepp::to<std::u8string>(bmp);
    REQUIRE(u8b.size() == 3);
    REQUIRE(u8b[0] == static_cast<char8_t>(0xE4));
    REQUIRE(u8b[1] == static_cast<char8_t>(0xB8));
    REQUIRE(u8b[2] == static_cast<char8_t>(0xAD));
}

TEST_CASE("memepp::to<std::u8string> — supplementary plane", "[u8string]")
{
    memepp::string s{ "A" "\xF0" "\x9F" "\x98" "\x80" "B", 6 };
    auto u8 = memepp::to<std::u8string>(s);

    REQUIRE(u8.size() == 6);
    REQUIRE(u8[0] == u8'A');
    REQUIRE(u8[1] == static_cast<char8_t>(0xF0));
    REQUIRE(u8[2] == static_cast<char8_t>(0x9F));
    REQUIRE(u8[3] == static_cast<char8_t>(0x98));
    REQUIRE(u8[4] == static_cast<char8_t>(0x80));
    REQUIRE(u8[5] == u8'B');

    /* string_view overload agrees. */
    auto u8v = memepp::to<std::u8string>(memepp::string_view{ s });
    REQUIRE(u8v == u8);
}

TEST_CASE("memepp::to<std::u8string> — embedded NUL preserved", "[u8string]")
{
    const char input[] = { 'a', '\0', 'b' };
    memepp::string s{ input, sizeof(input) };
    auto u8 = memepp::to<std::u8string>(s);

    REQUIRE(u8.size() == 3);
    REQUIRE(u8[0] == u8'a');
    REQUIRE(u8[1] == static_cast<char8_t>(0));
    REQUIRE(u8[2] == u8'b');
}

TEST_CASE("memepp::to<std::u8string> — 5/6-byte sequences pass through", "[u8string]")
{
    // No transcoding, so the library's extended UTF-8 (5/6-byte, up to 31 bits)
    // survives the round trip untouched even though it is not valid Unicode.
    memepp::string s{ "\xF8" "\x88" "\x80" "\x80" "\x80", 5 };
    auto u8 = memepp::to<std::u8string>(s);

    REQUIRE(u8.size() == 5);
    REQUIRE(std::memcmp(u8.data(), s.data(), 5) == 0);
    REQUIRE(memepp::from(u8) == s);
}

TEST_CASE("memepp::to<std::u8string> — roundtrip preserves content", "[u8string]")
{
    memepp::string s{ "A" "\xF0" "\x9F" "\x98" "\x80" "\xE4\xB8\xAD" "B", 10 };
    auto u8 = memepp::to<std::u8string>(s);
    REQUIRE(memepp::from(u8) == s);
}

#endif // __cpp_char8_t
