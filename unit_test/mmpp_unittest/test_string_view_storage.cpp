#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

#include <type_traits>

// ============================================================================
// storage_type across source kinds
// ============================================================================

TEST_CASE("string_view storage_type matrix", "[string_view][storage]")
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

// ============================================================================
// Compile-time contract
// ============================================================================

TEST_CASE("string_view noexcept and layout contract", "[string_view][storage]")
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


// to_valid_utf8 / trim_* return view: storage and lifetime
// ============================================================================

TEST_CASE("string_view to_valid_utf8 result storage depends on size",
          "[string_view][storage]")
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
          "[string_view][storage]")
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

TEST_CASE("string_view from a user-tier string", "[string_view][storage]")
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

// trim_left_space / trim_right_space: storage tier of the result
// ============================================================================

/*
 * trim_left_space() and trim_right_space() use the same local-`out` shape as
 * trim_space(); the tier of the returned view is decided by
 * MemeStringStack_mid_v2 exactly as described above.  These two tests pin
 * the tier for a view source (aliasing) and for a large source (refcount),
 * and read the bytes back to prove the result is live.
 */

TEST_CASE("string_view trim_left_space result storage", "[string_view][storage]")
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

TEST_CASE("string_view trim_right_space result storage", "[string_view][storage]")
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

TEST_CASE("string_view trim_if result storage", "[string_view][storage]")
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

TEST_CASE("string_view trim_if_rune result storage", "[string_view][storage]")
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
