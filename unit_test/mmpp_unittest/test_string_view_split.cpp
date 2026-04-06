#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>
#include <memepp/split/self.hpp>

#include <vector>
#include <list>
#include <deque>

// ---------------------------------------------------------------------------
// string_view::split  (dedicated tests; source keeps alive via memepp::string)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view split - keep_empty_parts basic", "[string_view]")
{
    memepp::string src = "11"":"":""22"":3:""444";
    memepp::string_view sv = src;

    std::vector<memepp::string_view> results;
    sv.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(results));

    REQUIRE(results.size() == 5);
    REQUIRE(results[0] == "11");
    REQUIRE(results[1] == "");
    REQUIRE(results[2] == "22");
    REQUIRE(results[3] == "3");
    REQUIRE(results[4] == "444");
}

TEST_CASE("memepp::string_view split - skip_empty_parts basic", "[string_view]")
{
    memepp::string src = "11"":"":""22"":3:""444";
    memepp::string_view sv = src;

    std::vector<memepp::string_view> results;
    sv.split(":", memepp::split_behav_t::skip_empty_parts, std::back_inserter(results));

    REQUIRE(results.size() == 4);
    REQUIRE(results[0] == "11");
    REQUIRE(results[1] == "22");
    REQUIRE(results[2] == "3");
    REQUIRE(results[3] == "444");
}

TEST_CASE("memepp::string_view split - result into string container", "[string_view]")
{
    memepp::string src = "a:b:c";
    memepp::string_view sv = src;

    std::vector<memepp::string> results;
    sv.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(results));

    REQUIRE(results.size() == 3);
    REQUIRE(results[0] == "a");
    REQUIRE(results[1] == "b");
    REQUIRE(results[2] == "c");
}

TEST_CASE("memepp::string_view split - list container", "[string_view]")
{
    memepp::string src = "foo|bar|baz";
    memepp::string_view sv = src;

    std::list<memepp::string> results;
    sv.split("|", memepp::split_behav_t::keep_empty_parts, std::back_inserter(results));

    REQUIRE(results.size() == 3);
    auto it = results.begin();
    REQUIRE(*it++ == "foo");
    REQUIRE(*it++ == "bar");
    REQUIRE(*it   == "baz");
}

TEST_CASE("memepp::string_view split - deque, string_view items", "[string_view]")
{
    memepp::string src = "x,y,z";
    memepp::string_view sv = src;

    std::deque<memepp::string_view> results;
    sv.split(",", memepp::split_behav_t::keep_empty_parts, std::back_inserter(results));

    REQUIRE(results.size() == 3);
    REQUIRE(results[0] == "x");
    REQUIRE(results[1] == "y");
    REQUIRE(results[2] == "z");
}

TEST_CASE("memepp::string_view split - delimiter at start produces leading empty", "[string_view]")
{
    memepp::string src = ":abc";
    memepp::string_view sv = src;

    std::vector<memepp::string_view> kept;
    sv.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(kept));
    REQUIRE(kept.size() == 2);
    REQUIRE(kept[0] == "");
    REQUIRE(kept[1] == "abc");

    std::vector<memepp::string_view> skipped;
    sv.split(":", memepp::split_behav_t::skip_empty_parts, std::back_inserter(skipped));
    REQUIRE(skipped.size() == 1);
    REQUIRE(skipped[0] == "abc");
}

TEST_CASE("memepp::string_view split - delimiter at end produces trailing empty", "[string_view]")
{
    memepp::string src = "abc:";
    memepp::string_view sv = src;

    std::vector<memepp::string_view> kept;
    sv.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(kept));
    REQUIRE(kept.size() == 2);
    REQUIRE(kept[0] == "abc");
    REQUIRE(kept[1] == "");

    std::vector<memepp::string_view> skipped;
    sv.split(":", memepp::split_behav_t::skip_empty_parts, std::back_inserter(skipped));
    REQUIRE(skipped.size() == 1);
    REQUIRE(skipped[0] == "abc");
}

TEST_CASE("memepp::string_view split - no delimiter found", "[string_view]")
{
    memepp::string src = "Hello";
    memepp::string_view sv = src;

    std::vector<memepp::string_view> results;
    sv.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(results));

    REQUIRE(results.size() == 1);
    REQUIRE(results[0] == "Hello");
}

TEST_CASE("memepp::string_view split - empty view", "[string_view]")
{
    memepp::string_view empty;

    std::vector<memepp::string_view> kept;
    empty.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(kept));
    REQUIRE(kept.size() == 1);
    REQUIRE(kept[0] == "");

    std::vector<memepp::string_view> skipped;
    empty.split(":", memepp::split_behav_t::skip_empty_parts, std::back_inserter(skipped));
    REQUIRE(skipped.empty());
}

TEST_CASE("memepp::string_view split - multi-char delimiter", "[string_view]")
{
    memepp::string src = "one::two::three";
    memepp::string_view sv = src;

    std::vector<memepp::string_view> results;
    sv.split("::", memepp::split_behav_t::keep_empty_parts, std::back_inserter(results));

    REQUIRE(results.size() == 3);
    REQUIRE(results[0] == "one");
    REQUIRE(results[1] == "two");
    REQUIRE(results[2] == "three");
}

TEST_CASE("memepp::string_view split - default behavior (back_insert without behavior) keeps empties", "[string_view]")
{
    memepp::string src = "a::b";
    memepp::string_view sv = src;

    std::vector<memepp::string_view> results;
    sv.split(":", std::back_inserter(results));  // uses default keep_empty_parts
    REQUIRE(results.size() == 3);
    REQUIRE(results[0] == "a");
    REQUIRE(results[1] == "");
    REQUIRE(results[2] == "b");
}
