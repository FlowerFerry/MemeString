#include <catch2/catch.hpp>

#include <memepp/buffer.hpp>
#include <meme/string.h>
#include <memepp/errc.hpp>

#include <stdexcept>

// ============================================================================
// 13. at() / operator[] — error paths
// ============================================================================

TEST_CASE("buffer at(-1) — negative index", "[buffer][element]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(buf.at(-1), memepp::errc_except);
#else
    memepp::set_errc(0);
    REQUIRE(buf.at(-1) == 0);
    REQUIRE(memepp::get_errc() != 0);
#endif
}

TEST_CASE("buffer operator[](size) — at boundary", "[buffer][element]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(buf[3], memepp::errc_except);
#else
    memepp::set_errc(0);
    REQUIRE(buf[3] == 0);
    REQUIRE(memepp::get_errc() != 0);
#endif
}

TEST_CASE("buffer operator[](-1) — negative index", "[buffer][element]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(buf[-1], memepp::errc_except);
#else
    memepp::set_errc(0);
    REQUIRE(buf[-1] == 0);
    REQUIRE(memepp::get_errc() != 0);
#endif
}

TEST_CASE("buffer at — empty buffer", "[buffer][element]")
{
    memepp::buffer buf;
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(buf.at(0), memepp::errc_except);
#else
    memepp::set_errc(0);
    REQUIRE(buf.at(0) == 0);
    REQUIRE(memepp::get_errc() != 0);
#endif
}

TEST_CASE("buffer at — success clears errc", "[buffer][element]")
{
    const uint8_t data[] = { 42 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(1));
    memepp::set_errc(memepp::get_errc()); // preserve whatever
    volatile uint8_t v = buf.at(0);
    (void)v;
    REQUIRE(memepp::get_errc() == 0);
}

// ============================================================================
// 14. iterator — comprehensive
// ============================================================================

TEST_CASE("buffer iterators — begin/end", "[buffer][iterator]")
{
    const uint8_t data[] = { 0x01, 0x02, 0x03 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));

    int count = 0;
    for (auto it = buf.begin(); it != buf.end(); ++it, ++count)
        REQUIRE(*it == data[count]);
    REQUIRE(count == 3);
}

TEST_CASE("buffer iterators — empty buffer begin==end", "[buffer][iterator]")
{
    memepp::buffer buf;
    REQUIRE(buf.begin() == buf.end());
    REQUIRE(buf.cbegin() == buf.cend());
}

TEST_CASE("buffer iterators — distance", "[buffer][iterator]")
{
    const uint8_t data[] = { 0, 1, 2, 3, 4 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(5));

    auto d = buf.cend() - buf.cbegin();
    REQUIRE(d == 5);
}

TEST_CASE("buffer iterators — random access arithmetic", "[buffer][iterator]")
{
    const uint8_t data[] = { 0, 10, 20, 30, 40 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(5));

    auto it = buf.cbegin();
    REQUIRE(*(it + 2) == 20);
    REQUIRE(*(it + 4) == 40);
    REQUIRE(*(buf.cend() - 1) == 40);
    REQUIRE(*(buf.cend() - 3) == 20);
}

TEST_CASE("buffer iterators — operator[]", "[buffer][iterator]")
{
    const uint8_t data[] = { 100, 200, 255, 128, 64 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(5));

    auto it = buf.cbegin();
    REQUIRE(it[0] == 100);
    REQUIRE(it[2] == 255);
    REQUIRE(it[4] == 64);
}

TEST_CASE("buffer iterators — pre/post increment", "[buffer][iterator]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));

    auto it = buf.cbegin();
    auto pre  = ++it;
    REQUIRE(*pre == 2);
    REQUIRE(*it == 2);

    auto post = it++;
    REQUIRE(*post == 2);
    REQUIRE(*it == 3);
}

TEST_CASE("buffer iterators — pre/post decrement", "[buffer][iterator]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));

    auto it = buf.cend();
    auto pre = --it;
    REQUIRE(*pre == 3);

    auto post = it--;
    REQUIRE(*post == 3);
    REQUIRE(*it == 2);
}

TEST_CASE("buffer iterators — range-for", "[buffer][iterator]")
{
    const uint8_t data[] = { 10, 20, 30 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));

    int idx = 0;
    for (auto b : buf) {
        REQUIRE(b == data[idx]);
        ++idx;
    }
    REQUIRE(idx == 3);
}

// ============================================================================
// slice: view source and negative parameters
// ============================================================================

TEST_CASE("buffer slice from view-tier buffer", "[buffer][slice][view]")
{
    // A buffer whose underlying handle is view-tier: after importing through
    // buffer(native_handle_type&&) the buffer copies the view data into an
    // owned (large) tier. Slice of that buffer is a copy, not a view.
    const uint8_t backing[] = { 0, 10, 20, 30, 40, 50 };
    MemeBufferStack_t raw;
    MemeStringViewUnsafeStack_init(
        reinterpret_cast<MemeStringStack_t*>(&raw), MMSTR__OBJ_SIZE, backing, 6);

    memepp::buffer owned(std::move(raw));
    REQUIRE(owned.size() == 6);
    // After importing a view handle, buffer is large (deep copy).
    REQUIRE(owned.storage_type() == memepp::buffer_storage_t::large);

    auto sl = owned.slice(2, 3);
    REQUIRE(sl.size() == 3);
    REQUIRE(sl.at(0) == 20);
    REQUIRE(sl.at(2) == 40);
    // Slice of large is a deep copy, not a view.
    REQUIRE(sl.storage_type() != memepp::buffer_storage_t::view_unsafe);
}

TEST_CASE("buffer slice — negative pos yields empty", "[buffer][slice]")
{
    // MemeBufferStack_slice treats pos < 0 (or pos >= size) as "no range" and
    // returns an empty buffer; it does not clamp pos to 0.
    const uint8_t data[] = { 1, 2, 3, 4, 5 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(5));

    auto sl = buf.slice(static_cast<memepp::buffer::size_type>(-1), 3);
    REQUIRE(sl.empty());
    REQUIRE(sl.size() == 0);
}

TEST_CASE("buffer slice — negative count to end", "[buffer][slice]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(5));

    auto sl = buf.slice(2, static_cast<memepp::buffer::size_type>(-1));
    REQUIRE(sl.size() == 3);
    REQUIRE(sl.at(0) == 3);
    REQUIRE(sl.at(2) == 5);
}

