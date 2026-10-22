
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
        mmint_t n_free = memepp::split(s01, ":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(kept));
        REQUIRE(n_free == 0);
        REQUIRE(kept.size() == 5);
        REQUIRE(kept[0] == "11");
        REQUIRE(kept[1] == "");
        REQUIRE(kept[2] == "22");
        REQUIRE(kept[3] == "3");
        REQUIRE(kept[4] == "444");

        // member function entry
        kept.clear();
        mmint_t n_member = s01.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(kept));
        REQUIRE(n_member == 0);
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
        mmint_t n_free = memepp::split(s01, ":", memepp::split_behav_t::skip_empty_parts, std::back_inserter(skipped));
        REQUIRE(n_free == 0);
        REQUIRE(skipped.size() == 4);
        REQUIRE(skipped[0] == "11");
        REQUIRE(skipped[1] == "22");
        REQUIRE(skipped[2] == "3");
        REQUIRE(skipped[3] == "444");

        // member function entry
        skipped.clear();
        mmint_t n_member = s01.split(":", memepp::split_behav_t::skip_empty_parts, std::back_inserter(skipped));
        REQUIRE(n_member == 0);
        REQUIRE(skipped.size() == 4);
        REQUIRE(skipped[0] == "11");
        REQUIRE(skipped[1] == "22");
        REQUIRE(skipped[2] == "3");
        REQUIRE(skipped[3] == "444");
    }

    SECTION("delimiter not found yields whole string")
    {
        std::list<memepp::string> no_match;
        mmint_t n = memepp::split(s01, "444", memepp::split_behav_t::skip_empty_parts, std::back_inserter(no_match));
        REQUIRE(n == 0);
        REQUIRE(no_match.size() == 1);
        REQUIRE(*no_match.begin() == "11::22:3:");
    }

    SECTION("leading delimiter keeps empty head, deque sink")
    {
        std::deque<memepp::string> leading_empty;
        mmint_t n = memepp::split(s01, "11", memepp::split_behav_t::keep_empty_parts, std::back_inserter(leading_empty));
        REQUIRE(n == 0);
        REQUIRE(leading_empty.size() == 2);
        REQUIRE(*leading_empty.rbegin() == "::22:3:444");
    }

    SECTION("trailing delimiter keeps empty tail")
    {
        std::vector<memepp::string> trailing_empty;
        mmint_t n = memepp::split(s01, "444", memepp::split_behav_t::keep_empty_parts, std::back_inserter(trailing_empty));
        REQUIRE(n == 0);
        REQUIRE(trailing_empty.size() == 2);
        REQUIRE(trailing_empty.front() == "11::22:3:");
        REQUIRE(trailing_empty.back() == "");
    }

    SECTION("string_view results stay view storage")
    {
        SECTION("keep_empty_parts")
        {
            std::vector<memepp::string_view> kept_views;
            mmint_t n = s01.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(kept_views));
            REQUIRE(n == 0);
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
            mmint_t n = s01.split(":", memepp::split_behav_t::skip_empty_parts, std::back_inserter(skipped_views));
            REQUIRE(n == 0);
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
        mmint_t n = s01.split(":", std::back_inserter(default_parts));
        REQUIRE(n == 0);
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
        mmint_t n_free = memepp::split(s02, "clover", memepp::split_behav_t::keep_empty_parts, std::back_inserter(long_kept));
        REQUIRE(n_free == 0);
        REQUIRE(long_kept.size() == 3);
        REQUIRE(long_kept[1] == " is a rare variant of the genus Cheatgrass (which includes the genus Clover and Alfalfa) that also has more than five leaves and up to eighteen leaves. "
            "In the West, it is considered a sign of great good fortune to find four-leaf ");

        long_kept.clear();
        mmint_t n_member = s02.split("clover", memepp::split_behav_t::keep_empty_parts, std::back_inserter(long_kept));
        REQUIRE(n_member == 0);
        REQUIRE(long_kept.size() == 3);
        REQUIRE(long_kept[1] == " is a rare variant of the genus Cheatgrass (which includes the genus Clover and Alfalfa) that also has more than five leaves and up to eighteen leaves. "
            "In the West, it is considered a sign of great good fortune to find four-leaf ");

        std::vector<memepp::string> long_skipped;
        mmint_t n_skipped = memepp::split(s02, "clover", memepp::split_behav_t::skip_empty_parts, std::back_inserter(long_skipped));
        REQUIRE(n_skipped == 0);
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
            mmint_t n = sv.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(sv_kept));
            REQUIRE(n == 0);
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
            mmint_t n = sv.split(":", memepp::split_behav_t::skip_empty_parts, std::back_inserter(sv_skipped));
            REQUIRE(n == 0);
            REQUIRE(sv_skipped.size() == 4);
            REQUIRE(sv_skipped[0] == "11");
            REQUIRE(sv_skipped[1] == "22");
            REQUIRE(sv_skipped[2] == "3");
            REQUIRE(sv_skipped[3] == "444");
        }

        SECTION("default behaviour")
        {
            std::deque<memepp::string_view> sv_default_parts;
            mmint_t n = sv.split(":", std::back_inserter(sv_default_parts));
            REQUIRE(n == 0);
            REQUIRE(sv_default_parts.size() == 5);
            REQUIRE(sv_default_parts[0] == "11");
            REQUIRE(sv_default_parts[1] == "");
            REQUIRE(sv_default_parts[2] == "22");
            REQUIRE(sv_default_parts[3] == "3");
            REQUIRE(sv_default_parts[4] == "444");
        }
    }

    SECTION("empty source with string result container")
    {
        memepp::string empty_str;

        SECTION("keep_empty_parts yields one empty string part")
        {
            std::vector<memepp::string> kept;
            mmint_t n = empty_str.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(kept));
            REQUIRE(n == 0);
            REQUIRE(kept.size() == 1);
            REQUIRE(kept[0] == "");
        }

        SECTION("skip_empty_parts yields nothing")
        {
            std::vector<memepp::string> skipped;
            mmint_t n = empty_str.split(":", memepp::split_behav_t::skip_empty_parts, std::back_inserter(skipped));
            REQUIRE(n == 0);
            REQUIRE(skipped.size() == 0);
        }
    }

    SECTION("key longer than source")
    {
        memepp::string short_str = "ab";

        SECTION("keep_empty_parts returns whole string")
        {
            std::vector<memepp::string> kept;
            mmint_t n = short_str.split("abcdef", memepp::split_behav_t::keep_empty_parts, std::back_inserter(kept));
            REQUIRE(n == 0);
            REQUIRE(kept.size() == 1);
            REQUIRE(kept[0] == "ab");
        }

        SECTION("skip_empty_parts returns whole string")
        {
            std::vector<memepp::string> skipped;
            mmint_t n = short_str.split("abcdef", memepp::split_behav_t::skip_empty_parts, std::back_inserter(skipped));
            REQUIRE(n == 0);
            REQUIRE(skipped.size() == 1);
            REQUIRE(skipped[0] == "ab");
        }
    }

    SECTION("key equals entire source")
    {
        memepp::string exact = "abc";

        SECTION("keep_empty_parts yields two empty parts")
        {
            std::vector<memepp::string> kept;
            mmint_t n = exact.split("abc", memepp::split_behav_t::keep_empty_parts, std::back_inserter(kept));
            REQUIRE(n == 0);
            REQUIRE(kept.size() == 2);
            REQUIRE(kept[0] == "");
            REQUIRE(kept[1] == "");
        }

        SECTION("skip_empty_parts yields nothing")
        {
            std::vector<memepp::string> skipped;
            mmint_t n = exact.split("abc", memepp::split_behav_t::skip_empty_parts, std::back_inserter(skipped));
            REQUIRE(n == 0);
            REQUIRE(skipped.size() == 0);
        }
    }

    SECTION("empty string_view source")
    {
        memepp::string_view empty_sv;

        SECTION("keep_empty_parts yields one empty part")
        {
            std::list<memepp::string_view> empty_kept;
            mmint_t n = empty_sv.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(empty_kept));
            REQUIRE(n == 0);
            REQUIRE(empty_kept.size() == 1);
            REQUIRE(empty_kept.front() == "");
        }

        SECTION("skip_empty_parts yields nothing")
        {
            std::list<memepp::string_view> empty_skipped;
            mmint_t n = empty_sv.split(":", memepp::split_behav_t::skip_empty_parts, std::back_inserter(empty_skipped));
            REQUIRE(n == 0);
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
	mmint_t n = user_str.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(results));
	REQUIRE(n == 0);
	REQUIRE(results.size() == 4);
	REQUIRE(results[0].size() == 100);
	REQUIRE(results[1].size() == 99);
	REQUIRE(results[2].size() == 99);
	REQUIRE(results[3].size() == 199);
}
