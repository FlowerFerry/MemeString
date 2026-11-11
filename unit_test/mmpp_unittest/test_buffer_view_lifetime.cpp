#include <catch2/catch.hpp>

#include <memepp/buffer_view.hpp>
#include <memepp/variable_buffer.hpp>
#include <memepp/buffer.hpp>
#include <memepp/string.hpp>

// Alias / lifetime semantics
// ============================================================================

TEST_CASE("buffer_view aliases its source and does not own it",
          "[buffer_view][lifetime]")
{
    /* A view over a variable_buffer must observe source mutations. */
    memepp::variable_buffer vb;
    vb.append(reinterpret_cast<const mmbyte_t*>("abc"), 3);

    memepp::buffer_view bv(vb);
    REQUIRE(bv.size() == 3);
    REQUIRE(bv.data() == vb.data());        /* same address, no copy */
    REQUIRE(bv.at(1) == 'b');

    vb[1] = 'Z';
    REQUIRE(bv.at(1) == 'Z');

    /* A copy of the view aliases the same bytes as the original. */
    memepp::buffer_view copy(bv);
    REQUIRE(copy.data() == bv.data());
    vb[1] = 'Q';
    REQUIRE(copy.at(1) == 'Q');
}

TEST_CASE("buffer_view outlives a shared backing buffer",
          "[buffer_view][lifetime]")
{
    /* A view built from a buffer shares its refcounted storage, so the view
     * stays valid after the source buffer goes out of scope. */
    const uint8_t raw[] = { 'p', 'q', 'r' };
    memepp::buffer_view bv;
    {
        memepp::buffer tmp(raw, 3);
        bv = tmp;
    }
    REQUIRE(bv.size() == 3);
    REQUIRE(bv.at(0) == 'p');
    REQUIRE(bv.at(2) == 'r');

    /* A copy taken before the source dies is equally valid. */
    memepp::buffer_view copy(bv);
    REQUIRE(copy.size() == 3);
    REQUIRE(copy.at(1) == 'q');
}

TEST_CASE("buffer_view move leaves the source usable",
          "[buffer_view][lifetime]")
{
    const uint8_t raw[] = { 'm', 'n', 'o' };
    memepp::buffer_view src(raw, 3);

    memepp::buffer_view dst(std::move(src));
    REQUIRE(dst.size() == 3);
    REQUIRE(dst.at(0) == 'm');

    /* The moved-from object is swapped to an empty-ish state; it must at
     * least remain destructible and re-assignable. */
    src = memepp::buffer_view(raw, 3);
    REQUIRE(src.size() == 3);
    REQUIRE(src.at(2) == 'o');
}

// ============================================================================

// string-backed view lifetime
// ============================================================================

TEST_CASE("buffer_view from string&&: shared branch keeps the block",
          "[buffer_view][gap2]")
{
    /*
     * buffer_view(string&&) branches on MemeString_isSharedStorageTypes.
     * When the string is large (shared), the view takes a refcount on the
     * block, so it survives the source string's destruction.
     */
    memepp::buffer_view bv;
    {
        std::string big(100, 'q');
        /* Explicitly request large so the refcount branch is taken. */
        memepp::string src(big.data(),
                           static_cast<memepp::string::size_type>(big.size()),
                           memepp::string_storage_t::large);
        /* A 100-byte string stays medium by default; requesting large may
         * or may not be honoured.  What matters is that it is shared. */
        REQUIRE(src.storage_type() != memepp::string_storage_t::small);

        bv = std::move(src);
    }
    /* The block is kept alive by the view's refcount. */
    REQUIRE(bv.size() == 100);
    REQUIRE(bv.at(0) == 'q');
    REQUIRE(bv.at(99) == 'q');
}

TEST_CASE("buffer_view from non-shared string is a borrow",
          "[buffer_view][gap2]")
{
    /*
     * When the string is medium or small (non-shared),
     * MemeBufferViewUnsafeStack_initByOther falls back to
     * MemeStringViewUnsafeStack_init, which is a pure view_unsafe over the
     * string's bytes.  The source must outlive the view.
     */
    const char backing[] = "hello";
    memepp::string src(backing, 5);   /* small/medium, not shared */
    memepp::buffer_view bv(src);
    REQUIRE(bv.size() == 5);
    REQUIRE(bv.at(0) == 'h');
    REQUIRE(bv.data() == reinterpret_cast<const uint8_t*>(src.data()));

    /* A string_view source is likewise a pure borrow. */
    memepp::string_view sv(backing, 5);
    memepp::buffer_view other(sv);
    REQUIRE(other.size() == 5);
    REQUIRE(other.at(0) == 'h');
    REQUIRE(other.data() == reinterpret_cast<const uint8_t*>(backing));
}

TEST_CASE("buffer_view from buffer shares the block", "[buffer_view][gap2]")
{
    /*
     * buffer_view(const buffer&) takes a refcount through
     * MemeBufferViewUnsafeStack_initByOther -> MemeStringStack_initByOther
     * (because the buffer is shared/large).  The view survives the source
     * buffer's destruction.
     */
    const uint8_t raw[] = { 'p', 'q', 'r' };
    memepp::buffer_view bv;
    {
        memepp::buffer tmp(raw, 3, memepp::buffer_storage_t::large);
        REQUIRE(tmp.storage_type() == memepp::buffer_storage_t::large);
        bv = tmp;
    }
    REQUIRE(bv.size() == 3);
    REQUIRE(bv.at(0) == 'p');
    REQUIRE(bv.at(2) == 'r');
}

// ============================================================================

// native_handle
// ============================================================================

TEST_CASE("buffer_view native_handle reflects the view state",
          "[buffer_view][gap2]")
{
    const uint8_t raw[] = { 'a', 'b', 'c' };
    memepp::buffer_view bv(raw, 3);

    /* The handle is the object itself. */
    REQUIRE(&bv.native_handle() == reinterpret_cast<const void*>(&bv));

    /* Its size/data agree with the wrapper. */
    REQUIRE(MemeBuffer_size(memepp::to_pointer(bv.native_handle())) == 3);
    REQUIRE(MemeBuffer_data(memepp::to_pointer(bv.native_handle())) == raw);
    REQUIRE(MemeBuffer_storageType(memepp::to_pointer(bv.native_handle()))
            == MemeBuffer_UnsafeStorageType_view);

    /* An empty view reports zero size through the C API too. */
    memepp::buffer_view e;
    REQUIRE(MemeBuffer_size(memepp::to_pointer(e.native_handle())) == 0);
    REQUIRE(MemeBuffer_isEmpty(memepp::to_pointer(e.native_handle())) == 1);

    /* After assignment the handle describes the new contents. */
    const uint8_t other[] = { 'p', 'q' };
    memepp::buffer_view otherv(other, 2);
    bv = otherv;
    REQUIRE(MemeBuffer_size(memepp::to_pointer(bv.native_handle())) == 2);
    REQUIRE(MemeBuffer_data(memepp::to_pointer(bv.native_handle())) == other);
}

// ============================================================================

// Empty view end()/cend() pointer arithmetic
// ============================================================================

TEST_CASE("buffer_view empty view iterator arithmetic",
          "[buffer_view][gap2]")
{
    memepp::buffer_view e;

    /* end() is data() + size().  Whatever data() is, the two iterators must
     * agree and the range must be empty. */
    REQUIRE(e.begin() == e.end());
    REQUIRE(e.cbegin() == e.cend());
    REQUIRE(static_cast<ptrdiff_t>(e.end() - e.begin()) == 0);
    REQUIRE(static_cast<ptrdiff_t>(e.cend() - e.cbegin()) == 0);

    /* A range-for over an empty view runs zero iterations. */
    size_t n = 0;
    for (uint8_t v : e)
    {
        (void)v;
        ++n;
    }
    REQUIRE(n == 0);
}

// ============================================================================
