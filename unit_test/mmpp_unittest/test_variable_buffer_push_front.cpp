
#include <catch2/catch.hpp>

#include <memepp/variable_buffer.hpp>
#include <megopp/endian/net.h>

// Each SECTION drives push_front growth all the way from small into medium
// storage, then asserts the resulting layout once instead of per iteration.
//
// After push_front(i) for i = 0..N-1 the buffer must read
//   element[k] == N-1-k   (last pushed at front, first pushed at back)
// so checking front, middle, the small->medium boundary and back catches
// memmove / front_capacity corruption that a front-only check would miss.

TEST_CASE("memepp::variable_buffer::push_front", "[variable_buffer]")
{
    SECTION("uint8")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 1024; ++i)
            buf.push_front(static_cast<uint8_t>(i));

        REQUIRE(buf.size() == 1024);
        REQUIRE(buf.front() == static_cast<uint8_t>(1023));
        REQUIRE(buf.back() == 0);
        // element[k] == 1023 - k
        REQUIRE(buf.data()[512] == static_cast<uint8_t>(511));
        REQUIRE(buf.data()[20] == static_cast<uint8_t>(1003));
        REQUIRE(buf.data()[21] == static_cast<uint8_t>(1002));
    }

    SECTION("uint16 little-endian")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 1024; ++i)
            buf.push_front(static_cast<uint16_t>(i), mgpp::endian_t::little_byte);

        REQUIRE(buf.size() == 1024 * sizeof(uint16_t));
#if MEGO_ENDIAN__BIG_BYTE
        uint16_t front = mgpp::endian::hton(static_cast<uint16_t>(1023));
        uint16_t mid = mgpp::endian::hton(static_cast<uint16_t>(511));
        uint16_t boundary = mgpp::endian::hton(static_cast<uint16_t>(1013));
        uint16_t back = mgpp::endian::hton(static_cast<uint16_t>(0));
#else
        uint16_t front = 1023, mid = 511, boundary = 1013, back = 0;
#endif
        REQUIRE(memcmp(buf.data(), &front, sizeof(uint16_t)) == 0);
        REQUIRE(memcmp(buf.data() + 512 * sizeof(uint16_t), &mid, sizeof(uint16_t)) == 0);
        // 10 elements = 20 bytes (small); element 10 first in medium
        REQUIRE(memcmp(buf.data() + 10 * sizeof(uint16_t), &boundary, sizeof(uint16_t)) == 0);
        REQUIRE(memcmp(buf.data() + 1023 * sizeof(uint16_t), &back, sizeof(uint16_t)) == 0);
    }

    SECTION("uint32 little-endian")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 512; ++i)
            buf.push_front(static_cast<uint32_t>(i), mgpp::endian_t::little_byte);

        REQUIRE(buf.size() == 512 * sizeof(uint32_t));
#if MEGO_ENDIAN__BIG_BYTE
        uint32_t front = mgpp::endian::hton(static_cast<uint32_t>(511));
        uint32_t mid = mgpp::endian::hton(static_cast<uint32_t>(255));
        uint32_t boundary = mgpp::endian::hton(static_cast<uint32_t>(506));
        uint32_t back = mgpp::endian::hton(static_cast<uint32_t>(0));
#else
        uint32_t front = 511, mid = 255, boundary = 506, back = 0;
#endif
        REQUIRE(memcmp(buf.data(), &front, sizeof(uint32_t)) == 0);
        REQUIRE(memcmp(buf.data() + 256 * sizeof(uint32_t), &mid, sizeof(uint32_t)) == 0);
        REQUIRE(memcmp(buf.data() + 5 * sizeof(uint32_t), &boundary, sizeof(uint32_t)) == 0);
        REQUIRE(memcmp(buf.data() + 511 * sizeof(uint32_t), &back, sizeof(uint32_t)) == 0);
    }

    SECTION("uint64 little-endian")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 256; ++i)
            buf.push_front(static_cast<uint64_t>(i), mgpp::endian_t::little_byte);

        REQUIRE(buf.size() == 256 * sizeof(uint64_t));
#if MEGO_ENDIAN__BIG_BYTE
        uint64_t front = mgpp::endian::hton(static_cast<uint64_t>(255));
        uint64_t mid = mgpp::endian::hton(static_cast<uint64_t>(127));
        uint64_t boundary = mgpp::endian::hton(static_cast<uint64_t>(253));
        uint64_t back = mgpp::endian::hton(static_cast<uint64_t>(0));
#else
        uint64_t front = 255, mid = 127, boundary = 253, back = 0;
#endif
        REQUIRE(memcmp(buf.data(), &front, sizeof(uint64_t)) == 0);
        REQUIRE(memcmp(buf.data() + 128 * sizeof(uint64_t), &mid, sizeof(uint64_t)) == 0);
        REQUIRE(memcmp(buf.data() + 2 * sizeof(uint64_t), &boundary, sizeof(uint64_t)) == 0);
        REQUIRE(memcmp(buf.data() + 255 * sizeof(uint64_t), &back, sizeof(uint64_t)) == 0);
    }

    SECTION("float little-endian")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 512; ++i)
            buf.push_front(static_cast<float>(i), mgpp::endian_t::little_byte);

        REQUIRE(buf.size() == 512 * sizeof(float));
#if MEGO_ENDIAN__BIG_BYTE
        float front = mgpp::endian::hton(static_cast<float>(511));
        float mid = mgpp::endian::hton(static_cast<float>(255));
        float boundary = mgpp::endian::hton(static_cast<float>(506));
        float back = mgpp::endian::hton(static_cast<float>(0));
#else
        float front = 511.0f, mid = 255.0f, boundary = 506.0f, back = 0.0f;
#endif
        REQUIRE(memcmp(buf.data(), &front, sizeof(float)) == 0);
        REQUIRE(memcmp(buf.data() + 256 * sizeof(float), &mid, sizeof(float)) == 0);
        REQUIRE(memcmp(buf.data() + 5 * sizeof(float), &boundary, sizeof(float)) == 0);
        REQUIRE(memcmp(buf.data() + 511 * sizeof(float), &back, sizeof(float)) == 0);
    }

    SECTION("double little-endian")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 256; ++i)
            buf.push_front(static_cast<double>(i), mgpp::endian_t::little_byte);

        REQUIRE(buf.size() == 256 * sizeof(double));
#if MEGO_ENDIAN__BIG_BYTE
        double front = mgpp::endian::hton(static_cast<double>(255));
        double mid = mgpp::endian::hton(static_cast<double>(127));
        double boundary = mgpp::endian::hton(static_cast<double>(253));
        double back = mgpp::endian::hton(static_cast<double>(0));
#else
        double front = 255.0, mid = 127.0, boundary = 253.0, back = 0.0;
#endif
        REQUIRE(memcmp(buf.data(), &front, sizeof(double)) == 0);
        REQUIRE(memcmp(buf.data() + 128 * sizeof(double), &mid, sizeof(double)) == 0);
        REQUIRE(memcmp(buf.data() + 2 * sizeof(double), &boundary, sizeof(double)) == 0);
        REQUIRE(memcmp(buf.data() + 255 * sizeof(double), &back, sizeof(double)) == 0);
    }

    SECTION("uint16 big-endian")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 1024; ++i)
            buf.push_front(static_cast<uint16_t>(i), mgpp::endian_t::big_byte);

        REQUIRE(buf.size() == 1024 * sizeof(uint16_t));
#if MEGO_ENDIAN__BIG_BYTE
        uint16_t front = 1023, mid = 511, boundary = 1013, back = 0;
#else
        uint16_t front = mgpp::endian::hton(static_cast<uint16_t>(1023));
        uint16_t mid = mgpp::endian::hton(static_cast<uint16_t>(511));
        uint16_t boundary = mgpp::endian::hton(static_cast<uint16_t>(1013));
        uint16_t back = mgpp::endian::hton(static_cast<uint16_t>(0));
#endif
        REQUIRE(memcmp(buf.data(), &front, sizeof(uint16_t)) == 0);
        REQUIRE(memcmp(buf.data() + 512 * sizeof(uint16_t), &mid, sizeof(uint16_t)) == 0);
        REQUIRE(memcmp(buf.data() + 10 * sizeof(uint16_t), &boundary, sizeof(uint16_t)) == 0);
        REQUIRE(memcmp(buf.data() + 1023 * sizeof(uint16_t), &back, sizeof(uint16_t)) == 0);
    }

    SECTION("uint32 big-endian")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 512; ++i)
            buf.push_front(static_cast<uint32_t>(i), mgpp::endian_t::big_byte);

        REQUIRE(buf.size() == 512 * sizeof(uint32_t));
#if MEGO_ENDIAN__BIG_BYTE
        uint32_t front = 511, mid = 255, boundary = 506, back = 0;
#else
        uint32_t front = mgpp::endian::hton(static_cast<uint32_t>(511));
        uint32_t mid = mgpp::endian::hton(static_cast<uint32_t>(255));
        uint32_t boundary = mgpp::endian::hton(static_cast<uint32_t>(506));
        uint32_t back = mgpp::endian::hton(static_cast<uint32_t>(0));
#endif
        REQUIRE(memcmp(buf.data(), &front, sizeof(uint32_t)) == 0);
        REQUIRE(memcmp(buf.data() + 256 * sizeof(uint32_t), &mid, sizeof(uint32_t)) == 0);
        REQUIRE(memcmp(buf.data() + 5 * sizeof(uint32_t), &boundary, sizeof(uint32_t)) == 0);
        REQUIRE(memcmp(buf.data() + 511 * sizeof(uint32_t), &back, sizeof(uint32_t)) == 0);
    }

    SECTION("uint64 big-endian")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 256; ++i)
            buf.push_front(static_cast<uint64_t>(i), mgpp::endian_t::big_byte);

        REQUIRE(buf.size() == 256 * sizeof(uint64_t));
#if MEGO_ENDIAN__BIG_BYTE
        uint64_t front = 255, mid = 127, boundary = 253, back = 0;
#else
        uint64_t front = mgpp::endian::hton(static_cast<uint64_t>(255));
        uint64_t mid = mgpp::endian::hton(static_cast<uint64_t>(127));
        uint64_t boundary = mgpp::endian::hton(static_cast<uint64_t>(253));
        uint64_t back = mgpp::endian::hton(static_cast<uint64_t>(0));
#endif
        REQUIRE(memcmp(buf.data(), &front, sizeof(uint64_t)) == 0);
        REQUIRE(memcmp(buf.data() + 128 * sizeof(uint64_t), &mid, sizeof(uint64_t)) == 0);
        REQUIRE(memcmp(buf.data() + 2 * sizeof(uint64_t), &boundary, sizeof(uint64_t)) == 0);
        REQUIRE(memcmp(buf.data() + 255 * sizeof(uint64_t), &back, sizeof(uint64_t)) == 0);
    }

    SECTION("float big-endian")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 512; ++i)
            buf.push_front(static_cast<float>(i), mgpp::endian_t::big_byte);

        REQUIRE(buf.size() == 512 * sizeof(float));
#if MEGO_ENDIAN__BIG_BYTE
        float front = 511.0f, mid = 255.0f, boundary = 506.0f, back = 0.0f;
#else
        float front = mgpp::endian::hton(static_cast<float>(511));
        float mid = mgpp::endian::hton(static_cast<float>(255));
        float boundary = mgpp::endian::hton(static_cast<float>(506));
        float back = mgpp::endian::hton(static_cast<float>(0));
#endif
        REQUIRE(memcmp(buf.data(), &front, sizeof(float)) == 0);
        REQUIRE(memcmp(buf.data() + 256 * sizeof(float), &mid, sizeof(float)) == 0);
        REQUIRE(memcmp(buf.data() + 5 * sizeof(float), &boundary, sizeof(float)) == 0);
        REQUIRE(memcmp(buf.data() + 511 * sizeof(float), &back, sizeof(float)) == 0);
    }

    SECTION("double big-endian")
    {
        memepp::variable_buffer buf;
        for (int i = 0; i < 256; ++i)
            buf.push_front(static_cast<double>(i), mgpp::endian_t::big_byte);

        REQUIRE(buf.size() == 256 * sizeof(double));
#if MEGO_ENDIAN__BIG_BYTE
        double front = 255.0, mid = 127.0, boundary = 253.0, back = 0.0;
#else
        double front = mgpp::endian::hton(static_cast<double>(255));
        double mid = mgpp::endian::hton(static_cast<double>(127));
        double boundary = mgpp::endian::hton(static_cast<double>(253));
        double back = mgpp::endian::hton(static_cast<double>(0));
#endif
        REQUIRE(memcmp(buf.data(), &front, sizeof(double)) == 0);
        REQUIRE(memcmp(buf.data() + 128 * sizeof(double), &mid, sizeof(double)) == 0);
        REQUIRE(memcmp(buf.data() + 2 * sizeof(double), &boundary, sizeof(double)) == 0);
        REQUIRE(memcmp(buf.data() + 255 * sizeof(double), &back, sizeof(double)) == 0);
    }
}
