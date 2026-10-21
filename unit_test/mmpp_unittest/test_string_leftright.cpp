
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>
#include <memepp/convert/std/string.hpp>

#include <string>
#include <cstdint>
#include <cstring>

TEST_CASE("memepp::string left", "[string]")
{
    memepp::string str = "0123456789";
    const std::string digits = "0123456789";

    // count in [0, size()]: the first count bytes
    for (int count = 0; count <= 10; ++count) {
        CAPTURE(count);
        const std::string expected = digits.substr(0, static_cast<size_t>(count));
        REQUIRE(str.left(count) == expected.c_str());
    }

    // count >= size(): the whole string
    REQUIRE(str.left(11) == "0123456789");

    // negative count: documented-by-test behavior - returns the whole string
    REQUIRE(str.left(-1) == "0123456789");
}

TEST_CASE("memepp::string right", "[string]")
{
    memepp::string str = "0123456789";
    const std::string digits = "0123456789";

    // count in [0, size()]: the last count bytes
    for (int count = 0; count <= 10; ++count) {
        CAPTURE(count);
        const std::string expected = digits.substr(digits.size() - static_cast<size_t>(count));
        REQUIRE(str.right(count) == expected.c_str());
    }

    // count >= size(): the whole string
    REQUIRE(str.right(11) == "0123456789");

    // negative count: documented-by-test behavior - returns the whole string
    REQUIRE(str.right(-1) == "0123456789");
}

TEST_CASE("memepp::string left/right - empty string", "[string]")
{
    memepp::string str;
    REQUIRE(str.size() == 0);
    REQUIRE(str.empty());

    SECTION("left on empty string") {
        REQUIRE(str.left(0) == "");
        REQUIRE(str.left(5) == "");
        REQUIRE(str.left(-1) == "");
    }

    SECTION("right on empty string") {
        REQUIRE(str.right(0) == "");
        REQUIRE(str.right(5) == "");
        REQUIRE(str.right(-1) == "");
    }
}

TEST_CASE("memepp::string right - count > size clamping", "[string]")
{
    memepp::string str = "0123456789";
    const std::string digits = "0123456789";

    // count > size(): clamped to size() via mid_v2 negative offset handling
    // right(count) calls substr(size() - count, count)
    // When count > size(), offset becomes negative, mid_v2 clamps to 0
    SECTION("count = size + 1") {
        REQUIRE(str.right(11) == "0123456789");
    }

    SECTION("count = size + 90") {
        REQUIRE(str.right(100) == "0123456789");
    }

    SECTION("count = INTPTR_MAX") {
        REQUIRE(str.right(INTPTR_MAX) == "0123456789");
    }
}

TEST_CASE("memepp::string left/right - npos count", "[string]")
{
    memepp::string str = "0123456789";

    // npos is -1, triggers negative count branch
    // left(npos) calls substr(0, -1) -> mid_v2 treats -1 as "to end"
    // right(npos) has explicit _count < 0 check, returns *this
    REQUIRE(str.left(memepp::string::npos) == "0123456789");
    REQUIRE(str.right(memepp::string::npos) == "0123456789");
}

TEST_CASE("memepp::string left/right - multi-byte UTF-8 input", "[string]")
{
    // UTF-8: "你好世界" = 12 bytes (4 runes × 3 bytes each)
    // Use explicit byte escapes to avoid charset conversion issues
    // 你=E4 BD A0, 好=E5 A5 BD, 世=E4 B8 96, 界=E7 95 8C
    const char utf8_data[] = "\xe4\xbd\xa0\xe5\xa5\xbd\xe4\xb8\x96\xe7\x95\x8c";
    memepp::string str(utf8_data, 12);
    REQUIRE(str.size() == 12);

    SECTION("left - byte boundaries") {
        REQUIRE(str.left(0) == "");
        REQUIRE(str.left(3) == memepp::string("\xe4\xbd\xa0", 3));           // 你
        REQUIRE(str.left(6) == memepp::string("\xe4\xbd\xa0\xe5\xa5\xbd", 6)); // 你好
        REQUIRE(str.left(9) == memepp::string("\xe4\xbd\xa0\xe5\xa5\xbd\xe4\xb8\x96", 9)); // 你好世
        REQUIRE(str.left(12) == str);
    }

    SECTION("left - cutting rune mid-sequence") {
        // left(7) = first 7 bytes = 你好 + first byte of 世
        auto result = str.left(7);
        REQUIRE(result.size() == 7);
        REQUIRE(std::memcmp(result.data(), utf8_data, 7) == 0);
    }

    SECTION("right - byte boundaries") {
        REQUIRE(str.right(0) == "");
        REQUIRE(str.right(3) == memepp::string("\xe7\x95\x8c", 3));           // 界
        REQUIRE(str.right(6) == memepp::string("\xe4\xb8\x96\xe7\x95\x8c", 6)); // 世界
        REQUIRE(str.right(9) == memepp::string("\xe5\xa5\xbd\xe4\xb8\x96\xe7\x95\x8c", 9)); // 好世界
        REQUIRE(str.right(12) == str);
    }

    SECTION("right - cutting rune mid-sequence") {
        // right(7) = last 7 bytes = last byte of 好 + 世界
        auto result = str.right(7);
        REQUIRE(result.size() == 7);
        REQUIRE(std::memcmp(result.data(), utf8_data + 5, 7) == 0);
    }
}

TEST_CASE("memepp::string left/right - medium storage", "[string]")
{
    // Construct string > small limit (21 bytes on 64-bit) but <= medium limit
    // Use MEME_STRING__OBJECT_SIZE - 1 to guarantee medium storage
    const mmint_t size = MEME_STRING__OBJECT_SIZE - 1;
    std::string src(size, 'x');
    memepp::string str(src.c_str(), size);

    REQUIRE(str.storage_type() == memepp::string_storage_t::medium);
    REQUIRE(str.size() == size);

    SECTION("left - basic") {
        auto result = str.left(10);
        REQUIRE(result.size() == 10);
        REQUIRE(std::memcmp(result.data(), "xxxxxxxxxx", 10) == 0);
    }

    SECTION("right - basic") {
        auto result = str.right(10);
        REQUIRE(result.size() == 10);
        REQUIRE(std::memcmp(result.data(), "xxxxxxxxxx", 10) == 0);
    }

    SECTION("left/right - count > size") {
        REQUIRE(str.left(size + 10) == str);
        REQUIRE(str.right(size + 10) == str);
    }
}

TEST_CASE("memepp::string left/right - large storage", "[string]")
{
    // Construct string > medium limit to force large storage (heap, refcounted)
    const mmint_t size = 500;
    std::string src(size, 'y');
    memepp::string str(src.c_str(), size, memepp::string_storage_t::large);

    REQUIRE(str.storage_type() == memepp::string_storage_t::large);
    REQUIRE(str.size() == size);

    SECTION("left - basic") {
        auto result = str.left(100);
        REQUIRE(result.size() == 100);
        REQUIRE(std::memcmp(result.data(), src.substr(0, 100).c_str(), 100) == 0);
    }

    SECTION("right - basic") {
        auto result = str.right(100);
        REQUIRE(result.size() == 100);
        REQUIRE(std::memcmp(result.data(), src.substr(size - 100).c_str(), 100) == 0);
    }

    SECTION("left/right - count > size") {
        REQUIRE(str.left(size + 100) == str);
        REQUIRE(str.right(size + 100) == str);
    }

    SECTION("left/right - npos") {
        REQUIRE(str.left(memepp::string::npos) == str);
        REQUIRE(str.right(memepp::string::npos) == str);
    }

    SECTION("left/right - zero-copy path") {
        // left(0) and right(0) should return empty string
        REQUIRE(str.left(0).empty());
        REQUIRE(str.right(0).empty());

        // left(size) and right(size) should return full string
        REQUIRE(str.left(size) == str);
        REQUIRE(str.right(size) == str);
    }
}

TEST_CASE("memepp::string left/right - user storage", "[string]")
{
    const mmint_t mediumLimit = MemeStringOption_getStorageMediumLimit();
    std::string data(mediumLimit + 100, 'L');
    data[0] = 'F';
    data[1] = 'I';
    data[2] = 'R';
    data[3] = 'S';
    data[4] = 'T';
    memepp::string user_str = memepp::from(std::move(data));
    REQUIRE(user_str.storage_type() == memepp::string_storage_t::user);

    memepp::string left5 = user_str.left(5);
    REQUIRE(left5 == "FIRST");

    memepp::string right3 = user_str.right(3);
    REQUIRE(right3 == "LLL");
}

TEST_CASE("memepp::string left/right - view storage", "[string]")
{
    const char* src = "Hello World";
    mmstrstk_t view_str;
    int rc = MemeStringViewUnsafeStack_init(&view_str, sizeof(view_str),
        reinterpret_cast<const uint8_t*>(src), -1);
    REQUIRE(rc == 0);
    REQUIRE(MemeString_storageType((mmstr_cptr_t)&view_str) == MemeString_UnsafeStorageType_view);

    memepp::string_view sv(
        reinterpret_cast<const char*>(MemeString_byteData((mmstr_cptr_t)&view_str)),
        MemeString_byteSize((mmstr_cptr_t)&view_str));
    memepp::string left5 = sv.substr(0, 5).to_string();
    REQUIRE(left5 == "Hello");

    memepp::string right5 = sv.substr(sv.size() - 5).to_string();
    REQUIRE(right5 == "World");

    MemeStringStack_unInit(&view_str, sizeof(view_str));
}
