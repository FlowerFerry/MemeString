#include <catch2/catch.hpp>

#include <memepp/buffer_span.hpp>
#include <memepp/buffer.hpp>
#include <memepp/variable_buffer.hpp>
#include <memepp/buffer_view.hpp>
#include <memepp/string_span.hpp>
#include <memepp/string.hpp>

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
    REQUIRE_THROWS_AS(bs.at(2), std::out_of_range);
    REQUIRE_THROWS_AS(bs.at(-1), std::out_of_range);
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
