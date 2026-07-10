
#include <catch2/catch.hpp>

#include <memepp/variable_buffer.hpp>
#include <megopp/endian/net.h>

TEST_CASE("memepp::variable_buffer::push_back", "[variable_buffer]") 
{
    memepp::variable_buffer buf01_01;

    for (int i = 0; i < 1024; ++i) {
        buf01_01.push_back(static_cast<uint8_t>(i));
        REQUIRE(buf01_01.size () == i + 1);
        REQUIRE(buf01_01.front() == 0);
        REQUIRE(buf01_01.back () == static_cast<uint8_t>(i));
    }
    REQUIRE(buf01_01.size() == 1024);

    do {
        memepp::variable_buffer buf01_02;
        for (int i = 0; i < 1024; ++i) {
            buf01_02.push_back(static_cast<uint16_t>(i), mgpp::endian_t::big_byte);
            REQUIRE(buf01_02.size () == (i + 1) * sizeof(uint16_t));
#if MEGO_ENDIAN__BIG_BYTE
            uint16_t value = static_cast<uint16_t>(i);
#else
            uint16_t value = mgpp::endian::hton(static_cast<uint16_t>(i));
#endif
            REQUIRE(memcmp(buf01_02.data() + (i * sizeof(uint16_t)), &value, sizeof(uint16_t)) == 0);
        }
    } while (0);

    do {
        memepp::variable_buffer buf01_03;
        for (int i = 0; i < 512; ++i) {
            buf01_03.push_back(static_cast<uint32_t>(i), mgpp::endian_t::big_byte);
            REQUIRE(buf01_03.size () == (i + 1) * sizeof(uint32_t));
#if MEGO_ENDIAN__BIG_BYTE
            uint32_t value = static_cast<uint32_t>(i);
#else
            uint32_t value = mgpp::endian::hton(static_cast<uint32_t>(i));
#endif
            REQUIRE(memcmp(buf01_03.data() + (i * sizeof(uint32_t)), &value, sizeof(uint32_t)) == 0);
        }
    } while (0);

    do {
        memepp::variable_buffer buf01_04;
        for (int i = 0; i < 256; ++i) {
            buf01_04.push_back(static_cast<uint64_t>(i), mgpp::endian_t::big_byte);
            REQUIRE(buf01_04.size () == (i + 1) * sizeof(uint64_t));
#if MEGO_ENDIAN__BIG_BYTE
            uint64_t value = static_cast<uint64_t>(i);
#else
            uint64_t value = mgpp::endian::hton(static_cast<uint64_t>(i));
#endif
            REQUIRE(memcmp(buf01_04.data() + (i * sizeof(uint64_t)), &value, sizeof(uint64_t)) == 0);
        }
    } while (0);

    do {
        memepp::variable_buffer buf01_05;
        for (int i = 0; i < 512; ++i) {
            buf01_05.push_back(static_cast<float>(i), mgpp::endian_t::big_byte);
            REQUIRE(buf01_05.size () == (i + 1) * sizeof(float));
#if MEGO_ENDIAN__BIG_BYTE
            float value = static_cast<float>(i);
#else
            float value = mgpp::endian::hton(static_cast<float>(i));
#endif
            REQUIRE(memcmp(buf01_05.data() + (i * sizeof(float)), &value, sizeof(float)) == 0);
        }
    } while (0);

    do {
        memepp::variable_buffer buf01_06;
        for (int i = 0; i < 256; ++i) {
            buf01_06.push_back(static_cast<double>(i), mgpp::endian_t::big_byte);
            REQUIRE(buf01_06.size () == (i + 1) * sizeof(double));
#if MEGO_ENDIAN__BIG_BYTE
            double value = static_cast<double>(i);
#else
            double value = mgpp::endian::hton(static_cast<double>(i));
#endif
            REQUIRE(memcmp(buf01_06.data() + (i * sizeof(double)), &value, sizeof(double)) == 0);
        }
    } while (0);
    
    do {
        memepp::variable_buffer buf01_02;
        for (int i = 0; i < 1024; ++i) {
            buf01_02.push_back(static_cast<uint16_t>(i), mgpp::endian_t::little_byte);
            REQUIRE(buf01_02.size () == (i + 1) * sizeof(uint16_t));
#if MEGO_ENDIAN__BIG_BYTE
            uint16_t value = mgpp::endian::hton(static_cast<uint16_t>(i));
#else
            uint16_t value = static_cast<uint16_t>(i);
#endif
            REQUIRE(memcmp(buf01_02.data() + (i * sizeof(uint16_t)), &value, sizeof(uint16_t)) == 0);
        }
    } while (0);

    do {
        memepp::variable_buffer buf01_03;
        for (int i = 0; i < 512; ++i) {
            buf01_03.push_back(static_cast<uint32_t>(i), mgpp::endian_t::little_byte);
            REQUIRE(buf01_03.size () == (i + 1) * sizeof(uint32_t));
#if MEGO_ENDIAN__BIG_BYTE
            uint32_t value = mgpp::endian::hton(static_cast<uint32_t>(i));
#else
            uint32_t value = static_cast<uint32_t>(i);
#endif
            REQUIRE(memcmp(buf01_03.data() + (i * sizeof(uint32_t)), &value, sizeof(uint32_t)) == 0);
        }
    } while (0);

    do {
        memepp::variable_buffer buf01_04;
        for (int i = 0; i < 256; ++i) {
            buf01_04.push_back(static_cast<uint64_t>(i), mgpp::endian_t::little_byte);
            REQUIRE(buf01_04.size () == (i + 1) * sizeof(uint64_t));
#if MEGO_ENDIAN__BIG_BYTE
            uint64_t value = mgpp::endian::hton(static_cast<uint64_t>(i));
#else
            uint64_t value = static_cast<uint64_t>(i);
#endif
            REQUIRE(memcmp(buf01_04.data() + (i * sizeof(uint64_t)), &value, sizeof(uint64_t)) == 0);
        }
    } while (0);

    do {
        memepp::variable_buffer buf01_05;
        for (int i = 0; i < 512; ++i) {
            buf01_05.push_back(static_cast<float>(i), mgpp::endian_t::little_byte);
            REQUIRE(buf01_05.size () == (i + 1) * sizeof(float));
#if MEGO_ENDIAN__BIG_BYTE
            float value = mgpp::endian::hton(static_cast<float>(i));
#else
            float value = static_cast<float>(i);
#endif
            REQUIRE(memcmp(buf01_05.data() + (i * sizeof(float)), &value, sizeof(float)) == 0);
        }
    } while (0);

    do {
        memepp::variable_buffer buf01_06;
        for (int i = 0; i < 256; ++i) {
            buf01_06.push_back(static_cast<double>(i), mgpp::endian_t::little_byte);
            REQUIRE(buf01_06.size () == (i + 1) * sizeof(double));
#if MEGO_ENDIAN__BIG_BYTE
            double value = mgpp::endian::hton(static_cast<double>(i));
#else
            double value = static_cast<double>(i);
#endif
            REQUIRE(memcmp(buf01_06.data() + (i * sizeof(double)), &value, sizeof(double)) == 0);
        }
    } while (0);
}

// ---------------------------------------------------------------------------
// variable_buffer: front / back / pop_back
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer front/back — single element", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(42);
    REQUIRE(buf.front() == 42);
    REQUIRE(buf.back() == 42);
}

TEST_CASE("memepp::variable_buffer front/back — multiple elements", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(10);
    buf.push_back(20);
    buf.push_back(30);
    REQUIRE(buf.front() == 10);
    REQUIRE(buf.back() == 30);
}

TEST_CASE("memepp::variable_buffer front/back — mutable", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(1);
    buf.push_back(2);

    buf.front() = 99;
    REQUIRE(buf.front() == 99);

    buf.back() = 88;
    REQUIRE(buf.back() == 88);
}

TEST_CASE("memepp::variable_buffer pop_back", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(1);
    buf.push_back(2);
    buf.push_back(3);

    buf.pop_back();
    REQUIRE(buf.size() == 2);
    REQUIRE(buf.back() == 2);

    buf.pop_back();
    REQUIRE(buf.size() == 1);
    REQUIRE(buf.back() == 1);

    buf.pop_back();
    REQUIRE(buf.size() == 0);
}

TEST_CASE("memepp::variable_buffer pop_back — empty does not crash", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.pop_back();
    REQUIRE(buf.empty());
}

TEST_CASE("memepp::variable_buffer pop_back — after reserve", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.reserve(100);
    buf.push_back(1);
    buf.push_back(2);
    buf.pop_back();
    REQUIRE(buf.size() == 1);
    REQUIRE(buf.back() == 1);
}

// ---------------------------------------------------------------------------
// variable_buffer: operator+=
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer operator+=", "[variable_buffer]")
{
    memepp::variable_buffer a;
    a.push_back(1);
    a.push_back(2);

    memepp::variable_buffer b;
    b.push_back(3);
    b.push_back(4);

    a += b;
    REQUIRE(a.size() == 4);
    REQUIRE(a.at(0) == 1);
    REQUIRE(a.at(3) == 4);
}

TEST_CASE("memepp::variable_buffer operator+= — move", "[variable_buffer]")
{
    memepp::variable_buffer a;
    a.push_back(1);

    memepp::variable_buffer b;
    b.push_back(2);

    a += std::move(b);
    REQUIRE(a.size() == 2);
    REQUIRE(a.at(1) == 2);
}

TEST_CASE("memepp::variable_buffer operator+= — empty rhs", "[variable_buffer]")
{
    memepp::variable_buffer a;
    a.push_back(1);
    a.push_back(2);

    memepp::variable_buffer b;
    a += b;
    REQUIRE(a.size() == 2);
}

// ---------------------------------------------------------------------------
// variable_buffer: swap
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer swap", "[variable_buffer]")
{
    memepp::variable_buffer a;
    a.push_back(1);
    a.push_back(2);

    memepp::variable_buffer b;
    b.push_back(99);
    b.push_back(100);
    b.push_back(101);

    a.swap(b);
    REQUIRE(a.size() == 3);
    REQUIRE(b.size() == 2);
    REQUIRE(a.front() == 99);
    REQUIRE(b.front() == 1);
}

// ---------------------------------------------------------------------------
// variable_buffer: operator=(string/string_view)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer operator=(string)", "[variable_buffer]")
{
    memepp::string s = "Hello";
    memepp::variable_buffer buf;
    buf = s;
    REQUIRE(buf.size() == 5);
    REQUIRE(memcmp(buf.data(), "Hello", 5) == 0);
}

TEST_CASE("memepp::variable_buffer operator=(string_view)", "[variable_buffer]")
{
    memepp::string_view sv = "World";
    memepp::variable_buffer buf;
    buf = sv;
    REQUIRE(buf.size() == 5);
    REQUIRE(memcmp(buf.data(), "World", 5) == 0);
}

// ---------------------------------------------------------------------------
// variable_buffer: operator[] mutable
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variable_buffer operator[] — mutable", "[variable_buffer]")
{
    memepp::variable_buffer buf;
    buf.push_back(10);
    buf.push_back(20);

    buf[0] = 99;
    REQUIRE(buf[0] == 99);
    REQUIRE(buf.data()[0] == 99);
}
