#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/rune_iterator.hpp>

#include <iterator>
#include <vector>

// Mixed 1/2/3/4-byte text:
//   'A'      -> 1 byte  (0x41)
//   U+00E9   -> 2 bytes (C3 A9)
//   U+4E2D   -> 3 bytes (E4 B8 AD)
//   U+1F600  -> 4 bytes (F0 9F 98 80)
static const char MIXED[] = "A\xC3\xA9\xE4\xB8\xAD\xF0\x9F\x98\x80";

// ---------------------------------------------------------------------------
// forward traversal over mixed-width text
// ---------------------------------------------------------------------------

TEST_CASE("const_rune_iterator: mixed 1/2/3/4-byte forward", "[rune_iterator]")
{
    memepp::string s(MIXED);

    std::vector<int> sizes;
    for (auto it = s.rune_begin(); it != s.rune_end(); ++it) {
        REQUIRE(it.is_valid());
        sizes.push_back(static_cast<int>(it.to_index().size()));
    }

    REQUIRE(sizes == std::vector<int>{ 1, 2, 3, 4 });
    REQUIRE(std::distance(s.rune_begin(), s.rune_end()) == 4);
}

// ---------------------------------------------------------------------------
// backward traversal over mixed-width text
// ---------------------------------------------------------------------------

TEST_CASE("const_rune_iterator: mixed backward", "[rune_iterator]")
{
    memepp::string s(MIXED);

    std::vector<int> sizes;
    for (auto it = std::prev(s.rune_end(), 1); it != s.rune_begin(); --it) {
        if (!it.is_valid())
            continue;
        sizes.push_back(static_cast<int>(it.to_index().size()));
    }
    sizes.push_back(static_cast<int>(s.rune_begin().to_index().size()));

    REQUIRE(sizes == std::vector<int>{ 4, 3, 2, 1 });
}

// ---------------------------------------------------------------------------
// empty string
// ---------------------------------------------------------------------------

TEST_CASE("const_rune_iterator: empty string", "[rune_iterator]")
{
    memepp::string s;

    REQUIRE(s.rune_begin() == s.rune_end());
    REQUIRE(std::distance(s.rune_begin(), s.rune_end()) == 0);
}

// ---------------------------------------------------------------------------
// invalid byte in the middle: traversal stops at it
// ---------------------------------------------------------------------------

TEST_CASE("const_rune_iterator: invalid byte in the middle", "[rune_iterator]")
{
    const char raw[] = { 'A', static_cast<char>(0xFF), 'A', '\0' };
    memepp::string s(raw, 3);

    auto it = s.rune_begin();
    REQUIRE(it.is_valid());

    ++it;
    REQUIRE_FALSE(it.is_valid());   // stuck on the invalid byte
}

// ---------------------------------------------------------------------------
// equality across copies
// ---------------------------------------------------------------------------

TEST_CASE("const_rune_iterator: copy equality", "[rune_iterator]")
{
    memepp::string s(MIXED);

    auto a = s.rune_begin();
    auto b = a;
    REQUIRE(a == b);

    ++b;
    REQUIRE(a != b);
    REQUIRE(a == s.rune_begin());

    auto c = s.rune_end();
    auto d = c;
    REQUIRE(c == d);
    REQUIRE(c == s.rune_end());
}
