#include <catch2/catch.hpp>

#include <memepp/buffer.hpp>

#include <utility>

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
