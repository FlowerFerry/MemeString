
#include <catch2/catch.hpp>
#include <memepp/string.hpp>
#include <megopp/util/scope_cleanup.h>

TEST_CASE("memepp::string replace", "[string]")
{
    memepp::string str01_01 { "Hello, World!" };
    memepp::string str01_02 { "World" };

    auto str01_03 = str01_01.replace("World", "Universe");
    REQUIRE(str01_03 == "Hello, Universe!");

    auto str01_04 = str01_01.replace("l", "L");
    REQUIRE(str01_04 == "HeLLo, WorLd!");

    auto str01_05 = str01_04.replace("o", "O");
    REQUIRE(str01_05 == "HeLLO, WOrLd!");
}
