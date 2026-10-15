
#include <catch2/catch.hpp>

#include <meme/string.h>
#include <memepp/string_view.hpp>
#include <memepp/string.hpp>
#include <memepp/errc.hpp>

#include <string>

TEST_CASE("memepp::string front/back", "[string]")
{
    SECTION("byte front/back on string and string_view")
    {
        memepp::string s01_01 = "Hello World.";
        memepp::string s02_01 = "Hello World!";
        const char* sz01_01 = "Hello World.";
        const char* sz02_01 = "Hello World!";
        memepp::string_view sv01_01 = sz01_01;
        memepp::string_view sv02_01 = sz02_01;

        REQUIRE(s01_01.front() == 'H');
        REQUIRE(s01_01.back() == '.');
        REQUIRE(s02_01.front() == 'H');
        REQUIRE(s02_01.back() == '!');

        REQUIRE(sv01_01.front() == 'H');
        REQUIRE(sv01_01.back() == '.');
        REQUIRE(sv02_01.front() == 'H');
        REQUIRE(sv02_01.back() == '!');

        memepp::string s03_01 = "Hello";
        const char* sz03_01 = "Hello";
        memepp::string_view sv03_01 = sz03_01;

        REQUIRE(s03_01.front() == 'H');
        REQUIRE(s03_01.back() == 'o');

        REQUIRE(sv03_01.front() == 'H');
        REQUIRE(sv03_01.back() == 'o');

        memepp::string s04_01 = "Hello World...";
        const char* sz04_01 = "Hello World...";
        memepp::string_view sv04_01 = sz04_01;

        REQUIRE(s04_01.front() == 'H');
        REQUIRE(s04_01.back() == '.');

        REQUIRE(sv04_01.front() == 'H');
        REQUIRE(sv04_01.back() == '.');
    }

    SECTION("valid ASCII rune")
    {
        memepp::string s = "Hello World!";
        auto front = s.rune_front();
        auto back  = s.rune_back();
        REQUIRE(front == 'H');
        REQUIRE(back  == '!');
    }

    SECTION("empty string")
    {
        memepp::string s;
        REQUIRE_FALSE(s.rune_front().valid());
        REQUIRE_FALSE(s.rune_back().valid());
    }

    SECTION("truncated UTF-8")
    {
        // E6 82 00: NUL-terminated construction truncates the view at the NUL,
        // so the view is the 2 bytes { E6 82 } - a 3-byte lead with no complete rune.
        uint8_t bytes[] = { 0xE6, 0x82, 0x00 };
        memepp::string_view sv(bytes);
        REQUIRE_FALSE(sv.rune_front().valid());
        REQUIRE_FALSE(sv.rune_back().valid());
    }

    SECTION("valid front rune, invalid back rune")
    {
        // E6 82 A8 = U+60A8 (valid 3-byte). The view ends at the NUL, so its tail
        // is the truncated 2-byte sequence { E5 A5 }.
        uint8_t bytes[] = { 0xe6, 0x82, 0xa8, 0xe5, 0xa5, 0x00 };
        memepp::string_view sv(bytes);
        auto front = sv.rune_front();
        auto back  = sv.rune_back();
        REQUIRE(front == u8"您");
        REQUIRE_FALSE(back.valid());
    }
}

TEST_CASE("memepp::string front/back — single character", "[string]")
{
    memepp::string s = "X";
    REQUIRE(s.storage_type() == memepp::string_storage_t::small);
    REQUIRE(s.front() == 'X');
    REQUIRE(s.back()  == 'X');
}

TEST_CASE("memepp::string front/back — medium storage", "[string]")
{
    const mmint_t smallLimit = MemeStringOption_getStorageSmallLimit();
    std::string data(smallLimit + 10, 'M');
    data.front() = 'A';
    data.back()  = 'Z';

    memepp::string s(data.c_str(), data.size());
    REQUIRE(s.storage_type() == memepp::string_storage_t::medium);
    REQUIRE(s.front() == 'A');
    REQUIRE(s.back()  == 'Z');
}

TEST_CASE("memepp::string front/back — large storage", "[string]")
{
    const mmint_t mediumLimit = MemeStringOption_getStorageMediumLimit();
    std::string data(mediumLimit + 100, 'L');
    data.front() = 'F';
    data.back()  = 'E';

    memepp::string s(data.c_str(), data.size());
    REQUIRE(s.storage_type() == memepp::string_storage_t::large);
    REQUIRE(s.front() == 'F');
    REQUIRE(s.back()  == 'E');
}

TEST_CASE("memepp::string_view front/back — partial view of larger buffer", "[string_view]")
{
    const char* buffer = "Hello, World! This is a longer string.";

    // View built from an explicit pointer + length.
    memepp::string_view sv(buffer + 7, 5);
    REQUIRE(sv.front() == 'W');
    REQUIRE(sv.back()  == 'd');

    // View built by substr of a larger buffer.
    memepp::string_view full(buffer);
    auto sub = full.substr(7, 5);
    REQUIRE(sub.front() == 'W');
    REQUIRE(sub.back()  == 'd');
}

TEST_CASE("memepp::string rune_front/rune_back — 2-byte rune", "[string]")
{
    // U+00E9 LATIN SMALL LETTER E WITH ACUTE - UTF-8: C3 A9
    const uint8_t bytes[] = { 0xC3, 0xA9, 0x41, 0xC3, 0xA9 }; // e-acute, 'A', e-acute
    memepp::string s(reinterpret_cast<const char*>(bytes), static_cast<memepp::string::size_type>(sizeof(bytes)));

    auto front = s.rune_front();
    auto back  = s.rune_back();

    REQUIRE(front.valid());
    REQUIRE(front.size() == 2);
    REQUIRE(static_cast<unsigned char>(front.data()[0]) == 0xC3);
    REQUIRE(static_cast<unsigned char>(front.data()[1]) == 0xA9);

    REQUIRE(back.valid());
    REQUIRE(back.size() == 2);
    REQUIRE(static_cast<unsigned char>(back.data()[0]) == 0xC3);
    REQUIRE(static_cast<unsigned char>(back.data()[1]) == 0xA9);
}

TEST_CASE("memepp::string rune_front/rune_back — 4-byte rune", "[string]")
{
    // U+1F600 GRINNING FACE - UTF-8: F0 9F 98 80
    const uint8_t bytes[] = { 0xF0, 0x9F, 0x98, 0x80, 0x41, 0xF0, 0x9F, 0x98, 0x80 };
    memepp::string s(reinterpret_cast<const char*>(bytes), static_cast<memepp::string::size_type>(sizeof(bytes)));

    auto front = s.rune_front();
    auto back  = s.rune_back();

    REQUIRE(front.valid());
    REQUIRE(front.size() == 4);
    REQUIRE(static_cast<unsigned char>(front.data()[0]) == 0xF0);
    REQUIRE(static_cast<unsigned char>(front.data()[1]) == 0x9F);
    REQUIRE(static_cast<unsigned char>(front.data()[2]) == 0x98);
    REQUIRE(static_cast<unsigned char>(front.data()[3]) == 0x80);

    REQUIRE(back.valid());
    REQUIRE(back.size() == 4);
    REQUIRE(static_cast<unsigned char>(back.data()[0]) == 0xF0);
    REQUIRE(static_cast<unsigned char>(back.data()[1]) == 0x9F);
    REQUIRE(static_cast<unsigned char>(back.data()[2]) == 0x98);
    REQUIRE(static_cast<unsigned char>(back.data()[3]) == 0x80);
}

TEST_CASE("memepp::string rune_front/rune_back — invalid first byte", "[string]")
{
    SECTION("continuation byte 0x80 as lead")
    {
        const uint8_t bytes[] = { 0x80, 0x41 };
        memepp::string s(reinterpret_cast<const char*>(bytes), static_cast<memepp::string::size_type>(sizeof(bytes)));
        REQUIRE_FALSE(s.rune_front().valid());
    }

    SECTION("0xFF is never a valid lead byte")
    {
        const uint8_t bytes[] = { 0xFF, 0x41 };
        memepp::string s(reinterpret_cast<const char*>(bytes), static_cast<memepp::string::size_type>(sizeof(bytes)));
        REQUIRE_FALSE(s.rune_front().valid());
    }

    SECTION("truncated 2-byte sequence at end")
    {
        // 'A' followed by a 2-byte lead byte with no continuation byte.
        const uint8_t bytes[] = { 0x41, 0xC3 };
        memepp::string s(reinterpret_cast<const char*>(bytes), static_cast<memepp::string::size_type>(sizeof(bytes)));
        REQUIRE(s.rune_front().valid());
        REQUIRE_FALSE(s.rune_back().valid());
    }
}

TEST_CASE("memepp::string_view rune_front/rune_back — 2-byte and 4-byte", "[string_view]")
{
    SECTION("2-byte rune")
    {
        const uint8_t bytes[] = { 0xC3, 0xA9, 0x41, 0xC3, 0xA9 };
        memepp::string_view sv(reinterpret_cast<const char*>(bytes),
            static_cast<memepp::string_view::size_type>(sizeof(bytes)));

        auto front = sv.rune_front();
        auto back  = sv.rune_back();
        REQUIRE(front.valid());
        REQUIRE(front.size() == 2);
        REQUIRE(back.valid());
        REQUIRE(back.size() == 2);
    }

    SECTION("4-byte rune")
    {
        const uint8_t bytes[] = { 0xF0, 0x9F, 0x98, 0x80, 0x41, 0xF0, 0x9F, 0x98, 0x80 };
        memepp::string_view sv(reinterpret_cast<const char*>(bytes),
            static_cast<memepp::string_view::size_type>(sizeof(bytes)));

        auto front = sv.rune_front();
        auto back  = sv.rune_back();
        REQUIRE(front.valid());
        REQUIRE(front.size() == 4);
        REQUIRE(back.valid());
        REQUIRE(back.size() == 4);
    }
}

TEST_CASE("memepp::string front/back throws on empty", "[string]")
{
    memepp::string s;
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(s.front(), memepp::errc_except);
    REQUIRE_THROWS_AS(s.back(),  memepp::errc_except);
#else
    memepp::set_errc(0);
    REQUIRE(s.front() == '\0');
    REQUIRE(memepp::get_errc() != 0);
    memepp::set_errc(0);
    REQUIRE(s.back() == '\0');
    REQUIRE(memepp::get_errc() != 0);
#endif
}

TEST_CASE("memepp::string_view front/back throws on empty", "[string_view]")
{
    memepp::string_view sv;
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(sv.front(), memepp::errc_except);
    REQUIRE_THROWS_AS(sv.back(),  memepp::errc_except);
#else
    memepp::set_errc(0);
    REQUIRE(sv.front() == '\0');
    REQUIRE(memepp::get_errc() != 0);
    memepp::set_errc(0);
    REQUIRE(sv.back() == '\0');
    REQUIRE(memepp::get_errc() != 0);
#endif
}
