
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>
#include <megopp/util/scope_cleanup.h>

TEST_CASE("memepp::string concat", "[string]")
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