
#include <catch2/catch.hpp>

#include <megopp/util/scope_cleanup.h>
#include <memepp/string_view.hpp>
#include <memepp/string.hpp>
#include <memepp/convert/std/string.hpp>

#include <string>

TEST_CASE("memepp::string mid", "[string]")
{
    mgec_t ec = 0;
    const mmint_t limit = MemeStringOption_getStorageMediumLimit();
    REQUIRE(limit > MemeStringOption_getStorageSmallLimit()); // medium tier must exist

    // Length driven by the live medium limit, so the source is always medium.
    const char* const kBase = "Hello, World! ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz";
    const size_t kBaseLen = 67;
    const size_t targetLen = kBaseLen < (size_t)limit ? kBaseLen : (size_t)limit;
    memepp::string str01_01(kBase, targetLen);
    REQUIRE(str01_01.storage_type() == memepp::string_storage_t::medium);

    SECTION("obj_size > 0: out uninitialized on entry")
    {
        // Each call passes MMSTR__OBJ_SIZE (positive), so the function treats
        // _out as uninitialized and initializes it before writing.  We must
        // uninit the previous result before each subsequent call to avoid leaks.
        mmstrstk_t ret01_01;

        ec = MemeStringStack_mid_v2(&str01_01.native_handle(), 0, 0, &ret01_01, MMSTR__OBJ_SIZE);
        REQUIRE(ec == 0);
        REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret01_01) == 0);
        REQUIRE(MemeString_compareByUtf8bytes(
            (mmstr_cptr_t)&ret01_01, (const uint8_t*)"", -1) == 0);
        REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret01_01) == mmstr_strg_small);

        mmstrstk_uninit_v0(&ret01_01, MMSTR__OBJ_SIZE);
        ec = MemeStringStack_mid_v2(&str01_01.native_handle(), 0, 1, &ret01_01, MMSTR__OBJ_SIZE);
        REQUIRE(ec == 0);
        REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret01_01) == 1);
        REQUIRE(MemeString_compareByUtf8bytes(
            (mmstr_cptr_t)&ret01_01, (const uint8_t*)"H", -1) == 0);
        REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret01_01) == mmstr_strg_small);

        mmstrstk_uninit_v0(&ret01_01, MMSTR__OBJ_SIZE);
        ec = MemeStringStack_mid_v2(&str01_01.native_handle(), -1, 2, &ret01_01, MMSTR__OBJ_SIZE);
        REQUIRE(ec == 0);
        REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret01_01) == 2);
        REQUIRE(MemeString_compareByUtf8bytes(
            (mmstr_cptr_t)&ret01_01, (const uint8_t*)"He", -1) == 0);
        REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret01_01) == mmstr_strg_small);

        mmstrstk_uninit_v0(&ret01_01, MMSTR__OBJ_SIZE);
        ec = MemeStringStack_mid_v2(&str01_01.native_handle(), 0, INTPTR_MAX, &ret01_01, MMSTR__OBJ_SIZE);
        REQUIRE(ec == 0);
        REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret01_01) == str01_01.size());
        REQUIRE(MemeString_compareByUtf8bytes((mmstr_cptr_t)&ret01_01, str01_01.bytes(), -1) == 0);
        REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret01_01) == mmstr_strg_medium);

        mmstrstk_uninit_v0(&ret01_01, MMSTR__OBJ_SIZE);
        ec = MemeStringStack_mid_v2(&str01_01.native_handle(), 0, -1, &ret01_01, MMSTR__OBJ_SIZE);
        REQUIRE(ec == 0);
        REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret01_01) == str01_01.size());
        REQUIRE(MemeString_compareByUtf8bytes((mmstr_cptr_t)&ret01_01, str01_01.bytes(), -1) == 0);
        REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret01_01) == mmstr_strg_medium);

        mmstrstk_uninit_v0(&ret01_01, MMSTR__OBJ_SIZE);
        ec = MemeStringStack_mid_v2(&str01_01.native_handle(), INTPTR_MAX, 1, &ret01_01, MMSTR__OBJ_SIZE);
        REQUIRE(ec == 0);
        REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret01_01) == 0);
        REQUIRE(MemeString_compareByUtf8bytes(
            (mmstr_cptr_t)&ret01_01, (const uint8_t*)"", -1) == 0);
        REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret01_01) == mmstr_strg_small);

        mmstrstk_uninit_v0(&ret01_01, MMSTR__OBJ_SIZE);
        ec = MemeStringStack_mid_v2(&str01_01.native_handle(), 1, 2, &ret01_01, MMSTR__OBJ_SIZE);
        REQUIRE(ec == 0);
        REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret01_01) == 2);
        REQUIRE(MemeString_compareByUtf8bytes(
            (mmstr_cptr_t)&ret01_01, (const uint8_t*)"el", -1) == 0);
        REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret01_01) == mmstr_strg_small);

        mmstrstk_uninit_v0(&ret01_01, MMSTR__OBJ_SIZE);
        ec = MemeStringStack_mid_v2(&str01_01.native_handle(), -1, -1, &ret01_01, MMSTR__OBJ_SIZE);
        REQUIRE(ec == 0);
        REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret01_01) == str01_01.size());
        REQUIRE(MemeString_compareByUtf8bytes((mmstr_cptr_t)&ret01_01, str01_01.bytes(), -1) == 0);
        REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret01_01) == mmstr_strg_medium);

        mmstrstk_uninit_v0(&ret01_01, MMSTR__OBJ_SIZE);
    }

    SECTION("obj_size <= 0: out already initialized, impl uninits before writing")
    {
        // All calls pass -1 for _obj_size, so the function queries the object
        // size from _out and uninitializes it before writing the new result.
        // We must initialize _out before the first call.
        mmstrstk_t ret01_01;
        mmstrstk_init_v0(&ret01_01, MMSTR__OBJ_SIZE);
        MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() {
            mmstrstk_uninit_v0(&ret01_01, MMSTR__OBJ_SIZE);
        });

        ec = MemeStringStack_mid_v2(&str01_01.native_handle(), 0, 1, &ret01_01, -1);
        REQUIRE(ec == 0);
        REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret01_01) == 1);
        REQUIRE(MemeString_compareByUtf8bytes(
            (mmstr_cptr_t)&ret01_01, (const uint8_t*)"H", -1) == 0);
        REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret01_01) == mmstr_strg_small);

        ec = MemeStringStack_mid_v2(&str01_01.native_handle(), -1, 2, &ret01_01, -1);
        REQUIRE(ec == 0);
        REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret01_01) == 2);
        REQUIRE(MemeString_compareByUtf8bytes(
            (mmstr_cptr_t)&ret01_01, (const uint8_t*)"He", -1) == 0);
        REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret01_01) == mmstr_strg_small);

        ec = MemeStringStack_mid_v2(&str01_01.native_handle(), 0, INTPTR_MAX, &ret01_01, -1);
        REQUIRE(ec == 0);
        REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret01_01) == str01_01.size());
        REQUIRE(MemeString_compareByUtf8bytes((mmstr_cptr_t)&ret01_01, str01_01.bytes(), -1) == 0);
        REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret01_01) == mmstr_strg_medium);

        ec = MemeStringStack_mid_v2(&str01_01.native_handle(), 0, -1, &ret01_01, -1);
        REQUIRE(ec == 0);
        REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret01_01) == str01_01.size());
        REQUIRE(MemeString_compareByUtf8bytes((mmstr_cptr_t)&ret01_01, str01_01.bytes(), -1) == 0);
        REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret01_01) == mmstr_strg_medium);

        ec = MemeStringStack_mid_v2(&str01_01.native_handle(), INTPTR_MAX, 1, &ret01_01, -1);
        REQUIRE(ec == 0);
        REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret01_01) == 0);
        REQUIRE(MemeString_compareByUtf8bytes(
            (mmstr_cptr_t)&ret01_01, (const uint8_t*)"", -1) == 0);
        REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret01_01) == mmstr_strg_small);

        ec = MemeStringStack_mid_v2(&str01_01.native_handle(), 1, 2, &ret01_01, -1);
        REQUIRE(ec == 0);
        REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret01_01) == 2);
        REQUIRE(MemeString_compareByUtf8bytes(
            (mmstr_cptr_t)&ret01_01, (const uint8_t*)"el", -1) == 0);
        REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret01_01) == mmstr_strg_small);

        ec = MemeStringStack_mid_v2(&str01_01.native_handle(), -1, -1, &ret01_01, -1);
        REQUIRE(ec == 0);
        REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret01_01) == str01_01.size());
        REQUIRE(MemeString_compareByUtf8bytes((mmstr_cptr_t)&ret01_01, str01_01.bytes(), -1) == 0);
        REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret01_01) == mmstr_strg_medium);
    }
}
TEST_CASE("memepp::string mid - user storage", "[string]")
{
    // user storage: produced by from(std::string&&) above the medium limit.
    const mmint_t mediumLimit = MemeStringOption_getStorageMediumLimit();
    std::string data(mediumLimit + 100, 'M');
    data[0] = 'A';
    data[1] = 'B';
    memepp::string user_str = memepp::from(std::move(data));
    REQUIRE(user_str.storage_type() == memepp::string_storage_t::user);

    mmstrstk_t ret;
    mmstrstk_init_v0(&ret, MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() {
        mmstrstk_uninit_v0(&ret, MMSTR__OBJ_SIZE);
    });

    mgec_t ec = MemeStringStack_mid_v2(&user_str.native_handle(), 0, 2, &ret, MMSTR__OBJ_SIZE);
    REQUIRE(ec == 0);
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret) == 2);
    REQUIRE(MemeString_compareByUtf8bytes(
        (mmstr_cptr_t)&ret, (const uint8_t*)"AB", -1) == 0);
}

TEST_CASE("memepp::string mid - view storage", "[string]")
{
    // C++ wrapper cannot produce view-type string (constructor deep-copies views).
    // Use C API directly to create a view, then mid() on it.
    const char* src = "Hello, World!";
    mmstrstk_t view_str;
    int rc = MemeStringViewUnsafeStack_init(&view_str, sizeof(view_str),
        reinterpret_cast<const uint8_t*>(src), -1);
    REQUIRE(rc == 0);
    REQUIRE(MemeString_storageType((mmstr_cptr_t)&view_str) == MemeString_UnsafeStorageType_view);

    mmstrstk_t ret;
    mmstrstk_init_v0(&ret, MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() {
        mmstrstk_uninit_v0(&ret, MMSTR__OBJ_SIZE);
    });

    mgec_t ec = MemeStringStack_mid_v2(&view_str, 0, 5, &ret, MMSTR__OBJ_SIZE);
    REQUIRE(ec == 0);
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret) == 5);
    REQUIRE(MemeString_compareByUtf8bytes(
        (mmstr_cptr_t)&ret, (const uint8_t*)"Hello", -1) == 0);

    MemeStringStack_unInit(&view_str, sizeof(view_str));
}

TEST_CASE("memepp::string mid - small storage", "[string]")
{
    // small storage: SSO inline buffer, whose max length depends on pointer
    // width (10 bytes on 32-bit, 21 on 64-bit).  Use "HelloWorld" (10 bytes)
    // so it fits the small buffer on every target.
    memepp::string small_str = "HelloWorld";
    REQUIRE(small_str.storage_type() == memepp::string_storage_t::small);

    mmstrstk_t ret;
    mmstrstk_init_v0(&ret, MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() {
        mmstrstk_uninit_v0(&ret, MMSTR__OBJ_SIZE);
    });

    // mid(0, 5) → "Hello"
    mgec_t ec = MemeStringStack_mid_v2(&small_str.native_handle(), 0, 5, &ret, MMSTR__OBJ_SIZE);
    REQUIRE(ec == 0);
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret) == 5);
    REQUIRE(MemeString_compareByUtf8bytes(
        (mmstr_cptr_t)&ret, (const uint8_t*)"Hello", -1) == 0);
    REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret) == mmstr_strg_small);

    // mid(5, 5) → "World"
    mmstrstk_uninit_v0(&ret, MMSTR__OBJ_SIZE);
    mmstrstk_init_v0(&ret, MMSTR__OBJ_SIZE);
    ec = MemeStringStack_mid_v2(&small_str.native_handle(), 5, 5, &ret, MMSTR__OBJ_SIZE);
    REQUIRE(ec == 0);
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret) == 5);
    REQUIRE(MemeString_compareByUtf8bytes(
        (mmstr_cptr_t)&ret, (const uint8_t*)"World", -1) == 0);
    REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret) == mmstr_strg_small);

    // mid(0, 0) → empty
    mmstrstk_uninit_v0(&ret, MMSTR__OBJ_SIZE);
    mmstrstk_init_v0(&ret, MMSTR__OBJ_SIZE);
    ec = MemeStringStack_mid_v2(&small_str.native_handle(), 0, 0, &ret, MMSTR__OBJ_SIZE);
    REQUIRE(ec == 0);
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret) == 0);
    REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret) == mmstr_strg_small);
}

TEST_CASE("memepp::string mid - large storage", "[string]")
{
    // large storage: > mediumLimit, reference-counted.
    // Use constructor with string_storage_t::large hint to force large storage.
    const mmint_t mediumLimit = MemeStringOption_getStorageMediumLimit();
    std::string data(mediumLimit + 50, 'X');
    data[0] = 'A';
    data[1] = 'B';
    data[2] = 'C';
    data[3] = 'D';
    data[4] = 'E';
    data[mediumLimit + 45] = 'Z';
    memepp::string large_str(data.data(), data.size(), memepp::string_storage_t::large);
    REQUIRE(large_str.storage_type() == memepp::string_storage_t::large);

    mmstrstk_t ret;
    mmstrstk_init_v0(&ret, MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() {
        mmstrstk_uninit_v0(&ret, MMSTR__OBJ_SIZE);
    });

    // mid(0, srcSize) → shared zero-copy branch (result still large, refcount+1)
    mgec_t ec = MemeStringStack_mid_v2(&large_str.native_handle(), 0, large_str.size(), &ret, MMSTR__OBJ_SIZE);
    REQUIRE(ec == 0);
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret) == large_str.size());
    REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret) == mmstr_strg_large);

    // mid(5, 10) → deep copy branch (result is small/medium)
    mmstrstk_uninit_v0(&ret, MMSTR__OBJ_SIZE);
    mmstrstk_init_v0(&ret, MMSTR__OBJ_SIZE);
    ec = MemeStringStack_mid_v2(&large_str.native_handle(), 5, 10, &ret, MMSTR__OBJ_SIZE);
    REQUIRE(ec == 0);
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret) == 10);
    REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret) == mmstr_strg_small);

    // mid(5, 0) → count==0 returns empty
    mmstrstk_uninit_v0(&ret, MMSTR__OBJ_SIZE);
    mmstrstk_init_v0(&ret, MMSTR__OBJ_SIZE);
    ec = MemeStringStack_mid_v2(&large_str.native_handle(), 5, 0, &ret, MMSTR__OBJ_SIZE);
    REQUIRE(ec == 0);
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret) == 0);
    REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret) == mmstr_strg_small);

    // mid(srcSize, 1) → offset==size boundary returns empty
    mmstrstk_uninit_v0(&ret, MMSTR__OBJ_SIZE);
    mmstrstk_init_v0(&ret, MMSTR__OBJ_SIZE);
    ec = MemeStringStack_mid_v2(&large_str.native_handle(), large_str.size(), 1, &ret, MMSTR__OBJ_SIZE);
    REQUIRE(ec == 0);
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret) == 0);
    REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret) == mmstr_strg_small);
}

TEST_CASE("memepp::string mid - offset==size boundary", "[string]")
{
    // offset==size should return empty regardless of storage type
    const mmint_t limit = MemeStringOption_getStorageMediumLimit();
    REQUIRE(limit > MemeStringOption_getStorageSmallLimit()); // medium tier must exist

    // Length driven by the live medium limit, so the source is always medium.
    const char* const kBase = "Hello, World! ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz";
    const size_t kBaseLen = 67;
    const size_t targetLen = kBaseLen < (size_t)limit ? kBaseLen : (size_t)limit;
    memepp::string medium_str(kBase, targetLen);
    REQUIRE(medium_str.storage_type() == memepp::string_storage_t::medium);

    mmstrstk_t ret;
    mmstrstk_init_v0(&ret, MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() {
        mmstrstk_uninit_v0(&ret, MMSTR__OBJ_SIZE);
    });

    // mid(srcSize, 0) → empty
    mgec_t ec = MemeStringStack_mid_v2(&medium_str.native_handle(), medium_str.size(), 0, &ret, MMSTR__OBJ_SIZE);
    REQUIRE(ec == 0);
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret) == 0);
    REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret) == mmstr_strg_small);

    // mid(srcSize, 5) → empty (offset clamped by count logic)
    mmstrstk_uninit_v0(&ret, MMSTR__OBJ_SIZE);
    mmstrstk_init_v0(&ret, MMSTR__OBJ_SIZE);
    ec = MemeStringStack_mid_v2(&medium_str.native_handle(), medium_str.size(), 5, &ret, MMSTR__OBJ_SIZE);
    REQUIRE(ec == 0);
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret) == 0);
    REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret) == mmstr_strg_small);

    // Also test on small source
    memepp::string small_str = "Hello";  // 5 bytes
    REQUIRE(small_str.storage_type() == memepp::string_storage_t::small);

    mmstrstk_uninit_v0(&ret, MMSTR__OBJ_SIZE);
    mmstrstk_init_v0(&ret, MMSTR__OBJ_SIZE);
    ec = MemeStringStack_mid_v2(&small_str.native_handle(), small_str.size(), 3, &ret, MMSTR__OBJ_SIZE);
    REQUIRE(ec == 0);
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret) == 0);
    REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret) == mmstr_strg_small);
}

TEST_CASE("memepp::string mid - multibyte UTF-8", "[string]")
{
    // Multibyte UTF-8: mid operates on bytes, not runes.
    // Escapes keep the source ASCII-only so no BOM / code-page dependency.
    // U+4F60 'ni'  = E4 BD A0   (3 bytes)
    // U+597D 'hao' = E5 A5 BD   (3 bytes)
    // U+1F30D globe = F0 9F 8C 8D (4 bytes)
    const char* const kNi   = "\xE4\xBD\xA0";
    const char* const kNiHao = "\xE4\xBD\xA0\xE5\xA5\xBD";
    const char* const kGlobe = "\xF0\x9F\x8C\x8D";

    memepp::string utf8_str = "\xE4\xBD\xA0\xE5\xA5\xBD\xE4\xB8\x96\xE7\x95\x8C\xF0\x9F\x8C\x8D";
    REQUIRE(utf8_str.size() == 16);  // 4*3 + 4 = 16 bytes

    mmstrstk_t ret;
    mmstrstk_init_v0(&ret, MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() {
        mmstrstk_uninit_v0(&ret, MMSTR__OBJ_SIZE);
    });

    // mid(0, 3) → first CJK character (3 bytes)
    mgec_t ec = MemeStringStack_mid_v2(&utf8_str.native_handle(), 0, 3, &ret, MMSTR__OBJ_SIZE);
    REQUIRE(ec == 0);
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret) == 3);
    REQUIRE(MemeString_compareByUtf8bytes(
        (mmstr_cptr_t)&ret, (const uint8_t*)kNi, -1) == 0);

    // mid(0, 6) → two CJK characters (6 bytes)
    mmstrstk_uninit_v0(&ret, MMSTR__OBJ_SIZE);
    mmstrstk_init_v0(&ret, MMSTR__OBJ_SIZE);
    ec = MemeStringStack_mid_v2(&utf8_str.native_handle(), 0, 6, &ret, MMSTR__OBJ_SIZE);
    REQUIRE(ec == 0);
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret) == 6);
    REQUIRE(MemeString_compareByUtf8bytes(
        (mmstr_cptr_t)&ret, (const uint8_t*)kNiHao, -1) == 0);

    // mid(12, 4) → 4-byte emoji
    mmstrstk_uninit_v0(&ret, MMSTR__OBJ_SIZE);
    mmstrstk_init_v0(&ret, MMSTR__OBJ_SIZE);
    ec = MemeStringStack_mid_v2(&utf8_str.native_handle(), 12, 4, &ret, MMSTR__OBJ_SIZE);
    REQUIRE(ec == 0);
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret) == 4);
    REQUIRE(MemeString_compareByUtf8bytes(
        (mmstr_cptr_t)&ret, (const uint8_t*)kGlobe, -1) == 0);

    // mid(1, 3) → partial character: mid is byte-level, no rune awareness.
    // 3 bytes from offset 1 are not a valid UTF-8 sequence but are returned as-is.
    mmstrstk_uninit_v0(&ret, MMSTR__OBJ_SIZE);
    mmstrstk_init_v0(&ret, MMSTR__OBJ_SIZE);
    ec = MemeStringStack_mid_v2(&utf8_str.native_handle(), 1, 3, &ret, MMSTR__OBJ_SIZE);
    REQUIRE(ec == 0);
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret) == 3);
    REQUIRE(MemeString_compareByUtf8bytes(
        (mmstr_cptr_t)&ret, (const uint8_t*)"\xBD\xA0\xE5", -1) == 0);
}
