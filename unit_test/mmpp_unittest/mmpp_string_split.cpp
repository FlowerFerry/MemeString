
#include <catch2/catch.hpp>

#include <memepp/split/self.hpp>
#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <list>
#include <deque>

TEST_CASE("memepp::string - 07", "string split")
{
	memepp::string s01 = "11"":"":""22"":3:""444";

	std::vector<memepp::string> results011;
	memepp::split(s01, ":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(results011));
	REQUIRE(results011.size() == 5);
	REQUIRE(results011[0] == "11");
	REQUIRE(results011[1] == "");
	REQUIRE(results011[2] == "22");
	REQUIRE(results011[3] == "3");
	REQUIRE(results011[4] == "444");

	results011.clear();
	s01.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(results011));
	REQUIRE(results011.size() == 5);
	REQUIRE(results011[0] == "11");
	REQUIRE(results011[1] == "");
	REQUIRE(results011[2] == "22");
	REQUIRE(results011[3] == "3");
	REQUIRE(results011[4] == "444");

	std::vector<memepp::string> results012;
	memepp::split(s01, ":", memepp::split_behav_t::skip_empty_parts, std::back_inserter(results012));
	REQUIRE(results012.size() == 4);
	REQUIRE(results012[0] == "11");
	REQUIRE(results012[1] == "22");
	REQUIRE(results012[2] == "3");
	REQUIRE(results012[3] == "444");

	results012.clear();
	s01.split(":", memepp::split_behav_t::skip_empty_parts, std::back_inserter(results012));
	REQUIRE(results012.size() == 4);
	REQUIRE(results012[0] == "11");
	REQUIRE(results012[1] == "22");
	REQUIRE(results012[2] == "3");
	REQUIRE(results012[3] == "444");

	std::list<memepp::string> results013;
	memepp::split(s01, "444", memepp::split_behav_t::skip_empty_parts, std::back_inserter(results013));
	REQUIRE(results013.size() == 1);
	REQUIRE(*results013.begin() == "11::22:3:");


	std::deque<memepp::string> results014;
	memepp::split(s01, "11", memepp::split_behav_t::keep_empty_parts, std::back_inserter(results014));
	REQUIRE(results014.size() == 2);
	REQUIRE(*results014.rbegin() == "::22:3:444");

	std::vector<memepp::string> results015;
	memepp::split(s01, "444", memepp::split_behav_t::keep_empty_parts, std::back_inserter(results015));
	REQUIRE(results015.size() == 2);
	REQUIRE(results015.front() == "11::22:3:");
	REQUIRE(results015.back() == "");

	std::vector<memepp::string_view> results016;
	s01.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(results016));
	REQUIRE(results016.size() == 5);
	REQUIRE(results016[0] == "11");
	REQUIRE(results016[0].storage_type() ==
		static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view));
	REQUIRE(results016[1] == "");
	REQUIRE(results016[1].storage_type() ==
		static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view));
	REQUIRE(results016[2] == "22");
	REQUIRE(results016[2].storage_type() ==
		static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view));
	REQUIRE(results016[3] == "3");
	REQUIRE(results016[3].storage_type() ==
		static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view));
	REQUIRE(results016[4] == "444");
	REQUIRE(results016[4].storage_type() ==
		static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view));

	std::vector<memepp::string_view> results017;
	s01.split(":", memepp::split_behav_t::skip_empty_parts, std::back_inserter(results017));
	REQUIRE(results017.size() == 4);
	REQUIRE(results017[0] == "11");
	REQUIRE(results017[0].storage_type() ==
		static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view));
	REQUIRE(results017[1] == "22");
	REQUIRE(results017[1].storage_type() ==
		static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view));
	REQUIRE(results017[2] == "3");
	REQUIRE(results017[2].storage_type() ==
		static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view));
	REQUIRE(results017[3] == "444");
	REQUIRE(results017[3].storage_type() ==
		static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view));

	std::deque<memepp::string_view> l18_s01;
    s01.split(":", std::back_inserter(l18_s01));
	REQUIRE(l18_s01.size() == 5);
	REQUIRE(l18_s01[0] == "11");
	REQUIRE(l18_s01[1] == "");
	REQUIRE(l18_s01[2] == "22");
	REQUIRE(l18_s01[3] == "3");
	REQUIRE(l18_s01[4] == "444");

	memepp::string s02 =
		"Four-leaf clover is a rare variant of the genus Cheatgrass (which includes the genus Clover and Alfalfa) that also has more than five leaves and up to eighteen leaves. "
		"In the West, it is considered a sign of great good fortune to find four-leaf clover.";

	std::vector<memepp::string> results021;
	memepp::split(s02, "clover", memepp::split_behav_t::keep_empty_parts, std::back_inserter(results021));
	REQUIRE(results021.size() == 3);
	REQUIRE(results021[1] == " is a rare variant of the genus Cheatgrass (which includes the genus Clover and Alfalfa) that also has more than five leaves and up to eighteen leaves. "
		"In the West, it is considered a sign of great good fortune to find four-leaf ");

	results021.clear();
	s02.split("clover", memepp::split_behav_t::keep_empty_parts, std::back_inserter(results021));
	REQUIRE(results021.size() == 3);
	REQUIRE(results021[1] == " is a rare variant of the genus Cheatgrass (which includes the genus Clover and Alfalfa) that also has more than five leaves and up to eighteen leaves. "
		"In the West, it is considered a sign of great good fortune to find four-leaf ");

	std::vector<memepp::string> results022;
	memepp::split(s02, "clover", memepp::split_behav_t::skip_empty_parts, std::back_inserter(results022));
	REQUIRE(results022.size() == 3);
	REQUIRE(results022[1] == " is a rare variant of the genus Cheatgrass (which includes the genus Clover and Alfalfa) that also has more than five leaves and up to eighteen leaves. "
		"In the West, it is considered a sign of great good fortune to find four-leaf ");

	memepp::string_view sv01 = s01;

	std::vector<memepp::string_view> l01_sv01;
	sv01.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(l01_sv01));
    REQUIRE(l01_sv01.size() == 5);
    REQUIRE(l01_sv01[0] == "11");
    REQUIRE(l01_sv01[1] == "");
    REQUIRE(l01_sv01[2] == "22");
    REQUIRE(l01_sv01[3] == "3");
    REQUIRE(l01_sv01[4] == "444");

    std::vector<memepp::string_view> l02_sv01;
    sv01.split(":", memepp::split_behav_t::skip_empty_parts, std::back_inserter(l02_sv01));
    REQUIRE(l02_sv01.size() == 4);
    REQUIRE(l02_sv01[0] == "11");
    REQUIRE(l02_sv01[1] == "22");
    REQUIRE(l02_sv01[2] == "3");
    REQUIRE(l02_sv01[3] == "444");

	memepp::string_view sv02;
    std::list<memepp::string_view> l01_sv02;
    sv02.split(":", memepp::split_behav_t::keep_empty_parts, std::back_inserter(l01_sv02));
    REQUIRE(l01_sv02.size() == 1);
    REQUIRE(l01_sv02.front() == "");

    std::list<memepp::string_view> l02_sv02;
    sv02.split(":", memepp::split_behav_t::skip_empty_parts, std::back_inserter(l02_sv02));
    REQUIRE(l02_sv02.size() == 0);

	std::deque<memepp::string_view> l03_sv01;
	sv01.split(":", std::back_inserter(l03_sv01));
	REQUIRE(l03_sv01.size() == 5);
	REQUIRE(l03_sv01[0] == "11");
	REQUIRE(l03_sv01[1] == "");
	REQUIRE(l03_sv01[2] == "22");
	REQUIRE(l03_sv01[3] == "3");
	REQUIRE(l03_sv01[4] == "444");
}
