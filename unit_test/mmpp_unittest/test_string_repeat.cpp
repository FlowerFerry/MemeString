
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>
#include <megopp/util/scope_cleanup.h>

TEST_CASE("memepp::string repeat", "[string]")
{
    mgec_t ec;
    mmstrstk_t stk01_01;
    ec = MemeStringStack_getRepeat_v2(&stk01_01, MMSTR__OBJ_SIZE, -1, "123", -1);
    auto cleanup01_01 = mgpp::util::scope_cleanup__create([&] { mmstrstk_uninit_v0(&stk01_01, MMSTR__OBJ_SIZE); });

    REQUIRE(ec == MGEC__OK);
    REQUIRE(strcmp(MemeString_cStr((mmstr_cptr_t)&stk01_01), "") == 0);

    cleanup01_01.early_exec();

    ec = MemeStringStack_getRepeat_v2(&stk01_01, MMSTR__OBJ_SIZE, 0, "123", -1);
    
    REQUIRE(ec == MGEC__OK);
    REQUIRE(strcmp(MemeString_cStr((mmstr_cptr_t)&stk01_01), "") == 0);
    
    cleanup01_01.early_exec();

    ec = MemeStringStack_getRepeat_v2(&stk01_01, MMSTR__OBJ_SIZE, 1, "123", -1);

    REQUIRE(ec == MGEC__OK);
    REQUIRE(strcmp(MemeString_cStr((mmstr_cptr_t)&stk01_01), "123") == 0);

    cleanup01_01.early_exec();
    
    ec = MemeStringStack_getRepeat_v2(&stk01_01, MMSTR__OBJ_SIZE, 2, "123", -1);

    REQUIRE(ec == MGEC__OK);
    REQUIRE(strcmp(MemeString_cStr((mmstr_cptr_t)&stk01_01), "123123") == 0);

    cleanup01_01.early_exec();

    ec = MemeStringStack_getRepeat_v2(&stk01_01, MMSTR__OBJ_SIZE, 1, "123", 0);

    REQUIRE(ec == MGEC__OK);
    REQUIRE(strcmp(MemeString_cStr((mmstr_cptr_t)&stk01_01), "") == 0);

    cleanup01_01.early_exec();

    ec = MemeStringStack_getRepeat_v2(&stk01_01, MMSTR__OBJ_SIZE, 2, NULL, 3);

    REQUIRE(ec == MGEC__OK);
    REQUIRE(strcmp(MemeString_cStr((mmstr_cptr_t)&stk01_01), "") == 0);

    cleanup01_01.early_exec();
}

TEST_CASE("memepp::string static repeat", "[string]")
{
    // basic repeat
    auto repeated = memepp::string::repeat("Ab", 3);
    REQUIRE(repeated == "AbAbAb");
    REQUIRE(repeated.size() == 6);
    REQUIRE(repeated.storage_type() == memepp::string_storage_t::small); // assuming small size

    // repeat with count 0 or 1
    REQUIRE(memepp::string::repeat("Cd", 0) == "");
    REQUIRE(memepp::string::repeat("Ef", 1) == "Ef");

    // repeat empty string
    REQUIRE(memepp::string::repeat("", 5) == "");

    // long repeat to test large storage
    auto long_str = memepp::string::repeat("1234567890", 100); // 1000 chars
    REQUIRE(long_str.size() == 1000);
    REQUIRE(long_str.storage_type() == memepp::string_storage_t::large);
    REQUIRE(long_str.starts_with("12345678901234567890"));
}
