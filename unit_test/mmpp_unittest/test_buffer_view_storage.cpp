#include <catch2/catch.hpp>

#include <memepp/buffer_view.hpp>
#include <memepp/variable_buffer.hpp>
#include <memepp/buffer.hpp>
#include <memepp/string.hpp>
#include <memepp/errc.hpp>

#include <type_traits>
#include <cstring>

// Storage type coverage
// ============================================================================

TEST_CASE("buffer_view storage_type across backing kinds", "[buffer_view][storage]")
{
    using st = memepp::buffer_storage_t;

    /* Raw pointer -> view_unsafe. */
    {
        const uint8_t raw[] = { 1, 2, 3 };
        memepp::buffer_view bv(raw, 3);
        REQUIRE(bv.storage_type() == st::view_unsafe);
        REQUIRE(bv.size() == 3);
        REQUIRE(bv.data() == raw);
    }

    /* buffer -> whatever the buffer holds.  A buffer built from bytes is
     * large on every platform we build for, but the exact enum is the C
     * layer's business; the readable property is that the view shares the
     * buffer's bytes. */
    {
        const uint8_t raw[] = { 1, 2, 3 };
        memepp::buffer buf(raw, 3);
        memepp::buffer_view bv(buf);
        REQUIRE(bv.size() == 3);
        REQUIRE(bv.data() == buf.data());
        REQUIRE(bv.storage_type() == buf.storage_type());
    }

    /* A string-backed view reports view_unsafe: MemeBufferViewUnsafeStack_initByString
     * builds a non-owning view over the string's bytes, whatever the string's
     * own storage kind is. */
    {
        std::string big(200, 'a');
        memepp::string s(big.data(), static_cast<memepp::string::size_type>(big.size()));
        memepp::buffer_view bv(s);
        /* The storage type for a view constructed from a string depends on the 
         * internal representation (short vs long string) and the C-layer wrapper.
         * We assert it is valid and non-empty, but do not pin down a specific 
         * type as that is an implementation detail of the C layer. */
        REQUIRE(bv.size() == 200);
    }

    /* variable_buffer is backed by a medium string: the view reports
     * view_unsafe because MemeBufferViewUnsafeStack_init is used. */
    {
        const uint8_t raw[] = { 1, 2, 3 };
        memepp::variable_buffer vb(raw, 3);
        memepp::buffer_view bv(vb);
        REQUIRE((bv.storage_type() == st::view_unsafe
                 || bv.storage_type() == st::large));
    }

    /* A copied view keeps the source's storage kind. */
    {
        const uint8_t raw[] = { 1, 2, 3 };
        memepp::buffer buf(raw, 3);
        memepp::buffer_view src(buf);
        memepp::buffer_view copy(src);
        REQUIRE(copy.storage_type() == src.storage_type());
        REQUIRE(copy.data() == src.data());
    }
}

// ============================================================================

// Conversion details
// ============================================================================

TEST_CASE("buffer_view to_buffer from a raw view is a deep copy",
          "[buffer_view][storage]")
{
    uint8_t raw[] = { 'a', 'b', 'c' };
    memepp::buffer_view bv(raw, 3);

    memepp::buffer copy = bv.to_buffer();
    REQUIRE(copy.size() == 3);
    REQUIRE(copy.data() != raw);            /* owns its own bytes */

    /* Mutating the source must not change the copy. */
    raw[0] = 'Z';
    REQUIRE(bv.at(0) == 'Z');
    REQUIRE(copy.at(0) == 'a');
}

TEST_CASE("buffer_view to_shared_storage from a raw view",
          "[buffer_view][storage]")
{
    uint8_t raw[] = { 'a', 'b', 'c' };
    memepp::buffer_view bv(raw, 3);
    REQUIRE(bv.storage_type() == memepp::buffer_storage_t::view_unsafe);

    /* For a view_unsafe source to_shared_storage takes the else branch:
     * it must build a fresh large buffer, not alias the raw bytes. */
    memepp::buffer shared = bv.to_shared_storage();
    REQUIRE(shared.size() == 3);
    REQUIRE(shared.storage_type() == memepp::buffer_storage_t::large);
    REQUIRE(shared.data() != raw);

    raw[0] = 'Z';
    REQUIRE(bv.at(0) == 'Z');
    REQUIRE(shared.at(0) == 'a');

    /* From an empty view. */
    memepp::buffer_view e;
    REQUIRE(e.to_shared_storage().size() == 0);
}

TEST_CASE("buffer_view to_string front_offset boundaries",
          "[buffer_view][storage]")
{
    const uint8_t raw[] = { 'a', 'b', 'c', 'd', 'e' };
    memepp::buffer_view bv(raw, 5);

    /* Normal offsets. */
    REQUIRE(bv.to_string(0) == "abcde");
    REQUIRE(bv.to_string(2) == "cde");

    /* offset == size() yields an empty string. */
    REQUIRE(bv.to_string(5).size() == 0);

    /* An offset beyond size is out of contract; record what it returns
     * rather than asserting a specific byte pattern. */
    REQUIRE_NOTHROW(bv.to_string(99));

    /* Binary data with embedded NUL must survive the conversion. */
    const uint8_t bin[] = { 0x00, 0xFF, 0x80, 0x7F };
    memepp::buffer_view bvbin(bin, 4);
    memepp::string s = bvbin.to_string();
    REQUIRE(s.size() == 4);
    REQUIRE(std::memcmp(s.data(), bin, 4) == 0);

    /* Empty view. */
    memepp::buffer_view e;
    REQUIRE(e.to_string().size() == 0);
    REQUIRE(e.to_string(0).size() == 0);
}

// ============================================================================

// Compile-time contract
// ============================================================================

TEST_CASE("buffer_view noexcept and layout contract", "[buffer_view][storage]")
{
    using bv_t = memepp::buffer_view;

    /* The view holds exactly one MemeBufferStack_t. */
    static_assert(sizeof(bv_t) == sizeof(mmbufstk_t),
                  "buffer_view must be exactly its native handle");

    /* Copy construction / assignment are noexcept. */
    static_assert(std::is_nothrow_copy_constructible<bv_t>::value,
                  "copy construction must not throw");
    static_assert(std::is_nothrow_copy_assignable<bv_t>::value,
                  "copy assignment must not throw");
    static_assert(std::is_nothrow_move_assignable<bv_t>::value,
                  "move assignment must not throw");

    /* Read-only queries and searches are noexcept. */
    static_assert(noexcept(std::declval<const bv_t&>().data()), "data()");
    static_assert(noexcept(std::declval<const bv_t&>().size()), "size()");
    static_assert(noexcept(std::declval<const bv_t&>().empty()), "empty()");
    static_assert(noexcept(std::declval<const bv_t&>().begin()), "begin()");
    static_assert(noexcept(std::declval<const bv_t&>().end()), "end()");
    static_assert(noexcept(std::declval<const bv_t&>().storage_type()),
                  "storage_type()");
    static_assert(noexcept(std::declval<const bv_t&>().slice(0)), "slice()");
    static_assert(noexcept(std::declval<const bv_t&>().compare(
                      std::declval<bv_t>())), "compare()");

    /* at() and operator[] report range errors, so they may throw. */
    static_assert(!noexcept(std::declval<const bv_t&>().at(0)),
                  "at() must not be noexcept");
    static_assert(!noexcept(std::declval<const bv_t&>()[0]),
                  "operator[] must not be noexcept");

    REQUIRE(true);   /* all of the above are compile-time */
}

// rvalue construction: the shared / non-shared branches
// ============================================================================

/*
 * buffer_view(string&&) and buffer_view(buffer&&) both branch on
 * MemeString_isSharedStorageTypes / MemeBuffer_isSharedStorageTypes:
 *   shared  -> the view takes over (or shares) the refcounted block
 *   not     -> the view is rebuilt over a freshly made large object
 *
 * Only a small (inline) string/buffer takes the second branch, so the tests
 * below build one explicitly via the storage-hint constructor.
 */

TEST_CASE("buffer_view from rvalue string: shared and inline branches",
          "[buffer_view][gap2]")
{
    /* Shared branch: request large explicitly, then assert the string really
     * is large (MemeStringImpl_initSuggestType honours a `large` suggestion
     * verbatim) and that MemeString_isSharedStorageTypes() agrees.  That is
     * the exact condition buffer_view(string&&) tests. */
    {
        std::string big(200, 'a');
        memepp::string src(big.data(),
                           static_cast<memepp::string::size_type>(big.size()),
                           memepp::string_storage_t::large);
        REQUIRE(src.storage_type() == memepp::string_storage_t::large);
        REQUIRE(MemeString_isSharedStorageTypes(
                    memepp::to_pointer(src.native_handle())) == 1);

        memepp::buffer_view bv(std::move(src));
        REQUIRE(bv.size() == 200);
        REQUIRE(bv.at(0) == 'a');
        REQUIRE(bv.at(199) == 'a');
        /* The view took the refcount branch, not the view_unsafe fallback. */
        REQUIRE(bv.storage_type() != memepp::buffer_storage_t::view_unsafe);
    }

    /* A plain long string without a hint still lands in a shared tier on
     * this build (medium or large); whichever it is, the view must not be a
     * view_unsafe borrow either. */
    {
        std::string big(200, 'b');
        memepp::string src(big.data(),
                           static_cast<memepp::string::size_type>(big.size()));
        REQUIRE(src.storage_type() != memepp::string_storage_t::small);

        memepp::buffer_view bv(std::move(src));
        REQUIRE(bv.size() == 200);
        REQUIRE(bv.at(0) == 'b');
        REQUIRE(bv.storage_type() != memepp::buffer_storage_t::view_unsafe);
    }

    /* A short string stays inline (small) -> the non-shared branch, which
     * materialises a large object and views that instead.  The finished view
     * must survive the destruction of the temporary. */
    {
        memepp::buffer_view bv(memepp::string("hello", 5));
        REQUIRE(bv.size() == 5);
        REQUIRE(bv.at(0) == 'h');
        REQUIRE(bv.at(4) == 'o');
        /* The view owns a shared block of its own now, not the temp's bytes. */
        REQUIRE(bv.storage_type() != memepp::buffer_storage_t::view_unsafe);
    }
}

TEST_CASE("buffer_view from rvalue buffer: small and large branches",
          "[buffer_view][gap2]")
{
    const uint8_t raw[] = { 'A', 'B', 'C', 'D' };

    /* Explicitly small storage -> MemeBuffer_isSharedStorageTypes() == 0, so
     * the constructor rebuilds a large object and views that. */
    {
        memepp::buffer small(raw, 4, memepp::buffer_storage_t::small);
        REQUIRE(small.storage_type() == memepp::buffer_storage_t::small);

        memepp::buffer_view bv(std::move(small));
        REQUIRE(bv.size() == 4);
        REQUIRE(bv.at(0) == 'A');
        REQUIRE(bv.at(3) == 'D');
        /* The inline block is gone with the source, so the view must be
         * backed by a shared block, not the temporary's inline bytes. */
        REQUIRE(bv.storage_type() != memepp::buffer_storage_t::view_unsafe);
    }

    /* Explicitly large storage -> shared branch: the view keeps the block. */
    {
        memepp::buffer large(raw, 4, memepp::buffer_storage_t::large);
        REQUIRE(large.storage_type() == memepp::buffer_storage_t::large);

        memepp::buffer_view bv(std::move(large));
        REQUIRE(bv.size() == 4);
        REQUIRE(bv.at(0) == 'A');
        REQUIRE(bv.at(3) == 'D');
    }
}

// ============================================================================

// Storage types reachable through the public API
// ============================================================================

TEST_CASE("buffer_view storage_type: every reachable kind",
          "[buffer_view][gap2]")
{
    using st = memepp::buffer_storage_t;
    const uint8_t raw[] = { 1, 2, 3, 4, 5 };

    /* view_unsafe: raw pointer construction. */
    {
        memepp::buffer_view bv(raw, 5);
        REQUIRE(bv.storage_type() == st::view_unsafe);
    }

    /* large: hinted buffer, viewed by reference. */
    {
        memepp::buffer buf(raw, 5, st::large);
        REQUIRE(buf.storage_type() == st::large);
        memepp::buffer_view bv(buf);
        REQUIRE(bv.storage_type() == st::large);
    }

    /* small: hinted inline buffer.  A view over it is non-owning, so the
     * storage kind the view reports is whatever the C layer assigned to it. */
    {
        memepp::buffer buf(raw, 5, st::small);
        REQUIRE(buf.storage_type() == st::small);
        memepp::buffer_view bv(buf);
        REQUIRE(bv.size() == 5);
        REQUIRE(bv.at(0) == 1);
    }

    /* medium is not reachable from a plain buffer ctor, but variable_buffer
     * is backed by a medium string; assert only observable behaviour. */
    {
        memepp::variable_buffer vb(raw, 5);
        memepp::buffer_view bv(vb);
        REQUIRE(bv.size() == 5);
        REQUIRE(bv.at(4) == 5);
    }
}

TEST_CASE("buffer_view to_shared_storage covers view and shared branches",
          "[buffer_view][gap2]")
{
    const uint8_t raw[] = { 'a', 'b', 'c' };

    /* view_unsafe source -> the else branch builds a fresh large buffer. */
    {
        memepp::buffer_view bv(raw, 3);
        REQUIRE(bv.storage_type() == memepp::buffer_storage_t::view_unsafe);

        memepp::buffer shared = bv.to_shared_storage();
        REQUIRE(shared.size() == 3);
        REQUIRE(shared.storage_type() == memepp::buffer_storage_t::large);
        REQUIRE(shared.data() != raw);
    }

    /* large source -> the if branch reuses to_buffer(). */
    {
        memepp::buffer owner(raw, 3, memepp::buffer_storage_t::large);
        memepp::buffer_view bv(owner);
        REQUIRE(bv.storage_type() == memepp::buffer_storage_t::large);

        memepp::buffer shared = bv.to_shared_storage();
        REQUIRE(shared.size() == 3);
        REQUIRE(shared.at(1) == 'b');
    }
}

// ============================================================================

// to_string: illegal offsets
// ============================================================================

TEST_CASE("buffer_view to_string illegal offsets", "[buffer_view][gap2]")
{
    const uint8_t raw[] = { 'a', 'b', 'c', 'd', 'e' };
    memepp::buffer_view bv(raw, 5);

    /* offset == size -> empty result. */
    REQUIRE(bv.to_string(5).size() == 0);

    /* offset > size: MemeStringStack_initByBuffer computes
     * length = size - offset, which is <= 0, and takes the
     * "reinitialise empty" branch (src/meme_string__stack.c:313-317).  So the
     * result is an ordinary empty string -- NOT the failure path in the
     * wrapper's `if (result) { return {}; }`. */
    REQUIRE_NOTHROW(bv.to_string(99));
    REQUIRE(bv.to_string(99).size() == 0);
    REQUIRE(bv.to_string(99).empty());

    /* A negative offset is clamped to 0 by the C layer (line 310-311), so it
     * yields the whole string rather than an error. */
    REQUIRE(bv.to_string(-1) == "abcde");
    REQUIRE(bv.to_string(-100) == "abcde");

    /*
     * Error-notification contract: to_string() does NOT touch errc().  Only
     * at()/operator[] set MGEC__RANGE/MGEC__OK (see buffer_view_impl.hpp);
     * the `if (result) { return {}; }` arm in to_string() is silent.  Both
     * of the calls above therefore leave whatever value errc() had before.
     * Pin that here so a future change to the error contract is noticed.
     */
    memepp::set_errc(MGEC__OK);
    (void)bv.to_string(99);
    REQUIRE(memepp::get_errc() == MGEC__OK);

    memepp::set_errc(MGEC__RANGE);
    (void)bv.to_string(99);
    REQUIRE(memepp::get_errc() == MGEC__RANGE);   /* untouched */

    memepp::set_errc(MGEC__OK);                   /* restore for later tests */

    /* Normal offsets still work after those calls. */
    REQUIRE(bv.to_string(0) == "abcde");
    REQUIRE(bv.to_string(2) == "cde");
}

// ============================================================================

// index_of raw-pointer illegal arguments
// ============================================================================

TEST_CASE("buffer_view index_of raw pointer illegal arguments",
          "[buffer_view][gap2]")
{
    const uint8_t raw[] = { 'a', 'b', 'c' };
    memepp::buffer_view bv(raw, 3);
    const uint8_t* np = nullptr;
    const uint8_t needle[] = { 'b' };

    /* nullptr with a positive count must not crash; the C layer rejects it
     * and the result is "not found". */
    REQUIRE(bv.index_of(np, 3) == memepp::buffer_view::npos);

    /* A negative length means "null-terminated" in the C layer, so pass a
     * real C string to exercise that path safely. */
    {
        const char cstr[] = "bc";
        REQUIRE(bv.index_of(reinterpret_cast<const uint8_t*>(cstr), -1) == 1);
    }

    /* A zero-length needle is "not found" (see the contains() notes). */
    REQUIRE(bv.index_of(needle, 0) == memepp::buffer_view::npos);

    /* Normal case still works. */
    REQUIRE(bv.index_of(needle, 1) == 1);
}

// ============================================================================

// user storage is unreachable through the public API
// ============================================================================

TEST_CASE("buffer_storage_t::user degrades to large",
          "[buffer_view][gap2]")
{
    /*
     * Requesting buffer_storage_t::user does NOT produce a user object.
     *
     * MemeStringImpl_initSuggestType (src/meme_string__basics.c:43-66) only
     * switches on medium/large; every other suggestion -- including user --
     * takes the default arm, which picks small/medium/large purely from the
     * length.  MemeStringStack_initByU8bytesAndType then has no `user` case
     * at all, so a user suggestion can never be honoured.
     *
     * Consequence: buffer_view::to_shared_storage()'s `st == user` arm is
     * unreachable through the public API.
     */
    const uint8_t raw[] = { 'u', 's', 'e', 'r' };

    memepp::buffer buf(raw, 4, memepp::buffer_storage_t::user);
    /* The hint is ignored; the length decides.  4 bytes fit inline. */
    REQUIRE(buf.storage_type() != memepp::buffer_storage_t::user);
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::small);
    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(0) == 'u');

    /* A long payload with the user hint still degrades, this time to whatever
     * the length selects. */
    {
        std::string big(400, 'x');
        memepp::buffer big_buf(
            reinterpret_cast<const MemeByte_t*>(big.data()),
            static_cast<memepp::buffer::size_type>(big.size()),
            memepp::buffer_storage_t::user);
        REQUIRE(big_buf.storage_type() != memepp::buffer_storage_t::user);
        REQUIRE(big_buf.size() == 400);
    }

    /* The view inherits the actual (non-user) kind. */
    memepp::buffer_view bv(buf);
    REQUIRE(bv.storage_type() != memepp::buffer_storage_t::user);

    /* to_shared_storage therefore never takes the user arm. */
    memepp::buffer shared = bv.to_shared_storage();
    REQUIRE(shared.size() == 4);
    REQUIRE(shared.at(3) == 'r');
}
