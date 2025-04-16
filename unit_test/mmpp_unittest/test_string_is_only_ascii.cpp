
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

TEST_CASE("memepp::string is_only_ascii", "[string]")
{
    memepp::string str;
    REQUIRE(str.is_only_ascii() == true);
    str = u8"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+[]{}|;:',.<>?";
    REQUIRE(str.is_only_ascii() == true);
    str = u8"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+[]{}|;:',.<>?あいうえお";
    REQUIRE(str.is_only_ascii() == false);
    str = u8"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+[]{}|;:',.<>?あいうえお1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+[]{}|;:',.<>?";
    REQUIRE(str.is_only_ascii() == false);
}
