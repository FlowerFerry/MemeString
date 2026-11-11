#include <catch2/catch.hpp>

#include <memepp/string_span.hpp>
#include <memepp/string.hpp>

#include <type_traits>

// to_string_ref()
// ============================================================================

TEST_CASE("string_span to_string_ref", "[string_span][contract]")
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
          "[string_span][contract]")
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

TEST_CASE("mm_span length boundaries", "[string_span][contract]")
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

TEST_CASE("string_span noexcept and layout contract", "[string_span][contract]")
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
          "[string_span][contract]")
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
          "[string_span][contract]")
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

// Out-of-contract modifier arguments: measured contract, not UB execution
// ============================================================================

TEST_CASE("string_span remove_prefix/remove_suffix out-of-range",
          "[string_span][contract]")
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

TEST_CASE("string_span substr with a negative position", "[string_span][contract]")
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

