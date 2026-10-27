#include <catch2/catch.hpp>
#include <memepp/variable_buffer.hpp>

TEST_CASE("variable_buffer::reserve - zero is no-op", "[variable_buffer]") {
    memepp::variable_buffer buf;
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::small);
    
    buf.reserve(0);
    
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::small);
    REQUIRE(buf.size() == 0);
    REQUIRE(buf.capacity() == 21); // small buffer capacity
}

TEST_CASE("variable_buffer::reserve - small to medium promotion", "[variable_buffer]") {
    memepp::variable_buffer buf;
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::small);
    
    buf.reserve(10);
    
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::medium);
    REQUIRE(buf.size() == 0);
    REQUIRE(buf.capacity() >= 10);
}

TEST_CASE("variable_buffer::reserve - at small boundary still promotes", "[variable_buffer]") {
    memepp::variable_buffer buf;
    int small_limit = ::MemeStringOption_getStorageSmallLimit();
    
    buf.reserve(small_limit);
    
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::medium);
    REQUIRE(buf.capacity() >= small_limit);
}

TEST_CASE("variable_buffer::reserve - preserves data small to medium", "[variable_buffer]") {
    memepp::variable_buffer buf;
    buf.push_back(static_cast<uint8_t>('a'));
    buf.push_back(static_cast<uint8_t>('b'));
    buf.push_back(static_cast<uint8_t>('c'));
    
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::small);
    REQUIRE(buf.size() == 3);
    
    buf.reserve(100);
    
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::medium);
    REQUIRE(buf.size() == 3);
    REQUIRE(buf[0] == static_cast<uint8_t>('a'));
    REQUIRE(buf[1] == static_cast<uint8_t>('b'));
    REQUIRE(buf[2] == static_cast<uint8_t>('c'));
}

TEST_CASE("variable_buffer::reserve - medium growth never shrinks", "[variable_buffer]") {
    memepp::variable_buffer buf;
    buf.reserve(100);
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::medium);
    
    int cap_before = buf.capacity();
    REQUIRE(cap_before >= 100);
    
    buf.reserve(50); // request smaller capacity
    
    int cap_after = buf.capacity();
    REQUIRE(cap_after >= cap_before); // should not shrink
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::medium);
}

TEST_CASE("variable_buffer::reserve - preserves data in medium growth", "[variable_buffer]") {
    memepp::variable_buffer buf;
    buf.reserve(50);
    
    for (int i = 0; i < 10; ++i) {
        buf.push_back(static_cast<uint8_t>('A' + i));
    }
    
    REQUIRE(buf.size() == 10);
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::medium);
    
    buf.reserve(500);
    
    REQUIRE(buf.size() == 10);
    REQUIRE(buf.storage_type() == memepp::buffer_storage_t::medium);
    for (int i = 0; i < 10; ++i) {
        REQUIRE(buf[i] == static_cast<uint8_t>('A' + i));
    }
}

TEST_CASE("variable_buffer::reserve - huge value throws", "[variable_buffer]") {
    memepp::variable_buffer buf;
    
    REQUIRE_THROWS_AS(buf.reserve((size_t)1 << 44), memepp::errc_except);
}
