
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>
#include <megopp/util/scope_cleanup.h>

TEST_CASE("MemeStringStack concat_v2 (C API)", "[string]")
{
    mmstrstk_t str01_01;
    mmstrstk_t str01_02;
    MemeStringStack_initByU8bytes(&str01_01, MMSTR__OBJ_SIZE, (const uint8_t*)"Hello, ", -1);
    MemeStringStack_initByU8bytes(&str01_02, MMSTR__OBJ_SIZE, (const uint8_t*)"World!", -1);
    auto str01_01_cleanup = 
        mgpp::util::scope_cleanup__create([&str01_01]() { MemeStringStack_unInit(&str01_01, MMSTR__OBJ_SIZE); });
    auto str01_02_cleanup =
        mgpp::util::scope_cleanup__create([&str01_02]() { MemeStringStack_unInit(&str01_02, MMSTR__OBJ_SIZE); });

    mmstrstk_t str01_03;
    MemeStringStack_concat_v2(&str01_01, &str01_02, &str01_03, MMSTR__OBJ_SIZE);
    auto str01_03_cleanup =
        mgpp::util::scope_cleanup__create([&str01_03]() { MemeStringStack_unInit(&str01_03, MMSTR__OBJ_SIZE); });
    
    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_03) == 13);
    REQUIRE(strncmp((const char*)MemeString_byteData((mmstr_cptr_t)&str01_03), "Hello, World!", 13) == 0);

    str01_01_cleanup.early_exec();
    str01_02_cleanup.early_exec();
    str01_03_cleanup.early_exec();

    MemeStringStack_init(&str01_01, MMSTR__OBJ_SIZE);
    MemeStringStack_init(&str01_02, MMSTR__OBJ_SIZE);

    MemeStringStack_concat_v2(&str01_01, &str01_02, &str01_03, MMSTR__OBJ_SIZE);

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_03) == 0);
    REQUIRE(strncmp((const char*)MemeString_byteData((mmstr_cptr_t)&str01_03), "", 0) == 0);

    str01_01_cleanup.early_exec();
    str01_02_cleanup.early_exec();
    str01_03_cleanup.early_exec();

    MemeStringStack_initByU8bytes(&str01_01, MMSTR__OBJ_SIZE, (const uint8_t*)"Hello, ", -1);
    MemeStringStack_init(&str01_02, MMSTR__OBJ_SIZE);

    MemeStringStack_concat_v2(&str01_01, &str01_02, &str01_03, MMSTR__OBJ_SIZE);

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_03) == 7);
    REQUIRE(strncmp((const char*)MemeString_byteData((mmstr_cptr_t)&str01_03), "Hello, ", 7) == 0);

    str01_01_cleanup.early_exec();
    str01_02_cleanup.early_exec();
    str01_03_cleanup.early_exec();

    MemeStringStack_init(&str01_01, MMSTR__OBJ_SIZE);
    MemeStringStack_initByU8bytes(&str01_02, MMSTR__OBJ_SIZE, (const uint8_t*)"World!", -1);

    MemeStringStack_concat_v2(&str01_01, &str01_02, &str01_03, MMSTR__OBJ_SIZE);

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_03) == 6);
    REQUIRE(strncmp((const char*)MemeString_byteData((mmstr_cptr_t)&str01_03), "World!", 6) == 0);

    str01_01_cleanup.early_exec();
    str01_02_cleanup.early_exec();
    str01_03_cleanup.early_exec();
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