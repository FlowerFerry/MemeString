
#include <catch2/catch.hpp>

#include <megopp/util/scope_cleanup.h>
#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

TEST_CASE("memepp::string - 35", "string mid")
{
    mgec_t ec = 0;
    memepp::string str01_01 = "Hello, World! ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz";
    
    mmstrstk_t ret01_01;
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() {
        mmstrstk_uninit_v0(&ret01_01, MMSTR__OBJ_SIZE);
    });

    ec = MemeStringStack_mid_v2(&str01_01.native_handle(), 0, 0, &ret01_01, MMSTR__OBJ_SIZE);
    REQUIRE(ec == 0);
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret01_01) == 0);
    REQUIRE(MemeString_compareByUtf8bytes(
        (mmstr_cptr_t)&ret01_01, (const uint8_t*)"", -1) == 0);
    REQUIRE(MemeString_storageType((mmstr_cptr_t)&ret01_01) == mmstr_strg_small);

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