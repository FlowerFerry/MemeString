#include <catch2/catch.hpp>

#include <memepp/variable_buffer.hpp>
#include <memepp/buffer.hpp>
#include <memepp/string_view.hpp>

using memepp::variable_buffer;
using memepp::buffer;
using memepp::string_view;

// ---------------------------------------------------------------------------
// variable_buffer::rfind(value_type, size_type) — single byte
// ---------------------------------------------------------------------------

TEST_CASE("variable_buffer rfind single byte — basic", "[variable_buffer][rfind]")
{
    variable_buffer buf;
    buf.push_back(10);
    buf.push_back(20);
    buf.push_back(30);

    REQUIRE(buf.rfind((variable_buffer::value_type)10) == 0);
    REQUIRE(buf.rfind((variable_buffer::value_type)20) == 1);
    REQUIRE(buf.rfind((variable_buffer::value_type)30) == 2);
    REQUIRE(buf.rfind((variable_buffer::value_type)99) == variable_buffer::npos);
}

TEST_CASE("variable_buffer rfind single byte — default pos (npos)", "[variable_buffer][rfind]")
{
    // 10 appears at 0 and 2; default npos → last occurrence (2)
    variable_buffer buf;
    buf.push_back(10);
    buf.push_back(20);
    buf.push_back(10);
    buf.push_back(30);

    REQUIRE(buf.rfind((variable_buffer::value_type)10) == 2);
}

TEST_CASE("variable_buffer rfind single byte — with pos", "[variable_buffer][rfind]")
{
    // 10 appears at 0 and 2
    variable_buffer buf;
    buf.push_back(10);
    buf.push_back(20);
    buf.push_back(10);
    buf.push_back(30);

    // pos bounds the search: start must be <= pos
    REQUIRE(buf.rfind((variable_buffer::value_type)10, 3) == 2);
    REQUIRE(buf.rfind((variable_buffer::value_type)10, 2) == 2);
    REQUIRE(buf.rfind((variable_buffer::value_type)10, 1) == 0);
    REQUIRE(buf.rfind((variable_buffer::value_type)10, 0) == 0);
}

TEST_CASE("variable_buffer rfind single byte — pos beyond size clamps to size", "[variable_buffer][rfind]")
{
    variable_buffer buf;
    buf.push_back(5);
    buf.push_back(6);

    // pos >= size behaves like npos
    REQUIRE(buf.rfind((variable_buffer::value_type)6, 100) == 1);
    REQUIRE(buf.rfind((variable_buffer::value_type)5, 100) == 0);
}

TEST_CASE("variable_buffer rfind single byte — empty buffer", "[variable_buffer][rfind]")
{
    variable_buffer buf;
    REQUIRE(buf.rfind((variable_buffer::value_type)0x42) == variable_buffer::npos);
    REQUIRE(buf.rfind((variable_buffer::value_type)0x42, 0) == variable_buffer::npos);
}

// ---------------------------------------------------------------------------
// variable_buffer::rfind(const_pointer, size_type, size_type) — byte pattern
// ---------------------------------------------------------------------------

TEST_CASE("variable_buffer rfind byte pattern — basic", "[variable_buffer][rfind]")
{
    variable_buffer buf;
    const uint8_t data[] = { 1, 2, 3, 4, 1, 2, 3, 4 };
    buf.append(data, sizeof(data));

    const uint8_t pattern[] = { 2, 3 };
    REQUIRE(buf.rfind(pattern, 7, 2) == 5);
    REQUIRE(buf.rfind(pattern, 4, 2) == 1);
    REQUIRE(buf.rfind(pattern, 0, 2) == variable_buffer::npos);
}

TEST_CASE("variable_buffer rfind byte pattern — default npos via value overload", "[variable_buffer][rfind]")
{
    // sanity: single-byte npos already covered; here verify full-buffer scan
    // returns the LAST match when no restrictive pos is used.
    variable_buffer buf;
    const uint8_t data[] = { 0xAA, 0xBB, 0xCC, 0xAA, 0xBB };
    buf.append(data, sizeof(data));

    const uint8_t pattern[] = { 0xAA, 0xBB };
    // npos == full buffer → last match of "AA BB" is index 3
    REQUIRE(buf.rfind(pattern, variable_buffer::npos, 2) == 3);
}

TEST_CASE("variable_buffer rfind byte pattern — not found", "[variable_buffer][rfind]")
{
    variable_buffer buf;
    const uint8_t data[] = { 1, 2, 3 };
    buf.append(data, sizeof(data));

    const uint8_t pattern[] = { 4, 5 };
    REQUIRE(buf.rfind(pattern, 2, 2) == variable_buffer::npos);
    REQUIRE(buf.rfind(pattern, variable_buffer::npos, 2) == variable_buffer::npos);
}

TEST_CASE("variable_buffer rfind byte pattern — needle larger than haystack", "[variable_buffer][rfind]")
{
    variable_buffer buf;
    buf.push_back(1);
    buf.push_back(2);

    const uint8_t pattern[] = { 1, 2, 3 };
    REQUIRE(buf.rfind(pattern, 1, 3) == variable_buffer::npos);
    REQUIRE(buf.rfind(pattern, variable_buffer::npos, 3) == variable_buffer::npos);
}

TEST_CASE("variable_buffer rfind byte pattern — empty needle", "[variable_buffer][rfind]")
{
    variable_buffer buf;
    const uint8_t data[] = { 1, 2, 3 };
    buf.append(data, sizeof(data));

    // zero-length needle never matches (consistent with find)
    REQUIRE(buf.rfind(data, 2, 0) == variable_buffer::npos);
    REQUIRE(buf.rfind(data, variable_buffer::npos, 0) == variable_buffer::npos);
}

// ---------------------------------------------------------------------------
// variable_buffer::rfind(const string_view&, size_type)
// ---------------------------------------------------------------------------

TEST_CASE("variable_buffer rfind string_view", "[variable_buffer][rfind]")
{
    variable_buffer buf;
    const uint8_t data[] = { 'H', 'e', 'l', 'l', 'o', 'H', 'e' };
    buf.append(data, sizeof(data));

    string_view sv("He");
    REQUIRE(buf.rfind(sv) == 5);
    REQUIRE(buf.rfind(sv, 4) == 0);
    REQUIRE(buf.rfind(sv, 0) == 0);

    string_view miss("xyz");
    REQUIRE(buf.rfind(miss) == variable_buffer::npos);
}

// ---------------------------------------------------------------------------
// variable_buffer::rfind(const variable_buffer&, size_type)
// ---------------------------------------------------------------------------

TEST_CASE("variable_buffer rfind variable_buffer overload", "[variable_buffer][rfind]")
{
    variable_buffer buf;
    const uint8_t data[] = { 9, 8, 7, 9, 8, 7 };
    buf.append(data, sizeof(data));

    const uint8_t pat[] = { 8, 7 };
    variable_buffer needle(pat, sizeof(pat));

    REQUIRE(buf.rfind(needle) == 4);
    REQUIRE(buf.rfind(needle, 2) == 1);

    const uint8_t miss[] = { 1, 1 };
    variable_buffer needle2(miss, sizeof(miss));
    REQUIRE(buf.rfind(needle2) == variable_buffer::npos);
}

// ---------------------------------------------------------------------------
// variable_buffer::rfind(const buffer&, size_type)
// ---------------------------------------------------------------------------

TEST_CASE("variable_buffer rfind buffer overload", "[variable_buffer][rfind]")
{
    variable_buffer buf;
    const uint8_t data[] = { 1, 5, 5, 2, 5, 5 };
    buf.append(data, sizeof(data));

    const uint8_t pat[] = { 5, 5 };
    buffer needle(pat, static_cast<buffer::size_type>(sizeof(pat)));

    REQUIRE(buf.rfind(needle) == 4);
    REQUIRE(buf.rfind(needle, 3) == 1);

    const uint8_t miss[] = { 3, 3 };
    buffer needle2(miss, static_cast<buffer::size_type>(sizeof(miss)));
    REQUIRE(buf.rfind(needle2) == variable_buffer::npos);
}

// ---------------------------------------------------------------------------
// variable_buffer::rfind — additional edge cases
// ---------------------------------------------------------------------------

TEST_CASE("variable_buffer rfind single byte — pos = 0 boundary", "[variable_buffer][rfind]")
{
    variable_buffer buf;
    buf.push_back(10);
    buf.push_back(20);
    buf.push_back(10);

    // pos = 0: only index 0 is valid
    REQUIRE(buf.rfind((variable_buffer::value_type)10, 0) == 0);
    REQUIRE(buf.rfind((variable_buffer::value_type)20, 0) == variable_buffer::npos);
}

TEST_CASE("variable_buffer rfind single byte — multiple occurrences returns last", "[variable_buffer][rfind]")
{
    variable_buffer buf;
    const uint8_t data[] = { 1, 2, 1, 2, 1, 2, 1 };
    buf.append(data, sizeof(data));

    // 1 appears at 0, 2, 4, 6; default npos → last (6)
    REQUIRE(buf.rfind((variable_buffer::value_type)1) == 6);
    // 2 appears at 1, 3, 5; default npos → last (5)
    REQUIRE(buf.rfind((variable_buffer::value_type)2) == 5);
}

TEST_CASE("variable_buffer rfind byte pattern — medium storage", "[variable_buffer][rfind]")
{
    variable_buffer buf;
    const uint8_t data[] = {
        'T','h','e',' ','q','u','i','c','k',' ','b','r','o','w','n',
        ' ','f','o','x',' ','j','u','m','p','s',' ','o','v','e','r',
        ' ','t','h','e',' ','l','a','z','y',' ','d','o','g','.'
    };
    buf.append(data, sizeof(data));

    const uint8_t pattern[] = { ' ', 't', 'h', 'e', ' ' };
    // " the " appears at index 30
    REQUIRE(buf.rfind(pattern, variable_buffer::npos, 5) == 30);
}

TEST_CASE("variable_buffer rfind byte pattern — consecutive matches", "[variable_buffer][rfind]")
{
    variable_buffer buf;
    const uint8_t data[] = { 1, 2, 1, 2, 1, 2 };
    buf.append(data, sizeof(data));

    const uint8_t pattern[] = { 1, 2 };
    // "1 2" appears at 0, 2, 4; last is 4
    REQUIRE(buf.rfind(pattern, variable_buffer::npos, 2) == 4);
    REQUIRE(buf.rfind(pattern, 3, 2) == 2);
    REQUIRE(buf.rfind(pattern, 1, 2) == 0);
}

TEST_CASE("variable_buffer rfind — binary data", "[variable_buffer][rfind]")
{
    variable_buffer buf;
    const uint8_t data[] = { 0x00, 0xFF, 0x00, 0xFF, 0x00 };
    buf.append(data, sizeof(data));

    REQUIRE(buf.rfind((variable_buffer::value_type)0xFF) == 3);
    REQUIRE(buf.rfind((variable_buffer::value_type)0x00) == 4);

    const uint8_t pattern[] = { 0x00, 0xFF };
    REQUIRE(buf.rfind(pattern, variable_buffer::npos, 2) == 2);
}
