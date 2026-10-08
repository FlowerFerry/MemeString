#include <catch2/catch.hpp>

#include <memepp/variable_buffer.hpp>

#include <cstdint>
#include <cstring>

// ============================================================================
// variable_buffer::reverse() — in-place, binary-safe, no allocation
// ============================================================================

TEST_CASE("variable_buffer reverse empty", "[variable_buffer][reverse]")
{
    memepp::variable_buffer buf;
    buf.reverse();
    REQUIRE(buf.size() == 0);
}

TEST_CASE("variable_buffer reverse single byte", "[variable_buffer][reverse]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x42);
    buf.reverse();
    REQUIRE(buf.size() == 1);
    REQUIRE(buf.at(0) == 0x42);
}

TEST_CASE("variable_buffer reverse even length", "[variable_buffer][reverse]")
{
    memepp::variable_buffer buf;
    const uint8_t src[] = { 0x01, 0x02, 0x03, 0x04 };
    buf.append(src, 4);
    buf.reverse();
    const uint8_t exp[] = { 0x04, 0x03, 0x02, 0x01 };
    REQUIRE(buf.size() == 4);
    REQUIRE(memcmp(buf.data(), exp, 4) == 0);
}

TEST_CASE("variable_buffer reverse odd length", "[variable_buffer][reverse]")
{
    memepp::variable_buffer buf;
    const uint8_t src[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
    buf.append(src, 5);
    buf.reverse();
    const uint8_t exp[] = { 0x05, 0x04, 0x03, 0x02, 0x01 };
    REQUIRE(buf.size() == 5);
    REQUIRE(buf.at(2) == 0x03);
    REQUIRE(memcmp(buf.data(), exp, 5) == 0);
}

TEST_CASE("variable_buffer reverse binary with NUL and 0xFF", "[variable_buffer][reverse]")
{
    memepp::variable_buffer buf;
    const uint8_t src[] = { 0x00, 0xFF, 0x00, 0x7F, 0x80 };
    buf.append(src, 5);
    buf.reverse();
    const uint8_t exp[] = { 0x80, 0x7F, 0x00, 0xFF, 0x00 };
    REQUIRE(buf.size() == 5);
    REQUIRE(memcmp(buf.data(), exp, 5) == 0);
}

TEST_CASE("variable_buffer reverse medium storage (>21 bytes)", "[variable_buffer][reverse]")
{
    memepp::variable_buffer buf;
    uint8_t src[32];
    for (int i = 0; i < 32; ++i)
        src[i] = static_cast<uint8_t>(31 - i);
    buf.append(src, 32);
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::medium);
    buf.reverse();
    REQUIRE(buf.size() == 32);
    for (int i = 0; i < 32; ++i)
        REQUIRE(buf.at(i) == static_cast<uint8_t>(i));
}

TEST_CASE("variable_buffer reverse twice restores original", "[variable_buffer][reverse]")
{
    memepp::variable_buffer buf;
    const uint8_t src[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01, 0x02 };
    buf.append(src, 6);
    buf.reverse();
    REQUIRE(memcmp(buf.data(), src, 6) != 0);
    buf.reverse();
    REQUIRE(memcmp(buf.data(), src, 6) == 0);
}

TEST_CASE("variable_buffer reverse returns *this (chaining)", "[variable_buffer][reverse]")
{
    memepp::variable_buffer buf;
    const uint8_t src[] = { 0x01, 0x02, 0x03, 0x04 };
    buf.append(src, 4);
    memepp::variable_buffer& ref = buf.reverse().reverse();
    REQUIRE(&ref == &buf);
    REQUIRE(memcmp(buf.data(), src, 4) == 0);
}
