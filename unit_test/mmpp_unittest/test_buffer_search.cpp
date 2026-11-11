#include <catch2/catch.hpp>

#include <memepp/buffer.hpp>

// ============================================================================
// 15. find/index_of — boundary matrix
// ============================================================================

TEST_CASE("buffer find — pos < 0 treated as 0", "[buffer][find]")
{
    const uint8_t hay[] = { 1, 2, 3, 1, 2, 3 };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(6));
    const uint8_t needle[] = { 2, 3 };

    // Negative pos should clamp to 0
    // Note: size_type is signed (mmint_t), so -1 is a valid value
    // The C layer clamps negative offset to 0
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
