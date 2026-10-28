#include <catch2/catch.hpp>

#include <memepp/variable_buffer.hpp>

// ---------------------------------------------------------------------------
// variable_buffer::insert -- single byte (const_iterator, const value_type&)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer insert single byte at begin", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x20);
    buf.push_back(0x30);

    buf.insert(buf.cbegin(), (uint8_t)0x10);

    REQUIRE(buf.size() == 3);
    REQUIRE(buf.at(0) == 0x10);
    REQUIRE(buf.at(1) == 0x20);
    REQUIRE(buf.at(2) == 0x30);
}

TEST_CASE("memepp::variable_buffer insert single byte at end", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x10);
    buf.push_back(0x20);

    buf.insert(buf.cend(), (uint8_t)0x30);

    REQUIRE(buf.size() == 3);
    REQUIRE(buf.at(0) == 0x10);
    REQUIRE(buf.at(1) == 0x20);
    REQUIRE(buf.at(2) == 0x30);
}

TEST_CASE("memepp::variable_buffer insert single byte in middle", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(0x10);
    buf.push_back(0x30);

    buf.insert(buf.cbegin() + 1, (uint8_t)0x20);

    REQUIRE(buf.size() == 3);
    REQUIRE(buf.at(0) == 0x10);
    REQUIRE(buf.at(1) == 0x20);
    REQUIRE(buf.at(2) == 0x30);
}

TEST_CASE("memepp::variable_buffer insert single byte into empty", "[variable_buffer]")
{
    memepp::variable_buffer buf;

    buf.insert(buf.cbegin(), (uint8_t)0xAA);

    REQUIRE(buf.size() == 1);
    REQUIRE(buf.at(0) == 0xAA);
}

TEST_CASE("memepp::variable_buffer insert single byte -- multiple inserts", "[variable_buffer]")
{
    memepp::variable_buffer buf;

    buf.insert(buf.cbegin(), (uint8_t)0x30);
    buf.insert(buf.cbegin(), (uint8_t)0x10);
    buf.insert(buf.cbegin() + 1, (uint8_t)0x20);

    REQUIRE(buf.size() == 3);
    REQUIRE(buf.at(0) == 0x10);
    REQUIRE(buf.at(1) == 0x20);
    REQUIRE(buf.at(2) == 0x30);
}

TEST_CASE("memepp::variable_buffer insert single byte -- after resize", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.resize(4, 0xFF);

    buf.insert(buf.cbegin() + 2, (uint8_t)0x77);

    REQUIRE(buf.size() == 5);
    REQUIRE(buf.at(0) == 0xFF);
    REQUIRE(buf.at(1) == 0xFF);
    REQUIRE(buf.at(2) == 0x77);
    REQUIRE(buf.at(3) == 0xFF);
    REQUIRE(buf.at(4) == 0xFF);
}

TEST_CASE("memepp::variable_buffer insert single byte -- large buffer", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    for (int i = 0; i < 100; ++i)
        buf.push_back((uint8_t)i);

    buf.insert(buf.cbegin() + 50, (uint8_t)0xFF);

    REQUIRE(buf.size() == 101);
    REQUIRE(buf.at(49) == 49);
    REQUIRE(buf.at(50) == 0xFF);
    REQUIRE(buf.at(51) == 50);
}

// ============================================================================
// variable_buffer: insert self (alias safety)
// ============================================================================

TEST_CASE("variable_buffer insert self bytes at position", "[variable_buffer][insert]")
{
    memepp::variable_buffer buf;
    const uint8_t seed[] = { 1, 2, 3 };
    buf.append(seed, 3);

    // Insert buf's own data at position 1 — aliasing
    buf.insert(buf.cbegin() + 1, buf.data(), buf.size());

    REQUIRE(buf.size() == 6);
    REQUIRE(buf.at(0) == 1);
    REQUIRE(buf.at(1) == 1);
    REQUIRE(buf.at(2) == 2);
    REQUIRE(buf.at(3) == 3);
    REQUIRE(buf.at(4) == 2);
    REQUIRE(buf.at(5) == 3);
}

TEST_CASE("variable_buffer insert self variable_buffer at begin", "[variable_buffer][insert]")
{
    memepp::variable_buffer buf;
    const uint8_t seed[] = { 0xAA, 0xBB };
    buf.append(seed, 2);

    buf.insert(buf.cbegin(), buf);

    REQUIRE(buf.size() == 4);
    REQUIRE(buf.at(0) == 0xAA);
    REQUIRE(buf.at(1) == 0xBB);
    REQUIRE(buf.at(2) == 0xAA);
    REQUIRE(buf.at(3) == 0xBB);
}

TEST_CASE("variable_buffer insert self forcing reallocation", "[variable_buffer][insert]")
{
    memepp::variable_buffer buf;
    const uint8_t seed[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    buf.append(seed, 8);

    // Shrink capacity to exactly size so the self-insert must reallocate.
    buf.reserve(buf.size());
    buf.insert(buf.cbegin() + 2, buf.data(), buf.size());

    REQUIRE(buf.size() == 16);
    REQUIRE(buf.at(0) == 1);
    REQUIRE(buf.at(1) == 2);
    REQUIRE(buf.at(2) == 1);
    REQUIRE(buf.at(3) == 2);
    REQUIRE(buf.at(4) == 3);
    REQUIRE(buf.at(5) == 4);
    REQUIRE(buf.at(6) == 5);
    REQUIRE(buf.at(7) == 6);
    REQUIRE(buf.at(8) == 7);
    REQUIRE(buf.at(9) == 8);
    REQUIRE(buf.at(10) == 3);
    REQUIRE(buf.at(11) == 4);
}
