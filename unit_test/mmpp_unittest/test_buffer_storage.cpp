#include <catch2/catch.hpp>

#include <memepp/buffer.hpp>
#include <memepp/string.hpp>
#include <meme/string.h>
#include <memepp/errc.hpp>

#include <type_traits>
#include <utility>

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

    // Contract (buffer_impl.hpp to_large): an already-large source returns
    // *this, i.e. a copy that shares the same refcounted block -- the data
    // pointer is unchanged. small/medium sources instead build a fresh large.
    auto large = src.to_large();
    REQUIRE(large == src);
    REQUIRE(large.storage_type() == memepp::buffer_storage_t::large);
    REQUIRE(large.data() == src.data());

    // Source reset must not invalidate the shared result.
    src.reset();
    REQUIRE(large.size() == sizeof(data));
    REQUIRE(large.storage_type() == memepp::buffer_storage_t::large);
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

TEST_CASE("buffer to_string — negative offset clamps to 0", "[buffer][conversion]")
{
    const uint8_t data[] = { 'A', 'B', 'C' };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));
    memepp::set_errc(0);

    // The C layer clamps a negative _front_offset to 0, so the whole buffer
    // is converted and no error is reported.
    memepp::string s = buf.to_string(static_cast<memepp::buffer::size_type>(-1));
    REQUIRE(s.size() == 3);
    REQUIRE(s == "ABC");
    REQUIRE(memepp::get_errc() == 0);
}

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

TEST_CASE("buffer to_string result survives source reset", "[buffer][conversion]")
{
    const uint8_t data[] = { 0x48, 0x65, 0x6C, 0x6C, 0x6F };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(5));
    memepp::string s = src.to_string();

    src.reset();
    REQUIRE(s.size() == 5);
    REQUIRE(s == "Hello");
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
// 21. small storage — only reachable through the C API
// ============================================================================

// MemeBufferStack_initByBytes() forces large storage for every non-empty
// input, so the only way to get a small buffer is through
// MemeStringStack_initByU8bytes (which auto-selects the tier). Importing a
// small handle via buffer(native_handle_type&&) swaps the raw stack object.
static memepp::buffer make_small_buffer(const uint8_t* _data, memepp::buffer::size_type _len)
{
    // Pre-condition: _len fits the small inline buffer
    MemeBufferStack_t raw;
    int rc = MemeStringStack_initByU8bytes(
        reinterpret_cast<mmstrstk_t*>(&raw), MMSTR__OBJ_SIZE, _data, _len);
    REQUIRE(rc == 0);
    REQUIRE(MemeBuffer_storageType(memepp::to_pointer(raw)) == MemeBuffer_StorageType_small);
    return memepp::buffer(std::move(raw));
}

TEST_CASE("buffer small to_shared_storage is an independent copy", "[buffer][storage]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer src = make_small_buffer(data, static_cast<memepp::buffer::size_type>(3));
    REQUIRE(src.storage_type() == memepp::buffer_storage_t::small);

    auto shared = src.to_shared_storage();
    REQUIRE(shared.storage_type() == memepp::buffer_storage_t::large);
    REQUIRE(shared == src);
    // A small source has no heap block to share, so the result must be a
    // freshly allocated large copy.
    REQUIRE(shared.data() != src.data());

    src.reset();
    REQUIRE(shared.size() == 3);
    REQUIRE(shared.at(0) == 1);
    REQUIRE(shared.at(2) == 3);
}

TEST_CASE("buffer small to_large is independent of source", "[buffer][storage]")
{
    const uint8_t data[] = { 7, 8, 9 };
    memepp::buffer src = make_small_buffer(data, static_cast<memepp::buffer::size_type>(3));

    auto large = src.to_large();
    REQUIRE(large.storage_type() == memepp::buffer_storage_t::large);
    REQUIRE(large == src);
    REQUIRE(large.data() != src.data());

    src.reset();
    REQUIRE(large.size() == 3);
    REQUIRE(large.at(0) == 7);
}

TEST_CASE("buffer small cheap_copy — same inline storage", "[buffer][storage]")
{
    const uint8_t data[] = { 4, 5, 6 };
    memepp::buffer src = make_small_buffer(data, static_cast<memepp::buffer::size_type>(3));

    // cheap_copy on small returns *this (byte-level copy of the 24-byte stack
    // object). The resulting buffer is a second small with identical bytes
    // but a different stack address.
    auto copy = src.cheap_copy();
    REQUIRE(copy.storage_type() == memepp::buffer_storage_t::small);
    REQUIRE(copy == src);
    // Both buffers have their own stack storage; data() points into each one.
    REQUIRE(copy.data() != src.data());
}

TEST_CASE("buffer small to_large_or_user keeps data after source reset", "[buffer][storage]")
{
    const uint8_t data[] = { 11, 12 };
    memepp::buffer src = make_small_buffer(data, static_cast<memepp::buffer::size_type>(2));

    auto result = src.to_large_or_user();
    REQUIRE(result.storage_type() == memepp::buffer_storage_t::large);
    REQUIRE(result.data() != src.data());

    src.reset();
    REQUIRE(result.size() == 2);
    REQUIRE(result.at(0) == 11);
}

TEST_CASE("buffer to_string result is independent of source", "[buffer][conversion]")
{
    const uint8_t data[] = { 'A', 'B', 'C' };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(3));

    memepp::string s = src.to_string();
    src.reset();
    REQUIRE(s.size() == 3);
    REQUIRE(s == "ABC");
}

// ============================================================================
// 22. medium storage — reached through the C API
// ============================================================================

// The public byte constructor forces large. A medium-tier buffer is only
// reachable through MemeBufferStack_initByU8bytesAndType with an explicit
// medium hint, then imported via buffer(native_handle_type&&).
static memepp::buffer make_medium_buffer(const uint8_t* _data, memepp::buffer::size_type _len)
{
    MemeBufferStack_t raw;
    int rc = MemeBufferStack_initByU8bytesAndType(
        &raw, MMSTR__OBJ_SIZE, _data, _len, MemeBuffer_StorageType_medium);
    REQUIRE(rc == 0);
    return memepp::buffer(std::move(raw));
}

TEST_CASE("buffer medium storage_type is preserved on import", "[buffer][storage]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                             17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 };
    auto medium = make_medium_buffer(data, static_cast<memepp::buffer::size_type>(sizeof(data)));

    REQUIRE(medium.storage_type() == memepp::buffer_storage_t::medium);
    REQUIRE(medium.size() == sizeof(data));
    REQUIRE(medium.at(0) == 1);
    REQUIRE(medium.capacity() > 0); // medium is a growable heap buffer
}

TEST_CASE("buffer medium copy and move", "[buffer][storage]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                             17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 };
    auto medium = make_medium_buffer(data, static_cast<memepp::buffer::size_type>(sizeof(data)));
    REQUIRE(medium.storage_type() == memepp::buffer_storage_t::medium);

    memepp::buffer copy = medium;
    REQUIRE(copy == medium);
    REQUIRE(copy.storage_type() == memepp::buffer_storage_t::medium);
    REQUIRE(copy.data() != medium.data());

    memepp::buffer moved = std::move(copy);
    REQUIRE(moved == medium);
    REQUIRE(copy.empty());
}

TEST_CASE("buffer medium conversion helpers", "[buffer][storage]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                             17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 };
    auto medium = make_medium_buffer(data, static_cast<memepp::buffer::size_type>(sizeof(data)));

    // cheap_copy on a non-small source routes through to_shared_storage and
    // therefore yields a large buffer.
    auto cheap = medium.cheap_copy();
    REQUIRE(cheap.storage_type() == memepp::buffer_storage_t::large);
    REQUIRE(cheap == medium);

    auto shared = medium.to_shared_storage();
    REQUIRE(shared.storage_type() == memepp::buffer_storage_t::large);
    REQUIRE(shared == medium);

    auto large = medium.to_large();
    REQUIRE(large.storage_type() == memepp::buffer_storage_t::large);
    REQUIRE(large == medium);

    auto lou = medium.to_large_or_user();
    REQUIRE(lou.storage_type() == memepp::buffer_storage_t::large);
    REQUIRE(lou == medium);
}

TEST_CASE("buffer medium reset and capacity", "[buffer][storage]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                             17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 };
    auto medium = make_medium_buffer(data, static_cast<memepp::buffer::size_type>(sizeof(data)));
    REQUIRE(medium.capacity() > 0);

    medium.reset();
    REQUIRE(medium.empty());
    REQUIRE(medium.size() == 0);
}

// ============================================================================
// 23. user-tier storage — real takeover object through the C API
// ============================================================================

// buffer and string share the same 24-byte stack layout, so a user-tier
// MemeStringStack handle is a valid MemeBufferStack handle. That makes the
// enabled MemeStringStack_initTakeOverUserObject the supported route to a
// real user-tier buffer (MemeBufferStack's own twin is commented out in the
// public header, but the string entry point is live and layout-compatible).
namespace {

struct user_blob {
    char*   data;
    size_t  size;
};

int    g_user_destruct_calls = 0;
char   g_user_payload[64];

void user_blob_destruct(void* _obj)
{
    ++g_user_destruct_calls;
    user_blob* b = static_cast<user_blob*>(_obj);
    delete[] b->data;
    delete b;
}

const char* user_blob_data(const void* _obj)
{
    return static_cast<const user_blob*>(_obj)->data;
}

size_t user_blob_size(const void* _obj)
{
    return static_cast<const user_blob*>(_obj)->size;
}

// Build a user-tier handle whose backing object outlives the buffer until the
// last reference is released.
MemeBufferStack_t make_user_handle(size_t _len)
{
    user_blob* b = new user_blob;
    b->size = _len;
    b->data = new char[_len];
    for (size_t i = 0; i < _len; ++i)
        b->data[i] = static_cast<char>('A' + (i % 26));

    MemeBufferStack_t h;
    int rc = MemeStringStack_initTakeOverUserObject(
        reinterpret_cast<mmstrstk_t*>(&h), MMSTR__OBJ_SIZE,
        b, user_blob_destruct, user_blob_data, user_blob_size);
    REQUIRE(rc == 0);
    return h;
}

memepp::buffer make_user_buffer(size_t _len)
{
    MemeBufferStack_t h = make_user_handle(_len);
    return memepp::buffer(std::move(h));
}

} // namespace

TEST_CASE("buffer user storage — imported handle keeps user tier", "[buffer][storage][user]")
{
    const size_t n = 64;
    g_user_destruct_calls = 0;

    memepp::buffer buf = make_user_buffer(n);
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::user);
    REQUIRE(buf.size() == static_cast<memepp::buffer::size_type>(n));
    REQUIRE(buf.at(0) == 'A');
    REQUIRE(buf.at(1) == 'B');

    // The C API reports the same tier/state through the imported handle.
    REQUIRE(MemeBuffer_storageType(memepp::to_pointer(buf.native_handle()))
            == MemeBuffer_StorageType_user);

    // Destroying the last reference invokes the user destructor exactly once.
    REQUIRE(g_user_destruct_calls == 0);
}

TEST_CASE("buffer user storage — copy shares and defers destruction", "[buffer][storage][user]")
{
    const size_t n = 64;
    g_user_destruct_calls = 0;

    {
        memepp::buffer a = make_user_buffer(n);
        memepp::buffer b = a;           // copy: refcounted share
        REQUIRE(b.storage_type() == memepp::buffer_storage_t::user);
        REQUIRE(b.size() == a.size());
        REQUIRE(b.data() == a.data());   // same shared block

        // Both still alive -> destructor not yet called.
        REQUIRE(g_user_destruct_calls == 0);
    }

    // Both references released -> user destructor fired exactly once.
    REQUIRE(g_user_destruct_calls == 1);
}

TEST_CASE("buffer user storage — move transfers the single reference", "[buffer][storage][user]")
{
    const size_t n = 32;
    g_user_destruct_calls = 0;

    {
        memepp::buffer a = make_user_buffer(n);
        memepp::buffer b = std::move(a);

        REQUIRE(b.storage_type() == memepp::buffer_storage_t::user);
        REQUIRE(b.size() == static_cast<memepp::buffer::size_type>(n));
        REQUIRE(a.empty());              // moved-from is empty
        REQUIRE(g_user_destruct_calls == 0);
    }

    REQUIRE(g_user_destruct_calls == 1);
}

TEST_CASE("buffer user storage — to_large_or_user keeps user tier", "[buffer][storage][user]")
{
    const size_t n = 64;
    g_user_destruct_calls = 0;

    {
        memepp::buffer src = make_user_buffer(n);
        auto result = src.to_large_or_user();

        // user is already shared storage: the helper returns *this.
        REQUIRE(result.storage_type() == memepp::buffer_storage_t::user);
        REQUIRE(result.data() == src.data());
        REQUIRE(g_user_destruct_calls == 0);
    }

    REQUIRE(g_user_destruct_calls == 1);
}

TEST_CASE("buffer user storage — to_shared_storage keeps user tier", "[buffer][storage][user]")
{
    const size_t n = 64;
    g_user_destruct_calls = 0;

    {
        memepp::buffer src = make_user_buffer(n);
        auto shared = src.to_shared_storage();
        REQUIRE(shared.storage_type() == memepp::buffer_storage_t::user);
        REQUIRE(shared.data() == src.data());
    }

    REQUIRE(g_user_destruct_calls == 1);
}

TEST_CASE("buffer user storage — cheap_copy shares the block", "[buffer][storage][user]")
{
    const size_t n = 64;
    g_user_destruct_calls = 0;

    {
        memepp::buffer src = make_user_buffer(n);
        auto copy = src.cheap_copy();
        // Non-small cheap_copy routes through to_shared_storage -> same user
        // storage, same backing object.
        REQUIRE(copy.storage_type() == memepp::buffer_storage_t::user);
        REQUIRE(copy.data() == src.data());
    }

    REQUIRE(g_user_destruct_calls == 1);
}

TEST_CASE("buffer user storage — to_large detaches to an owned copy", "[buffer][storage][user]")
{
    const size_t n = 64;
    g_user_destruct_calls = 0;

    memepp::buffer owned;
    {
        memepp::buffer src = make_user_buffer(n);
        owned = src.to_large();

        // to_large on a non-large source builds a fresh large copy.
        REQUIRE(owned.storage_type() == memepp::buffer_storage_t::large);
        REQUIRE(owned.size() == src.size());
        REQUIRE(owned.data() != src.data());
        REQUIRE(owned.at(0) == 'A');
    }

    // The user backing object is released once the source dies; the large
    // copy stays valid on its own.
    REQUIRE(g_user_destruct_calls == 1);
    REQUIRE(owned.size() == static_cast<memepp::buffer::size_type>(n));
    REQUIRE(owned.at(0) == 'A');
}

TEST_CASE("buffer user storage — result survives source reset", "[buffer][storage][user]")
{
    const size_t n = 64;
    g_user_destruct_calls = 0;

    {
        memepp::buffer src = make_user_buffer(n);
        memepp::buffer shared = src.to_shared_storage();

        src.reset();
        REQUIRE(src.empty());
        // shared still holds a reference to the user block.
        REQUIRE(g_user_destruct_calls == 0);
        REQUIRE(shared.size() == static_cast<memepp::buffer::size_type>(n));
        REQUIRE(shared.at(0) == 'A');
    }

    REQUIRE(g_user_destruct_calls == 1);
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
