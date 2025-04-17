
#include <catch2/catch.hpp>

#include <memepp/variable_buffer.hpp>
#include <megopp/endian/net.h>

TEST_CASE("memepp::variable_buffer::push_front", "[variable_buffer]") 
{
    memepp::variable_buffer buf01_01;

    for (int i = 0; i < 1024; ++i) {
        buf01_01.push_front(static_cast<uint8_t>(i));
        REQUIRE(buf01_01.size () == i + 1);
        REQUIRE(buf01_01.front() == static_cast<uint8_t>(i));
        REQUIRE(buf01_01.back () == 0);
    }
    REQUIRE(buf01_01.size() == 1024);

    do {
        memepp::variable_buffer buf01_02;
        for (int i = 0; i < 1024; ++i) {
            buf01_02.push_front(static_cast<uint16_t>(i), mgpp::endian_t::little_byte);
            REQUIRE(buf01_02.size () == (i + 1) * sizeof(uint16_t));
#if MEGO_ENDIAN__BIG_BYTE
            uint16_t value = mgpp::endian::hton(static_cast<uint16_t>(i));
#else
            uint16_t value = static_cast<uint16_t>(i);
#endif
            REQUIRE(memcmp(buf01_02.data(), &value, sizeof(uint16_t)) == 0);
        }
    } while (0);

    do {
        memepp::variable_buffer buf01_03;
        for (int i = 0; i < 512; ++i) {
            buf01_03.push_front(static_cast<uint32_t>(i), mgpp::endian_t::little_byte);
            REQUIRE(buf01_03.size () == (i + 1) * sizeof(uint32_t));
#if MEGO_ENDIAN__BIG_BYTE
            uint32_t value = mgpp::endian::hton(static_cast<uint32_t>(i));
#else
            uint32_t value = static_cast<uint32_t>(i);
#endif
            REQUIRE(memcmp(buf01_03.data(), &value, sizeof(uint32_t)) == 0);
        }
    } while (0);

    do {
        memepp::variable_buffer buf01_04;
        for (int i = 0; i < 256; ++i) {
            buf01_04.push_front(static_cast<uint64_t>(i), mgpp::endian_t::little_byte);
            REQUIRE(buf01_04.size () == (i + 1) * sizeof(uint64_t));
#if MEGO_ENDIAN__BIG_BYTE
            uint64_t value = mgpp::endian::hton(static_cast<uint64_t>(i));
#else
            uint64_t value = static_cast<uint64_t>(i);
#endif
            REQUIRE(memcmp(buf01_04.data(), &value, sizeof(uint64_t)) == 0);
        }
    } while (0);

    do {
        memepp::variable_buffer buf01_05;
        for (int i = 0; i < 512; ++i) {
            buf01_05.push_front(static_cast<float>(i), mgpp::endian_t::little_byte);
            REQUIRE(buf01_05.size () == (i + 1) * sizeof(float));
#if MEGO_ENDIAN__BIG_BYTE
            float value = mgpp::endian::hton(static_cast<float>(i));
#else
            float value = static_cast<float>(i);
#endif
            REQUIRE(memcmp(buf01_05.data(), &value, sizeof(float)) == 0);
        }
    } while (0);

    do {
        memepp::variable_buffer buf01_06;
        for (int i = 0; i < 256; ++i) {
            buf01_06.push_front(static_cast<double>(i), mgpp::endian_t::little_byte);
            REQUIRE(buf01_06.size () == (i + 1) * sizeof(double));
#if MEGO_ENDIAN__BIG_BYTE
            double value = mgpp::endian::hton(static_cast<double>(i));
#else
            double value = static_cast<double>(i);
#endif
            REQUIRE(memcmp(buf01_06.data(), &value, sizeof(double)) == 0);
        }
    } while (0);

    do {
        memepp::variable_buffer buf01_07;
        for (int i = 0; i < 1024; ++i) {
            buf01_07.push_front(static_cast<uint16_t>(i), mgpp::endian_t::big_byte);
            REQUIRE(buf01_07.size () == (i + 1) * sizeof(uint16_t));
#if MEGO_ENDIAN__BIG_BYTE
            uint16_t value = static_cast<uint16_t>(i);
#else
            uint16_t value = mgpp::endian::hton(static_cast<uint16_t>(i));
#endif
            REQUIRE(memcmp(buf01_07.data(), &value, sizeof(uint16_t)) == 0);
        }
    } while (0);

    do {
        memepp::variable_buffer buf01_08;
        for (int i = 0; i < 512; ++i) {
            buf01_08.push_front(static_cast<uint32_t>(i), mgpp::endian_t::big_byte);
            REQUIRE(buf01_08.size () == (i + 1) * sizeof(uint32_t));
#if MEGO_ENDIAN__BIG_BYTE
            uint32_t value = static_cast<uint32_t>(i);
#else
            uint32_t value = mgpp::endian::hton(static_cast<uint32_t>(i));
#endif
            REQUIRE(memcmp(buf01_08.data(), &value, sizeof(uint32_t)) == 0);
        }
    } while (0);

    do {
        memepp::variable_buffer buf01_09;
        for (int i = 0; i < 256; ++i) {
            buf01_09.push_front(static_cast<uint64_t>(i), mgpp::endian_t::big_byte);
            REQUIRE(buf01_09.size () == (i + 1) * sizeof(uint64_t));
#if MEGO_ENDIAN__BIG_BYTE
            uint64_t value = static_cast<uint64_t>(i);
#else
            uint64_t value = mgpp::endian::hton(static_cast<uint64_t>(i));
#endif
            REQUIRE(memcmp(buf01_09.data(), &value, sizeof(uint64_t)) == 0);
        }
    } while (0);

    do {
        memepp::variable_buffer buf01_10;
        for (int i = 0; i < 512; ++i) {
            buf01_10.push_front(static_cast<float>(i), mgpp::endian_t::big_byte);
            REQUIRE(buf01_10.size () == (i + 1) * sizeof(float));
#if MEGO_ENDIAN__BIG_BYTE
            float value = static_cast<float>(i);
#else
            float value = mgpp::endian::hton(static_cast<float>(i));
#endif
            REQUIRE(memcmp(buf01_10.data(), &value, sizeof(float)) == 0);
        }
    } while (0);

    do {
        memepp::variable_buffer buf01_11;
        for (int i = 0; i < 256; ++i) {
            buf01_11.push_front(static_cast<double>(i), mgpp::endian_t::big_byte);
            REQUIRE(buf01_11.size () == (i + 1) * sizeof(double));
#if MEGO_ENDIAN__BIG_BYTE
            double value = static_cast<double>(i);
#else
            double value = mgpp::endian::hton(static_cast<double>(i));
#endif
            REQUIRE(memcmp(buf01_11.data(), &value, sizeof(double)) == 0);
        }
    } while (0);
}
