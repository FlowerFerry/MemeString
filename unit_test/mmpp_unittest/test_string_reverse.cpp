
#include <catch2/catch.hpp>

#include <memepp/string.hpp>

#include <string>
#include <vector>

TEST_CASE("memepp::string reverse - ASCII", "[string][reverse]")
{
    memepp::string s = "abc";
    auto r = s.reverse();
    REQUIRE(r == "cba");
    REQUIRE(r.size() == 3);
}

TEST_CASE("memepp::string reverse - Chinese", "[string][reverse]")
{
    // 你好 -> 好你
    memepp::string s("\xE4\xBD\xA0\xE5\xA5\xBD", 6);
    auto r = s.reverse();
    REQUIRE(r.size() == 6);
    REQUIRE(r == memepp::string("\xE5\xA5\xBD\xE4\xBD\xA0", 6));
}

TEST_CASE("memepp::string reverse - mixed ASCII + CJK", "[string][reverse]")
{
    // "a你b" -> "b你a"
    memepp::string s("a\xE4\xBD\xA0" "b", 5);
    auto r = s.reverse();
    REQUIRE(r.size() == 5);
    REQUIRE(r == memepp::string("b\xE4\xBD\xA0" "a", 5));
}

TEST_CASE("memepp::string reverse - emoji (4-byte rune)", "[string][reverse]")
{
    // 😀x -> x😀
    memepp::string s("\xF0\x9F\x98\x80x", 5);
    auto r = s.reverse();
    REQUIRE(r.size() == 5);
    REQUIRE(r == memepp::string("x\xF0\x9F\x98\x80", 5));
}

TEST_CASE("memepp::string reverse - empty", "[string][reverse]")
{
    memepp::string s = "";
    auto r = s.reverse();
    REQUIRE(r.empty());
    REQUIRE(r.size() == 0);
}

TEST_CASE("memepp::string reverse - single rune", "[string][reverse]")
{
    // 你
    memepp::string s("\xE4\xBD\xA0", 3);
    auto r = s.reverse();
    REQUIRE(r == s);
    REQUIRE(r.size() == 3);
}

TEST_CASE("memepp::string reverse - double reverse restores original", "[string][reverse]")
{
    const char original_data[] =
        "Hello\xE4\xBD\xA0\xE5\xA5\xBD\xF0\x9F\x98\x80!";
    memepp::string original(original_data, sizeof(original_data) - 1);
    auto r1 = original.reverse();
    auto r2 = r1.reverse();

    const char expected_data[] =
        "!\xF0\x9F\x98\x80\xE5\xA5\xBD\xE4\xBD\xA0olleH";
    memepp::string expected(expected_data, sizeof(expected_data) - 1);
    REQUIRE(r1 == expected);
    REQUIRE(r2 == original);
    REQUIRE(r2.size() == original.size());
}

TEST_CASE("memepp::string reverse - medium storage (long string)", "[string][reverse]")
{
    // 40 x 3-byte rune = 120 bytes -> medium storage
    std::vector<std::string> runes;
    std::string in;
    for (int i = 0; i < 40; ++i) {
        std::string rune;
        rune += (char)(0xE4 + (i % 2));
        rune += (char)(0xB8 + (i % 4));
        rune += (char)(0x80 + (i % 8));
        runes.push_back(rune);
        in += rune;
    }
    memepp::string s(in.data(), (int)in.size());
    REQUIRE(s.size() == 120);

    auto r = s.reverse();
    REQUIRE(r.size() == 120);

    std::string expected;
    for (auto it = runes.rbegin(); it != runes.rend(); ++it)
        expected += *it;
    memepp::string expected_string(expected.data(), (int)expected.size());
    REQUIRE(r == expected_string);

    // double reverse restores
    auto r2 = r.reverse();
    REQUIRE(r2 == s);
}
