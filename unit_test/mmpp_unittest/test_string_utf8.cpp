
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>
#include <memepp/convert/std/string.hpp>
#include <string>
#include <megopp/util/scope_cleanup.h>

TEST_CASE("MemeStringStack to_valid_utf8 (C API)", "[string]") 
{
    mmstrstk_t str01_01;
    mmstrstk_t str01_02;

    // Note: early_exec() runs the cleanup once and marks the guard as canceled - every
    // later call on the same guard is a no-op. A fresh scope_cleanup must therefore be
    // created for each round, otherwise the string toValidUtf8_v2 produces in rounds 2..7
    // is never uninit'd (harmless while it stays small/SSO, a real leak once it grows).

    // --- Round 1: empty input ---
    MemeStringStack_init(&str01_01, MMSTR__OBJ_SIZE);
    auto str01_01_r1_cleanup =
        mgpp::util::scope_cleanup__create([&str01_01]() { MemeStringStack_unInit(&str01_01, MMSTR__OBJ_SIZE); });

    MemeStringStack_toValidUtf8_v2(&str01_01, &str01_02, MMSTR__OBJ_SIZE);
    auto str01_02_r1_cleanup =
        mgpp::util::scope_cleanup__create([&str01_02]() { MemeStringStack_unInit(&str01_02, MMSTR__OBJ_SIZE); });

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_02) == 0);
    REQUIRE(strncmp(MemeString_cStr((mmstr_cptr_t)&str01_02), "", 1) == 0);

    str01_01_r1_cleanup.early_exec();
    str01_02_r1_cleanup.early_exec();

    // --- Round 2: plain ASCII ---
    MemeStringStack_initByU8bytes(&str01_01, MMSTR__OBJ_SIZE, (const uint8_t*)"Hello, World!", -1);
    auto str01_01_r2_cleanup =
        mgpp::util::scope_cleanup__create([&str01_01]() { MemeStringStack_unInit(&str01_01, MMSTR__OBJ_SIZE); });

    MemeStringStack_toValidUtf8_v2(&str01_01, &str01_02, MMSTR__OBJ_SIZE);
    auto str01_02_r2_cleanup =
        mgpp::util::scope_cleanup__create([&str01_02]() { MemeStringStack_unInit(&str01_02, MMSTR__OBJ_SIZE); });

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_02) == 13);
    REQUIRE(strncmp(MemeString_cStr((mmstr_cptr_t)&str01_02), "Hello, World!", 13) == 0);

    str01_01_r2_cleanup.early_exec();
    str01_02_r2_cleanup.early_exec();

    // --- Round 3: complete 4-byte sequence ---
    MemeStringStack_initByU8bytes(&str01_01, MMSTR__OBJ_SIZE, (const uint8_t*)"Hello, \xF0\x9F\x98\x8A", -1);
    auto str01_01_r3_cleanup =
        mgpp::util::scope_cleanup__create([&str01_01]() { MemeStringStack_unInit(&str01_01, MMSTR__OBJ_SIZE); });

    MemeStringStack_toValidUtf8_v2(&str01_01, &str01_02, MMSTR__OBJ_SIZE);
    auto str01_02_r3_cleanup =
        mgpp::util::scope_cleanup__create([&str01_02]() { MemeStringStack_unInit(&str01_02, MMSTR__OBJ_SIZE); });

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_02) == 11);
    REQUIRE(strncmp(MemeString_cStr((mmstr_cptr_t)&str01_02), "Hello, \xF0\x9F\x98\x8A", 11) == 0);

    str01_01_r3_cleanup.early_exec();
    str01_02_r3_cleanup.early_exec();

    // --- Round 4: truncated tail sequence ---
    MemeStringStack_initByU8bytes(&str01_01, MMSTR__OBJ_SIZE, (const uint8_t*)"Hello, \xF0\x9F\x98", -1);
    auto str01_01_r4_cleanup =
        mgpp::util::scope_cleanup__create([&str01_01]() { MemeStringStack_unInit(&str01_01, MMSTR__OBJ_SIZE); });

    MemeStringStack_toValidUtf8_v2(&str01_01, &str01_02, MMSTR__OBJ_SIZE);
    auto str01_02_r4_cleanup =
        mgpp::util::scope_cleanup__create([&str01_02]() { MemeStringStack_unInit(&str01_02, MMSTR__OBJ_SIZE); });

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_02) == 7);
    REQUIRE(strncmp(MemeString_cStr((mmstr_cptr_t)&str01_02), "Hello, ", 7) == 0);

    str01_01_r4_cleanup.early_exec();
    str01_02_r4_cleanup.early_exec();

    // --- Round 5: orphan continuation bytes ---
    MemeStringStack_initByU8bytes(&str01_01, MMSTR__OBJ_SIZE, (const uint8_t*)"Hello, \x98\x8A", -1);
    auto str01_01_r5_cleanup =
        mgpp::util::scope_cleanup__create([&str01_01]() { MemeStringStack_unInit(&str01_01, MMSTR__OBJ_SIZE); });

    MemeStringStack_toValidUtf8_v2(&str01_01, &str01_02, MMSTR__OBJ_SIZE);
    auto str01_02_r5_cleanup =
        mgpp::util::scope_cleanup__create([&str01_02]() { MemeStringStack_unInit(&str01_02, MMSTR__OBJ_SIZE); });

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_02) == 7);
    REQUIRE(strncmp(MemeString_cStr((mmstr_cptr_t)&str01_02), "Hello, ", 7) == 0);

    str01_01_r5_cleanup.early_exec();
    str01_02_r5_cleanup.early_exec();

    // --- Round 6: sequence at the beginning ---
    MemeStringStack_initByU8bytes(&str01_01, MMSTR__OBJ_SIZE, (const uint8_t*)"\xF0\x9F\x98\x8A, World!", -1);
    auto str01_01_r6_cleanup =
        mgpp::util::scope_cleanup__create([&str01_01]() { MemeStringStack_unInit(&str01_01, MMSTR__OBJ_SIZE); });

    MemeStringStack_toValidUtf8_v2(&str01_01, &str01_02, MMSTR__OBJ_SIZE);
    auto str01_02_r6_cleanup =
        mgpp::util::scope_cleanup__create([&str01_02]() { MemeStringStack_unInit(&str01_02, MMSTR__OBJ_SIZE); });

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_02) == 12);
    REQUIRE(strncmp(MemeString_cStr((mmstr_cptr_t)&str01_02), "\xF0\x9F\x98\x8A, World!", 12) == 0);

    str01_01_r6_cleanup.early_exec();
    str01_02_r6_cleanup.early_exec();

    // --- Round 7: truncated sequence at the beginning ---
    MemeStringStack_initByU8bytes(&str01_01, MMSTR__OBJ_SIZE, (const uint8_t*)"\xF0\x9F\x98, World!", -1);
    auto str01_01_r7_cleanup =
        mgpp::util::scope_cleanup__create([&str01_01]() { MemeStringStack_unInit(&str01_01, MMSTR__OBJ_SIZE); });

    MemeStringStack_toValidUtf8_v2(&str01_01, &str01_02, MMSTR__OBJ_SIZE);
    auto str01_02_r7_cleanup =
        mgpp::util::scope_cleanup__create([&str01_02]() { MemeStringStack_unInit(&str01_02, MMSTR__OBJ_SIZE); });

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_02) == 0);
    REQUIRE(strncmp(MemeString_cStr((mmstr_cptr_t)&str01_02), "", 1) == 0);

    str01_01_r7_cleanup.early_exec();
    str01_02_r7_cleanup.early_exec();
}

TEST_CASE("memepp::string to_valid_utf8 - user storage", "[string]")
{
    // user storage: produced by from(std::string&&) above the medium limit.
    const mmint_t mediumLimit = MemeStringOption_getStorageMediumLimit();
    std::string data(mediumLimit + 100, 'A');
    data[50] = '\xF0';
    data[51] = '\x9F';
    data[52] = '\x98';
    data[53] = '\x8A';
    memepp::string user_str = memepp::from(std::move(data));
    REQUIRE(user_str.storage_type() == memepp::string_storage_t::user);

    mmstrstk_t ret;
    mmstrstk_init_v0(&ret, MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() {
        mmstrstk_uninit_v0(&ret, MMSTR__OBJ_SIZE);
    });

    MemeStringStack_toValidUtf8_v2(&user_str.native_handle(), &ret, MMSTR__OBJ_SIZE);
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret) == user_str.size());
}

TEST_CASE("memepp::string to_valid_utf8 - view storage", "[string]")
{
    // C++ wrapper cannot produce view-type string (constructor deep-copies views).
    // Use C API directly to create a view.
    const char* src = "Hello, \xF0\x9F\x98\x8A, World!";
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

    MemeStringStack_toValidUtf8_v2(&view_str, &ret, MMSTR__OBJ_SIZE);
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&ret) == 19);

    MemeStringStack_unInit(&view_str, sizeof(view_str));
}
