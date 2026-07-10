#include <catch2/catch.hpp>

#include <memepp/variable_buffer.hpp>

// ---------------------------------------------------------------------------
// variable_buffer::starts_with(const_pointer, size_type)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer starts_with bytes", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 0x01, 0x02, 0x03, 0x04 };
    buf.append(data, sizeof(data));

    REQUIRE(buf.starts_with(data, 2));
    REQUIRE_FALSE(buf.starts_with(data + 1, 3));
    REQUIRE(buf.starts_with(data, 0));
    REQUIRE_FALSE(buf.starts_with(data, 100));
}

TEST_CASE("memepp::variable_buffer starts_with bytes — empty buffer", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 0x01, 0x02 };

    REQUIRE(buf.starts_with(data, 0));
    REQUIRE_FALSE(buf.starts_with(data, 1));
}

TEST_CASE("memepp::variable_buffer starts_with bytes — exact match", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 0xAA, 0xBB, 0xCC };
    buf.append(data, sizeof(data));

    REQUIRE(buf.starts_with(data, 3));
}

// ---------------------------------------------------------------------------
// variable_buffer::starts_with(value_type)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer starts_with single byte", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x42);
    buf.push_back(0x43);

    REQUIRE(buf.starts_with((uint8_t)0x42));
    REQUIRE_FALSE(buf.starts_with((uint8_t)0x43));
    REQUIRE_FALSE(buf.starts_with((uint8_t)0xFF));
}

TEST_CASE("memepp::variable_buffer starts_with single byte — empty", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    REQUIRE_FALSE(buf.starts_with((uint8_t)0x00));
}

// ---------------------------------------------------------------------------
// variable_buffer::ends_with(const_pointer, size_type)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer ends_with bytes", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 0x01, 0x02, 0x03, 0x04 };
    buf.append(data, sizeof(data));

    REQUIRE(buf.ends_with(data + 2, 2));
    REQUIRE_FALSE(buf.ends_with(data, 3));
    REQUIRE(buf.ends_with(data, 0));
    REQUIRE_FALSE(buf.ends_with(data, 100));
}

TEST_CASE("memepp::variable_buffer ends_with bytes — empty buffer", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 0x01, 0x02 };

    REQUIRE(buf.ends_with(data, 0));
    REQUIRE_FALSE(buf.ends_with(data, 1));
}

TEST_CASE("memepp::variable_buffer ends_with bytes — exact match", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 0xAA, 0xBB, 0xCC };
    buf.append(data, sizeof(data));

    REQUIRE(buf.ends_with(data, 3));
}

// ---------------------------------------------------------------------------
// variable_buffer::ends_with(value_type)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer ends_with single byte", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x42);
    buf.push_back(0x43);

    REQUIRE(buf.ends_with((uint8_t)0x43));
    REQUIRE_FALSE(buf.ends_with((uint8_t)0x42));
    REQUIRE_FALSE(buf.ends_with((uint8_t)0xFF));
}

TEST_CASE("memepp::variable_buffer ends_with single byte — empty", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    REQUIRE_FALSE(buf.ends_with((uint8_t)0x00));
}

// ---------------------------------------------------------------------------
// variable_buffer::contains(const_pointer, size_type)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer contains bytes", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t data[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
    buf.append(data, sizeof(data));

    REQUIRE(buf.contains(data + 1, 3));
    REQUIRE(buf.contains(data, 5));
    REQUIRE_FALSE(buf.contains(data, 6));
}

TEST_CASE("memepp::variable_buffer contains bytes — single byte", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x7F);

    REQUIRE(buf.contains((const uint8_t*)buf.data(), 1));
    REQUIRE_FALSE(buf.contains((const uint8_t*)"\xFF", 1));
}

TEST_CASE("memepp::variable_buffer contains bytes — empty buffer", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    const uint8_t needle[] = { 0x01 };

    REQUIRE_FALSE(buf.contains(needle, 1));
}

// ---------------------------------------------------------------------------
// variable_buffer::contains(value_type)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer contains single byte", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x10);
    buf.push_back(0x20);
    buf.push_back(0x30);

    REQUIRE(buf.contains((uint8_t)0x10));
    REQUIRE(buf.contains((uint8_t)0x20));
    REQUIRE(buf.contains((uint8_t)0x30));
    REQUIRE_FALSE(buf.contains((uint8_t)0x40));
}

TEST_CASE("memepp::variable_buffer contains single byte — empty", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    REQUIRE_FALSE(buf.contains((uint8_t)0x00));
}

// ---------------------------------------------------------------------------
// variable_buffer::starts_with / ends_with / contains — large data
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer starts_with ends_with contains — large", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    for (int i = 0; i < 100; ++i)
        buf.push_back((uint8_t)i);

    const uint8_t prefix[] = { 0, 1, 2, 3 };
    const uint8_t suffix[] = { 96, 97, 98, 99 };
    const uint8_t middle[] = { 50, 51, 52 };
    const uint8_t absent[] = { 0xFF, 0xFE };

    REQUIRE(buf.starts_with(prefix, 4));
    REQUIRE(buf.ends_with(suffix, 4));
    REQUIRE(buf.contains(middle, 3));
    REQUIRE_FALSE(buf.contains(absent, 2));
    REQUIRE_FALSE(buf.starts_with(absent, 1));
    REQUIRE_FALSE(buf.ends_with(absent, 1));
}
