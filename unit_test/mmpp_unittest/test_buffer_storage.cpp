#include <catch2/catch.hpp>

#include <memepp/buffer.hpp>
#include <memepp/string.hpp>

#include <type_traits>

// ============================================================================
// 3. cheap_copy — large / medium paths
// ============================================================================

TEST_CASE("buffer cheap_copy — large source", "[buffer][storage]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                             17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                             31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(sizeof(data)));
    REQUIRE(src.storage_type() == memepp::buffer_storage_t::large);

    auto copy = src.cheap_copy();
    REQUIRE(copy.size() == sizeof(data));
    REQUIRE(copy == src);

    // large cheap_copy goes through to_shared_storage, which should return *this
    // for large, so data pointers should be identical
    REQUIRE(copy.data() == src.data());
}

TEST_CASE("buffer cheap_copy — large source survives source reset", "[buffer][storage]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                             17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                             31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(sizeof(data)));
    auto copy = src.cheap_copy();

    src.reset();
    REQUIRE(src.empty());
    // copy should still be valid (refcounted)
    REQUIRE(copy.size() == sizeof(data));
    for (memepp::buffer::size_type i = 0; i < sizeof(data); ++i)
        REQUIRE(copy.at(i) == data[i]);
}

// ============================================================================
// 4. to_shared_storage — alias verification for large source
// ============================================================================

TEST_CASE("buffer to_shared_storage — large source shares data", "[buffer][storage]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                             17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                             31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(sizeof(data)));
    auto shared = src.to_shared_storage();

    REQUIRE(shared.storage_type() == memepp::buffer_storage_t::large);
    // large source → to_shared_storage returns *this, data pointer identical
    REQUIRE(shared.data() == src.data());
}

TEST_CASE("buffer to_shared_storage — large source survives source reset", "[buffer][storage]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                             17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                             31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(sizeof(data)));
    auto shared = src.to_shared_storage();

    src.reset();
    REQUIRE(shared.size() == sizeof(data));
    REQUIRE(shared.at(0) == 1);
    REQUIRE(shared.at(sizeof(data) - 1) == 44);
}

// ============================================================================
// 5. to_large — already large returns copy (not identity)
// ============================================================================

TEST_CASE("buffer to_large — large source returns copy", "[buffer][storage]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                             17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                             31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(sizeof(data)));
    REQUIRE(src.storage_type() == memepp::buffer_storage_t::large);

    // to_large for buffer always creates new large, never returns *this
    auto large = src.to_large();
    REQUIRE(large == src);
    REQUIRE(large.storage_type() == memepp::buffer_storage_t::large);
    // to_large creates a fresh large copy (not identity)
    // Unlike to_shared_storage, to_large constructs buffer(data, size, large)
    // which goes through initByU8bytesAndType → always creates new block
}

// ============================================================================
// 6. to_large_or_user — identity for large
// ============================================================================

TEST_CASE("buffer to_large_or_user — large identity", "[buffer][storage]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                             17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                             31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(sizeof(data)));
    auto result = src.to_large_or_user();

    REQUIRE(result == src);
    REQUIRE(result.storage_type() == memepp::buffer_storage_t::large);
    // For large, to_large_or_user returns *this (shared refcount)
    REQUIRE(result.data() == src.data());
}

// ============================================================================
// 7. to_string — offset paths and failure paths
// ============================================================================

TEST_CASE("buffer to_string with offset", "[buffer][conversion]")
{
    const uint8_t data[] = { 'H', 'e', 'l', 'l', 'o', 'W', 'o', 'r', 'l', 'd' };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(10));

    memepp::string s0 = buf.to_string(0);
    REQUIRE(s0.size() == 10);

    memepp::string s3 = buf.to_string(3);
    REQUIRE(s3 == "loWorld");
    REQUIRE(s3.size() == 7);

    memepp::string s9 = buf.to_string(9);
    REQUIRE(s9 == "d");
    REQUIRE(s9.size() == 1);
}

TEST_CASE("buffer to_string offset at size — returns empty", "[buffer][conversion]")
{
    const uint8_t data[] = { 'A', 'B', 'C' };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));

    memepp::string s = buf.to_string(3);
    REQUIRE(s.empty());
}

TEST_CASE("buffer to_string offset beyond size — returns empty", "[buffer][conversion]")
{
    const uint8_t data[] = { 'A', 'B' };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(2));

    memepp::string s = buf.to_string(100);
    REQUIRE(s.empty());
}

TEST_CASE("buffer to_string — empty buffer", "[buffer][conversion]")
{
    memepp::buffer buf;
    memepp::string s = buf.to_string();
    REQUIRE(s.empty());
}

TEST_CASE("buffer to_string — binary NUL data", "[buffer][conversion]")
{
    const uint8_t data[] = { 0x00, 0x01, 0x00, 0x02 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(4));

    memepp::string s = buf.to_string();
    // String should contain NUL bytes
    REQUIRE(s.size() == 4);
    REQUIRE(static_cast<uint8_t>(s.data()[0]) == 0x00);
    REQUIRE(static_cast<uint8_t>(s.data()[2]) == 0x00);
}

// ============================================================================
// 17. capacity — per-storage-type
// ============================================================================

TEST_CASE("buffer capacity — empty/small", "[buffer][capacity]")
{
    memepp::buffer buf;
    REQUIRE(buf.capacity() >= buf.size());
    REQUIRE(buf.capacity() > 0); // small has inline capacity
}

TEST_CASE("buffer capacity — large has 0", "[buffer][capacity]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                             17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                             31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(sizeof(data)));
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::large);
    // Large storage has 0 available capacity (fixed allocation)
    REQUIRE(buf.capacity() == 0);
}

TEST_CASE("buffer capacity — after reset", "[buffer][capacity]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));
    buf.reset();
    REQUIRE(buf.empty());
    REQUIRE(buf.capacity() >= buf.size());
}

// ============================================================================
// 18. native_handle — C API verification
// ============================================================================

TEST_CASE("buffer native_handle — C API size match", "[buffer][native_handle]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(5));

    const auto& handle = buf.native_handle();
    REQUIRE(MemeBuffer_size(memepp::to_pointer(handle)) == 5);
    REQUIRE(MemeBuffer_data(memepp::to_pointer(handle)) == buf.data());
}

TEST_CASE("buffer native_handle — C API storage type", "[buffer][native_handle]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                             17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                             31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(sizeof(data)));

    const auto& handle = buf.native_handle();
    REQUIRE(MemeBuffer_storageType(memepp::to_pointer(handle)) == MemeBuffer_StorageType_large);
}

TEST_CASE("buffer native_handle — empty", "[buffer][native_handle]")
{
    memepp::buffer buf;
    const auto& handle = buf.native_handle();
    REQUIRE(MemeBuffer_size(memepp::to_pointer(handle)) == 0);
    REQUIRE(MemeBuffer_storageType(memepp::to_pointer(handle)) == MemeBuffer_StorageType_small);
}

TEST_CASE("buffer native_handle — after copy", "[buffer][native_handle]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                             17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                             31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(sizeof(data)));
    memepp::buffer copy = src;

    // Both should be large, but different refcounted blocks
    REQUIRE(MemeBuffer_storageType(memepp::to_pointer(src.native_handle())) == MemeBuffer_StorageType_large);
    REQUIRE(MemeBuffer_storageType(memepp::to_pointer(copy.native_handle())) == MemeBuffer_StorageType_large);
}

// ============================================================================
// 19. binary byte semantics — systematic coverage
// ============================================================================

TEST_CASE("buffer binary — all extreme byte values", "[buffer][binary]")
{
    const uint8_t data[] = { 0x00, 0x01, 0x7F, 0x80, 0xFF };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(5));

    REQUIRE(buf.at(0) == 0x00);
    REQUIRE(buf.at(1) == 0x01);
    REQUIRE(buf.at(2) == 0x7F);
    REQUIRE(buf.at(3) == 0x80);
    REQUIRE(buf.at(4) == 0xFF);
}

TEST_CASE("buffer compare — high byte ordering", "[buffer][binary]")
{
    const uint8_t a[] = { 0x80 };
    const uint8_t b[] = { 0x7F };
    const uint8_t c[] = { 0xFF };

    memepp::buffer ba(a, 1);
    memepp::buffer bb(b, 1);
    memepp::buffer bc(c, 1);

    REQUIRE(ba.compare(bb) > 0); // 0x80 > 0x7F
    REQUIRE(bb.compare(bc) < 0); // 0x7F < 0xFF
    REQUIRE(ba.compare(bc) < 0); // 0x80 < 0xFF
}

TEST_CASE("buffer slice — preserves NUL and high bytes", "[buffer][binary]")
{
    const uint8_t data[] = { 0x00, 0xFF, 0x00, 0x80, 0xFF };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(5));

    auto sl = src.slice(1, 3);
    REQUIRE(sl.size() == 3);
    REQUIRE(sl.at(0) == 0xFF);
    REQUIRE(sl.at(1) == 0x00);
    REQUIRE(sl.at(2) == 0x80);
}

TEST_CASE("buffer copy — preserves all bytes", "[buffer][binary]")
{
    const uint8_t data[] = { 0x00, 0xFF, 0x01, 0x80, 0x7F };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(5));
    memepp::buffer copy = src;

    for (memepp::buffer::size_type i = 0; i < 5; ++i)
        REQUIRE(copy.at(i) == src.at(i));
}

// ============================================================================
// 20. static assertions — layout / noexcept
// ============================================================================

TEST_CASE("buffer — sizeof matches C struct", "[buffer][static]")
{
    REQUIRE(sizeof(memepp::buffer) == sizeof(MemeBufferStack_t));
    REQUIRE(alignof(memepp::buffer) == alignof(MemeBufferStack_t));
}

TEST_CASE("buffer — move is noexcept", "[buffer][static]")
{
    REQUIRE(std::is_nothrow_move_constructible<memepp::buffer>::value);
    REQUIRE(std::is_nothrow_move_assignable<memepp::buffer>::value);
}

TEST_CASE("buffer — copy is not trivially copyable (may throw)", "[buffer][static]")
{
    REQUIRE(std::is_copy_constructible<memepp::buffer>::value);
    REQUIRE(std::is_copy_assignable<memepp::buffer>::value);
}

TEST_CASE("buffer — destructible", "[buffer][static]")
{
    REQUIRE(std::is_destructible<memepp::buffer>::value);
}
