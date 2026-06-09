#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

// ---------------------------------------------------------------------------
// string_view::starts_with  — all overloads
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view starts_with — string_view", "[string_view]")
{
    memepp::string_view sv = "Hello, World!";
    REQUIRE(sv.starts_with(memepp::string_view("Hello")));
    REQUIRE(sv.starts_with(memepp::string_view("")));
    REQUIRE(sv.starts_with(memepp::string_view("Hello, World!")));
    REQUIRE_FALSE(sv.starts_with(memepp::string_view("World")));
    REQUIRE_FALSE(sv.starts_with(memepp::string_view("Hello, World!!")));
}

TEST_CASE("memepp::string_view starts_with — const char*", "[string_view]")
{
    memepp::string_view sv = "Hello, World!";
    REQUIRE(sv.starts_with("Hello"));
    REQUIRE(sv.starts_with(""));
    REQUIRE_FALSE(sv.starts_with("World"));
}

TEST_CASE("memepp::string_view starts_with — const char* with count", "[string_view]")
{
    memepp::string_view sv = "Hello, World!";
    REQUIRE(sv.starts_with("HelloXYZ", 5));
    REQUIRE(sv.starts_with("HelloXYZ", 0));
    REQUIRE_FALSE(sv.starts_with("HelloXYZ", 6));
    REQUIRE_FALSE(sv.starts_with("Hello", 6)); // 6 > "Hello" length
}

TEST_CASE("memepp::string_view starts_with — const_pointer", "[string_view]")
{
    memepp::string_view sv = "Hello, World!";
    const memepp::string_view::const_pointer p =
        reinterpret_cast<const memepp::string_view::const_pointer>("Hello");
    REQUIRE(sv.starts_with(p));
    REQUIRE_FALSE(sv.starts_with(
        reinterpret_cast<const memepp::string_view::const_pointer>("World")));
}

TEST_CASE("memepp::string_view starts_with — const_pointer with count", "[string_view]")
{
    memepp::string_view sv = "Hello, World!";
    const memepp::string_view::const_pointer p =
        reinterpret_cast<const memepp::string_view::const_pointer>("HelloXYZ");
    REQUIRE(sv.starts_with(p, 5));
    REQUIRE_FALSE(sv.starts_with(p, 6));
}

TEST_CASE("memepp::string_view starts_with — rune", "[string_view]")
{
    memepp::string_view sv = u8"你好世界"; // 你好世界
    memepp::rune r_head{ reinterpret_cast<const uint8_t*>(u8"你"), -1 }; // 你
    memepp::rune r_wrong{ reinterpret_cast<const uint8_t*>(u8"好"), -1 }; // 好
    REQUIRE(sv.starts_with(r_head));
    REQUIRE_FALSE(sv.starts_with(r_wrong));
}

TEST_CASE("memepp::string_view starts_with — edge cases", "[string_view]")
{
    memepp::string_view empty;
    memepp::string_view sv = "abc";

    REQUIRE_FALSE(empty.starts_with("a"));
    REQUIRE(empty.starts_with(""));

    REQUIRE(sv.starts_with(""));
    REQUIRE_FALSE(sv.starts_with("abcd"));
}

// ---------------------------------------------------------------------------
// string_view::ends_with — all overloads
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view ends_with — string_view", "[string_view]")
{
    memepp::string_view sv = "Hello, World!";
    REQUIRE(sv.ends_with(memepp::string_view("World!")));
    REQUIRE(sv.ends_with(memepp::string_view("")));
    REQUIRE(sv.ends_with(memepp::string_view("Hello, World!")));
    REQUIRE_FALSE(sv.ends_with(memepp::string_view("Hello")));
    REQUIRE_FALSE(sv.ends_with(memepp::string_view(", World!!")));
}

TEST_CASE("memepp::string_view ends_with — const char*", "[string_view]")
{
    memepp::string_view sv = "Hello, World!";
    REQUIRE(sv.ends_with("World!"));
    REQUIRE(sv.ends_with(""));
    REQUIRE_FALSE(sv.ends_with("Hello"));
}

TEST_CASE("memepp::string_view ends_with — const char* with count", "[string_view]")
{
    memepp::string_view sv = "Hello, World!";
    // count limits bytes taken from pattern: ends_with checks if view ends with the first `count` bytes of the pattern
    REQUIRE(sv.ends_with("World!extra", 6));
    REQUIRE(sv.ends_with("", 0));
    REQUIRE_FALSE(sv.ends_with("World!extra", 7));
}

TEST_CASE("memepp::string_view ends_with — const_pointer", "[string_view]")
{
    memepp::string_view sv = "Hello, World!";
    const memepp::string_view::const_pointer p =
        reinterpret_cast<const memepp::string_view::const_pointer>("World!");
    REQUIRE(sv.ends_with(p));
    REQUIRE_FALSE(sv.ends_with(
        reinterpret_cast<const memepp::string_view::const_pointer>("Hello")));
}

TEST_CASE("memepp::string_view ends_with — const_pointer with count", "[string_view]")
{
    memepp::string_view sv = "Hello, World!";
    // count limits bytes taken from pattern prefix: first 6 bytes of "World!XYZ" = "World!"
    const memepp::string_view::const_pointer p =
        reinterpret_cast<const memepp::string_view::const_pointer>("World!XYZ");
    REQUIRE(sv.ends_with(p, 6));
    REQUIRE_FALSE(sv.ends_with(p, 7));
}

TEST_CASE("memepp::string_view ends_with — rune", "[string_view]")
{
    memepp::string_view sv = u8"你好世界"; // 你好世界
    memepp::rune r_tail{ reinterpret_cast<const uint8_t*>(u8"界"), -1 }; // 界
    memepp::rune r_wrong{ reinterpret_cast<const uint8_t*>(u8"世"), -1 }; // 世
    REQUIRE(sv.ends_with(r_tail));
    REQUIRE_FALSE(sv.ends_with(r_wrong));
}

TEST_CASE("memepp::string_view ends_with — edge cases", "[string_view]")
{
    memepp::string_view empty;
    memepp::string_view sv = "abc";

    REQUIRE_FALSE(empty.ends_with("a"));
    REQUIRE(empty.ends_with(""));

    REQUIRE(sv.ends_with(""));
    REQUIRE_FALSE(sv.ends_with("xabc"));
}
