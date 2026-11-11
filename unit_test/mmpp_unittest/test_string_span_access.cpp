#include <catch2/catch.hpp>

#include <memepp/string_span.hpp>
#include <memepp/string.hpp>

// Empty span: element access, iterators, rune iterators
// ============================================================================

TEST_CASE("string_span empty: iterators and rune iterators",
          "[string_span][access]")
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
          "[string_span][access]")
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
          "[string_span][access]")
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

TEST_CASE("string_span substr boundaries", "[string_span][access]")
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

TEST_CASE("string_span copy boundaries", "[string_span][access]")
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

TEST_CASE("string_span rune_front/rune_back multi-byte", "[string_span][access]")
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

TEST_CASE("string_span runes: 4-byte emoji and mixed ASCII", "[string_span][access]")
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
          "[string_span][access]")
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
          "[string_span][access]")
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

// copy(): null destination
// ============================================================================

TEST_CASE("string_span copy with a null destination", "[string_span][access]")
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
          "[string_span][access]")
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

TEST_CASE("string_span empty: rune iterators are equal", "[string_span][access]")
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
