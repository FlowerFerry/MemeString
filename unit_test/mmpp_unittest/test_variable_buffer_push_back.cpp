
#include <catch2/catch.hpp>

#include <memepp/variable_buffer.hpp>
#include <megopp/endian/net.h>

// Each SECTION drives push_back growth all the way from small into medium
// storage, then asserts at key boundary points (first element, the element
// straddling the small->medium transition, last element) instead of one
// REQUIRE per iteration.

TEST_CASE("memepp::variable_buffer::push_back", "[variable_buffer]")
{
    SECTION("uint8")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 1024; ++i)
            buf.push_back(static_cast<uint8_t>(i));

        REQUIRE(buf.size() == 1024);
        REQUIRE(buf.front() == 0);
        REQUIRE(buf.back() == static_cast<uint8_t>(1023));
        REQUIRE(buf.data()[20] == 20);
        REQUIRE(buf.data()[21] == 21);
    }

    SECTION("uint16 big-endian")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 1024; ++i)
            buf.push_back(static_cast<uint16_t>(i), mgpp::endian_t::big_byte);

        REQUIRE(buf.size() == 1024 * sizeof(uint16_t));
#if MEGO_ENDIAN__BIG_BYTE
        uint16_t first = 0, mid = 10, last = 1023;
#else
        uint16_t first = mgpp::endian::hton(static_cast<uint16_t>(0));
        uint16_t mid = mgpp::endian::hton(static_cast<uint16_t>(10));
        uint16_t last = mgpp::endian::hton(static_cast<uint16_t>(1023));
#endif
        REQUIRE(memcmp(buf.data(), &first, sizeof(uint16_t)) == 0);
        REQUIRE(memcmp(buf.data() + 10 * sizeof(uint16_t), &mid, sizeof(uint16_t)) == 0);
        REQUIRE(memcmp(buf.data() + 1023 * sizeof(uint16_t), &last, sizeof(uint16_t)) == 0);
    }

    SECTION("uint32 big-endian")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 512; ++i)
            buf.push_back(static_cast<uint32_t>(i), mgpp::endian_t::big_byte);

        REQUIRE(buf.size() == 512 * sizeof(uint32_t));
#if MEGO_ENDIAN__BIG_BYTE
        uint32_t first = 0, mid = 5, last = 511;
#else
        uint32_t first = mgpp::endian::hton(static_cast<uint32_t>(0));
        uint32_t mid = mgpp::endian::hton(static_cast<uint32_t>(5));
        uint32_t last = mgpp::endian::hton(static_cast<uint32_t>(511));
#endif
        REQUIRE(memcmp(buf.data(), &first, sizeof(uint32_t)) == 0);
        REQUIRE(memcmp(buf.data() + 5 * sizeof(uint32_t), &mid, sizeof(uint32_t)) == 0);
        REQUIRE(memcmp(buf.data() + 511 * sizeof(uint32_t), &last, sizeof(uint32_t)) == 0);
    }

    SECTION("uint64 big-endian")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 256; ++i)
            buf.push_back(static_cast<uint64_t>(i), mgpp::endian_t::big_byte);

        REQUIRE(buf.size() == 256 * sizeof(uint64_t));
#if MEGO_ENDIAN__BIG_BYTE
        uint64_t first = 0, mid = 2, last = 255;
#else
        uint64_t first = mgpp::endian::hton(static_cast<uint64_t>(0));
        uint64_t mid = mgpp::endian::hton(static_cast<uint64_t>(2));
        uint64_t last = mgpp::endian::hton(static_cast<uint64_t>(255));
#endif
        REQUIRE(memcmp(buf.data(), &first, sizeof(uint64_t)) == 0);
        REQUIRE(memcmp(buf.data() + 2 * sizeof(uint64_t), &mid, sizeof(uint64_t)) == 0);
        REQUIRE(memcmp(buf.data() + 255 * sizeof(uint64_t), &last, sizeof(uint64_t)) == 0);
    }

    SECTION("float big-endian")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 512; ++i)
            buf.push_back(static_cast<float>(i), mgpp::endian_t::big_byte);

        REQUIRE(buf.size() == 512 * sizeof(float));
#if MEGO_ENDIAN__BIG_BYTE
        float first = 0.0f, mid = 5.0f, last = 511.0f;
#else
        float first = mgpp::endian::hton(static_cast<float>(0));
        float mid = mgpp::endian::hton(static_cast<float>(5));
        float last = mgpp::endian::hton(static_cast<float>(511));
#endif
        REQUIRE(memcmp(buf.data(), &first, sizeof(float)) == 0);
        REQUIRE(memcmp(buf.data() + 5 * sizeof(float), &mid, sizeof(float)) == 0);
        REQUIRE(memcmp(buf.data() + 511 * sizeof(float), &last, sizeof(float)) == 0);
    }

    SECTION("double big-endian")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 256; ++i)
            buf.push_back(static_cast<double>(i), mgpp::endian_t::big_byte);

        REQUIRE(buf.size() == 256 * sizeof(double));
#if MEGO_ENDIAN__BIG_BYTE
        double first = 0.0, mid = 2.0, last = 255.0;
#else
        double first = mgpp::endian::hton(static_cast<double>(0));
        double mid = mgpp::endian::hton(static_cast<double>(2));
        double last = mgpp::endian::hton(static_cast<double>(255));
#endif
        REQUIRE(memcmp(buf.data(), &first, sizeof(double)) == 0);
        REQUIRE(memcmp(buf.data() + 2 * sizeof(double), &mid, sizeof(double)) == 0);
        REQUIRE(memcmp(buf.data() + 255 * sizeof(double), &last, sizeof(double)) == 0);
    }

    SECTION("uint16 little-endian")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 1024; ++i)
            buf.push_back(static_cast<uint16_t>(i), mgpp::endian_t::little_byte);

        REQUIRE(buf.size() == 1024 * sizeof(uint16_t));
#if MEGO_ENDIAN__BIG_BYTE
        uint16_t first = mgpp::endian::hton(static_cast<uint16_t>(0));
        uint16_t mid = mgpp::endian::hton(static_cast<uint16_t>(10));
        uint16_t last = mgpp::endian::hton(static_cast<uint16_t>(1023));
#else
        uint16_t first = 0, mid = 10, last = 1023;
#endif
        REQUIRE(memcmp(buf.data(), &first, sizeof(uint16_t)) == 0);
        REQUIRE(memcmp(buf.data() + 10 * sizeof(uint16_t), &mid, sizeof(uint16_t)) == 0);
        REQUIRE(memcmp(buf.data() + 1023 * sizeof(uint16_t), &last, sizeof(uint16_t)) == 0);
    }

    SECTION("uint32 little-endian")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 512; ++i)
            buf.push_back(static_cast<uint32_t>(i), mgpp::endian_t::little_byte);

        REQUIRE(buf.size() == 512 * sizeof(uint32_t));
#if MEGO_ENDIAN__BIG_BYTE
        uint32_t first = mgpp::endian::hton(static_cast<uint32_t>(0));
        uint32_t mid = mgpp::endian::hton(static_cast<uint32_t>(5));
        uint32_t last = mgpp::endian::hton(static_cast<uint32_t>(511));
#else
        uint32_t first = 0, mid = 5, last = 511;
#endif
        REQUIRE(memcmp(buf.data(), &first, sizeof(uint32_t)) == 0);
        REQUIRE(memcmp(buf.data() + 5 * sizeof(uint32_t), &mid, sizeof(uint32_t)) == 0);
        REQUIRE(memcmp(buf.data() + 511 * sizeof(uint32_t), &last, sizeof(uint32_t)) == 0);
    }

    SECTION("uint64 little-endian")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 256; ++i)
            buf.push_back(static_cast<uint64_t>(i), mgpp::endian_t::little_byte);

        REQUIRE(buf.size() == 256 * sizeof(uint64_t));
#if MEGO_ENDIAN__BIG_BYTE
        uint64_t first = mgpp::endian::hton(static_cast<uint64_t>(0));
        uint64_t mid = mgpp::endian::hton(static_cast<uint64_t>(2));
        uint64_t last = mgpp::endian::hton(static_cast<uint64_t>(255));
#else
        uint64_t first = 0, mid = 2, last = 255;
#endif
        REQUIRE(memcmp(buf.data(), &first, sizeof(uint64_t)) == 0);
        REQUIRE(memcmp(buf.data() + 2 * sizeof(uint64_t), &mid, sizeof(uint64_t)) == 0);
        REQUIRE(memcmp(buf.data() + 255 * sizeof(uint64_t), &last, sizeof(uint64_t)) == 0);
    }

    SECTION("float little-endian")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 512; ++i)
            buf.push_back(static_cast<float>(i), mgpp::endian_t::little_byte);

        REQUIRE(buf.size() == 512 * sizeof(float));
#if MEGO_ENDIAN__BIG_BYTE
        float first = mgpp::endian::hton(static_cast<float>(0));
        float mid = mgpp::endian::hton(static_cast<float>(5));
        float last = mgpp::endian::hton(static_cast<float>(511));
#else
        float first = 0.0f, mid = 5.0f, last = 511.0f;
#endif
        REQUIRE(memcmp(buf.data(), &first, sizeof(float)) == 0);
        REQUIRE(memcmp(buf.data() + 5 * sizeof(float), &mid, sizeof(float)) == 0);
        REQUIRE(memcmp(buf.data() + 511 * sizeof(float), &last, sizeof(float)) == 0);
    }

    SECTION("double little-endian")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 256; ++i)
            buf.push_back(static_cast<double>(i), mgpp::endian_t::little_byte);

        REQUIRE(buf.size() == 256 * sizeof(double));
#if MEGO_ENDIAN__BIG_BYTE
        double first = mgpp::endian::hton(static_cast<double>(0));
        double mid = mgpp::endian::hton(static_cast<double>(2));
        double last = mgpp::endian::hton(static_cast<double>(255));
#else
        double first = 0.0, mid = 2.0, last = 255.0;
#endif
        REQUIRE(memcmp(buf.data(), &first, sizeof(double)) == 0);
        REQUIRE(memcmp(buf.data() + 2 * sizeof(double), &mid, sizeof(double)) == 0);
        REQUIRE(memcmp(buf.data() + 255 * sizeof(double), &last, sizeof(double)) == 0);
    }
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
