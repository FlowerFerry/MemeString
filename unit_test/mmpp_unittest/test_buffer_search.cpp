#include <catch2/catch.hpp>

#include <memepp/buffer.hpp>

#include <utility>


TEST_CASE("buffer find (buffer) — negative pos", "[buffer][find]")
{
    const uint8_t hay[] = { 1, 2, 3, 1, 2, 3 };
    const uint8_t needle[] = { 2, 3 };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(6));
    memepp::buffer sub(needle, static_cast<memepp::buffer::size_type>(2));

    // negative pos → C layer clamps to 0
    REQUIRE(haystack.find(sub, static_cast<memepp::buffer::size_type>(-1)) == 1);
    REQUIRE(haystack.find(sub, 0) == 1);
}

TEST_CASE("buffer find (byte) — negative pos", "[buffer][find]")
{
    const uint8_t hay[] = { 0x10, 0x20, 0x30, 0x20 };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(4));

    REQUIRE(haystack.find((uint8_t)0x20, static_cast<memepp::buffer::size_type>(-1)) == 1);
    REQUIRE(haystack.find((uint8_t)0x20, 0) == 1);
}

TEST_CASE("buffer index_of — negative length", "[buffer][find]")
{
    const uint8_t hay[] = { 'H', 'e', 'l', 'l', 'o' };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(5));
    // -1 → strlen("Hello") = 5, so needle equals haystack → found at 0
    const uint8_t needle[] = { 'H', 'e', 'l', 'l', 'o', 0 };
    REQUIRE(haystack.index_of(needle, static_cast<memepp::buffer::size_type>(-1)) == 0);
}

TEST_CASE("buffer starts_with — negative count uses strlen", "[buffer][starts_ends]")
{
    // A negative count makes the C layer call strlen() on the needle, so the
    // needle must be a NUL-terminated C string.
    const uint8_t hay[] = { 'H', 'e', 'l', 'l', 'o' };
    memepp::buffer buf(hay, static_cast<memepp::buffer::size_type>(5));
    const char prefix[] = "Hel\0";  // NUL-terminated
    REQUIRE(buf.starts_with(reinterpret_cast<const uint8_t*>(prefix),
                           static_cast<memepp::buffer::size_type>(-1)));
    REQUIRE_FALSE(buf.starts_with(reinterpret_cast<const uint8_t*>("XYZ"),
                                 static_cast<memepp::buffer::size_type>(-1)));
}

TEST_CASE("buffer ends_with — negative count uses strlen", "[buffer][starts_ends]")
{
    const uint8_t hay[] = { 'H', 'e', 'l', 'l', 'o' };
    memepp::buffer buf(hay, static_cast<memepp::buffer::size_type>(5));
    REQUIRE(buf.ends_with(reinterpret_cast<const uint8_t*>("llo"),
                         static_cast<memepp::buffer::size_type>(-1)));
    REQUIRE_FALSE(buf.ends_with(reinterpret_cast<const uint8_t*>("XYZ"),
                               static_cast<memepp::buffer::size_type>(-1)));
}
// ============================================================================
// 15. find/index_of — boundary matrix
// ============================================================================
TEST_CASE("buffer find — nullptr needle with positive count", "[buffer][find]")
{
    const uint8_t hay[] = { 1, 2, 3 };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(3));
    // null pointer → the C search returns -1 (no match)
    REQUIRE(haystack.find(static_cast<const uint8_t*>(nullptr), 0, 2) == memepp::buffer::npos);
}

TEST_CASE("buffer find — negative count on non-empty haystack", "[buffer][find]")
{
    const uint8_t hay[] = { 'a', 'b', 'c', '\0', 'X' };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(5));
    // negative count → strlen("abc") = 3 → literal "abc" found at 0
    REQUIRE(haystack.find(hay, 0, static_cast<memepp::buffer::size_type>(-1)) == 0);
}

TEST_CASE("buffer index_of — nullptr with zero length", "[buffer][find]")
{
    const uint8_t hay[] = { 1, 2, 3 };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(3));
    // empty needle (null, 0) → index_of returns npos
    REQUIRE(haystack.index_of(static_cast<const uint8_t*>(nullptr), 0) == memepp::buffer::npos);
}

TEST_CASE("buffer empty buffer — all find overloads", "[buffer][find]")
{
    memepp::buffer empty;
    const uint8_t needle[] = { 1 };

    REQUIRE(empty.find(memepp::buffer(), 0) == memepp::buffer::npos);
    REQUIRE(empty.find(needle, 0, 1) == memepp::buffer::npos);
    REQUIRE(empty.find((uint8_t)1, 0) == memepp::buffer::npos);
    REQUIRE(empty.index_of(memepp::buffer()) == memepp::buffer::npos);
    REQUIRE(empty.index_of(needle, 1) == memepp::buffer::npos);
    REQUIRE(!empty.contains(needle, 1));
}

TEST_CASE("buffer large storage — search works", "[buffer][find]")
{
    uint8_t hay[64];
    for (int i = 0; i < 64; ++i) hay[i] = static_cast<uint8_t>(i);
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(64));
    REQUIRE(haystack.storage_type() == memepp::buffer_storage_t::large);

    const uint8_t needle[] = { 30, 31, 32 };
    REQUIRE(haystack.find(needle, 0, 3) == 30);
    REQUIRE(haystack.find((uint8_t)63, 0) == 63);
    REQUIRE(haystack.contains(needle, 3));
    REQUIRE(haystack.index_of(needle, 3) == 30);
}

TEST_CASE("buffer medium storage — search works", "[buffer][find]")
{
    uint8_t hay[30];
    for (int i = 0; i < 30; ++i) hay[i] = static_cast<uint8_t>(i + 1);
    MemeBufferStack_t raw;
    REQUIRE(MemeBufferStack_initByU8bytesAndType(
        &raw, MMSTR__OBJ_SIZE, hay, 30, MemeBuffer_StorageType_medium) == 0);
    memepp::buffer haystack(std::move(raw));
    REQUIRE(haystack.storage_type() == memepp::buffer_storage_t::medium);

    const uint8_t needle[] = { 10, 11 };
    REQUIRE(haystack.find(needle, 0, 2) == 9);
    REQUIRE(haystack.contains(needle, 2));
    REQUIRE(haystack.starts_with(hay, 2));
    REQUIRE(haystack.ends_with(hay + 28, 2));
}

TEST_CASE("buffer overlapping matches", "[buffer][find]")
{
    const uint8_t hay[] = { 'a', 'a', 'a', 'a' };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(4));
    const uint8_t needle[] = { 'a', 'a' };

    REQUIRE(haystack.find(needle, 0, 2) == 0);
    REQUIRE(haystack.find(needle, 1, 2) == 1);
    REQUIRE(haystack.find(needle, 2, 2) == 2);
    REQUIRE(haystack.find(needle, 3, 2) == memepp::buffer::npos);
}


TEST_CASE("buffer find — pos < 0 treated as 0", "[buffer][find]")
{
    const uint8_t hay[] = { 1, 2, 3, 1, 2, 3 };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(6));
    const uint8_t needle[] = { 2, 3 };

    // size_type is signed (mmint_t); the C layer clamps a negative offset to
    // 0, so find(.., -1, ..) must behave exactly like find(.., 0, ..).
    REQUIRE(haystack.find(needle, static_cast<memepp::buffer::size_type>(-1), 2) == 1);
    REQUIRE(haystack.find(needle, 0, 2) == 1);
}

TEST_CASE("buffer find — pos == size", "[buffer][find]")
{
    const uint8_t hay[] = { 1, 2, 3 };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(3));
    const uint8_t needle[] = { 1, 2 };

    REQUIRE(haystack.find(needle, 3, 2) == memepp::buffer::npos);
}

TEST_CASE("buffer find — pos > size", "[buffer][find]")
{
    const uint8_t hay[] = { 1, 2, 3 };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(3));
    const uint8_t needle[] = { 1, 2 };

    REQUIRE(haystack.find(needle, 10, 2) == memepp::buffer::npos);
}

TEST_CASE("buffer index_of — empty needle", "[buffer][find]")
{
    const uint8_t hay[] = { 1, 2, 3 };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(3));
    memepp::buffer empty;

    REQUIRE(haystack.index_of(empty) == memepp::buffer::npos);
}

TEST_CASE("buffer find — embedded NUL", "[buffer][find]")
{
    const uint8_t hay[] = { 0x01, 0x00, 0x02, 0x00, 0x03 };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(5));
    const uint8_t needle[] = { 0x00, 0x02 };

    REQUIRE(haystack.find(needle, 0, 2) == 1);
    // Resume at 2: remaining bytes {0x02,0x00,0x03} contain no 0x00,0x02 pair
    REQUIRE(haystack.find(needle, 2, 2) == memepp::buffer::npos);
}

TEST_CASE("buffer find — high byte data", "[buffer][find]")
{
    const uint8_t hay[] = { 0xFF, 0xFE, 0xFD, 0xFE, 0xFF };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(5));
    const uint8_t needle[] = { 0xFE, 0xFD };

    REQUIRE(haystack.find(needle, 0, 2) == 1);
    REQUIRE(haystack.find(needle, 2, 2) == memepp::buffer::npos);
}

TEST_CASE("buffer find byte — NUL byte", "[buffer][find]")
{
    const uint8_t hay[] = { 0x01, 0x00, 0x02, 0x00, 0x03 };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(5));

    REQUIRE(haystack.find((uint8_t)0x00, 0) == 1);
    REQUIRE(haystack.find((uint8_t)0x00, 2) == 3);
    REQUIRE(haystack.find((uint8_t)0x00, 4) == memepp::buffer::npos);
}

// ============================================================================
// 16. contains/starts_with/ends_with — pointer edge cases
// ============================================================================
TEST_CASE("buffer contains — nullptr with positive count is false", "[buffer][starts_ends]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));
    // C layer: null needle → len forced to 0 → empty needle → index_of
    // returns npos for empty needle → contains returns false
    REQUIRE(!buf.contains(static_cast<const uint8_t*>(nullptr), static_cast<memepp::buffer::size_type>(5)));
}

TEST_CASE("buffer starts_with — nullptr with positive count", "[buffer][starts_ends]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));
    // null needle → len forced to 0 → empty needle → starts_with returns 1
    REQUIRE(buf.starts_with(static_cast<const uint8_t*>(nullptr), static_cast<memepp::buffer::size_type>(5)));
}

TEST_CASE("buffer ends_with — nullptr with positive count", "[buffer][starts_ends]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));
    // null needle → len forced to 0 → empty needle → ends_with returns 1
    REQUIRE(buf.ends_with(static_cast<const uint8_t*>(nullptr), static_cast<memepp::buffer::size_type>(5)));
}

TEST_CASE("buffer contains — negative count is treated as strlen", "[buffer][starts_ends]")
{
    const uint8_t data[] = { 'H', 'e', 'l', 'l', 'o', '\0', 'X' };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(7));
    // A negative count triggers strlen on the haystack pointer — the needle
    // "Hello" (NUL-terminated) is found in the data at offset 0.
    REQUIRE(buf.contains(data, static_cast<memepp::buffer::size_type>(-1)));
}

TEST_CASE("buffer find — negative count treated as strlen", "[buffer][find]")
{
    const uint8_t data[] = { 't', 'e', 's', 't', '\0', 'X' };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(6));
    // find(pointer, pos=0, count=-1) → strlen("test")=4 → found at 0
    REQUIRE(buf.find(data, 0, static_cast<memepp::buffer::size_type>(-1)) == 0);
}


TEST_CASE("buffer contains — nullptr with 0 count", "[buffer][find]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));

    // nullptr with count 0 → empty needle → not found (C layer returns -1 for 0-length)
    REQUIRE(!buf.contains(static_cast<const uint8_t*>(nullptr), static_cast<memepp::buffer::size_type>(0)));
}

TEST_CASE("buffer starts_with — nullptr with 0 count", "[buffer][starts_ends]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));

    // nullptr with count 0 → empty needle → every string starts with ""
    REQUIRE(buf.starts_with(static_cast<const uint8_t*>(nullptr), static_cast<memepp::buffer::size_type>(0)));
}

TEST_CASE("buffer ends_with — nullptr with 0 count", "[buffer][starts_ends]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));

    REQUIRE(buf.ends_with(static_cast<const uint8_t*>(nullptr), static_cast<memepp::buffer::size_type>(0)));
}

TEST_CASE("buffer starts_with — needle equals haystack", "[buffer][starts_ends]")
{
    const uint8_t data[] = { 0xAA, 0xBB, 0xCC };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));

    REQUIRE(buf.starts_with(data, static_cast<memepp::buffer::size_type>(3)));
}

TEST_CASE("buffer ends_with — needle equals haystack", "[buffer][starts_ends]")
{
    const uint8_t data[] = { 0xAA, 0xBB, 0xCC };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));

    REQUIRE(buf.ends_with(data, static_cast<memepp::buffer::size_type>(3)));
}

TEST_CASE("buffer contains — needle longer than haystack", "[buffer][find]")
{
    const uint8_t data[] = { 1, 2 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(2));
    const uint8_t needle[] = { 1, 2, 3 };

    REQUIRE(!buf.contains(needle, static_cast<memepp::buffer::size_type>(3)));
}

TEST_CASE("buffer starts_with / ends_with — high byte needle", "[buffer][starts_ends]")
{
    const uint8_t hay[] = { 0xFF, 0xFE, 0x80, 0x00 };
    memepp::buffer buf(hay, static_cast<memepp::buffer::size_type>(4));

    const uint8_t prefix[] = { 0xFF, 0xFE };
    const uint8_t suffix[] = { 0x00 };

    REQUIRE(buf.starts_with(prefix, static_cast<memepp::buffer::size_type>(2)));
    REQUIRE(buf.ends_with(suffix, static_cast<memepp::buffer::size_type>(1)));
}
