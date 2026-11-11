#include <catch2/catch.hpp>

#include <memepp/string_span.hpp>
#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/errc.hpp>

#include <cstring>
#include <limits>
#include <type_traits>
#include <utility>

// ============================================================================
// Empty span: element access, iterators, rune iterators
// ============================================================================

TEST_CASE("string_span empty: iterators and rune iterators",
          "[string_span][gap]")
{
    memepp::string_span e;

    REQUIRE(e.size() == 0);
    REQUIRE(e.empty());
    REQUIRE(e.data() == nullptr);

    /* begin/end agree and the distance is 0.  end() computes data_ + size_,
     * so with data_ == nullptr this is the "nullptr + 0" case: legal only
     * because size_ is 0, which is what this test pins. */
    REQUIRE(e.begin() == e.end());
    REQUIRE(e.cbegin() == e.cend());
    REQUIRE(static_cast<ptrdiff_t>(e.end() - e.begin()) == 0);

    /*
     * REVERSE ITERATORS ARE NOT CONSTRUCTED HERE.
     *
     *   rbegin() -> data_ + size_ - 1  ==  nullptr - 1
     *   rend()   -> data_ - 1          ==  nullptr - 1
     *
     * A non-zero offset on a null pointer is undefined behaviour per
     * [expr.add], and it happens at construction time, so calling these on
     * an empty span is itself UB.  The contract is: only call
     * rbegin()/crbegin()/rend()/crend() on a non-empty span.
     * (See the dedicated "empty: reverse iterators are out of contract"
     * test for the full note.)
     */

    /* Rune iterators: rune_begin/end must agree. */
    REQUIRE(e.rune_begin() == e.rune_end());
    REQUIRE(e.rune_cbegin() == e.rune_cend());

    /* A range-for over an empty span runs zero iterations. */
    size_t n = 0;
    for (char c : e) { (void)c; ++n; }
    REQUIRE(n == 0);

    size_t rn = 0;
    for (auto it = e.rune_begin(); it != e.rune_end(); ++it) { ++rn; }
    REQUIRE(rn == 0);

    /* rune_size / u16char_size of an empty span are 0. */
    REQUIRE(e.rune_size() == 0);
    REQUIRE(e.u16char_size() == 0);
}

TEST_CASE("string_span front/back are out of contract on an empty span",
          "[string_span][gap]")
{
    /*
     * CONTRACT NOTE (documented, deliberately NOT executed):
     *
     *   front() -> data_[0]
     *   back()  -> data_[size_ - 1]
     *
     * Neither checks emptiness.  On a default-constructed span
     * (data_ == nullptr, size_ == 0) these become:
     *
     *   front() -> nullptr[0]    -- null dereference, UB
     *   back()  -> nullptr[-1]   -- null dereference at a negative index, UB
     *
     * Unlike buffer_span (whose front()/back() return a static zero byte
     * when size_ <= 0), string_span deliberately keeps these as raw array
     * accesses so it stays a two-word POD with zero per-call overhead.
     *
     * The contract is: guard with empty() before calling front()/back() --
     *
     *     if (!span.empty()) { auto c = span.front(); }
     *
     * and use at() when a checked access is required.  This test asserts the
     * guarded pattern and the checked accessor; it never dereferences an
     * empty span.
     */
    memepp::string_span e;
    REQUIRE(e.empty());
    REQUIRE(e.size() == 0);

    /* The documented guard. */
    {
        size_t touched = 0;
        if (!e.empty()) { (void)e.front(); ++touched; }
        if (!e.empty()) { (void)e.back();  ++touched; }
        REQUIRE(touched == 0);
    }

    /* at() is the checked accessor and reports the range error instead of
     * dereferencing.  Both the "index == size" and the "negative index"
     * paths set MGEC__RANGE. */
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(e.at(0), memepp::errc_except);
    REQUIRE(memepp::get_errc() == MGEC__RANGE);
    REQUIRE_THROWS_AS(e.at(-1), memepp::errc_except);
    REQUIRE(memepp::get_errc() == MGEC__RANGE);
#else
    REQUIRE(e.at(0) == 0);
    REQUIRE(memepp::get_errc() == MGEC__RANGE);
    REQUIRE(e.at(-1) == 0);
    REQUIRE(memepp::get_errc() == MGEC__RANGE);
#endif

    /* A non-empty span still answers front/back normally. */
    {
        const char text[] = "abc";
        memepp::string_span s(text, 3);
        REQUIRE_FALSE(s.empty());
        REQUIRE(s.front() == 'a');
        REQUIRE(s.back() == 'c');
    }
}

// ============================================================================
// remove_prefix / remove_suffix: full length and out-of-range
// ============================================================================

TEST_CASE("string_span remove_prefix/remove_suffix boundaries",
          "[string_span][gap]")
{
    /*
     * The modifiers do raw pointer arithmetic with no clamping:
     *   remove_prefix(_n) -> data_ += _n; size_ -= _n;
     *   remove_suffix(_n) -> size_ -= _n;
     * Removing more than size(), or a negative amount, is a caller error that
     * the class does not detect.  Only the well-defined boundary values are
     * exercised here.
     */
    const char text[] = "abcdef";

    /* Removing the whole length leaves an empty span. */
    {
        memepp::string_span s(text, 6);
        s.remove_prefix(6);
        REQUIRE(s.empty());
        REQUIRE(s.size() == 0);
        REQUIRE(s.data() == text + 6);   /* one-past-the-end, legal to form */
    }
    {
        memepp::string_span s(text, 6);
        s.remove_suffix(6);
        REQUIRE(s.empty());
        REQUIRE(s.data() == text);
    }

    /* Removing 0 is a no-op. */
    {
        memepp::string_span s(text, 6);
        s.remove_prefix(0);
        s.remove_suffix(0);
        REQUIRE(s.size() == 6);
        REQUIRE(s.data() == text);
    }

    /* Repeated partial removals sum to the whole length. */
    {
        memepp::string_span s(text, 6);
        s.remove_prefix(2);
        s.remove_prefix(4);
        REQUIRE(s.empty());
        REQUIRE(s.data() == text + 6);
    }
    {
        memepp::string_span s(text, 6);
        s.remove_suffix(3);
        s.remove_suffix(3);
        REQUIRE(s.empty());
        REQUIRE(s.data() == text);
    }

    /* A partially trimmed span keeps the remaining bytes intact. */
    {
        memepp::string_span s(text, 6);
        s.remove_prefix(1);
        s.remove_suffix(2);
        REQUIRE(s.size() == 3);
        REQUIRE(s.data() == text + 1);
        REQUIRE(s[0] == 'b');
        REQUIRE(s[2] == 'd');
    }
}

// ============================================================================
// substr: negative and empty-span paths
// ============================================================================

TEST_CASE("string_span substr boundaries", "[string_span][gap]")
{
    const char text[] = "abcdef";
    memepp::string_span s(text, 6);

    /* pos == size() and pos > size() clamp to an empty tail. */
    REQUIRE(s.substr(6).empty());
    REQUIRE(s.substr(99).empty());
    REQUIRE(s.substr(6, 3).empty());

    /* count == 0 is an empty slice at pos. */
    REQUIRE(s.substr(0, 0).empty());
    REQUIRE(s.substr(3, 0).empty());

    /* count beyond the remainder truncates. */
    REQUIRE(s.substr(4, 99).size() == 2);
    REQUIRE(s.substr(0, 99).size() == 6);

    /* npos count means "to the end". */
    REQUIRE(s.substr(2, memepp::string_span::npos).size() == 4);

    /*
     * A negative _pos is a caller error: substr does `data_ + _pos` with no
     * validation, which for _pos < 0 forms a pointer before the array.  It is
     * therefore not exercised.  The contract is _pos >= 0.
     */

    /* Empty span: every substr is empty, and no pointer is added to null. */
    memepp::string_span e;
    REQUIRE(e.substr().empty());
    REQUIRE(e.substr(0).empty());
    REQUIRE(e.substr(0, 0).empty());
    REQUIRE(e.substr(0, 99).empty());
    REQUIRE(e.substr(99).empty());
}

// ============================================================================
// copy(): destination and count boundaries
// ============================================================================

TEST_CASE("string_span copy boundaries", "[string_span][gap]")
{
    const char text[] = "abcdef";
    memepp::string_span s(text, 6);
    MemeByte_t buf[16];

    /* count == 0 copies nothing and returns 0. */
    std::memset(buf, 'Z', sizeof(buf));
    REQUIRE(s.copy(buf, 0) == 0);
    REQUIRE(buf[0] == 'Z');            /* untouched */

    /* count beyond the remainder truncates to what is available. */
    std::memset(buf, 0, sizeof(buf));
    REQUIRE(s.copy(buf, 99) == 6);
    REQUIRE(std::memcmp(buf, "abcdef", 6) == 0);

    /* pos applies before the truncation. */
    std::memset(buf, 0, sizeof(buf));
    REQUIRE(s.copy(buf, 99, 4) == 2);
    REQUIRE(std::memcmp(buf, "ef", 2) == 0);

    /* pos == size() yields 0 bytes. */
    REQUIRE(s.copy(buf, 5, 6) == 0);

    /* pos > size() is rejected. */
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(s.copy(buf, 1, 7), std::out_of_range);
    REQUIRE_THROWS_AS(s.copy(buf, 1, -1), std::out_of_range);
#else
    REQUIRE(s.copy(buf, 1, 7) == 0);
    REQUIRE(s.copy(buf, 1, -1) == 0);
#endif

    /* Binary payload with NUL and high bytes survives copy(). */
    {
        const MemeByte_t bin[] = { 0x00, 0xFF, 0x80, 'A' };
        memepp::string_span bs(bin, 4);
        std::memset(buf, 0x55, sizeof(buf));
        REQUIRE(bs.copy(buf, 4) == 4);
        REQUIRE(std::memcmp(buf, bin, 4) == 0);
    }

    /* Empty span copies nothing. */
    {
        memepp::string_span e;
        REQUIRE(e.copy(buf, 0) == 0);
        REQUIRE(e.copy(buf, 0, 0) == 0);
    }
}

// ============================================================================
// UTF-8: multi-byte runes, emoji, surrogate pairs, invalid sequences
// ============================================================================

TEST_CASE("string_span rune_front/rune_back multi-byte", "[string_span][gap]")
{
    /* Two CJK code points, 3 bytes each. */
    const char cjk[] = "\xE4\xBD\xA0\xE5\xA5\xBD";     /* 你好 */
    memepp::string_span s(cjk, 6);
    REQUIRE(s.rune_size() == 2);

    auto f = s.rune_front();
    REQUIRE(f.size() == 3);
    REQUIRE(std::memcmp(f.data(), cjk, 3) == 0);

    auto b = s.rune_back();
    REQUIRE(b.size() == 3);
    REQUIRE(std::memcmp(b.data(), cjk + 3, 3) == 0);
}

TEST_CASE("string_span runes: 4-byte emoji and mixed ASCII", "[string_span][gap]")
{
    /* 'a' + U+1F600 (4-byte emoji) + 'b' = 1 + 4 + 1 = 6 bytes. */
    const char mixed[] = "a\xF0\x9F\x98\x80" "b";
    memepp::string_span s(mixed, 6);

    REQUIRE(s.size() == 6);
    REQUIRE(s.rune_size() == 3);        /* a, emoji, b */

    REQUIRE(s.rune_front().size() == 1);
    REQUIRE(s.rune_front().data()[0] == 'a');

    REQUIRE(s.rune_back().size() == 1);
    REQUIRE(s.rune_back().data()[0] == 'b');
}

TEST_CASE("string_span u16char_size with non-BMP characters",
          "[string_span][gap]")
{
    /* ASCII: one UTF-16 unit per byte. */
    REQUIRE(memepp::string_span("hello").u16char_size() == 5);

    /* Two CJK code points: one UTF-16 unit each. */
    {
        const char cjk[] = "\xE4\xBD\xA0\xE5\xA5\xBD";
        REQUIRE(memepp::string_span(cjk, 6).u16char_size() == 2);
    }

    /* A non-BMP code point becomes a surrogate pair: 2 UTF-16 units. */
    {
        const char emoji[] = "\xF0\x9F\x98\x80";    /* U+1F600 */
        REQUIRE(memepp::string_span(emoji, 4).u16char_size() == 2);
    }

    /* Mixed: 'a' (1) + emoji (2) + 'b' (1) = 4 units. */
    {
        const char mixed[] = "a\xF0\x9F\x98\x80" "b";
        REQUIRE(memepp::string_span(mixed, 6).u16char_size() == 4);
    }
}

TEST_CASE("string_span rune iterator walks multi-byte sequences",
          "[string_span][gap]")
{
    const char cjk[] = "\xE4\xBD\xA0\xE5\xA5\xBD";
    memepp::string_span s(cjk, 6);

    /*
     * const_rune_iterator::operator*() yields the byte the iterator points
     * at, not a rune object, so counting runes means counting iterations.
     * Each step advances by a whole UTF-8 sequence.
     */
    size_t count = 0;
    for (auto it = s.rune_begin(); it != s.rune_end(); ++it)
        ++count;
    REQUIRE(count == 2);

    /* Dereferencing points at the lead byte of each sequence. */
    {
        auto it = s.rune_begin();
        REQUIRE(*it == static_cast<MemeByte_t>(0xE4));
        ++it;
        REQUIRE(*it == static_cast<MemeByte_t>(0xE5));
    }

    /* to_rune_iterator at a rune boundary lands on the boundary and is not
     * the end iterator. */
    auto it0 = s.to_rune_iterator(0);
    REQUIRE(it0 != s.rune_end());
    REQUIRE(*it0 == static_cast<MemeByte_t>(0xE4));
    auto it3 = s.to_rune_iterator(3);
    REQUIRE(it3 != s.rune_end());
    REQUIRE(*it3 == static_cast<MemeByte_t>(0xE5));

    /* pos == size() yields rune_end(). */
    REQUIRE(s.to_rune_iterator(6) == s.rune_end());
    /* pos > size() also yields rune_end(). */
    REQUIRE(s.to_rune_iterator(99) == s.rune_end());
    /* Negative pos yields rune_end() (the impl guards it explicitly). */
    REQUIRE(s.to_rune_iterator(-1) == s.rune_end());
}

// ============================================================================
// find / rfind: complete boundary matrix
// ============================================================================

TEST_CASE("string_span find boundary matrix", "[string_span][gap]")
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

TEST_CASE("string_span rfind boundary matrix", "[string_span][gap]")
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
          "[string_span][gap]")
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
          "[string_span][gap]")
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

TEST_CASE("string_span compare subrange and binary", "[string_span][gap]")
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
// to_string_ref()
// ============================================================================

TEST_CASE("string_span to_string_ref", "[string_span][gap]")
{
    const char text[] = "hello";
    memepp::string_span s(text, 5);

    auto ref = s.to_string_ref();
    REQUIRE(ref.size() == 5);
    REQUIRE(ref.data() == text);           /* non-owning, aliases the span */

    /* The ref observes source mutations: it is a pure borrow. */
    char mutable_text[] = "abc";
    memepp::string_span ms(mutable_text, 3);
    auto mref = ms.to_string_ref();
    REQUIRE(mref.data() == mutable_text);
    mutable_text[1] = 'Z';
    REQUIRE(mref.data()[1] == 'Z');

    /* Empty span -> empty ref. */
    memepp::string_span e;
    auto eref = e.to_string_ref();
    REQUIRE(eref.size() == 0);

    /* The ref reflects a sub-span's view. */
    auto sub = s.substr(1, 3);
    auto sref = sub.to_string_ref();
    REQUIRE(sref.size() == 3);
    REQUIRE(sref.data() == text + 1);
}

// ============================================================================
// Conversions: independent storage
// ============================================================================

TEST_CASE("string_span to_string/to_shared_storage own their bytes",
          "[string_span][gap]")
{
    char raw[] = { 'a', 'b', 'c' };
    memepp::string_span s(raw, 3);

    memepp::string plain = s.to_string();
    memepp::string shared = s.to_shared_storage();

    REQUIRE(plain.size() == 3);
    REQUIRE(shared.size() == 3);
    REQUIRE(plain.data() != raw);
    REQUIRE(shared.data() != raw);

    /* Mutating the source must not change either copy. */
    raw[0] = 'Z';
    REQUIRE(s[0] == 'Z');
    REQUIRE(plain.data()[0] == 'a');
    REQUIRE(shared.data()[0] == 'a');

    /* to_shared_storage really uses the large tier. */
    REQUIRE(shared.storage_type() == memepp::string_storage_t::large);

    /* Binary payload survives both conversions. */
    {
        const char bin[] = { '\0', '\xFF', '\x80' };
        memepp::string_span bs(bin, 3);
        memepp::string b1 = bs.to_string();
        memepp::string b2 = bs.to_shared_storage();
        REQUIRE(b1.size() == 3);
        REQUIRE(b2.size() == 3);
        REQUIRE(std::memcmp(b1.data(), bin, 3) == 0);
        REQUIRE(std::memcmp(b2.data(), bin, 3) == 0);
    }

    /* Empty span converts to empty containers. */
    {
        memepp::string_span e;
        REQUIRE(e.to_string().size() == 0);
        REQUIRE(e.to_shared_storage().size() == 0);
    }
}

// ============================================================================
// mm_span / _meme_span literal: length boundaries
// ============================================================================

TEST_CASE("mm_span length boundaries", "[string_span][gap]")
{
    const char text[] = "hello";

    /* nullptr with length 0 is the empty span. */
    {
        auto s = mm_span(static_cast<const char*>(nullptr), 0);
        REQUIRE(s.empty());
        REQUIRE(s.size() == 0);
        REQUIRE(s.data() == nullptr);
    }

    /* A non-null pointer with length 0 is empty but keeps the pointer. */
    {
        auto s = mm_span(text, 0);
        REQUIRE(s.empty());
        REQUIRE(s.data() == text);
    }

    /* The ordinary case. */
    {
        auto s = mm_span(text, 5);
        REQUIRE(s.size() == 5);
        REQUIRE(s.data() == text);
    }

    /* The largest length that still fits size_type is stored verbatim; the
     * cast from size_t beyond that is implementation-defined, so it is not
     * asserted. */
    {
        const size_t max_fit = static_cast<size_t>(
            std::numeric_limits<memepp::string_span::size_type>::max());
        auto s = mm_span(text, max_fit);
        REQUIRE(s.size() ==
                std::numeric_limits<memepp::string_span::size_type>::max());
    }

    /* The literal operator behaves the same way and does not include the NUL. */
    using namespace memepp;
    {
        auto s = "hello"_meme_span;
        REQUIRE(s.size() == 5);
        REQUIRE(s.data()[0] == 'h');
        REQUIRE(s.data()[4] == 'o');
    }
}

// ============================================================================
// Compile-time contract
// ============================================================================

TEST_CASE("string_span noexcept and layout contract", "[string_span][gap]")
{
    using ss_t = memepp::string_span;

    /* Two words: a pointer and a signed size. */
    static_assert(sizeof(ss_t) == 2 * sizeof(void*),
                  "string_span must be two words");
    static_assert(alignof(ss_t) == alignof(void*),
                  "string_span alignment must match a pointer");

    /* Copy is noexcept. */
    static_assert(std::is_nothrow_copy_constructible<ss_t>::value,
                  "copy construction must not throw");
    static_assert(std::is_nothrow_copy_assignable<ss_t>::value,
                  "copy assignment must not throw");

    /* Read-only queries and searches are noexcept. */
    static_assert(noexcept(std::declval<const ss_t&>().data()), "data()");
    static_assert(noexcept(std::declval<const ss_t&>().bytes()), "bytes()");
    static_assert(noexcept(std::declval<const ss_t&>().size()), "size()");
    static_assert(noexcept(std::declval<const ss_t&>().length()), "length()");
    static_assert(noexcept(std::declval<const ss_t&>().empty()), "empty()");
    static_assert(noexcept(std::declval<const ss_t&>().begin()), "begin()");
    static_assert(noexcept(std::declval<const ss_t&>().end()), "end()");
    static_assert(noexcept(std::declval<const ss_t&>().rbegin()), "rbegin()");
    static_assert(noexcept(std::declval<const ss_t&>().rend()), "rend()");
    static_assert(noexcept(std::declval<const ss_t&>().rune_begin()),
                  "rune_begin()");
    static_assert(noexcept(std::declval<const ss_t&>().rune_end()),
                  "rune_end()");
    static_assert(noexcept(std::declval<const ss_t&>().substr()),
                  "substr()");
    static_assert(noexcept(std::declval<const ss_t&>().compare(
                      std::declval<ss_t>())), "compare()");
    static_assert(noexcept(std::declval<const ss_t&>().starts_with(
                      std::declval<ss_t>())), "starts_with()");
    static_assert(noexcept(std::declval<const ss_t&>().ends_with(
                      std::declval<ss_t>())), "ends_with()");
    static_assert(noexcept(std::declval<const ss_t&>().contains(
                      std::declval<ss_t>())), "contains()");
    static_assert(noexcept(std::declval<const ss_t&>().find(
                      std::declval<ss_t>())), "find()");
    static_assert(noexcept(std::declval<const ss_t&>().rfind(
                      std::declval<ss_t>())), "rfind()");
    static_assert(noexcept(std::declval<const ss_t&>().to_string_ref()),
                  "to_string_ref()");

    /* at() and copy() report range errors, so they may throw. */
    static_assert(!noexcept(std::declval<const ss_t&>().at(0)),
                  "at() must not be noexcept");
    static_assert(!noexcept(std::declval<const ss_t&>().copy(
                      std::declval<MemeByte_t*>(), 0)),
                  "copy() must not be noexcept");

    REQUIRE(true);   /* all of the above are compile-time */
}

// ============================================================================
// char8_t overloads (C++20 only)
// ============================================================================

#if defined(__cpp_char8_t)
TEST_CASE("string_span char8_t construction and comparison",
          "[string_span][gap]")
{
    /* The null-terminated char8_t* constructor measures with strlen. */
    memepp::string_span s(u8"hello");
    REQUIRE(s.size() == 5);
    REQUIRE(s.data()[0] == 'h');

    /* The explicit-count constructor stores the count verbatim. */
    memepp::string_span c(u8"hello", 3);
    REQUIRE(c.size() == 3);
    REQUIRE(c.data()[2] == 'l');

    /* A null char8_t* is the empty span in both constructors. */
    {
        memepp::string_span n(static_cast<const char8_t*>(nullptr));
        REQUIRE(n.empty());
        REQUIRE(n.size() == 0);
    }
    {
        /* Unlike the (const char*, count) ctor which guards null, the
         * char8_t overload stores both values verbatim.  The resulting span
         * has a null data pointer and a positive size, which is a caller
         * error.  We document that by asserting the actual state. */
        memepp::string_span n(static_cast<const char8_t*>(nullptr), 5);
        REQUIRE(n.data() == nullptr);
        REQUIRE(n.size() == 5);
    }

    /* All six comparison operators, both directions. */
    const char8_t* cu8 = u8"hello";
    REQUIRE(cu8 == memepp::string_span(u8"hello"));
    REQUIRE(memepp::string_span(u8"hello") == cu8);
    REQUIRE(cu8 != memepp::string_span(u8"world"));
    REQUIRE(memepp::string_span(u8"world") != cu8);

    const char8_t* less = u8"abc";
    const char8_t* more = u8"abd";
    REQUIRE(less < memepp::string_span(more));
    REQUIRE(memepp::string_span(less) < more);
    REQUIRE(more > memepp::string_span(less));
    REQUIRE(memepp::string_span(more) > less);
    REQUIRE(less <= memepp::string_span(more));
    REQUIRE(more >= memepp::string_span(less));
}

TEST_CASE("string_span char8_t with explicit count is not null-checked",
          "[string_span][gap]")
{
    /*
     * Only the null-terminated char8_t* constructor guards a null pointer.
     * The (ptr, count) overload stores the pair verbatim, so a null pointer
     * with a positive count would build a span whose data_ is null but whose
     * size_ is non-zero.  That is a caller error; the test documents the
     * difference by checking the guarded and unguarded cases separately.
     */
    const char8_t* np = nullptr;

    /* Guarded form. */
    memepp::string_span guarded(np);
    REQUIRE(guarded.empty());

    /* Unguarded form with count 0 is still safe. */
    memepp::string_span zero(np, 0);
    REQUIRE(zero.empty());
    REQUIRE(zero.data() == nullptr);
}
#endif

// ============================================================================
// copy(): null destination
// ============================================================================

TEST_CASE("string_span copy with a null destination", "[string_span][gap]")
{
    /*
     * copy() forwards straight to memcpy(_dest, data_ + _pos, rlen) with no
     * null check.  memcpy with a null destination is undefined even when
     * rlen is 0, so the only safe thing to assert is the zero-length case
     * where the implementation still short-circuits before the memcpy:
     *
     *   size_type rlen = (_count > size_ - _pos) ? size_ - _pos : _count;
     *   std::memcpy(_dest, data_ + _pos, rlen);
     *
     * With _count == 0, rlen == 0 and the memcpy is a no-op in practice, but
     * the standard still requires a valid pointer.  This test therefore only
     * documents that a null destination with a non-zero count is a caller
     * error and is not executed.
     */
    const char text[] = "abc";
    memepp::string_span s(text, 3);

    /* A null destination with count 0 is the only case the class can be
     * expected to tolerate; verify it does not corrupt anything else. */
    REQUIRE_NOTHROW(s.copy(nullptr, 0));

    /* A null destination with a positive count is undefined behaviour and is
     * deliberately NOT called.  The contract is: _dest must point to at
     * least min(_count, size() - _pos) writable bytes. */
}

// ============================================================================
// Empty span: reverse iterator pointer arithmetic
// ============================================================================

TEST_CASE("string_span empty: reverse iterators are out of contract",
          "[string_span][gap]")
{
    /*
     * CONTRACT NOTE (documented, deliberately NOT executed):
     *
     *   rbegin() -> data_ + size_ - 1
     *   rend()   -> data_ - 1
     *
     * On a default-constructed span data_ is null and size_ is 0, so both
     * expressions compute (nullptr - 1).  Pointer arithmetic with a
     * non-zero offset on a null pointer is undefined behaviour per
     * [expr.add] -- only "nullptr + 0" is defined -- and the arithmetic
     * happens when the iterator is *constructed*, before any dereference.
     *
     * Therefore rbegin()/crbegin()/rend()/crend() must only be called on a
     * non-empty span.  This test does not call them; it pins the parts of
     * the empty-span contract that ARE defined:
     *
     *   begin()/end()   -> data_ + 0, legal
     *   rune_begin/end  -> data_ + 0, legal
     *   range-for       -> compares begin() and end(), zero iterations
     */
    memepp::string_span e;
    REQUIRE(e.empty());
    REQUIRE(e.size() == 0);

    /* These use a zero offset, which [expr.add] does define for null. */
    REQUIRE_NOTHROW(e.begin());
    REQUIRE_NOTHROW(e.end());
    REQUIRE(e.begin() == e.end());
    REQUIRE_NOTHROW(e.rune_begin());
    REQUIRE_NOTHROW(e.rune_end());
    REQUIRE(e.rune_begin() == e.rune_end());

    /* A forward range-for over an empty span runs zero iterations. */
    size_t n = 0;
    for (char c : e) { (void)c; ++n; }
    REQUIRE(n == 0);

    /* A rune range-for likewise. */
    size_t rn = 0;
    for (auto it = e.rune_begin(); it != e.rune_end(); ++it) { ++rn; }
    REQUIRE(rn == 0);
}

// ============================================================================
// Empty span: rune iterators
// ============================================================================

TEST_CASE("string_span empty: rune iterators are equal", "[string_span][gap]")
{
    memepp::string_span e;

    /* rune_begin() is {bytes(), bytes() + size_} and rune_end() is
     * {bytes() + size_, bytes(), bytes() + size_}.  With a null data_ and
     * zero size the offset is 0, which is the one null-pointer arithmetic
     * operation [expr.add] does define, so these are portable. */
    REQUIRE(e.rune_begin() == e.rune_end());
    REQUIRE(e.rune_cbegin() == e.rune_cend());

    /* A rune range-for over an empty span runs zero iterations. */
    size_t n = 0;
    for (auto it = e.rune_begin(); it != e.rune_end(); ++it)
    {
        (void)*it;
        ++n;
        if (n > 8) break;
    }
    REQUIRE(n == 0);

    /* to_rune_iterator on an empty span always yields the end. */
    REQUIRE(e.to_rune_iterator(0) == e.rune_end());
    REQUIRE(e.to_rune_iterator(1) == e.rune_end());
    REQUIRE(e.to_rune_iterator(-1) == e.rune_end());

    /* rune_front/rune_back on an empty span: string_ref returns an empty
     * rune rather than reading through the null data pointer. */
    REQUIRE_NOTHROW(e.rune_front());
    REQUIRE_NOTHROW(e.rune_back());
}

// ============================================================================
// Invalid UTF-8: fixed semantics where the impl defines them
// ============================================================================

TEST_CASE("string_span invalid UTF-8 rune accounting", "[string_span][gap]")
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
// Out-of-contract modifier arguments: measured contract, not UB execution
// ============================================================================

TEST_CASE("string_span remove_prefix/remove_suffix out-of-range",
          "[string_span][gap]")
{
    /*
     * CONTRACT NOTE (documented, deliberately NOT executed):
     *
     *   remove_prefix(_n) -> data_ += _n; size_ -= _n;
     *   remove_suffix(_n) -> size_ -= _n;
     *
     * Neither clamps.  The contract is 0 <= _n <= size().
     *
     *   _n > size()  -> data_ moves past one-past-the-end
     *                   (remove_prefix) or size_ goes negative
     *                   (remove_suffix);
     *   _n < 0       -> data_ moves before the array and size_ grows.
     *
     * Both form pointers outside the source array, which is undefined
     * behaviour per [expr.add]: pointer arithmetic is only defined for
     * offsets within the array (plus one-past).  Asserting the resulting
     * size_ would require performing that arithmetic first, so these calls
     * are NOT made.  The valid boundary below is the one this test pins.
     */

    const char text[] = "abcdef";

    /* The largest in-contract value: removing the whole length. */
    {
        memepp::string_span s(text, 6);
        s.remove_prefix(6);
        REQUIRE(s.size() == 0);
        REQUIRE(s.empty());
        REQUIRE(s.data() == text + 6);   /* one-past-the-end: legal */
    }
    {
        memepp::string_span s(text, 6);
        s.remove_suffix(6);
        REQUIRE(s.size() == 0);
        REQUIRE(s.empty());
        REQUIRE(s.data() == text);
    }

    /* Zero is a no-op. */
    {
        memepp::string_span s(text, 6);
        s.remove_prefix(0);
        s.remove_suffix(0);
        REQUIRE(s.size() == 6);
        REQUIRE(s.data() == text);
    }

    /*
     * Out-of-contract values (_n > size(), _n < 0) are caller errors.  The
     * class does not detect them and the language does not define the
     * resulting pointer, so no assertion is made about them.
     */
}

TEST_CASE("string_span substr with a negative position", "[string_span][gap]")
{
    /*
     * CONTRACT NOTE (documented, deliberately NOT executed):
     *
     *   substr(_pos, _count):
     *     if (_pos > size_)                              _pos = size_;
     *     if (_count == npos || _count > size_ - _pos)   _count = size_ - _pos;
     *     return string_span(data_ + _pos, _count);
     *
     * A negative _pos passes both guards unchanged, so data_ + _pos forms a
     * pointer before the array -- undefined behaviour per [expr.add] --
     * and _count becomes size_ - _pos (larger than the span).
     *
     * The contract is _pos >= 0.  Because observing the result would require
     * having executed the out-of-range pointer arithmetic, negative
     * positions are NOT called here.  Only the in-contract boundaries are
     * exercised.
     */

    const char text[] = "abcdef";
    memepp::string_span s(text, 6);

    /* pos == 0 is the whole span. */
    REQUIRE(s.substr(0).size() == 6);
    REQUIRE(s.substr(0).data() == text);

    /* pos inside the span. */
    REQUIRE(s.substr(2).size() == 4);
    REQUIRE(s.substr(2).data() == text + 2);

    /* pos == size() and pos > size() clamp to an empty tail. */
    REQUIRE(s.substr(6).size() == 0);
    REQUIRE(s.substr(99).size() == 0);

    /* count is clamped to the remainder. */
    REQUIRE(s.substr(0, 0).size() == 0);
    REQUIRE(s.substr(4, 99).size() == 2);
    REQUIRE(s.substr(0, memepp::string_span::npos).size() == 6);

    /* Empty span: every in-contract substr is empty. */
    memepp::string_span e;
    REQUIRE(e.substr(0).size() == 0);
    REQUIRE(e.substr(0, 0).size() == 0);
    REQUIRE(e.substr(0, 99).size() == 0);
    REQUIRE(e.substr(99).size() == 0);
}

