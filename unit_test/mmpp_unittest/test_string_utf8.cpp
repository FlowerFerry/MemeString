
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>
#include <megopp/util/scope_cleanup.h>

TEST_CASE("memepp::string to vaild utf8", "[string]") 
{
    mmstrstk_t str01_01;
    MemeStringStack_init(&str01_01, MMSTR__OBJ_SIZE);

    auto str01_01_cleanup = 
        mgpp::util::scope_cleanup__create([&str01_01]() { MemeStringStack_unInit(&str01_01, MMSTR__OBJ_SIZE); });
    
    mmstrstk_t str01_02;
    MemeStringStack_toValidUtf8_v2(&str01_01, &str01_02, MMSTR__OBJ_SIZE);

    auto str01_02_cleanup = 
        mgpp::util::scope_cleanup__create([&str01_02]() { MemeStringStack_unInit(&str01_02, MMSTR__OBJ_SIZE); });

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_02) == 0);
    REQUIRE(strncmp(MemeString_cStr((mmstr_cptr_t)&str01_02), "", 1) == 0);

    str01_01_cleanup.early_exec();
    str01_02_cleanup.early_exec();

    MemeStringStack_initByU8bytes (&str01_01, MMSTR__OBJ_SIZE, (const uint8_t*)"Hello, World!", -1);
    MemeStringStack_toValidUtf8_v2(&str01_01, &str01_02, MMSTR__OBJ_SIZE);

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_02) == 13);
    REQUIRE(strncmp(MemeString_cStr((mmstr_cptr_t)&str01_02), "Hello, World!", 13) == 0);

    str01_01_cleanup.early_exec();
    str01_02_cleanup.early_exec();

    MemeStringStack_initByU8bytes(&str01_01, MMSTR__OBJ_SIZE, (const uint8_t*)"Hello, \xF0\x9F\x98\x8A", -1);
    MemeStringStack_toValidUtf8_v2(&str01_01, &str01_02, MMSTR__OBJ_SIZE);

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_02) == 11);
    REQUIRE(strncmp(MemeString_cStr((mmstr_cptr_t)&str01_02), "Hello, \xF0\x9F\x98\x8A", 11) == 0);

    str01_01_cleanup.early_exec();
    str01_02_cleanup.early_exec();

    MemeStringStack_initByU8bytes(&str01_01, MMSTR__OBJ_SIZE, (const uint8_t*)"Hello, \xF0\x9F\x98", -1);
    MemeStringStack_toValidUtf8_v2(&str01_01, &str01_02, MMSTR__OBJ_SIZE);

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_02) == 7);
    REQUIRE(strncmp(MemeString_cStr((mmstr_cptr_t)&str01_02), "Hello, ", 7) == 0);

    str01_01_cleanup.early_exec();
    str01_02_cleanup.early_exec();

    MemeStringStack_initByU8bytes(&str01_01, MMSTR__OBJ_SIZE, (const uint8_t*)"Hello, \x98\x8A", -1);
    MemeStringStack_toValidUtf8_v2(&str01_01, &str01_02, MMSTR__OBJ_SIZE);

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_02) == 7);
    REQUIRE(strncmp(MemeString_cStr((mmstr_cptr_t)&str01_02), "Hello, ", 7) == 0);

    str01_01_cleanup.early_exec();
    str01_02_cleanup.early_exec();

    MemeStringStack_initByU8bytes(&str01_01, MMSTR__OBJ_SIZE, (const uint8_t*)"\xF0\x9F\x98\x8A, World!", -1);
    MemeStringStack_toValidUtf8_v2(&str01_01, &str01_02, MMSTR__OBJ_SIZE);

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_02) == 12);
    REQUIRE(strncmp(MemeString_cStr((mmstr_cptr_t)&str01_02), "\xF0\x9F\x98\x8A, World!", 12) == 0);

    str01_01_cleanup.early_exec();
    str01_02_cleanup.early_exec();

    MemeStringStack_initByU8bytes(&str01_01, MMSTR__OBJ_SIZE, (const uint8_t*)"\xF0\x9F\x98, World!", -1);
    MemeStringStack_toValidUtf8_v2(&str01_01, &str01_02, MMSTR__OBJ_SIZE);

    REQUIRE(MemeString_byteSize((mmstr_cptr_t)&str01_02) == 0);
    REQUIRE(strncmp(MemeString_cStr((mmstr_cptr_t)&str01_02), "", 1) == 0);
}
