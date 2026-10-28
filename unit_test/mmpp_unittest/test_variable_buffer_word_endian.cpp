#include <catch2/catch.hpp>
#include <memepp/variable_buffer.hpp>
#include <megopp/endian/net.h>

// Test word endian modes (big_word, little_word) for variable_buffer methods.
// These modes apply word_swap before byte_swap, affecting 4+ byte types.
// We use mgpp::endian::htot() to compute expected values, which handles
// host endianness automatically.

TEST_CASE("variable_buffer::push_back with word endian", "[variable_buffer][endian]")
{
    SECTION("uint32_t big_word")
    {
        memepp::variable_buffer buf;
        uint32_t val = 0x12345678;
        buf.push_back(val, mgpp::endian_t::big_word);
        
        uint32_t expected = mgpp::endian::htot(val, mgpp::endian_t::big_word);
        REQUIRE(buf.size() == sizeof(uint32_t));
        REQUIRE(memcmp(buf.data(), &expected, sizeof(uint32_t)) == 0);
    }
    
    SECTION("uint32_t little_word")
    {
        memepp::variable_buffer buf;
        uint32_t val = 0x12345678;
        buf.push_back(val, mgpp::endian_t::little_word);
        
        uint32_t expected = mgpp::endian::htot(val, mgpp::endian_t::little_word);
        REQUIRE(buf.size() == sizeof(uint32_t));
        REQUIRE(memcmp(buf.data(), &expected, sizeof(uint32_t)) == 0);
    }
    
    SECTION("uint64_t big_word")
    {
        memepp::variable_buffer buf;
        uint64_t val = 0x0123456789ABCDEF;
        buf.push_back(val, mgpp::endian_t::big_word);
        
        uint64_t expected = mgpp::endian::htot(val, mgpp::endian_t::big_word);
        REQUIRE(buf.size() == sizeof(uint64_t));
        REQUIRE(memcmp(buf.data(), &expected, sizeof(uint64_t)) == 0);
    }
    
    SECTION("uint64_t little_word")
    {
        memepp::variable_buffer buf;
        uint64_t val = 0x0123456789ABCDEF;
        buf.push_back(val, mgpp::endian_t::little_word);
        
        uint64_t expected = mgpp::endian::htot(val, mgpp::endian_t::little_word);
        REQUIRE(buf.size() == sizeof(uint64_t));
        REQUIRE(memcmp(buf.data(), &expected, sizeof(uint64_t)) == 0);
    }
    
    SECTION("float big_word")
    {
        memepp::variable_buffer buf;
        float val = 3.14159f;
        buf.push_back(val, mgpp::endian_t::big_word);
        
        float expected = mgpp::endian::htot(val, mgpp::endian_t::big_word);
        REQUIRE(buf.size() == sizeof(float));
        REQUIRE(memcmp(buf.data(), &expected, sizeof(float)) == 0);
    }
    
    SECTION("float little_word")
    {
        memepp::variable_buffer buf;
        float val = 3.14159f;
        buf.push_back(val, mgpp::endian_t::little_word);
        
        float expected = mgpp::endian::htot(val, mgpp::endian_t::little_word);
        REQUIRE(buf.size() == sizeof(float));
        REQUIRE(memcmp(buf.data(), &expected, sizeof(float)) == 0);
    }
    
    SECTION("double big_word")
    {
        memepp::variable_buffer buf;
        double val = 2.718281828;
        buf.push_back(val, mgpp::endian_t::big_word);
        
        double expected = mgpp::endian::htot(val, mgpp::endian_t::big_word);
        REQUIRE(buf.size() == sizeof(double));
        REQUIRE(memcmp(buf.data(), &expected, sizeof(double)) == 0);
    }
    
    SECTION("double little_word")
    {
        memepp::variable_buffer buf;
        double val = 2.718281828;
        buf.push_back(val, mgpp::endian_t::little_word);
        
        double expected = mgpp::endian::htot(val, mgpp::endian_t::little_word);
        REQUIRE(buf.size() == sizeof(double));
        REQUIRE(memcmp(buf.data(), &expected, sizeof(double)) == 0);
    }
}

TEST_CASE("variable_buffer::push_front with word endian", "[variable_buffer][endian]")
{
    SECTION("uint32_t big_word")
    {
        memepp::variable_buffer buf;
        uint32_t val = 0x12345678;
        buf.push_front(val, mgpp::endian_t::big_word);
        
        uint32_t expected = mgpp::endian::htot(val, mgpp::endian_t::big_word);
        REQUIRE(buf.size() == sizeof(uint32_t));
        REQUIRE(memcmp(buf.data(), &expected, sizeof(uint32_t)) == 0);
    }
    
    SECTION("uint32_t little_word")
    {
        memepp::variable_buffer buf;
        uint32_t val = 0x12345678;
        buf.push_front(val, mgpp::endian_t::little_word);
        
        uint32_t expected = mgpp::endian::htot(val, mgpp::endian_t::little_word);
        REQUIRE(buf.size() == sizeof(uint32_t));
        REQUIRE(memcmp(buf.data(), &expected, sizeof(uint32_t)) == 0);
    }
    
    SECTION("uint64_t big_word")
    {
        memepp::variable_buffer buf;
        uint64_t val = 0x0123456789ABCDEF;
        buf.push_front(val, mgpp::endian_t::big_word);
        
        uint64_t expected = mgpp::endian::htot(val, mgpp::endian_t::big_word);
        REQUIRE(buf.size() == sizeof(uint64_t));
        REQUIRE(memcmp(buf.data(), &expected, sizeof(uint64_t)) == 0);
    }
    
    SECTION("uint64_t little_word")
    {
        memepp::variable_buffer buf;
        uint64_t val = 0x0123456789ABCDEF;
        buf.push_front(val, mgpp::endian_t::little_word);
        
        uint64_t expected = mgpp::endian::htot(val, mgpp::endian_t::little_word);
        REQUIRE(buf.size() == sizeof(uint64_t));
        REQUIRE(memcmp(buf.data(), &expected, sizeof(uint64_t)) == 0);
    }
}

TEST_CASE("variable_buffer::append with word endian", "[variable_buffer][endian]")
{
    SECTION("uint32_t big_word")
    {
        memepp::variable_buffer buf;
        uint32_t val = 0x12345678;
        buf.append(val, mgpp::endian_t::big_word);
        
        uint32_t expected = mgpp::endian::htot(val, mgpp::endian_t::big_word);
        REQUIRE(buf.size() == sizeof(uint32_t));
        REQUIRE(memcmp(buf.data(), &expected, sizeof(uint32_t)) == 0);
    }
    
    SECTION("uint32_t little_word")
    {
        memepp::variable_buffer buf;
        uint32_t val = 0x12345678;
        buf.append(val, mgpp::endian_t::little_word);
        
        uint32_t expected = mgpp::endian::htot(val, mgpp::endian_t::little_word);
        REQUIRE(buf.size() == sizeof(uint32_t));
        REQUIRE(memcmp(buf.data(), &expected, sizeof(uint32_t)) == 0);
    }
    
    SECTION("uint64_t big_word")
    {
        memepp::variable_buffer buf;
        uint64_t val = 0x0123456789ABCDEF;
        buf.append(val, mgpp::endian_t::big_word);
        
        uint64_t expected = mgpp::endian::htot(val, mgpp::endian_t::big_word);
        REQUIRE(buf.size() == sizeof(uint64_t));
        REQUIRE(memcmp(buf.data(), &expected, sizeof(uint64_t)) == 0);
    }
    
    SECTION("uint64_t little_word")
    {
        memepp::variable_buffer buf;
        uint64_t val = 0x0123456789ABCDEF;
        buf.append(val, mgpp::endian_t::little_word);
        
        uint64_t expected = mgpp::endian::htot(val, mgpp::endian_t::little_word);
        REQUIRE(buf.size() == sizeof(uint64_t));
        REQUIRE(memcmp(buf.data(), &expected, sizeof(uint64_t)) == 0);
    }
}

TEST_CASE("variable_buffer::prepend with word endian", "[variable_buffer][endian]")
{
    SECTION("uint32_t big_word")
    {
        memepp::variable_buffer buf;
        uint32_t val = 0x12345678;
        buf.prepend(val, mgpp::endian_t::big_word);
        
        uint32_t expected = mgpp::endian::htot(val, mgpp::endian_t::big_word);
        REQUIRE(buf.size() == sizeof(uint32_t));
        REQUIRE(memcmp(buf.data(), &expected, sizeof(uint32_t)) == 0);
    }
    
    SECTION("uint32_t little_word")
    {
        memepp::variable_buffer buf;
        uint32_t val = 0x12345678;
        buf.prepend(val, mgpp::endian_t::little_word);
        
        uint32_t expected = mgpp::endian::htot(val, mgpp::endian_t::little_word);
        REQUIRE(buf.size() == sizeof(uint32_t));
        REQUIRE(memcmp(buf.data(), &expected, sizeof(uint32_t)) == 0);
    }
    
    SECTION("uint64_t big_word")
    {
        memepp::variable_buffer buf;
        uint64_t val = 0x0123456789ABCDEF;
        buf.prepend(val, mgpp::endian_t::big_word);
        
        uint64_t expected = mgpp::endian::htot(val, mgpp::endian_t::big_word);
        REQUIRE(buf.size() == sizeof(uint64_t));
        REQUIRE(memcmp(buf.data(), &expected, sizeof(uint64_t)) == 0);
    }
    
    SECTION("uint64_t little_word")
    {
        memepp::variable_buffer buf;
        uint64_t val = 0x0123456789ABCDEF;
        buf.prepend(val, mgpp::endian_t::little_word);
        
        uint64_t expected = mgpp::endian::htot(val, mgpp::endian_t::little_word);
        REQUIRE(buf.size() == sizeof(uint64_t));
        REQUIRE(memcmp(buf.data(), &expected, sizeof(uint64_t)) == 0);
    }
}

TEST_CASE("variable_buffer::insert(const_iterator) with word endian", "[variable_buffer][endian]")
{
    SECTION("uint32_t big_word at begin")
    {
        memepp::variable_buffer buf;
        buf.push_back(static_cast<uint8_t>(0xAA));
        
        uint32_t val = 0x12345678;
        buf.insert(buf.cbegin(), val, mgpp::endian_t::big_word);
        
        uint32_t expected = mgpp::endian::htot(val, mgpp::endian_t::big_word);
        REQUIRE(buf.size() == sizeof(uint32_t) + 1);
        REQUIRE(memcmp(buf.data(), &expected, sizeof(uint32_t)) == 0);
        REQUIRE(buf[sizeof(uint32_t)] == 0xAA);
    }
    
    SECTION("uint32_t little_word at end")
    {
        memepp::variable_buffer buf;
        buf.push_back(static_cast<uint8_t>(0xAA));
        
        uint32_t val = 0x12345678;
        buf.insert(buf.cend(), val, mgpp::endian_t::little_word);
        
        uint32_t expected = mgpp::endian::htot(val, mgpp::endian_t::little_word);
        REQUIRE(buf.size() == sizeof(uint32_t) + 1);
        REQUIRE(buf[0] == 0xAA);
        REQUIRE(memcmp(buf.data() + 1, &expected, sizeof(uint32_t)) == 0);
    }
    
    SECTION("uint64_t big_word in middle")
    {
        memepp::variable_buffer buf;
        buf.push_back(static_cast<uint8_t>(0xAA));
        buf.push_back(static_cast<uint8_t>(0xBB));
        
        uint64_t val = 0x0123456789ABCDEF;
        buf.insert(buf.cbegin() + 1, val, mgpp::endian_t::big_word);
        
        uint64_t expected = mgpp::endian::htot(val, mgpp::endian_t::big_word);
        REQUIRE(buf.size() == sizeof(uint64_t) + 2);
        REQUIRE(buf[0] == 0xAA);
        REQUIRE(memcmp(buf.data() + 1, &expected, sizeof(uint64_t)) == 0);
        REQUIRE(buf[sizeof(uint64_t) + 1] == 0xBB);
    }
    
    SECTION("uint64_t little_word at position")
    {
        memepp::variable_buffer buf;
        buf.push_back(static_cast<uint8_t>(0xAA));
        buf.push_back(static_cast<uint8_t>(0xBB));
        
        uint64_t val = 0x0123456789ABCDEF;
        buf.insert(1, val, mgpp::endian_t::little_word);
        
        uint64_t expected = mgpp::endian::htot(val, mgpp::endian_t::little_word);
        REQUIRE(buf.size() == sizeof(uint64_t) + 2);
        REQUIRE(buf[0] == 0xAA);
        REQUIRE(memcmp(buf.data() + 1, &expected, sizeof(uint64_t)) == 0);
        REQUIRE(buf[sizeof(uint64_t) + 1] == 0xBB);
    }
}

TEST_CASE("variable_buffer::insert(size_type) with word endian", "[variable_buffer][endian]")
{
    SECTION("uint32_t big_word at position 0")
    {
        memepp::variable_buffer buf;
        buf.push_back(static_cast<uint8_t>(0xAA));
        
        uint32_t val = 0x12345678;
        buf.insert(0, val, mgpp::endian_t::big_word);
        
        uint32_t expected = mgpp::endian::htot(val, mgpp::endian_t::big_word);
        REQUIRE(buf.size() == sizeof(uint32_t) + 1);
        REQUIRE(memcmp(buf.data(), &expected, sizeof(uint32_t)) == 0);
        REQUIRE(buf[sizeof(uint32_t)] == 0xAA);
    }
    
    SECTION("uint32_t little_word at end")
    {
        memepp::variable_buffer buf;
        buf.push_back(static_cast<uint8_t>(0xAA));
        
        uint32_t val = 0x12345678;
        buf.insert(buf.size(), val, mgpp::endian_t::little_word);
        
        uint32_t expected = mgpp::endian::htot(val, mgpp::endian_t::little_word);
        REQUIRE(buf.size() == sizeof(uint32_t) + 1);
        REQUIRE(buf[0] == 0xAA);
        REQUIRE(memcmp(buf.data() + 1, &expected, sizeof(uint32_t)) == 0);
    }
    
    SECTION("uint64_t big_word in middle")
    {
        memepp::variable_buffer buf;
        buf.push_back(static_cast<uint8_t>(0xAA));
        buf.push_back(static_cast<uint8_t>(0xBB));
        
        uint64_t val = 0x0123456789ABCDEF;
        buf.insert(1, val, mgpp::endian_t::big_word);
        
        uint64_t expected = mgpp::endian::htot(val, mgpp::endian_t::big_word);
        REQUIRE(buf.size() == sizeof(uint64_t) + 2);
        REQUIRE(buf[0] == 0xAA);
        REQUIRE(memcmp(buf.data() + 1, &expected, sizeof(uint64_t)) == 0);
        REQUIRE(buf[sizeof(uint64_t) + 1] == 0xBB);
    }
    
    SECTION("uint64_t little_word at position 1")
    {
        memepp::variable_buffer buf;
        buf.push_back(static_cast<uint8_t>(0xAA));
        buf.push_back(static_cast<uint8_t>(0xBB));
        
        uint64_t val = 0x0123456789ABCDEF;
        buf.insert(1, val, mgpp::endian_t::little_word);
        
        uint64_t expected = mgpp::endian::htot(val, mgpp::endian_t::little_word);
        REQUIRE(buf.size() == sizeof(uint64_t) + 2);
        REQUIRE(buf[0] == 0xAA);
        REQUIRE(memcmp(buf.data() + 1, &expected, sizeof(uint64_t)) == 0);
        REQUIRE(buf[sizeof(uint64_t) + 1] == 0xBB);
    }
}

TEST_CASE("variable_buffer multiple word endian operations", "[variable_buffer][endian]")
{
    SECTION("mixed endian types grow small to medium")
    {
        memepp::variable_buffer buf;
        
        // Push multiple values with different endian modes
        uint32_t val1 = 0x11223344;
        uint64_t val2 = 0xAABBCCDDEEFF0011;
        uint32_t val3 = 0x55667788;
        
        buf.push_back(val1, mgpp::endian_t::big_word);
        buf.push_back(val2, mgpp::endian_t::little_word);
        buf.push_back(val3, mgpp::endian_t::big_word);
        
        uint32_t exp1 = mgpp::endian::htot(val1, mgpp::endian_t::big_word);
        uint64_t exp2 = mgpp::endian::htot(val2, mgpp::endian_t::little_word);
        uint32_t exp3 = mgpp::endian::htot(val3, mgpp::endian_t::big_word);
        
        REQUIRE(buf.size() == sizeof(uint32_t) + sizeof(uint64_t) + sizeof(uint32_t));
        REQUIRE(memcmp(buf.data(), &exp1, sizeof(uint32_t)) == 0);
        REQUIRE(memcmp(buf.data() + sizeof(uint32_t), &exp2, sizeof(uint64_t)) == 0);
        REQUIRE(memcmp(buf.data() + sizeof(uint32_t) + sizeof(uint64_t), &exp3, sizeof(uint32_t)) == 0);
    }
    
    SECTION("prepend and append with word endian")
    {
        memepp::variable_buffer buf;
        
        uint32_t val1 = 0x11223344;
        uint64_t val2 = 0xAABBCCDDEEFF0011;
        
        buf.append(val1, mgpp::endian_t::big_word);
        buf.prepend(val2, mgpp::endian_t::little_word);
        
        uint32_t exp1 = mgpp::endian::htot(val1, mgpp::endian_t::big_word);
        uint64_t exp2 = mgpp::endian::htot(val2, mgpp::endian_t::little_word);
        
        REQUIRE(buf.size() == sizeof(uint64_t) + sizeof(uint32_t));
        REQUIRE(memcmp(buf.data(), &exp2, sizeof(uint64_t)) == 0);
        REQUIRE(memcmp(buf.data() + sizeof(uint64_t), &exp1, sizeof(uint32_t)) == 0);
    }
}
