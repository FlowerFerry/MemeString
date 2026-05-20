
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

#include <tuple>

// ---------------------------------------------------------------------------
// memepp::string_view — cut
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view cut — found", "[string_view_cut]")
{
    memepp::string_view sv = "hello,world";
    memepp::string_view before, after;
    bool found;
    std::tie(before, after, found) = sv.cut(",");
    REQUIRE(found  == true);
    REQUIRE(before == memepp::string_view{ "hello" });
    REQUIRE(after  == memepp::string_view{ "world" });
}

TEST_CASE("memepp::string_view cut — not found", "[string_view_cut]")
{
    memepp::string_view sv = "hello world";
    memepp::string_view before, after;
    bool found;
    std::tie(before, after, found) = sv.cut(",");
    REQUIRE(found  == false);
    REQUIRE(before == sv);
    REQUIRE(after  == memepp::string_view{});
}

TEST_CASE("memepp::string_view cut — sep at start", "[string_view_cut]")
{
    memepp::string_view sv = ",world";
    memepp::string_view before, after;
    bool found;
    std::tie(before, after, found) = sv.cut(",");
    REQUIRE(found  == true);
    REQUIRE(before == memepp::string_view{ "" });
    REQUIRE(after  == memepp::string_view{ "world" });
}

TEST_CASE("memepp::string_view cut — sep at end", "[string_view_cut]")
{
    memepp::string_view sv = "hello,";
    memepp::string_view before, after;
    bool found;
    std::tie(before, after, found) = sv.cut(",");
    REQUIRE(found  == true);
    REQUIRE(before == memepp::string_view{ "hello" });
    REQUIRE(after  == memepp::string_view{ "" });
}

TEST_CASE("memepp::string_view cut — first occurrence only", "[string_view_cut]")
{
    memepp::string_view sv = "a:b:c";
    memepp::string_view before, after;
    bool found;
    std::tie(before, after, found) = sv.cut(":");
    REQUIRE(found  == true);
    REQUIRE(before == memepp::string_view{ "a" });
    REQUIRE(after  == memepp::string_view{ "b:c" });
}

// ---------------------------------------------------------------------------
// memepp::string_view — cut_prefix
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view cut_prefix — found", "[string_view_cut]")
{
    memepp::string_view sv = "Hello World";
    memepp::string_view after;
    bool found;
    std::tie(after, found) = sv.cut_prefix("Hello ");
    REQUIRE(found == true);
    REQUIRE(after == memepp::string_view{ "World" });
}

TEST_CASE("memepp::string_view cut_prefix — not found", "[string_view_cut]")
{
    memepp::string_view sv = "Hello World";
    memepp::string_view after;
    bool found;
    std::tie(after, found) = sv.cut_prefix("World");
    REQUIRE(found == false);
    REQUIRE(after == sv);
}

TEST_CASE("memepp::string_view cut_prefix — empty prefix", "[string_view_cut]")
{
    memepp::string_view sv = "Hello";
    memepp::string_view after;
    bool found;
    std::tie(after, found) = sv.cut_prefix("");
    REQUIRE(found == true);
    REQUIRE(after == sv);
}

// ---------------------------------------------------------------------------
// memepp::string_view — cut_suffix
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view cut_suffix — found", "[string_view_cut]")
{
    memepp::string_view sv = "Hello World";
    memepp::string_view before;
    bool found;
    std::tie(before, found) = sv.cut_suffix(" World");
    REQUIRE(found  == true);
    REQUIRE(before == memepp::string_view{ "Hello" });
}

TEST_CASE("memepp::string_view cut_suffix — not found", "[string_view_cut]")
{
    memepp::string_view sv = "Hello World";
    memepp::string_view before;
    bool found;
    std::tie(before, found) = sv.cut_suffix("Hello");
    REQUIRE(found  == false);
    REQUIRE(before == sv);
}

TEST_CASE("memepp::string_view cut_suffix — empty suffix", "[string_view_cut]")
{
    memepp::string_view sv = "Hello";
    memepp::string_view before;
    bool found;
    std::tie(before, found) = sv.cut_suffix("");
    REQUIRE(found  == true);
    REQUIRE(before == sv);
}

// ---------------------------------------------------------------------------
// memepp::string — cut
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string cut — found", "[string_cut]")
{
    memepp::string s = "hello,world";
    memepp::string_view before, after;
    bool found;
    std::tie(before, after, found) = s.cut(",");
    REQUIRE(found  == true);
    REQUIRE(before == memepp::string_view{ "hello" });
    REQUIRE(after  == memepp::string_view{ "world" });
}

TEST_CASE("memepp::string cut — not found", "[string_cut]")
{
    memepp::string s = "hello world";
    memepp::string_view before, after;
    bool found;
    std::tie(before, after, found) = s.cut(",");
    REQUIRE(found  == false);
    REQUIRE(before == memepp::string_view{ "hello world" });
    REQUIRE(after  == memepp::string_view{});
}

TEST_CASE("memepp::string cut — first occurrence only", "[string_cut]")
{
    memepp::string s = "a:b:c";
    memepp::string_view before, after;
    bool found;
    std::tie(before, after, found) = s.cut(":");
    REQUIRE(found  == true);
    REQUIRE(before == memepp::string_view{ "a" });
    REQUIRE(after  == memepp::string_view{ "b:c" });
}

// ---------------------------------------------------------------------------
// memepp::string — cut_prefix
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string cut_prefix — found", "[string_cut]")
{
    memepp::string s = "Hello World";
    memepp::string_view after;
    bool found;
    std::tie(after, found) = s.cut_prefix("Hello ");
    REQUIRE(found == true);
    REQUIRE(after == memepp::string_view{ "World" });
}

TEST_CASE("memepp::string cut_prefix — not found", "[string_cut]")
{
    memepp::string s = "Hello World";
    memepp::string_view after;
    bool found;
    std::tie(after, found) = s.cut_prefix("World");
    REQUIRE(found == false);
    REQUIRE(after == memepp::string_view{ "Hello World" });
}

// ---------------------------------------------------------------------------
// memepp::string — cut_suffix
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string cut_suffix — found", "[string_cut]")
{
    memepp::string s = "Hello World";
    memepp::string_view before;
    bool found;
    std::tie(before, found) = s.cut_suffix(" World");
    REQUIRE(found  == true);
    REQUIRE(before == memepp::string_view{ "Hello" });
}

TEST_CASE("memepp::string cut_suffix — not found", "[string_cut]")
{
    memepp::string s = "Hello World";
    memepp::string_view before;
    bool found;
    std::tie(before, found) = s.cut_suffix("Hello");
    REQUIRE(found  == false);
    REQUIRE(before == memepp::string_view{ "Hello World" });
}
