
#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/convert/std/string.hpp>

#include <meme/string.h>

#include <string>

namespace {

// Native handle of a string_view, so the C API can be inspected directly:
// string_view deliberately exposes no max_size() member.
mmstr_cptr_t pointer_of(const memepp::string_view& _view)
{
    return memepp::to_pointer(_view.native_handle());
}

} // namespace

TEST_CASE("memepp::string max_size", "[string]")
{
    memepp::string s;
    REQUIRE(s.max_size() == MemeStringOption_getStorageSmallLimit());

    memepp::string s2 = "hello";
    REQUIRE(s2.max_size() == MemeStringOption_getStorageSmallLimit());

    // max_size() must not track content: two small strings of different
    // length report the same inline-buffer ceiling.
    REQUIRE(s.max_size() == s2.max_size());
}

TEST_CASE("memepp::string max_size per storage type", "[string]")
{
    const char* data = "0123456789";

    memepp::string small_str = "hi";
    REQUIRE(small_str.storage_type() == memepp::string_storage_t::small);
    REQUIRE(small_str.max_size() == MemeStringOption_getStorageSmallLimit());

    memepp::string medium_str(data, 10, memepp::string_storage_t::medium);
    REQUIRE(medium_str.storage_type() == memepp::string_storage_t::medium);

    memepp::string large_str(data, 10, memepp::string_storage_t::large);
    REQUIRE(large_str.storage_type() == memepp::string_storage_t::large);

    // user storage: only produced by from() for inputs above the medium limit.
    std::string user_data(500, '0');
    memepp::string user_str = memepp::from(std::move(user_data));
    REQUIRE(user_str.storage_type() == memepp::string_storage_t::user);
    REQUIRE(user_str.size() == 500);

    memepp::string_view view_str = "0123456789";

    // medium/large/user/view share the same size_ bitfield, therefore the same
    // ceiling -- regardless of how much data each instance currently holds.
    const auto ceiling = medium_str.max_size();
    REQUIRE(large_str.max_size() == ceiling);
    REQUIRE(user_str.max_size() == ceiling);
    REQUIRE(MemeString_maxByteSize(pointer_of(view_str)) == ceiling);

    // The ceiling is an upper bound, not a mirror of the current size. This is
    // what regressed when large/user/view returned their own size_.
    REQUIRE(ceiling > medium_str.size());
    REQUIRE(large_str.max_size() > large_str.size());
    REQUIRE(user_str.max_size() > user_str.size());

    // The small inline buffer is a *different* ceiling: it cannot grow.
    REQUIRE(small_str.max_size() < ceiling);
}
