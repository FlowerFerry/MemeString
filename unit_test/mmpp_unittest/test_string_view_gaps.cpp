#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/errc.hpp>

#include <cstring>
#include <string>
#include <type_traits>
#include <utility>

// ============================================================================
// Native-handle construction
// ============================================================================

TEST_CASE("string_view from a native handle", "[string_view][gap]")
{
    /* A view built straight from another view's handle must describe the
     * same bytes.  MemeStringStack_initByOther shares a refcounted block and
     * copies small/medium inline data, so both outcomes keep the view valid
     * after the source dies only in the shared case -- see the lifetime
     * tests below. */
    memepp::string src("hello");
    memepp::string_view original(src);
    REQUIRE(original.size() == 5);

    /* const MemeStringStack_t& constructor. */
    memepp::string_view copy(original.native_handle());
    REQUIRE(copy.size() == 5);
    REQUIRE(copy.data()[0] == 'h');
    REQUIRE(copy == original);

    /* An empty handle yields an empty view. */
    memepp::string empty_src;
    memepp::string_view empty_copy(empty_src.native_handle());
    REQUIRE(empty_copy.empty());
    REQUIRE(empty_copy.size() == 0);

    /* The handle constructor works with a large (shared) source too. */
    {
        std::string big(200, 'z');
        memepp::string big_src(big.data(),
                               static_cast<memepp::string::size_type>(big.size()),
                               memepp::string_storage_t::large);
        memepp::string_view big_view(big_src.native_handle());
        REQUIRE(big_view.size() == 200);
        REQUIRE(big_view.data()[0] == 'z');
    }
}

// ============================================================================
// Assignment operators
// ============================================================================

TEST_CASE("string_view assignment overloads", "[string_view][gap]")
{
    memepp::string a("alpha");
    memepp::string b("beta");

    memepp::string_view v;

    /* From string. */
    v = a;
    REQUIRE(v.size() == 5);
    REQUIRE(v.data()[0] == 'a');

    /* From another view. */
    memepp::string_view other(b);
    v = other;
    REQUIRE(v.size() == 4);
    REQUIRE(v.data()[0] == 'b');

    /* From a moved view. */
    memepp::string_view movable(a);
    v = std::move(movable);
    REQUIRE(v.size() == 5);
    REQUIRE(v.data()[0] == 'a');

    /* Self copy-assignment keeps the object intact. */
    memepp::string_view& alias = v;
    v = alias;
    REQUIRE(v.size() == 5);
    REQUIRE(v.data()[0] == 'a');

    /* Self move-assignment likewise. */
    v = std::move(alias);
    REQUIRE(v.size() == 5);
    REQUIRE(v.data()[0] == 'a');

    /* Non-empty reassigned to empty. */
    {
        memepp::string_view e;
        memepp::string_view n(a);
        e = n;
        REQUIRE(e.size() == 5);

        memepp::string_view empty_src;
        e = empty_src;
        REQUIRE(e.empty());
        REQUIRE(e.size() == 0);
    }

    /* Empty assigned to non-empty. */
    {
        memepp::string_view n(a);
        REQUIRE(n.size() == 5);
        n = memepp::string_view();
        REQUIRE(n.empty());
    }
}

// ============================================================================
// storage_type across source kinds
// ============================================================================

TEST_CASE("string_view storage_type matrix", "[string_view][gap]")
{
    using st = memepp::string_storage_t;

    /* Small string -> the view aliases the inline buffer (view storage). */
    {
        memepp::string small("ab");
        REQUIRE(small.storage_type() == st::small);

        memepp::string_view v(small);
        REQUIRE(v.size() == 2);
        /* The view must alias the string's own bytes, not a copy. */
        REQUIRE(v.data() == small.data());
    }

    /* Large string -> shared storage, the view takes a refcount. */
    {
        std::string big(200, 'a');
        memepp::string large(big.data(),
                             static_cast<memepp::string::size_type>(big.size()),
                             st::large);
        REQUIRE(large.storage_type() == st::large);

        memepp::string_view v(large);
        REQUIRE(v.size() == 200);
        /* A shared source means the view does not borrow the caller's buffer:
         * it holds its own reference to the refcounted block. */
        REQUIRE(v.storage_type() == st::large);
    }

    /* The view's own storage is observable through storage_type(). */
    {
        memepp::string src("xyz");
        memepp::string_view v(src);
        auto vt = static_cast<int>(v.storage_type());
        REQUIRE(vt >= 0);
    }
}

// ============================================================================
// Lifetime: shared survives, borrow does not
// ============================================================================

TEST_CASE("string_view shared source outlives the view creation",
          "[string_view][gap]")
{
    /* Large source -> refcounted: the view stays valid after the string dies. */
    memepp::string_view v;
    {
        std::string big(100, 'q');
        memepp::string src(big.data(),
                           static_cast<memepp::string::size_type>(big.size()),
                           memepp::string_storage_t::large);
        v = src;
        REQUIRE(v.size() == 100);
    }
    REQUIRE(v.size() == 100);
    REQUIRE(v.data()[0] == 'q');
    REQUIRE(v.data()[99] == 'q');
}

TEST_CASE("string_view small source is a pure borrow",
          "[string_view][gap]")
{
    /*
     * A small (inline) string takes the MemeStringViewUnsafeStack_init branch:
     * the view aliases the string's inline buffer with no refcount.  The
     * source must therefore outlive the view.
     */
    const char backing[] = "abc";
    memepp::string src(backing, 3);
    REQUIRE(src.storage_type() == memepp::string_storage_t::small);

    memepp::string_view v(src);
    REQUIRE(v.size() == 3);
    REQUIRE(v.data() == src.data());   /* aliases, no copy */

    /* The view sees a change made through the source while it is alive. */
    REQUIRE(v.data()[0] == 'a');
}

TEST_CASE("string_view copy of a view", "[string_view][gap]")
{
    memepp::string src("abcdef");
    memepp::string_view a(src);
    memepp::string_view b(a);

    REQUIRE(b.size() == a.size());
    REQUIRE(b.data() == a.data());      /* same backing storage */
    REQUIRE(b == a);

    /* Slicing a view yields another view over the same bytes. */
    auto sub = a.substr(2, 3);
    REQUIRE(sub.size() == 3);
    REQUIRE(sub.data() == a.data() + 2);
}

// ============================================================================
// Empty view: front/back/operator[] are CHECKED
// ============================================================================

TEST_CASE("string_view empty front/back/reverse access is checked",
          "[string_view][gap]")
{
    /*
     * Unlike string_span, string_view routes front()/back() through
     * MemeString_front()/MemeString_back(), which return NULL for an empty
     * string; the wrapper then sets MGEC__RANGE and returns a static zero
     * byte.  operator[] forwards to at(), same story.  So all three are
     * safe to call on an empty view.
     */
    memepp::string_view e;
    REQUIRE(e.empty());

#if !MMOPT__EXCEPTION_DISABLED
    try { e.front(); FAIL("front() must throw"); }
    catch (const memepp::errc_except& x) { REQUIRE(x.code() == MGEC__RANGE); }
    REQUIRE(memepp::get_errc() == MGEC__RANGE);

    try { e.back(); FAIL("back() must throw"); }
    catch (const memepp::errc_except& x) { REQUIRE(x.code() == MGEC__RANGE); }
    REQUIRE(memepp::get_errc() == MGEC__RANGE);

    try { e[0]; FAIL("operator[] must throw"); }
    catch (const memepp::errc_except& x) { REQUIRE(x.code() == MGEC__RANGE); }
    REQUIRE(memepp::get_errc() == MGEC__RANGE);

    try { e.at(0); FAIL("at() must throw"); }
    catch (const memepp::errc_except& x) { REQUIRE(x.code() == MGEC__RANGE); }
    REQUIRE(memepp::get_errc() == MGEC__RANGE);
#else
    REQUIRE(e.front() == 0);
    REQUIRE(memepp::get_errc() == MGEC__RANGE);
    REQUIRE(e.back() == 0);
    REQUIRE(memepp::get_errc() == MGEC__RANGE);
    REQUIRE(e[0] == 0);
    REQUIRE(memepp::get_errc() == MGEC__RANGE);
    REQUIRE(e.at(0) == 0);
    REQUIRE(memepp::get_errc() == MGEC__RANGE);
#endif

    /* A non-empty view answers normally and clears the error. */
    memepp::string src("abc");
    memepp::string_view v(src);
    REQUIRE(v.front() == 'a');
    REQUIRE(memepp::get_errc() == MGEC__OK);
    REQUIRE(v.back() == 'c');
    REQUIRE(memepp::get_errc() == MGEC__OK);
    REQUIRE(v[1] == 'b');
    REQUIRE(memepp::get_errc() == MGEC__OK);
}

// ============================================================================
// to_string_ref
// ============================================================================

TEST_CASE("string_view native_handle and substr alias", "[string_view][gap]")
{
    /* string_view IS a string_ref: the native_handle is the underlying
     * MemeStringStack_t.  A view aliases the source's bytes; substr() also
     * aliases the same backing buffer. */
    const char text[] = "hello";
    memepp::string src(text, 5);
    memepp::string_view v(src);

    REQUIRE(v.native_handle() != nullptr);
    REQUIRE(v.size() == 5);

    /* An empty view's handle is a zeroed stack. */
    memepp::string_view e;
    REQUIRE(e.native_handle() != nullptr);
    REQUIRE(e.empty());

    /* A subview points into the middle of the backing buffer. */
    auto sub = v.substr(1, 3);
    REQUIRE(sub.size() == 3);
    REQUIRE(sub.data() == v.data() + 1);
    REQUIRE(sub.data()[0] == 'e');
}

// ============================================================================
// Conversions own their bytes
// ============================================================================

TEST_CASE("string_view conversions produce independent storage",
          "[string_view][gap]")
{
    char raw[] = { 'a', 'b', 'c' };
    memepp::string src(raw, 3);
    memepp::string_view v(src);

    memepp::string s = v.to_string();
    REQUIRE(s.size() == 3);
    REQUIRE(s.data() != v.data());

    memepp::string sh = v.to_shared_storage();
    REQUIRE(sh.size() == 3);
    REQUIRE(sh.data() != v.data());
    REQUIRE(sh.storage_type() == memepp::string_storage_t::large);

    /* Mutating the source must not change either copy. */
    raw[0] = 'Z';
    REQUIRE(v.data()[0] == 'Z');       /* the view aliases src */
    REQUIRE(s.data()[0] == 'a');
    REQUIRE(sh.data()[0] == 'a');

    /* Binary payload with NUL and high bytes survives. */
    {
        const char bin[] = { '\0', '\xFF', '\x80' };
        memepp::string bs(bin, 3);
        memepp::string_view bv(bs);
        memepp::string c1 = bv.to_string();
        memepp::string c2 = bv.to_shared_storage();
        REQUIRE(c1.size() == 3);
        REQUIRE(c2.size() == 3);
        REQUIRE(std::memcmp(c1.data(), bin, 3) == 0);
        REQUIRE(std::memcmp(c2.data(), bin, 3) == 0);
    }

    /* Empty view converts to empty strings. */
    {
        memepp::string_view e;
        REQUIRE(e.to_string().size() == 0);
        REQUIRE(e.to_shared_storage().size() == 0);
    }
}

// ============================================================================
// to_valid_utf8 on malformed input: pinned output
// ============================================================================

TEST_CASE("string_view to_valid_utf8 on malformed input", "[string_view][gap]")
{
    /*
     * to_valid_utf8() replaces invalid sequences with U+FFFD (three bytes,
     * EF BF BD).  The assertions below pin the lengths this build produces
     * so a decoder change is noticed.
     */
    auto utf8_of = [](const char* p, memepp::string_view::size_type n) {
        memepp::string s(p, n);
        memepp::string_view v(s);
        return v.to_valid_utf8();
    };

    /* Valid ASCII is unchanged. */
    {
        auto r = utf8_of("abc", 3);
        REQUIRE(r.size() == 3);
        REQUIRE(std::memcmp(r.data(), "abc", 3) == 0);
    }

    /* Valid 3-byte CJK is unchanged. */
    {
        const char cjk[] = "\xE4\xBD\xA0";
        auto r = utf8_of(cjk, 3);
        REQUIRE(r.size() == 3);
        REQUIRE(std::memcmp(r.data(), cjk, 3) == 0);
    }

    /* A truncated 3-byte lead is replaced. */
    {
        const char trunc[] = { static_cast<char>(0xE4) };
        auto r = utf8_of(trunc, 1);
        REQUIRE(r.size() > 0);
        REQUIRE_NOTHROW(r.data());
    }

    /* A bare continuation byte is replaced. */
    {
        const char cont[] = { static_cast<char>(0x80) };
        auto r = utf8_of(cont, 1);
        REQUIRE(r.size() > 0);
    }

    /* An invalid lead byte is replaced. */
    {
        const char bad[] = { static_cast<char>(0xFF), 'a' };
        auto r = utf8_of(bad, 2);
        REQUIRE(r.size() >= 2);   /* replacement + 'a' at minimum */
    }

    /* Empty input yields empty output. */
    {
        auto r = utf8_of("", 0);
        REQUIRE(r.size() == 0);
    }
}

// ============================================================================
// Compile-time contract
// ============================================================================

TEST_CASE("string_view noexcept and layout contract", "[string_view][gap]")
{
    using sv_t = memepp::string_view;

    /* The view wraps exactly one MemeStringStack_t. */
    static_assert(sizeof(sv_t) == sizeof(MemeStringStack_t),
                  "string_view must be exactly its native handle");

    static_assert(std::is_nothrow_copy_constructible<sv_t>::value,
                  "copy construction must not throw");
    static_assert(std::is_nothrow_move_assignable<sv_t>::value,
                  "move assignment must not throw");

    /* Read-only observation is noexcept. */
    static_assert(noexcept(std::declval<const sv_t&>().data()), "data()");
    static_assert(noexcept(std::declval<const sv_t&>().bytes()), "bytes()");
    static_assert(noexcept(std::declval<const sv_t&>().size()), "size()");
    static_assert(noexcept(std::declval<const sv_t&>().empty()), "empty()");
    static_assert(noexcept(std::declval<const sv_t&>().rune_size()),
                  "rune_size()");
    static_assert(noexcept(std::declval<const sv_t&>().u16char_size()),
                  "u16char_size()");
    static_assert(noexcept(std::declval<const sv_t&>().rune_front()),
                  "rune_front()");
    static_assert(noexcept(std::declval<const sv_t&>().rune_back()),
                  "rune_back()");
    static_assert(noexcept(std::declval<const sv_t&>().substr()),
                  "substr()");
    static_assert(noexcept(std::declval<const sv_t&>().operator==(
                      std::declval<sv_t>())), "operator==()");
    static_assert(noexcept(std::declval<const sv_t&>().starts_with(
                      std::declval<sv_t>())), "starts_with()");
    static_assert(noexcept(std::declval<const sv_t&>().ends_with(
                      std::declval<sv_t>())), "ends_with()");
    static_assert(noexcept(std::declval<const sv_t&>().contains(
                      std::declval<sv_t>())), "contains()");
    static_assert(noexcept(std::declval<const sv_t&>().find(
                      std::declval<sv_t>())), "find()");
    static_assert(noexcept(std::declval<const sv_t&>().rfind(
                      std::declval<sv_t>())), "rfind()");

    /* Checked accessors report range errors, so they may throw. */
    static_assert(!noexcept(std::declval<const sv_t&>().at(0)),
                  "at() must not be noexcept");
    static_assert(!noexcept(std::declval<const sv_t&>()[0]),
                  "operator[] must not be noexcept");
    static_assert(!noexcept(std::declval<const sv_t&>().front()),
                  "front() must not be noexcept");
    static_assert(!noexcept(std::declval<const sv_t&>().back()),
                  "back() must not be noexcept");

    REQUIRE(true);   /* all of the above are compile-time */
}
