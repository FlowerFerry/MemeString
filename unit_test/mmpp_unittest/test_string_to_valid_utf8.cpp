
#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <megopp/util/scope_cleanup.h>

#include <cstring>

TEST_CASE("memepp::string to_valid_utf8", "[string]")
{
    // empty string
    memepp::string s01 = "";
    auto r01 = s01.to_valid_utf8();
    REQUIRE(r01 == "");
    REQUIRE(r01.size() == 0);

    // already valid ASCII
    memepp::string s02 = "Hello, World!";
    auto r02 = s02.to_valid_utf8();
    REQUIRE(r02 == "Hello, World!");

    // already valid UTF-8 with multibyte (emoji)
    memepp::string s03("Hello, \xF0\x9F\x98\x8A", 11);
    auto r03 = s03.to_valid_utf8();
    REQUIRE(r03.size() == 11);
    REQUIRE(r03 == s03);

    // invalid UTF-8: truncated multibyte sequence (\xF0\x9F\x98 only, missing last byte)
    memepp::string s04("Hello, \xF0\x9F\x98", 10);
    auto r04 = s04.to_valid_utf8();
    // invalid tail bytes stripped → only "Hello, " remains
    REQUIRE(r04 == "Hello, ");
}

TEST_CASE("memepp::string to_valid_utf8 - truncates at the first invalid byte", "[string]")
{
    const char middle_invalid[] = "ab\xFF" "cd";
    memepp::string middle(middle_invalid, sizeof(middle_invalid) - 1);
    auto middle_result = middle.to_valid_utf8();
    REQUIRE(middle_result.size() == 2);
    REQUIRE(std::memcmp(middle_result.data(), middle_invalid, 2) == 0);

    const char invalid_at_start[] = "\x80" "abc";
    memepp::string at_start(invalid_at_start, sizeof(invalid_at_start) - 1);
    auto start_result = at_start.to_valid_utf8();
    REQUIRE(start_result.empty());

    const char isolated_continuation[] = "A\x80" "B";
    memepp::string isolated(isolated_continuation, sizeof(isolated_continuation) - 1);
    auto isolated_result = isolated.to_valid_utf8();
    REQUIRE(isolated_result.size() == 1);
    REQUIRE(std::memcmp(isolated_result.data(), isolated_continuation, 1) == 0);

    const char invalid_ff[] = "A\xFF" "B";
    memepp::string with_ff(invalid_ff, sizeof(invalid_ff) - 1);
    auto ff_result = with_ff.to_valid_utf8();
    REQUIRE(ff_result.size() == 1);
    REQUIRE(std::memcmp(ff_result.data(), invalid_ff, 1) == 0);

    const char invalid_fe[] = "A\xFE" "B";
    memepp::string with_fe(invalid_fe, sizeof(invalid_fe) - 1);
    auto fe_result = with_fe.to_valid_utf8();
    REQUIRE(fe_result.size() == 1);
    REQUIRE(std::memcmp(fe_result.data(), invalid_fe, 1) == 0);
}

TEST_CASE("memepp::string to_valid_utf8 - preserves project-compatible UTF-8 bytes", "[string]")
{
    const char overlong[] = "\xC0\x80";
    const char surrogate[] = "\xED\xA0\x80";
    const char above_unicode_max[] = "\xF4\x90\x80\x80";

    const memepp::string values[] = {
        memepp::string(overlong, sizeof(overlong) - 1),
        memepp::string(surrogate, sizeof(surrogate) - 1),
        memepp::string(above_unicode_max, sizeof(above_unicode_max) - 1)
    };
    const char* inputs[] = { overlong, surrogate, above_unicode_max };
    const memepp::string::size_type sizes[] = {
        static_cast<memepp::string::size_type>(sizeof(overlong) - 1),
        static_cast<memepp::string::size_type>(sizeof(surrogate) - 1),
        static_cast<memepp::string::size_type>(sizeof(above_unicode_max) - 1)
    };

    for (int i = 0; i < 3; ++i) {
        auto result = values[i].to_valid_utf8();
        REQUIRE(result.size() == sizes[i]);
        REQUIRE(std::memcmp(result.data(), inputs[i], static_cast<std::size_t>(sizes[i])) == 0);
    }
}

TEST_CASE("memepp::string to_valid_utf8 - preserves embedded NUL bytes", "[string]")
{
    const char input[] = { 'a', '\0', 'b' };
    memepp::string value(input, sizeof(input));
    auto result = value.to_valid_utf8();

    REQUIRE(result.size() == sizeof(input));
    REQUIRE(std::memcmp(result.data(), input, sizeof(input)) == 0);
}
