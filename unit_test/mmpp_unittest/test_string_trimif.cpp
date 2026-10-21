
#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/convert/std/string.hpp>
#include <string>
#include <megopp/util/scope_cleanup.h>

TEST_CASE("memepp::string trim_if byte predicate", "[string]")
{
    // trim leading/trailing spaces
    memepp::string s01 = "  Hello, World!  ";
    auto r01 = s01.trim_if([](mmbyte_t ch) { return ch == ' '; });
    REQUIRE(r01 == "Hello, World!");

    // trim leading/trailing custom chars
    memepp::string s02 = "~~~Hello~~~";
    auto r02 = s02.trim_if([](mmbyte_t ch) { return ch == '~'; });
    REQUIRE(r02 == "Hello");

    // no trimmable chars → unchanged
    memepp::string s03 = "Hello";
    auto r03 = s03.trim_if([](mmbyte_t ch) { return ch == '~'; });
    REQUIRE(r03 == "Hello");

    // all chars trimmable → empty
    memepp::string s04 = "!!!";
    auto r04 = s04.trim_if([](mmbyte_t ch) { return ch == '!'; });
    REQUIRE(r04 == "");

    // empty string
    memepp::string s05 = "";
    auto r05 = s05.trim_if([](mmbyte_t ch) { return ch == ' '; });
    REQUIRE(r05 == "");

    // mixed leading/trailing
    memepp::string s06 = "!!Hello!";
    auto r06 = s06.trim_if([](mmbyte_t ch) { return ch == '!'; });
    REQUIRE(r06 == "Hello");
}

TEST_CASE("memepp::string trim_if_rune rune predicate", "[string]")
{
    // trim ASCII spaces via rune
    memepp::string s01 = "   abc   ";
    auto r01 = s01.trim_if_rune([](const memepp::rune& r) {
        return r == memepp::rune(' ');
    });
    REQUIRE(r01 == "abc");

    // trim multibyte Unicode ideographic space (U+3000, UTF-8: \xE3\x80\x80)
    memepp::string s02 = "\xE3\x80\x80" "abc" "\xE3\x80\x80";
    auto r02 = s02.trim_if_rune([](const memepp::rune& r) {
        // U+3000 = \xE3\x80\x80
        const char u3000[4] = { '\xE3', '\x80', '\x80', '\0' };
        return r == memepp::rune(reinterpret_cast<const uint8_t*>(u3000), 3);
    });
    REQUIRE(r02 == "abc");

    // no trimming needed
    memepp::string s03 = "Hello";
    auto r03 = s03.trim_if_rune([](const memepp::rune& r) {
        return r == memepp::rune(' ');
    });
    REQUIRE(r03 == "Hello");

    // all trimmable
    memepp::string s04 = "   ";
    auto r04 = s04.trim_if_rune([](const memepp::rune& r) {
        return r == memepp::rune(' ');
    });
    REQUIRE(r04 == "");
}

TEST_CASE("memepp::string trim_if - user storage", "[string]")
{
    const mmint_t mediumLimit = MemeStringOption_getStorageMediumLimit();
    std::string data(mediumLimit + 100, 'X');
    data[0] = 'A';
    data[1] = 'B';
    memepp::string user_str = memepp::from(std::move(data));
    REQUIRE(user_str.storage_type() == memepp::string_storage_t::user);

    memepp::string trimmed = user_str.trim_if([](uint8_t c) { return c == 'X'; });
    REQUIRE(trimmed == "AB");
}

TEST_CASE("memepp::string trim_if - view storage", "[string]")
{
    const char* src = "XXXHelloXXX";
    mmstrstk_t view_str;
    int rc = MemeStringViewUnsafeStack_init(&view_str, sizeof(view_str),
        reinterpret_cast<const uint8_t*>(src), -1);
    REQUIRE(rc == 0);
    REQUIRE(MemeString_storageType((mmstr_cptr_t)&view_str) == MemeString_UnsafeStorageType_view);

    memepp::string_view sv(
        reinterpret_cast<const char*>(MemeString_byteData((mmstr_cptr_t)&view_str)),
        MemeString_byteSize((mmstr_cptr_t)&view_str));
    memepp::string_view trimmed = sv.trim_if([](uint8_t c) { return c == 'X'; });
    REQUIRE(trimmed == "Hello");

    MemeStringStack_unInit(&view_str, sizeof(view_str));
}
