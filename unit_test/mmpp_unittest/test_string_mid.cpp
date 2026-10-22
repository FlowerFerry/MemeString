
#include <catch2/catch.hpp>

#include <megopp/util/scope_cleanup.h>
#include <memepp/string_view.hpp>
#include <memepp/string.hpp>
#include <memepp/convert/std/string.hpp>

#include <string>

TEST_CASE("memepp::string mid", "[string]")
{
    mgec_t ec = 0;
    memepp::string str01_01 = "Hello, World! ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz";

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
