
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
