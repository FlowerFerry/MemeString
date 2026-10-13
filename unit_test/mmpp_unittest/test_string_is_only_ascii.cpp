
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

TEST_CASE("memepp::string is_only_ascii boundary bytes", "[string]")
{
    memepp::string str;
    
    // 0x7F (DEL, 127) should be ASCII
    char c7f = '\x7F';
    str = memepp::string(&c7f, 1);
    REQUIRE(str.is_only_ascii() == true);
    
    // 0x80 (128) should NOT be ASCII
    char c80 = '\x80';
    str = memepp::string(&c80, 1);
    REQUIRE(str.is_only_ascii() == false);
    
    // 0xFF (255, invalid UTF-8) should NOT be ASCII
    char cff = '\xFF';
    str = memepp::string(&cff, 1);
    REQUIRE(str.is_only_ascii() == false);
}

TEST_CASE("memepp::string is_only_ascii embedded NUL", "[string]")
{
    char data[] = { 'h', 'e', 'l', 'l', 'o', '\0', 'w', 'o', 'r', 'l', 'd' };
    memepp::string str(data, sizeof(data));
    REQUIRE(str.is_only_ascii() == true);
}

TEST_CASE("memepp::string is_only_ascii non-ASCII at end", "[string]")
{
    char data[] = { 'h', 'e', 'l', 'l', 'o', '\x80' };
    memepp::string str(data, sizeof(data));
    REQUIRE(str.is_only_ascii() == false);
}

TEST_CASE("memepp::string is_only_ascii large storage", "[string]")
{
    // Construct string exceeding medium limit
    std::string large_data(1000, 'a');
    memepp::string str(large_data.data(), large_data.size());
    REQUIRE(str.storage_type() == memepp::string_storage_t::large);
    REQUIRE(str.is_only_ascii() == true);
    
    // Large type with non-ASCII
    std::string large_non_ascii(1000, 'a');
    large_non_ascii[500] = '\x80';
    memepp::string str2(large_non_ascii.data(), large_non_ascii.size());
    REQUIRE(str2.is_only_ascii() == false);
}
