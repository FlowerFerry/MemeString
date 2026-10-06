#include <catch2/catch.hpp>

#include <memepp/variable_buffer.hpp>
#include <memepp/buffer_span.hpp>
#include <memepp/string_span.hpp>

#include <cstring>

using memepp::variable_buffer;
using memepp::buffer_span;
using memepp::string_span;

// Helper to create buffer_span from string literal
static buffer_span make_span(const char* s) {
    return buffer_span(reinterpret_cast<const memepp::buffer_span::value_type*>(s), static_cast<memepp::buffer_span::size_type>(strlen(s)));
}

static buffer_span make_span(const char* s, size_t len) {
    return buffer_span(reinterpret_cast<const memepp::buffer_span::value_type*>(s), static_cast<memepp::buffer_span::size_type>(len));
}

static string_span make_string_span(const char* s) {
    return string_span(s, static_cast<memepp::string_span::size_type>(strlen(s)));
}

// ---------------------------------------------------------------------------
// variable_buffer::replace — equal-length (fast path, zero allocation)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer replace — equal-length single match", "[variable_buffer]")
{
    const char data[] = "hello world";
    variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::value_type*>(data), 11);
    buf.replace(make_span("world", 5), make_span("meme!", 5));
    REQUIRE(buf.size() == 11);
    REQUIRE(memcmp(buf.data(), "hello meme!", 11) == 0);
}

TEST_CASE("memepp::variable_buffer replace — equal-length multiple matches", "[variable_buffer]")
{
    const char data[] = "abcabc";
    variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::value_type*>(data), 6);
    buf.replace(make_span("abc", 3), make_span("xyz", 3));
    REQUIRE(buf.size() == 6);
    REQUIRE(memcmp(buf.data(), "xyzxyz", 6) == 0);
}

// ---------------------------------------------------------------------------
// variable_buffer::replace — longer replacement (delta > 0)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer replace — longer replacement", "[variable_buffer]")
{
    const char data[] = "a b c";
    variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::value_type*>(data), 5);
    buf.replace(make_span(" ", 1), make_span("XX", 2));
    REQUIRE(buf.size() == 7);
    REQUIRE(memcmp(buf.data(), "aXXbXXc", 7) == 0);
}

TEST_CASE("memepp::variable_buffer replace — longer replacement single", "[variable_buffer]")
{
    const char data[] = "hello";
    variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::value_type*>(data), 5);
    buf.replace(make_span("llo", 3), make_span("LLLOOO", 6));
    REQUIRE(buf.size() == 8);
    REQUIRE(memcmp(buf.data(), "heLLLOOO", 8) == 0);
}

// ---------------------------------------------------------------------------
// variable_buffer::replace — shorter replacement (delta < 0)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer replace — shorter replacement", "[variable_buffer]")
{
    const char data[] = "aXXbXXc";
    variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::value_type*>(data), 7);
    buf.replace(make_span("XX", 2), make_span("Y", 1));
    REQUIRE(buf.size() == 5);
    REQUIRE(memcmp(buf.data(), "aYbYc", 5) == 0);
}

TEST_CASE("memepp::variable_buffer replace — shorter replacement shrink to nothing", "[variable_buffer]")
{
    const char data[] = "hello world";
    variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::value_type*>(data), 11);
    buf.replace(make_span(" ", 1), make_span("", 0));
    REQUIRE(buf.size() == 10);
    REQUIRE(memcmp(buf.data(), "helloworld", 10) == 0);
}

// ---------------------------------------------------------------------------
// variable_buffer::replace — count limit
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer replace — count limit 1", "[variable_buffer]")
{
    const char data[] = "aaa";
    variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::value_type*>(data), 3);
    buf.replace(make_span("a", 1), make_span("bb", 2), 1);
    REQUIRE(buf.size() == 4);
    REQUIRE(memcmp(buf.data(), "bbaa", 4) == 0);
}

TEST_CASE("memepp::variable_buffer replace — count limit 2", "[variable_buffer]")
{
    const char data[] = "aaa";
    variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::value_type*>(data), 3);
    buf.replace(make_span("a", 1), make_span("bb", 2), 2);
    REQUIRE(buf.size() == 5);
    REQUIRE(memcmp(buf.data(), "bbbba", 5) == 0);
}

TEST_CASE("memepp::variable_buffer replace — count 0 means no-op", "[variable_buffer]")
{
    const char data[] = "hello";
    variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::value_type*>(data), 5);
    buf.replace(make_span("l", 1), make_span("LL", 2), 0);
    REQUIRE(buf.size() == 5);
    REQUIRE(memcmp(buf.data(), "hello", 5) == 0);
}

// ---------------------------------------------------------------------------
// variable_buffer::replace — edge cases
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer replace — no match", "[variable_buffer]")
{
    const char data[] = "hello";
    variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::value_type*>(data), 5);
    buf.replace(make_span("xyz", 3), make_span("abc", 3));
    REQUIRE(buf.size() == 5);
    REQUIRE(memcmp(buf.data(), "hello", 5) == 0);
}

TEST_CASE("memepp::variable_buffer replace — empty old pattern", "[variable_buffer]")
{
    const char data[] = "hello";
    variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::value_type*>(data), 5);
    buf.replace(make_span("", 0), make_span("x", 1));
    REQUIRE(buf.size() == 5);
}

TEST_CASE("memepp::variable_buffer replace — empty buffer", "[variable_buffer]")
{
    variable_buffer buf;
    buf.replace(make_span("a", 1), make_span("b", 1));
    REQUIRE(buf.size() == 0);
}

TEST_CASE("memepp::variable_buffer replace — replace entire content", "[variable_buffer]")
{
    const char data[] = "hello";
    variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::value_type*>(data), 5);
    buf.replace(make_span("hello", 5), make_span("world", 5));
    REQUIRE(buf.size() == 5);
    REQUIRE(memcmp(buf.data(), "world", 5) == 0);
}

TEST_CASE("memepp::variable_buffer replace — replace entire content with shorter", "[variable_buffer]")
{
    const char data[] = "hello";
    variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::value_type*>(data), 5);
    buf.replace(make_span("hello", 5), make_span("hi", 2));
    REQUIRE(buf.size() == 2);
    REQUIRE(memcmp(buf.data(), "hi", 2) == 0);
}

// ---------------------------------------------------------------------------
// variable_buffer::replace — string_span overload
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer replace — string_span overload", "[variable_buffer]")
{
    const char data[] = "hello";
    variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::value_type*>(data), 5);
    buf.replace(make_span("llo", 3), make_string_span("LL"));
    REQUIRE(buf.size() == 4);
    REQUIRE(memcmp(buf.data(), "heLL", 4) == 0);
}

// ---------------------------------------------------------------------------
// variable_buffer::replace — non-overlapping semantics
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer replace — non-overlapping", "[variable_buffer]")
{
    const char data[] = "aaa";
    variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::value_type*>(data), 3);
    buf.replace(make_span("aa", 2), make_span("X", 1));
    // "aaa": match at 0 consumes 0,1; next search from 2 -> "a" no match.
    REQUIRE(buf.size() == 2);
    REQUIRE(memcmp(buf.data(), "Xa", 2) == 0);
}

// ---------------------------------------------------------------------------
// variable_buffer::replace — count = -1 (all) with growing single-byte pattern
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer replace — all occurrences grow single-byte", "[variable_buffer]")
{
    const char data[] = "aaa";
    variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::value_type*>(data), 3);
    buf.replace(make_span("a", 1), make_span("bb", 2));  // count = -1 (all)
    REQUIRE(buf.size() == 6);
    REQUIRE(memcmp(buf.data(), "bbbbbb", 6) == 0);
}

// ---------------------------------------------------------------------------
// variable_buffer::replace — medium storage (data > small buffer threshold)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer replace — medium equal-length", "[variable_buffer]")
{
    const char data[] =
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog.";
    const size_t n = sizeof(data) - 1;  // 90 bytes, exceeds the small buffer
    variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::value_type*>(data), n);

    // Replace "fox" (3) with "cat" (3) — equal length, medium path.
    buf.replace(make_span("fox", 3), make_span("cat", 3));

    REQUIRE(buf.size() == n);
    REQUIRE(memcmp(buf.data() + 16, "cat", 3) == 0);
    REQUIRE(memcmp(buf.data() + 61, "cat", 3) == 0);
}

TEST_CASE("memepp::variable_buffer replace — medium longer replacement", "[variable_buffer]")
{
    const char data[] =
        "aaaa bbbb cccc dddd eeee ffff gggg hhhh iiii jjjj";
    const size_t n = sizeof(data) - 1;  // 49 bytes, exceeds the small buffer
    variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::value_type*>(data), n);

    // Replace " " (1) with "__" (2) — growing, medium path.
    buf.replace(make_span(" ", 1), make_span("__", 2));

    REQUIRE(buf.size() == n + 9);
    // Verify a couple of the replaced separators.
    REQUIRE(memcmp(buf.data() + 4, "__", 2) == 0);
    REQUIRE(memcmp(buf.data() + 10, "__", 2) == 0);
}

TEST_CASE("memepp::variable_buffer replace — medium shorter replacement", "[variable_buffer]")
{
    const char data[] =
        "aaaa____bbbb____cccc____dddd____eeee____ffff";
    const size_t n = sizeof(data) - 1;  // 43 bytes, exceeds the small buffer
    variable_buffer buf(reinterpret_cast<const memepp::variable_buffer::value_type*>(data), n);

    // Replace "____" (4) with " " (1) — shrinking, medium path.
    buf.replace(make_span("____", 4), make_span(" ", 1));

    REQUIRE(buf.size() == n - 15);  // 5 matches * 3 bytes removed
    REQUIRE(memcmp(buf.data() + 4, " ", 1) == 0);
    REQUIRE(memcmp(buf.data() + 9, " ", 1) == 0);
}
