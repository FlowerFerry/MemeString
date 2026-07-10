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
