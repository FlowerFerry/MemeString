
#include <catch2/catch.hpp>

#include <memepp/variable_buffer.hpp>

TEST_CASE("memepp::variable_buffer::clear", "[variable_buffer]") 
{
    memepp::variable_buffer buf01_01;
    buf01_01.clear();
    REQUIRE(buf01_01.size()  == 0);
    REQUIRE(buf01_01.empty() == true);
    REQUIRE(buf01_01.storage_type() == memepp::buffer_storage_t::small);

    for (int i = 0; i < MMSTR__OBJ_SIZE; ++i) 
    {
        buf01_01.push_back(static_cast<uint8_t>(i));
    }

    buf01_01.clear();
    REQUIRE(buf01_01.size()  == 0);
    REQUIRE(buf01_01.empty() == true);
    REQUIRE(buf01_01.storage_type() == memepp::buffer_storage_t::medium);

    memepp::variable_buffer buf01_02;

    for (int i = 0; i < 10; ++i) {
        buf01_02.push_back(static_cast<uint8_t>(i));
    }

    buf01_02.clear();
    REQUIRE(buf01_02.size()  == 0);
    REQUIRE(buf01_02.empty() == true);
    REQUIRE(buf01_02.storage_type() == memepp::buffer_storage_t::small);
    
}
