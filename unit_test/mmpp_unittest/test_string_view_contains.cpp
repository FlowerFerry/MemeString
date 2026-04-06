#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

// ---------------------------------------------------------------------------
// string_view::contains
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view contains - true cases", "[string_view]")
{
    memepp::string_view s = "Hello, World!";

    REQUIRE(s.contains(memepp::string_view("Hello")));
    REQUIRE(s.contains(memepp::string_view("World")));
    REQUIRE(s.contains(memepp::string_view("!")));
    REQUIRE(s.contains(memepp::string_view("Hello, World!"))); // full view

    REQUIRE(s.contains("Hello"));
    REQUIRE(s.contains("World"));
    REQUIRE(s.contains(","));

    REQUIRE(s.contains("Hello!", 5));   // "Hello" (5 bytes)
    REQUIRE(s.contains("World!!", 6));  // "World!" (6 bytes)

    REQUIRE(s.contains('H'));
    REQUIRE(s.contains('!'));

    REQUIRE(s.contains(memepp::rune('H')));
    REQUIRE(s.contains(memepp::rune('!')));
}

TEST_CASE("memepp::string_view contains - false cases", "[string_view]")
{
    memepp::string_view s = "Hello, World!";

    REQUIRE_FALSE(s.contains(memepp::string_view("xyz")));
    REQUIRE_FALSE(s.contains("xyz"));
    REQUIRE_FALSE(s.contains("hello")); // case sensitive
    REQUIRE_FALSE(s.contains("Hello, World! Extra")); // longer than s
    REQUIRE_FALSE(s.contains("World!!", 7));           // 7-byte version not present
    REQUIRE_FALSE(s.contains('z'));
    REQUIRE_FALSE(s.contains(memepp::rune('z')));
}

TEST_CASE("memepp::string_view contains - empty / edge cases", "[string_view]")
{
    memepp::string_view empty;
    memepp::string_view s = "abc";

    REQUIRE_FALSE(empty.contains("a"));
    REQUIRE_FALSE(empty.contains('a'));
    REQUIRE_FALSE(empty.contains(memepp::rune('a')));

    REQUIRE_FALSE(s.contains(""));
    REQUIRE_FALSE(s.contains(memepp::string_view("")));
}

TEST_CASE("memepp::string_view contains - pointer overload", "[string_view]")
{
    memepp::string_view s = "AABBCC";
    const memepp::string_view::const_pointer p =
        reinterpret_cast<const memepp::string_view::const_pointer>("BB");

    REQUIRE(s.contains(p));
    REQUIRE(s.contains(p, 2));

    const memepp::string_view::const_pointer p2 =
        reinterpret_cast<const memepp::string_view::const_pointer>("DD");
    REQUIRE_FALSE(s.contains(p2));
    REQUIRE_FALSE(s.contains(p2, 2));
}

// ---------------------------------------------------------------------------
// string_view::starts_with
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view starts_with - true cases", "[string_view]")
{
    memepp::string_view s = "Hello, World!";

    REQUIRE(s.starts_with(memepp::string_view("Hello")));
    REQUIRE(s.starts_with("Hello"));
    REQUIRE(s.starts_with("Hello,", 6));
    REQUIRE(s.starts_with(memepp::rune('H')));
    REQUIRE(s.starts_with(memepp::string_view("Hello, World!")));
}

TEST_CASE("memepp::string_view starts_with - false cases", "[string_view]")
{
    memepp::string_view s = "Hello, World!";

    REQUIRE_FALSE(s.starts_with(memepp::string_view("World")));
    REQUIRE_FALSE(s.starts_with("hello"));
    REQUIRE_FALSE(s.starts_with("Hello, World! Extra"));
    REQUIRE_FALSE(s.starts_with(memepp::rune('W')));
}

TEST_CASE("memepp::string_view starts_with - empty / edge cases", "[string_view]")
{
    memepp::string_view empty;
    memepp::string_view s = "abc";

    REQUIRE_FALSE(empty.starts_with("a"));
    REQUIRE_FALSE(empty.starts_with(memepp::rune('a')));
    REQUIRE_FALSE(s.starts_with("abcdef")); // pattern longer
}

TEST_CASE("memepp::string_view starts_with - pointer overload", "[string_view]")
{
    memepp::string_view s = "PrefixData";
    const memepp::string_view::const_pointer p =
        reinterpret_cast<const memepp::string_view::const_pointer>("Prefix");

    REQUIRE(s.starts_with(p));
    REQUIRE(s.starts_with(p, 6));

    const memepp::string_view::const_pointer p2 =
        reinterpret_cast<const memepp::string_view::const_pointer>("Data");
    REQUIRE_FALSE(s.starts_with(p2));
}

// ---------------------------------------------------------------------------
// string_view::ends_with
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view ends_with - true cases", "[string_view]")
{
    memepp::string_view s = "Hello, World!";

    REQUIRE(s.ends_with(memepp::string_view("World!")));
    REQUIRE(s.ends_with("World!"));
    REQUIRE(s.ends_with("World!", 6));
    REQUIRE(s.ends_with(memepp::rune('!')));
    REQUIRE(s.ends_with(memepp::string_view("Hello, World!")));
}

TEST_CASE("memepp::string_view ends_with - false cases", "[string_view]")
{
    memepp::string_view s = "Hello, World!";

    REQUIRE_FALSE(s.ends_with(memepp::string_view("Hello")));
    REQUIRE_FALSE(s.ends_with("world!"));
    REQUIRE_FALSE(s.ends_with("Hello, World! Extra"));
    REQUIRE_FALSE(s.ends_with(memepp::rune('H')));
}

TEST_CASE("memepp::string_view ends_with - empty / edge cases", "[string_view]")
{
    memepp::string_view empty;
    memepp::string_view s = "abc";

    REQUIRE_FALSE(empty.ends_with("a"));
    REQUIRE_FALSE(empty.ends_with(memepp::rune('a')));
    REQUIRE_FALSE(s.ends_with("xyzabc")); // pattern longer
}

TEST_CASE("memepp::string_view ends_with - pointer overload", "[string_view]")
{
    memepp::string_view s = "DataSuffix";
    const memepp::string_view::const_pointer p =
        reinterpret_cast<const memepp::string_view::const_pointer>("Suffix");

    REQUIRE(s.ends_with(p));
    REQUIRE(s.ends_with(p, 6));

    const memepp::string_view::const_pointer p2 =
        reinterpret_cast<const memepp::string_view::const_pointer>("Data");
    REQUIRE_FALSE(s.ends_with(p2));
}
