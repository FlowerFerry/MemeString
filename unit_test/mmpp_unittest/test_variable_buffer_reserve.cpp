
#include <catch2/catch.hpp>

#include <memepp/variable_buffer.hpp>

TEST_CASE("memepp::variable_buffer::reserve", "[variable_buffer]") 
{
    memepp::variable_buffer buf01_01;
    buf01_01.reserve(MMSTR__OBJ_SIZE);
    REQUIRE(buf01_01.size()  == 0);
    REQUIRE(buf01_01.empty() == true);
    REQUIRE(buf01_01.storage_type() == memepp::buffer_storage_t::medium);

}