#include <catch2/catch.hpp>

#include <memepp/buffer.hpp>
#include <meme/string.h>

#include <utility>
#include <cstring>

// ============================================================================
// 11. self-assignment
// ============================================================================

TEST_CASE("buffer self copy assignment", "[buffer][assignment]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));
    buf = buf;
    REQUIRE(buf.size() == 3);
    REQUIRE(buf.at(0) == 1);
    REQUIRE(buf.at(2) == 3);
}

TEST_CASE("buffer self move assignment", "[buffer][assignment]")
{
    const uint8_t data[] = { 10, 20, 30 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));
    buf = std::move(buf);
    // Self-move: swap with self, data unchanged
    REQUIRE(buf.size() == 3);
    REQUIRE(buf.at(0) == 10);
}

TEST_CASE("buffer move assignment — source state", "[buffer][assignment]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                             17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                             31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(sizeof(data)));
    memepp::buffer dst;
    dst = std::move(src);

    REQUIRE(dst.size() == sizeof(data));
    // After move, src should be empty (swapped with empty)
    REQUIRE(src.empty());
    REQUIRE(src.size() == 0);
}

TEST_CASE("buffer copy assignment — overwrites existing large", "[buffer][assignment]")
{
    const uint8_t d1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                           17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                           31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 };
    memepp::buffer src(d1, static_cast<memepp::buffer::size_type>(sizeof(d1)));
    memepp::buffer dst(src); // dst is also large

    const uint8_t d2[] = { 'A', 'B', 'C' };
    memepp::buffer small(d2, static_cast<memepp::buffer::size_type>(3));

    dst = small;
    REQUIRE(dst.size() == 3);
    REQUIRE(dst.at(0) == 'A');
    REQUIRE(dst.at(2) == 'C');
}

// ============================================================================
// 12. swap — edge cases
// ============================================================================

TEST_CASE("buffer self swap", "[buffer][swap]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));
    buf.swap(buf);
    REQUIRE(buf.size() == 3);
    REQUIRE(buf.at(0) == 1);
}

TEST_CASE("buffer empty ↔ non-empty swap", "[buffer][swap]")
{
    const uint8_t data[] = { 0xAA, 0xBB };
    memepp::buffer empty;
    memepp::buffer full(data, static_cast<memepp::buffer::size_type>(2));

    empty.swap(full);
    REQUIRE(empty.size() == 2);
    REQUIRE(empty.at(0) == 0xAA);
    REQUIRE(full.empty());
}

TEST_CASE("buffer empty ↔ empty swap", "[buffer][swap]")
{
    memepp::buffer a;
    memepp::buffer b;
    a.swap(b);
    REQUIRE(a.empty());
    REQUIRE(b.empty());
}

TEST_CASE("buffer small ↔ large swap", "[buffer][swap]")
{
    const uint8_t small_data[] = { 1, 2, 3 };
    const uint8_t large_data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                                   17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                                   31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 };

    memepp::buffer small(small_data, static_cast<memepp::buffer::size_type>(3));
    memepp::buffer large(large_data, static_cast<memepp::buffer::size_type>(sizeof(large_data)));

    auto small_st_before = small.storage_type();
    auto large_st_before = large.storage_type();

    small.swap(large);

    REQUIRE(small.size() == sizeof(large_data));
    REQUIRE(large.size() == 3);
    // Storage types also swap
    REQUIRE(small.storage_type() == large_st_before);
    REQUIRE(large.storage_type() == small_st_before);
}

// ============================================================================
// 12. storage-specific assignment and swap
// ============================================================================

namespace {
memepp::buffer make_small_buf(const uint8_t* _d, memepp::buffer::size_type _n)
{
    MemeBufferStack_t raw;
    MemeStringStack_initByU8bytes(reinterpret_cast<mmstrstk_t*>(&raw), MMSTR__OBJ_SIZE, _d, _n);
    return memepp::buffer(std::move(raw));
}
memepp::buffer make_medium_buf(const uint8_t* _d, memepp::buffer::size_type _n)
{
    MemeBufferStack_t raw;
    MemeBufferStack_initByU8bytesAndType(
        &raw, MMSTR__OBJ_SIZE, _d, _n, MemeBuffer_StorageType_medium);
    return memepp::buffer(std::move(raw));
}
} // namespace

TEST_CASE("buffer small move assignment", "[buffer][assignment]")
{
    const uint8_t d1[] = { 1, 2, 3 };
    const uint8_t d2[] = { 9, 9, 9, 9 };
    memepp::buffer a = make_small_buf(d1, 3);
    memepp::buffer b = make_small_buf(d2, 4);
    REQUIRE(a.storage_type() == memepp::buffer_storage_t::small);
    REQUIRE(b.storage_type() == memepp::buffer_storage_t::small);

    // buffer move-assignment is a swap: b ends up holding a's previous value.
    a = std::move(b);
    REQUIRE(a.size() == 4);
    REQUIRE(a.at(0) == 9);
    REQUIRE(a.storage_type() == memepp::buffer_storage_t::small);
    REQUIRE(b.size() == 3);
    REQUIRE(b.at(0) == 1);
}

TEST_CASE("buffer medium copy assignment", "[buffer][assignment]")
{
    const uint8_t d1[] = { 1, 2, 3 };
    const uint8_t d2[] = { 5, 6, 7, 8, 9, 10 };
    memepp::buffer a = make_medium_buf(d1, 3);
    memepp::buffer b = make_medium_buf(d2, 6);
    REQUIRE(b.storage_type() == memepp::buffer_storage_t::medium);

    a = b;
    REQUIRE(a.size() == 6);
    REQUIRE(a == b);
    REQUIRE(a.data() != b.data());   // deep copy, not shared
    REQUIRE(b.size() == 6);          // source unchanged
}

TEST_CASE("buffer medium move assignment", "[buffer][assignment]")
{
    const uint8_t d1[] = { 1, 2, 3 };
    const uint8_t d2[] = { 5, 6, 7, 8, 9, 10 };
    memepp::buffer a = make_medium_buf(d1, 3);
    memepp::buffer b = make_medium_buf(d2, 6);
    auto expected_len = b.size();

    // Move-assignment swaps: b holds a's previous 3-byte payload afterwards.
    a = std::move(b);
    REQUIRE(a.size() == expected_len);
    REQUIRE(a.at(0) == 5);
    REQUIRE(b.size() == 3);
    REQUIRE(b.at(0) == 1);
}

TEST_CASE("buffer small <-> medium swap", "[buffer][swap]")
{
    const uint8_t ds[] = { 1, 2, 3 };
    const uint8_t dm[] = { 10, 20, 30, 40, 50, 60 };
    memepp::buffer sm = make_small_buf(ds, 3);
    memepp::buffer md = make_medium_buf(dm, 6);
    REQUIRE(sm.storage_type() == memepp::buffer_storage_t::small);
    REQUIRE(md.storage_type() == memepp::buffer_storage_t::medium);

    sm.swap(md);
    REQUIRE(sm.size() == 6);
    REQUIRE(sm.storage_type() == memepp::buffer_storage_t::medium);
    REQUIRE(sm.at(0) == 10);
    REQUIRE(md.size() == 3);
    REQUIRE(md.storage_type() == memepp::buffer_storage_t::small);
    REQUIRE(md.at(0) == 1);
}

TEST_CASE("buffer medium <-> large swap", "[buffer][swap]")
{
    const uint8_t dm[] = { 1, 2, 3, 4, 5, 6 };
    const uint8_t dl[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                           17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                           31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 };
    memepp::buffer md = make_medium_buf(dm, 6);
    memepp::buffer lg(dl, static_cast<memepp::buffer::size_type>(sizeof(dl)));
    REQUIRE(lg.storage_type() == memepp::buffer_storage_t::large);

    md.swap(lg);
    REQUIRE(md.storage_type() == memepp::buffer_storage_t::large);
    REQUIRE(md.size() == sizeof(dl));
    REQUIRE(lg.storage_type() == memepp::buffer_storage_t::medium);
    REQUIRE(lg.size() == 6);
}

TEST_CASE("buffer view handle — copy and move keep the view alive", "[buffer][lifetime][view]")
{
    // A view-tier handle imported into buffer is deep-copied to an owned tier
    // (buffer(native_handle_type&&) branches on view and calls initByBytes).
    const uint8_t backing[] = { 1, 2, 3, 4, 5 };
    MemeBufferStack_t raw;
    MemeBufferStack_init(&raw, MMSTR__OBJ_SIZE);
    MemeStringViewUnsafeStack_init(
        reinterpret_cast<MemeStringStack_t*>(&raw), MMSTR__OBJ_SIZE, backing, 5);

    memepp::buffer owned(std::move(raw));
    // Imported view is copied out, so it survives the original backing array.
    REQUIRE(owned.size() == 5);
    REQUIRE(owned.at(0) == 1);

    // The imported view is already a large buffer; a copy is refcount-shared,
    // so the data pointer is identical.
    memepp::buffer copy = owned;
    REQUIRE(copy == owned);
    REQUIRE(copy.data() == owned.data());

    memepp::buffer moved = std::move(copy);
    REQUIRE(moved.size() == 5);
    REQUIRE(copy.empty());
}

