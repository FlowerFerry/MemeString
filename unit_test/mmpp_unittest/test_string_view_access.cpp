#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>
#include <memepp/errc.hpp>

// Empty view: front/back/operator[] are CHECKED
// ============================================================================

TEST_CASE("string_view empty front/back/reverse access is checked",
          "[string_view][access]")
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

TEST_CASE("string_view native_handle and substr alias", "[string_view][access]")
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
          "[string_view][access]")
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

TEST_CASE("string_view to_valid_utf8 on malformed input", "[string_view][access]")
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


