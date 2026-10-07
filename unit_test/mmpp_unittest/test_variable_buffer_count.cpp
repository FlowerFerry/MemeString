#include <catch2/catch.hpp>

#include <memepp/variable_buffer.hpp>

// ---------------------------------------------------------------------------
// variable_buffer::count(value_type)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer count byte — empty buffer", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    REQUIRE(buf.count(static_cast<uint8_t>(0x41)) == 0);
}

TEST_CASE("memepp::variable_buffer count byte — not found", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 0x01, 0x02, 0x03, 0x04 };
    buf.append(data, sizeof(data));
    REQUIRE(buf.count(static_cast<uint8_t>(0xFF)) == 0);
}

TEST_CASE("memepp::variable_buffer count byte — appears N times", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 0x41, 0x42, 0x41, 0x43, 0x41 };
    buf.append(data, sizeof(data));
    REQUIRE(buf.count(static_cast<uint8_t>(0x41)) == 3);
}

// ---------------------------------------------------------------------------
// variable_buffer::count(const_pointer, size_type)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer count bytes — empty buffer", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t needle[] = { 0xAA };
    REQUIRE(buf.count(needle, 1) == 0);
}

TEST_CASE("memepp::variable_buffer count bytes — size == 0", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 0x41, 0x41, 0x41 };
    buf.append(data, sizeof(data));
    const uint8_t needle[] = { 0x41 };
    REQUIRE(buf.count(needle, 0) == 0);
}

TEST_CASE("memepp::variable_buffer count bytes — needle longer than buffer", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 0x01, 0x02 };
    buf.append(data, sizeof(data));
    const uint8_t needle[] = { 0x01, 0x02, 0x03, 0x04 };
    REQUIRE(buf.count(needle, 4) == 0);
}

TEST_CASE("memepp::variable_buffer count bytes — not found", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 0x01, 0x02, 0x03 };
    buf.append(data, sizeof(data));
    const uint8_t needle[] = { 0x05, 0x06 };
    REQUIRE(buf.count(needle, 2) == 0);
}

TEST_CASE("memepp::variable_buffer count bytes — exact full match", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 0x0A, 0x0B, 0x0C };
    buf.append(data, sizeof(data));
    REQUIRE(buf.count(data, sizeof(data)) == 1);
}

TEST_CASE("memepp::variable_buffer count bytes — repeated", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD };
    buf.append(data, sizeof(data));
    const uint8_t needle[] = { 0xAB, 0xCD };
    REQUIRE(buf.count(needle, 2) == 3);
}

// ---------------------------------------------------------------------------
// Non-overlapping semantics
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer count bytes — non-overlapping adjacent", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 0xAA, 0xBB, 0xAA, 0xBB };
    buf.append(data, sizeof(data));
    const uint8_t needle[] = { 0xAA, 0xBB };
    REQUIRE(buf.count(needle, 2) == 2);
}

TEST_CASE("memepp::variable_buffer count bytes — non-overlapping rejects overlap", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 0xAA, 0xAA, 0xAA };
    buf.append(data, sizeof(data));
    const uint8_t needle[] = { 0xAA, 0xAA };
    // Overlapping would give 2; non-overlapping gives 1.
    REQUIRE(buf.count(needle, 2) == 1);
}

TEST_CASE("memepp::variable_buffer count bytes — non-overlapping ABA in ABABA", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 'A', 'B', 'A', 'B', 'A' };
    buf.append(data, sizeof(data));
    const uint8_t needle[] = { 'A', 'B', 'A' };
    // "ABABA": match at 0 consumes 0,1,2; next search from 3 -> "BA" no match.
    REQUIRE(buf.count(needle, 3) == 1);
}

// ---------------------------------------------------------------------------
// Consistency with find() loop
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer count — consistent with find loop", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 0x41, 0x42, 0x41, 0x41, 0x42, 0x41 };
    buf.append(data, sizeof(data));
    const uint8_t needle[] = { 0x41, 0x42 };

    size_t expected = 0;
    auto pos = static_cast<memepp::variable_buffer::size_type>(0);
    while (true)
    {
        auto idx = buf.find(needle, pos, 2);
        if (idx == memepp::variable_buffer::npos)
            break;
        ++expected;
        pos = static_cast<memepp::variable_buffer::size_type>(idx + 2);
    }

    REQUIRE(buf.count(needle, 2) == expected);
}

// ---------------------------------------------------------------------------
// variable_buffer::count — medium storage & full-match edge cases
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer count byte — all bytes match", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 0x07, 0x07, 0x07, 0x07 };
    buf.append(data, sizeof(data));
    REQUIRE(buf.count(static_cast<uint8_t>(0x07)) == 4);
}

TEST_CASE("memepp::variable_buffer count byte — medium storage", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = {
        'T','h','e',' ','q','u','i','c','k',' ','b','r','o','w','n',
        ' ','f','o','x',' ','j','u','m','p','s',' ','o','v','e','r',
        ' ','t','h','e',' ','l','a','z','y',' ','d','o','g','.'
    };
    buf.append(data, sizeof(data));
    REQUIRE(buf.count(static_cast<uint8_t>(' ')) == 8);
    REQUIRE(buf.count(static_cast<uint8_t>('e')) == 3);
}

TEST_CASE("memepp::variable_buffer count bytes — medium storage repeated", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = {
        'a','b','c',' ','a','b','c',' ','a','b','c',' ','a','b','c',
        ' ','a','b','c',' ','a','b','c',' ','a','b','c'
    };
    buf.append(data, sizeof(data));
    const uint8_t needle[] = { 'a','b','c' };
    REQUIRE(buf.count(needle, 3) == 7);
}

TEST_CASE("memepp::variable_buffer count bytes — whole buffer is one match", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 0xDE, 0xAD, 0xBE, 0xEF };
    buf.append(data, sizeof(data));
    const uint8_t needle[] = { 0xDE, 0xAD, 0xBE, 0xEF };
    REQUIRE(buf.count(needle, 4) == 1);
    REQUIRE(buf.count(needle, 0) == 0);
}
