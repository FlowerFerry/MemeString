#include <catch2/catch.hpp>

#include <memepp/buffer_span.hpp>
#include <memepp/buffer.hpp>
#include <memepp/variable_buffer.hpp>
#include <memepp/buffer_view.hpp>
#include <memepp/string_span.hpp>
#include <memepp/string.hpp>
#include <memepp/errc.hpp>

// ============================================================================
//  member types & npos
// ============================================================================

TEST_CASE("buffer_span member types", "[buffer_span]")
{
    using bs = memepp::buffer_span;
    static_assert(sizeof(bs::value_type) == 1, "");
    static_assert(sizeof(bs::size_type) >= 4, "");
    static_assert(sizeof(bs::difference_type) >= 4, "");
    SUCCEED();
}

TEST_CASE("buffer_span npos", "[buffer_span]")
{
    REQUIRE(memepp::buffer_span::npos == static_cast<memepp::buffer_span::size_type>(-1));
}

// ============================================================================
//  constructors
// ============================================================================

TEST_CASE("buffer_span default constructor", "[buffer_span]")
{
    memepp::buffer_span bs;
    REQUIRE(bs.empty());
    REQUIRE(bs.size() == 0);
    REQUIRE(bs.data() == nullptr);
}

TEST_CASE("buffer_span constructor from pointer+size", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    memepp::buffer_span bs(raw, 5);
    REQUIRE(bs.size() == 5);
    REQUIRE(bs[0] == 0x01);
    REQUIRE(bs[4] == 0x05);
    REQUIRE(!bs.empty());
}

TEST_CASE("buffer_span constructor from pointer+size zero", "[buffer_span]")
{
    const uint8_t raw[] = {0x01};
    memepp::buffer_span bs(raw, 0);
    REQUIRE(bs.size() == 0);
    REQUIRE(bs.empty());
}

TEST_CASE("buffer_span constructor from buffer", "[buffer_span]")
{
    const uint8_t raw[] = {0xAA, 0xBB, 0xCC};
    memepp::buffer buf(raw, 3);
    memepp::buffer_span bs(buf);
    REQUIRE(bs.size() == 3);
    REQUIRE(bs[0] == 0xAA);
    REQUIRE(bs[2] == 0xCC);
    REQUIRE(memepp::buffer_span(raw, 3) == bs);
}

TEST_CASE("buffer_span constructor from variable_buffer", "[buffer_span]")
{
    const uint8_t raw[] = {0x10, 0x20, 0x30, 0x40};
    memepp::variable_buffer vbuf(raw, 4);
    memepp::buffer_span bs(vbuf);
    REQUIRE(bs.size() == 4);
    REQUIRE(bs[0] == 0x10);
    REQUIRE(bs[3] == 0x40);
    REQUIRE(memepp::buffer_span(raw, 4) == bs);
}

TEST_CASE("buffer_span constructor from buffer_view", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03};
    memepp::buffer_view bv(raw, 3);
    memepp::buffer_span bs(bv);
    REQUIRE(bs.size() == 3);
    REQUIRE(bs[1] == 0x02);
    REQUIRE(memepp::buffer_span(raw, 3) == bs);
}

TEST_CASE("buffer_span constructor from string_span", "[buffer_span]")
{
    memepp::string_span sv("hello", 5);
    memepp::buffer_span bs(sv);
    REQUIRE(bs.size() == 5);
    REQUIRE(bs[0] == 'h');
    REQUIRE(bs[4] == 'o');
}

TEST_CASE("buffer_span copy constructor", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03};
    memepp::buffer_span a(raw, 3);
    memepp::buffer_span b(a);
    REQUIRE(b == a);
    REQUIRE(b.data() == a.data());  // same underlying pointer
}

TEST_CASE("buffer_span copy assignment", "[buffer_span]")
{
    const uint8_t raw1[] = {0x01, 0x02};
    const uint8_t raw2[] = {0x03, 0x04, 0x05};
    memepp::buffer_span a(raw1, 2);
    memepp::buffer_span b(raw2, 3);
    b = a;
    REQUIRE(b == a);
    REQUIRE(b.size() == 2);
}

// ============================================================================
//  element access
// ============================================================================

TEST_CASE("buffer_span operator[]", "[buffer_span]")
{
    const uint8_t raw[] = {0x0A, 0x0B, 0x0C};
    memepp::buffer_span bs(raw, 3);
    REQUIRE(bs[0] == 0x0A);
    REQUIRE(bs[1] == 0x0B);
    REQUIRE(bs[2] == 0x0C);
}

TEST_CASE("buffer_span at", "[buffer_span]")
{
    const uint8_t raw[] = {0x10, 0x20, 0x30, 0x40};
    memepp::buffer_span bs(raw, 4);
    REQUIRE(bs.at(0) == 0x10);
    REQUIRE(bs.at(3) == 0x40);
}

TEST_CASE("buffer_span at out_of_range", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02};
    memepp::buffer_span bs(raw, 2);
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(bs.at(2), memepp::errc_except);
    REQUIRE_THROWS_AS(bs.at(-1), memepp::errc_except);
#else
    memepp::set_errc(0);
    REQUIRE(bs.at(2) == 0);
    REQUIRE(memepp::get_errc() != 0);
    memepp::set_errc(0);
    REQUIRE(bs.at(-1) == 0);
    REQUIRE(memepp::get_errc() != 0);
#endif
}

TEST_CASE("buffer_span front", "[buffer_span]")
{
    const uint8_t raw[] = {0xAB, 0xCD};
    memepp::buffer_span bs(raw, 2);
    REQUIRE(bs.front() == 0xAB);
}

TEST_CASE("buffer_span back", "[buffer_span]")
{
    const uint8_t raw[] = {0xAB, 0xCD};
    memepp::buffer_span bs(raw, 2);
    REQUIRE(bs.back() == 0xCD);
}

TEST_CASE("buffer_span data", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03};
    memepp::buffer_span bs(raw, 3);
    REQUIRE(std::memcmp(bs.data(), raw, 3) == 0);
}

// ============================================================================
//  capacity
// ============================================================================

TEST_CASE("buffer_span size", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    memepp::buffer_span bs(raw, 5);
    REQUIRE(bs.size() == 5);
}

TEST_CASE("buffer_span empty", "[buffer_span]")
{
    REQUIRE(memepp::buffer_span().empty());
    const uint8_t raw[] = {0x01};
    REQUIRE_FALSE(memepp::buffer_span(raw, 1).empty());
}

// ============================================================================
//  modifiers
// ============================================================================

TEST_CASE("buffer_span remove_prefix", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    memepp::buffer_span bs(raw, 5);
    bs.remove_prefix(2);
    REQUIRE(bs.size() == 3);
    REQUIRE(bs[0] == 0x03);
    REQUIRE(bs[2] == 0x05);
}

TEST_CASE("buffer_span remove_prefix zero", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03};
    memepp::buffer_span bs(raw, 3);
    bs.remove_prefix(0);
    REQUIRE(bs.size() == 3);
    REQUIRE(bs[0] == 0x01);
}

TEST_CASE("buffer_span remove_suffix", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    memepp::buffer_span bs(raw, 5);
    bs.remove_suffix(2);
    REQUIRE(bs.size() == 3);
    REQUIRE(bs.back() == 0x03);
}

TEST_CASE("buffer_span remove_suffix zero", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03};
    memepp::buffer_span bs(raw, 3);
    bs.remove_suffix(0);
    REQUIRE(bs.size() == 3);
    REQUIRE(bs[2] == 0x03);
}

TEST_CASE("buffer_span swap", "[buffer_span]")
{
    const uint8_t rawA[] = {0xAA, 0xAA, 0xAA};
    const uint8_t rawB[] = {0xBB, 0xBB};
    memepp::buffer_span a(rawA, 3);
    memepp::buffer_span b(rawB, 2);
    a.swap(b);
    REQUIRE(a.size() == 2);
    REQUIRE(b.size() == 3);
    REQUIRE(a[0] == 0xBB);
    REQUIRE(b[0] == 0xAA);
}

TEST_CASE("buffer_span swap self", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03};
    memepp::buffer_span a(raw, 3);
    a.swap(a);
    REQUIRE(a.size() == 3);
    REQUIRE(a[0] == 0x01);
}

// ============================================================================
//  slice / first / last
// ============================================================================

TEST_CASE("buffer_span slice basic", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    memepp::buffer_span bs(raw, 5);
    auto s = bs.slice(1, 3);
    REQUIRE(s.size() == 3);
    REQUIRE(s[0] == 0x02);
    REQUIRE(s[2] == 0x04);
}

TEST_CASE("buffer_span slice default count", "[buffer_span]")
{
    const uint8_t raw[] = {0x0A, 0x0B, 0x0C, 0x0D};
    memepp::buffer_span bs(raw, 4);
    auto s = bs.slice(2);
    REQUIRE(s.size() == 2);
    REQUIRE(s[0] == 0x0C);
}

TEST_CASE("buffer_span slice pos past end", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02};
    memepp::buffer_span bs(raw, 2);
    auto s = bs.slice(10);
    REQUIRE(s.empty());
}

TEST_CASE("buffer_span slice count exceeds", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03};
    memepp::buffer_span bs(raw, 3);
    auto s = bs.slice(1, 100);
    REQUIRE(s.size() == 2);
}

TEST_CASE("buffer_span first", "[buffer_span]")
{
    const uint8_t raw[] = {0x0A, 0x0B, 0x0C, 0x0D, 0x0E};
    memepp::buffer_span bs(raw, 5);
    auto f = bs.first(2);
    REQUIRE(f.size() == 2);
    REQUIRE(f[0] == 0x0A);
    REQUIRE(f[1] == 0x0B);
}

TEST_CASE("buffer_span first exceeds size", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02};
    memepp::buffer_span bs(raw, 2);
    auto f = bs.first(10);
    REQUIRE(f.size() == 2);
}

TEST_CASE("buffer_span first zero", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03};
    memepp::buffer_span bs(raw, 3);
    auto f = bs.first(0);
    REQUIRE(f.size() == 0);
    REQUIRE(f.empty());
}

TEST_CASE("buffer_span last", "[buffer_span]")
{
    const uint8_t raw[] = {0x0A, 0x0B, 0x0C, 0x0D, 0x0E};
    memepp::buffer_span bs(raw, 5);
    auto l = bs.last(2);
    REQUIRE(l.size() == 2);
    REQUIRE(l[0] == 0x0D);
    REQUIRE(l[1] == 0x0E);
}

TEST_CASE("buffer_span last exceeds size", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02};
    memepp::buffer_span bs(raw, 2);
    auto l = bs.last(10);
    REQUIRE(l.size() == 2);
}

TEST_CASE("buffer_span last zero", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03};
    memepp::buffer_span bs(raw, 3);
    auto l = bs.last(0);
    REQUIRE(l.size() == 0);
    REQUIRE(l.empty());
}

// ============================================================================
//  find — byte
// ============================================================================

TEST_CASE("buffer_span find byte", "[buffer_span]")
{
    const uint8_t raw[] = {0x10, 0x20, 0x30, 0x20, 0x40};
    memepp::buffer_span bs(raw, 5);
    REQUIRE(bs.find(0x20) == 1);
    REQUIRE(bs.find(0x40) == 4);
    REQUIRE(bs.find(0xFF) == memepp::buffer_span::npos);
}

TEST_CASE("buffer_span find byte with pos", "[buffer_span]")
{
    const uint8_t raw[] = {0x10, 0x20, 0x30, 0x20, 0x40};
    memepp::buffer_span bs(raw, 5);
    REQUIRE(bs.find(0x20, 2) == 3);
    REQUIRE(bs.find(0x10, 1) == memepp::buffer_span::npos);
}

TEST_CASE("buffer_span find byte zero-length target", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03};
    memepp::buffer_span bs(raw, 3);
    // find with empty span (via count=0) returns pos
    REQUIRE(bs.find(raw, 0, 0) == 0);
    REQUIRE(bs.find(raw, 2, 0) == 2);
}

TEST_CASE("buffer_span find byte empty span", "[buffer_span]")
{
    memepp::buffer_span bs;
    REQUIRE(bs.find(0x00) == memepp::buffer_span::npos);
}

// ============================================================================
//  rfind — byte
// ============================================================================

TEST_CASE("buffer_span rfind byte", "[buffer_span]")
{
    const uint8_t raw[] = {0x10, 0x20, 0x30, 0x20, 0x40};
    memepp::buffer_span bs(raw, 5);
    REQUIRE(bs.rfind(0x20) == 3);
    REQUIRE(bs.rfind(0x10) == 0);
    REQUIRE(bs.rfind(0xFF) == memepp::buffer_span::npos);
}

TEST_CASE("buffer_span rfind byte with pos", "[buffer_span]")
{
    const uint8_t raw[] = {0x10, 0x20, 0x30, 0x20, 0x40};
    memepp::buffer_span bs(raw, 5);
    REQUIRE(bs.rfind(0x20, 1) == 1);  // only searches in [0..1]
    REQUIRE(bs.rfind(0x30, 1) == memepp::buffer_span::npos);
}

TEST_CASE("buffer_span rfind byte empty span", "[buffer_span]")
{
    memepp::buffer_span bs;
    REQUIRE(bs.rfind(0x00) == memepp::buffer_span::npos);
}

// ============================================================================
//  find — span (multi-byte)
// ============================================================================

TEST_CASE("buffer_span find span", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03, 0x02, 0x03, 0x04};
    memepp::buffer_span bs(raw, 6);
    const uint8_t needle[] = {0x02, 0x03};
    REQUIRE(bs.find(needle, 0, 2) == 1);
    REQUIRE(bs.find(needle, 2, 2) == 3);
}

TEST_CASE("buffer_span find span not found", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03};
    memepp::buffer_span bs(raw, 3);
    const uint8_t needle[] = {0x04};
    REQUIRE(bs.find(needle, 0, 1) == memepp::buffer_span::npos);
}

TEST_CASE("buffer_span find span too long", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02};
    memepp::buffer_span bs(raw, 2);
    const uint8_t needle[] = {0x01, 0x02, 0x03};
    REQUIRE(bs.find(needle, 0, 3) == memepp::buffer_span::npos);
}

// ============================================================================
//  rfind — span (multi-byte)
// ============================================================================

TEST_CASE("buffer_span rfind span", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03, 0x01, 0x02, 0x03};
    memepp::buffer_span bs(raw, 6);
    const uint8_t needle[] = {0x01, 0x02};
    REQUIRE(bs.rfind(memepp::buffer_span(needle, 2)) == 3);
}

TEST_CASE("buffer_span rfind span with pos", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03, 0x01, 0x02};
    memepp::buffer_span bs(raw, 5);
    const uint8_t needle[] = {0x01, 0x02};
    REQUIRE(bs.rfind(needle, 0, 2) == 0);       // only searches [0..0]
}

TEST_CASE("buffer_span rfind empty needle", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03};
    memepp::buffer_span bs(raw, 3);
    memepp::buffer_span empty;

    // empty needle: pos == npos or pos >= size -> size
    REQUIRE(bs.rfind(empty) == 3);
    REQUIRE(bs.rfind(empty, memepp::buffer_span::npos) == 3);
    REQUIRE(bs.rfind(empty, 3) == 3);
    REQUIRE(bs.rfind(empty, 99) == 3);

    // empty needle: pos < size -> pos
    REQUIRE(bs.rfind(empty, 2) == 2);
    REQUIRE(bs.rfind(empty, 1) == 1);
    REQUIRE(bs.rfind(empty, 0) == 0);
}

TEST_CASE("buffer_span rfind empty needle on empty span", "[buffer_span]")
{
    memepp::buffer_span bs;
    REQUIRE(bs.rfind(memepp::buffer_span()) == 0);
}

TEST_CASE("buffer_span rfind needle longer than span", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02};
    memepp::buffer_span bs(raw, 2);
    const uint8_t nl[] = {0x01, 0x02, 0x03};
    REQUIRE(bs.rfind(memepp::buffer_span(nl, 3)) == memepp::buffer_span::npos);
}

TEST_CASE("buffer_span rfind needle equals span", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03};
    memepp::buffer_span bs(raw, 3);
    REQUIRE(bs.rfind(memepp::buffer_span(raw, 3)) == 0);
}

TEST_CASE("buffer_span rfind match starts exactly at pos", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03, 0x01, 0x02};
    memepp::buffer_span bs(raw, 5);
    const uint8_t nl[] = {0x01, 0x02};

    // match starts exactly at pos -> returned as-is
    REQUIRE(bs.rfind(memepp::buffer_span(nl, 2), 3) == 3);
    REQUIRE(bs.rfind(memepp::buffer_span(nl, 2), 4) == 3);
    // pos before the last occurrence
    REQUIRE(bs.rfind(memepp::buffer_span(nl, 2), 2) == 0);
    REQUIRE(bs.rfind(memepp::buffer_span(nl, 2), 1) == 0);
    REQUIRE(bs.rfind(memepp::buffer_span(nl, 2), 0) == 0);
}

TEST_CASE("buffer_span rfind pos at or beyond size", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03, 0x01, 0x02};
    memepp::buffer_span bs(raw, 5);
    const uint8_t nl[] = {0x01, 0x02};

    // pos == size and pos > size both clamp to size - needle
    REQUIRE(bs.rfind(memepp::buffer_span(nl, 2), 5) == 3);
    REQUIRE(bs.rfind(memepp::buffer_span(nl, 2), 99) == 3);
    REQUIRE(bs.rfind(memepp::buffer_span(nl, 2), memepp::buffer_span::npos) == 3);
}

TEST_CASE("buffer_span rfind size - pos < needle", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03, 0x01, 0x02};
    memepp::buffer_span bs(raw, 5);
    const uint8_t nl[] = {0x01, 0x02};

    // size - pos == 1 < needle 2 -> clamped to size - needle, single probe
    REQUIRE(bs.rfind(memepp::buffer_span(nl, 2), 4) == 3);
    // size - pos == 4 >= needle 2 -> no clamp
    REQUIRE(bs.rfind(memepp::buffer_span(nl, 2), 1) == 0);
}

TEST_CASE("buffer_span rfind negative pos", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03, 0x01, 0x02};
    memepp::buffer_span bs(raw, 5);
    const uint8_t nl[] = {0x01, 0x02};

    // pos < 0 and not npos -> npos
    REQUIRE(bs.rfind(memepp::buffer_span(nl, 2), -5) == memepp::buffer_span::npos);
    REQUIRE(bs.rfind(memepp::buffer_span(nl, 2), -99) == memepp::buffer_span::npos);
}

TEST_CASE("buffer_span rfind byte pos at or beyond size", "[buffer_span]")
{
    const uint8_t raw[] = {0x10, 0x20, 0x30, 0x20, 0x40};
    memepp::buffer_span bs(raw, 5);

    REQUIRE(bs.rfind(0x20, 5) == 3);
    REQUIRE(bs.rfind(0x20, 99) == 3);
    REQUIRE(bs.rfind(0x20, memepp::buffer_span::npos) == 3);
}

TEST_CASE("buffer_span rfind byte pos zero", "[buffer_span]")
{
    const uint8_t raw[] = {0x10, 0x20, 0x30, 0x20, 0x40};
    memepp::buffer_span bs(raw, 5);

    REQUIRE(bs.rfind(0x10, 0) == 0);
    REQUIRE(bs.rfind(0x20, 0) == memepp::buffer_span::npos);
    REQUIRE(bs.rfind(0xFF, 0) == memepp::buffer_span::npos);
}

TEST_CASE("buffer_span rfind negative pos byte", "[buffer_span]")
{
    const uint8_t raw[] = {0x10, 0x20, 0x30, 0x20, 0x40};
    memepp::buffer_span bs(raw, 5);

    REQUIRE(bs.rfind(0x20, -5) == memepp::buffer_span::npos);

    memepp::buffer_span empty;
    REQUIRE(empty.rfind(0x00, -5) == memepp::buffer_span::npos);
    REQUIRE(empty.rfind(0x00, 0) == memepp::buffer_span::npos);
}

TEST_CASE("buffer_span rfind multi-byte needle on empty span", "[buffer_span]")
{
    memepp::buffer_span bs;
    const uint8_t nl[] = {0x01, 0x02};
    REQUIRE(bs.rfind(memepp::buffer_span(nl, 2)) == memepp::buffer_span::npos);
    REQUIRE(bs.rfind(memepp::buffer_span(nl, 2), 0) == memepp::buffer_span::npos);
    REQUIRE(bs.rfind(memepp::buffer_span(nl, 2), memepp::buffer_span::npos)
            == memepp::buffer_span::npos);
}

TEST_CASE("buffer_span rfind buffer_span with explicit pos", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x01, 0x02, 0x01, 0x02};
    memepp::buffer_span bs(raw, 6);
    const uint8_t nl[] = {0x01, 0x02};
    memepp::buffer_span needle(nl, 2);

    REQUIRE(bs.rfind(needle, 0) == 0);
    REQUIRE(bs.rfind(needle, 1) == 0);
    REQUIRE(bs.rfind(needle, 2) == 2);
    REQUIRE(bs.rfind(needle, 3) == 2);
    REQUIRE(bs.rfind(needle, 4) == 4);
    REQUIRE(bs.rfind(needle, 5) == 4);
    REQUIRE(bs.rfind(needle, 6) == 4);

    // const_pointer+count overload must agree with the buffer_span overload
    REQUIRE(bs.rfind(nl, 3, 2) == bs.rfind(needle, 3));
    REQUIRE(bs.rfind(nl, 4, 2) == bs.rfind(needle, 4));
}

// ============================================================================
//  contains
// ============================================================================

TEST_CASE("buffer_span contains byte", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03};
    memepp::buffer_span bs(raw, 3);
    REQUIRE(bs.contains(0x02));
    REQUIRE_FALSE(bs.contains(0xFF));
    REQUIRE_FALSE(memepp::buffer_span().contains(0x00));
}

TEST_CASE("buffer_span contains span", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03, 0x04};
    memepp::buffer_span bs(raw, 4);
    const uint8_t needle[] = {0x02, 0x03};
    REQUIRE(bs.contains(needle, 2));
    const uint8_t bad[] = {0x05};
    REQUIRE_FALSE(bs.contains(bad, 1));
}

TEST_CASE("buffer_span contains span identical", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02};
    memepp::buffer_span bs(raw, 2);
    REQUIRE(bs.contains(raw, 2));
}

// ============================================================================
//  starts_with
// ============================================================================

TEST_CASE("buffer_span starts_with span", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03, 0x04};
    memepp::buffer_span bs(raw, 4);
    const uint8_t prefix[] = {0x01, 0x02};
    REQUIRE(bs.starts_with(prefix, 2));
    const uint8_t not_prefix[] = {0x02, 0x03};
    REQUIRE_FALSE(bs.starts_with(not_prefix, 2));
    REQUIRE(bs.starts_with(raw, 0));  // empty prefix
    REQUIRE_FALSE(bs.starts_with(raw, 5));  // longer than span
}

TEST_CASE("buffer_span starts_with byte", "[buffer_span]")
{
    const uint8_t raw[] = {0xAB, 0xCD};
    memepp::buffer_span bs(raw, 2);
    REQUIRE(bs.starts_with(0xAB));
    REQUIRE_FALSE(bs.starts_with(0xCD));
    REQUIRE_FALSE(memepp::buffer_span().starts_with(0x00));
}

// ============================================================================
//  ends_with
// ============================================================================

TEST_CASE("buffer_span ends_with span", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03, 0x04};
    memepp::buffer_span bs(raw, 4);
    const uint8_t suffix[] = {0x03, 0x04};
    REQUIRE(bs.ends_with(suffix, 2));
    const uint8_t not_suffix[] = {0x01, 0x02};
    REQUIRE_FALSE(bs.ends_with(not_suffix, 2));
    REQUIRE(bs.ends_with(raw, 0));  // empty suffix
    REQUIRE_FALSE(bs.ends_with(raw, 5));  // longer than span
}

TEST_CASE("buffer_span ends_with byte", "[buffer_span]")
{
    const uint8_t raw[] = {0xAB, 0xCD};
    memepp::buffer_span bs(raw, 2);
    REQUIRE(bs.ends_with(0xCD));
    REQUIRE_FALSE(bs.ends_with(0xAB));
    REQUIRE_FALSE(memepp::buffer_span().ends_with(0x00));
}

// ============================================================================
//  compare
// ============================================================================

TEST_CASE("buffer_span compare equal", "[buffer_span]")
{
    const uint8_t a[] = {0x01, 0x02, 0x03};
    const uint8_t b[] = {0x01, 0x02, 0x03};
    memepp::buffer_span sa(a, 3), sb(b, 3);
    REQUIRE(sa.compare(sb) == 0);
}

TEST_CASE("buffer_span compare prefix difference", "[buffer_span]")
{
    const uint8_t a[] = {0x01, 0x02, 0x03};
    const uint8_t b[] = {0x01, 0x04, 0x03};
    memepp::buffer_span sa(a, 3), sb(b, 3);
    REQUIRE(sa.compare(sb) < 0);
    REQUIRE(sb.compare(sa) > 0);
}

TEST_CASE("buffer_span compare length difference", "[buffer_span]")
{
    const uint8_t a[] = {0x01, 0x02};
    const uint8_t b[] = {0x01, 0x02, 0x03};
    memepp::buffer_span sa(a, 2), sb(b, 3);
    REQUIRE(sa.compare(sb) < 0);
    REQUIRE(sb.compare(sa) > 0);
}

TEST_CASE("buffer_span compare empty", "[buffer_span]")
{
    memepp::buffer_span empty;
    const uint8_t raw[] = {0x01};
    memepp::buffer_span nonempty(raw, 1);
    REQUIRE(empty.compare(empty) == 0);
    REQUIRE(empty.compare(nonempty) < 0);
    REQUIRE(nonempty.compare(empty) > 0);
}

TEST_CASE("buffer_span compare pos+count", "[buffer_span]")
{
    const uint8_t a[] = {0x01, 0x02, 0x03, 0x04};
    memepp::buffer_span sa(a, 4);
    REQUIRE(sa.compare(1, 2, memepp::buffer_span(a + 1, 2)) == 0);
}

TEST_CASE("buffer_span compare pos+count both sides", "[buffer_span]")
{
    const uint8_t a[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    const uint8_t b[] = {0xFF, 0x02, 0x03, 0xFF};
    memepp::buffer_span sa(a, 5), sb(b, 4);
    REQUIRE(sa.compare(1, 2, sb, 1, 2) == 0);
}

// ============================================================================
//  comparison operators
// ============================================================================

TEST_CASE("buffer_span operator==", "[buffer_span]")
{
    const uint8_t a[] = {0x01, 0x02};
    const uint8_t b[] = {0x01, 0x02};
    REQUIRE(memepp::buffer_span(a, 2) == memepp::buffer_span(b, 2));
    REQUIRE_FALSE(memepp::buffer_span(a, 2) == memepp::buffer_span(a, 1));
}

TEST_CASE("buffer_span operator!=", "[buffer_span]")
{
    const uint8_t a[] = {0x01, 0x02};
    const uint8_t b[] = {0x01, 0x03};
    REQUIRE(memepp::buffer_span(a, 2) != memepp::buffer_span(b, 2));
    REQUIRE_FALSE(memepp::buffer_span(a, 2) != memepp::buffer_span(a, 2));
}

TEST_CASE("buffer_span operator<", "[buffer_span]")
{
    const uint8_t a[] = {0x01, 0x02};
    const uint8_t b[] = {0x01, 0x03};
    REQUIRE(memepp::buffer_span(a, 2) < memepp::buffer_span(b, 2));
    REQUIRE_FALSE(memepp::buffer_span(b, 2) < memepp::buffer_span(a, 2));
}

TEST_CASE("buffer_span operator<=, operator>, operator>=", "[buffer_span]")
{
    const uint8_t a[] = {0x01, 0x02};
    const uint8_t b[] = {0x01, 0x03};
    memepp::buffer_span sa(a, 2), sb(b, 2);
    REQUIRE(sa <= sb);
    REQUIRE(sa <= sa);
    REQUIRE(sb > sa);
    REQUIRE(sb >= sb);
}

// ============================================================================
//  conversions
// ============================================================================

TEST_CASE("buffer_span to_buffer", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03};
    memepp::buffer_span bs(raw, 3);
    auto buf = bs.to_buffer();
    REQUIRE(buf.size() == 3);
    REQUIRE(buf.at(0) == 0x01);
    REQUIRE(buf.at(2) == 0x03);
}

TEST_CASE("buffer_span to_string", "[buffer_span]")
{
    const uint8_t raw[] = {'A', 'B', 'C'};
    memepp::buffer_span bs(raw, 3);
    auto s = bs.to_string();
    REQUIRE(s.size() == 3);
    REQUIRE(s.at(0) == 'A');
}

TEST_CASE("buffer_span to_variable_buffer", "[buffer_span]")
{
    const uint8_t raw[] = {0x10, 0x20};
    memepp::buffer_span bs(raw, 2);
    auto vbuf = bs.to_variable_buffer();
    REQUIRE(vbuf.size() == 2);
    REQUIRE(vbuf[0] == 0x10);
}

// ============================================================================
//  free function
// ============================================================================

TEST_CASE("mm_bspan", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03};
    auto bs = mm_bspan(raw, 3);
    REQUIRE(bs.size() == 3);
    REQUIRE(bs[0] == 0x01);
    REQUIRE(bs[2] == 0x03);
}

// ============================================================================
//  edge cases
// ============================================================================

TEST_CASE("buffer_span single byte", "[buffer_span]")
{
    const uint8_t raw[] = {0x7F};
    memepp::buffer_span bs(raw, 1);
    REQUIRE(bs.size() == 1);
    REQUIRE(bs[0] == 0x7F);
    REQUIRE(bs.front() == 0x7F);
    REQUIRE(bs.back() == 0x7F);
    REQUIRE(bs.data() == raw);
    auto sub = bs.slice(0, 1);
    REQUIRE(sub == bs);
    REQUIRE(bs.find(0x7F) == 0);
    REQUIRE(bs.rfind(0x7F) == 0);
    REQUIRE(bs.starts_with(0x7F));
    REQUIRE(bs.ends_with(0x7F));
}

TEST_CASE("buffer_span chain modifiers", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
    memepp::buffer_span bs(raw, 6);
    bs.remove_prefix(1);
    bs.remove_suffix(1);
    REQUIRE(bs.size() == 4);
    REQUIRE(bs[0] == 0x02);
    REQUIRE(bs[3] == 0x05);
}

TEST_CASE("buffer_span large data", "[buffer_span]")
{
    // Verify span handles larger data without issues
    uint8_t raw[1024];
    for (size_t i = 0; i < 1024; ++i)
        raw[i] = static_cast<uint8_t>(i & 0xFF);
    memepp::buffer_span bs(raw, 1024);
    REQUIRE(bs.size() == 1024);
    REQUIRE(bs[0] == 0x00);
    REQUIRE(bs[1023] == 0xFF);
    REQUIRE(bs.find(0x80) == 128);
    REQUIRE(bs.rfind(0xFF) == 1023);
}

TEST_CASE("buffer_span from buffer_view constructed from variable_buffer", "[buffer_span]")
{
    memepp::variable_buffer vbuf;
    vbuf.push_back(0x01);
    vbuf.push_back(0x02);
    vbuf.push_back(0x03);
    memepp::buffer_view bv(vbuf);
    memepp::buffer_span bs(bv);
    REQUIRE(bs.size() == 3);
    REQUIRE(bs[0] == 0x01);
    REQUIRE(bs[2] == 0x03);
}

// ============================================================================
//  to_shared_storage
// ============================================================================

TEST_CASE("buffer_span to_shared_storage", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03};
    memepp::buffer_span bs(raw, 3);
    auto buf = bs.to_shared_storage();
    REQUIRE(buf.size() == 3);
    REQUIRE(std::memcmp(buf.data(), raw, 3) == 0);
}

// ---------------------------------------------------------------------------
// find / rfind — const_pointer+pos+count (direct overload test)
// ---------------------------------------------------------------------------

TEST_CASE("buffer_span find const_pointer with pos and count", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03, 0x02, 0x03, 0x04};
    memepp::buffer_span bs(raw, 6);
    const uint8_t needle[] = {0x02, 0x03};
    REQUIRE(bs.find(needle, 2, 2) == 3);
    REQUIRE(bs.find(needle, 5, 2) == memepp::buffer_span::npos);
}

TEST_CASE("buffer_span rfind const_pointer with pos and count", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03, 0x01, 0x02};
    memepp::buffer_span bs(raw, 5);
    const uint8_t needle[] = {0x01, 0x02};
    REQUIRE(bs.rfind(needle, 2, 2) == 0);
    REQUIRE(bs.rfind(needle, 4, 2) == 3);
}

// ---------------------------------------------------------------------------
// contains / starts_with / ends_with — buffer_span overload
// ---------------------------------------------------------------------------

TEST_CASE("buffer_span contains buffer_span", "[buffer_span]")
{
    const uint8_t raw[] = {0x01, 0x02, 0x03, 0x04};
    memepp::buffer_span bs(raw, 4);
    const uint8_t nd[] = {0x02, 0x03};
    const uint8_t not_found[] = {0xFF};
    REQUIRE(bs.contains(memepp::buffer_span(nd, 2)));
    REQUIRE_FALSE(bs.contains(memepp::buffer_span(not_found, 1)));
}

TEST_CASE("buffer_span starts_with buffer_span", "[buffer_span]")
{
    const uint8_t raw[] = {0xAA, 0xBB, 0xCC};
    memepp::buffer_span bs(raw, 3);
    const uint8_t pf[] = {0xAA, 0xBB};
    const uint8_t not_prefix[] = {0xFF, 0xEE};
    REQUIRE(bs.starts_with(memepp::buffer_span(pf, 2)));
    REQUIRE_FALSE(bs.starts_with(memepp::buffer_span(not_prefix, 2)));
}

TEST_CASE("buffer_span ends_with buffer_span", "[buffer_span]")
{
    const uint8_t raw[] = {0x11, 0x22, 0x33, 0x44};
    memepp::buffer_span bs(raw, 4);
    const uint8_t sf[] = {0x33, 0x44};
    REQUIRE(bs.ends_with(memepp::buffer_span(sf, 2)));
    REQUIRE_FALSE(bs.ends_with(memepp::buffer_span(sf, 1)));
}


/* ===================================================================== */
/* Gap coverage: positions, empty spans, slicing bounds, conversions      */
/* ===================================================================== */

TEST_CASE("buffer_span find: negative and out-of-range positions",
          "[buffer_span]")
{
    const uint8_t raw[] = { 'a', 'b', 'c', 'd', 'e' };
    memepp::buffer_span bs(raw, 5);
    const uint8_t nd[] = { 'c', 'd' };

    /* find(buffer_span): _pos < 0 is clamped to 0. */
    REQUIRE(bs.find(memepp::buffer_span(nd, 2), -1) == 2);
    REQUIRE(bs.find(memepp::buffer_span(nd, 2), -100) == 2);

    /* find(buffer_span): _pos == size() -> npos (needle does not fit). */
    REQUIRE(bs.find(memepp::buffer_span(nd, 2), 5) == memepp::buffer_span::npos);
    /* _pos > size() -> npos. */
    REQUIRE(bs.find(memepp::buffer_span(nd, 2), 6) == memepp::buffer_span::npos);
    REQUIRE(bs.find(memepp::buffer_span(nd, 2), 99) == memepp::buffer_span::npos);

    /* find(byte): _pos < 0 is clamped to 0. */
    REQUIRE(bs.find((uint8_t)'a', -1) == 0);
    REQUIRE(bs.find((uint8_t)'e', -100) == 4);

    /* find(byte): _pos == size() and beyond -> npos (loop never runs). */
    REQUIRE(bs.find((uint8_t)'e', 5) == memepp::buffer_span::npos);
    REQUIRE(bs.find((uint8_t)'e', 6) == memepp::buffer_span::npos);
    REQUIRE(bs.find((uint8_t)'a', 99) == memepp::buffer_span::npos);

    /* The pointer+count overload must reach the same behaviour. */
    REQUIRE(bs.find(nd, -1, 2) == 2);      /* pos clamps to 0 */
    REQUIRE(bs.find(nd, 5, 2) == memepp::buffer_span::npos);

    /* Empty haystack: negative position clamps, then nothing can match. */
    memepp::buffer_span empty;
    REQUIRE(empty.find(memepp::buffer_span(nd, 2), -1)
            == memepp::buffer_span::npos);
    REQUIRE(empty.find((uint8_t)'a', -1) == memepp::buffer_span::npos);
}

TEST_CASE("buffer_span find: empty needle and empty haystack combinations",
          "[buffer_span]")
{
    const uint8_t raw[] = { 'a', 'b', 'c' };
    memepp::buffer_span bs(raw, 3);
    memepp::buffer_span empty;

    /* An empty needle matches at _pos, including _pos == size(). */
    REQUIRE(bs.find(memepp::buffer_span()) == 0);
    REQUIRE(bs.find(memepp::buffer_span(), 2) == 2);
    REQUIRE(bs.find(memepp::buffer_span(), 3) == 3);
    REQUIRE(bs.find(memepp::buffer_span(), 4) == memepp::buffer_span::npos);

    /* Empty haystack + empty needle: 0, but a positive _pos is out of range. */
    REQUIRE(empty.find(memepp::buffer_span()) == 0);
    REQUIRE(empty.find(memepp::buffer_span(), 0) == 0);
    REQUIRE(empty.find(memepp::buffer_span(), 1) == memepp::buffer_span::npos);

    /* Empty haystack + non-empty needle: never matches. */
    REQUIRE(empty.find(memepp::buffer_span(raw, 3)) == memepp::buffer_span::npos);
    REQUIRE(empty.find((uint8_t)'a') == memepp::buffer_span::npos);

    /* Null pointer with a zero count is the empty needle. */
    REQUIRE(bs.find(static_cast<const uint8_t*>(nullptr), 0, 0) == 0);
    REQUIRE(bs.find(static_cast<const uint8_t*>(nullptr), 2, 0) == 2);

    /* Direct buffer_span overload with an explicit needle. */
    const uint8_t nd[] = { 'b', 'c' };
    REQUIRE(bs.find(memepp::buffer_span(nd, 2)) == 1);
    REQUIRE(bs.find(memepp::buffer_span(nd, 2), 2) == memepp::buffer_span::npos);
}

TEST_CASE("buffer_span rfind: pointer overload with empty needle",
          "[buffer_span]")
{
    const uint8_t raw[] = { 'a', 'b', 'c', 'd', 'e' };
    memepp::buffer_span bs(raw, 5);
    memepp::buffer_span empty;

    /* Empty needle returns min(_pos, size()). */
    const uint8_t* np = nullptr;
    REQUIRE(bs.rfind(np, memepp::buffer_span::npos, 0) == 5);
    REQUIRE(bs.rfind(np, 2, 0) == 2);
    REQUIRE(bs.rfind(np, 0, 0) == 0);

    /* Empty haystack. */
    REQUIRE(empty.rfind(np, memepp::buffer_span::npos, 0) == 0);
    REQUIRE(empty.rfind(np, 0, 0) == 0);

    /* Non-empty needle via the pointer overload. */
    const uint8_t nd[] = { 'b', 'c' };
    REQUIRE(bs.rfind(nd, memepp::buffer_span::npos, 2) == 1);
    REQUIRE(bs.rfind(nd, 0, 2) == memepp::buffer_span::npos);
}

TEST_CASE("buffer_span empty-range contains/starts_with/ends_with",
          "[buffer_span]")
{
    const uint8_t raw[] = { 'a', 'b', 'c' };
    memepp::buffer_span bs(raw, 3);
    memepp::buffer_span empty;

    /* An empty needle is always contained. */
    REQUIRE(bs.contains(memepp::buffer_span()));
    REQUIRE(empty.contains(memepp::buffer_span()));

    /* An empty prefix/suffix always matches, including on an empty span. */
    REQUIRE(bs.starts_with(memepp::buffer_span()));
    REQUIRE(bs.ends_with(memepp::buffer_span()));
    REQUIRE(empty.starts_with(memepp::buffer_span()));
    REQUIRE(empty.ends_with(memepp::buffer_span()));

    /* A non-empty needle on an empty haystack never matches. */
    REQUIRE_FALSE(empty.contains(memepp::buffer_span(raw, 3)));
    REQUIRE_FALSE(empty.starts_with(memepp::buffer_span(raw, 3)));
    REQUIRE_FALSE(empty.ends_with(memepp::buffer_span(raw, 3)));
}

TEST_CASE("buffer_span slice/first/last: bounds and negative counts",
          "[buffer_span]")
{
    const uint8_t raw[] = { 'a', 'b', 'c', 'd', 'e' };
    memepp::buffer_span bs(raw, 5);
    memepp::buffer_span empty;

    /* slice(size) is the empty tail, slice(size, 0) the same. */
    REQUIRE(bs.slice(5).size() == 0);
    REQUIRE(bs.slice(5).empty());
    REQUIRE(bs.slice(5, 0).size() == 0);

    /* A count beyond the remaining length is clamped. */
    REQUIRE(bs.slice(3).size() == 2);
    REQUIRE(bs.slice(0, 99).size() == 5);
    REQUIRE(bs.slice(3, 99).size() == 2);

    /* npos means "to the end". */
    REQUIRE(bs.slice(2, memepp::buffer_span::npos).size() == 3);

    /* _pos > size clamps (no negative data_ offset). */
    REQUIRE(bs.slice(99).size() == 0);
    REQUIRE(bs.slice(99, 99).size() == 0);

    /* Empty span: every slice is empty. */
    REQUIRE(empty.slice(0).size() == 0);
    REQUIRE(empty.slice(0, 0).size() == 0);
    REQUIRE(empty.slice(99).size() == 0);

    /* first()/last() clamp to size. */
    REQUIRE(bs.first(5).size() == 5);
    REQUIRE(bs.first(99).size() == 5);
    REQUIRE(bs.last(5).size() == 5);
    REQUIRE(bs.last(99).size() == 5);

    /* last(size) starts at data_ + 0. */
    REQUIRE(bs.last(5).data() == bs.data());

    /* Empty span: first(0)/last(0) are empty; larger counts clamp to 0. */
    REQUIRE(empty.first(0).size() == 0);
    REQUIRE(empty.first(1).size() == 0);
    REQUIRE(empty.last(0).size() == 0);
    REQUIRE(empty.last(1).size() == 0);
}

TEST_CASE("buffer_span remove_prefix/remove_suffix edge cases",
          "[buffer_span]")
{
    const uint8_t raw[] = { 'a', 'b', 'c', 'd', 'e' };
    memepp::buffer_span bs(raw, 5);

    /* Removing the whole length leaves an empty span at the end. */
    {
        memepp::buffer_span t(raw, 5);
        t.remove_prefix(5);
        REQUIRE(t.size() == 0);
        REQUIRE(t.empty());
        REQUIRE(t.data() == raw + 5);
    }
    {
        memepp::buffer_span t(raw, 5);
        t.remove_suffix(5);
        REQUIRE(t.size() == 0);
        REQUIRE(t.data() == raw);
    }

    /* remove_prefix(0) is a no-op. */
    {
        memepp::buffer_span t(raw, 5);
        t.remove_prefix(0);
        REQUIRE(t.size() == 5);
        REQUIRE(t.data() == raw);
    }

    /* remove_suffix(0) is a no-op. */
    {
        memepp::buffer_span t(raw, 5);
        t.remove_suffix(0);
        REQUIRE(t.size() == 5);
    }

    /* Empty span: removing 0 keeps it empty. */
    {
        memepp::buffer_span t;
        t.remove_prefix(0);
        t.remove_suffix(0);
        REQUIRE(t.empty());
    }

    /* Removing down to empty and then again by 0 stays consistent. */
    {
        memepp::buffer_span t(raw, 5);
        t.remove_prefix(5);
        t.remove_prefix(0);
        REQUIRE(t.size() == 0);
        REQUIRE(t.data() == raw + 5);
    }
}

TEST_CASE("buffer_span compare: out-of-range positions and truncation",
          "[buffer_span]")
{
    const uint8_t raw[] = { 'a', 'b', 'c', 'd', 'e' };
    memepp::buffer_span bs(raw, 5);

    /* Out-of-range _pos clamps through slice(), so the range is empty. */
    REQUIRE(bs.compare(99, 3, bs.slice(0, 0)) == 0);     /* empty vs empty */
    REQUIRE(bs.compare(99, 3, memepp::buffer_span()) == 0);
    REQUIRE(bs.compare(99, 3, bs) < 0);                  /* empty vs "abcde" */

    /* A count beyond the remainder truncates. */
    REQUIRE(bs.compare(2, 99, bs.slice(2)) == 0);        /* "cde" vs "cde" */

    /* Truncated equal, then different lengths. */
    REQUIRE(bs.compare(0, 2, memepp::buffer_span(raw, 3)) < 0);   /* "ab" < "abc" */
    REQUIRE(bs.compare(0, 3, memepp::buffer_span(raw, 2)) > 0);   /* "abc" > "ab" */

    /* Both ranges empty -> equal. */
    REQUIRE(bs.compare(2, 0, bs, 3, 0) == 0);

    /* Empty range against a non-empty one. */
    REQUIRE(bs.compare(2, 0, bs, 0, 2) < 0);
    REQUIRE(bs.compare(0, 2, bs, 2, 0) > 0);

    /* High bytes compare as unsigned through memcmp. */
    const uint8_t hi[] = { 0xFF };
    const uint8_t lo[] = { 0x01 };
    REQUIRE(memepp::buffer_span(hi, 1).compare(memepp::buffer_span(lo, 1)) > 0);
    REQUIRE(memepp::buffer_span(lo, 1).compare(memepp::buffer_span(hi, 1)) < 0);
}

TEST_CASE("buffer_span comparison operators: full truth matrix",
          "[buffer_span]")
{
    const uint8_t small_raw[] = { 'a', 'b' };
    const uint8_t big_raw[]   = { 'a', 'b', 'c' };
    memepp::buffer_span small(small_raw, 2);
    memepp::buffer_span big(big_raw, 3);
    memepp::buffer_span same(small_raw, 2);      /* equal contents, other pointer */
    memepp::buffer_span empty;

    /* small < big */
    REQUIRE(small < big);
    REQUIRE_FALSE(small > big);
    REQUIRE(small <= big);
    REQUIRE_FALSE(small >= big);
    REQUIRE(small != big);
    REQUIRE_FALSE(small == big);

    /* big > small */
    REQUIRE_FALSE(big < small);
    REQUIRE(big > small);
    REQUIRE_FALSE(big <= small);
    REQUIRE(big >= small);

    /* Equal contents from different pointers. */
    REQUIRE(small == same);
    REQUIRE_FALSE(small != same);
    REQUIRE(small <= same);
    REQUIRE(small >= same);
    REQUIRE_FALSE(small < same);
    REQUIRE_FALSE(small > same);

    /* Non-empty vs empty. */
    REQUIRE(empty < big);
    REQUIRE(big > empty);
    REQUIRE_FALSE(empty > big);
    REQUIRE_FALSE(big < empty);
    REQUIRE(empty <= big);
    REQUIRE(big >= empty);

    /* Empty vs empty. */
    REQUIRE(empty == memepp::buffer_span());
    REQUIRE(empty <= memepp::buffer_span());
    REQUIRE(empty >= memepp::buffer_span());
    REQUIRE_FALSE(empty < memepp::buffer_span());
    REQUIRE_FALSE(empty > memepp::buffer_span());
}

TEST_CASE("buffer_span copy assignment: self and empty combinations",
          "[buffer_span]")
{
    const uint8_t raw[] = { 'a', 'b', 'c' };
    memepp::buffer_span a(raw, 3);

    /* Self-assignment is a no-op. */
    a = a;
    REQUIRE(a.size() == 3);
    REQUIRE(a.data() == raw);

    /* Non-empty assigned to empty. */
    memepp::buffer_span empty;
    empty = a;
    REQUIRE(empty.size() == 3);
    REQUIRE(empty.data() == raw);

    /* Empty assigned to non-empty. */
    memepp::buffer_span b(raw, 3);
    b = memepp::buffer_span();
    REQUIRE(b.empty());
    REQUIRE(b.data() == nullptr);

    /* Chained assignment keeps data/size consistent. */
    memepp::buffer_span c;
    c = b = a;
    REQUIRE(c.size() == 3);
    REQUIRE(c.data() == raw);
    REQUIRE(b.size() == 3);
    REQUIRE(b.data() == raw);
}

TEST_CASE("buffer_span swap: empty and non-empty combinations",
          "[buffer_span]")
{
    const uint8_t raw[] = { 'a', 'b', 'c' };
    memepp::buffer_span a(raw, 3);
    memepp::buffer_span empty;

    /* Non-empty <-> empty. */
    a.swap(empty);
    REQUIRE(a.empty());
    REQUIRE(a.data() == nullptr);
    REQUIRE(empty.size() == 3);
    REQUIRE(empty.data() == raw);

    /* The swapped-to object still works. */
    REQUIRE(empty.front() == 'a');
    REQUIRE(empty.back() == 'c');
    REQUIRE(empty.find((uint8_t)'b') == 1);
    REQUIRE(empty.rfind((uint8_t)'b') == 1);

    /* Empty <-> empty. */
    memepp::buffer_span e1, e2;
    e1.swap(e2);
    REQUIRE(e1.empty());
    REQUIRE(e2.empty());
    REQUIRE(e1.data() == nullptr);
}

TEST_CASE("buffer_span conversion: empty, NUL and high bytes",
          "[buffer_span]")
{
    /* Empty span converts to an empty container. */
    {
        memepp::buffer_span empty;
        REQUIRE(empty.to_buffer().size() == 0);
        REQUIRE(empty.to_string().size() == 0);
        REQUIRE(empty.to_variable_buffer().size() == 0);
        REQUIRE(empty.to_shared_storage().size() == 0);
    }

    /* Embedded NUL and high bytes survive every conversion byte-for-byte. */
    {
        const uint8_t raw[] = { 0x00, 0xFF, 0x80, 0x00, 0x7F };
        memepp::buffer_span bs(raw, 5);

        memepp::buffer b = bs.to_buffer();
        REQUIRE(b.size() == 5);
        REQUIRE(std::memcmp(b.data(), raw, 5) == 0);

        memepp::string s = bs.to_string();
        REQUIRE(s.size() == 5);
        REQUIRE(std::memcmp(s.data(), raw, 5) == 0);

        memepp::variable_buffer vb = bs.to_variable_buffer();
        REQUIRE(vb.size() == 5);
        REQUIRE(std::memcmp(vb.data(), raw, 5) == 0);

        memepp::buffer sh = bs.to_shared_storage();
        REQUIRE(sh.size() == 5);
        REQUIRE(std::memcmp(sh.data(), raw, 5) == 0);
    }

    /* A converted buffer owns its own storage: changing it must not touch
     * the original span data. */
    {
        uint8_t raw[] = { 'a', 'b', 'c' };
        memepp::buffer_span bs(raw, 3);

        memepp::buffer b = bs.to_buffer();
        REQUIRE(b.data() != raw);          /* deep copy */

        raw[0] = 'z';
        REQUIRE(bs[0] == 'z');             /* the span still aliases raw */
        REQUIRE(b[0] == 'a');              /* the copy is independent */
    }
}

TEST_CASE("buffer_span construction from empty sources",
          "[buffer_span]")
{
    memepp::buffer empty_buf;
    memepp::buffer_span a(empty_buf);
    REQUIRE(a.empty());
    REQUIRE(a.size() == 0);

    memepp::variable_buffer empty_vb;
    memepp::buffer_span b(empty_vb);
    REQUIRE(b.empty());
    REQUIRE(b.size() == 0);

    memepp::buffer_view empty_bv;
    memepp::buffer_span c(empty_bv);
    REQUIRE(c.empty());
    REQUIRE(c.size() == 0);

    memepp::string_span empty_ss;
    memepp::buffer_span d(empty_ss);
    REQUIRE(d.empty());
    REQUIRE(d.size() == 0);
}

TEST_CASE("buffer_span aliasing: span observes source mutations",
          "[buffer_span]")
{
    /* buffer_span is strictly non-owning: it must alias the source bytes. */
    memepp::variable_buffer vb;
    vb.append(reinterpret_cast<const mmbyte_t*>("abc"), 3);

    memepp::buffer_span bs(vb);
    REQUIRE(bs.size() == 3);
    REQUIRE(bs.data() == vb.data());        /* same address, no copy */
    REQUIRE(bs[1] == 'b');

    /* Mutating the source through its mutable accessor is visible. */
    vb[1] = 'Z';
    REQUIRE(bs[1] == 'Z');

    /* To confirm the copy direction: a converted buffer must NOT alias. */
    memepp::buffer copy = bs.to_buffer();
    REQUIRE(copy.data() != bs.data());
    REQUIRE(copy[1] == 'Z');
    vb[1] = 'Q';
    REQUIRE(bs[1] == 'Q');
    REQUIRE(copy[1] == 'Z');                /* the copy is frozen */
}

TEST_CASE("mm_bspan: zero length and null pointer",
          "[buffer_span]")
{
    /* nullptr with length 0 is the empty span. */
    memepp::buffer_span a = mm_bspan(static_cast<const MemeByte_t*>(nullptr), 0);
    REQUIRE(a.empty());
    REQUIRE(a.size() == 0);
    REQUIRE(a.data() == nullptr);

    /* A non-null pointer with length 0 is also empty. */
    const uint8_t raw[] = { 'a', 'b', 'c' };
    memepp::buffer_span b = mm_bspan(raw, 0);
    REQUIRE(b.empty());
    REQUIRE(b.size() == 0);
    REQUIRE(b.data() == raw);              /* pointer preserved */

    /* The normal case still works. */
    memepp::buffer_span c = mm_bspan(raw, 3);
    REQUIRE(c.size() == 3);
    REQUIRE(c.data() == raw);
}
