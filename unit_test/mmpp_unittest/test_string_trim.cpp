
#include <catch2/catch.hpp>

#include <meme/string.h>

#include <memepp/string.hpp>
#include <megopp/util/scope_cleanup.h>

TEST_CASE("memepp::string trim by cuts", "[string]")
{
    mmstrstk_t stk01_01;
    mgec_t ec = 0;
    MemeStringStack_initByU8bytes(
        &stk01_01, MMSTR__OBJ_SIZE, (const uint8_t*)"~~~Hello, World!!!", -1);

    memepp::string str01_01;
    ec = MemeStringStack_trimByCuts_v2(
        &stk01_01, "~!", -1, const_cast<mmstrstk_t*>(&str01_01.native_handle()), -1);
    REQUIRE(ec == 0);
    REQUIRE(str01_01 == "Hello, World");
}
