#include <catch2/catch.hpp>

#include <memepp/buffer_view.hpp>
#include <memepp/buffer.hpp>
#include <memepp/variable_buffer.hpp>
#include <memepp/string.hpp>
#include <memepp/string_view.hpp>

// ---------------------------------------------------------------------------
// Assignment operators
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer_view assign from buffer", "[buffer_view]")
{
    const uint8_t data[] = { 'A', 'B', 'C' };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));

    memepp::buffer_view bv;
    bv = buf;
    REQUIRE(bv.size() == 3);
    REQUIRE(bv.at(0) == 'A');
    REQUIRE(bv.at(2) == 'C');
}

TEST_CASE("memepp::buffer_view assign from string", "[buffer_view]")
{
    memepp::string s("hello");
    memepp::buffer_view bv;
    bv = s;
    REQUIRE(bv.size() == 5);
    REQUIRE(bv.at(0) == 'h');
}

TEST_CASE("memepp::buffer_view assign from string_view", "[buffer_view]")
{
    memepp::string s("world");
    memepp::string_view sv(s);
    memepp::buffer_view bv;
    bv = sv;
    REQUIRE(bv.size() == 5);
}

TEST_CASE("memepp::buffer_view copy assignment", "[buffer_view]")
{
    memepp::string s("abc");
    memepp::buffer_view src(s);
    memepp::buffer_view dst;
    dst = src;
    REQUIRE(dst.size() == src.size());
    REQUIRE(dst == src);
}

TEST_CASE("memepp::buffer_view move assignment", "[buffer_view]")
{
    memepp::string s("xyz");
    memepp::buffer_view src(s);
    memepp::buffer_view dst;
    dst = std::move(src);
    REQUIRE(dst.size() == 3);
    REQUIRE(dst.at(0) == 'x');
}

// ---------------------------------------------------------------------------
// Iterators
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer_view iterators cover all bytes", "[buffer_view]")
{
    const char data[] = "ABCDE";
    memepp::string s(data, 5);
    memepp::buffer_view bv(s);

    int idx = 0;
    for (auto it = bv.cbegin(); it != bv.cend(); ++it, ++idx)
        REQUIRE(*it == static_cast<uint8_t>(data[idx]));
    REQUIRE(idx == 5);
}

TEST_CASE("memepp::buffer_view begin/end on empty view", "[buffer_view]")
{
    memepp::buffer_view bv;
    REQUIRE(bv.begin() == bv.end());
}

// ---------------------------------------------------------------------------
// index_of
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer_view index_of found", "[buffer_view]")
{
    memepp::string haystack("Hello World");
    memepp::string needle_s("World");
    memepp::buffer_view bv(haystack);
    memepp::buffer_view needle(needle_s);

    REQUIRE(bv.index_of(needle) == 6);
}

TEST_CASE("memepp::buffer_view index_of not found", "[buffer_view]")
{
    memepp::string haystack("Hello World");
    memepp::string needle_s("Earth");
    memepp::buffer_view bv(haystack);
    memepp::buffer_view needle(needle_s);

    REQUIRE(bv.index_of(needle) == memepp::buffer_view::npos);
}

TEST_CASE("memepp::buffer_view index_of raw pointer", "[buffer_view]")
{
    memepp::string haystack("abcdef");
    memepp::buffer_view bv(haystack);

    const uint8_t pattern[] = { 'c', 'd', 'e' };
    REQUIRE(bv.index_of(pattern, 3) == 2);
    REQUIRE(bv.index_of(pattern, 0) == memepp::buffer_view::npos);
}

TEST_CASE("memepp::buffer_view index_of at start", "[buffer_view]")
{
    memepp::string haystack("abcdef");
    memepp::string needle_s("abc");
    memepp::buffer_view bv(haystack);
    memepp::buffer_view needle(needle_s);

    REQUIRE(bv.index_of(needle) == 0);
}

TEST_CASE("memepp::buffer_view index_of at end", "[buffer_view]")
{
    memepp::string haystack("abcdef");
    memepp::string needle_s("def");
    memepp::buffer_view bv(haystack);
    memepp::buffer_view needle(needle_s);

    REQUIRE(bv.index_of(needle) == 3);
}

// ---------------------------------------------------------------------------
// find
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer_view find found", "[buffer_view]")
{
    memepp::string haystack("abcabcabc");
    memepp::string needle_s("abc");
    memepp::buffer_view bv(haystack);
    memepp::buffer_view needle(needle_s);

    REQUIRE(bv.find(needle, 0) == 0);
    REQUIRE(bv.find(needle, 1) == 3);
    REQUIRE(bv.find(needle, 4) == 6);
}

TEST_CASE("memepp::buffer_view find not found", "[buffer_view]")
{
    memepp::string haystack("abcdef");
    memepp::string needle_s("xyz");
    memepp::buffer_view bv(haystack);
    memepp::buffer_view needle(needle_s);

    REQUIRE(bv.find(needle, 0) == memepp::buffer_view::npos);
}

TEST_CASE("memepp::buffer_view find with offset beyond match", "[buffer_view]")
{
    memepp::string haystack("abcabc");
    memepp::string needle_s("abc");
    memepp::buffer_view bv(haystack);
    memepp::buffer_view needle(needle_s);

    REQUIRE(bv.find(needle, 4) == memepp::buffer_view::npos);
}

// ---------------------------------------------------------------------------
// contains
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer_view contains true", "[buffer_view]")
{
    memepp::string s("Hello World");
    memepp::buffer_view bv(s);

    memepp::string needle_s("World");
    memepp::buffer_view needle(needle_s);
    REQUIRE(bv.contains(needle));
}

TEST_CASE("memepp::buffer_view contains false", "[buffer_view]")
{
    memepp::string s("Hello World");
    memepp::buffer_view bv(s);

    memepp::string needle_s("Earth");
    memepp::buffer_view needle(needle_s);
    REQUIRE(!bv.contains(needle));
}

TEST_CASE("memepp::buffer_view contains raw pointer", "[buffer_view]")
{
    memepp::string s("abcdef");
    memepp::buffer_view bv(s);

    const uint8_t yes[] = { 'b', 'c' };
    const uint8_t no[]  = { 'z', 'z' };
    REQUIRE(bv.contains(yes, 2));
    REQUIRE(!bv.contains(no, 2));
}

// ---------------------------------------------------------------------------
// starts_with
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer_view starts_with true", "[buffer_view]")
{
    memepp::string s("Hello World");
    memepp::buffer_view bv(s);

    memepp::string prefix_s("Hello");
    memepp::buffer_view prefix(prefix_s);
    REQUIRE(bv.starts_with(prefix));
}

TEST_CASE("memepp::buffer_view starts_with false", "[buffer_view]")
{
    memepp::string s("Hello World");
    memepp::buffer_view bv(s);

    memepp::string prefix_s("World");
    memepp::buffer_view prefix(prefix_s);
    REQUIRE(!bv.starts_with(prefix));
}

TEST_CASE("memepp::buffer_view starts_with empty prefix", "[buffer_view]")
{
    memepp::string s("Hello");
    memepp::buffer_view bv(s);
    memepp::buffer_view empty;
    REQUIRE(bv.starts_with(empty));
}

TEST_CASE("memepp::buffer_view starts_with raw pointer", "[buffer_view]")
{
    memepp::string s("abcdef");
    memepp::buffer_view bv(s);

    const uint8_t yes[] = { 'a', 'b', 'c' };
    const uint8_t no[]  = { 'b', 'c', 'd' };
    REQUIRE(bv.starts_with(yes, 3));
    REQUIRE(!bv.starts_with(no, 3));
}

// ---------------------------------------------------------------------------
// ends_with
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer_view ends_with true", "[buffer_view]")
{
    memepp::string s("Hello World");
    memepp::buffer_view bv(s);

    memepp::string suffix_s("World");
    memepp::buffer_view suffix(suffix_s);
    REQUIRE(bv.ends_with(suffix));
}

TEST_CASE("memepp::buffer_view ends_with false", "[buffer_view]")
{
    memepp::string s("Hello World");
    memepp::buffer_view bv(s);

    memepp::string suffix_s("Hello");
    memepp::buffer_view suffix(suffix_s);
    REQUIRE(!bv.ends_with(suffix));
}

TEST_CASE("memepp::buffer_view ends_with empty suffix", "[buffer_view]")
{
    memepp::string s("Hello");
    memepp::buffer_view bv(s);
    memepp::buffer_view empty;
    REQUIRE(bv.ends_with(empty));
}

TEST_CASE("memepp::buffer_view ends_with raw pointer", "[buffer_view]")
{
    memepp::string s("abcdef");
    memepp::buffer_view bv(s);

    const uint8_t yes[] = { 'd', 'e', 'f' };
    const uint8_t no[]  = { 'a', 'b', 'c' };
    REQUIRE(bv.ends_with(yes, 3));
    REQUIRE(!bv.ends_with(no, 3));
}

// ---------------------------------------------------------------------------
// to_buffer
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer_view to_buffer", "[buffer_view]")
{
    memepp::string s("hello");
    memepp::buffer_view bv(s);
    memepp::buffer buf = bv.to_buffer();

    REQUIRE(buf.size() == 5);
    REQUIRE(buf.at(0) == 'h');
    REQUIRE(buf.at(4) == 'o');
}

TEST_CASE("memepp::buffer_view to_buffer from empty view", "[buffer_view]")
{
    memepp::buffer_view bv;
    memepp::buffer buf = bv.to_buffer();
    REQUIRE(buf.empty());
}

// ---------------------------------------------------------------------------
// to_string
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer_view to_string", "[buffer_view]")
{
    memepp::string s("world");
    memepp::buffer_view bv(s);
    memepp::string result = bv.to_string();
    REQUIRE(result == "world");
    REQUIRE(result.size() == 5);
}

TEST_CASE("memepp::buffer_view to_string with front_offset", "[buffer_view]")
{
    memepp::string s("abcdef");
    memepp::buffer_view bv(s);
    memepp::string result = bv.to_string(2);
    REQUIRE(result == "cdef");
    REQUIRE(result.size() == 4);
}

TEST_CASE("memepp::buffer_view to_string from empty view", "[buffer_view]")
{
    memepp::buffer_view bv;
    memepp::string result = bv.to_string();
    REQUIRE(result.empty());
}

// ---------------------------------------------------------------------------
// equality operators
// ---------------------------------------------------------------------------

TEST_CASE("memepp::buffer_view operator== and operator!=", "[buffer_view]")
{
    memepp::string s1("abc");
    memepp::string s2("abc");
    memepp::string s3("xyz");

    memepp::buffer_view bv1(s1);
    memepp::buffer_view bv2(s2);
    memepp::buffer_view bv3(s3);

    REQUIRE(bv1 == bv2);
    REQUIRE(bv1 != bv3);
    REQUIRE(!(bv1 != bv2));
    REQUIRE(!(bv1 == bv3));
}

TEST_CASE("memepp::buffer_view empty views are equal", "[buffer_view]")
{
    memepp::buffer_view a;
    memepp::buffer_view b;
    REQUIRE(a == b);
}
