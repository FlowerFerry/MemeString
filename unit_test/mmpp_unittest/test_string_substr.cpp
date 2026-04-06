#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>

// ---------------------------------------------------------------------------
// string::substr  (C++ API; distinct from C API tests in test_string_mid.cpp)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string substr - normal ranges", "[string]")
{
    memepp::string s = "Hello, World!";

    // from the beginning
    REQUIRE(s.substr(0, 5) == "Hello");

    // from middle
    REQUIRE(s.substr(7, 5) == "World");

    // single char
    REQUIRE(s.substr(0, 1) == "H");
    REQUIRE(s.substr(12, 1) == "!");

    // rest of string (default count = npos)
    REQUIRE(s.substr(7) == "World!");
    REQUIRE(s.substr(0) == "Hello, World!");
}

TEST_CASE("memepp::string substr - count larger than remaining", "[string]")
{
    memepp::string s = "abcde";

    // count beyond length → clamp to end
    REQUIRE(s.substr(3, 100) == "de");
    REQUIRE(s.substr(0, 999) == "abcde");
}

TEST_CASE("memepp::string substr - pos equals size returns empty", "[string]")
{
    memepp::string s = "abc";

    REQUIRE(s.substr(3) == "");
    REQUIRE(s.substr(3, 5) == "");
}

TEST_CASE("memepp::string substr - empty string", "[string]")
{
    memepp::string empty;

    REQUIRE(empty.substr()     == "");
    REQUIRE(empty.substr(0)    == "");
    REQUIRE(empty.substr(0, 0) == "");
}

TEST_CASE("memepp::string substr - count zero returns empty", "[string]")
{
    memepp::string s = "Hello";

    REQUIRE(s.substr(0, 0) == "");
    REQUIRE(s.substr(2, 0) == "");
}

TEST_CASE("memepp::string substr - entire string via npos", "[string]")
{
    memepp::string s = "Hello";

    REQUIRE(s.substr(0, memepp::string::npos) == "Hello");
}

TEST_CASE("memepp::string substr - storage type preserved for small result", "[string]")
{
    // A medium/large source should still give small storage for a short substr
    memepp::string s = "ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890";
    REQUIRE(s.storage_type() != memepp::string_storage_t::small);

    auto sub = s.substr(0, 3);
    REQUIRE(sub == "ABC");
    REQUIRE(sub.storage_type() == memepp::string_storage_t::small);
}
