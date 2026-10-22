
#include <catch2/catch.hpp>

#include <memepp/split/self.hpp>
#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/convert/std/string.hpp>
#include <list>
#include <deque>
#include <string>
#include <vector>

#include <megopp/util/scope_cleanup.h>

TEST_CASE("memepp::string split", "[string]")
{
	memepp::string s01 = "11"":"":""22"":3:""444";
    memepp::string s02 =
        "Four-leaf clover is a rare variant of the genus Cheatgrass (which includes the genus Clover and Alfalfa) that also has more than five leaves and up to eighteen leaves. "
        "In the West, it is considered a sign of great good fortune to find four-leaf clover.";

    SECTION("keep_empty_parts retains adjacent empty parts")
    {
        // free function entry
        std::vector<memepp::string> kept;
        memepp::split(s01, ":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(kept));
        REQUIRE(kept.size() == 5);
        REQUIRE(kept[0] == "11");
        REQUIRE(kept[1] == "");
        REQUIRE(kept[2] == "22");
        REQUIRE(kept[3] == "3");
        REQUIRE(kept[4] == "444");

        // member function entry
        kept.clear();
        s01.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(kept));
        REQUIRE(kept.size() == 5);
        REQUIRE(kept[0] == "11");
        REQUIRE(kept[1] == "");
        REQUIRE(kept[2] == "22");
        REQUIRE(kept[3] == "3");
        REQUIRE(kept[4] == "444");
    }

    SECTION("skip_empty_parts drops empty parts")
    {
        // free function entry
        std::vector<memepp::string> skipped;
        memepp::split(s01, ":", memepp::split_behav_t::skip_empty_parts, std::back_inserter(skipped));
        REQUIRE(skipped.size() == 4);
        REQUIRE(skipped[0] == "11");
        REQUIRE(skipped[1] == "22");
        REQUIRE(skipped[2] == "3");
        REQUIRE(skipped[3] == "444");

        // member function entry
        skipped.clear();
        s01.split(":", memepp::split_behav_t::skip_empty_parts, std::back_inserter(skipped));
        REQUIRE(skipped.size() == 4);
        REQUIRE(skipped[0] == "11");
        REQUIRE(skipped[1] == "22");
        REQUIRE(skipped[2] == "3");
        REQUIRE(skipped[3] == "444");
    }

    SECTION("delimiter not found yields whole string")
    {
        std::list<memepp::string> no_match;
        memepp::split(s01, "444", memepp::split_behav_t::skip_empty_parts, std::back_inserter(no_match));
        REQUIRE(no_match.size() == 1);
        REQUIRE(*no_match.begin() == "11::22:3:");
    }

    SECTION("leading delimiter keeps empty head, deque sink")
    {
        std::deque<memepp::string> leading_empty;
        memepp::split(s01, "11", memepp::split_behav_t::keep_empty_parts, std::back_inserter(leading_empty));
        REQUIRE(leading_empty.size() == 2);
        REQUIRE(*leading_empty.rbegin() == "::22:3:444");
    }

    SECTION("trailing delimiter keeps empty tail")
    {
        std::vector<memepp::string> trailing_empty;
        memepp::split(s01, "444", memepp::split_behav_t::keep_empty_parts, std::back_inserter(trailing_empty));
        REQUIRE(trailing_empty.size() == 2);
        REQUIRE(trailing_empty.front() == "11::22:3:");
        REQUIRE(trailing_empty.back() == "");
    }

    SECTION("string_view results stay view storage")
    {
        SECTION("keep_empty_parts")
        {
            std::vector<memepp::string_view> kept_views;
            s01.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(kept_views));
            REQUIRE(kept_views.size() == 5);
            REQUIRE(kept_views[0] == "11");
            REQUIRE(kept_views[0].storage_type() ==
                static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view));
            REQUIRE(kept_views[1] == "");
            REQUIRE(kept_views[1].storage_type() ==
                static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view));
            REQUIRE(kept_views[2] == "22");
            REQUIRE(kept_views[2].storage_type() ==
                static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view));
            REQUIRE(kept_views[3] == "3");
            REQUIRE(kept_views[3].storage_type() ==
                static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view));
            REQUIRE(kept_views[4] == "444");
            REQUIRE(kept_views[4].storage_type() ==
                static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view));
        }

        SECTION("skip_empty_parts")
        {
            std::vector<memepp::string_view> skipped_views;
            s01.split(":", memepp::split_behav_t::skip_empty_parts, std::back_inserter(skipped_views));
            REQUIRE(skipped_views.size() == 4);
            REQUIRE(skipped_views[0] == "11");
            REQUIRE(skipped_views[0].storage_type() ==
                static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view));
            REQUIRE(skipped_views[1] == "22");
            REQUIRE(skipped_views[1].storage_type() ==
                static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view));
            REQUIRE(skipped_views[2] == "3");
            REQUIRE(skipped_views[2].storage_type() ==
                static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view));
            REQUIRE(skipped_views[3] == "444");
            REQUIRE(skipped_views[3].storage_type() ==
                static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view));
        }
    }

    SECTION("default behaviour equals keep_empty_parts")
    {
        std::deque<memepp::string_view> default_parts;
        s01.split(":", std::back_inserter(default_parts));
        REQUIRE(default_parts.size() == 5);
        REQUIRE(default_parts[0] == "11");
        REQUIRE(default_parts[1] == "");
        REQUIRE(default_parts[2] == "22");
        REQUIRE(default_parts[3] == "3");
        REQUIRE(default_parts[4] == "444");
    }

    SECTION("multi-byte delimiter in long text")
    {
        // keep and skip produce identical results here: no empty parts arise from
        // splitting s02 on "clover", so this also cross-checks the two paths agree.
        std::vector<memepp::string> long_kept;
        memepp::split(s02, "clover", memepp::split_behav_t::keep_empty_parts, std::back_inserter(long_kept));
        REQUIRE(long_kept.size() == 3);
        REQUIRE(long_kept[1] == " is a rare variant of the genus Cheatgrass (which includes the genus Clover and Alfalfa) that also has more than five leaves and up to eighteen leaves. "
            "In the West, it is considered a sign of great good fortune to find four-leaf ");

        long_kept.clear();
        s02.split("clover", memepp::split_behav_t::keep_empty_parts, std::back_inserter(long_kept));
        REQUIRE(long_kept.size() == 3);
        REQUIRE(long_kept[1] == " is a rare variant of the genus Cheatgrass (which includes the genus Clover and Alfalfa) that also has more than five leaves and up to eighteen leaves. "
            "In the West, it is considered a sign of great good fortune to find four-leaf ");

        std::vector<memepp::string> long_skipped;
        memepp::split(s02, "clover", memepp::split_behav_t::skip_empty_parts, std::back_inserter(long_skipped));
        REQUIRE(long_skipped.size() == 3);
        REQUIRE(long_skipped[1] == " is a rare variant of the genus Cheatgrass (which includes the genus Clover and Alfalfa) that also has more than five leaves and up to eighteen leaves. "
            "In the West, it is considered a sign of great good fortune to find four-leaf ");
    }

    SECTION("string_view source splits correctly")
    {
        memepp::string_view sv = s01;

        SECTION("keep_empty_parts")
        {
            std::vector<memepp::string_view> sv_kept;
            sv.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(sv_kept));
            REQUIRE(sv_kept.size() == 5);
            REQUIRE(sv_kept[0] == "11");
            REQUIRE(sv_kept[1] == "");
            REQUIRE(sv_kept[2] == "22");
            REQUIRE(sv_kept[3] == "3");
            REQUIRE(sv_kept[4] == "444");
        }

        SECTION("skip_empty_parts")
        {
            std::vector<memepp::string_view> sv_skipped;
            sv.split(":", memepp::split_behav_t::skip_empty_parts, std::back_inserter(sv_skipped));
            REQUIRE(sv_skipped.size() == 4);
            REQUIRE(sv_skipped[0] == "11");
            REQUIRE(sv_skipped[1] == "22");
            REQUIRE(sv_skipped[2] == "3");
            REQUIRE(sv_skipped[3] == "444");
        }

        SECTION("default behaviour")
        {
            std::deque<memepp::string_view> sv_default_parts;
            sv.split(":", std::back_inserter(sv_default_parts));
            REQUIRE(sv_default_parts.size() == 5);
            REQUIRE(sv_default_parts[0] == "11");
            REQUIRE(sv_default_parts[1] == "");
            REQUIRE(sv_default_parts[2] == "22");
            REQUIRE(sv_default_parts[3] == "3");
            REQUIRE(sv_default_parts[4] == "444");
        }
    }

    SECTION("empty string_view source")
    {
        memepp::string_view empty_sv;

        SECTION("keep_empty_parts yields one empty part")
        {
            std::list<memepp::string_view> empty_kept;
            empty_sv.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(empty_kept));
            REQUIRE(empty_kept.size() == 1);
            REQUIRE(empty_kept.front() == "");
        }

        SECTION("skip_empty_parts yields nothing")
        {
            std::list<memepp::string_view> empty_skipped;
            empty_sv.split(":", memepp::split_behav_t::skip_empty_parts, std::back_inserter(empty_skipped));
            REQUIRE(empty_skipped.size() == 0);
        }
    }
}

TEST_CASE("memepp::string split - user storage", "[string]")
{
	// user storage: only produced by from() for inputs above the medium limit.
	std::string user_data(500, 'a');
	user_data[100] = ':';
	user_data[200] = ':';
	user_data[300] = ':';
	memepp::string user_str = memepp::from(std::move(user_data));
	REQUIRE(user_str.storage_type() == memepp::string_storage_t::user);
	REQUIRE(user_str.size() == 500);

	std::vector<memepp::string_view> results;
	user_str.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(results));
	REQUIRE(results.size() == 4);
	REQUIRE(results[0].size() == 100);
	REQUIRE(results[1].size() == 99);
	REQUIRE(results[2].size() == 99);
	REQUIRE(results[3].size() == 199);
}
