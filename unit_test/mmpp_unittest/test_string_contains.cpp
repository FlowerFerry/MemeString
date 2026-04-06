#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>

// ---------------------------------------------------------------------------
// string::contains
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string contains - true cases", "[string]")
{
    memepp::string s = "Hello, World!";

    // contains(string_view)
    REQUIRE(s.contains(memepp::string_view("Hello")));
    REQUIRE(s.contains(memepp::string_view("World")));
    REQUIRE(s.contains(memepp::string_view("!")));
    REQUIRE(s.contains(memepp::string_view("Hello, World!"))); // full string

    // contains(const char*)
    REQUIRE(s.contains("Hello"));
    REQUIRE(s.contains("World"));
    REQUIRE(s.contains(","));

    // contains(const char*, count)
    REQUIRE(s.contains("Hello!", 5));  // "Hello" (5 bytes)
    REQUIRE(s.contains("World!!", 6)); // "World!" (6 bytes)

    // contains(char)
    REQUIRE(s.contains('H'));
    REQUIRE(s.contains('!'));
    REQUIRE(s.contains(','));

    // contains(rune)
    REQUIRE(s.contains(memepp::rune('H')));
    REQUIRE(s.contains(memepp::rune('!')));
}

TEST_CASE("memepp::string contains - false cases", "[string]")
{
    memepp::string s = "Hello, World!";

    REQUIRE_FALSE(s.contains(memepp::string_view("xyz")));
    REQUIRE_FALSE(s.contains("xyz"));
    REQUIRE_FALSE(s.contains("hello")); // case sensitive
    REQUIRE_FALSE(s.contains("Hello, World! Extra")); // longer than s
    REQUIRE_FALSE(s.contains("World!!", 7));           // 7-byte prefix not present
    REQUIRE_FALSE(s.contains('z'));
    REQUIRE_FALSE(s.contains(memepp::rune('z')));
}

TEST_CASE("memepp::string contains - empty / edge cases", "[string]")
{
    memepp::string empty;
    memepp::string s = "abc";

    // empty string contains nothing
    REQUIRE_FALSE(empty.contains("a"));
    REQUIRE_FALSE(empty.contains('a'));
    REQUIRE_FALSE(empty.contains(memepp::rune('a')));

    // non-empty string does not contain empty pattern
    REQUIRE_FALSE(s.contains(""));
    REQUIRE_FALSE(s.contains(memepp::string_view("")));
}

TEST_CASE("memepp::string contains - pointer overload", "[string]")
{
    memepp::string s = "AABBCC";
    const memepp::string::const_pointer p =
        reinterpret_cast<const memepp::string::const_pointer>("BB");

    REQUIRE(s.contains(p));
    REQUIRE(s.contains(p, 2));  // "BB"

    const memepp::string::const_pointer p2 =
        reinterpret_cast<const memepp::string::const_pointer>("DD");
    REQUIRE_FALSE(s.contains(p2));
    REQUIRE_FALSE(s.contains(p2, 2));
}

// ---------------------------------------------------------------------------
// string::starts_with
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string starts_with - true cases", "[string]")
{
    memepp::string s = "Hello, World!";

    REQUIRE(s.starts_with(memepp::string_view("Hello")));
    REQUIRE(s.starts_with("Hello"));
    REQUIRE(s.starts_with("Hello,", 6));
    REQUIRE(s.starts_with(memepp::rune('H')));
    REQUIRE(s.starts_with(memepp::string_view("Hello, World!"))); // full string
}

TEST_CASE("memepp::string starts_with - false cases", "[string]")
{
    memepp::string s = "Hello, World!";

    REQUIRE_FALSE(s.starts_with(memepp::string_view("World")));
    REQUIRE_FALSE(s.starts_with("hello")); // case sensitive
    REQUIRE_FALSE(s.starts_with("Hello, World! Extra")); // longer
    REQUIRE_FALSE(s.starts_with(memepp::rune('W')));
    REQUIRE_FALSE(s.starts_with("Hello,", 7)); // 7 chars: "Hello, " — the space+comma combo
}

TEST_CASE("memepp::string starts_with - empty / edge cases", "[string]")
{
    memepp::string empty;
    memepp::string s = "abc";

    REQUIRE_FALSE(empty.starts_with("a"));
    REQUIRE_FALSE(empty.starts_with(memepp::rune('a')));
    // pattern longer than string
    REQUIRE_FALSE(s.starts_with("abcdef"));
}

TEST_CASE("memepp::string starts_with - pointer overload", "[string]")
{
    memepp::string s = "PrefixData";
    const memepp::string::const_pointer p =
        reinterpret_cast<const memepp::string::const_pointer>("Prefix");

    REQUIRE(s.starts_with(p));
    REQUIRE(s.starts_with(p, 6));

    const memepp::string::const_pointer p2 =
        reinterpret_cast<const memepp::string::const_pointer>("Data");
    REQUIRE_FALSE(s.starts_with(p2));
}

// ---------------------------------------------------------------------------
// string::ends_with
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string ends_with - true cases", "[string]")
{
    memepp::string s = "Hello, World!";

    REQUIRE(s.ends_with(memepp::string_view("World!")));
    REQUIRE(s.ends_with("World!"));
    REQUIRE(s.ends_with("World!", 6));
    REQUIRE(s.ends_with(memepp::rune('!')));
    REQUIRE(s.ends_with(memepp::string_view("Hello, World!"))); // full string
}

TEST_CASE("memepp::string ends_with - false cases", "[string]")
{
    memepp::string s = "Hello, World!";

    REQUIRE_FALSE(s.ends_with(memepp::string_view("Hello")));
    REQUIRE_FALSE(s.ends_with("world!")); // case sensitive
    REQUIRE_FALSE(s.ends_with("Hello, World! Extra")); // longer
    REQUIRE_FALSE(s.ends_with(memepp::rune('H')));
    REQUIRE_FALSE(s.ends_with("World!", 7)); // 7 bytes: " World!" — leading space not present at end
}

TEST_CASE("memepp::string ends_with - empty / edge cases", "[string]")
{
    memepp::string empty;
    memepp::string s = "abc";

    REQUIRE_FALSE(empty.ends_with("a"));
    REQUIRE_FALSE(empty.ends_with(memepp::rune('a')));
    // pattern longer than string
    REQUIRE_FALSE(s.ends_with("xyzabc"));
}

TEST_CASE("memepp::string ends_with - pointer overload", "[string]")
{
    memepp::string s = "DataSuffix";
    const memepp::string::const_pointer p =
        reinterpret_cast<const memepp::string::const_pointer>("Suffix");

    REQUIRE(s.ends_with(p));
    REQUIRE(s.ends_with(p, 6));

    const memepp::string::const_pointer p2 =
        reinterpret_cast<const memepp::string::const_pointer>("Data");
    REQUIRE_FALSE(s.ends_with(p2));
}
