#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

// ---------------------------------------------------------------------------
// string_view::repeat  (returns memepp::string)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view repeat - count zero returns empty", "[string_view]")
{
    memepp::string_view s = "abc";

    auto r = s.repeat(0);
    REQUIRE(r == "");
    REQUIRE(r.empty());
}

TEST_CASE("memepp::string_view repeat - count one returns copy", "[string_view]")
{
    memepp::string_view s = "Hello";

    auto r = s.repeat(1);
    REQUIRE(r == "Hello");
    REQUIRE(r.size() == 5);
}

TEST_CASE("memepp::string_view repeat - count multiple", "[string_view]")
{
    memepp::string_view s = "ab";

    REQUIRE(s.repeat(2) == "abab");
    REQUIRE(s.repeat(3) == "ababab");
    REQUIRE(s.repeat(5) == "ababababab");
}

TEST_CASE("memepp::string_view repeat - empty view", "[string_view]")
{
    memepp::string_view empty;

    REQUIRE(empty.repeat(0) == "");
    REQUIRE(empty.repeat(1) == "");
    REQUIRE(empty.repeat(5) == "");
}

TEST_CASE("memepp::string_view repeat - large count produces large storage", "[string_view]")
{
    memepp::string_view s = "1234567890";  // 10 chars

    auto r = s.repeat(100);  // 1000 chars
    REQUIRE(r.size() == 1000);
    REQUIRE(r.storage_type() == memepp::string_storage_t::large);
    REQUIRE(r.starts_with("12345678901234567890"));
    REQUIRE(r.ends_with("1234567890"));
}

TEST_CASE("memepp::string_view repeat - single char view", "[string_view]")
{
    memepp::string_view s = "x";

    REQUIRE(s.repeat(4) == "xxxx");
    REQUIRE(s.repeat(1) == "x");
    REQUIRE(s.repeat(0) == "");
}

TEST_CASE("memepp::string_view repeat - result is memepp::string", "[string_view]")
{
    memepp::string_view s = "Hi";
    auto r = s.repeat(3);

    REQUIRE(r == "HiHiHi");
    REQUIRE(r.size() == 6);
}
