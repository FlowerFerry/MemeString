#include <catch2/catch.hpp>

#include <megopp/help/snprintf.h>

#include <string>
#include <cstring>

TEST_CASE("mgpp::help::snprintf - basic string format", "[snprintf]")
{
    std::string result = mgpp::help::snprintf(0, "hello %s", -1, "world");
    REQUIRE(result == "hello world");
}

TEST_CASE("mgpp::help::snprintf - integer format", "[snprintf]")
{
    std::string result = mgpp::help::snprintf(0, "%d + %d = %d", -1, 1, 2, 3);
    REQUIRE(result == "1 + 2 = 3");
}

TEST_CASE("mgpp::help::snprintf - no format arguments", "[snprintf]")
{
    std::string result = mgpp::help::snprintf(0, "plain text", -1);
    REQUIRE(result == "plain text");
}

TEST_CASE("mgpp::help::snprintf - null format string returns empty", "[snprintf]")
{
    std::string result = mgpp::help::snprintf(0, nullptr, 0);
    REQUIRE(result.empty());
}

TEST_CASE("mgpp::help::snprintf - limit truncates output", "[snprintf]")
{
    std::string result = mgpp::help::snprintf(3, "hello", -1);
    REQUIRE(result == "hel");
}

TEST_CASE("mgpp::help::snprintf - limit equal to full length returns full string", "[snprintf]")
{
    std::string result = mgpp::help::snprintf(5, "hello", -1);
    REQUIRE(result == "hello");
}

TEST_CASE("mgpp::help::snprintf - output longer than internal buffer triggers heap fallback", "[snprintf]")
{
    // Produce output longer than the default 128-byte stack buffer
    const std::string fill(150, 'x');
    std::string result = mgpp::help::snprintf(0, "%s", -1, fill.c_str());
    REQUIRE(result == fill);
}

TEST_CASE("mgpp::help::snprintf - fmt_len specifies exact length (not null-terminated lookup)", "[snprintf]")
{
    // "hello world" but we tell the function the format is only 5 chars ("hello")
    const char* fmt = "hello world";
    std::string result = mgpp::help::snprintf(0, fmt, 5);
    REQUIRE(result == "hello");
}
