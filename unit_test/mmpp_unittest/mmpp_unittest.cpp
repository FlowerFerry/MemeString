
#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <megopp/util/scope_cleanup.h>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#pragma comment(lib, "libmeme_string.lib")

const char test_large_string_01[] = 
		"C++是一种被广泛使用的计算机程序设计语言。"
		"它是一种通用程序设计语言，支持多重编程范式，例如过程化程序设计、数据抽象、面向对象程序设计、泛型程序设计和设计模式等。"
		"比雅尼·斯特劳斯特鲁普博士在贝尔实验室工作期间在20世纪80年代发明并实现了C++。"
		"起初，这种语言被称作“C with Classes”（“包含‘类’的C语言”），作为C语言的增强版出现。"
		"随后，C++不断增加新特性。虚函数（virtual function）、"
		"运算符重载（operator overloading）、多继承（multiple inheritance）、"
		"标准模板库（standard template library, STL）、异常处理（exception）、"
		"运行时类型信息（runtime type information）、名字空间（namespace）等概念逐渐纳入标准。1998年，"
		"国际标准组织（ISO）颁布了C++程序设计语言的第一个国际标准ISO / IEC 14882:1998，"
		"目前最新标准为ISO / IEC 14882 : 2020。根据《C++编程思想》（Thinking in C++）一书，"
		"C++与C的代码执行效率往往相差在 ±5% 之间。";

TEST_CASE("memepp::string - 01", "string constructions") 
{
	REQUIRE(sizeof(memepp::string) == MEME_STRING__OBJECT_SIZE);

	memepp::string s01;
	REQUIRE(s01.storage_type() == memepp::string_storage_t::small);
	REQUIRE(s01.size() == 0);
	REQUIRE(s01.size() == strlen(s01.data()) );
	REQUIRE(s01.size() == strlen(s01.c_str()));
	REQUIRE(s01.empty());
	REQUIRE(s01.capacity() == MEME_STRING__OBJECT_SIZE - 2);

#if INTPTR_MAX == INT64_MAX
	const char* test01 = "This is test string...";
#else
    const char* test01 = "teststring";
#endif
	memepp::string s02(test01, MEME_STRING__OBJECT_SIZE - 2);
	REQUIRE(s02.storage_type() == memepp::string_storage_t::small);
	REQUIRE(s02.size() == MEME_STRING__OBJECT_SIZE - 2);
	REQUIRE(s02.size() == strlen(s02.data()) );
	REQUIRE(s02.size() == strlen(s02.c_str()));
	REQUIRE(!strcmp(s02.data() , test01));
	REQUIRE(!strcmp(s02.c_str(), test01));
	REQUIRE(s02 == test01);
	REQUIRE(test01 == s02);
	REQUIRE(s02.empty() == false);
	REQUIRE(s02.capacity() == 0);

#if INTPTR_MAX == INT64_MAX
	const char* test02 = "This is test string....";
#else
    const char* test02 = "test string";
#endif
    memepp::string s03(test02, MEME_STRING__OBJECT_SIZE - 1);
    REQUIRE(s03.storage_type() == memepp::string_storage_t::medium);
    REQUIRE(s03.size() == strlen(test02));
    REQUIRE(s03.size() == strlen(s03.data()));
    REQUIRE(s03.size() == strlen(s03.c_str()));
    REQUIRE(!strcmp(s03.data(), test02));
    REQUIRE(!strcmp(s03.c_str(), test02));
    REQUIRE(s03 == test02);
    REQUIRE(test02 == s03);
    REQUIRE(s03.empty() == false);
	REQUIRE(MemeString_checkHeadTailMemory(memepp::to_pointer(s03.native_handle())) == 1);

	memepp::string s04("test");
	REQUIRE(s04.storage_type() == memepp::string_storage_t::small);
	REQUIRE(s04.size() == 4);
	REQUIRE(s04.size() == strlen(s04.data()) );
	REQUIRE(s04.size() == strlen(s04.c_str()));
	REQUIRE(!strcmp(s04.data() , "test"));
	REQUIRE(!strcmp(s04.c_str(), "test"));
	REQUIRE(s04 == "test");
	REQUIRE("test" == s04);
	REQUIRE(s04.empty() == false);
	REQUIRE(s04.capacity() == (MEME_STRING__OBJECT_SIZE - 2) - 4);

    memepp::string s05("test", 2);
    REQUIRE(s05.storage_type() == memepp::string_storage_t::small);
    REQUIRE(s05.size() == 2);
    REQUIRE(s05.size() == strlen(s05.data()));
    REQUIRE(s05.size() == strlen(s05.c_str()));
    REQUIRE(!strcmp(s05.data(), "te"));
    REQUIRE(!strcmp(s05.c_str(), "te"));
    REQUIRE(s05 == "te");
    REQUIRE("te" == s05);
    REQUIRE(s05.empty() == false);
    REQUIRE(s05.capacity() == (MEME_STRING__OBJECT_SIZE - 2) - 2);
    
    memepp::string s06("test", 4);
    REQUIRE(s06.storage_type() == memepp::string_storage_t::small);
    REQUIRE(s06.size() == 4);
    REQUIRE(s06.size() == strlen(s06.data()));
    REQUIRE(s06.size() == strlen(s06.c_str()));
    REQUIRE(!strcmp(s06.data(), "test"));
    REQUIRE(!strcmp(s06.c_str(), "test"));
    REQUIRE(s06 == "test");
    REQUIRE("test" == s06);
    REQUIRE(s06.empty() == false);
    REQUIRE(s06.capacity() == (MEME_STRING__OBJECT_SIZE - 2) - 4);

}

TEST_CASE("memepp::string - 02", "string constructions")
{

	memepp::string s03(test_large_string_01);

	REQUIRE(s03.storage_type() == memepp::string_storage_t::large);
	REQUIRE(s03.empty() == false);
    REQUIRE(s03.size() == strlen(test_large_string_01));
    REQUIRE(s03.size() == strlen(s03.data()));
    REQUIRE(s03.size() == strlen(s03.c_str()));
    REQUIRE(!strcmp(s03.data(), test_large_string_01));
    REQUIRE(!strcmp(s03.c_str(), test_large_string_01));
    REQUIRE(s03 == test_large_string_01);
    REQUIRE(test_large_string_01 == s03);
	REQUIRE(MemeString_checkHeadTailMemory(memepp::to_pointer(s03.native_handle())) == 1);
    

}

TEST_CASE("memepp::string - 03", "string move or swap")
{
	memepp::string s01(memepp::string { test_large_string_01 });
	REQUIRE(s01.empty() == false);
	REQUIRE(s01.size() == strlen(test_large_string_01));
	REQUIRE( !strcmp(s01.data(), test_large_string_01));
	REQUIRE(s01.storage_type() == memepp::string_storage_t::large);
	REQUIRE(MemeString_checkHeadTailMemory(memepp::to_pointer(s01.native_handle())) == 1);

	memepp::string s02;
	s02.swap(s01);
	REQUIRE(s01.empty());
	REQUIRE(s01.size() == 0);
	REQUIRE(s01.storage_type() == memepp::string_storage_t::small);
	REQUIRE(s02.empty() == false);
	REQUIRE(s02.size() == strlen(test_large_string_01));
	REQUIRE( !strcmp(s02.data(), test_large_string_01));
	REQUIRE(s02.storage_type() == memepp::string_storage_t::large);
	REQUIRE(MemeString_checkHeadTailMemory(memepp::to_pointer(s02.native_handle())) == 1);

	memepp::string s03 = std::move(s02);
	REQUIRE(s02.empty());
	REQUIRE(s02.size() == 0);
	REQUIRE(s02.storage_type() == memepp::string_storage_t::small);
	REQUIRE(s03.empty() == false);
	REQUIRE(s03.size() == strlen(test_large_string_01));
	REQUIRE( !strcmp(s03.data(), test_large_string_01));
	REQUIRE(s03 == test_large_string_01);
	REQUIRE(s03.storage_type() == memepp::string_storage_t::large);
	REQUIRE(MemeString_checkHeadTailMemory(memepp::to_pointer(s03.native_handle())) == 1);

	memepp::string s04; 
	s04 = s03;

	s01 = std::move(s03);
	REQUIRE(s03.empty());
	REQUIRE(s03.size() == 0);
	REQUIRE(s03.storage_type() == memepp::string_storage_t::small);
	REQUIRE(s01.empty() == false);
	REQUIRE(s01.size() == strlen(test_large_string_01));
	REQUIRE( !strcmp(s01.data(), test_large_string_01));
	REQUIRE(s01.storage_type() == memepp::string_storage_t::large);
	REQUIRE(MemeString_checkHeadTailMemory(memepp::to_pointer(s01.native_handle())) == 1);


    
}

#include <memepp/convert/std/string.hpp>
#include <memepp/compare/std/string.hpp>

TEST_CASE("memepp::string - 04", "string mutual convert of std::string")
{
	std::string stdstr01;
	auto s01 = memepp::from(stdstr01);
	auto s02 = memepp::from(std::move(stdstr01));
	REQUIRE(s01.storage_type() == memepp::string_storage_t::small);
	REQUIRE(s01.size() == 0);
	REQUIRE(s01.size() == strlen(s01.data()));
	REQUIRE(s01.size() == strlen(s01.c_str()));
	REQUIRE(s01.empty());
	REQUIRE(s01.capacity() == MEME_STRING__OBJECT_SIZE - 2);
	REQUIRE(s02.storage_type() == memepp::string_storage_t::small);
	REQUIRE(s02.size() == 0);
	REQUIRE(s02.size() == strlen(s02.data()));
	REQUIRE(s02.size() == strlen(s02.c_str()));
	REQUIRE(s02.empty());
	REQUIRE(s02.capacity() == MEME_STRING__OBJECT_SIZE - 2);
	REQUIRE(s01 == s02);

    std::string stdstr02 = "test";
    auto s03 = memepp::from(stdstr02);
    auto s04 = memepp::from(std::move(stdstr02));
    REQUIRE(s03.storage_type() == memepp::string_storage_t::small);
    REQUIRE(s03.size() == 4);
    REQUIRE(s03.size() == strlen(s03.data()));
    REQUIRE(s03.size() == strlen(s03.c_str()));
    REQUIRE(!strcmp(s03.data(), "test"));
    REQUIRE(!strcmp(s03.c_str(), "test"));
    REQUIRE(s03 == "test");
    REQUIRE("test" == s03);
    REQUIRE(s03.empty() == false);
    REQUIRE(s03.capacity() == (MEME_STRING__OBJECT_SIZE - 2) - 4);
    REQUIRE(s04.storage_type() == memepp::string_storage_t::small);
    REQUIRE(s04.size() == 4);
    REQUIRE(s04.size() == strlen(s04.data()));
    REQUIRE(s04.size() == strlen(s04.c_str()));
    REQUIRE(!strcmp(s04.data(), "test"));
    REQUIRE(!strcmp(s04.c_str(), "test"));
    REQUIRE(s04 == "test");
    REQUIRE("test" == s04);
    REQUIRE(s04.empty() == false);
    REQUIRE(s04.capacity() == (MEME_STRING__OBJECT_SIZE - 2) - 4);
    REQUIRE(s03 == s04);
    
	std::string stdstr030 { "test 123456789123456790", MEME_STRING__OBJECT_SIZE - 1 };
	std::string stdstr031 { "test 123456789123456790", MEME_STRING__OBJECT_SIZE - 1 };
    auto s05 = memepp::from(stdstr030);
    auto s06 = memepp::from(std::move(stdstr030));
    REQUIRE(s05.storage_type() == memepp::string_storage_t::medium);
    REQUIRE(s05.size() == MEME_STRING__OBJECT_SIZE - 1);
    REQUIRE(s05.size() == strlen(s05.data()));
    REQUIRE(s05.size() == strlen(s05.c_str()));
    REQUIRE(!strcmp(s05.data(), stdstr031.data()));
    REQUIRE(!strcmp(s05.c_str(), stdstr031.c_str()));
    REQUIRE(s05 == stdstr031);
    REQUIRE(stdstr031 == s05);
    REQUIRE(s05.empty() == false);
    REQUIRE(s06.storage_type() == memepp::string_storage_t::medium);
    REQUIRE(s06.size() == MEME_STRING__OBJECT_SIZE - 1);
    REQUIRE(s06.size() == strlen(s06.data()));
    REQUIRE(s06.size() == strlen(s06.c_str()));
    REQUIRE(!strcmp(s06.data(), stdstr031.data()));
    REQUIRE(!strcmp(s06.c_str(), stdstr031.c_str()));
    REQUIRE(s06 == stdstr031);
    REQUIRE(stdstr031 == s06);
    REQUIRE(s06.empty() == false);
    REQUIRE(s05 == s06);
	REQUIRE(MemeString_checkHeadTailMemory(memepp::to_pointer(s05.native_handle())) == 1);

	std::string stdstr11 = 
		"Four-leaf clover is a rare variant of the genus Cheatgrass (which includes the genus Clover and Alfalfa) that also has more than five leaves and up to eighteen leaves. "
		"In the West, it is considered a sign of great good fortune to find four-leaf clover.";
	auto s11 = memepp::from(stdstr11);
	REQUIRE(s11.storage_type() == memepp::string_storage_t::large);
	REQUIRE(s11.size() == stdstr11.size());
	REQUIRE(!strcmp(s11.data(), stdstr11.data()));
	REQUIRE(s11 == stdstr11);
	REQUIRE((s11 != stdstr11) == false);
	//auto s12 = memepp::fromUnsafe(std::move(stdstr11));
	//REQUIRE(s12.storage_type() == memepp::string_storage_t::user);
	//REQUIRE(s12.size() == s11.size());
	//REQUIRE(!strcmp(s12.data(), s11.data()));
	//REQUIRE(s11 == s12);
	//REQUIRE(s12 == s11);
	//REQUIRE(stdstr11.empty());
	//REQUIRE(MemeString_checkHeadTailMemory(memepp::to_pointer(s11.native_handle())) == 1);
	//REQUIRE(MemeString_checkHeadTailMemory(memepp::to_pointer(s12.native_handle())) == 1);
}

#include <memepp/compare/std/vector.hpp>

TEST_CASE("memepp::string - 05", "string mutual convert of std::vector")
{
}

TEST_CASE("memepp::string - 06", "string index of")
{
	memepp::string s01 = "etebjkgdodoijnakccv";
    
	REQUIRE(s01.find("etebjkgdodoijnakccv", 1) == -1);
	REQUIRE(s01.find("etebjkgdodoijnakccv", 0) == 0);
    REQUIRE(s01.find("etebjkgdodoijnakccv") == 0);
 //   REQUIRE(s01.find("etebjkgdodoijnakccv", 0, 1) == 0);
 //   REQUIRE(s01.find("etebjkgdodoijnakccv", 0, 2) == 0);
 //   REQUIRE(s01.find("etebjkgdodoijnakccv", 0, 3) == 0);
 //   REQUIRE(s01.find("etebjkgdodoijnakccv", 0, 4) == 0);
 //   REQUIRE(s01.find("etebjkgdodoijnakccv", 0, 5) == 0);
 //   REQUIRE(s01.find("etebjkgdodoijnakccv", 0, 6) == 0);
 //   REQUIRE(s01.find("etebjkgdodoijnakccv", 0, 7) == 0);
 //   REQUIRE(s01.find("etebjkgdodoijnakccv", 0, 8) == 0);
 //   REQUIRE(s01.find("etebjkgdodoijnakccv", 0, 9) == 0);
 //   REQUIRE(s01.find("etebjkgdodoijnakccv", 0, 10) == 0);
 //   REQUIRE(s01.find("etebjkgdodoijnakccv", 0, 11) == 0);
 //   REQUIRE(s01.find("etebjkgdodoijnakccv", 0, 12) == 0);
 //   REQUIRE(s01.find("etebjkgdodoijnakccv", 0, 13) == 0);
 //   REQUIRE(s01.find("etebjkgdodoijnakccv", 0, 14) == 0);
 //   REQUIRE(s01.find("etebjkgdodoijnakccv", 0, 15) == 0);
 //   REQUIRE(s01.find("etebjkgdodoijnakccv", 0, 16) == 0);
 //   REQUIRE(s01.find("etebjkgdodoijnakccv", 0, 17) == 0);
 //   REQUIRE(s01.find("etebjkgdodoijnakccv", 0, 18) == 0);
 //   REQUIRE(s01.find("etebjkgdodoijnakccv", 0, 19) == 0);
	//REQUIRE(s01.find("etebjkgdodoijnakccv", 1, 1) == 2);
	//REQUIRE(s01.find("k", 3, 10) == 5);
	
	REQUIRE(s01.index_of("ete") == 0);
    REQUIRE(s01.index_of("etebjkgdodoijnakccv") == 0);
	REQUIRE(s01.index_of("eteb") == 0);
	REQUIRE(s01.index_of("kccv") == s01.size() -4);
	REQUIRE(s01.index_of("gdod") == 6);
	REQUIRE(s01.index_of("asdggg") == -1);
	REQUIRE(s01.index_of("etebjkgdodoijnakccv1") == -1);
	REQUIRE(s01.index_of("b") == 3);
	REQUIRE(s01.index_of("B", memepp::case_sensit_t::all_insensitive) == 3);
    REQUIRE(s01.index_of("b", memepp::case_sensit_t::all_insensitive) == 3);
    REQUIRE(s01.index_of("v", memepp::case_sensit_t::all_insensitive) == s01.size() - 1);
	
	memepp::string s02;
	REQUIRE(s02.index_of("0") == -1);
    REQUIRE(s02.index_of("0", memepp::case_sensit_t::all_insensitive) == -1);
    
	memepp::string s03_01 = "sdsjlkas odf!fsd     hks  jlkdd hkfd hjs fsd fdfsd sfdhjs&*^&(*GDUHDJLK h35 yuisisfFSIDFUk834 df";
	REQUIRE(s03_01.index_of("sfdhjs", true) == 51);
	REQUIRE(s03_01.index_of("sd", true) == -1);
	REQUIRE(s03_01.index_of("df", true) == 94);
	REQUIRE(s03_01.index_of("sdsjlkas", true) == 0);
	REQUIRE(s03_01.index_of("fsd", true) == 13);
	REQUIRE(s03_01.index_of("fsd", -1, 13, true) == 41);
	//REQUIRE(s03_01.index_of(" ", true) == 8);
}


#include "memepp/split/self.hpp"
#include <list>
#include <array>
#include <deque>
#include <vector>

TEST_CASE("memepp::string - 07", "string split")
{
	memepp::string s01 = "11"":"":""22"":3:""444";
	
	std::vector<memepp::string> results011;
	memepp::split(s01, ":", memepp::split_behavior_t::keep_empty_parts, std::back_inserter(results011));
	REQUIRE(results011.size() == 5);
    REQUIRE(results011[0] == "11");
    REQUIRE(results011[1] == "");
    REQUIRE(results011[2] == "22");
    REQUIRE(results011[3] == "3");
    REQUIRE(results011[4] == "444");

	results011.clear();
	s01.split(":", memepp::split_behavior_t::keep_empty_parts, std::back_inserter(results011));
	REQUIRE(results011.size() == 5);
    REQUIRE(results011[0] == "11");
    REQUIRE(results011[1] == "");
    REQUIRE(results011[2] == "22");
    REQUIRE(results011[3] == "3");
    REQUIRE(results011[4] == "444");

	std::vector<memepp::string> results012;
	memepp::split(s01, ":", memepp::split_behavior_t::skip_empty_parts, std::back_inserter(results012));
	REQUIRE(results012.size() == 4);
    REQUIRE(results012[0] == "11");
    REQUIRE(results012[1] == "22");
    REQUIRE(results012[2] == "3");
    REQUIRE(results012[3] == "444");

	results012.clear();
	s01.split(":", memepp::split_behavior_t::skip_empty_parts, std::back_inserter(results012));
	REQUIRE(results012.size() == 4);
    REQUIRE(results012[0] == "11");
    REQUIRE(results012[1] == "22");
    REQUIRE(results012[2] == "3");
    REQUIRE(results012[3] == "444");

	std::list<memepp::string> results013;
	memepp::split(s01, "444", memepp::split_behavior_t::skip_empty_parts, std::back_inserter(results013));
	REQUIRE(results013.size() == 1);
	REQUIRE(*results013.begin() == "11::22:3:");
    

	std::deque<memepp::string> results014;
	memepp::split(s01, "11", memepp::split_behavior_t::keep_empty_parts, std::back_inserter(results014));
	REQUIRE(results014.size() == 2);
	REQUIRE(*results014.rbegin() == "::22:3:444");

	std::vector<memepp::string> results015;
	memepp::split(s01, "444", memepp::split_behavior_t::keep_empty_parts, std::back_inserter(results015));
	REQUIRE(results015.size() == 2);
	REQUIRE(results015.front() == "11::22:3:");
    REQUIRE(results015.back() == "");

	std::vector<memepp::string_view> results016;
	s01.split(":", memepp::split_behavior_t::keep_empty_parts, std::back_inserter(results016));
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
    s01.split(":", memepp::split_behavior_t::skip_empty_parts, std::back_inserter(results017));
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

	memepp::string s02 = 
		"Four-leaf clover is a rare variant of the genus Cheatgrass (which includes the genus Clover and Alfalfa) that also has more than five leaves and up to eighteen leaves. "
		"In the West, it is considered a sign of great good fortune to find four-leaf clover.";

	std::vector<memepp::string> results021;
	memepp::split(s02, "clover", memepp::split_behavior_t::keep_empty_parts, std::back_inserter(results021));
	REQUIRE(results021.size() == 3);
	REQUIRE(results021[1] == " is a rare variant of the genus Cheatgrass (which includes the genus Clover and Alfalfa) that also has more than five leaves and up to eighteen leaves. "
		"In the West, it is considered a sign of great good fortune to find four-leaf ");

	results021.clear(); 
	s02.split("clover", memepp::split_behavior_t::keep_empty_parts, std::back_inserter(results021));
	REQUIRE(results021.size() == 3);
    REQUIRE(results021[1] == " is a rare variant of the genus Cheatgrass (which includes the genus Clover and Alfalfa) that also has more than five leaves and up to eighteen leaves. "
        "In the West, it is considered a sign of great good fortune to find four-leaf ");

    std::vector<memepp::string> results022;
    memepp::split(s02, "clover", memepp::split_behavior_t::skip_empty_parts, std::back_inserter(results022));
    REQUIRE(results022.size() == 3);
    REQUIRE(results022[1] == " is a rare variant of the genus Cheatgrass (which includes the genus Clover and Alfalfa) that also has more than five leaves and up to eighteen leaves. "
        "In the West, it is considered a sign of great good fortune to find four-leaf ");
    
	memepp::string_view sv01 = s01;
	
    std::vector<memepp::string_view> results023;
    sv01.split(":", memepp::split_behavior_t::keep_empty_parts, std::back_inserter(results023));
}

#include <memepp/variable_buffer.hpp>

TEST_CASE("memepp::string - 08", "variable_buffer constructions")
{
	memepp::variable_buffer b01;
	REQUIRE(sizeof(memepp::variable_buffer) == MEME_STRING__OBJECT_SIZE);
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::small);
	REQUIRE(b01.size() == 0);
	REQUIRE(b01.empty());
	REQUIRE(b01.capacity() == MEME_STRING__OBJECT_SIZE - 2);
    REQUIRE(b01.data() != nullptr);
    REQUIRE(b01.data()[0] == '\0');

#if INTPTR_MAX == INT64_MAX
	uint8_t buf01[] = { 
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 
		0x0C, 0x0D, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 };
#else
    uint8_t buf01[] = {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A };
#endif
	memepp::variable_buffer b02(buf01, sizeof(buf01));
	REQUIRE(b02.storage_type() == memepp::buffer_storage_t::small);
	REQUIRE(b02.size() == MEME_STRING__OBJECT_SIZE - 2);

	uint8_t buf02[] = {
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0F, 0x10, 
		0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1F, 0x20,
	};

	memepp::variable_buffer b03(buf02, sizeof(buf02));
	REQUIRE(b03.storage_type() == memepp::buffer_storage_t::medium);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b03.native_handle())) == 1);

	b02.push_back(0x18);
	memepp::variable_buffer b04(b02);
	REQUIRE(b04.storage_type() == memepp::buffer_storage_t::medium);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b04.native_handle())) == 1);

#if INTPTR_MAX == INT64_MAX
	uint8_t buf03[] = { 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
#else
	uint8_t buf03[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
#endif
	memepp::variable_buffer b05(MEME_STRING__OBJECT_SIZE - 2, 0);
	REQUIRE(b05.storage_type() == memepp::buffer_storage_t::small);
	REQUIRE(b05.size() == MEME_STRING__OBJECT_SIZE - 2);
	REQUIRE(memcmp(b05.data(), buf03, sizeof(buf03)) == 0);


	uint8_t buf04[] = {
		0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 
		0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
		0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
		0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06
	};
	memepp::variable_buffer b06(32, 6);
	REQUIRE(b06.storage_type() == memepp::buffer_storage_t::medium);
	REQUIRE(b06.size() == sizeof(buf04));
	REQUIRE(memcmp(b06.data(), buf04, sizeof(buf04)) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b06.native_handle())) == 1);

}

TEST_CASE("memepp::string - 09", "variable_buffer resize")
{
	memepp::variable_buffer b01;

	b01.resize(0);
	REQUIRE(sizeof(memepp::variable_buffer) == MEME_STRING__OBJECT_SIZE);
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::small);
	REQUIRE(b01.size() == 0);
	REQUIRE(b01.empty());
	REQUIRE(b01.capacity() == MEME_STRING__OBJECT_SIZE - 2);
	REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b01.native_handle())) == 1);

	b01.resize(MEME_STRING__OBJECT_SIZE - 2, 1);
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::small);
	REQUIRE(b01.size() == MEME_STRING__OBJECT_SIZE - 2);

	uint8_t buf01[] = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 };
	REQUIRE(0 == memcmp(b01.data(), buf01, b01.size()));
	REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b01.native_handle())) == 1);


	b01.resize(10);
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::small);
	REQUIRE(b01.size() == 10);

	uint8_t buf02[] = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 };
	REQUIRE(0 == memcmp(b01.data(), buf02, b01.size()));
	REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b01.native_handle())) == 1);


	b01.resize(20, 2);
#if INTPTR_MAX == INT64_MAX
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::small);
#else
    REQUIRE(b01.storage_type() == memepp::buffer_storage_t::medium);
#endif
	REQUIRE(b01.size() == 20);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b01.native_handle())) == 1);

	uint8_t buf03[] = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 };
	REQUIRE(0 == memcmp(b01.data(), buf03, b01.size()));
	REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b01.native_handle())) == 1);


	b01.resize(10, 2);
#if INTPTR_MAX == INT64_MAX
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::small);
#else
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::medium);
#endif
	REQUIRE(b01.size() == 10);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b01.native_handle())) == 1);

	uint8_t buf04[] = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 };
	REQUIRE(0 == memcmp(b01.data(), buf04, b01.size()));
	REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b01.native_handle())) == 1);


	b01.resize(23, 3);
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::medium);
	REQUIRE(b01.size() == 23);
	REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b01.native_handle())) == 1);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b01.native_handle())) == 1);

	uint8_t buf05[] = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 };
	REQUIRE(0 == memcmp(b01.data(), buf05, b01.size()));


	b01.resize(42, 4);
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::medium);
	REQUIRE(b01.size() == 42);
	REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b01.native_handle())) == 1);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b01.native_handle())) == 1);

	uint8_t buf06[] = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04 };
	REQUIRE(0 == memcmp(b01.data(), buf06, b01.size()));


	b01.resize(128, 5);
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::medium);
	REQUIRE(b01.size() == 128);
	REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b01.native_handle())) == 1);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b01.native_handle())) == 1);

	uint8_t buf07[] = { 
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
		0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
		0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
		0x05, 0x05
	};
	REQUIRE(0 == memcmp(b01.data(), buf07, b01.size()));


	b01.resize(1, 6);
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::medium);
	REQUIRE(b01.size() == 1);
	uint8_t buf08[] = { 0x01 };
	REQUIRE(0 == memcmp(b01.data(), buf08, b01.size()));
	REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b01.native_handle())) == 1);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b01.native_handle())) == 1);


	b01.resize(256, 7);
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::medium);
	REQUIRE(b01.size() == 256);
	uint8_t buf09[] = { 
		0x01, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07
	};
	REQUIRE(0 == memcmp(b01.data(), buf09, b01.size()));
	REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b01.native_handle())) == 1);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b01.native_handle())) == 1);


	b01.resize(65536, 8);
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::medium);
	REQUIRE(b01.size() == 65536);
	REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b01.native_handle())) == 1);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b01.native_handle())) == 1);
}

TEST_CASE("memepp::string - 10", "variable_buffer append")
{
	memepp::variable_buffer b01;
	b01.resize(10, 1);

	uint8_t buf01[] = { 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 };
	b01.append(buf01, sizeof(buf01));
#if INTPTR_MAX == INT64_MAX
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::small);
#else
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::medium);
#endif
	REQUIRE(b01.size() == 22);	
	uint8_t buf02[] = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 };
	REQUIRE(0 == memcmp(b01.data(), buf02, b01.size()));
	REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b01.native_handle())) == 1);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b01.native_handle())) == 1);


	b01.resize(0);
	uint8_t buf03[] = { 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 };
	b01.append(buf03, sizeof(buf03));
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::medium);
	REQUIRE(b01.size() == 23);
	REQUIRE(0 == memcmp(b01.data(), buf03, b01.size()));
	REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b01.native_handle())) == 1);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b01.native_handle())) == 1);

	b01.append(b01);
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::medium);
	REQUIRE(b01.size() == 46);
	uint8_t buf04[] = { 
		0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
		0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03
	};
	REQUIRE(0 == memcmp(b01.data(), buf04, b01.size()));
	REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b01.native_handle())) == 1);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b01.native_handle())) == 1);


	b01.push_back(4).push_back(5);
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::medium);
	REQUIRE(b01.size() == 48);
	uint8_t buf05[] = {
		0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
		0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
		0x04, 0x05
	};
	REQUIRE(0 == memcmp(b01.data(), buf05, b01.size()));
	REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b01.native_handle())) == 1);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b01.native_handle())) == 1);


	MemeInteger_t vb08_totalSize = 0;
	memepp::variable_buffer vb08; vb08.reserve(512);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(vb08.native_handle())) == 1);
	for (int index = 0, total = 256; index < total; ++index)
	{
		std::vector<uint8_t> buf((size_t(rand()) % 128) + 1, 0xFF);

		vb08.append(buf.data(), buf.size());
		vb08_totalSize += buf.size();
		REQUIRE(vb08.size() == vb08_totalSize);
		REQUIRE(MemeString_checkHeadTailMemory(
			(MemeString_Const_t)memepp::to_pointer(vb08.native_handle())) == 1);
	}
	for (int index = 0, total = (size_t(rand()) % 128) + 1; index < total; ++index)
	{
		vb08.push_back(0xFF);
		vb08_totalSize += 1;
		REQUIRE(vb08.size() == vb08_totalSize);
		REQUIRE(MemeString_checkHeadTailMemory(
			(MemeString_Const_t)memepp::to_pointer(vb08.native_handle())) == 1);
	}
}

#include <memepp/buffer.hpp>

TEST_CASE("memepp::string - 12", "buffer to_string")
{
	memepp::buffer b00;
	REQUIRE(b00.storage_type() == memepp::buffer_storage_t::small);

#if INTPTR_MAX == INT64_MAX
	uint8_t buf01[] = { 
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11,
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0x00
	};
#else
	uint8_t buf01[] = {
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x00, 0x00
	};
#endif

	memepp::buffer b01 { buf01, sizeof(buf01) };
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::large);
	REQUIRE(b01.size() == sizeof(buf01));
	REQUIRE(memcmp(b01.data(), buf01, sizeof(buf01)) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b01.native_handle())) == 1);

	auto s01 = b01.to_string(0);
	REQUIRE(s01.storage_type() == memepp::string_storage_t::small);
	REQUIRE(s01.size() == sizeof(buf01) - 2);
	REQUIRE(memcmp(s01.data(), buf01, sizeof(buf01) - 2) == 0);

	uint8_t buf02[] = {
		0x01, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07
	};

	memepp::buffer b02 { buf02, sizeof(buf02) };
	REQUIRE(b02.storage_type() == memepp::buffer_storage_t::large);
	REQUIRE(b02.size() == sizeof(buf02));
	REQUIRE(memcmp(b02.data(), buf02, sizeof(buf02)) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b02.native_handle())) == 1);

	auto s021 = b02.to_string(-1);
	REQUIRE(s021.storage_type() == memepp::string_storage_t::large);
	REQUIRE(s021.size() == sizeof(buf02));
	REQUIRE(memcmp(s021.data(), buf02, sizeof(buf02)) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(s021.native_handle())) == 1);

	auto s022 = b02.to_string(16);
	REQUIRE(s022.storage_type() == memepp::string_storage_t::large);
	REQUIRE(s022.size() == sizeof(buf02) - 16);
	REQUIRE(memcmp(s022.data(), buf02 + 16, sizeof(buf02) - 16) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(s022.native_handle())) == 1);

	uint8_t buf03[] = {
		0x01, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	memepp::buffer b03{ buf03, sizeof(buf03) };
	REQUIRE(b03.storage_type() == memepp::buffer_storage_t::large);
	REQUIRE(b03.size() == sizeof(buf03));
	REQUIRE(memcmp(b03.data(), buf03, sizeof(buf03)) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b03.native_handle())) == 1);

	auto s03 = b03.to_string(24);
	REQUIRE(s03.storage_type() == memepp::string_storage_t::large);
	REQUIRE(s03.size() == sizeof(buf03) - 32);
	REQUIRE(memcmp(s03.data(), buf03 + 24, sizeof(buf03) - 32) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b03.native_handle())) == 1);
}

TEST_CASE("memepp::string - 13", "en lower an en upper")
{
	memepp::string s01 = "aHbjhjhhkKhhkhAAA";
	auto l01 = s01.to_en_lower();
#if INTPTR_MAX == INT64_MAX
	REQUIRE(l01.storage_type() == memepp::string_storage_t::small);
#else
	REQUIRE(l01.storage_type() == memepp::string_storage_t::medium);
#endif
	REQUIRE(l01 == "ahbjhjhhkkhhkhaaa");
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(l01.native_handle())) == 1);

	auto u01 = s01.to_en_upper();
#if INTPTR_MAX == INT64_MAX
	REQUIRE(u01.storage_type() == memepp::string_storage_t::small);
#else
	REQUIRE(u01.storage_type() == memepp::string_storage_t::medium);
#endif
	REQUIRE(u01 == "AHBJHJHHKKHHKHAAA");
    
	memepp::string s02 = "SDJODSjassnkldslkdsSFDHKgdhkgdnkDSKJDDASHUhisffskjfbkGDASGK好";
	auto l02 = s02.to_en_lower();
#if INTPTR_MAX == INT64_MAX
	REQUIRE(l02.storage_type() == memepp::string_storage_t::medium);
#else
	REQUIRE(l02.storage_type() == memepp::string_storage_t::large);
#endif
	REQUIRE(l02 == "sdjodsjassnkldslkdssfdhkgdhkgdnkdskjddashuhisffskjfbkgdasgk好");

	auto u02 = s02.to_en_upper(); 
#if INTPTR_MAX == INT64_MAX
	REQUIRE(u02.storage_type() == memepp::string_storage_t::medium);
#else
	REQUIRE(u02.storage_type() == memepp::string_storage_t::large);
#endif
	REQUIRE(u02 == "SDJODSJASSNKLDSLKDSSFDHKGDHKGDNKDSKJDDASHUHISFFSKJFBKGDASGK好");
}

TEST_CASE("memepp::string - 14", "starts_with")
{
    memepp::string s01 = "aHbjhjhhkKhhkhAAA";
    REQUIRE(s01.starts_with("aHbjhjhhkKhhkhAAA"));
    REQUIRE(s01.starts_with("aHbjhjhhkKhhkhAA"));
    REQUIRE(s01.starts_with("aHbjhjhhkKhhkhA"));
    REQUIRE(s01.starts_with("aHbjhjhhkKhhkh"));
    REQUIRE(s01.starts_with("aHbjhjhhkKhhk"));
    REQUIRE(s01.starts_with("aHbjhjhhkKhh"));
    REQUIRE(s01.starts_with("aHbjhjhhkKh"));
    REQUIRE(s01.starts_with("aHbjhjhhkK"));
    REQUIRE(s01.starts_with("aHbjhjhhk"));
    REQUIRE(s01.starts_with("aHbjhjhh"));
    REQUIRE(s01.starts_with("aHbjhjh"));
    REQUIRE(s01.starts_with("aHbjhj"));
    REQUIRE(s01.starts_with("aHbjh"));
    REQUIRE(s01.starts_with("aHbj"));
    REQUIRE(s01.starts_with("aHb"));
    REQUIRE(s01.starts_with("aH"));
    REQUIRE(s01.starts_with("a"));
    REQUIRE(s01.starts_with(""));
    REQUIRE_FALSE(s01.starts_with("aHbjhjhhkKhhkhAAAA"));
    REQUIRE_FALSE(s01.starts_with("aHbjhjhhkKhhkhAAAB"));
    REQUIRE_FALSE(s01.starts_with("aHbjhjhhkKhhkhAB"));
    REQUIRE_FALSE(s01.starts_with("aHbjhjhhkKhhkhB"));
    REQUIRE_FALSE(s01.starts_with("aHbjhjhhkKhhkhh"));
    REQUIRE_FALSE(s01.starts_with("aHbjhjhhkKhhkhk"));
    REQUIRE_FALSE(s01.starts_with("aHbjhjhhkKhhhk"));
    
}

TEST_CASE("memepp::string - 15", "ends_with")
{
    memepp::string s01 = "aHbjhjhhkKhhkhAAA";
    
    REQUIRE(s01.ends_with("aHbjhjhhkKhhkhAAA"));
    REQUIRE(s01.ends_with("HbjhjhhkKhhkhAAA"));
    REQUIRE(s01.ends_with("bjhjhhkKhhkhAAA"));
    REQUIRE(s01.ends_with("jhjhhkKhhkhAAA"));
    REQUIRE(s01.ends_with("hjhhkKhhkhAAA"));
    REQUIRE(s01.ends_with("jhhkKhhkhAAA"));
    REQUIRE(s01.ends_with("hhkKhhkhAAA"));
    REQUIRE(s01.ends_with("hkKhhkhAAA"));
    REQUIRE(s01.ends_with("kKhhkhAAA"));
    REQUIRE(s01.ends_with("KhhkhAAA"));
    REQUIRE(s01.ends_with("hhkhAAA"));
    REQUIRE(s01.ends_with("hkhAAA"));
    REQUIRE(s01.ends_with("khAAA"));
    REQUIRE(s01.ends_with("hAAA"));
    REQUIRE(s01.ends_with("AAA"));
    REQUIRE(s01.ends_with("AA"));
    REQUIRE(s01.ends_with("A"));
    REQUIRE(s01.ends_with(""));
    REQUIRE_FALSE(s01.ends_with("aHbjhjhhkKhhkhAAAA"));
    REQUIRE_FALSE(s01.ends_with("aHbjhjhhkKhhkhAAAB"));
    REQUIRE_FALSE(s01.ends_with("aHbjhjhhkKhhkhAB"));
    REQUIRE_FALSE(s01.ends_with("aHbjhjhhkKhhkhB"));
    REQUIRE_FALSE(s01.ends_with("aHbjhjhhkKhhkhh"));
    REQUIRE_FALSE(s01.ends_with("aHbjhjhhkKhhkhk"));
    REQUIRE_FALSE(s01.ends_with("aHbjhjhhkKhhhk"));

}

TEST_CASE("memepp::string - 16", "contains") 
{
    memepp::string s01 = "aHbjhjhhkKhhkhAAA";

    REQUIRE(s01.contains("aHbjhjhhkKhhkhAAA"));
    REQUIRE(s01.contains("HbjhjhhkKhhkhAAA"));
    REQUIRE(s01.contains("bjhjhhkKhhkhAAA"));
    REQUIRE(s01.contains("jhjhhkKhhkhAAA"));
    REQUIRE(s01.contains("hjhhkKhhkhAAA"));
    REQUIRE(s01.contains("jhhkKhhkhAAA"));
    REQUIRE(s01.contains("hhkKhhkhAAA"));
    REQUIRE(s01.contains("hkKhhkhAAA"));
    REQUIRE(s01.contains("kKhhkhAAA"));
    REQUIRE(s01.contains("KhhkhAAA"));
    REQUIRE(s01.contains("hhkhAAA"));
    REQUIRE(s01.contains("hkhAAA"));
    REQUIRE(s01.contains("khAAA"));
    REQUIRE(s01.contains("hAAA"));
    REQUIRE(s01.contains("AAA"));
    REQUIRE(s01.contains("AA"));
    REQUIRE(s01.contains("A"));
    REQUIRE_FALSE(s01.contains("aHbjhjhhkKhhkhAAAA"));
    REQUIRE_FALSE(s01.contains("aHbjhjhhkKhhkhAAAB"));
    REQUIRE_FALSE(s01.contains("aHbjhjhhkKhhkhAB"));
    REQUIRE_FALSE(s01.contains("aHbjhjhhkKhhkhB"));
    REQUIRE_FALSE(s01.contains("aHbjhjhhkKhhkhh"));
    REQUIRE_FALSE(s01.contains("aHbjhjhhkKhhkhk"));
    REQUIRE_FALSE(s01.contains("aHbjhjhhkKhhhk"));
    
}

TEST_CASE("memepp::string - 17", "trim_space")
{
    memepp::string s01 = "  aHbjhjhhkKhhkhAAA  ";
    REQUIRE(s01.trim_space() == "aHbjhjhhkKhhkhAAA");

    memepp::string s02 = "  aHbjhjhhkKhhkhAAA";
    REQUIRE(s02.trim_space() == "aHbjhjhhkKhhkhAAA");
    
    memepp::string s03 = "aHbjhjhhkKhhkhAAA  ";
    REQUIRE(s03.trim_space() == "aHbjhjhhkKhhkhAAA");
    
    memepp::string s04 = "aHbjhjhhkKhhkhAAA";
    REQUIRE(s04.trim_space() == "aHbjhjhhkKhhkhAAA");
    
    memepp::string s05 = "  ";
    REQUIRE(s05.trim_space() == "");
    
    memepp::string s06 = "";
    REQUIRE(s06.trim_space() == "");
    
    memepp::string s07 = " ";
    REQUIRE(s07.trim_space() == "");

	memepp::string s08 = "\t\t  aaaaafsnjhd2312312312fdsssss \r\r ";
    auto s08_trimmed = s08.trim_space();
    REQUIRE(s08_trimmed == "aaaaafsnjhd2312312312fdsssss");
    REQUIRE(s08_trimmed.storage_type() == memepp::string_storage_t::medium);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(s08_trimmed.native_handle())) == 1);
    
	memepp::string_view sv08 = s08;
	auto sv08_trimmed = sv08.trim_space();
    REQUIRE(sv08_trimmed == "aaaaafsnjhd2312312312fdsssss");
    REQUIRE(sv08_trimmed.storage_type() == 
		static_cast<memepp::string_storage_t>(MemeString_UnsafeStorageType_view));
    
    //auto sv08_trimmed = mm_view(s08_trimmed);
    //REQUIRE(sv08_trimmed == "aaaaafsnjhd2312312312fdsssss");
    //REQUIRE(sv08_trimmed.storage_type() == memepp::string_storage_t::medium);
}

TEST_CASE("memepp::string - 18", "trim_left_space")
{
    memepp::string s01 = "  aHbjhjhhkKhhkhAAA  ";
    REQUIRE(s01.trim_left_space() == "aHbjhjhhkKhhkhAAA  ");

    memepp::string s02 = "  aHbjhjhhkKhhkhAAA";
    REQUIRE(s02.trim_left_space() == "aHbjhjhhkKhhkhAAA");

    memepp::string s03 = "aHbjhjhhkKhhkhAAA  ";
    REQUIRE(s03.trim_left_space() == "aHbjhjhhkKhhkhAAA  ");

    memepp::string s04 = "aHbjhjhhkKhhkhAAA";
    REQUIRE(s04.trim_left_space() == "aHbjhjhhkKhhkhAAA");

    memepp::string s05 = "  ";
    REQUIRE(s05.trim_left_space() == "");

    memepp::string s06 = "";
    REQUIRE(s06.trim_left_space() == "");

    memepp::string s07 = " ";
    REQUIRE(s07.trim_left_space() == "");

}

TEST_CASE("memepp::string - 19", "trim_right_space")
{
    memepp::string s01 = "  aHbjhjhhkKhhkhAAA  ";
    REQUIRE(s01.trim_right_space() == "  aHbjhjhhkKhhkhAAA");

    memepp::string s02 = "  aHbjhjhhkKhhkhAAA";
    REQUIRE(s02.trim_right_space() == "  aHbjhjhhkKhhkhAAA");

    memepp::string s03 = "aHbjhjhhkKhhkhAAA  ";
    REQUIRE(s03.trim_right_space() == "aHbjhjhhkKhhkhAAA");

    memepp::string s04 = "aHbjhjhhkKhhkhAAA";
    REQUIRE(s04.trim_right_space() == "aHbjhjhhkKhhkhAAA");

    memepp::string s05 = "  ";
    REQUIRE(s05.trim_right_space() == "");

    memepp::string s06 = "";
    REQUIRE(s06.trim_right_space() == "");

    memepp::string s07 = " ";
    REQUIRE(s07.trim_right_space() == "");
}

TEST_CASE("memepp::string - 20", "substr")
{
    memepp::string s01 = "aHbjhjhhkKhhkhAAA";
    REQUIRE(s01.substr(0, 0)  == "");
    REQUIRE(s01.substr(0, 1)  == "a");
    REQUIRE(s01.substr(0, 2)  == "aH");
    REQUIRE(s01.substr(0, 3)  == "aHb");
    REQUIRE(s01.substr(0, 4)  == "aHbj");
    REQUIRE(s01.substr(0, 5)  == "aHbjh");
    REQUIRE(s01.substr(0, 6)  == "aHbjhj");
    REQUIRE(s01.substr(0, 7)  == "aHbjhjh");
    REQUIRE(s01.substr(0, 8)  == "aHbjhjhh");
    REQUIRE(s01.substr(0, 9)  == "aHbjhjhhk");
    REQUIRE(s01.substr(0, 10) == "aHbjhjhhkK");
    REQUIRE(s01.substr(0, 11) == "aHbjhjhhkKh");
    REQUIRE(s01.substr(0, 12) == "aHbjhjhhkKhh");
    REQUIRE(s01.substr(0, 13) == "aHbjhjhhkKhhk");
    REQUIRE(s01.substr(0, 14) == "aHbjhjhhkKhhkh");
    REQUIRE(s01.substr(0, 15) == "aHbjhjhhkKhhkhA");
    REQUIRE(s01.substr(0, 16) == "aHbjhjhhkKhhkhAA");
    REQUIRE(s01.substr(0, 17) == "aHbjhjhhkKhhkhAAA");
    REQUIRE(s01.substr(0, 18) == "aHbjhjhhkKhhkhAAA");
    REQUIRE(s01.substr(0) == "aHbjhjhhkKhhkhAAA");

	REQUIRE(s01.substr(1, 0) == "");
    REQUIRE(s01.substr(1, 1) == "H");
    REQUIRE(s01.substr(1, 2) == "Hb");
    REQUIRE(s01.substr(1, 3) == "Hbj");
    REQUIRE(s01.substr(1, 4) == "Hbjh");
    REQUIRE(s01.substr(1, 5) == "Hbjhj");
    REQUIRE(s01.substr(1, 6) == "Hbjhjh");
    REQUIRE(s01.substr(1, 7) == "Hbjhjhh");
    REQUIRE(s01.substr(1, 8) == "Hbjhjhhk");
    REQUIRE(s01.substr(1, 9) == "HbjhjhhkK");
    REQUIRE(s01.substr(1, 10) == "HbjhjhhkKh");
    REQUIRE(s01.substr(1, 11) == "HbjhjhhkKhh");
    REQUIRE(s01.substr(1, 12) == "HbjhjhhkKhhk");
    REQUIRE(s01.substr(1, 13) == "HbjhjhhkKhhkh");
    REQUIRE(s01.substr(1, 14) == "HbjhjhhkKhhkhA");
    REQUIRE(s01.substr(1, 15) == "HbjhjhhkKhhkhAA");
    REQUIRE(s01.substr(1, 16) == "HbjhjhhkKhhkhAAA");
    REQUIRE(s01.substr(1, 17) == "HbjhjhhkKhhkhAAA");
    REQUIRE(s01.substr(1) == "HbjhjhhkKhhkhAAA");
    
    REQUIRE(s01.substr(2, 0) == "");
    REQUIRE(s01.substr(2, 1) == "b");
    REQUIRE(s01.substr(2, 2) == "bj");
    REQUIRE(s01.substr(2, 3) == "bjh");
    REQUIRE(s01.substr(2, 4) == "bjhj");
    REQUIRE(s01.substr(2, 5) == "bjhjh");
    REQUIRE(s01.substr(2, 6) == "bjhjhh");
    REQUIRE(s01.substr(2, 7) == "bjhjhhk");
    REQUIRE(s01.substr(2, 8) == "bjhjhhkK");
    REQUIRE(s01.substr(2, 9) == "bjhjhhkKh");
    REQUIRE(s01.substr(2, 10) == "bjhjhhkKhh");
    REQUIRE(s01.substr(2, 11) == "bjhjhhkKhhk");
    REQUIRE(s01.substr(2, 12) == "bjhjhhkKhhkh");
    REQUIRE(s01.substr(2, 13) == "bjhjhhkKhhkhA");
    REQUIRE(s01.substr(2, 14) == "bjhjhhkKhhkhAA");
    REQUIRE(s01.substr(2, 15) == "bjhjhhkKhhkhAAA");
    REQUIRE(s01.substr(2, 16) == "bjhjhhkKhhkhAAA");
    REQUIRE(s01.substr(2) == "bjhjhhkKhhkhAAA");
    
    REQUIRE(s01.substr(3, 0) == "");
    REQUIRE(s01.substr(3, 1) == "j");
    REQUIRE(s01.substr(3, 2) == "jh");
    REQUIRE(s01.substr(3, 3) == "jhj");
    REQUIRE(s01.substr(3, 4) == "jhjh");
    REQUIRE(s01.substr(3, 5) == "jhjhh");
    REQUIRE(s01.substr(3, 6) == "jhjhhk");
    REQUIRE(s01.substr(3, 7) == "jhjhhkK");
    REQUIRE(s01.substr(3, 8) == "jhjhhkKh");
    REQUIRE(s01.substr(3, 9) == "jhjhhkKhh");
    REQUIRE(s01.substr(3, 10) == "jhjhhkKhhk");
    REQUIRE(s01.substr(3, 11) == "jhjhhkKhhkh");
    REQUIRE(s01.substr(3, 12) == "jhjhhkKhhkhA");
    REQUIRE(s01.substr(3, 13) == "jhjhhkKhhkhAA");
    REQUIRE(s01.substr(3, 14) == "jhjhhkKhhkhAAA");
    REQUIRE(s01.substr(3, 15) == "jhjhhkKhhkhAAA");
    REQUIRE(s01.substr(3) == "jhjhhkKhhkhAAA");

	REQUIRE(s01.substr(16, 0) == "");
    REQUIRE(s01.substr(16, 1) == "A");
    REQUIRE(s01.substr(16) == "A");

    REQUIRE(s01.substr(17, 0) == "");
    REQUIRE(s01.substr(17) == "");

	REQUIRE(s01.substr(18, 0) == "");
	REQUIRE(s01.substr(18) == "");
}

TEST_CASE("memepp::string - 21", "operator=")
{
	auto sz01 = "C++ (pronounced \"C plus plus\") is a general-purpose programming language created by Danish computer scientist Bjarne Stroustrup as an extension of the C programming language, "
		"or \"C with Classes\". The language has expanded significantly over time, "
		"and modern C++ now has object-oriented, generic, "
		"and functional features in addition to facilities for low-level memory manipulation. "
		"It is almost always implemented as a compiled language, and many vendors provide C++ compilers, "
		"including the Free Software Foundation, LLVM, Microsoft, Intel, Embarcadero, Oracle, and IBM, "
		"so it is available on many platforms.";
	memepp::string s01 = sz01;
	memepp::string s02;
	s02 = s01;
	REQUIRE(s02 == sz01);
    REQUIRE(s02.storage_type() == memepp::string_storage_t::large);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(s02.native_handle())) == 1);

	std::string s03 = sz01;
	s02 = mm_from(s03);
	REQUIRE(s02 == sz01);
	REQUIRE(s02.storage_type() == memepp::string_storage_t::large);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(s02.native_handle())) == 1);

	auto sz11 = "01234567890123456789012345678901234567890123456789";
	std::string s11 = sz11;
	memepp::string s12;
	s12 = mm_from(s11);
	REQUIRE(s12 == sz11);
#if INTPTR_MAX == INT64_MAX
	REQUIRE(s12.storage_type() == memepp::string_storage_t::medium);
#else
	REQUIRE(s12.storage_type() == memepp::string_storage_t::large);
#endif
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(s12.native_handle())) == 1);
}

TEST_CASE("memepp::string - 22", "rfind")
{
    char sz01[] = "C++ (pronounced \"C plus plus\") is a general-purpose programming language created by Danish computer scientist Bjarne Stroustrup as an extension of the C programming language, "
        "or \"C with Classes\". The language has expanded significantly over time, "
        "and modern C++ now has object-oriented, generic, "
        "and functional features in addition to facilities for low-level memory manipulation. "
        "It is almost always implemented as a compiled language, and many vendors provide C++ compilers, "
        "including the Free Software Foundation, LLVM, Microsoft, Intel, Embarcadero, Oracle, and IBM, "
        "so it is available on many platforms.";
	auto f011 = strstr(sz01 + 0, "C++");
	auto f012 = strstr(f011 + 3, "C++");
	auto f013 = strstr(f012 + 3, "C++");

    memepp::string s01 = sz01;
    REQUIRE(s01.rfind("C++") == f013 - sz01);
    REQUIRE(s01.rfind("C++ (pronounced \"C plus plus\")") == 0);
    REQUIRE(s01.rfind("on many platforms.") == sizeof(sz01) - sizeof("on many platforms."));
	
	REQUIRE(s01.last_index_of("on", -1, 0, 580, true ) == -1);
	REQUIRE(sz01 + s01.last_index_of("on", -1, 0, 580, false) == sz01 + 513);
	REQUIRE(sz01 + s01.last_index_of("on", -1, 513, -1, true) == sz01 + 590);

	REQUIRE(s01.last_index_of("C++", false) == f013 - sz01);
	REQUIRE(s01.last_index_of("C++", -1, 0, f013 - sz01, true) == f012 - sz01);
	REQUIRE(s01.last_index_of("C++ (pronounced \"C plus plus\")", false) == 0);
	REQUIRE(s01.last_index_of("on", false) == sizeof(sz01) - sizeof("on many platforms."));

	memepp::string s02_01 = "fd sdhufjkdas kfsd ks fd78fs77&** 67fd ifd gfjh*&&*f safjh^&IIR^&%R";
	REQUIRE(s02_01.last_index_of("f", true) == 51);
	REQUIRE(s02_01.last_index_of("R", true) == 66);
	REQUIRE(s02_01.last_index_of("fd", true) == 0);
}

TEST_CASE("memepp::string - 23", "variable_buffer insert")
{
	srand((unsigned int)time(NULL));

#if INTPTR_MAX == INT64_MAX
    uint8_t buf01[24] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18 };
#else
    uint8_t buf01[12] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C };
#endif
	memepp::variable_buffer vb01;
	vb01.insert(0, buf01, MEME_STRING__OBJECT_SIZE - 4);
    REQUIRE(vb01.storage_type() == memepp::buffer_storage_t::small);
    REQUIRE(vb01.size() == MEME_STRING__OBJECT_SIZE - 4);
    REQUIRE(memcmp(vb01.data(), buf01, MEME_STRING__OBJECT_SIZE - 4) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(vb01.native_handle())) == 1);
    
	vb01.insert(vb01.size(), buf01 + vb01.size(), 2);
    REQUIRE(vb01.storage_type() == memepp::buffer_storage_t::small);
    REQUIRE(vb01.size() == MEME_STRING__OBJECT_SIZE - 2);
    REQUIRE(memcmp(vb01.data(), buf01, MEME_STRING__OBJECT_SIZE - 2) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(vb01.native_handle())) == 1);

	memepp::variable_buffer vb02;
    vb02.insert(0, buf01 + 2, MEME_STRING__OBJECT_SIZE - 4);
    REQUIRE(vb02.storage_type() == memepp::buffer_storage_t::small);
    REQUIRE(vb02.size() == MEME_STRING__OBJECT_SIZE - 4);
    REQUIRE(memcmp(vb02.data(), buf01 + 2, MEME_STRING__OBJECT_SIZE - 4) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(vb02.native_handle())) == 1);

    vb02.insert(0, buf01, 2);
    REQUIRE(vb02.storage_type() == memepp::buffer_storage_t::small);
    REQUIRE(vb02.size() == MEME_STRING__OBJECT_SIZE - 2);
    REQUIRE(memcmp(vb02.data(), buf01, MEME_STRING__OBJECT_SIZE - 2) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(vb02.native_handle())) == 1);

	for (int index = 0; index < 10; ++index) {
		int randValue = size_t(rand()) % (MEME_STRING__OBJECT_SIZE - 4);
		memepp::variable_buffer vb00{ buf01, MEME_STRING__OBJECT_SIZE - 4 };
        vb00.insert(randValue, buf01 + randValue, 2);
        REQUIRE(vb00.storage_type() == memepp::buffer_storage_t::small);
        REQUIRE(vb00.size() == MEME_STRING__OBJECT_SIZE - 2);
        REQUIRE(memcmp(vb00.data(), buf01, randValue) == 0);
        REQUIRE(memcmp(vb00.data() + randValue, buf01 + randValue, 2) == 0);
        REQUIRE(memcmp(vb00.data() + randValue + 2, buf01 + randValue, MEME_STRING__OBJECT_SIZE - 4 - randValue) == 0);

		REQUIRE(MemeString_checkHeadTailMemory(
			(MemeString_Const_t)memepp::to_pointer(vb00.native_handle())) == 1);
    }

	auto limit = MemeStringOption_getStorageMediumLimit();
	std::vector<uint8_t> vec01(limit * 2, 0x07);
	std::vector<uint8_t> vec02(limit, 0x06);
	memepp::variable_buffer vb03(vec01.data(), vec01.size());
    vb03.insert(0, buf01, 2);
    REQUIRE(vb03.storage_type() == memepp::buffer_storage_t::medium);
    REQUIRE(vb03.size() == limit * 2 + 2);
    REQUIRE(memcmp(vb03.data(), buf01, 2) == 0);
    REQUIRE(memcmp(vb03.data() + 2, vec01.data(), vec01.size()) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(vb03.native_handle())) == 1);
    
	memepp::variable_buffer vb04(vec01.data(), vec01.size());
	vb04.insert(0, vec02.data(), vec02.size());
    REQUIRE(vb04.storage_type() == memepp::buffer_storage_t::medium);
    REQUIRE(vb04.size() == vec01.size() + vec02.size());
    REQUIRE(memcmp(vb04.data(), vec02.data(), vec02.size()) == 0);
    REQUIRE(memcmp(vb04.data() + vec02.size(), vec01.data(), vec01.size()) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(vb04.native_handle())) == 1);

	memepp::variable_buffer vb05(vec01.data(), vec01.size());
	vb05.insert(vb05.size(), vec02.data(), vec02.size());
    REQUIRE(vb05.storage_type() == memepp::buffer_storage_t::medium);
    REQUIRE(vb05.size() == vec01.size() + vec02.size());
    REQUIRE(memcmp(vb05.data(), vec01.data(), vec01.size()) == 0);
    REQUIRE(memcmp(vb05.data() + vec01.size(), vec02.data(), vec02.size()) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(vb05.native_handle())) == 1);

    for (int index = 0; index < 10; ++index) {
		int randValue = size_t(rand()) % vec01.size();
		memepp::variable_buffer vb06(vec01.data(), vec01.size());
		vb06.insert(randValue, vec02.data(), vec02.size());
		REQUIRE(vb06.storage_type() == memepp::buffer_storage_t::medium);
		REQUIRE(vb06.size() == vec01.size() + vec02.size());
		REQUIRE(memcmp(vb06.data(), vec01.data(), randValue) == 0);
		REQUIRE(memcmp(vb06.data() + randValue, vec02.data(), vec02.size()) == 0);
		REQUIRE(memcmp(vb06.data() + randValue + vec02.size(), vec01.data() + randValue, vec01.size() - randValue) == 0);
		REQUIRE(MemeString_checkHeadTailMemory(
			(MemeString_Const_t)memepp::to_pointer(vb06.native_handle())) == 1);
	}

	MemeInteger_t vb07_totalSize = 0;
	memepp::variable_buffer vb07; vb07.reserve(512);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(vb07.native_handle())) == 1);
	for (int index = 0, total = 256; index < total; ++index)
	{ 
		std::vector<uint8_t> buf((size_t(rand()) % 128) + 1, 0xFF);

		vb07.insert(rand() % (vb07.size() == 0 ? 1 : vb07.size()), buf.data(), buf.size());

		vb07_totalSize += buf.size();
		REQUIRE(vb07.size() == vb07_totalSize);
        REQUIRE(MemeString_checkHeadTailMemory(
            (MemeString_Const_t)memepp::to_pointer(vb07.native_handle())) == 1);
	}
    
}

#include <memepp/string_builder.hpp>
#include <memepp/operation/std/string.hpp>

TEST_CASE("memepp::string - 24", "string_builder append & prepend")
{
    memepp::string s1 = u8"1234567890";
	memepp::string s2 = u8"abcdefghij";
    std::string    s3 = u8"0987654321";
	memepp::string s4 = u8"klmnopqrst";
	memepp::string_view sv4 = s4;

	auto sb11 = memepp::string_builder{} + s1 + s2;
	REQUIRE(sb11 == u8"1234567890abcdefghij");

    auto sb12 = memepp::string_builder{} + s2 + s1;
    REQUIRE(sb12 == u8"abcdefghij1234567890");

    auto sb13 = memepp::string_builder{} + s1 + u8"1234567890";
    REQUIRE(sb13 == u8"12345678901234567890");
	
    auto sb14 = memepp::string_builder{} + u8"1234567890" + s1;
    REQUIRE(sb14 == u8"12345678901234567890");

    auto sb15 = memepp::string_builder{} + s1 + u8"1234567890" + s2;
    REQUIRE(sb15 == u8"12345678901234567890abcdefghij");

    auto sb16 = memepp::string_builder{} + s1 + u8"1234567890" + s2 + u8"abcdefghij";
    REQUIRE(sb16 == u8"12345678901234567890abcdefghijabcdefghij");

    auto sb17 = memepp::string_builder{} + s1 + u8"1234567890" + s2 + u8"abcdefghij" + s1;
    REQUIRE(sb17 == u8"12345678901234567890abcdefghijabcdefghij1234567890");
	
    auto sb80 = memepp::string_builder{} + u8"1234567890" + s2 + u8"abcdefghij" + s1 + u8"1234567890";
    REQUIRE(sb80 == u8"1234567890abcdefghijabcdefghij12345678901234567890");

	auto sb81 = sb80;
	REQUIRE(sb80 == u8"");
    REQUIRE(sb81 == u8"1234567890abcdefghijabcdefghij12345678901234567890");

	memepp::string s81 = sb81;
    REQUIRE(s81 == u8"1234567890abcdefghijabcdefghij12345678901234567890");
	
	auto s11 = memepp::string(memepp::string_builder{} + s1 + u8"1234567890" + s2 + u8"abcdefghij" + s1 + u8"1234567890");
    REQUIRE(s11 == u8"12345678901234567890abcdefghijabcdefghij12345678901234567890");

    auto sb21 = memepp::string_builder{} + s1 + s2 + s3;
    REQUIRE(sb21 == u8"1234567890abcdefghij0987654321");
	
    auto sb23 = memepp::string_builder{} + s1 + s2 + s3 + sv4;
    REQUIRE(sb23 == u8"1234567890abcdefghij0987654321klmnopqrst");
	
    auto sb24 = memepp::string_builder{} + s2 + s1 + s3 + sv4;
    REQUIRE(sb24 == u8"abcdefghij12345678900987654321klmnopqrst");

	auto sb25 = memepp::string_builder{} + s2 + sv4 + s1 + s3;
    REQUIRE(sb25 == u8"abcdefghijklmnopqrst12345678900987654321");
}

#include <memepp/buffer_view.hpp>

TEST_CASE("memepp::string - 25", "buffer_view constructor")
{
    memepp::buffer_view bv1;
    REQUIRE(bv1.size() == 0);
	
    memepp::buffer_view bv2(nullptr, 0);
    REQUIRE(bv2.size() == 0);
	
    memepp::buffer_view bv3(nullptr, 1);
    REQUIRE(bv3.size() == 0);
	
    memepp::buffer buf1;
    memepp::buffer_view bv4(buf1);
    REQUIRE(bv4.size() == 0);

	memepp::variable_buffer vb1;
	memepp::buffer_view bv5(vb1);
	REQUIRE(bv5.size() == 0);
	 
	memepp::string s1;
    memepp::buffer_view bv6(s1);
    REQUIRE(bv6.size() == 0);

    memepp::string_view sv1;
    memepp::buffer_view bv7(sv1);
    REQUIRE(bv7.size() == 0); 

    memepp::string s2 = u8"1234567890";
    memepp::buffer_view bv8(s2);
    REQUIRE(bv8.size() == s2.size());
    REQUIRE(bv8 == s2);
	
    memepp::string_view sv2 = s2;
    memepp::buffer_view bv9(sv2);
    REQUIRE(bv9.size() == sv2.size());
    REQUIRE(bv9 == sv2);
    REQUIRE(bv8 == bv9);
}

#include <memepp/convert/std/u16string.hpp>

TEST_CASE("memepp::string - 26", "utf16")
{
	auto c16_01_01 = u"\u7684\u4E00\u4E0D\u662F\u4E86\u4EBA\u5728\u6709\u6211\u4ED6\u8FD9\u4E3A\u4E4B\u6765\u5927\u4EE5\u4E2A\u4E2D\u4E0A\u4EEC";
	auto c8_01_01 = "\xE7\x9A\x84\xE4\xB8\x80\xE4\xB8\x8D\xE6\x98\xAF\xE4\xBA\x86\xE4\xBA\xBA\xE5\x9C\xA8\xE6\x9C\x89\xE6\x88\x91\xE4\xBB\x96\xE8\xBF\x99\xE4\xB8\xBA\xE4\xB9\x8B\xE6\x9D\xA5\xE5\xA4\xA7\xE4\xBB\xA5\xE4\xB8\xAA\xE4\xB8\xAD\xE4\xB8\x8A\xE4\xBB\xAC";
	memepp::string s01_01{ c8_01_01 };
    REQUIRE(s01_01.rune_size()    == 20);
    REQUIRE(s01_01.u16char_size() == 20);
	
    auto u16_01_01 = mm_to<std::u16string>(s01_01);
    REQUIRE(u16_01_01.size() == 20);
    REQUIRE(u16_01_01 == c16_01_01);

	auto s01_02 = mm_from(u16_01_01);
    REQUIRE(s01_02.size() == 60);
    REQUIRE(s01_02.rune_size()    == 20);
    REQUIRE(s01_02.u16char_size() == 20);
    REQUIRE(s01_02 == s01_01);
	REQUIRE(s01_02 == c8_01_01);
	

	auto c16_02_01 = u"\u7684\u4E00";
	auto bad8_02_01 = "\xE7\x9A\x84\xE4\xB8\x80\xE4\x00\x8D\xE6\x98\xAF\xE4\xBA\x86\xE4\xBA\xBA\xE5\x9C\xA8\xE6\x9C\x89\xE6\x88\x91\xE4\xBB\x96\xE8\xBF\x99\xE4\xB8\xBA\xE4\xB9\x8B\xE6\x9D\xA5\xE5\xA4\xA7\xE4\xBB\xA5\xE4\xB8\xAA\xE4\xB8\xAD\xE4\xB8\x8A\xE4\xBB\xAC";
    memepp::string s02_01{ bad8_02_01 };
    REQUIRE(s02_01.rune_size()    == 2);
    REQUIRE(s02_01.u16char_size() == 2);

    auto u16_02_01 = mm_to<std::u16string>(s02_01);
    REQUIRE(u16_02_01.size() == 2);
    REQUIRE(u16_02_01 == c16_02_01);

    auto s02_02 = mm_from(u16_02_01);
    REQUIRE(s02_02.size() == 6);
    REQUIRE(s02_02.rune_size() == 2);
    REQUIRE(s02_02.u16char_size() == 2);
	REQUIRE(s02_02 == memepp::string_view{ s02_01.data(), 6 });
	
	
	auto c16_03_01 = u"\u7684\u4E00";
	auto bad8_03_01 = "\xE7\x9A\x84\xE4\xB8\x80\xE4\x01\x8D\xE6\x98\xAF\xE4\xBA\x86\xE4\xBA\xBA\xE5\x9C\xA8\xE6\x9C\x89\xE6\x88\x91\xE4\xBB\x96\xE8\xBF\x99\xE4\xB8\xBA\xE4\xB9\x8B\xE6\x9D\xA5\xE5\xA4\xA7\xE4\xBB\xA5\xE4\xB8\xAA\xE4\xB8\xAD\xE4\xB8\x8A\xE4\xBB\xAC";
    memepp::string s03_01{ bad8_03_01 };
    REQUIRE(s03_01.rune_size()    == 20);
    REQUIRE(s03_01.u16char_size() == 2 );

    auto u16_03_01 = mm_to<std::u16string>(s03_01);
    REQUIRE(u16_03_01.size() == 2 );
    REQUIRE(u16_03_01 == c16_03_01);

    auto c8_04_01 = "\x7F \xC2\x80 \xDF\xBF \xE0\xA0\x80 \xEF\xBF\xBF \xF0\x90\x80\x80 "
		/*"\xF7\xBF\xBF\xBF "*/
		/*"\xF8\x88\x80\x80\x80 \xFB\xBF\xBF\xBF\xBF \xFC\x84\x80\x80\x80\x80 \xFD\xBF\xBF\xBF\xBF\xBF "*/;
    memepp::string s04_01{ c8_04_01 };
    REQUIRE(s04_01.rune_size()    == 12);
    REQUIRE(s04_01.u16char_size() == 13);

    auto u16_04_01 = mm_to<std::u16string>(s04_01);
    REQUIRE(u16_04_01.size() == 13);

    auto s04_02 = mm_from(u16_04_01);
    REQUIRE(s04_02.size()         == strlen(c8_04_01));
    REQUIRE(s04_02.rune_size()    == 12);
    REQUIRE(s04_02.u16char_size() == 13);
    REQUIRE(s04_02 == s04_01);
    REQUIRE(s04_02 == c8_04_01);

}

TEST_CASE("memepp::string - 27", "utf8")
{
	auto c8_01_01 = "\xE7\x9A\x84\xE4\xB8\x80\xE4\xB8\x8D\xE6\x98\xAF\xE4\xBA\x86\xE4\xBA\xBA\xE5\x9C\xA8\xE6\x9C\x89\xE6\x88\x91\xE4\xBB\x96\xE8\xBF\x99\xE4\xB8\xBA\xE4\xB9\x8B\xE6\x9D\xA5\xE5\xA4\xA7\xE4\xBB\xA5\xE4\xB8\xAA\xE4\xB8\xAD\xE4\xB8\x8A\xE4\xBB\xAC";
	memepp::string s01_01{ c8_01_01 };
	
	auto s01_02 = s01_01.mapping_convert([](memepp::rune&) 
	{ 
		return -1; 
	});
    REQUIRE(s01_02.size() == 0);

    auto s01_03 = s01_01.mapping_convert([](memepp::rune&)
    {
        return 0;
    });
    REQUIRE(s01_03.size() == s01_01.size());
	REQUIRE(s01_03 == s01_01);

	auto count = 0;
    for (auto it = s01_01.rune_begin(); it != s01_01.rune_end(); ++it)
    {
        ++count;
		
		auto i = it.to_index();
		REQUIRE(i.data() != nullptr);

		auto r = it.to_rune();
        REQUIRE(r.valid());
    }
    REQUIRE(count == 20);

	auto iter_size = std::distance(s01_01.rune_begin(), s01_01.rune_end());
    REQUIRE(s01_01.rune_size() == iter_size);

	auto it01_01 = s01_01.rune_begin();
	it01_01 = std::next(it01_01, 100);
    REQUIRE(it01_01 == s01_01.rune_end());

    it01_01 = std::prev(it01_01, 100);
    REQUIRE(it01_01 == s01_01.rune_begin());

    memepp::string s01_04{ c8_01_01, 11 };
	auto it01_04 = std::next(s01_04.rune_begin(), 10);
	count = static_cast<int>(std::distance(s01_04.rune_begin(), it01_04));
	REQUIRE(count == 3);
	
	count = 0;
    for (auto it = s01_04.rune_begin(); 
		it != s01_04.rune_end(); ++it)
    {
        ++count;
    }
    REQUIRE(count == 3);

	count = 0;
    std::for_each(s01_04.rune_begin(), s01_04.rune_end(), [&count](const memepp::rune& r)
    {
        REQUIRE(r.valid());
        ++count;
    });
    REQUIRE(count == 3);

	auto it01_05 = std::prev(it01_04, 10);
    REQUIRE(it01_05 == s01_04.rune_begin());

	it01_05 = s01_04.rune_end();
	it01_05 = std::prev(it01_05, 10);
    REQUIRE(it01_05 == s01_04.rune_begin());

	count = 0;
    for (auto it = std::prev(s01_04.rune_end(), 1); 
		it != s01_04.rune_begin(); --it)
    {
		if (!it.is_valid())
			continue;
		
		++count;
    }
    REQUIRE(count == 2);
	
}

TEST_CASE("memepp::string - 28", "variable_buffer remove")
{
    memepp::variable_buffer vb01_01;
	vb01_01.append(mm_view("1234567890", 10));
	
	vb01_01.remove(0, 0);
    REQUIRE(vb01_01.size() == 10);
    REQUIRE(vb01_01 == mm_view("1234567890", 10));
	vb01_01.remove(0, 1);
    REQUIRE(vb01_01.size() == 9);
    REQUIRE(vb01_01 == mm_view("234567890", 9));
	vb01_01.remove(0, 2);
    REQUIRE(vb01_01.size() == 7);
	REQUIRE(vb01_01 == mm_view("4567890", 7));
	vb01_01.remove(0, 3);
    REQUIRE(vb01_01.size() == 4);
    REQUIRE(vb01_01 == mm_view("7890", 4));
    vb01_01.remove(0, 4);
    REQUIRE(vb01_01.size() == 0);
    REQUIRE(vb01_01 == mm_view("", 0));
    vb01_01.remove(0, 5);
    REQUIRE(vb01_01.size() == 0);

    memepp::variable_buffer vb01_02;
    vb01_02.append(mm_view("1234567890", 10));

    vb01_02.remove(1, 0);
    REQUIRE(vb01_02.size() == 10);
    REQUIRE(vb01_02 == mm_view("1234567890", 10));
    vb01_02.remove(1, 1);
    REQUIRE(vb01_02.size() == 9);
    REQUIRE(vb01_02 == mm_view("134567890", 9));
    vb01_02.remove(1, 2);
    REQUIRE(vb01_02.size() == 7);
    REQUIRE(vb01_02 == mm_view("1567890", 7));
    vb01_02.remove(1, 3);
    REQUIRE(vb01_02.size() == 4);
    REQUIRE(vb01_02 == mm_view("1890", 4));
    vb01_02.remove(1, 4);
    REQUIRE(vb01_02.size() == 1);
    REQUIRE(vb01_02 == mm_view("1", 1));
    vb01_02.remove(1, 5);
    REQUIRE(vb01_02.size() == 1);
    REQUIRE(vb01_02 == mm_view("1", 1));

    memepp::variable_buffer vb01_03;
    vb01_03.append(mm_view("1234567890", 10));
	
    vb01_03.remove(-1, -1);
    REQUIRE(vb01_03.size() == 0);
    REQUIRE(vb01_03 == mm_view("", 0));

    memepp::variable_buffer vb01_04;
    vb01_04.append(mm_view("1234567890123456789012345678901234567890", 40));

    vb01_04.remove(0, 0);
    REQUIRE(vb01_04.size() == 40);
    REQUIRE(vb01_04 == mm_view("1234567890123456789012345678901234567890", 40));
    vb01_04.remove(0, 1);
    REQUIRE(vb01_04.size() == 39);
    REQUIRE(vb01_04 == mm_view("234567890123456789012345678901234567890", 39));
    vb01_04.remove(0, 2);
    REQUIRE(vb01_04.size() == 37);
    REQUIRE(vb01_04 == mm_view("4567890123456789012345678901234567890", 37));
    vb01_04.remove(0, 3);
    REQUIRE(vb01_04.size() == 34);
    REQUIRE(vb01_04 == mm_view("7890123456789012345678901234567890", 34));
	vb01_04.remove(0, 4);
    REQUIRE(vb01_04.size() == 30);
    REQUIRE(vb01_04 == mm_view("123456789012345678901234567890", 30));
    vb01_04.remove(0, 5);
    REQUIRE(vb01_04.size() == 25);
    REQUIRE(vb01_04 == mm_view("6789012345678901234567890", 25));
    vb01_04.remove(0, 6);
    REQUIRE(vb01_04.size() == 19);
    REQUIRE(vb01_04 == mm_view("2345678901234567890", 19));
    vb01_04.remove(0, 7);
    REQUIRE(vb01_04.size() == 12);
    REQUIRE(vb01_04 == mm_view("901234567890", 12));
    vb01_04.remove(0, 8);
    REQUIRE(vb01_04.size() == 4);
    REQUIRE(vb01_04 == mm_view("7890", 4));
    vb01_04.remove(0, 9);
    REQUIRE(vb01_04.size() == 0);

    memepp::variable_buffer vb01_05;
    vb01_05.append(mm_view("1234567890123456789012345678901234567890", 40));
	
    vb01_05.remove(-1, -1);
    REQUIRE(vb01_05.size() == 0);
    REQUIRE(vb01_05 == mm_view("", 0));

    memepp::variable_buffer vb01_06;
    vb01_06.append(mm_view("1234567890123456789012345678901234567890", 40));
	
    vb01_06.remove(3, 0);
    REQUIRE(vb01_06.size() == 40);
    REQUIRE(vb01_06 == mm_view("1234567890123456789012345678901234567890", 40));
    vb01_06.remove(3, 1);
    REQUIRE(vb01_06.size() == 39);
    REQUIRE(vb01_06 == mm_view("123567890123456789012345678901234567890", 39));
    vb01_06.remove(3, 2);
    REQUIRE(vb01_06.size() == 37);
    REQUIRE(vb01_06 == mm_view("1237890123456789012345678901234567890", 37));
    vb01_06.remove(3, 3);
    REQUIRE(vb01_06.size() == 34);
    REQUIRE(vb01_06 == mm_view("1230123456789012345678901234567890", 34));
    vb01_06.remove(3, 4);
    REQUIRE(vb01_06.size() == 30);
    REQUIRE(vb01_06 == mm_view("123456789012345678901234567890", 30));
    vb01_06.remove(3, 5);
    REQUIRE(vb01_06.size() == 25);
    REQUIRE(vb01_06 == mm_view("1239012345678901234567890", 25));
    vb01_06.remove(3, 6);
    REQUIRE(vb01_06.size() == 19);
    REQUIRE(vb01_06 == mm_view("1235678901234567890", 19));
    vb01_06.remove(3, 7);
    REQUIRE(vb01_06.size() == 12);
    REQUIRE(vb01_06 == mm_view("123234567890", 12));
    vb01_06.remove(3, 8);
    REQUIRE(vb01_06.size() == 4);
    REQUIRE(vb01_06 == mm_view("1230", 4));
    vb01_06.remove(3, 9);
    REQUIRE(vb01_06.size() == 3);
    REQUIRE(vb01_06 == mm_view("123", 3));
}

#include <memepp/convert/std/vector.hpp>

TEST_CASE("memepp::string - 29", "import from dll and export into dll")
{
	memepp::string_view sv01_01 { "123456789" };
    auto str01_01 = memepp::import_from_dll<memepp::string>(sv01_01.native_handle(), MMSTR__OBJ_SIZE);
    REQUIRE(sv01_01 == str01_01);
    REQUIRE(str01_01.storage_type() == memepp::string_storage_t::small);
    auto stk01_01 = memepp::export_into_dll<mmstrstk_t>(str01_01, MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&stk01_01]() { mmstrstk_uninit(&stk01_01, MMSTR__OBJ_SIZE); });
    REQUIRE(sv01_01 == MemeString_cStr(memepp::to_pointer(stk01_01)));
    REQUIRE(MemeString_storageType(memepp::to_pointer(stk01_01)) == MemeString_StorageType_small);
	auto stk01_02 = memepp::export_into_dll<mmstrstk_t>(std::move(str01_01), MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&stk01_02]() { mmstrstk_uninit(&stk01_02, MMSTR__OBJ_SIZE); });
    REQUIRE(sv01_01 == MemeString_cStr(memepp::to_pointer(stk01_02)));
    REQUIRE(MemeString_storageType(memepp::to_pointer(stk01_02)) == MemeString_StorageType_small);

	auto str02_01 = mm_from(std::string{ "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789" });
    REQUIRE(str02_01.storage_type() == memepp::string_storage_t::user);
	auto str02_02 = memepp::import_from_dll<memepp::string>(str02_01.native_handle(), MMSTR__OBJ_SIZE);
    REQUIRE(str02_01 == str02_02);
    REQUIRE(str02_02.storage_type() == memepp::string_storage_t::large);
    auto stk02_01 = memepp::export_into_dll<mmstrstk_t>(str02_02, MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&stk02_01]() { mmstrstk_uninit(&stk02_01, MMSTR__OBJ_SIZE); });
    REQUIRE(str02_01 == MemeString_cStr(memepp::to_pointer(stk02_01)));
    REQUIRE(MemeString_storageType(memepp::to_pointer(stk02_01)) == MemeString_StorageType_large);

	auto str03_01 = mm_from(std::vector<uint8_t>{
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 });
    REQUIRE(str03_01.storage_type() == memepp::string_storage_t::user);
    auto str03_02 = memepp::import_from_dll<memepp::string>(str03_01.native_handle(), MMSTR__OBJ_SIZE);
    REQUIRE(str03_01 == str03_02);
    REQUIRE(str03_02.storage_type() == memepp::string_storage_t::large);
    auto stk03_01 = memepp::export_into_dll<mmstrstk_t>(str03_02, MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&stk03_01]() { mmstrstk_uninit(&stk03_01, MMSTR__OBJ_SIZE); });
    REQUIRE(str03_01 == MemeString_cStr(memepp::to_pointer(stk03_01)));
    REQUIRE(MemeString_storageType(memepp::to_pointer(stk03_01)) == MemeString_StorageType_large);

	auto str04_01 = memepp::string{ "012345678901234567890123456789" };
    REQUIRE(str04_01.storage_type() == memepp::string_storage_t::medium);
    auto str04_02 = memepp::import_from_dll<memepp::string>(str04_01.native_handle(), MMSTR__OBJ_SIZE);
    REQUIRE(str04_01 == str04_02);
    REQUIRE(str04_02.storage_type() == memepp::string_storage_t::medium);
    auto stk04_01 = memepp::export_into_dll<mmstrstk_t>(str04_02, MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&stk04_01]() { mmstrstk_uninit(&stk04_01, MMSTR__OBJ_SIZE); });
    REQUIRE(str04_01 == MemeString_cStr(memepp::to_pointer(stk04_01)));
    REQUIRE(MemeString_storageType(memepp::to_pointer(stk04_01)) == MemeString_StorageType_medium);
}

TEST_CASE("memepp::string - 30", "operator+")
{
    memepp::string str01 = "   Hello";
    memepp::string str02 = "  World!";
    memepp::string str03 = str01 + str02;
	
    REQUIRE(str03 == "   Hello  World!");

	memepp::string str04 = "   fsihusffiofsdoiusfdoihsdfghuiofdsgu988989er89ewru9irwehuijhidhjkfdhjkfgdjkdfg";
    memepp::string str05 = "  World!";
    memepp::string str06 = str04 + str05;
	
    REQUIRE(str06 == "   fsihusffiofsdoiusfdoihsdfghuiofdsgu988989er89ewru9irwehuijhidhjkfdhjkfgdjkdfg  World!");
}

#include <memepp/variant.hpp>

TEST_CASE("memepp::variant - 01", "variant basic operations")
{
	mgec_t ec = 0;
	memepp::variant v01_01;
    REQUIRE(v01_01.is_null());
    REQUIRE(v01_01.is_type(memepp::meta::typid::null));
    REQUIRE(v01_01.type_id() == memepp::meta::typid::null);

	v01_01 = mmbyte_t{ 1 };
    REQUIRE(v01_01.is_null() == false);
    REQUIRE(v01_01.is_type(memepp::meta::typid::byte));
    REQUIRE(v01_01.type_id() == memepp::meta::typid::byte);
	auto value01_01 = v01_01.get_or<mmbyte_t>();
    REQUIRE(value01_01 == 1);

	auto v01_02 = v01_01;
    REQUIRE(v01_02.is_null() == false);
    REQUIRE(v01_02.is_type(memepp::meta::typid::byte));
    REQUIRE(v01_02.type_id() == memepp::meta::typid::byte);

	v01_01 = char{ '1' };
    REQUIRE(v01_01.is_null() == false);
    REQUIRE(v01_01.is_type(memepp::meta::typid::char_t));
    REQUIRE(v01_01.type_id() == memepp::meta::typid::char_t);
    auto value01_02 = v01_01.get_or<char>();
    REQUIRE(value01_02 == '1');

    v01_02 = v01_01;
    REQUIRE(v01_02.is_null() == false);
    REQUIRE(v01_02.is_type(memepp::meta::typid::char_t));
    REQUIRE(v01_02.type_id() == memepp::meta::typid::char_t);
    auto value01_03 = v01_02.get_or<char>();
    REQUIRE(value01_03 == '1');

    v01_01 = wchar_t{ 1 };
    REQUIRE(v01_01.is_null() == false);
	REQUIRE(v01_01.is_type(memepp::meta::typid::wchar));
    REQUIRE(v01_01.type_id() == memepp::meta::typid::wchar);
    auto value01_04 = v01_01.get_or<wchar_t>();
    REQUIRE(value01_04 == 1);

    v01_02 = v01_01;
    REQUIRE(v01_02.is_null() == false);
    REQUIRE(v01_02.is_type(memepp::meta::typid::wchar));
    REQUIRE(v01_02.type_id() == memepp::meta::typid::wchar);
    auto value01_05 = v01_02.get_or<wchar_t>();
    REQUIRE(value01_05 == 1);

    v01_01 = int64_t{ 1 };
    REQUIRE(v01_01.is_null() == false);
    REQUIRE(v01_01.is_type(memepp::meta::typid::int64));
    REQUIRE(v01_01.type_id() == memepp::meta::typid::int64);
    auto value01_06 = v01_01.get_or<int64_t>();
    REQUIRE(value01_06 == 1);

    v01_02 = v01_01;
    REQUIRE(v01_02.is_null() == false);
    REQUIRE(v01_02.is_type(memepp::meta::typid::int64));
    REQUIRE(v01_02.type_id() == memepp::meta::typid::int64);
    auto value01_07 = v01_02.get_or<int64_t>();
    REQUIRE(value01_07 == 1);

    v01_01 = double{ 1.0 };
    REQUIRE(v01_01.is_null() == false);
    REQUIRE(v01_01.is_type(memepp::meta::typid::double_t));
    REQUIRE(v01_01.type_id() == memepp::meta::typid::double_t);
    auto value01_08 = v01_01.get_or<double>();
    REQUIRE(value01_08 == 1.0);

    v01_02 = v01_01;
    REQUIRE(v01_02.is_null() == false);
    REQUIRE(v01_02.is_type(memepp::meta::typid::double_t));
    REQUIRE(v01_02.type_id() == memepp::meta::typid::double_t);
    auto value01_09 = v01_02.get_or<double>();
    REQUIRE(value01_09 == 1.0);

    v01_01 = uint64_t{ 1 };
    REQUIRE(v01_01.is_null() == false);
    REQUIRE(v01_01.is_type(memepp::meta::typid::uint64));
    REQUIRE(v01_01.type_id() == memepp::meta::typid::uint64);
    auto value01_10 = v01_01.get_or<uint64_t>();
    REQUIRE(value01_10 == 1);
	
    v01_02 = v01_01;
    REQUIRE(v01_02.is_null() == false);
	REQUIRE(v01_02.is_type(memepp::meta::typid::uint64));
    REQUIRE(v01_02.type_id() == memepp::meta::typid::uint64);
    auto value01_11 = v01_02.get_or<uint64_t>();
    REQUIRE(value01_11 == 1);
	
    v01_01 = memepp::rune{ '1' };
    REQUIRE(v01_01.is_null() == false);
    REQUIRE(v01_01.is_type(memepp::meta::typid::rune));
    REQUIRE(v01_01.type_id() == memepp::meta::typid::rune);
    auto value01_14 = v01_01.get_or<memepp::rune>();

    v01_02 = v01_01;
    REQUIRE(v01_02.is_null() == false);
    REQUIRE(v01_02.is_type(memepp::meta::typid::rune));
    REQUIRE(v01_02.type_id() == memepp::meta::typid::rune);
    auto value01_15 = v01_02.get_or<memepp::rune>();
    
    v01_01.reset();
    REQUIRE(v01_01.is_null());
    REQUIRE(v01_01.is_type(memepp::meta::typid::null));
    REQUIRE(v01_01.type_id() == memepp::meta::typid::null);
	
    v01_02 = v01_01;
    REQUIRE(v01_02.is_null());
	REQUIRE(v01_02.is_type(memepp::meta::typid::null));
	REQUIRE(v01_02.type_id() == memepp::meta::typid::null);

	v01_01 = memepp::string{ "1" };
	REQUIRE(v01_01.is_null() == false);
	REQUIRE(v01_01.is_type(memepp::meta::typid::string));
	REQUIRE(v01_01.type_id() == memepp::meta::typid::string);
	auto value01_12 = v01_01.get_or<memepp::string>();
	REQUIRE(value01_12 == "1");

	v01_02 = v01_01;
	REQUIRE(v01_02.is_null() == false);
	REQUIRE(v01_02.is_type(memepp::meta::typid::string));
	REQUIRE(v01_02.type_id() == memepp::meta::typid::string);
	auto value01_13 = v01_02.get_or<memepp::string>();
	REQUIRE(value01_13 == "1");

	v01_01 = memepp::string{ "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789" };
    REQUIRE(v01_01.is_null() == false);
    REQUIRE(v01_01.is_type(memepp::meta::typid::string));
    REQUIRE(v01_01.type_id() == memepp::meta::typid::string);
    auto value01_16 = v01_01.get_or<memepp::string>();
    REQUIRE(value01_16 == "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789");
	
    v01_02 = v01_01;
    REQUIRE(v01_02.is_null() == false);
	REQUIRE(v01_02.is_type(memepp::meta::typid::string));
    REQUIRE(v01_02.type_id() == memepp::meta::typid::string);
    auto value01_17 = v01_02.get_or<memepp::string>();
    REQUIRE(value01_17 == "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789");
	
	memepp::string_view sv01_01;
	ec = v01_02.try_get(sv01_01);
    REQUIRE(ec == 0);
    REQUIRE(sv01_01 == value01_16);
    REQUIRE(sv01_01 == value01_17);

	v01_01 = memepp::buffer{ (const uint8_t*)"1", 1 };
    REQUIRE(v01_01.is_null() == false);
    REQUIRE(v01_01.is_type(memepp::meta::typid::buffer));
    REQUIRE(v01_01.type_id() == memepp::meta::typid::buffer);
    auto value01_18 = v01_01.get_or<memepp::buffer>();
    REQUIRE(value01_18.size() == 1);
    REQUIRE(value01_18.at(0) == '1');

    v01_02 = v01_01;
    REQUIRE(v01_02.is_null() == false);
    REQUIRE(v01_02.is_type(memepp::meta::typid::buffer));
    REQUIRE(v01_02.type_id() == memepp::meta::typid::buffer);
    auto value01_19 = v01_02.get_or<memepp::buffer>();
    REQUIRE(value01_19.size() == 1);
    REQUIRE(value01_19.at(0) == '1');

}

TEST_CASE("memepp::variant - 02", "import from dll and export into dll")
{
	auto str01_01 = mm_from(std::string{ "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789" });
	memepp::variant var01_01 = str01_01;
	memepp::variant var01_02 = memepp::import_from_dll<memepp::variant>(var01_01.native_handle(), MMVAR__OBJ_SIZE);
    REQUIRE(var01_02.is_null() == false);
    REQUIRE(var01_02.is_type(memepp::meta::typid::string));
	auto str01_02 = var01_02.get_or<memepp::string>();
	REQUIRE(str01_02 == str01_01);
	REQUIRE(str01_02.storage_type() == memepp::string_storage_t::large);
    auto stk01_01 = memepp::export_into_dll<mmvarstk_t>(var01_02, MMVAR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&stk01_01]() { MemeVariantStack_unInit(&stk01_01, MMVAR__OBJ_SIZE); });
    
}

#include <thread>
#include <atomic>

bool MulitThreadRead__isSucc01 = false;

TEST_CASE("memepp::string - 200", "mulit thread read")
{
	do {

		std::string s =
			u8"未初始化的指针"
			u8"指向您分配但随后被释放的堆的指针。所以现在它指向一个你可以写入的位置，但现在你不能。"
			u8"本身已损坏的指针，例如被堆栈超出范围的数组覆盖的本地指针。"
			u8"堆中数组的数组越界条件";

		static const auto destruct_func = [](void* _object)
		{
			MulitThreadRead__isSucc01 = true;
			delete reinterpret_cast<std::string*>(_object);
		};
		static const auto data_func = [](const void* _object) { return reinterpret_cast<const std::string*>(_object)->data(); };
		static const auto size_func = [](const void* _object) { return reinterpret_cast<const std::string*>(_object)->size(); };

		memepp::string s01;
		auto obj = new std::string(std::move(s));
		auto ret = MemeStringStack_initTakeOverUserObject(
			const_cast<MemeStringStack_t*>(&(s01.native_handle())), MEME_STRING__OBJECT_SIZE,
			obj, destruct_func, data_func, size_func);
		if (ret) {
			destruct_func(obj);
		}

		std::vector<std::thread> thrs_;
		std::atomic_bool stop { false };
		for (auto index = 0; index < 64; ++index)
		{
			thrs_.push_back(std::thread([&s01, &stop]
			{
				auto src = s01.data();
				auto dst = (const char*)(NULL);

				while (!stop) {
					auto s = s01;
					std::this_thread::yield();
					dst = s.data();
				}
				REQUIRE(dst == src);
			}));
		}

		std::this_thread::sleep_for(std::chrono::seconds(5));
		stop = true;

		for (auto& thr : thrs_) {
			thr.join();
		}

	} while (0);
	REQUIRE(MulitThreadRead__isSucc01 == true);

	do {
		memepp::string s02 =
			u8"未初始化的指针"
			u8"指向您分配但随后被释放的堆的指针。所以现在它指向一个你可以写入的位置，但现在你不能。"
			u8"本身已损坏的指针，例如被堆栈超出范围的数组覆盖的本地指针。"
			u8"堆中数组的数组越界条件";

		std::vector<std::thread> thrs_;
		std::atomic_bool stop { false };
		for (auto index = 0; index < 64; ++index)
		{
			thrs_.push_back(std::thread([&s02, &stop]
			{
				auto src = s02.data();
				auto dst = (const char*)(NULL);

				while (!stop) {
					auto s = s02;
					std::this_thread::yield();
					dst = s.data();
				}
				REQUIRE(dst == src);
			}));
		}

		std::this_thread::sleep_for(std::chrono::seconds(5));

		stop = true;

		for (auto& thr : thrs_) {
			thr.join();
		}

	} while (0);

}


TEST_CASE("memepp::string_view - 00", "Accidents encountered in engineering practice")
{
#if INTPTR_MAX == INT64_MAX
	memepp::string s01 = "0123456789012345678";
#else
	memepp::string s01 = "0123456789";
#endif
	REQUIRE(s01.storage_type() == memepp::string_storage_t::small);
#if INTPTR_MAX == INT64_MAX
    REQUIRE(s01.size() == 19);
#else
	REQUIRE(s01.size() == 10);
#endif
    REQUIRE(s01.data() != nullptr);

	auto func01 = [](const memepp::string_view& _sv) { return memepp::string_view{ _sv.data(), _sv.size() }; };

	auto sv01 = func01(s01);
	REQUIRE(sv01.data() == s01.data());
    REQUIRE(sv01.size() == s01.size());
    REQUIRE(sv01 == s01);
    
}

TEST_CASE("memepp::string - 00", "Accidents encountered in engineering practice")
{

	std::string std02 = "01234567890123456789012345678901234567890123456789";

	memepp::string s02;
	auto func02 = [&](const memepp::string& _s)
	{
		s02 = _s;
	};
	func02(mm_from(std02));
	REQUIRE(s02 == std02);
#if INTPTR_MAX == INT64_MAX
	REQUIRE(s02.storage_type() == memepp::string_storage_t::medium);
#else
	REQUIRE(s02.storage_type() == memepp::string_storage_t::large);
#endif
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(s02.native_handle())) == 1);

	auto path = memepp::c_format(1024, "%s/%s_%d_%s",
		"Z:/path", "lt", 0, "db");
    REQUIRE(path == "Z:/path/lt_0_db");
    REQUIRE(path.size() == 15);
	

	do {
		std::vector<memepp::string> mstrs;
		std::vector<const char*>    cstrs;

		for (auto index = 0; index < 128; ++index)
		{
			memepp::string s{ "0" };
			cstrs.emplace_back(s.c_str());
			mstrs.emplace_back(std::move(s));
		}

		for (auto index = 0; index < mstrs.size(); ++index)
		{
			REQUIRE((void*)mstrs[index].c_str() != (void*)cstrs[index]);
		}
	} while (0);

	do {
		std::vector<memepp::string> mstrs1;
		std::vector<memepp::string> mstrs2;

		for (auto index = 0; index < 128; ++index)
		{
			mstrs1.emplace_back("0123456789");
		}

        mstrs2 = mstrs1;

		for (auto index = 0; index < mstrs2.size(); ++index)
		{
			REQUIRE((void*)mstrs2[index].c_str() != (void*)mstrs1[index].c_str());
		}

	} while (0);
	
	do {
		std::vector<memepp::string> mstrs;
		std::vector<const char*>    cstrs;

		for (auto index = 0; index < 128; ++index)
		{
			mstrs.emplace_back("012345678901234567890123456789012345678901234567890123456789");
			cstrs.emplace_back(mstrs.back().c_str());
		}

		for (auto index = 0; index < mstrs.size(); ++index)
		{
			REQUIRE((void*)mstrs[index].c_str() == (void*)cstrs[index]);
		}
	} while (0);
	
	do {
		int64_t v = INT64_MAX;
		std::vector<char> buf;
        buf.resize(32, 0);
        auto len = snprintf(buf.data(), buf.size(), "%lld - %lld", v, v);
        REQUIRE(len == 41);
	} while (0);
}
