
#include <catch2/catch.hpp>

#include <memepp/variable_buffer.hpp>
#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/buffer.hpp>
#include <memepp/buffer_view.hpp>
#include <memepp/buffer_span.hpp>
#include <megopp/endian/net.h>

#include <cstring>

// ============================================================================
// variable_buffer: prepend(const_pointer, size_type)
// ============================================================================

TEST_CASE("variable_buffer prepend raw bytes", "[variable_buffer][prepend]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x03);
    buf.push_back(0x04);

    const uint8_t hdr[] = { 0x01, 0x02 };
    buf.prepend(hdr, 2);

    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(0) == 0x01);
    REQUIRE(buf.at(1) == 0x02);
    REQUIRE(buf.at(2) == 0x03);
    REQUIRE(buf.at(3) == 0x04);
}

// ============================================================================
// variable_buffer: prepend(const variable_buffer&)
// ============================================================================

TEST_CASE("variable_buffer prepend variable_buffer", "[variable_buffer][prepend]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x03);
    buf.push_back(0x04);

    memepp::variable_buffer other;
    const uint8_t hdr[] = { 0x01, 0x02 };
    other.append(hdr, 2);

    buf.prepend(other);
    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(0) == 0x01);
    REQUIRE(buf.at(1) == 0x02);
    REQUIRE(buf.at(2) == 0x03);
    REQUIRE(buf.at(3) == 0x04);
}

// ============================================================================
// variable_buffer: prepend(const string&)
// ============================================================================

TEST_CASE("variable_buffer prepend string", "[variable_buffer][prepend]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x00);

    memepp::string s = "abc";
    buf.prepend(s);
    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(0) == 'a');
    REQUIRE(buf.at(1) == 'b');
    REQUIRE(buf.at(2) == 'c');
    REQUIRE(buf.at(3) == 0x00);
}

// ============================================================================
// variable_buffer: prepend(const string_view&)
// ============================================================================

TEST_CASE("variable_buffer prepend string_view", "[variable_buffer][prepend]")
{
    memepp::variable_buffer buf;
    buf.push_back(0xFF);

    memepp::string_view sv("xyz");
    buf.prepend(sv);
    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(0) == 'x');
    REQUIRE(buf.at(1) == 'y');
    REQUIRE(buf.at(2) == 'z');
    REQUIRE(buf.at(3) == 0xFF);
}

// ============================================================================
// variable_buffer: prepend(const buffer&)
// ============================================================================

TEST_CASE("variable_buffer prepend buffer", "[variable_buffer][prepend]")
{
    memepp::variable_buffer buf;
    buf.push_back(0xCC);

    const uint8_t raw[] = { 0xAA, 0xBB };
    memepp::buffer b(raw, 2);
    buf.prepend(b);
    REQUIRE(buf.size() == 3);
    REQUIRE(buf.at(0) == 0xAA);
    REQUIRE(buf.at(1) == 0xBB);
    REQUIRE(buf.at(2) == 0xCC);
}

// ============================================================================
// variable_buffer: prepend(const buffer_view&)
// ============================================================================

TEST_CASE("variable_buffer prepend buffer_view", "[variable_buffer][prepend]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x40);

    const uint8_t raw[] = { 0x10, 0x20, 0x30 };
    memepp::buffer_view bv(raw, 3);
    buf.prepend(bv);
    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(0) == 0x10);
    REQUIRE(buf.at(2) == 0x30);
    REQUIRE(buf.at(3) == 0x40);
}

// ============================================================================
// variable_buffer: prepend(const buffer_span&)
// ============================================================================

TEST_CASE("variable_buffer prepend buffer_span", "[variable_buffer][prepend]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x05);

    const uint8_t raw[] = { 0x01, 0x02, 0x03, 0x04 };
    memepp::buffer_span bs(raw, 4);
    buf.prepend(bs);
    REQUIRE(buf.size() == 5);
    REQUIRE(buf.at(0) == 0x01);
    REQUIRE(buf.at(4) == 0x05);
}

// ============================================================================
// variable_buffer: prepend empty
// ============================================================================

TEST_CASE("variable_buffer prepend empty buffer_view", "[variable_buffer][prepend]")
{
    memepp::variable_buffer buf;
    buf.push_back(1);

    memepp::buffer_view empty;
    buf.prepend(empty);
    REQUIRE(buf.size() == 1);
    REQUIRE(buf.at(0) == 1);
}

// ============================================================================
// variable_buffer: prepend to empty buffer
// ============================================================================

TEST_CASE("variable_buffer prepend to empty buffer", "[variable_buffer][prepend]")
{
    memepp::variable_buffer buf;
    const uint8_t hdr[] = { 0xAA, 0xBB };
    buf.prepend(hdr, 2);
    REQUIRE(buf.size() == 2);
    REQUIRE(buf.at(0) == 0xAA);
    REQUIRE(buf.at(1) == 0xBB);
}

// ============================================================================
// variable_buffer: prepend triggers small→medium upgrade
// ============================================================================

TEST_CASE("variable_buffer prepend triggers small to medium upgrade", "[variable_buffer][prepend]")
{
    memepp::variable_buffer buf;
    // Fill with enough data to force medium storage
    for (int i = 0; i < 100; ++i) {
        buf.push_back(static_cast<uint8_t>(i));
    }
    REQUIRE(buf.size() == 100);

    // Prepend more data
    const uint8_t hdr[] = { 0xFE, 0xED, 0xFA, 0xCE };
    buf.prepend(hdr, 4);
    REQUIRE(buf.size() == 104);
    REQUIRE(buf.at(0) == 0xFE);
    REQUIRE(buf.at(1) == 0xED);
    REQUIRE(buf.at(2) == 0xFA);
    REQUIRE(buf.at(3) == 0xCE);
    REQUIRE(buf.at(4) == 0);  // original data[0]
    REQUIRE(buf.at(103) == 99);  // original data[99]
}

// ============================================================================
// variable_buffer: prepend self (self-doubling)
// ============================================================================

TEST_CASE("variable_buffer prepend self doubles content", "[variable_buffer][prepend]")
{
    memepp::variable_buffer buf;
    const uint8_t seed[] = { 1, 2, 3 };
    buf.append(seed, 3);

    // Prepend own data
    buf.prepend(buf.data(), buf.size());
    REQUIRE(buf.size() == 6);
    // Result should be: [1,2,3,1,2,3]
    REQUIRE(buf.at(0) == 1);
    REQUIRE(buf.at(1) == 2);
    REQUIRE(buf.at(2) == 3);
    REQUIRE(buf.at(3) == 1);
    REQUIRE(buf.at(4) == 2);
    REQUIRE(buf.at(5) == 3);
}

TEST_CASE("variable_buffer prepend self variable_buffer", "[variable_buffer][prepend]")
{
    memepp::variable_buffer buf;
    const uint8_t seed[] = { 0xAA, 0xBB };
    buf.append(seed, 2);

    buf.prepend(buf);
    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(0) == 0xAA);
    REQUIRE(buf.at(1) == 0xBB);
    REQUIRE(buf.at(2) == 0xAA);
    REQUIRE(buf.at(3) == 0xBB);
}

// ============================================================================
// variable_buffer: prepend with endian template
// ============================================================================

TEST_CASE("variable_buffer prepend uint16_t little_byte", "[variable_buffer][prepend]")
{
    memepp::variable_buffer buf;
    buf.push_back(0xFF);

    uint16_t val = 0x1234;
    buf.prepend(val, mgpp::endian_t::little_byte);
    REQUIRE(buf.size() == 3);

#if MEGO_ENDIAN__BIG_BYTE
    uint16_t expected = mgpp::endian::hton(val);
#else
    uint16_t expected = val;
#endif
    uint16_t actual;
    memcpy(&actual, buf.data(), sizeof(actual));
    REQUIRE(actual == expected);
    REQUIRE(buf.at(2) == 0xFF);
}

TEST_CASE("variable_buffer prepend uint32_t big_byte", "[variable_buffer][prepend]")
{
    memepp::variable_buffer buf;
    buf.push_back(0xCC);

    uint32_t val = 0xDEADBEEF;
    buf.prepend(val, mgpp::endian_t::big_byte);
    REQUIRE(buf.size() == 5);

#if MEGO_ENDIAN__BIG_BYTE
    uint32_t expected = val;
#else
    uint32_t expected = mgpp::endian::hton(val);
#endif
    uint32_t actual;
    memcpy(&actual, buf.data(), sizeof(actual));
    REQUIRE(actual == expected);
    REQUIRE(buf.at(4) == 0xCC);
}

// ============================================================================
// variable_buffer: repeated prepend
// ============================================================================

TEST_CASE("variable_buffer repeated single-byte prepend", "[variable_buffer][prepend]")
{
    memepp::variable_buffer buf;
    for (int i = 0; i < 50; ++i) {
        buf.push_front(static_cast<uint8_t>(i));
    }
    REQUIRE(buf.size() == 50);
    REQUIRE(buf.front() == 49);
    REQUIRE(buf.back() == 0);

    // Now prepend more
    for (int i = 50; i < 100; ++i) {
        buf.push_front(static_cast<uint8_t>(i));
    }
    REQUIRE(buf.size() == 100);
    REQUIRE(buf.front() == 99);
    REQUIRE(buf.at(49) == 50);
    REQUIRE(buf.at(50) == 49);
    REQUIRE(buf.back() == 0);
}

// ============================================================================
// variable_buffer: prepend then append interleaved
// ============================================================================

TEST_CASE("variable_buffer prepend and append interleaved", "[variable_buffer][prepend]")
{
    memepp::variable_buffer buf;

    const uint8_t hdr[] = { 0x01, 0x02 };
    buf.prepend(hdr, 2);
    REQUIRE(buf.size() == 2);

    const uint8_t tail[] = { 0x05, 0x06 };
    buf.append(tail, 2);
    REQUIRE(buf.size() == 4);

    const uint8_t mid[] = { 0x03, 0x04 };
    buf.insert(buf.cbegin() + 2, mid, 2);
    REQUIRE(buf.size() == 6);

    REQUIRE(buf.at(0) == 0x01);
    REQUIRE(buf.at(1) == 0x02);
    REQUIRE(buf.at(2) == 0x03);
    REQUIRE(buf.at(3) == 0x04);
    REQUIRE(buf.at(4) == 0x05);
    REQUIRE(buf.at(5) == 0x06);
}
