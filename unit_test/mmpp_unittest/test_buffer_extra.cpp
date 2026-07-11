#include <catch2/catch.hpp>

#include <memepp/buffer.hpp>
#include <memepp/string.hpp>

// ============================================================================
// buffer: find(const buffer&, size_type _pos)
// ============================================================================

TEST_CASE("buffer find with buffer and offset", "[buffer][find]")
{
    const uint8_t hay[] = { 1, 2, 3, 1, 2, 3 };
    const uint8_t needle[] = { 2, 3 };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(6));
    memepp::buffer sub(needle, static_cast<memepp::buffer::size_type>(2));

    REQUIRE(haystack.find(sub, 0) == 1);
    REQUIRE(haystack.find(sub, 1) == 1);
    REQUIRE(haystack.find(sub, 2) == 4);
    REQUIRE(haystack.find(sub, 5) == memepp::buffer::npos);
}

TEST_CASE("buffer find with buffer not found", "[buffer][find]")
{
    const uint8_t hay[] = { 1, 2, 3 };
    const uint8_t needle[] = { 9, 9 };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(3));
    memepp::buffer sub(needle, static_cast<memepp::buffer::size_type>(2));

    REQUIRE(haystack.find(sub, 0) == memepp::buffer::npos);
}

TEST_CASE("buffer find with buffer empty needle", "[buffer][find]")
{
    const uint8_t hay[] = { 1, 2, 3 };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(3));
    memepp::buffer empty;

    // MemeBuffer_find returns npos for zero-length needle
    REQUIRE(haystack.find(empty, 0) == memepp::buffer::npos);
    REQUIRE(haystack.find(empty, 2) == memepp::buffer::npos);
}

// ============================================================================
// buffer: find(const_pointer, size_type _pos, size_type _count)
// ============================================================================

TEST_CASE("buffer find with pointer pos and count", "[buffer][find]")
{
    const uint8_t hay[] = { 10, 20, 30, 10, 20, 30 };
    const uint8_t needle[] = { 20, 30 };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(6));

    REQUIRE(haystack.find(needle, 0, 2) == 1);
    REQUIRE(haystack.find(needle, 2, 2) == 4);
    REQUIRE(haystack.find(needle, 5, 2) == memepp::buffer::npos);
}

TEST_CASE("buffer find with pointer zero count", "[buffer][find]")
{
    const uint8_t hay[] = { 1, 2, 3 };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(3));

    REQUIRE(haystack.find(hay, 0, 0) == memepp::buffer::npos);
    REQUIRE(haystack.find(hay, 2, 0) == memepp::buffer::npos);
}

// ============================================================================
// buffer: find(value_type _byte, size_type _pos)
// ============================================================================

TEST_CASE("buffer find byte with offset", "[buffer][find]")
{
    const uint8_t hay[] = { 0x10, 0x20, 0x30, 0x20, 0x40 };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(5));

    REQUIRE(haystack.find((uint8_t)0x20, 0) == 1);
    REQUIRE(haystack.find((uint8_t)0x20, 2) == 3);
    REQUIRE(haystack.find((uint8_t)0x10, 1) == memepp::buffer::npos);
}

TEST_CASE("buffer find byte not found", "[buffer][find]")
{
    const uint8_t hay[] = { 1, 2, 3 };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(3));

    REQUIRE(haystack.find((uint8_t)0xFF, 0) == memepp::buffer::npos);
}

// ============================================================================
// buffer: to_shared_storage()
// ============================================================================

TEST_CASE("buffer to_shared_storage — converts to large", "[buffer]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));

    auto shared = buf.to_shared_storage();
    REQUIRE(shared.size() == 3);
    REQUIRE(shared == buf);
    REQUIRE(shared.storage_type() == memepp::buffer_storage_t::large);
}

TEST_CASE("buffer to_shared_storage — large stays large", "[buffer]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                             17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                             31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(sizeof(data)));
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::large);

    auto shared = buf.to_shared_storage();
    REQUIRE(shared.size() == sizeof(data));
    REQUIRE(shared == buf);
    REQUIRE(shared.storage_type() == memepp::buffer_storage_t::large);
}

TEST_CASE("buffer to_shared_storage — empty", "[buffer]")
{
    memepp::buffer buf;
    auto shared = buf.to_shared_storage();
    REQUIRE(shared.empty());
    REQUIRE(shared.storage_type() == memepp::buffer_storage_t::large);
}

// ============================================================================
// buffer: native_handle()
// ============================================================================

TEST_CASE("buffer native_handle accessor", "[buffer]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));

    const auto& handle = buf.native_handle();
    (void)handle;
    REQUIRE(buf.size() == 3);
}
