
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>
#include <memepp/convert/std/string.hpp>
#include <string>
#include <megopp/util/scope_cleanup.h>

TEST_CASE("MemeStringStack concat_v2 (C API)", "[string]")
{
    mgec_t ec;
    mmstrstk_t str01_01;
    mmstrstk_t str01_02;
    mmstrstk_t str01_03;

    // Note: early_exec() runs the cleanup once and marks the guard as canceled - every
    // later call on the same guard is a no-op. A fresh scope_cleanup must therefore be
    // created for each round, otherwise the strings concat_v2 produces in rounds 2..4 are
    // never uninit'd (harmless while they stay small/SSO, a real leak once they grow).

    // --- Round 1: "Hello, " + "World!" ---
    MemeStringStack_initByU8bytes(&str01_01, MMSTR__OBJ_SIZE, (const uint8_t*)"Hello, ", -1);
    MemeStringStack_initByU8bytes(&str01_02, MMSTR__OBJ_SIZE, (const uint8_t*)"World!", -1);
    auto str01_01_r1_cleanup =
        mgpp::util::scope_cleanup__create([&str01_01]() { MemeStringStack_unInit(&str01_01, MMSTR__OBJ_SIZE); });
    auto str01_02_r1_cleanup =
        mgpp::util::scope_cleanup__create([&str01_02]() { MemeStringStack_unInit(&str01_02, MMSTR__OBJ_SIZE); });

    ec = MemeStringStack_concat_v2(&str01_01, &str01_02, &str01_03, MMSTR__OBJ_SIZE);
    REQUIRE(ec == MGEC__OK);
    auto str01_03_r1_cleanup =
        mgpp::util::scope_cleanup__create([&str01_03]() { MemeStringStack_unInit(&str01_03, MMSTR__OBJ_SIZE); });

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_03) == 13);
    REQUIRE(strncmp((const char*)MemeString_byteData((mmstr_cptr_t)&str01_03), "Hello, World!", 13) == 0);

    str01_01_r1_cleanup.early_exec();
    str01_02_r1_cleanup.early_exec();
    str01_03_r1_cleanup.early_exec();

    // --- Round 2: "" + "" ---
    MemeStringStack_init(&str01_01, MMSTR__OBJ_SIZE);
    MemeStringStack_init(&str01_02, MMSTR__OBJ_SIZE);
    auto str01_01_r2_cleanup =
        mgpp::util::scope_cleanup__create([&str01_01]() { MemeStringStack_unInit(&str01_01, MMSTR__OBJ_SIZE); });
    auto str01_02_r2_cleanup =
        mgpp::util::scope_cleanup__create([&str01_02]() { MemeStringStack_unInit(&str01_02, MMSTR__OBJ_SIZE); });

    ec = MemeStringStack_concat_v2(&str01_01, &str01_02, &str01_03, MMSTR__OBJ_SIZE);
    REQUIRE(ec == MGEC__OK);
    auto str01_03_r2_cleanup =
        mgpp::util::scope_cleanup__create([&str01_03]() { MemeStringStack_unInit(&str01_03, MMSTR__OBJ_SIZE); });

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_03) == 0);
    REQUIRE(strncmp((const char*)MemeString_byteData((mmstr_cptr_t)&str01_03), "", 0) == 0);

    str01_01_r2_cleanup.early_exec();
    str01_02_r2_cleanup.early_exec();
    str01_03_r2_cleanup.early_exec();

    // --- Round 3: "Hello, " + "" ---
    MemeStringStack_initByU8bytes(&str01_01, MMSTR__OBJ_SIZE, (const uint8_t*)"Hello, ", -1);
    MemeStringStack_init(&str01_02, MMSTR__OBJ_SIZE);
    auto str01_01_r3_cleanup =
        mgpp::util::scope_cleanup__create([&str01_01]() { MemeStringStack_unInit(&str01_01, MMSTR__OBJ_SIZE); });
    auto str01_02_r3_cleanup =
        mgpp::util::scope_cleanup__create([&str01_02]() { MemeStringStack_unInit(&str01_02, MMSTR__OBJ_SIZE); });

    ec = MemeStringStack_concat_v2(&str01_01, &str01_02, &str01_03, MMSTR__OBJ_SIZE);
    REQUIRE(ec == MGEC__OK);
    auto str01_03_r3_cleanup =
        mgpp::util::scope_cleanup__create([&str01_03]() { MemeStringStack_unInit(&str01_03, MMSTR__OBJ_SIZE); });

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_03) == 7);
    REQUIRE(strncmp((const char*)MemeString_byteData((mmstr_cptr_t)&str01_03), "Hello, ", 7) == 0);

    str01_01_r3_cleanup.early_exec();
    str01_02_r3_cleanup.early_exec();
    str01_03_r3_cleanup.early_exec();

    // --- Round 4: "" + "World!" ---
    MemeStringStack_init(&str01_01, MMSTR__OBJ_SIZE);
    MemeStringStack_initByU8bytes(&str01_02, MMSTR__OBJ_SIZE, (const uint8_t*)"World!", -1);
    auto str01_01_r4_cleanup =
        mgpp::util::scope_cleanup__create([&str01_01]() { MemeStringStack_unInit(&str01_01, MMSTR__OBJ_SIZE); });
    auto str01_02_r4_cleanup =
        mgpp::util::scope_cleanup__create([&str01_02]() { MemeStringStack_unInit(&str01_02, MMSTR__OBJ_SIZE); });

    ec = MemeStringStack_concat_v2(&str01_01, &str01_02, &str01_03, MMSTR__OBJ_SIZE);
    REQUIRE(ec == MGEC__OK);
    auto str01_03_r4_cleanup =
        mgpp::util::scope_cleanup__create([&str01_03]() { MemeStringStack_unInit(&str01_03, MMSTR__OBJ_SIZE); });

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_03) == 6);
    REQUIRE(strncmp((const char*)MemeString_byteData((mmstr_cptr_t)&str01_03), "World!", 6) == 0);

    str01_01_r4_cleanup.early_exec();
    str01_02_r4_cleanup.early_exec();
    str01_03_r4_cleanup.early_exec();
}

// ---------------------------------------------------------------------------
// memepp::string::concat  (C++ API)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string concat(string) - C++ API", "[string]")
{
    memepp::string s1 = "Hello, ";
    memepp::string s2 = "World!";

    auto r = s1.concat(s2);
    REQUIRE(r == "Hello, World!");
    REQUIRE(r.size() == 13);
}

TEST_CASE("memepp::string concat(string_view) - C++ API", "[string]")
{
    memepp::string s1 = "Hello, ";
    memepp::string_view sv = "World!";

    auto r = s1.concat(sv);
    REQUIRE(r == "Hello, World!");
    REQUIRE(r.size() == 13);
}

TEST_CASE("memepp::string concat - empty lhs", "[string]")
{
    memepp::string empty;
    memepp::string s = "World!";

    REQUIRE(empty.concat(s)                        == "World!");
    REQUIRE(empty.concat(memepp::string_view("X")) == "X");
}

TEST_CASE("memepp::string concat - empty rhs", "[string]")
{
    memepp::string s = "Hello, ";
    memepp::string empty;
    memepp::string_view empty_sv;

    REQUIRE(s.concat(empty)    == "Hello, ");
    REQUIRE(s.concat(empty_sv) == "Hello, ");
}

TEST_CASE("memepp::string concat - both empty", "[string]")
{
    memepp::string empty1;
    memepp::string empty2;

    REQUIRE(empty1.concat(empty2)               == "");
    REQUIRE(empty1.concat(memepp::string_view{}) == "");
}

TEST_CASE("memepp::string concat - original is not modified", "[string]")
{
    memepp::string s1 = "Hello";
    memepp::string s2 = " World";

    auto r = s1.concat(s2);
    REQUIRE(r  == "Hello World");
    REQUIRE(s1 == "Hello");  // s1 must remain unchanged
}

TEST_CASE("memepp::string concat - user storage", "[string]")
{
    const mmint_t mediumLimit = MemeStringOption_getStorageMediumLimit();
    std::string data(mediumLimit + 100, 'U');
    memepp::string user_str = memepp::from(std::move(data));
    REQUIRE(user_str.storage_type() == memepp::string_storage_t::user);

    memepp::string result = user_str.concat(memepp::string_view("XYZ"));
    REQUIRE(result.size() == user_str.size() + 3);
    REQUIRE(result.ends_with("XYZ"));
}

TEST_CASE("memepp::string concat - view storage", "[string]")
{
    const char* src = "Hello";
    mmstrstk_t view_str;
    int rc = MemeStringViewUnsafeStack_init(&view_str, sizeof(view_str),
        reinterpret_cast<const uint8_t*>(src), -1);
    REQUIRE(rc == 0);
    REQUIRE(MemeString_storageType((mmstr_cptr_t)&view_str) == MemeString_UnsafeStorageType_view);

    memepp::string_view sv(
        reinterpret_cast<const char*>(MemeString_byteData((mmstr_cptr_t)&view_str)),
        MemeString_byteSize((mmstr_cptr_t)&view_str));
    memepp::string result = sv.to_string().concat(memepp::string_view(" World"));
    REQUIRE(result == "Hello World");

    MemeStringStack_unInit(&view_str, sizeof(view_str));
}
