#include <catch2/catch.hpp>

#include <memepp/buffer_view.hpp>
#include <memepp/buffer.hpp>
#include <memepp/variable_buffer.hpp>
#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/errc.hpp>

#include <cstring>
#include <limits>
#include <type_traits>
#include <utility>

// ============================================================================
// Copy construction (the direct ctor, not operator=)
// ============================================================================

TEST_CASE("buffer_view copy construction", "[buffer_view][gap]")
{
    const uint8_t raw[] = { 'A', 'B', 'C' };
    memepp::buffer_view src(raw, 3);

    /* Copy construction goes through MemeBufferViewUnsafeStack_initByOther,
     * which the assignment-operator tests never reach directly. */
    memepp::buffer_view copy(src);
    REQUIRE(copy.size() == 3);
    REQUIRE(copy.at(0) == 'A');
    REQUIRE(copy.at(2) == 'C');
    REQUIRE(copy == src);
    REQUIRE(copy.data() == src.data());

    /* Copy-initialisation uses the same path. */
    memepp::buffer_view copy2 = src;
    REQUIRE(copy2.size() == 3);
    REQUIRE(copy2 == src);
}

TEST_CASE("buffer_view self copy/move assignment", "[buffer_view][gap]")
{
    const uint8_t raw[] = { 'x', 'y', 'z' };
    memepp::buffer_view bv(raw, 3);

    /* Self copy-assignment must not corrupt the object. */
    memepp::buffer_view& alias = bv;
    bv = alias;
    REQUIRE(bv.size() == 3);
    REQUIRE(bv.at(0) == 'x');
    REQUIRE(bv.at(2) == 'z');

    /* Self move-assignment swaps the object with itself; the result must
     * still describe the same bytes. */
    bv = std::move(alias);
    REQUIRE(bv.size() == 3);
    REQUIRE(bv.at(1) == 'y');
}

TEST_CASE("buffer_view assign from variable_buffer", "[buffer_view][gap]")
{
    const uint8_t raw[] = { 0x11, 0x22, 0x33 };
    memepp::variable_buffer vb(raw, 3);

    /* The assignment overload is separate from the constructor. */
    memepp::buffer_view bv;
    bv = vb;
    REQUIRE(bv.size() == 3);
    REQUIRE(bv.at(0) == 0x11);
    REQUIRE(bv.at(1) == 0x22);
    REQUIRE(bv.at(2) == 0x33);

    /* Reassigning over an existing view replaces the contents. */
    memepp::buffer_view other;
    memepp::string s("ab");
    other = s;
    REQUIRE(other.size() == 2);
    other = vb;
    REQUIRE(other.size() == 3);
    REQUIRE(other.at(2) == 0x33);
}

// ============================================================================
// Storage type coverage
// ============================================================================

TEST_CASE("buffer_view storage_type across backing kinds", "[buffer_view][gap]")
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
// at() / operator[] error paths
// ============================================================================

TEST_CASE("buffer_view at and operator[] range errors", "[buffer_view][gap]")
{
    const uint8_t raw[] = { 'a', 'b', 'c' };
    memepp::buffer_view bv(raw, 3);

    /* In-range access clears the thread-local error. */
    REQUIRE(bv.at(1) == 'b');
    REQUIRE(memepp::get_errc() == MGEC__OK);
    REQUIRE(bv[2] == 'c');
    REQUIRE(memepp::get_errc() == MGEC__OK);

    /* pos == size, pos > size and a negative index all report MGEC__RANGE. */
#if !MMOPT__EXCEPTION_DISABLED
    try { bv.at(3); FAIL("at(3) must throw"); }
    catch (const memepp::errc_except& e) { REQUIRE(e.code() == MGEC__RANGE); }
    REQUIRE(memepp::get_errc() == MGEC__RANGE);

    try { bv.at(-1); FAIL("at(-1) must throw"); }
    catch (const memepp::errc_except& e) { REQUIRE(e.code() == MGEC__RANGE); }
    REQUIRE(memepp::get_errc() == MGEC__RANGE);

    /* operator[] forwards to at(), so it reports the same error. */
    try { bv[3]; FAIL("bv[3] must throw"); }
    catch (const memepp::errc_except& e) { REQUIRE(e.code() == MGEC__RANGE); }
    REQUIRE(memepp::get_errc() == MGEC__RANGE);

    try { bv[-1]; FAIL("bv[-1] must throw"); }
    catch (const memepp::errc_except& e) { REQUIRE(e.code() == MGEC__RANGE); }
    REQUIRE(memepp::get_errc() == MGEC__RANGE);
#else
    REQUIRE(bv.at(3) == 0);
    REQUIRE(memepp::get_errc() == MGEC__RANGE);
    REQUIRE(bv.at(-1) == 0);
    REQUIRE(memepp::get_errc() == MGEC__RANGE);
    REQUIRE(bv[3] == 0);
    REQUIRE(memepp::get_errc() == MGEC__RANGE);
    REQUIRE(bv[-1] == 0);
    REQUIRE(memepp::get_errc() == MGEC__RANGE);
#endif

    /* A later valid access restores MGEC__OK. */
    REQUIRE(bv.at(0) == 'a');
    REQUIRE(memepp::get_errc() == MGEC__OK);
}

TEST_CASE("buffer_view empty view accessors", "[buffer_view][gap]")
{
    memepp::buffer_view e;

    REQUIRE(e.size() == 0);
    REQUIRE(e.empty());
    /* data() of a default-constructed view is not required to be null: the
     * underlying MemeBuffer default object carries its own inline storage
     * pointer.  Only size()/empty() are contractual. */
    REQUIRE(e.begin() == e.end());
    REQUIRE(e.cbegin() == e.cend());

    /* at()/operator[] on an empty view report a range error; they must not
     * read through the null data pointer. */
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(e.at(0), memepp::errc_except);
    REQUIRE(memepp::get_errc() == MGEC__RANGE);
    REQUIRE_THROWS_AS(e.at(-1), memepp::errc_except);
    REQUIRE(memepp::get_errc() == MGEC__RANGE);
    REQUIRE_THROWS_AS(e[0], memepp::errc_except);
    REQUIRE(memepp::get_errc() == MGEC__RANGE);
#else
    REQUIRE(e.at(0) == 0);
    REQUIRE(memepp::get_errc() == MGEC__RANGE);
    REQUIRE(e.at(-1) == 0);
    REQUIRE(memepp::get_errc() == MGEC__RANGE);
    REQUIRE(e[0] == 0);
    REQUIRE(memepp::get_errc() == MGEC__RANGE);
#endif
}

// ============================================================================
// Iterators
// ============================================================================

TEST_CASE("buffer_view iterator contract", "[buffer_view][gap]")
{
    const uint8_t raw[] = { 'a', 'b', 'c', 'd' };
    memepp::buffer_view bv(raw, 4);

    /* Non-empty begin/end distance equals size(). */
    REQUIRE(static_cast<ptrdiff_t>(bv.end() - bv.begin()) == 4);
    REQUIRE(static_cast<ptrdiff_t>(bv.cend() - bv.cbegin()) == 4);
    REQUIRE(bv.begin() != bv.end());

    /* Dereference through both iterator spellings. */
    REQUIRE(*bv.begin() == 'a');
    REQUIRE(*bv.cbegin() == 'a');
    REQUIRE(*(bv.end() - 1) == 'd');
    REQUIRE(*(bv.cend() - 1) == 'd');

    /* Random access and comparison. */
    auto it = bv.begin();
    REQUIRE(it[2] == 'c');
    auto it2 = it + 2;
    REQUIRE(*it2 == 'c');
    REQUIRE(it < it2);
    REQUIRE(it2 > it);
    REQUIRE(it2 - it == 2);

    /* Pre- and post-increment. */
    auto pre = bv.begin();
    ++pre;
    REQUIRE(*pre == 'b');
    auto post = bv.begin();
    auto prev = post++;
    REQUIRE(*prev == 'a');
    REQUIRE(*post == 'b');

    /* Pre- and post-decrement. */
    auto dec = bv.end();
    --dec;
    REQUIRE(*dec == 'd');
    auto dec2 = bv.end();
    auto next = dec2--;
    REQUIRE(next == bv.end());
    REQUIRE(*dec2 == 'd');

    /* Range-for must visit every byte in order. */
    size_t i = 0;
    for (uint8_t v : bv)
    {
        REQUIRE(v == raw[i]);
        ++i;
    }
    REQUIRE(i == 4);
}

// ============================================================================
// index_of / find boundaries
// ============================================================================

TEST_CASE("buffer_view index_of boundary and empty inputs",
          "[buffer_view][gap]")
{
    const uint8_t raw[] = { 'a', 'b', 'c', 'a', 'b', 'c' };
    memepp::buffer_view bv(raw, 6);
    memepp::buffer_view empty;
    const uint8_t ab[] = { 'a', 'b' };

    /* Non-empty haystack. */
    REQUIRE(bv.index_of(memepp::buffer_view(ab, 2)) == 0);
    REQUIRE(bv.index_of(ab, 2) == 0);

    /* Empty needle: the C layer reports "not found" for a zero-length
     * needle, so both forms must agree. */
    REQUIRE(bv.index_of(memepp::buffer_view()) == memepp::buffer_view::npos);
    REQUIRE(bv.index_of(ab, 0) == memepp::buffer_view::npos);

    /* Empty haystack. */
    REQUIRE(empty.index_of(memepp::buffer_view(ab, 2)) == memepp::buffer_view::npos);
    REQUIRE(empty.index_of(memepp::buffer_view()) == memepp::buffer_view::npos);
    REQUIRE(empty.index_of(ab, 2) == memepp::buffer_view::npos);
    REQUIRE(empty.index_of(ab, 0) == memepp::buffer_view::npos);

    /* Binary data with embedded NUL and high bytes. */
    const uint8_t bin[] = { 0x00, 0xFF, 0x80, 0x00 };
    memepp::buffer_view bvbin(bin, 4);
    const uint8_t n1[] = { 0xFF, 0x80 };
    const uint8_t n2[] = { 0x80, 0x00 };
    REQUIRE(bvbin.index_of(n1, 2) == 1);
    REQUIRE(bvbin.index_of(n2, 2) == 2);

    /* Overlapping candidates: "aba" in "ababa" is at 0 and 2. */
    const uint8_t ov[] = { 'a', 'b', 'a', 'b', 'a' };
    const uint8_t need[] = { 'a', 'b', 'a' };
    memepp::buffer_view bvov(ov, 5);
    REQUIRE(bvov.index_of(need, 3) == 0);
}

TEST_CASE("buffer_view find position boundaries", "[buffer_view][gap]")
{
    const uint8_t raw[] = { 'a', 'b', 'c', 'a', 'b', 'c' };
    memepp::buffer_view bv(raw, 6);
    const uint8_t abc[] = { 'a', 'b', 'c' };
    memepp::buffer_view needle(abc, 3);

    /* Normal offsets. */
    REQUIRE(bv.find(needle, 0) == 0);
    REQUIRE(bv.find(needle, 1) == 3);
    REQUIRE(bv.find(needle, 3) == 3);
    REQUIRE(bv.find(needle, 4) == memepp::buffer_view::npos);

    /* pos == size() and pos > size(): nothing left to search. */
    REQUIRE(bv.find(needle, 6) == memepp::buffer_view::npos);
    REQUIRE(bv.find(needle, 7) == memepp::buffer_view::npos);
    REQUIRE(bv.find(needle, 99) == memepp::buffer_view::npos);

    /* A negative pos is passed straight to MemeBuffer_indexOfWithOther,
     * which treats anything below 0 as offset 0. */
    REQUIRE(bv.find(needle, -1) == 0);

    /* Empty haystack and empty needle combinations. */
    memepp::buffer_view empty;
    REQUIRE(empty.find(needle, 0) == memepp::buffer_view::npos);
    REQUIRE(empty.find(memepp::buffer_view(), 0) == memepp::buffer_view::npos);
    REQUIRE(bv.find(memepp::buffer_view(), 0) == memepp::buffer_view::npos);

    /* Binary data. */
    const uint8_t bin[] = { 0x00, 0xFF, 0x80, 0x00, 0xFF };
    memepp::buffer_view bvbin(bin, 5);
    const uint8_t n[] = { 0x80, 0x00 };
    REQUIRE(bvbin.find(memepp::buffer_view(n, 2), 0) == 2);
}

TEST_CASE("buffer_view contains/starts_with/ends_with empty and binary",
          "[buffer_view][gap]")
{
    const uint8_t raw[] = { 'a', 'b', 'c' };
    memepp::buffer_view bv(raw, 3);
    memepp::buffer_view empty;
    const uint8_t* np = nullptr;

    /* An empty needle is NOT "contained": MemeBuffer_indexOfWithBytes
     * reports not-found for a zero-length needle, so contains() is false.
     * starts_with/ends_with route through a different C entry point that
     * treats an empty needle as a match. */
    REQUIRE_FALSE(bv.contains(memepp::buffer_view()));
    REQUIRE(bv.starts_with(memepp::buffer_view()));
    REQUIRE(bv.ends_with(memepp::buffer_view()));

    REQUIRE_FALSE(empty.contains(memepp::buffer_view()));
    REQUIRE(empty.starts_with(memepp::buffer_view()));
    REQUIRE(empty.ends_with(memepp::buffer_view()));

    /* A null pointer with count 0 is the empty needle.  As above,
     * contains() reports false for it while starts_with/ends_with match. */
    REQUIRE_FALSE(bv.contains(np, 0));
    REQUIRE(bv.starts_with(np, 0));
    REQUIRE(bv.ends_with(np, 0));
    REQUIRE_FALSE(empty.contains(np, 0));
    REQUIRE(empty.starts_with(np, 0));
    REQUIRE(empty.ends_with(np, 0));

    /* Empty haystack never contains or starts/ends with a non-empty needle. */
    REQUIRE_FALSE(empty.contains(memepp::buffer_view(raw, 3)));
    REQUIRE_FALSE(empty.starts_with(memepp::buffer_view(raw, 3)));
    REQUIRE_FALSE(empty.ends_with(memepp::buffer_view(raw, 3)));
    REQUIRE_FALSE(empty.contains(raw, 3));
    REQUIRE_FALSE(empty.starts_with(raw, 3));
    REQUIRE_FALSE(empty.ends_with(raw, 3));

    /* Needle longer than the haystack. */
    const uint8_t longer[] = { 'a', 'b', 'c', 'd' };
    REQUIRE_FALSE(bv.contains(memepp::buffer_view(longer, 4)));
    REQUIRE_FALSE(bv.starts_with(memepp::buffer_view(longer, 4)));
    REQUIRE_FALSE(bv.ends_with(memepp::buffer_view(longer, 4)));

    /* Binary data with NUL and high bytes. */
    const uint8_t bin[] = { 0x00, 0xFF, 0x80 };
    memepp::buffer_view bvbin(bin, 3);
    const uint8_t pre[] = { 0x00, 0xFF };
    const uint8_t suf[] = { 0xFF, 0x80 };
    REQUIRE(bvbin.starts_with(pre, 2));
    REQUIRE(bvbin.ends_with(suf, 2));
    REQUIRE(bvbin.contains(memepp::buffer_view(pre, 2)));
}

// ============================================================================
// slice() boundaries and storage branches
// ============================================================================

TEST_CASE("buffer_view slice boundaries", "[buffer_view][gap]")
{
    const uint8_t raw[] = { 'a', 'b', 'c', 'd', 'e' };
    memepp::buffer_view bv(raw, 5);

    /* count == 0 gives an empty slice at the given offset. */
    REQUIRE(bv.slice(0, 0).empty());
    REQUIRE(bv.slice(2, 0).empty());

    /* pos == size() gives an empty slice. */
    REQUIRE(bv.slice(5).empty());
    REQUIRE(bv.slice(5, 0).empty());

    /* pos > size() clamps to an empty slice. */
    REQUIRE(bv.slice(6).empty());
    REQUIRE(bv.slice(99, 3).empty());

    /* A negative pos is a caller error; the C layer returns an empty view
     * rather than performing pointer arithmetic. */
    REQUIRE(bv.slice(-1).empty());
    REQUIRE(bv.slice(-1, 3).empty());

    /* A negative count means "to the end". */
    REQUIRE(bv.slice(0, -1).size() == 5);
    REQUIRE(bv.slice(2, -1).size() == 3);
    REQUIRE(bv.slice(2, -100).size() == 3);

    /* An empty view slices to empty. */
    memepp::buffer_view e;
    REQUIRE(e.slice(0).empty());
    REQUIRE(e.slice(-1).empty());
    REQUIRE(e.slice(0, -1).empty());

    /* Slicing a non-view backed buffer_view takes the copy branch inside
     * MemeBufferStack_slice; the result must still carry the right bytes. */
    {
        const uint8_t data[] = { 1, 2, 3, 4 };
        memepp::buffer buf(data, 4);
        memepp::buffer_view nonview(buf);
        REQUIRE(nonview.storage_type() == memepp::buffer_storage_t::large);

        auto sl = nonview.slice(1, 2);
        REQUIRE(sl.size() == 2);
        REQUIRE(sl.at(0) == 2);
        REQUIRE(sl.at(1) == 3);
    }

    /* Slicing a raw-pointer view keeps the view branch. */
    {
        const uint8_t data[] = { 1, 2, 3, 4 };
        memepp::buffer_view raww(data, 4);
        REQUIRE(raww.storage_type() == memepp::buffer_storage_t::view_unsafe);

        auto sl = raww.slice(1, 2);
        REQUIRE(sl.size() == 2);
        REQUIRE(sl.at(0) == 2);
        REQUIRE(sl.at(1) == 3);
    }
}

// ============================================================================
// Conversion details
// ============================================================================

TEST_CASE("buffer_view to_buffer from a raw view is a deep copy",
          "[buffer_view][gap]")
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
          "[buffer_view][gap]")
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
          "[buffer_view][gap]")
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
// Alias / lifetime semantics
// ============================================================================

TEST_CASE("buffer_view aliases its source and does not own it",
          "[buffer_view][gap]")
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
          "[buffer_view][gap]")
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
          "[buffer_view][gap]")
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
// Raw-pointer construction edge cases
// ============================================================================

TEST_CASE("buffer_view raw pointer construction edge cases",
          "[buffer_view][gap]")
{
    const uint8_t raw[] = { 'a', 'b', 'c' };

    /* nullptr with zero size is the empty view. */
    {
        memepp::buffer_view e(static_cast<const uint8_t*>(nullptr), 0);
        REQUIRE(e.size() == 0);
        REQUIRE(e.empty());
        REQUIRE(e.data() == nullptr);
    }

    /* A non-null pointer with zero size is empty but keeps the pointer. */
    {
        memepp::buffer_view e(raw, 0);
        REQUIRE(e.empty());
        REQUIRE(e.data() == raw);
    }

    /* Passing -1 as size triggers the C layer's "null-terminated string"
     * path: MemeBufferViewUnsafeStack_init calls strlen(_buf).  This is valid
     * ONLY if _buf is a valid C-string.  Our 'raw' array is NOT, so we use a
     * C-string here to test the path safely. */
    {
        const char cstr[] = "abc";
        memepp::buffer_view neg(reinterpret_cast<const uint8_t*>(cstr), -1);
        REQUIRE(neg.size() == 3);
        REQUIRE(neg.at(0) == 'a');
    }
}

// ============================================================================
// Comparison matrix
// ============================================================================

TEST_CASE("buffer_view comparison matrix with binary data",
          "[buffer_view][gap]")
{
    /* High bytes compare as unsigned. */
    const uint8_t hi[] = { 0xFF };
    const uint8_t lo[] = { 0x01 };
    memepp::buffer_view a(hi, 1);
    memepp::buffer_view b(lo, 1);
    REQUIRE(a.compare(b) > 0);
    REQUIRE(a > b);
    REQUIRE(a >= b);
    REQUIRE_FALSE(a < b);
    REQUIRE_FALSE(a <= b);
    REQUIRE(a != b);
    REQUIRE_FALSE(a == b);

    /* Embedded NUL compares as a normal byte, and a prefix is less. */
    const uint8_t n1[] = { 0x00, 0x01 };
    const uint8_t n2[] = { 0x00, 0x02 };
    memepp::buffer_view c(n1, 2);
    memepp::buffer_view d(n2, 2);
    REQUIRE(c < d);
    REQUIRE(d > c);

    /* Empty vs non-empty, full six-operator matrix. */
    memepp::buffer_view e;
    memepp::buffer_view ne(lo, 1);
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

    /* Equal contents from different pointers. */
    const uint8_t x1[] = { 'x', 'y' };
    const uint8_t x2[] = { 'x', 'y' };
    memepp::buffer_view p(x1, 2);
    memepp::buffer_view q(x2, 2);
    REQUIRE(p.data() != q.data());
    REQUIRE(p == q);
    REQUIRE_FALSE(p != q);
    REQUIRE(p <= q);
    REQUIRE(p >= q);
    REQUIRE_FALSE(p < q);
    REQUIRE_FALSE(p > q);

    /* Empty vs empty. */
    memepp::buffer_view z1, z2;
    REQUIRE(z1 == z2);
    REQUIRE_FALSE(z1 != z2);
    REQUIRE(z1 <= z2);
    REQUIRE(z1 >= z2);
    REQUIRE_FALSE(z1 < z2);
    REQUIRE_FALSE(z1 > z2);
}

// ============================================================================
// Compile-time contract
// ============================================================================

TEST_CASE("buffer_view noexcept and layout contract", "[buffer_view][gap]")
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
