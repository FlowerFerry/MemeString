#include <catch2/catch.hpp>

#include <memepp/buffer.hpp>

// ============================================================================
// 1. buffer(native_handle_type&&) — move from C handle
// ============================================================================

TEST_CASE("buffer native_handle move — large handle", "[buffer][native_handle]")
{
    MemeBufferStack_t raw;
    const uint8_t data[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE };
    MemeBufferStack_init(&raw, MMSTR__OBJ_SIZE);
    MemeBufferStack_initByBytes(&raw, MMSTR__OBJ_SIZE, data, 5);

    memepp::buffer buf(std::move(raw));
    REQUIRE(buf.size() == 5);
    REQUIRE(buf.at(0) == 0xAA);
    REQUIRE(buf.at(4) == 0xEE);
    MemeBufferStack_unInit(&raw, MMSTR__OBJ_SIZE); // should be safe no-op
}

TEST_CASE("buffer native_handle move — view handle copies", "[buffer][native_handle]")
{
    MemeBufferStack_t raw;
    const uint8_t data[] = { 0x11, 0x22, 0x33 };
    MemeBufferStack_init(&raw, MMSBLDR__OBJ_SIZE);
    // Create a small buffer, then copy it — small is not view,
    // so test view path explicitly
    MemeBufferStack_unInit(&raw, MMSBLDR__OBJ_SIZE);

    // Build a view-backed stack
    const uint8_t view_data[] = { 0x01, 0x02, 0x03, 0x04 };
    MemeBufferStack_init(&raw, MMSBLDR__OBJ_SIZE);
    // ViewUnsafeStack_init for buffer uses view type
    MemeStringViewUnsafeStack_init(
        reinterpret_cast<MemeStringStack_t*>(&raw), MMSTR__OBJ_SIZE,
        view_data, 4);

    // Move a view handle → should deep copy
    memepp::buffer buf(std::move(raw));
    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(0) == 0x01);
    REQUIRE(buf.at(3) == 0x04);

    // After move, raw should be uninitializable safely
    MemeBufferStack_unInit(&raw, MMSBLDR__OBJ_SIZE);
}

// ============================================================================
// 2. storage hint — small / medium / user
// ============================================================================

TEST_CASE("buffer construct with large hint — confirms large", "[buffer][storage]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3),
                       memepp::buffer_storage_t::large);
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::large);
    REQUIRE(buf.size() == 3);
}

TEST_CASE("buffer construct with user hint — user hint is not retained", "[buffer][storage]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3),
                       memepp::buffer_storage_t::user);
    // user hint via byte construction should not create user storage
    REQUIRE(buf.storage_type() != memepp::buffer_storage_t::user);
}

TEST_CASE("buffer empty with storage hints", "[buffer][storage]")
{
    memepp::buffer buf_s(static_cast<const uint8_t*>(nullptr), static_cast<memepp::buffer::size_type>(0), memepp::buffer_storage_t::small);
    memepp::buffer buf_l(static_cast<const uint8_t*>(nullptr), static_cast<memepp::buffer::size_type>(0), memepp::buffer_storage_t::large);
    REQUIRE(buf_s.empty());
    REQUIRE(buf_l.empty());
}

// ============================================================================
// 8. sub-range constructor — boundary cases
// ============================================================================

TEST_CASE("buffer sub-range — pos == size", "[buffer][constructors]")
{
    const uint8_t data[] = { 0, 1, 2, 3, 4 };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(5));

    memepp::buffer sub(src, 5);
    REQUIRE(sub.empty());
    REQUIRE(sub.size() == 0);
}

TEST_CASE("buffer sub-range — pos > size", "[buffer][constructors]")
{
    const uint8_t data[] = { 0, 1, 2 };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(3));

    memepp::buffer sub(src, 10);
    REQUIRE(sub.empty());
}

TEST_CASE("buffer sub-range — count == 0", "[buffer][constructors]")
{
    const uint8_t data[] = { 0, 1, 2, 3 };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(4));

    memepp::buffer sub(src, 1, 0);
    REQUIRE(sub.empty());
}

TEST_CASE("buffer sub-range — count > remaining clamps", "[buffer][constructors]")
{
    const uint8_t data[] = { 0, 1, 2, 3, 4 };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(5));

    // Must clamp to the 2 bytes available after pos=3, not over-read.
    memepp::buffer sub(src, 3, 100);
    REQUIRE(sub.size() == 2);
    REQUIRE(sub.at(0) == 3);
    REQUIRE(sub.at(1) == 4);
}

TEST_CASE("buffer sub-range — negative count clamps to remaining", "[buffer][constructors]")
{
    const uint8_t data[] = { 0, 1, 2, 3, 4 };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(5));

    memepp::buffer sub(src, 2, static_cast<memepp::buffer::size_type>(-1));
    REQUIRE(sub.size() == 3);
    REQUIRE(sub.at(0) == 2);
    REQUIRE(sub.at(2) == 4);
}

TEST_CASE("buffer sub-range — negative pos clamps to 0", "[buffer][constructors]")
{
    const uint8_t data[] = { 0, 1, 2, 3 };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(4));

    memepp::buffer sub(src, static_cast<memepp::buffer::size_type>(-1), 2);
    REQUIRE(sub.size() == 2);
    REQUIRE(sub.at(0) == 0);
    REQUIRE(sub.at(1) == 1);

    memepp::buffer sub2(src, static_cast<memepp::buffer::size_type>(-1));
    REQUIRE(sub2.size() == 4);
    REQUIRE(sub2.at(0) == 0);
}

TEST_CASE("buffer sub-range — empty source, both overloads", "[buffer][constructors]")
{
    memepp::buffer src;
    memepp::buffer sub(src, 0);
    REQUIRE(sub.empty());

    memepp::buffer sub2(src, 0, 5);
    REQUIRE(sub2.empty());
}

TEST_CASE("buffer sub-range — binary NUL data", "[buffer][constructors]")
{
    const uint8_t data[] = { 0x00, 0x01, 0x00, 0xFF };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(4));

    memepp::buffer sub(src, 1, 2);
    REQUIRE(sub.size() == 2);
    REQUIRE(sub.at(0) == 0x01);
    REQUIRE(sub.at(1) == 0x00);
}

// ============================================================================
// 9. range constructor — edge cases
// ============================================================================

TEST_CASE("buffer range constructor — nullptr nullptr", "[buffer][constructors]")
{
    const uint8_t* begin = nullptr;
    const uint8_t* end = nullptr;
    memepp::buffer buf(begin, end);
    REQUIRE(buf.empty());
    REQUIRE(buf.size() == 0);
}

TEST_CASE("buffer range constructor — single element range", "[buffer][constructors]")
{
    const uint8_t data[] = { 0xAB };
    memepp::buffer buf(data, data + 1);
    REQUIRE(buf.size() == 1);
    REQUIRE(buf.at(0) == 0xAB);
}

// ============================================================================
// 10. pointer+size constructor — null / negative size
// ============================================================================

TEST_CASE("buffer pointer+size — nullptr with 0", "[buffer][constructors]")
{
    memepp::buffer buf(static_cast<const uint8_t*>(nullptr), static_cast<memepp::buffer::size_type>(0));
    REQUIRE(buf.empty());
    REQUIRE(buf.size() == 0);
}

// ============================================================================
// 19a. C-level MemeBufferStack_initByBytes validation
// ============================================================================

TEST_CASE("MemeBufferStack_initByBytes — negative length yields empty buffer", "[buffer][capivalidation]")
{
    const uint8_t data[] = { 1, 2, 3 };
    MemeBufferStack_t raw;
    MemeBufferStack_init(&raw, MMSTR__OBJ_SIZE);

    // Negative length cannot describe bytes: C layer produces an empty,
    // fully-initialised buffer and returns success.
    int rc = MemeBufferStack_initByBytes(
        &raw, MMSTR__OBJ_SIZE, data, static_cast<MemeInteger_t>(-1));
    REQUIRE(rc == 0);
    REQUIRE(MemeBuffer_size(memepp::to_pointer(raw)) == 0);
    REQUIRE(MemeBuffer_isEmpty(memepp::to_pointer(raw)));
    MemeBufferStack_unInit(&raw, MMSTR__OBJ_SIZE);
}

TEST_CASE("MemeBufferStack_initByBytes — null ptr yields empty buffer", "[buffer][capivalidation]")
{
    MemeBufferStack_t raw;
    MemeBufferStack_init(&raw, MMSTR__OBJ_SIZE);

    int rc = MemeBufferStack_initByBytes(
        &raw, MMSTR__OBJ_SIZE, static_cast<const MemeByte_t*>(nullptr), 0);
    REQUIRE(rc == 0);
    REQUIRE(MemeBuffer_size(memepp::to_pointer(raw)) == 0);
    MemeBufferStack_unInit(&raw, MMSTR__OBJ_SIZE);

    // Null with a positive length is equally empty (nothing to copy).
    MemeBufferStack_init(&raw, MMSTR__OBJ_SIZE);
    rc = MemeBufferStack_initByBytes(
        &raw, MMSTR__OBJ_SIZE, static_cast<const MemeByte_t*>(nullptr), 4);
    REQUIRE(rc == 0);
    REQUIRE(MemeBuffer_size(memepp::to_pointer(raw)) == 0);
    MemeBufferStack_unInit(&raw, MMSTR__OBJ_SIZE);
}

TEST_CASE("MemeBufferStack_initByBytes — normal path still works", "[buffer][capivalidation]")
{
    const uint8_t data[] = { 0xAA, 0xBB, 0xCC };
    MemeBufferStack_t raw;
    MemeBufferStack_init(&raw, MMSTR__OBJ_SIZE);

    int rc = MemeBufferStack_initByBytes(&raw, MMSTR__OBJ_SIZE, data, 3);
    REQUIRE(rc == 0);
    REQUIRE(MemeBuffer_size(memepp::to_pointer(raw)) == 3);
    REQUIRE(MemeBuffer_data(memepp::to_pointer(raw))[0] == 0xAA);
    MemeBufferStack_unInit(&raw, MMSTR__OBJ_SIZE);
}

TEST_CASE("buffer negative count constructor is rejected safely", "[buffer][capivalidation]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(3));

    // The C layer treats a negative byte length as "empty" and returns a
    // valid empty buffer, so this must not crash or read a garbage length.
    memepp::buffer b(src.data(), static_cast<memepp::buffer::size_type>(-1));
    REQUIRE(b.empty());
    REQUIRE(b.size() == 0);
}
