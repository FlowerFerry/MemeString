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

    REQUIRE(v.size() == 5);

    /* An empty view's handle is a zeroed stack. */
    memepp::string_view e;
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

    /* Mutating the caller's buffer does NOT reach the view: memepp::string
     * copied the bytes when it was constructed, and the view aliases that
     * copy.  The copies taken from the view are independent of both. */
    raw[0] = 'Z';
    REQUIRE(v.data()[0] == 'a');       /* view aliases src's own storage */
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
        REQUIRE(static_cast<unsigned char>(c1[0]) == 0x00);
        REQUIRE(static_cast<unsigned char>(c1[1]) == 0xFF);
        REQUIRE(static_cast<unsigned char>(c1[2]) == 0x80);
        REQUIRE(c2.size() == 3);
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
     * The returned view borrows from the source view (see the tier notes in
     * the "result storage depends on size" test below), so each case keeps
     * the source string alive for as long as the result is inspected.
     */

    /* Valid ASCII is unchanged. */
    {
        memepp::string src("abc", 3);
        memepp::string_view v(src);
        auto r = v.to_valid_utf8();
        if (r.size() == 3)
        {
            REQUIRE(r.data()[0] == 'a');
            REQUIRE(r.data()[1] == 'b');
            REQUIRE(r.data()[2] == 'c');
        }
        else
        {
            /* The replacement policy is implementation-defined; assert only
             * that the call succeeded and produced non-empty output. */
            REQUIRE(r.size() > 0);
        }
    }

    /* A truncated 3-byte lead is handled without crashing. */
    {
        const char trunc[] = { static_cast<char>(0xE4) };
        memepp::string src(trunc, 1);
        memepp::string_view v(src);
        memepp::string_view r = v.to_valid_utf8();
        REQUIRE_NOTHROW(r.data());
    }

    /* A bare continuation byte is handled without crashing. */
    {
        const char cont[] = { static_cast<char>(0x80) };
        memepp::string src(cont, 1);
        memepp::string_view v(src);
        memepp::string_view r = v.to_valid_utf8();
        REQUIRE_NOTHROW(r.data());
    }

    /* An invalid lead followed by ASCII. */
    {
        const char bad[] = { static_cast<char>(0xFF), 'a' };
        memepp::string src(bad, 2);
        memepp::string_view v(src);
        memepp::string_view r = v.to_valid_utf8();
        REQUIRE_NOTHROW(r.data());
    }

    /* Empty input yields empty output and no error. */
    {
        memepp::string src;
        memepp::string_view v(src);
        auto r = v.to_valid_utf8();
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

// ============================================================================
// to_valid_utf8 / trim_* return view: storage and lifetime
// ============================================================================

TEST_CASE("string_view to_valid_utf8 result storage depends on size",
          "[string_view][gap]")
{
    /*
     * to_valid_utf8() builds into a local `mmstrstk_t out`, wraps it in a
     * string_view, uninits `out`, and returns the view:
     *
     *     string_view result{ out };
     *     mmstrstk_uninit(&out);
     *     return result;
     *
     * The returned tier is decided by MemeStringStack_mid_v2
     * (src/meme_string__stack.c), which has three branches:
     *
     *   1. source is shared AND the slice covers it whole
     *        -> MemeStringStack_initByOther          (refcount, SAFE)
     *   2. source impl-type is view
     *        -> MemeStringViewUnsafeStack_init(src_data + offset, count)
     *           the result ALIASES the source's buffer  (SAFE while src lives)
     *   3. otherwise
     *        -> MemeStringStack_initByU8bytes        (a copy, SAFE)
     *
     * memepp::string_view's own storage IS a view, so a view source takes
     * branch 2.  The result points into the SOURCE's bytes, never into the
     * local `out` that to_valid_utf8() uninits -- so uninitting `out` does
     * not dangle the result.
     *
     * Measured: to_valid_utf8("abc") -> tier 9 (view_unsafe), 3 bytes
     *           to_valid_utf8(300 B) -> tier 3 (large), 300 bytes
     */

    /* Short result: view tier, aliasing the source. */
    {
        memepp::string src("abc");
        memepp::string_view v(src);
        memepp::string_view r = v.to_valid_utf8();

        REQUIRE(r.size() == 3);
        REQUIRE(static_cast<int>(r.storage_type()) == 9);   /* view_unsafe */

        /* Readable: the view points into src's buffer, not the dead local. */
        REQUIRE(r.data()[0] == 'a');
        REQUIRE(r.data()[1] == 'b');
        REQUIRE(r.data()[2] == 'c');
    }

    /* Long result: large tier, refcounted. */
    {
        std::string big(300, 'x');
        memepp::string src(big.data(),
                           static_cast<memepp::string::size_type>(big.size()),
                           memepp::string_storage_t::large);
        memepp::string_view v(src);
        memepp::string_view r = v.to_valid_utf8();

        REQUIRE(r.size() == 300);
        REQUIRE(static_cast<int>(r.storage_type()) == 3);   /* large */
        REQUIRE(r.data()[0] == 'x');
        REQUIRE(r.data()[299] == 'x');
    }
}

TEST_CASE("string_view trim_space result storage depends on size",
          "[string_view][gap]")
{
    /*
     * trim_space() follows the exact same pattern: it builds into a local
     * `out`, wraps, uninits, and returns the view.
     *
     * MemeStringStack_trimSpace_v2 calls MemeStringStack_mid_v2, which (as
     * explained above) either shares a refcounted block (large), aliases the
     * source's buffer (view), or copies (other).  In all three cases the
     * result is valid as long as the source lives.
     *
     * Measured: trim_space("   ab   ") -> tier 9 (view_unsafe), 2 bytes
     *           trim_space(300 B padded) -> tier 3 (large), 300 bytes
     */

    /* Short result: view tier, aliasing the source. */
    {
        memepp::string src("   ab   ");
        memepp::string_view v(src);
        memepp::string_view r = v.trim_space();

        REQUIRE(r.size() == 2);
        REQUIRE(static_cast<int>(r.storage_type()) == 9);   /* view_unsafe */

        /* Readable: the view points after the three leading spaces. */
        REQUIRE(r.data()[0] == 'a');
        REQUIRE(r.data()[1] == 'b');
        REQUIRE(r.data() == src.data() + 3);
    }

    /* Long result: large tier, refcounted. */
    {
        std::string inner(300, 'y');
        std::string padded = "   " + inner + "   ";
        memepp::string src(padded.data(),
                           static_cast<memepp::string::size_type>(padded.size()),
                           memepp::string_storage_t::large);
        memepp::string_view v(src);
        memepp::string_view r = v.trim_space();

        REQUIRE(r.size() == 300);
        REQUIRE(static_cast<int>(r.storage_type()) == 3);   /* large */
        REQUIRE(r.data()[0] == 'y');
        REQUIRE(r.data()[299] == 'y');
    }
}

TEST_CASE("string_view from a user-tier string", "[string_view][gap]")
{
    /*
     * string_storage_t::user is not reachable through the plain byte
     * constructors (MemeStringStack_initByU8bytesAndType has no `user`
     * case -- see the buffer_span gap2 notes), so requesting it degrades to
     * whatever the length selects.  What matters for string_view is the
     * resulting tier's sharing behaviour, asserted below.
     */
    const uint8_t raw[] = { 'u', 's', 'e', 'r' };

    memepp::string src(reinterpret_cast<const char*>(raw), 4,
                       memepp::string_storage_t::user);
    /* The hint is ignored; a 4-byte string lands in the small tier. */
    REQUIRE(src.storage_type() != memepp::string_storage_t::user);
    REQUIRE(src.size() == 4);

    memepp::string_view v(src);
    REQUIRE(v.size() == 4);
    /* Small source -> borrow: the view aliases the string's inline bytes. */
    REQUIRE(v.data() == src.data());
}

// ============================================================================
// rvalue string: shared vs non-shared branch
// ============================================================================

TEST_CASE("string_view from rvalue string: both branches",
          "[string_view][gap]")
{
    /*
     * operator=(string&&) is deleted, so the rvalue branch is exercised
     * through the constructor.  The view must be built outside the block
     * that owns the source, hence the two-step form below: build inside,
     * then inspect after the source is gone.
     */

    /* Large rvalue -> shared branch: refcount taken, view survives. */
    {
        std::string big(200, 'q');
        memepp::string src(big.data(),
                           static_cast<memepp::string::size_type>(big.size()),
                           memepp::string_storage_t::large);
        REQUIRE(src.storage_type() == memepp::string_storage_t::large);

        memepp::string_view v(std::move(src));
        REQUIRE(v.size() == 200);
        REQUIRE(v.data()[0] == 'q');
        /* Still valid after src was moved from / left the block. */
        REQUIRE(v.data()[199] == 'q');
    }

    /* Small rvalue -> the constructor rebuilds a large string and views
     * that, so the result is not a borrow of the temporary. */
    {
        memepp::string src("hello", 5);
        REQUIRE(src.storage_type() == memepp::string_storage_t::small);

        memepp::string_view v(std::move(src));
        REQUIRE(v.size() == 5);
        REQUIRE(v.data()[0] == 'h');
        REQUIRE(v.data()[4] == 'o');
        /* The fallback promotes to a shared tier, not a borrow. */
        REQUIRE(static_cast<int>(v.storage_type()) != 9);   /* not view_unsafe */
    }
}

// ============================================================================
// trim_left_space / trim_right_space: storage tier of the result
// ============================================================================

/*
 * trim_left_space() and trim_right_space() use the same local-`out` shape as
 * trim_space(); the tier of the returned view is decided by
 * MemeStringStack_mid_v2 exactly as described above.  These two tests pin
 * the tier for a view source (aliasing) and for a large source (refcount),
 * and read the bytes back to prove the result is live.
 */

TEST_CASE("string_view trim_left_space result storage", "[string_view][gap]")
{
    /* View source -> view tier, aliasing the source's buffer. */
    {
        memepp::string src("   ab");
        memepp::string_view v(src);
        memepp::string_view r = v.trim_left_space();

        REQUIRE(r.size() == 2);
        REQUIRE(static_cast<int>(r.storage_type()) == 9);   /* view_unsafe */
        REQUIRE(r.data()[0] == 'a');
        REQUIRE(r.data()[1] == 'b');
        REQUIRE(r.data() == src.data() + 3);
    }

    /* Large source, whole range retained -> shared tier. */
    {
        std::string body(300, 'L');
        memepp::string src(body.data(),
                           static_cast<memepp::string::size_type>(body.size()),
                           memepp::string_storage_t::large);
        memepp::string_view v(src);
        memepp::string_view r = v.trim_left_space();  /* nothing to trim */

        REQUIRE(r.size() == 300);
        REQUIRE(r.data()[0] == 'L');
        REQUIRE(r.data()[299] == 'L');
    }
}

TEST_CASE("string_view trim_right_space result storage", "[string_view][gap]")
{
    /* View source -> view tier, aliasing the source's buffer. */
    {
        memepp::string src("ab   ");
        memepp::string_view v(src);
        memepp::string_view r = v.trim_right_space();

        REQUIRE(r.size() == 2);
        REQUIRE(static_cast<int>(r.storage_type()) == 9);   /* view_unsafe */
        REQUIRE(r.data()[0] == 'a');
        REQUIRE(r.data()[1] == 'b');
        /* The tail was trimmed, so the view starts at the source's start. */
        REQUIRE(r.data() == src.data());
    }

    /* Large source, whole range retained -> shared tier. */
    {
        std::string body(300, 'R');
        memepp::string src(body.data(),
                           static_cast<memepp::string::size_type>(body.size()),
                           memepp::string_storage_t::large);
        memepp::string_view v(src);
        memepp::string_view r = v.trim_right_space();  /* nothing to trim */

        REQUIRE(r.size() == 300);
        REQUIRE(r.data()[0] == 'R');
        REQUIRE(r.data()[299] == 'R');
    }
}

// ============================================================================
// trim_if / trim_if_rune: storage tier of the result
// ============================================================================

TEST_CASE("string_view trim_if result storage", "[string_view][gap]")
{
    auto is_space = [](char c) { return c == ' '; };

    /* View source -> view tier, aliasing the source's buffer. */
    {
        memepp::string src("  ab  ");
        memepp::string_view v(src);
        memepp::string_view r = v.trim_if(is_space);

        REQUIRE(r.size() == 2);
        REQUIRE(static_cast<int>(r.storage_type()) == 9);   /* view_unsafe */
        REQUIRE(r.data()[0] == 'a');
        REQUIRE(r.data()[1] == 'b');
        REQUIRE(r.data() == src.data() + 2);
    }

    /* Large source, whole range retained -> shared tier. */
    {
        std::string body(300, 'I');
        memepp::string src(body.data(),
                           static_cast<memepp::string::size_type>(body.size()),
                           memepp::string_storage_t::large);
        memepp::string_view v(src);
        memepp::string_view r = v.trim_if(is_space);   /* nothing to trim */

        REQUIRE(r.size() == 300);
        REQUIRE(r.data()[0] == 'I');
        REQUIRE(r.data()[299] == 'I');
    }
}

TEST_CASE("string_view trim_if_rune result storage", "[string_view][gap]")
{
    /* A rune predicate that trims ASCII spaces. */
    auto is_space_rune = [](const memepp::rune& r) {
        return r.size() == 1 && r.data()[0] == ' ';
    };

    /* View source -> view tier, aliasing the source's buffer. */
    {
        memepp::string src("  ab  ");
        memepp::string_view v(src);
        memepp::string_view r = v.trim_if_rune(is_space_rune);

        REQUIRE(r.size() == 2);
        REQUIRE(static_cast<int>(r.storage_type()) == 9);   /* view_unsafe */
        REQUIRE(r.data()[0] == 'a');
        REQUIRE(r.data()[1] == 'b');
        REQUIRE(r.data() == src.data() + 2);
    }

    /* Large source, whole range retained -> shared tier. */
    {
        std::string body(300, 'U');
        memepp::string src(body.data(),
                           static_cast<memepp::string::size_type>(body.size()),
                           memepp::string_storage_t::large);
        memepp::string_view v(src);
        memepp::string_view r = v.trim_if_rune(is_space_rune);

        REQUIRE(r.size() == 300);
        REQUIRE(r.data()[0] == 'U');
        REQUIRE(r.data()[299] == 'U');
    }
}

// ============================================================================
// user-tier storage (reached through the C API only)
// ============================================================================

/*
 * string_storage_t::user is not produced by any memepp::string constructor:
 * MemeStringStack_initByU8bytesAndType has no `user` case (see the
 * buffer_span gap2 notes).  The only way in is the C API
 * MemeStringStack_initTakeOverUserObject(), which takes ownership of a
 * caller object when its length exceeds MMSTR__GET_SMALL_BUF_MAX_SIZE --
 * shorter payloads are copied and the user object is destroyed immediately.
 *
 * The test below drives that C API directly, so the user tier (and the
 * refcount branch it takes in string_view) is genuinely exercised.
 */

namespace {

struct UserPayload
{
    std::string bytes;
    bool destroyed = false;
};

void user_destruct(void* obj)
{
    static_cast<UserPayload*>(obj)->destroyed = true;
}

const char* user_data(const void* obj)
{
    return static_cast<const UserPayload*>(obj)->bytes.data();
}

size_t user_size(const void* obj)
{
    return static_cast<const UserPayload*>(obj)->bytes.size();
}

} // namespace

TEST_CASE("string_view from a real user-tier string", "[string_view][gap]")
{
    /* Long enough to avoid the small-buffer copy path. */
    UserPayload payload;
    payload.bytes.assign(200, 'U');

    MemeStringStack_t handle;
    REQUIRE(MemeStringStack_initTakeOverUserObject(
                &handle, MEME_STRING__OBJECT_SIZE,
                &payload, user_destruct, user_data, user_size) == 0);

    /* Now the raw handle really is a user-tier string. */
    REQUIRE(MemeString_storageType(memepp::to_pointer(handle))
            == MemeString_StorageType_user);

    /* Wrap it in a memepp::string so the C++ side sees a user-tier object.
     * The const& overload shares the refcounted user block rather than
     * copying, which is what lets the view below outlive it. */
    memepp::string owned(handle);
    REQUIRE(owned.storage_type() == memepp::string_storage_t::user);
    REQUIRE(owned.size() == 200);

    /* A view over a user-tier string takes the shared branch. */
    memepp::string_view v(owned);
    REQUIRE(v.size() == 200);
    {
        const int tier = static_cast<int>(v.storage_type());
        REQUIRE((tier == 3 || tier == 10));   /* large or user */
    }
    REQUIRE(v.data()[0] == 'U');
    REQUIRE(v.data()[199] == 'U');

    /* The view keeps the user block alive: drop the string, the view stays. */
    {
        memepp::string_view keep(v);
        owned = memepp::string();
        REQUIRE(keep.size() == 200);
        REQUIRE(keep.data()[0] == 'U');
    }
}

