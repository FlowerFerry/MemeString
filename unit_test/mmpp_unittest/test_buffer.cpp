#include <catch2/catch.hpp>

#include <memepp/buffer.hpp>
#include <memepp/string.hpp>

// ---------------------------------------------------------------------------
// constructors
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer default constructor", "[buffer]")
{
    memepp::buffer buf;
    REQUIRE(buf.empty());
    REQUIRE(buf.size() == 0);
    REQUIRE(buf.data() != nullptr);
}

TEST_CASE("memepp::buffer construct from bytes", "[buffer]")
{
    const uint8_t data[] = { 'H', 'e', 'l', 'l', 'o' };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(5));
    REQUIRE(!buf.empty());
    REQUIRE(buf.size() == 5);
    REQUIRE(buf.at(0) == 'H');
    REQUIRE(buf.at(4) == 'o');
}

// TEST_CASE("memepp::buffer fill constructor", "[buffer]")
// {
//     memepp::buffer buf(static_cast<memepp::buffer::size_type>(4),
//                        static_cast<uint8_t>(0xAB));
//     REQUIRE(buf.size() == 4);
//     for (int i = 0; i < 4; ++i)
//         REQUIRE(buf.at(i) == static_cast<uint8_t>(0xAB));
// }

TEST_CASE("memepp::buffer copy constructor", "[buffer]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5 };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(5));
    memepp::buffer dst(src);
    REQUIRE(dst.size() == src.size());
    REQUIRE(dst == src);
}

TEST_CASE("memepp::buffer move constructor", "[buffer]")
{
    const uint8_t data[] = { 10, 20, 30 };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(3));
    memepp::buffer dst(std::move(src));
    REQUIRE(dst.size() == 3);
    REQUIRE(dst.at(0) == 10);
    REQUIRE(dst.at(2) == 30);
}

TEST_CASE("memepp::buffer construct from range", "[buffer]")
{
    const uint8_t data[] = { 10, 20, 30, 40, 50 };
    memepp::buffer buf(data, data + 5);
    REQUIRE(buf.size() == 5);
    REQUIRE(buf.at(0) == 10);
    REQUIRE(buf.at(2) == 30);
    REQUIRE(buf.at(4) == 50);
}

TEST_CASE("memepp::buffer construct from range - all bytes copied correctly", "[buffer]")
{
    const uint8_t data[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
    memepp::buffer buf(data, data + 8);
    REQUIRE(buf.size() == 8);
    for (memepp::buffer::size_type i = 0; i < 8; ++i)
        REQUIRE(buf.at(i) == data[i]);
}

TEST_CASE("memepp::buffer construct from range - empty range", "[buffer]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, data); // _begin == _end
    REQUIRE(buf.empty());
    REQUIRE(buf.size() == 0);
}

TEST_CASE("memepp::buffer construct from range - single element", "[buffer]")
{
    const uint8_t data[] = { 0xAB };
    memepp::buffer buf(data, data + 1);
    REQUIRE(buf.size() == 1);
    REQUIRE(buf.at(0) == 0xAB);
}

TEST_CASE("memepp::buffer copy sub-range constructor", "[buffer]")
{
    const uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6 };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(7));

    // pos only
    memepp::buffer b1(src, 2);
    REQUIRE(b1.size() == 5);
    REQUIRE(b1.at(0) == 2);

    // pos + count
    memepp::buffer b2(src, 2, 3);
    REQUIRE(b2.size() == 3);
    REQUIRE(b2.at(0) == 2);
    REQUIRE(b2.at(2) == 4);
}

// ---------------------------------------------------------------------------
// assign
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer copy assignment", "[buffer]")
{
    const uint8_t data[] = { 7, 8, 9 };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(3));
    memepp::buffer dst;
    dst = src;
    REQUIRE(dst == src);
}

TEST_CASE("memepp::buffer move assignment", "[buffer]")
{
    const uint8_t data[] = { 11, 22 };
    memepp::buffer src(data, static_cast<memepp::buffer::size_type>(2));
    memepp::buffer dst;
    dst = std::move(src);
    REQUIRE(dst.size() == 2);
    REQUIRE(dst.at(0) == 11);
}

// ---------------------------------------------------------------------------
// element access
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer data pointer matches at()", "[buffer]")
{
    const uint8_t data[] = { 0xDE, 0xAD, 0xBE, 0xEF };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(4));
    const uint8_t* ptr = buf.data();
    REQUIRE(ptr[0] == 0xDE);
    REQUIRE(ptr[3] == 0xEF);
    REQUIRE(ptr[1] == buf.at(1));
}

// ---------------------------------------------------------------------------
// iterators
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer iterators cover all elements", "[buffer]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(5));

    int idx = 0;
    for (auto it = buf.cbegin(); it != buf.cend(); ++it, ++idx)
        REQUIRE(*it == data[idx]);
    REQUIRE(idx == 5);
}

// ---------------------------------------------------------------------------
// index_of / contains
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer index_of", "[buffer]")
{
    const uint8_t hay[]    = { 0, 1, 2, 3, 4, 5, 6 };
    const uint8_t needle[] = { 3, 4, 5 };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(7));
    memepp::buffer sub(needle, static_cast<memepp::buffer::size_type>(3));

    REQUIRE(haystack.index_of(sub) == 3);
    REQUIRE(haystack.index_of(needle, static_cast<memepp::buffer::size_type>(3)) == 3);

    const uint8_t absent[] = { 9, 9 };
    REQUIRE(haystack.index_of(absent,
        static_cast<memepp::buffer::size_type>(2)) == memepp::buffer::npos);
    REQUIRE(haystack.index_of(memepp::buffer()) == memepp::buffer::npos);
}

TEST_CASE("memepp::buffer contains", "[buffer]")
{
    const uint8_t data[]   = { 0xAA, 0xBB, 0xCC, 0xDD };
    const uint8_t needle[] = { 0xBB, 0xCC };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(4));
    memepp::buffer sub(needle, static_cast<memepp::buffer::size_type>(2));

    REQUIRE(buf.contains(sub));
    REQUIRE(buf.contains(needle, static_cast<memepp::buffer::size_type>(2)));

    const uint8_t absent[] = { 0xFF };
    REQUIRE(!buf.contains(absent, static_cast<memepp::buffer::size_type>(1)));
}

// ---------------------------------------------------------------------------
// reset / swap / to_large
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer reset", "[buffer]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));
    REQUIRE(!buf.empty());
    buf.reset();
    REQUIRE(buf.empty());
    REQUIRE(buf.size() == 0);
}

TEST_CASE("memepp::buffer swap", "[buffer]")
{
    const uint8_t d1[] = { 1, 2, 3 };
    const uint8_t d2[] = { 4, 5 };
    memepp::buffer a(d1, static_cast<memepp::buffer::size_type>(3));
    memepp::buffer b(d2, static_cast<memepp::buffer::size_type>(2));

    a.swap(b);
    REQUIRE(a.size() == 2);
    REQUIRE(b.size() == 3);
    REQUIRE(a.at(0) == 4);
    REQUIRE(b.at(0) == 1);
}

TEST_CASE("memepp::buffer to_large", "[buffer]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));

    memepp::buffer large = buf.to_large();
    REQUIRE(large.size() == 3);
    REQUIRE(large == buf);
    REQUIRE(large.storage_type() == memepp::buffer_storage_t::large);
}

// ---------------------------------------------------------------------------
// equality operators
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer operator== and operator!=", "[buffer]")
{
    const uint8_t d1[] = { 1, 2, 3 };
    const uint8_t d2[] = { 1, 2, 3 };
    const uint8_t d3[] = { 4, 5, 6 };

    memepp::buffer a(d1, static_cast<memepp::buffer::size_type>(3));
    memepp::buffer b(d2, static_cast<memepp::buffer::size_type>(3));
    memepp::buffer c(d3, static_cast<memepp::buffer::size_type>(3));

    REQUIRE(a == b);
    REQUIRE(a != c);
    REQUIRE(!(a != b));
    REQUIRE(!(a == c));
}

TEST_CASE("memepp::buffer empty buffers are equal", "[buffer]")
{
    memepp::buffer a;
    memepp::buffer b;
    REQUIRE(a == b);
}

// ---------------------------------------------------------------------------
// to_string
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer to_string 2", "[buffer]")
{
    const uint8_t data[] = { 'H', 'i' };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(2));
    memepp::string s = buf.to_string();
    REQUIRE(s == "Hi");
    REQUIRE(s.size() == 2);
}

// ---------------------------------------------------------------------------
// starts_with / ends_with
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer starts_with buffer", "[buffer]")
{
    const uint8_t raw[] = { 0x01, 0x02, 0x03, 0x04 };
    memepp::buffer buf(raw, static_cast<memepp::buffer::size_type>(4));
    const uint8_t prefix[] = { 0x01, 0x02 };
    memepp::buffer pf(prefix, static_cast<memepp::buffer::size_type>(2));
    REQUIRE(buf.starts_with(pf));
    const uint8_t not_prefix[] = { 0x02, 0x03 };
    memepp::buffer np(not_prefix, static_cast<memepp::buffer::size_type>(2));
    REQUIRE_FALSE(buf.starts_with(np));
}

TEST_CASE("memepp::buffer starts_with bytes", "[buffer]")
{
    const uint8_t raw[] = { 0xAB, 0xCD, 0xEF };
    memepp::buffer buf(raw, static_cast<memepp::buffer::size_type>(3));
    REQUIRE(buf.starts_with(raw, static_cast<memepp::buffer::size_type>(1)));
    REQUIRE_FALSE(buf.starts_with(raw + 1, static_cast<memepp::buffer::size_type>(2)));
    REQUIRE(buf.starts_with(raw, static_cast<memepp::buffer::size_type>(0)));
    REQUIRE_FALSE(buf.starts_with(raw, static_cast<memepp::buffer::size_type>(10)));
}

TEST_CASE("memepp::buffer ends_with buffer", "[buffer]")
{
    const uint8_t raw[] = { 0x01, 0x02, 0x03, 0x04 };
    memepp::buffer buf(raw, static_cast<memepp::buffer::size_type>(4));
    const uint8_t suffix[] = { 0x03, 0x04 };
    memepp::buffer sf(suffix, static_cast<memepp::buffer::size_type>(2));
    REQUIRE(buf.ends_with(sf));
    const uint8_t not_suffix[] = { 0x01, 0x02 };
    memepp::buffer ns(not_suffix, static_cast<memepp::buffer::size_type>(2));
    REQUIRE_FALSE(buf.ends_with(ns));
}

TEST_CASE("memepp::buffer ends_with bytes", "[buffer]")
{
    const uint8_t raw[] = { 0xAB, 0xCD, 0xEF };
    memepp::buffer buf(raw, static_cast<memepp::buffer::size_type>(3));
    REQUIRE(buf.ends_with(raw + 2, static_cast<memepp::buffer::size_type>(1)));
    REQUIRE_FALSE(buf.ends_with(raw, static_cast<memepp::buffer::size_type>(2)));
    REQUIRE(buf.ends_with(raw, static_cast<memepp::buffer::size_type>(0)));
    REQUIRE_FALSE(buf.ends_with(raw, static_cast<memepp::buffer::size_type>(10)));
}

TEST_CASE("memepp::buffer starts_with ends_with empty buffer", "[buffer]")
{
    memepp::buffer empty;
    const uint8_t data[] = { 0x01 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(1));

    REQUIRE(empty.starts_with(empty));
    REQUIRE(empty.ends_with(empty));
    REQUIRE_FALSE(empty.starts_with(buf));
    REQUIRE_FALSE(empty.ends_with(buf));
    REQUIRE(buf.starts_with(empty));
    REQUIRE(buf.ends_with(empty));
}

// ---------------------------------------------------------------------------
// buffer: storage_type
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer storage_type — empty", "[buffer]")
{
    memepp::buffer buf;
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::small);
}

// ---------------------------------------------------------------------------
// buffer: capacity
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer capacity", "[buffer]")
{
    memepp::buffer buf;
    REQUIRE(buf.capacity() >= buf.size());
}

// ---------------------------------------------------------------------------
// buffer: cheap_copy
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer cheap_copy", "[buffer]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));
    auto copy = buf.cheap_copy();
    REQUIRE(copy.size() == 3);
    REQUIRE(copy == buf);
}

TEST_CASE("memepp::buffer cheap_copy — empty", "[buffer]")
{
    memepp::buffer buf;
    auto copy = buf.cheap_copy();
    REQUIRE(copy.empty());
}

// ---------------------------------------------------------------------------
// buffer: to_large_or_user
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer to_large_or_user — small", "[buffer]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));
    auto result = buf.to_large_or_user();
    REQUIRE(result == buf);
    REQUIRE(result.empty() == false);
}

TEST_CASE("memepp::buffer to_large_or_user — empty", "[buffer]")
{
    memepp::buffer buf;
    auto result = buf.to_large_or_user();
    REQUIRE(result.empty());
}

TEST_CASE("memepp::buffer to_large_or_user — large", "[buffer]")
{
    const uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                             17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                             31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(sizeof(data)));
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::large);
    auto result = buf.to_large_or_user();
    REQUIRE(result == buf);
    REQUIRE(result.storage_type() == memepp::buffer_storage_t::large);
}

// ---------------------------------------------------------------------------
// buffer: to_large extra
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer to_large — empty", "[buffer]")
{
    memepp::buffer buf;
    auto large = buf.to_large();
    REQUIRE(large.empty());
    REQUIRE(large.storage_type() == memepp::buffer_storage_t::large);
}

TEST_CASE("memepp::buffer to_large — small", "[buffer]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));
    auto large = buf.to_large();
    REQUIRE(large.size() == 3);
    REQUIRE(large.storage_type() == memepp::buffer_storage_t::large);
}

// ---------------------------------------------------------------------------
// buffer: reset — already empty
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer reset — already empty", "[buffer]")
{
    memepp::buffer buf;
    buf.reset();
    REQUIRE(buf.empty());
}

// ---------------------------------------------------------------------------
// buffer: construct with storage hint
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer construct with storage hint — large", "[buffer]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3),
                       memepp::buffer_storage_t::large);
    REQUIRE(buf.size() == 3);
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::large);
}

// ---------------------------------------------------------------------------
// buffer: index_of — not found
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer index_of — not found", "[buffer]")
{
    const uint8_t hay[] = { 1, 2, 3, 4, 5 };
    memepp::buffer haystack(hay, static_cast<memepp::buffer::size_type>(5));
    memepp::buffer empty;
    REQUIRE(haystack.index_of(empty) == memepp::buffer::npos);
}
