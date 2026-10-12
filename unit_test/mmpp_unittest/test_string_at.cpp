
#include <catch2/catch.hpp>

#include <meme/string_fwd.h>
#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/errc.hpp>

#include <stdexcept>
#include <limits>
#include <string>
#include <memepp/convert/std/string.hpp>

TEST_CASE("memepp::string at", "[string]")
{
    memepp::string str = "0123456789";
    for (int i = 0; i < 10; ++i) {
        CAPTURE(i);
        REQUIRE(str.at(i) == '0' + i);
    }

    // out of range
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(str.at(10), memepp::errc_except);
    REQUIRE_THROWS_AS(str.at(-1), memepp::errc_except);
#else
    // exception disabled: at() returns sentinel '\0' and sets errc
    memepp::set_errc(0);  // reset errc
    REQUIRE(str.at(10) == '\0');
    REQUIRE(memepp::get_errc() != 0);
    
    memepp::set_errc(0);  // reset errc
    REQUIRE(str.at(-1) == '\0');
    REQUIRE(memepp::get_errc() != 0);
#endif

    memepp::string_view sv = str;
    for (int i = 0; i < 10; ++i) {
        CAPTURE(i);
        REQUIRE(sv.at(i) == '0' + i);
    }

    // out of range
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(sv.at(10), memepp::errc_except);
    REQUIRE_THROWS_AS(sv.at(-1), memepp::errc_except);
#else
    // exception disabled: at() returns sentinel '\0' and sets errc
    memepp::set_errc(0);  // reset errc
    REQUIRE(sv.at(10) == '\0');
    REQUIRE(memepp::get_errc() != 0);
    
    memepp::set_errc(0);  // reset errc
    REQUIRE(sv.at(-1) == '\0');
    REQUIRE(memepp::get_errc() != 0);
#endif
}

TEST_CASE("memepp::string at on empty string", "[string]")
{
    memepp::string empty_str;
    REQUIRE(empty_str.size() == 0);
    REQUIRE(empty_str.empty());

#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(empty_str.at(0), memepp::errc_except);
#else
    memepp::set_errc(0);
    REQUIRE(empty_str.at(0) == '\0');
    REQUIRE(memepp::get_errc() != 0);
#endif

    memepp::string_view empty_sv;
    REQUIRE(empty_sv.size() == 0);
    REQUIRE(empty_sv.empty());

#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(empty_sv.at(0), memepp::errc_except);
#else
    memepp::set_errc(0);
    REQUIRE(empty_sv.at(0) == '\0');
    REQUIRE(memepp::get_errc() != 0);
#endif
}

TEST_CASE("memepp::string at with different storage types", "[string]")
{
    const char* data = "0123456789";

    // medium storage
    memepp::string medium_str(data, 10, memepp::string_storage_t::medium);
    REQUIRE(medium_str.storage_type() == memepp::string_storage_t::medium);
    for (int i = 0; i < 10; ++i) {
        CAPTURE(i);
        REQUIRE(medium_str.at(i) == '0' + i);
    }
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(medium_str.at(10), memepp::errc_except);
    REQUIRE_THROWS_AS(medium_str.at(-1), memepp::errc_except);
#else
    memepp::set_errc(0);
    REQUIRE(medium_str.at(10) == '\0');
    REQUIRE(memepp::get_errc() != 0);
    memepp::set_errc(0);
    REQUIRE(medium_str.at(-1) == '\0');
    REQUIRE(memepp::get_errc() != 0);
#endif

    // large storage
    memepp::string large_str(data, 10, memepp::string_storage_t::large);
    REQUIRE(large_str.storage_type() == memepp::string_storage_t::large);
    for (int i = 0; i < 10; ++i) {
        CAPTURE(i);
        REQUIRE(large_str.at(i) == '0' + i);
    }
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(large_str.at(10), memepp::errc_except);
    REQUIRE_THROWS_AS(large_str.at(-1), memepp::errc_except);
#else
    memepp::set_errc(0);
    REQUIRE(large_str.at(10) == '\0');
    REQUIRE(memepp::get_errc() != 0);
    memepp::set_errc(0);
    REQUIRE(large_str.at(-1) == '\0');
    REQUIRE(memepp::get_errc() != 0);
#endif

    // user storage (via memepp::from(std::move(std::string)))
    // Note: from() only creates user storage when string is large enough
    // (>= MemeStringOption_getStorageMediumLimit() + MMSTR__OBJ_SIZE)
    std::string user_data(500, '0');
    memepp::string user_str = memepp::from(std::move(user_data));
    REQUIRE(user_str.storage_type() == memepp::string_storage_t::user);
    REQUIRE(user_str.size() == 500);
    for (int i = 0; i < 500; ++i) {
        CAPTURE(i);
        REQUIRE(user_str.at(i) == '0');
    }
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(user_str.at(500), memepp::errc_except);
    REQUIRE_THROWS_AS(user_str.at(-1), memepp::errc_except);
#else
    memepp::set_errc(0);
    REQUIRE(user_str.at(500) == '\0');
    REQUIRE(memepp::get_errc() != 0);
    memepp::set_errc(0);
    REQUIRE(user_str.at(-1) == '\0');
    REQUIRE(memepp::get_errc() != 0);
#endif

    // view storage — string_view built from a small string holds a non-owning view.
    // string_storage_t has no `view` enumerator, so cast the C unsafe-storage value.
    const auto view_storage =
        static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view);
    memepp::string_view view_str = "0123456789";
    REQUIRE(view_str.storage_type() == view_storage);
    for (int i = 0; i < 10; ++i) {
        CAPTURE(i);
        REQUIRE(view_str.at(i) == '0' + i);
    }
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(view_str.at(10), memepp::errc_except);
    REQUIRE_THROWS_AS(view_str.at(-1), memepp::errc_except);
#else
    memepp::set_errc(0);
    REQUIRE(view_str.at(10) == '\0');
    REQUIRE(memepp::get_errc() != 0);
    memepp::set_errc(0);
    REQUIRE(view_str.at(-1) == '\0');
    REQUIRE(memepp::get_errc() != 0);
#endif
}

TEST_CASE("memepp::string at with size_type max", "[string]")
{
    memepp::string str = "0123456789";
    auto max_val = std::numeric_limits<memepp::string::size_type>::max();

#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(str.at(max_val), memepp::errc_except);
#else
    memepp::set_errc(0);
    REQUIRE(str.at(max_val) == '\0');
    REQUIRE(memepp::get_errc() != 0);
#endif

    memepp::string_view sv = str;
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(sv.at(max_val), memepp::errc_except);
#else
    memepp::set_errc(0);
    REQUIRE(sv.at(max_val) == '\0');
    REQUIRE(memepp::get_errc() != 0);
#endif
}

TEST_CASE("memepp::string operator[]", "[string]")
{
    memepp::string str = "0123456789";
    REQUIRE(str[0] == '0');
    REQUIRE(str[1] == '1');
    REQUIRE(str[2] == '2');
    REQUIRE(str[3] == '3');
    REQUIRE(str[9] == '9');

    memepp::string_view sv = str;
    REQUIRE(sv[0] == '0');
    REQUIRE(sv[1] == '1');
    REQUIRE(sv[9] == '9');

    // out of range (operator[] delegates to at(), so it throws the same way)
#if !MMOPT__EXCEPTION_DISABLED
    REQUIRE_THROWS_AS(str[10], memepp::errc_except);
    REQUIRE_THROWS_AS(str[-1], memepp::errc_except);
    REQUIRE_THROWS_AS(sv[10], memepp::errc_except);
    REQUIRE_THROWS_AS(sv[-1], memepp::errc_except);
#else
    memepp::set_errc(0);
    REQUIRE(str[10] == '\0');
    REQUIRE(memepp::get_errc() != 0);

    memepp::set_errc(0);
    REQUIRE(str[-1] == '\0');
    REQUIRE(memepp::get_errc() != 0);

    memepp::set_errc(0);
    REQUIRE(sv[10] == '\0');
    REQUIRE(memepp::get_errc() != 0);

    memepp::set_errc(0);
    REQUIRE(sv[-1] == '\0');
    REQUIRE(memepp::get_errc() != 0);
#endif
}
