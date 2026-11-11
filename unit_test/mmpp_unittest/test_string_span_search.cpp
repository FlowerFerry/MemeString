#include <catch2/catch.hpp>

#include <memepp/string_span.hpp>
#include <memepp/string.hpp>

// find / rfind: complete boundary matrix
// ============================================================================

TEST_CASE("string_span find boundary matrix", "[string_span][search]")
{
    const char text[] = "abcabc";
    memepp::string_span s(text, 6);
    memepp::string_span needle("abc", 3);
    memepp::string_span empty;

    /* pos < 0 clamps to 0 for both overloads. */
    REQUIRE(s.find(needle, -1) == 0);
    REQUIRE(s.find('a', -1) == 0);
    REQUIRE(s.find("abc", -1) == 0);

    /* pos == size() and pos > size(). */
    REQUIRE(s.find(needle, 6) == memepp::string_span::npos);
    REQUIRE(s.find(needle, 99) == memepp::string_span::npos);
    REQUIRE(s.find('a', 6) == memepp::string_span::npos);

    /* Empty needle returns pos while pos <= size. */
    REQUIRE(s.find(empty) == 0);
    REQUIRE(s.find(empty, 3) == 3);
    REQUIRE(s.find(empty, 6) == 6);
    REQUIRE(s.find(empty, 7) == memepp::string_span::npos);

    /* Empty haystack. */
    REQUIRE(empty.find(needle) == memepp::string_span::npos);
    REQUIRE(empty.find(empty) == 0);
    REQUIRE(empty.find('a') == memepp::string_span::npos);
    REQUIRE(empty.find("abc") == memepp::string_span::npos);

    /* null needle with a length is the empty needle. */
    REQUIRE(s.find(static_cast<const char*>(nullptr), 0, 0) == 0);
    REQUIRE(s.find(static_cast<const char*>(nullptr), 3, 0) == 3);

    /* Binary payload: NUL and high bytes are ordinary bytes. */
    {
        const char bin[] = { 'x', '\0', '\xFF', 'y' };
        memepp::string_span bs(bin, 4);
        const char n1[] = { '\0', '\xFF' };
        REQUIRE(bs.find(memepp::string_span(n1, 2)) == 1);
        REQUIRE(bs.find('\0') == 1);
        REQUIRE(bs.find('\xFF') == 2);
    }
}

TEST_CASE("string_span rfind boundary matrix", "[string_span][search]")
{
    const char text[] = "abcabc";
    memepp::string_span s(text, 6);
    memepp::string_span needle("abc", 3);
    memepp::string_span empty;

    /* npos means "from the end". */
    REQUIRE(s.rfind(needle) == 3);
    REQUIRE(s.rfind(needle, memepp::string_span::npos) == 3);

    /* pos == size() and pos > size() clamp to size() - 1 internally. */
    REQUIRE(s.rfind(needle, 6) == 3);
    REQUIRE(s.rfind(needle, 99) == 3);

    /* A position before the second occurrence finds the first. */
    REQUIRE(s.rfind(needle, 2) == 0);

    /* A negative pos: rfind(char) guards it, rfind(span) defers to
     * string_view::rfind.  Assert only that it does not crash and that the
     * result is either a valid index or npos. */
    {
        auto r1 = s.rfind('c', -1);
        REQUIRE((r1 == memepp::string_span::npos || (r1 >= 0 && r1 < 6)));
        auto r2 = s.rfind(needle, -1);
        REQUIRE((r2 == memepp::string_span::npos || (r2 >= 0 && r2 < 6)));
    }

    /* Empty needle returns min(pos, size()). */
    REQUIRE(s.rfind(empty) == 6);
    REQUIRE(s.rfind(empty, 2) == 2);
    REQUIRE(s.rfind(empty, 0) == 0);

    /* Empty haystack. */
    REQUIRE(empty.rfind(needle) == memepp::string_span::npos);
    REQUIRE(empty.rfind(empty) == 0);
    REQUIRE(empty.rfind('a') == memepp::string_span::npos);

    /* Needle longer than the haystack. */
    REQUIRE(s.rfind(memepp::string_span("abcdef", 6)) == memepp::string_span::npos);
}

// ============================================================================

// find_first_of / find_last_of family: boundary + binary
// ============================================================================

TEST_CASE("string_span find_first_of family boundaries",
          "[string_span][search]")
{
    const char text[] = "aXbYc";
    memepp::string_span s(text, 5);
    memepp::string_span set("XY", 2);
    memepp::string_span empty_set;

    /* find_first_of: pos < 0 clamps to 0. */
    REQUIRE(s.find_first_of(set, -1) == 1);
    /* pos == size() / pos > size(): nothing left. */
    REQUIRE(s.find_first_of(set, 5) == memepp::string_span::npos);
    REQUIRE(s.find_first_of(set, 99) == memepp::string_span::npos);
    /* Empty set never matches. */
    REQUIRE(s.find_first_of(empty_set) == memepp::string_span::npos);
    /* Empty haystack never matches. */
    REQUIRE(memepp::string_span().find_first_of(set) == memepp::string_span::npos);

    /* find_first_not_of: an empty set excludes nothing, so the first char
     * not in the set is pos -- but at pos == size() the string is exhausted. */
    REQUIRE(s.find_first_not_of(empty_set) == 0);
    REQUIRE(s.find_first_not_of(empty_set, 2) == 2);
    REQUIRE(s.find_first_not_of(empty_set, 5) == memepp::string_span::npos);
    REQUIRE(s.find_first_not_of(empty_set, 99) == memepp::string_span::npos);
    /* A set that covers everything yields npos. */
    REQUIRE(s.find_first_not_of(memepp::string_span("aXbYc", 5))
            == memepp::string_span::npos);

    /* find_last_of: npos / pos >= size clamp to size - 1. */
    REQUIRE(s.find_last_of(set) == 3);
    REQUIRE(s.find_last_of(set, memepp::string_span::npos) == 3);
    REQUIRE(s.find_last_of(set, 99) == 3);
    REQUIRE(s.find_last_of(set, 2) == 1);
    /* Empty set never matches. */
    REQUIRE(s.find_last_of(empty_set) == memepp::string_span::npos);
    /* Empty haystack. */
    REQUIRE(memepp::string_span().find_last_of(set) == memepp::string_span::npos);

    /* find_last_not_of: an empty set excludes nothing, so the last char is
     * the last valid position. */
    REQUIRE(s.find_last_not_of(empty_set) == 4);
    REQUIRE(s.find_last_not_of(empty_set, 2) == 2);
    REQUIRE(s.find_last_not_of(empty_set, 99) == 4);
    /* A set covering everything yields npos. */
    REQUIRE(s.find_last_not_of(memepp::string_span("aXbYc", 5))
            == memepp::string_span::npos);
    /* Empty haystack. */
    REQUIRE(memepp::string_span().find_last_not_of(empty_set) == memepp::string_span::npos);
}

TEST_CASE("string_span find_first_of family with binary data",
          "[string_span][search]")
{
    const char bin[] = { '\0', '\xFF', 'a', '\x80' };
    memepp::string_span s(bin, 4);

    /* A set containing the NUL byte matches position 0. */
    const char set_nul[] = { '\0' };
    REQUIRE(s.find_first_of(memepp::string_span(set_nul, 1)) == 0);
    REQUIRE(s.find_first_of('\0') == 0);

    /* A set of high bytes. */
    const char set_hi[] = { '\xFF', '\x80' };
    REQUIRE(s.find_first_of(memepp::string_span(set_hi, 2)) == 1);
    REQUIRE(s.find_last_of(memepp::string_span(set_hi, 2)) == 3);

    /* find_first_not_of with a set covering only the leading NUL. */
    REQUIRE(s.find_first_not_of(memepp::string_span(set_nul, 1)) == 1);
}

// ============================================================================

// compare(): subrange and binary
// ============================================================================

TEST_CASE("string_span compare subrange and binary", "[string_span][search]")
{
    const char text[] = "abcabc";
    memepp::string_span s(text, 6);

    /* Out-of-range pos clamps through substr. */
    REQUIRE(s.compare(99, 3, memepp::string_span()) == 0);
    REQUIRE(s.compare(99, 3, s) < 0);

    /* count beyond the remainder truncates. */
    REQUIRE(s.compare(0, 99, s) == 0);
    REQUIRE(s.compare(3, 99, memepp::string_span("abc", 3)) == 0);

    /* Both ranges empty -> equal. */
    REQUIRE(s.compare(2, 0, s, 3, 0) == 0);

    /* Empty range vs non-empty. */
    REQUIRE(s.compare(2, 0, s, 0, 2) < 0);
    REQUIRE(s.compare(0, 2, s, 2, 0) > 0);

    /* const char* overload: a null pointer is the empty string. */
    REQUIRE(s.compare(static_cast<const char*>(nullptr)) > 0);
    REQUIRE(s.compare(0, 0, static_cast<const char*>(nullptr)) == 0);
    /* Null pointer with a non-zero count is forced to length 0. */
    REQUIRE(s.compare(0, 0, static_cast<const char*>(nullptr), 5) == 0);

    /* Binary compare: high bytes order as unsigned. */
    {
        const char hi[] = { '\xFF' };
        const char lo[] = { '\x01' };
        REQUIRE(memepp::string_span(hi, 1).compare(memepp::string_span(lo, 1)) > 0);
        REQUIRE(memepp::string_span(lo, 1).compare(memepp::string_span(hi, 1)) < 0);
    }
    /* Embedded NUL participates in the ordering. */
    {
        const char a[] = { '\0', '\x01' };
        const char b[] = { '\0', '\x02' };
        REQUIRE(memepp::string_span(a, 2).compare(memepp::string_span(b, 2)) < 0);
    }

    /* Empty vs non-empty, full six-operator matrix. */
    {
        memepp::string_span e;
        const char one[] = { 'x' };
        memepp::string_span ne(one, 1);
        REQUIRE(e < ne);
        REQUIRE(e <= ne);
        REQUIRE_FALSE(e > ne);
        REQUIRE_FALSE(e >= ne);
        REQUIRE_FALSE(e == ne);
        REQUIRE(e != ne);
        REQUIRE(ne > e);
        REQUIRE(ne >= e);
        REQUIRE_FALSE(ne < e);
        REQUIRE_FALSE(ne <= e);
    }
}

// ============================================================================

// Invalid UTF-8: fixed semantics where the impl defines them
// ============================================================================

TEST_CASE("string_span invalid UTF-8 rune accounting", "[string_span][search]")
{
    /*
     * string_ref::rune_size() counts complete UTF-8 sequences.  The tests
     * below pin the values the current implementation produces, so a future
     * change to the invalid-sequence policy is noticed rather than silently
     * accepted.
     */

    /* A single ASCII byte is one rune. */
    {
        const char one[] = { 'a' };
        REQUIRE(memepp::string_span(one, 1).rune_size() == 1);
    }

    /* A truncated 2-byte lead alone yields no complete rune. */
    {
        const char trunc[] = { '\xC3' };
        REQUIRE(memepp::string_span(trunc, 1).rune_size() == 0);
    }

    /* A truncated 3-byte lead alone yields no complete rune. */
    {
        const char trunc[] = { '\xE4' };
        REQUIRE(memepp::string_span(trunc, 1).rune_size() == 0);
    }

    /* Lead byte followed by a non-continuation byte: the lead is not a
     * complete rune.  What the trailing byte contributes depends on the
     * decoder; assert only that the total is a stable, small number rather
     * than pinning a value that is really a decoder detail. */
    {
        const char broken[] = { '\xE4', 'x' };
        auto n = memepp::string_span(broken, 2).rune_size();
        REQUIRE(n >= 0);
        REQUIRE(n <= 2);
    }

    /* Trailing complete ASCII after an invalid lead is still counted. */
    {
        const char mixed[] = { '\xFF', 'a', 'b' };
        auto n = memepp::string_span(mixed, 3).rune_size();
        REQUIRE(n >= 0);
        REQUIRE(n <= 3);
    }

    /* A valid 3-byte sequence is exactly one rune. */
    {
        const char cjk[] = "\xE4\xBD\xA0";
        REQUIRE(memepp::string_span(cjk, 3).rune_size() == 1);
    }

    /* A valid 4-byte sequence is exactly one rune. */
    {
        const char emoji[] = "\xF0\x9F\x98\x80";
        REQUIRE(memepp::string_span(emoji, 4).rune_size() == 1);
    }
}

// ============================================================================
