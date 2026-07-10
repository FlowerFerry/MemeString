#include <catch2/catch.hpp>

#include <memepp/buffer.hpp>
#include <memepp/buffer_view.hpp>
#include <memepp/variable_buffer.hpp>
#include <memepp/string.hpp>
#include <stdexcept>

// ============================================================================
// buffer: compare()
// ============================================================================

TEST_CASE("buffer compare equal", "[buffer][compare]")
{
    const uint8_t d[] = { 1, 2, 3 };
    memepp::buffer a(d, 3);
    memepp::buffer b(d, 3);
    REQUIRE(a.compare(b) == 0);
}

TEST_CASE("buffer compare less by lexicographic", "[buffer][compare]")
{
    const uint8_t d1[] = { 1, 2, 3 };
    const uint8_t d2[] = { 1, 2, 5 };
    memepp::buffer a(d1, 3);
    memepp::buffer b(d2, 3);
    REQUIRE(a.compare(b) < 0);
    REQUIRE(b.compare(a) > 0);
}

TEST_CASE("buffer compare less by length (prefix)", "[buffer][compare]")
{
    const uint8_t d1[] = { 1, 2 };
    const uint8_t d2[] = { 1, 2, 3 };
    memepp::buffer a(d1, 2);
    memepp::buffer b(d2, 3);
    REQUIRE(a.compare(b) < 0);
    REQUIRE(b.compare(a) > 0);
}

TEST_CASE("buffer compare empty buffers", "[buffer][compare]")
{
    memepp::buffer a;
    memepp::buffer b;
    REQUIRE(a.compare(b) == 0);
}

TEST_CASE("buffer compare empty vs non-empty", "[buffer][compare]")
{
    memepp::buffer a;
    const uint8_t d[] = { 0 };
    memepp::buffer b(d, 1);
    REQUIRE(a.compare(b) < 0);
    REQUIRE(b.compare(a) > 0);
}

// ============================================================================
// buffer: operator <, >, <=, >=
// ============================================================================

TEST_CASE("buffer ordering operators", "[buffer][compare]")
{
    const uint8_t d1[] = { 10, 20, 30 };
    const uint8_t d2[] = { 10, 20, 40 };
    memepp::buffer a(d1, 3);
    memepp::buffer b(d2, 3);

    REQUIRE(a < b);
    REQUIRE(a <= b);
    REQUIRE_FALSE(a > b);
    REQUIRE_FALSE(a >= b);
    REQUIRE_FALSE(b < a);
    REQUIRE_FALSE(b <= a);
    REQUIRE(b > a);
    REQUIRE(b >= a);
}

TEST_CASE("buffer ordering equal values", "[buffer][compare]")
{
    const uint8_t d[] = { 1, 2, 3 };
    memepp::buffer a(d, 3);
    memepp::buffer b(d, 3);

    REQUIRE_FALSE(a < b);
    REQUIRE(a <= b);
    REQUIRE_FALSE(a > b);
    REQUIRE(a >= b);
}

TEST_CASE("buffer ordering with copy constructed", "[buffer][compare]")
{
    const uint8_t d[] = { 1, 2, 3 };
    memepp::buffer a(d, 3);
    memepp::buffer b(a);

    REQUIRE_FALSE(a < b);
    REQUIRE(a <= b);
    REQUIRE_FALSE(a > b);
    REQUIRE(a >= b);
    REQUIRE(a.compare(b) == 0);
}

// ============================================================================
// buffer: operator[]
// ============================================================================

TEST_CASE("buffer operator[] basic", "[buffer][element]")
{
    const uint8_t d[] = { 'A', 'B', 'C' };
    memepp::buffer buf(d, 3);
    REQUIRE(buf[0] == 'A');
    REQUIRE(buf[1] == 'B');
    REQUIRE(buf[2] == 'C');
}

TEST_CASE("buffer operator[] from copy", "[buffer][element]")
{
    const uint8_t d[] = { 0x11, 0x22, 0x33 };
    memepp::buffer src(d, 3);
    memepp::buffer buf(src);
    REQUIRE(buf[0] == 0x11);
    REQUIRE(buf[2] == 0x33);
}

// ============================================================================
// buffer: at() out-of-range
// ============================================================================

TEST_CASE("buffer at out-of-range throws", "[buffer][element]")
{
    const uint8_t d[] = { 1, 2, 3 };
    memepp::buffer buf(d, 3);
    REQUIRE_NOTHROW(buf.at(0));
    REQUIRE_NOTHROW(buf.at(2));
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(buf.at(3), std::out_of_range);
    REQUIRE_THROWS_AS(buf.at(100), std::out_of_range);
#endif
}

// ============================================================================
// buffer: slice()
// ============================================================================

TEST_CASE("buffer slice full", "[buffer][slice]")
{
    const uint8_t d[] = { 1, 2, 3, 4, 5 };
    memepp::buffer buf(d, 5);
    auto sl = buf.slice(0, 5);
    REQUIRE(sl.size() == 5);
    REQUIRE(sl == buf);
}

TEST_CASE("buffer slice partial", "[buffer][slice]")
{
    const uint8_t d[] = { 10, 20, 30, 40, 50 };
    memepp::buffer buf(d, 5);
    auto sl = buf.slice(1, 3);
    REQUIRE(sl.size() == 3);
    REQUIRE(sl[0] == 20);
    REQUIRE(sl[2] == 40);
}

TEST_CASE("buffer slice default args", "[buffer][slice]")
{
    const uint8_t d[] = { 0, 1, 2, 3, 4 };
    memepp::buffer buf(d, 5);
    auto sl = buf.slice();
    REQUIRE(sl.size() == 5);
    REQUIRE(sl == buf);
}

TEST_CASE("buffer slice count exceeds available", "[buffer][slice]")
{
    const uint8_t d[] = { 1, 2, 3 };
    memepp::buffer buf(d, 3);
    auto sl = buf.slice(1, 100);
    REQUIRE(sl.size() == 2);
    REQUIRE(sl[0] == 2);
    REQUIRE(sl[1] == 3);
}

TEST_CASE("buffer slice pos out of range returns empty", "[buffer][slice]")
{
    const uint8_t d[] = { 1, 2, 3 };
    memepp::buffer buf(d, 3);
    auto sl = buf.slice(3);
    REQUIRE(sl.empty());
    REQUIRE(sl.size() == 0);
    auto sl2 = buf.slice(100);
    REQUIRE(sl2.empty());
}

// ============================================================================
// buffer_view: compare()
// ============================================================================

TEST_CASE("buffer_view compare equal", "[buffer_view][compare]")
{
    memepp::string s("hello");
    memepp::buffer_view a(s);
    memepp::buffer_view b(s);
    REQUIRE(a.compare(b) == 0);
}

TEST_CASE("buffer_view compare less", "[buffer_view][compare]")
{
    memepp::string s1("abc");
    memepp::string s2("abd");
    memepp::buffer_view a(s1);
    memepp::buffer_view b(s2);
    REQUIRE(a.compare(b) < 0);
    REQUIRE(b.compare(a) > 0);
}

TEST_CASE("buffer_view compare prefix shorter", "[buffer_view][compare]")
{
    memepp::string s1("ab");
    memepp::string s2("abc");
    memepp::buffer_view a(s1);
    memepp::buffer_view b(s2);
    REQUIRE(a.compare(b) < 0);
}

TEST_CASE("buffer_view compare empty", "[buffer_view][compare]")
{
    memepp::buffer_view a;
    memepp::buffer_view b;
    REQUIRE(a.compare(b) == 0);
}

// ============================================================================
// buffer_view: operator <, >, <=, >=
// ============================================================================

TEST_CASE("buffer_view ordering operators", "[buffer_view][compare]")
{
    memepp::string s1("apple");
    memepp::string s2("banana");
    memepp::buffer_view a(s1);
    memepp::buffer_view b(s2);

    REQUIRE(a < b);
    REQUIRE(a <= b);
    REQUIRE_FALSE(a > b);
    REQUIRE_FALSE(a >= b);
    REQUIRE(b > a);
    REQUIRE(b >= a);
}

TEST_CASE("buffer_view ordering equal", "[buffer_view][compare]")
{
    memepp::string s("same");
    memepp::buffer_view a(s);
    memepp::buffer_view b(s);

    REQUIRE_FALSE(a < b);
    REQUIRE(a <= b);
    REQUIRE_FALSE(a > b);
    REQUIRE(a >= b);
}

// ============================================================================
// buffer_view: operator[]
// ============================================================================

TEST_CASE("buffer_view operator[]", "[buffer_view][element]")
{
    memepp::string s("XYZ");
    memepp::buffer_view bv(s);
    REQUIRE(bv[0] == 'X');
    REQUIRE(bv[1] == 'Y');
    REQUIRE(bv[2] == 'Z');
}

// ============================================================================
// buffer_view: at() out-of-range
// ============================================================================

TEST_CASE("buffer_view at out-of-range throws", "[buffer_view][element]")
{
    memepp::string s("test");
    memepp::buffer_view bv(s);
    REQUIRE_NOTHROW(bv.at(0));
    REQUIRE_NOTHROW(bv.at(3));
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(bv.at(4), std::out_of_range);
    REQUIRE_THROWS_AS(bv.at(100), std::out_of_range);
#endif
}

// ============================================================================
// buffer_view: slice() with view storage reuse
// ============================================================================

TEST_CASE("buffer_view slice reuses view storage", "[buffer_view][slice]")
{
    memepp::string s("abcdefgh");
    memepp::buffer_view bv(s);
    // Slice from a view-backed buffer_view -> should produce another view
    auto sl = bv.slice(2, 4);
    REQUIRE(sl.size() == 4);
    REQUIRE(sl[0] == 'c');
    REQUIRE(sl[3] == 'f');
}

// ============================================================================
// variable_buffer: compare()
// ============================================================================

TEST_CASE("variable_buffer compare equal", "[variable_buffer][compare]")
{
    memepp::variable_buffer a;
    memepp::variable_buffer b;
    const uint8_t d[] = { 1, 2, 3 };
    a.append(d, 3);
    b.append(d, 3);
    REQUIRE(a.compare(b) == 0);
}

TEST_CASE("variable_buffer compare less lexicographic", "[variable_buffer][compare]")
{
    memepp::variable_buffer a;
    memepp::variable_buffer b;
    const uint8_t d1[] = { 5, 6 };
    const uint8_t d2[] = { 5, 7 };
    a.append(d1, 2);
    b.append(d2, 2);
    REQUIRE(a.compare(b) < 0);
    REQUIRE(b.compare(a) > 0);
}

TEST_CASE("variable_buffer compare less by length", "[variable_buffer][compare]")
{
    memepp::variable_buffer a;
    memepp::variable_buffer b;
    const uint8_t d1[] = { 1, 2 };
    const uint8_t d2[] = { 1, 2, 3 };
    a.append(d1, 2);
    b.append(d2, 3);
    REQUIRE(a.compare(b) < 0);
}

TEST_CASE("variable_buffer compare empty", "[variable_buffer][compare]")
{
    memepp::variable_buffer a;
    memepp::variable_buffer b;
    REQUIRE(a.compare(b) == 0);
}

TEST_CASE("variable_buffer compare empty vs data", "[variable_buffer][compare]")
{
    memepp::variable_buffer a;
    memepp::variable_buffer b;
    b.push_back(1);
    REQUIRE(a.compare(b) < 0);
    REQUIRE(b.compare(a) > 0);
}

// ============================================================================
// variable_buffer: operator ==, !=, <, >, <=, >=
// ============================================================================

TEST_CASE("variable_buffer ordering operators", "[variable_buffer][compare]")
{
    memepp::variable_buffer a;
    memepp::variable_buffer b;
    const uint8_t d1[] = { 10, 20 };
    const uint8_t d2[] = { 10, 30 };
    a.append(d1, 2);
    b.append(d2, 2);

    REQUIRE(a < b);
    REQUIRE(a <= b);
    REQUIRE_FALSE(a > b);
    REQUIRE_FALSE(a >= b);
    REQUIRE(b > a);
    REQUIRE(b >= a);
    REQUIRE_FALSE(a == b);
    REQUIRE(a != b);
}

TEST_CASE("variable_buffer equality operators", "[variable_buffer][compare]")
{
    memepp::variable_buffer a;
    memepp::variable_buffer b;
    const uint8_t d[] = { 0xAB, 0xCD };
    a.append(d, 2);
    b.append(d, 2);

    REQUIRE(a == b);
    REQUIRE_FALSE(a != b);
}

TEST_CASE("variable_buffer ordering equal values", "[variable_buffer][compare]")
{
    memepp::variable_buffer a;
    memepp::variable_buffer b;
    const uint8_t d[] = { 1, 2, 3 };
    a.append(d, 3);
    b.append(d, 3);

    REQUIRE_FALSE(a < b);
    REQUIRE(a <= b);
    REQUIRE_FALSE(a > b);
    REQUIRE(a >= b);
    REQUIRE(a == b);
}
