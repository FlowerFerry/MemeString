
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>
#include <memepp/convert/std/string.hpp>
#include <string>

// ---------------------------------------------------------------------------
// memepp::string_view — trim_prefix
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view trim_prefix — found", "[string_view_trim_prefix]")
{
    memepp::string_view sv = "Hello World";
    REQUIRE(sv.trim_prefix("Hello ") == memepp::string_view{ "World" });
}

TEST_CASE("memepp::string_view trim_prefix — not found", "[string_view_trim_prefix]")
{
    memepp::string_view sv = "Hello World";
    REQUIRE(sv.trim_prefix("World") == sv);
}

TEST_CASE("memepp::string_view trim_prefix — empty prefix", "[string_view_trim_prefix]")
{
    memepp::string_view sv = "Hello";
    REQUIRE(sv.trim_prefix("") == sv);
}

TEST_CASE("memepp::string_view trim_prefix — empty string", "[string_view_trim_prefix]")
{
    memepp::string_view sv = "";
    REQUIRE(sv.trim_prefix("x") == memepp::string_view{ "" });
}

TEST_CASE("memepp::string_view trim_prefix — prefix equals whole string", "[string_view_trim_prefix]")
{
    memepp::string_view sv = "Hello";
    REQUIRE(sv.trim_prefix("Hello") == memepp::string_view{ "" });
}

// ---------------------------------------------------------------------------
// memepp::string_view — trim_suffix
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view trim_suffix — found", "[string_view_trim_suffix]")
{
    memepp::string_view sv = "Hello World";
    REQUIRE(sv.trim_suffix(" World") == memepp::string_view{ "Hello" });
}

TEST_CASE("memepp::string_view trim_suffix — not found", "[string_view_trim_suffix]")
{
    memepp::string_view sv = "Hello World";
    REQUIRE(sv.trim_suffix("Hello") == sv);
}

TEST_CASE("memepp::string_view trim_suffix — empty suffix", "[string_view_trim_suffix]")
{
    memepp::string_view sv = "Hello";
    REQUIRE(sv.trim_suffix("") == sv);
}

TEST_CASE("memepp::string_view trim_suffix — empty string", "[string_view_trim_suffix]")
{
    memepp::string_view sv = "";
    REQUIRE(sv.trim_suffix("x") == memepp::string_view{ "" });
}

TEST_CASE("memepp::string_view trim_suffix — suffix equals whole string", "[string_view_trim_suffix]")
{
    memepp::string_view sv = "Hello";
    REQUIRE(sv.trim_suffix("Hello") == memepp::string_view{ "" });
}

// ---------------------------------------------------------------------------
// memepp::string — trim_prefix
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string trim_prefix — found", "[string_trim_prefix]")
{
    memepp::string s = "Hello World";
    REQUIRE(s.trim_prefix("Hello ") == memepp::string_view{ "World" });
}

TEST_CASE("memepp::string trim_prefix — not found", "[string_trim_prefix]")
{
    memepp::string s = "Hello World";
    REQUIRE(s.trim_prefix("World") == memepp::string_view{ "Hello World" });
}

TEST_CASE("memepp::string trim_prefix — empty prefix", "[string_trim_prefix]")
{
    memepp::string s = "Hello";
    REQUIRE(s.trim_prefix("") == memepp::string_view{ "Hello" });
}

TEST_CASE("memepp::string trim_prefix — prefix equals whole string", "[string_trim_prefix]")
{
    memepp::string s = "Hello";
    REQUIRE(s.trim_prefix("Hello") == memepp::string_view{ "" });
}

// ---------------------------------------------------------------------------
// memepp::string — trim_suffix
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string trim_suffix — found", "[string_trim_suffix]")
{
    memepp::string s = "Hello World";
    REQUIRE(s.trim_suffix(" World") == memepp::string_view{ "Hello" });
}

TEST_CASE("memepp::string trim_suffix — not found", "[string_trim_suffix]")
{
    memepp::string s = "Hello World";
    REQUIRE(s.trim_suffix("Hello") == memepp::string_view{ "Hello World" });
}

TEST_CASE("memepp::string trim_suffix — empty suffix", "[string_trim_suffix]")
{
    memepp::string s = "Hello";
    REQUIRE(s.trim_suffix("") == memepp::string_view{ "Hello" });
}

TEST_CASE("memepp::string trim_suffix — suffix equals whole string", "[string_trim_suffix]")
{
    memepp::string s = "Hello";
    REQUIRE(s.trim_suffix("Hello") == memepp::string_view{ "" });
}

TEST_CASE("memepp::string trim_prefix/suffix - user storage", "[string]")
{
    const mmint_t mediumLimit = MemeStringOption_getStorageMediumLimit();
    std::string data(mediumLimit + 100, 'T');
    data[0] = 'P';
    data[1] = 'R';
    data[2] = 'E';
    data[3] = 'F';
    data[4] = 'I';
    data[5] = 'X';
    memepp::string user_str = memepp::from(std::move(data));
    REQUIRE(user_str.storage_type() == memepp::string_storage_t::user);

    memepp::string trimmed = user_str.trim_prefix("PREFIX");
    REQUIRE(trimmed.size() == user_str.size() - 6);
}

TEST_CASE("memepp::string trim_prefix/suffix - view storage", "[string]")
{
    const char* src = "PREFIXHelloSUFFIX";
    mmstrstk_t view_str;
    int rc = MemeStringViewUnsafeStack_init(&view_str, sizeof(view_str),
        reinterpret_cast<const uint8_t*>(src), -1);
    REQUIRE(rc == 0);
    REQUIRE(MemeString_storageType((mmstr_cptr_t)&view_str) == MemeString_UnsafeStorageType_view);

    memepp::string_view sv(
        reinterpret_cast<const char*>(MemeString_byteData((mmstr_cptr_t)&view_str)),
        MemeString_byteSize((mmstr_cptr_t)&view_str));
    memepp::string_view trimmed = sv.trim_prefix("PREFIX").trim_suffix("SUFFIX");
    REQUIRE(trimmed == "Hello");

    MemeStringStack_unInit(&view_str, sizeof(view_str));
}
