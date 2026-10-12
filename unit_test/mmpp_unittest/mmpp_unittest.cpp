
#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <megopp/util/scope_cleanup.h>
#include <memepp/convert/std/c_str.hpp>
#include <random>

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

TEST_CASE("memepp::string constructors 01", "[string]") 
{
	REQUIRE(sizeof(memepp::string) == MEME_STRING__OBJECT_SIZE);

	memepp::string s01;
	REQUIRE(s01.storage_type() == memepp::string_storage_t::small);
	REQUIRE(s01.size() == 0);
	REQUIRE(s01.size() == strlen(s01.data()) );
	REQUIRE(s01.size() == strlen(s01.c_str()));
	REQUIRE(s01.empty());
	REQUIRE(s01.capacity() == MemeStringOption_getStorageSmallLimit());

	const mmint_t smallLimit = MemeStringOption_getStorageSmallLimit();
	std::string test01_src(smallLimit, 'x');
	const char* test01 = test01_src.c_str();
	memepp::string s02(test01, smallLimit);
	REQUIRE(s02.storage_type() == memepp::string_storage_t::small);
	REQUIRE(s02.size() == MemeStringOption_getStorageSmallLimit());
	REQUIRE(s02.size() == strlen(s02.data()) );
	REQUIRE(s02.size() == strlen(s02.c_str()));
	REQUIRE(!strcmp(s02.data() , test01));
	REQUIRE(!strcmp(s02.c_str(), test01));
	REQUIRE(s02 == test01);
	REQUIRE(test01 == s02);
	REQUIRE(s02.empty() == false);
	REQUIRE(s02.capacity() == 0);

    std::string test02_src(MEME_STRING__OBJECT_SIZE - 1, 'y');
    const char* test02 = test02_src.c_str();
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
	REQUIRE(s04.capacity() == MemeStringOption_getStorageSmallLimit() - 4);

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
    REQUIRE(s05.capacity() == MemeStringOption_getStorageSmallLimit() - 2);
    
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
    REQUIRE(s06.capacity() == MemeStringOption_getStorageSmallLimit() - 4);

}

TEST_CASE("memepp::string constructors 02", "[string]")
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

#include <memepp/convert/std/string.hpp>
#include <memepp/compare/std/string.hpp>

TEST_CASE("memepp::string move or swap", "[string]")
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
	REQUIRE(s04 == s03);
	REQUIRE(s04.storage_type() == memepp::string_storage_t::large);
	REQUIRE(s04.data() == s03.data());

	s01 = std::move(s03);
	REQUIRE(s03.empty());
	REQUIRE(s03.size() == 0);
	REQUIRE(s03.storage_type() == memepp::string_storage_t::small);
	REQUIRE(s01.empty() == false);
	REQUIRE(s01.size() == strlen(test_large_string_01));
	REQUIRE( !strcmp(s01.data(), test_large_string_01));
	REQUIRE(s01.storage_type() == memepp::string_storage_t::large);
	REQUIRE(MemeString_checkHeadTailMemory(memepp::to_pointer(s01.native_handle())) == 1);

	// --- self-assignment (copy) — must be a no-op ---
	{
		memepp::string self_s(test_large_string_01);
		REQUIRE(self_s.storage_type() == memepp::string_storage_t::large);
		const char* old_data = self_s.data();
		memepp::string& ref = self_s;
		self_s = ref;
		REQUIRE(self_s == test_large_string_01);
		REQUIRE(self_s.storage_type() == memepp::string_storage_t::large);
		REQUIRE(self_s.data() == old_data);
	}

	// --- self-move-assignment — must be a no-op (use alias to avoid -Wself-move) ---
	{
		memepp::string self_s(test_large_string_01);
		REQUIRE(self_s.storage_type() == memepp::string_storage_t::large);
		const char* old_data = self_s.data();
		memepp::string& ref = self_s;
		self_s = std::move(ref);
		REQUIRE(self_s == test_large_string_01);
		REQUIRE(self_s.storage_type() == memepp::string_storage_t::large);
		REQUIRE(self_s.data() == old_data);
	}

	// --- self-swap — must preserve content ---
	{
		memepp::string self_s(test_large_string_01);
		const char* old_data = self_s.data();
		self_s.swap(self_s);
		REQUIRE(self_s == test_large_string_01);
		REQUIRE(self_s.data() == old_data);
	}

	// --- move-assign onto non-empty large target (swap semantics: no release) ---
	{
		std::string large_a(test_large_string_01);
		std::string large_b(400, 'B'); // different large content (400 > medium limit)

		memepp::string target(large_a.c_str(), large_a.size());
		REQUIRE(target.storage_type() == memepp::string_storage_t::large);

		memepp::string source(large_b.c_str(), large_b.size());
		REQUIRE(source.storage_type() == memepp::string_storage_t::large);
		REQUIRE(target != source);

		target = std::move(source);
		// target now holds source's content ...
		REQUIRE(target.size() == large_b.size());
		REQUIRE(target.storage_type() == memepp::string_storage_t::large);
		REQUIRE(target == large_b);
		// ... and source holds target's old content (swap, not release)
		REQUIRE(source.size() == large_a.size());
		REQUIRE(source.storage_type() == memepp::string_storage_t::large);
		REQUIRE(source == test_large_string_01);
		REQUIRE(MemeString_checkHeadTailMemory(memepp::to_pointer(target.native_handle())) == 1);
		REQUIRE(MemeString_checkHeadTailMemory(memepp::to_pointer(source.native_handle())) == 1);
	}

	// --- medium source: swap and move-assign ---
	{
		const mmint_t smallLimit = MemeStringOption_getStorageSmallLimit();
		std::string med_src(smallLimit + 10, 'M');

		memepp::string m1(med_src.c_str(), med_src.size());
		REQUIRE(m1.storage_type() == memepp::string_storage_t::medium);

		// swap medium with empty
		memepp::string m2;
		m2.swap(m1);
		REQUIRE(m1.empty());
		REQUIRE(m1.storage_type() == memepp::string_storage_t::small);
		REQUIRE(m2 == med_src);
		REQUIRE(m2.storage_type() == memepp::string_storage_t::medium);

		// move-assign medium onto empty
		memepp::string m3 = std::move(m2);
		REQUIRE(m2.empty());
		REQUIRE(m3 == med_src);
		REQUIRE(m3.storage_type() == memepp::string_storage_t::medium);

		// move-assign medium onto non-empty large target
		memepp::string m4(test_large_string_01);
		REQUIRE(m4.storage_type() == memepp::string_storage_t::large);
		m4 = std::move(m3);
		REQUIRE(m4 == med_src);
		REQUIRE(m4.storage_type() == memepp::string_storage_t::medium);
		// m3 now holds the old large data (swap semantics)
		REQUIRE(m3 == test_large_string_01);
		REQUIRE(m3.storage_type() == memepp::string_storage_t::large);
	}

	// --- user source: swap and move-assign ---
	{
		std::string big(500, 'U');
		memepp::string u1 = memepp::from(std::move(big));
		REQUIRE(u1.storage_type() == memepp::string_storage_t::user);
		REQUIRE(big.empty()); // takeover

		// swap user with empty
		memepp::string u2;
		u2.swap(u1);
		REQUIRE(u1.empty());
		REQUIRE(u1.storage_type() == memepp::string_storage_t::small);
		REQUIRE(u2.storage_type() == memepp::string_storage_t::user);
		REQUIRE(u2.size() == 500);

		// move-assign user onto empty
		memepp::string u3 = std::move(u2);
		REQUIRE(u2.empty());
		REQUIRE(u3.storage_type() == memepp::string_storage_t::user);
		REQUIRE(u3.size() == 500);

		// move-assign user onto non-empty large target
		memepp::string u4(test_large_string_01);
		REQUIRE(u4.storage_type() == memepp::string_storage_t::large);
		u4 = std::move(u3);
		REQUIRE(u4.storage_type() == memepp::string_storage_t::user);
		REQUIRE(u4.size() == 500);
		// u3 now holds the old large data (swap semantics)
		REQUIRE(u3 == test_large_string_01);
		REQUIRE(u3.storage_type() == memepp::string_storage_t::large);
	}

}

#include <memepp/convert/std/string.hpp>
#include <memepp/compare/std/string.hpp>

TEST_CASE("memepp::string mutual convert of std::string", "[string]")
{
	std::string stdstr01;
	auto s01 = memepp::from(stdstr01);
	auto s02 = memepp::from(std::move(stdstr01));
	REQUIRE(s01.storage_type() == memepp::string_storage_t::small);
	REQUIRE(s01.size() == 0);
	REQUIRE(s01.size() == strlen(s01.data()));
	REQUIRE(s01.size() == strlen(s01.c_str()));
	REQUIRE(s01.empty());
	REQUIRE(s01.capacity() == MemeStringOption_getStorageSmallLimit());
	REQUIRE(s02.storage_type() == memepp::string_storage_t::small);
	REQUIRE(s02.size() == 0);
	REQUIRE(s02.size() == strlen(s02.data()));
	REQUIRE(s02.size() == strlen(s02.c_str()));
	REQUIRE(s02.empty());
	REQUIRE(s02.capacity() == MemeStringOption_getStorageSmallLimit());
	REQUIRE(s01 == s02);

    std::string stdstr02 = "test";
    auto s03 = memepp::from(stdstr02);
    auto s04 = memepp::from(std::move(stdstr02));
    // small: copy path in from(std::string&&), source unchanged
    REQUIRE(stdstr02 == "test");
    REQUIRE(s03.storage_type() == memepp::string_storage_t::small);
    REQUIRE(s03.size() == 4);
    REQUIRE(s03.size() == strlen(s03.data()));
    REQUIRE(s03.size() == strlen(s03.c_str()));
    REQUIRE(!strcmp(s03.data(), "test"));
    REQUIRE(!strcmp(s03.c_str(), "test"));
    REQUIRE(s03 == "test");
    REQUIRE("test" == s03);
    REQUIRE(s03.empty() == false);
    REQUIRE(s03.capacity() == MemeStringOption_getStorageSmallLimit() - 4);
    REQUIRE(s04.storage_type() == memepp::string_storage_t::small);
    REQUIRE(s04.size() == 4);
    REQUIRE(s04.size() == strlen(s04.data()));
    REQUIRE(s04.size() == strlen(s04.c_str()));
    REQUIRE(!strcmp(s04.data(), "test"));
    REQUIRE(!strcmp(s04.c_str(), "test"));
    REQUIRE(s04 == "test");
    REQUIRE("test" == s04);
    REQUIRE(s04.empty() == false);
    REQUIRE(s04.capacity() == MemeStringOption_getStorageSmallLimit() - 4);
    REQUIRE(s03 == s04);
    
	std::string stdstr030 { "test 123456789123456790", MEME_STRING__OBJECT_SIZE - 1 };
	std::string stdstr031 { "test 123456789123456790", MEME_STRING__OBJECT_SIZE - 1 };
    auto s05 = memepp::from(stdstr030);
    auto s06 = memepp::from(std::move(stdstr030));
    // medium: copy path in from(std::string&&), source unchanged
    REQUIRE(stdstr030 == stdstr031);
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
		"In the West, it is considered a sign of great good fortune to find four-leaf clover."
		"Four-leaf clover is a rare variant of the genus Cheatgrass (which includes the genus Clover and Alfalfa) that also has more than five leaves and up to eighteen leaves. "
        "In the West, it is considered a sign of great good fortune to find four-leaf clover.";
	auto s11 = memepp::from(stdstr11);
	REQUIRE(s11.storage_type() == memepp::string_storage_t::large);
	REQUIRE(s11.size() == stdstr11.size());
	REQUIRE(!strcmp(s11.data(), stdstr11.data()));
	REQUIRE(s11 == stdstr11);
	REQUIRE((s11 != stdstr11) == false);
	// large: takeover path in from(std::string&&), source emptied
	auto s12 = memepp::from(std::move(stdstr11));
	REQUIRE(s12.storage_type() == memepp::string_storage_t::user);
	REQUIRE(s12.size() == s11.size());
	REQUIRE(!strcmp(s12.data(), s11.data()));
	REQUIRE(s11 == s12);
	REQUIRE(s12 == s11);
	REQUIRE(stdstr11.empty());
	REQUIRE(MemeString_checkHeadTailMemory(memepp::to_pointer(s11.native_handle())) == 1);
	REQUIRE(MemeString_checkHeadTailMemory(memepp::to_pointer(s12.native_handle())) == 1);

	// === Reverse direction: memepp::string -> std::string ===

	// small
	{
		memepp::string ms_small = memepp::from(std::string{"test"});
		REQUIRE(ms_small.storage_type() == memepp::string_storage_t::small);
		auto back = memepp::into<std::string>(ms_small);
		REQUIRE(back == "test");
		auto back2 = mm_to<std::string>(ms_small);
		REQUIRE(back2 == "test");
	}

	// medium
	{
		std::string src{ "test 123456789123456790", MEME_STRING__OBJECT_SIZE - 1 };
		memepp::string ms_med = memepp::from(src);
		REQUIRE(ms_med.storage_type() == memepp::string_storage_t::medium);
		auto back = memepp::into<std::string>(ms_med);
		REQUIRE(back == src);
		auto back2 = mm_to<std::string>(ms_med);
		REQUIRE(back2 == src);
	}

	// large
	{
		std::string big{
			"Four-leaf clover is a rare variant of the genus Cheatgrass "
			"(which includes the genus Clover and Alfalfa) that also has "
			"more than five leaves and up to eighteen leaves. "
			"In the West, it is considered a sign of great good fortune "
			"to find four-leaf clover. "
			"Four-leaf clover is a rare variant of the genus Cheatgrass "
			"(which includes the genus Clover and Alfalfa) that also has "
			"more than five leaves and up to eighteen leaves. "
			"In the West, it is considered a sign of great good fortune "
			"to find four-leaf clover."
		};
		memepp::string ms_large = memepp::from(big);
		REQUIRE(ms_large.storage_type() == memepp::string_storage_t::large);
		auto back = memepp::into<std::string>(ms_large);
		REQUIRE(back == big);
		auto back2 = mm_to<std::string>(ms_large);
		REQUIRE(back2 == big);
	}

	// user (takeover from moved std::string)
	{
		std::string big{
			"Four-leaf clover is a rare variant of the genus Cheatgrass "
			"(which includes the genus Clover and Alfalfa) that also has "
			"more than five leaves and up to eighteen leaves. "
			"In the West, it is considered a sign of great good fortune "
			"to find four-leaf clover. "
			"Four-leaf clover is a rare variant of the genus Cheatgrass "
			"(which includes the genus Clover and Alfalfa) that also has "
			"more than five leaves and up to eighteen leaves. "
			"In the West, it is considered a sign of great good fortune "
			"to find four-leaf clover."
		};
		memepp::string ms_user = memepp::from(std::move(big));
		REQUIRE(ms_user.storage_type() == memepp::string_storage_t::user);
		REQUIRE(big.empty());
		auto back = memepp::into<std::string>(ms_user);
		REQUIRE(back.size() > 0);
		// round-trip: back -> memepp::string -> compare
		memepp::string ms_roundtrip = memepp::from(back);
		REQUIRE(ms_roundtrip == ms_user);
	}
}

#include <memepp/convert/std/vector.hpp>

TEST_CASE("memepp::string mutual convert of std::vector", "[string]")
{
	do {
		const mmint_t smallLimit = MemeStringOption_getStorageSmallLimit();
		std::vector<uint8_t> vec(smallLimit);
		for (mmint_t i = 0; i < smallLimit; ++i)
			vec[i] = static_cast<uint8_t>('A' + (i % 26));

		auto s_cc = mm_from(vec);
		REQUIRE(s_cc.size() == smallLimit);
		REQUIRE(s_cc.storage_type() == memepp::string_storage_t::small);

		auto s_mv = mm_from(std::vector<uint8_t>(vec));
		REQUIRE(s_mv == s_cc);

		auto back_exact = mm_into<std::vector<uint8_t>>(s_cc);
		REQUIRE(back_exact.size() == static_cast<size_t>(smallLimit));
		REQUIRE(back_exact == vec);
	} while (0);

	do {
		std::string src(1024, '\0');
		for (size_t i = 0; i < src.size(); ++i)
			src[i] = static_cast<char>('A' + (i % 26));
		std::vector<uint8_t> vec(src.begin(), src.end());

		auto s = mm_from(vec);
		REQUIRE(s.size() == 1024);
		REQUIRE(s.storage_type() == memepp::string_storage_t::large);
		REQUIRE(!memcmp(s.data(), src.data(), src.size()));

		auto back = mm_into<std::vector<uint8_t>>(s);
		REQUIRE(back.size() == vec.size());
		REQUIRE(back == vec);
	} while (0);

	do {
		const char raw[] = "vector<char> round trip";
		std::vector<char> vec(raw, raw + sizeof(raw) - 1);

		auto s_cc = mm_from(vec);
		REQUIRE(s_cc.size() == sizeof(raw) - 1);
		REQUIRE(!strcmp(s_cc.data(), raw));

		auto s_mv = mm_from(std::vector<char>(vec));
		REQUIRE(s_mv == s_cc);

		auto back = mm_into<std::vector<char>>(s_cc);
		REQUIRE(back.size() == vec.size());
		REQUIRE(back == vec);
	} while (0);

	do {
		std::vector<uint8_t> empty;
		auto s = mm_from(empty);
		REQUIRE(s.empty());
		REQUIRE(s.size() == 0);

		auto back = mm_into<std::vector<uint8_t>>(s);
		REQUIRE(back.empty());
	} while (0);

	do {
		memepp::string s = "hello vector";
		auto vec = mm_into<std::vector<char>>(s);
		REQUIRE(vec.size() == s.size());
		REQUIRE(!memcmp(vec.data(), s.data(), s.size()));
		REQUIRE(mm_from(vec) == s);
	} while (0);
}

TEST_CASE("memepp::string index of", "[string]")
{
	memepp::string s01 = "etebjkgdodoijnakccv";
    
	REQUIRE(s01.find("etebjkgdodoijnakccv", 1) == -1);
	REQUIRE(s01.find("etebjkgdodoijnakccv", 0) == 0);
    REQUIRE(s01.find("etebjkgdodoijnakccv") == 0);

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
	REQUIRE(s03_01.index_of(" ", true) == 17);
}


#include "memepp/split/self.hpp"
#include <list>
#include <array>
#include <deque>
#include <vector>
#include <algorithm>

#include <memepp/variable_buffer.hpp>

TEST_CASE("memepp::variable_buffer constructions", "[variable_buffer]")
{
	memepp::variable_buffer b01;
	REQUIRE(sizeof(memepp::variable_buffer) == MEME_STRING__OBJECT_SIZE);
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::small);
	REQUIRE(b01.size() == 0);
	REQUIRE(b01.empty());
	REQUIRE(b01.capacity() == MemeStringOption_getStorageSmallLimit());
    REQUIRE(b01.data() != nullptr);
    REQUIRE(b01.data()[0] == '\0');

	std::vector<uint8_t> buf01(MemeStringOption_getStorageSmallLimit());
	for (size_t i = 0; i < buf01.size(); ++i)
		buf01[i] = static_cast<uint8_t>(0x01 + i);
	memepp::variable_buffer b02(buf01.data(), buf01.size());
	REQUIRE(b02.storage_type() == memepp::buffer_storage_t::small);
	REQUIRE(b02.size() == MemeStringOption_getStorageSmallLimit());

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

	std::vector<uint8_t> buf03(MemeStringOption_getStorageSmallLimit(), 0x00);
	memepp::variable_buffer b05(MemeStringOption_getStorageSmallLimit(), 0);
	REQUIRE(b05.storage_type() == memepp::buffer_storage_t::small);
	REQUIRE(b05.size() == MemeStringOption_getStorageSmallLimit());
	REQUIRE(memcmp(b05.data(), buf03.data(), MemeStringOption_getStorageSmallLimit()) == 0);


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

TEST_CASE("memepp::variable_buffer append", "[variable_buffer]")
{
	static constexpr uint32_t APPEND_SEED = 0x12345678u;
	std::mt19937 rng(APPEND_SEED);
	INFO("variable_buffer append test seed: " << APPEND_SEED);

	memepp::variable_buffer b01;
	b01.resize(10, 1);

	uint8_t buf01[] = { 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 };
	b01.append(buf01, sizeof(buf01));
#if INTPTR_MAX == INT64_MAX
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::small);
#else
	REQUIRE(b01.storage_type() == memepp::buffer_storage_t::medium);
#endif
	REQUIRE(b01.size() == 21);
	uint8_t buf02[] = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 };
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
		std::uniform_int_distribution<size_t> size_dist(1, 128);
		std::vector<uint8_t> buf(size_dist(rng), 0xFF);

		vb08.append(buf.data(), buf.size());
		vb08_totalSize += buf.size();
		REQUIRE(vb08.size() == vb08_totalSize);
		REQUIRE(MemeString_checkHeadTailMemory(
			(MemeString_Const_t)memepp::to_pointer(vb08.native_handle())) == 1);
	}
	std::uniform_int_distribution<size_t> push_total_dist(1, 128);
	for (int index = 0, total = static_cast<int>(push_total_dist(rng)); index < total; ++index)
	{
		vb08.push_back(0xFF);
		vb08_totalSize += 1;
		REQUIRE(vb08.size() == vb08_totalSize);
		REQUIRE(MemeString_checkHeadTailMemory(
			(MemeString_Const_t)memepp::to_pointer(vb08.native_handle())) == 1);
	}
}

#include <memepp/buffer.hpp>

TEST_CASE("memepp::buffer to_string", "[buffer]")
{
	memepp::buffer b00;
	REQUIRE(b00.storage_type() == memepp::buffer_storage_t::small);

#if INTPTR_MAX == INT64_MAX
	uint8_t buf01[] = { 
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11,
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x00, 0x00
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

	std::vector<uint8_t> buf02(256, 0x07);
	buf02[0] = 0x01;

	memepp::buffer b02 { buf02.data(), static_cast<memepp::buffer::size_type>(buf02.size()) };
	REQUIRE(b02.storage_type() == memepp::buffer_storage_t::large);
	REQUIRE(b02.size() == buf02.size());
	REQUIRE(memcmp(b02.data(), buf02.data(), buf02.size()) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b02.native_handle())) == 1);

	auto s021 = b02.to_string(-1);
	REQUIRE(s021.storage_type() == memepp::string_storage_t::large);
	REQUIRE(s021.size() == buf02.size());
	REQUIRE(memcmp(s021.data(), buf02.data(), buf02.size()) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(s021.native_handle())) == 1);

	auto s022 = b02.to_string(16);
	REQUIRE(s022.storage_type() == memepp::string_storage_t::large);
	REQUIRE(s022.size() == buf02.size() - 16);
	REQUIRE(memcmp(s022.data(), buf02.data() + 16, buf02.size() - 16) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(s022.native_handle())) == 1);

	std::vector<uint8_t> buf03(256, 0x07);
	buf03[0] = 0x01;
	std::fill(buf03.begin() + 248, buf03.end(), 0x00);

	memepp::buffer b03{ buf03.data(), static_cast<memepp::buffer::size_type>(buf03.size()) };
	REQUIRE(b03.storage_type() == memepp::buffer_storage_t::large);
	REQUIRE(b03.size() == buf03.size());
	REQUIRE(memcmp(b03.data(), buf03.data(), buf03.size()) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(b03.native_handle())) == 1);

	auto s03 = b03.to_string(24);
	REQUIRE(s03.storage_type() == memepp::string_storage_t::large);
	REQUIRE(s03.size() == buf03.size() - 32);
	REQUIRE(memcmp(s03.data(), buf03.data() + 24, buf03.size() - 32) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(s03.native_handle())) == 1);
}

TEST_CASE("memepp::string en lower an en upper", "[string]")
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
	REQUIRE(l02.storage_type() == memepp::string_storage_t::medium);
	REQUIRE(l02 == "sdjodsjassnkldslkdssfdhkgdhkgdnkdskjddashuhisffskjfbkgdasgk好");

	auto u02 = s02.to_en_upper(); 
	REQUIRE(u02.storage_type() == memepp::string_storage_t::medium);
	REQUIRE(u02 == "SDJODSJASSNKLDSLKDSSFDHKGDHKGDNKDSKJDDASHUHISFFSKJFBKGDASGK好");
}

TEST_CASE("memepp::string starts_with", "[string]")
{
    memepp::string s01 = "aHbjhjhhkKhhkhAAA";
    
    // All prefixes should match (count overload: exercises same C core as the ladder)
    for (int len = 0; len <= static_cast<int>(s01.size()); ++len) {
        INFO("Testing prefix length: " << len);
        REQUIRE(s01.starts_with(s01.data(), len));
    }
    
    // False cases: longer than string or mismatch
    REQUIRE_FALSE(s01.starts_with("aHbjhjhhkKhhkhAAAA"));
    REQUIRE_FALSE(s01.starts_with("aHbjhjhhkKhhkhAAAB"));
    REQUIRE_FALSE(s01.starts_with("aHbjhjhhkKhhkhAB"));
    REQUIRE_FALSE(s01.starts_with("aHbjhjhhkKhhkhB"));
    REQUIRE_FALSE(s01.starts_with("aHbjhjhhkKhhkhh"));
    REQUIRE_FALSE(s01.starts_with("aHbjhjhhkKhhkhk"));
    REQUIRE_FALSE(s01.starts_with("aHbjhjhhkKhhhk"));
}

TEST_CASE("memepp::string ends_with", "[string]")
{
    memepp::string s01 = "aHbjhjhhkKhhkhAAA";
    
    // All suffixes should match (count overload, no temp allocation)
    for (int len = 0; len <= static_cast<int>(s01.size()); ++len) {
        INFO("Testing suffix length: " << len);
        REQUIRE(s01.ends_with(s01.data() + (s01.size() - len), len));
    }
    
    // False cases: longer than string or mismatch
    REQUIRE_FALSE(s01.ends_with("aHbjhjhhkKhhkhAAAA"));
    REQUIRE_FALSE(s01.ends_with("aHbjhjhhkKhhkhAAAB"));
    REQUIRE_FALSE(s01.ends_with("aHbjhjhhkKhhkhAB"));
    REQUIRE_FALSE(s01.ends_with("aHbjhjhhkKhhkhB"));
    REQUIRE_FALSE(s01.ends_with("aHbjhjhhkKhhkhh"));
    REQUIRE_FALSE(s01.ends_with("aHbjhjhhkKhhkhk"));
    REQUIRE_FALSE(s01.ends_with("aHbjhjhhkKhhhk"));
}

TEST_CASE("memepp::string contains", "[string]") 
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

TEST_CASE("memepp::string trim_space", "[string]")
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

TEST_CASE("memepp::string trim_left_space", "[string]")
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

TEST_CASE("memepp::string trim_right_space", "[string]")
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

TEST_CASE("memepp::string substr", "[string]")
{
    memepp::string s01 = "aHbjhjhhkKhhkhAAA";
    std::string expected = "aHbjhjhhkKhhkhAAA";
    
    // Test all (pos, count) combinations systematically
    for (int pos = 0; pos <= static_cast<int>(s01.size()) + 1; ++pos) {
        for (int count = 0; count <= static_cast<int>(s01.size()) + 1; ++count) {
            INFO("pos=" << pos << " count=" << count);
            auto result = s01.substr(pos, count);
            
            // Compute expected using std::string semantics
            if (pos > static_cast<int>(expected.size())) {
                REQUIRE(result.empty());
            } else {
                int actual_count = count;
                if (count < 0) {
                    actual_count = static_cast<int>(expected.size()) - pos;
                } else if (count > static_cast<int>(expected.size()) - pos) {
                    actual_count = static_cast<int>(expected.size()) - pos;
                }
                std::string exp = expected.substr(pos, actual_count);
                REQUIRE(result == exp.c_str());
            }
        }
        // Test default count (npos)
        INFO("pos=" << pos << " count=npos");
        auto result = s01.substr(pos);
        if (pos > static_cast<int>(expected.size())) {
            REQUIRE(result.empty());
        } else {
            std::string exp = expected.substr(pos);
            REQUIRE(result == exp.c_str());
        }
    }
    
    // Negative pos is clamped to 0 by the C core
    REQUIRE(s01.substr(-1, 5) == expected.substr(0, 5).c_str());
    REQUIRE(s01.substr(-10, 3) == expected.substr(0, 3).c_str());
    
    // Negative count means "rest of the string"
    REQUIRE(s01.substr(5, -1) == expected.substr(5).c_str());
    REQUIRE(s01.substr(0, -1) == expected.c_str());
}

TEST_CASE("memepp::string operator=", "[string]")
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
	REQUIRE(s12.storage_type() == memepp::string_storage_t::medium);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(s12.native_handle())) == 1);
}

TEST_CASE("memepp::string rfind", "[string]")
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
	
	// Derive expected positions via strstr instead of hardcoding magic offsets
	auto f_on_llvm = strstr(sz01, "on, LLVM");          // last "on" before "available on"
	auto f_on_many = strstr(sz01, "on many platforms."); // only full-match "on" in text
	auto f_avail_on = strstr(sz01, "available on");      // limit boundary for search range

	REQUIRE(s01.last_index_of("on", -1, 0, f_avail_on - sz01, true) == -1);
	REQUIRE(s01.last_index_of("on", -1, 0, f_avail_on - sz01, false) == f_on_llvm - sz01);
	REQUIRE(s01.last_index_of("on", -1, f_on_llvm - sz01, -1, true) == f_on_many - sz01);

	REQUIRE(s01.last_index_of("C++", false) == f013 - sz01);
	REQUIRE(s01.last_index_of("C++", -1, 0, f013 - sz01, true) == f012 - sz01);
	REQUIRE(s01.last_index_of("C++ (pronounced \"C plus plus\")", false) == 0);
	REQUIRE(s01.last_index_of("on", false) == sizeof(sz01) - sizeof("on many platforms."));

	memepp::string s02_01 = "fd sdhufjkdas kfsd ks fd78fs77&** 67fd ifd gfjh*&&*f safjh^&IIR^&%R";
	REQUIRE(s02_01.last_index_of("f", true) == 51);
	REQUIRE(s02_01.last_index_of("R", true) == 66);
	REQUIRE(s02_01.last_index_of("fd", true) == 0);
}

TEST_CASE("memepp::variable_buffer insert", "[variable_buffer]")
{
	static constexpr uint32_t INSERT_SEED = 0x87654321u;
	std::mt19937 rng(INSERT_SEED);
	INFO("variable_buffer insert test seed: " << INSERT_SEED);

#if INTPTR_MAX == INT64_MAX
    uint8_t buf01[24] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18 };
#else
    uint8_t buf01[12] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C };
#endif
	memepp::variable_buffer vb01;
	vb01.insert(0, buf01, MemeStringOption_getStorageSmallLimit() - 2);
    REQUIRE(vb01.storage_type() == memepp::buffer_storage_t::small);
    REQUIRE(vb01.size() == MemeStringOption_getStorageSmallLimit() - 2);
    REQUIRE(memcmp(vb01.data(), buf01, MemeStringOption_getStorageSmallLimit() - 2) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(vb01.native_handle())) == 1);
    
	vb01.insert(vb01.size(), buf01 + vb01.size(), 2);
    REQUIRE(vb01.storage_type() == memepp::buffer_storage_t::small);
    REQUIRE(vb01.size() == MemeStringOption_getStorageSmallLimit());
    REQUIRE(memcmp(vb01.data(), buf01, MemeStringOption_getStorageSmallLimit()) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(vb01.native_handle())) == 1);

	memepp::variable_buffer vb02;
    vb02.insert(0, buf01 + 2, MemeStringOption_getStorageSmallLimit() - 2);
    REQUIRE(vb02.storage_type() == memepp::buffer_storage_t::small);
    REQUIRE(vb02.size() == MemeStringOption_getStorageSmallLimit() - 2);
    REQUIRE(memcmp(vb02.data(), buf01 + 2, MemeStringOption_getStorageSmallLimit() - 2) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(vb02.native_handle())) == 1);

    vb02.insert(0, buf01, 2);
    REQUIRE(vb02.storage_type() == memepp::buffer_storage_t::small);
    REQUIRE(vb02.size() == MemeStringOption_getStorageSmallLimit());
    REQUIRE(memcmp(vb02.data(), buf01, MemeStringOption_getStorageSmallLimit()) == 0);
	REQUIRE(MemeString_checkHeadTailMemory(
		(MemeString_Const_t)memepp::to_pointer(vb02.native_handle())) == 1);

	std::uniform_int_distribution<int> small_dist(0, static_cast<int>(MemeStringOption_getStorageSmallLimit()) - 3);
	for (int index = 0; index < 10; ++index) {
		int randValue = small_dist(rng);
		memepp::variable_buffer vb00{ buf01, MemeStringOption_getStorageSmallLimit() - 2 };
        vb00.insert(randValue, buf01 + randValue, 2);
        REQUIRE(vb00.storage_type() == memepp::buffer_storage_t::small);
        REQUIRE(vb00.size() == MemeStringOption_getStorageSmallLimit());
        REQUIRE(memcmp(vb00.data(), buf01, randValue) == 0);
        REQUIRE(memcmp(vb00.data() + randValue, buf01 + randValue, 2) == 0);
        REQUIRE(memcmp(vb00.data() + randValue + 2, 
            buf01 + randValue, MemeStringOption_getStorageSmallLimit() - 2 - randValue) == 0);

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

    std::uniform_int_distribution<int> medium_dist(0, static_cast<int>(vec01.size()) - 1);
    for (int index = 0; index < 10; ++index) {
		int randValue = medium_dist(rng);
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
	std::uniform_int_distribution<size_t> buf_size_dist(1, 128);
	for (int index = 0, total = 256; index < total; ++index)
	{ 
		std::vector<uint8_t> buf(buf_size_dist(rng), 0xFF);

		size_t insert_pos = (vb07.size() == 0) ? 0 : std::uniform_int_distribution<size_t>(0, vb07.size() - 1)(rng);
		vb07.insert(insert_pos, buf.data(), buf.size());

		vb07_totalSize += buf.size();
		REQUIRE(vb07.size() == vb07_totalSize);
        REQUIRE(MemeString_checkHeadTailMemory(
            (MemeString_Const_t)memepp::to_pointer(vb07.native_handle())) == 1);
	}
    
}

#include <memepp/string_builder.hpp>
#include <memepp/operation/std/string.hpp>

TEST_CASE("memepp::string_builder append", "[string_builder]")
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

	// copy-ctor steals: auto_ptr-like ownership transfer (see string_builder_impl.hpp)
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

TEST_CASE("memepp::buffer_view constructors", "[buffer_view]")
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

TEST_CASE("memepp::string utf16", "[string]")
{
	SECTION("valid CJK conversion")
	{
	static constexpr const char16_t* C16_CJK_20 = u"\u7684\u4E00\u4E0D\u662F\u4E86\u4EBA\u5728\u6709\u6211\u4ED6\u8FD9\u4E3A\u4E4B\u6765\u5927\u4EE5\u4E2A\u4E2D\u4E0A\u4EEC";
	static constexpr const char* C8_CJK_20 = "\xE7\x9A\x84\xE4\xB8\x80\xE4\xB8\x8D\xE6\x98\xAF\xE4\xBA\x86\xE4\xBA\xBA\xE5\x9C\xA8\xE6\x9C\x89\xE6\x88\x91\xE4\xBB\x96\xE8\xBF\x99\xE4\xB8\xBA\xE4\xB9\x8B\xE6\x9D\xA5\xE5\xA4\xA7\xE4\xBB\xA5\xE4\xB8\xAA\xE4\xB8\xAD\xE4\xB8\x8A\xE4\xBB\xAC";
	memepp::string s01_01{ C8_CJK_20 };
    REQUIRE(s01_01.rune_size()    == 20);
    REQUIRE(s01_01.u16char_size() == 20);
	
    auto u16_01_01 = mm_to<std::u16string>(s01_01);
    REQUIRE(u16_01_01.size() == 20);
    REQUIRE(u16_01_01 == C16_CJK_20);

	auto s01_02 = mm_from(u16_01_01);
    REQUIRE(s01_02.size() == 60);
    REQUIRE(s01_02.rune_size()    == 20);
    REQUIRE(s01_02.u16char_size() == 20);
    REQUIRE(s01_02 == s01_01);
	REQUIRE(s01_02 == C8_CJK_20);
	}
	

	SECTION("truncated continuation byte")
	{
	static constexpr const char16_t* C16_CJK_02 = u"\u7684\u4E00";
	static constexpr const char* C8_CJK_BAD_02 = "\xE7\x9A\x84\xE4\xB8\x80\xE4\x00\x8D\xE6\x98\xAF\xE4\xBA\x86\xE4\xBA\xBA\xE5\x9C\xA8\xE6\x9C\x89\xE6\x88\x91\xE4\xBB\x96\xE8\xBF\x99\xE4\xB8\xBA\xE4\xB9\x8B\xE6\x9D\xA5\xE5\xA4\xA7\xE4\xBB\xA5\xE4\xB8\xAA\xE4\xB8\xAD\xE4\xB8\x8A\xE4\xBB\xAC";
    memepp::string s02_01{ C8_CJK_BAD_02 };
    REQUIRE(s02_01.rune_size()    == 2);
    REQUIRE(s02_01.u16char_size() == 2);

    auto u16_02_01 = mm_to<std::u16string>(s02_01);
    REQUIRE(u16_02_01.size() == 2);
    REQUIRE(u16_02_01 == C16_CJK_02);

    auto s02_02 = mm_from(u16_02_01);
    REQUIRE(s02_02.size() == 6);
    REQUIRE(s02_02.rune_size() == 2);
    REQUIRE(s02_02.u16char_size() == 2);
	REQUIRE(s02_02 == memepp::string_view{ s02_01.data(), 6 });
	}
	
	
	SECTION("invalid continuation byte (loose vs strict)")
	{
	static constexpr const char16_t* C16_CJK_03 = u"\u7684\u4E00";
	static constexpr const char* C8_CJK_BAD_03 = "\xE7\x9A\x84\xE4\xB8\x80\xE4\x01\x8D\xE6\x98\xAF\xE4\xBA\x86\xE4\xBA\xBA\xE5\x9C\xA8\xE6\x9C\x89\xE6\x88\x91\xE4\xBB\x96\xE8\xBF\x99\xE4\xB8\xBA\xE4\xB9\x8B\xE6\x9D\xA5\xE5\xA4\xA7\xE4\xBB\xA5\xE4\xB8\xAA\xE4\xB8\xAD\xE4\xB8\x8A\xE4\xBB\xAC";
    memepp::string s03_01{ C8_CJK_BAD_03 };
    // Design rationale: rune_size() uses loose validation (checks only the leading
    // byte for performance), while u16char_size() uses strict validation (must fully
    // decode to compute UTF-16 output size). This is intentional.
    // For \xE4\x01\x8D: rune_size counts it as 1 rune (valid lead byte), u16char_size
    // stops at the first invalid continuation byte.
    REQUIRE(s03_01.rune_size()    == 20);  // Loose: skips invalid sequences, continues counting
    REQUIRE(s03_01.u16char_size() == 2 );  // Strict: stops at first invalid byte

    auto u16_03_01 = mm_to<std::u16string>(s03_01);
    REQUIRE(u16_03_01.size() == 2 );
    REQUIRE(u16_03_01 == C16_CJK_03);
	}

	SECTION("boundary codepoints (U+007F .. U+10FFFF)")
	{
    // Note: 5/6-byte UTF-8 sequences (0xF8.., 0xFC..) are supported by rune operations
    // but cannot round-trip through UTF-16 (max U+10FFFF). See "utf8 extended" test case.
	static constexpr const char* C8_BOUNDARY = "\x7F \xC2\x80 \xDF\xBF \xE0\xA0\x80 \xEF\xBF\xBF \xF0\x90\x80\x80 ";
    memepp::string s04_01{ C8_BOUNDARY };
    REQUIRE(s04_01.rune_size()    == 12);
    REQUIRE(s04_01.u16char_size() == 13);

    auto u16_04_01 = mm_to<std::u16string>(s04_01);
    REQUIRE(u16_04_01.size() == 13);

    auto s04_02 = mm_from(u16_04_01);
    REQUIRE(s04_02.size()         == strlen(C8_BOUNDARY));
    REQUIRE(s04_02.rune_size()    == 12);
    REQUIRE(s04_02.u16char_size() == 13);
    REQUIRE(s04_02 == s04_01);
    REQUIRE(s04_02 == C8_BOUNDARY);
	}

}

TEST_CASE("memepp::string utf8 extended (5/6-byte)", "[string]")
{
	// 5/6-byte UTF-8 lead bytes are valid for the library's rune operations
	// (31-bit code point support), but the encoded code points exceed U+10FFFF
	// and therefore cannot be represented in UTF-16.
	auto c8_5byte = "\xF8\x88\x80\x80\x80 \xFB\xBF\xBF\xBF\xBF ";
	memepp::string s5{ c8_5byte };
	// rune_size uses loose validation (lead byte only): 5-byte + space + 5-byte + space = 4
	REQUIRE(s5.rune_size() == 4);
	// Strict UTF-16 size computation stops at the first sequence that cannot
	// be represented in UTF-16 (code point > U+10FFFF).
	REQUIRE(s5.u16char_size() == 0);

	auto c8_6byte = "\xFC\x84\x80\x80\x80\x80 \xFD\xBF\xBF\xBF\xBF\xBF ";
	memepp::string s6{ c8_6byte };
	// 6-byte + space + 6-byte + space = 4
	REQUIRE(s6.rune_size() == 4);
	REQUIRE(s6.u16char_size() == 0);

	// A trailing 4-byte rune must still be counted by rune_size (loose
	// validation) while u16char_size stops at the invalid 5-byte sequence.
	auto c8_mixed = "\xF8\x88\x80\x80\x80 \xF0\x90\x80\x80";
	memepp::string s7{ c8_mixed };
	// 5-byte + space + 4-byte = 3
	REQUIRE(s7.rune_size() == 3);
	REQUIRE(s7.u16char_size() == 0);
}

TEST_CASE("memepp::string utf8", "[string]")
{
	static constexpr const char* C8_CJK_20_01 = "\xE7\x9A\x84\xE4\xB8\x80\xE4\xB8\x8D\xE6\x98\xAF\xE4\xBA\x86\xE4\xBA\xBA\xE5\x9C\xA8\xE6\x9C\x89\xE6\x88\x91\xE4\xBB\x96\xE8\xBF\x99\xE4\xB8\xBA\xE4\xB9\x8B\xE6\x9D\xA5\xE5\xA4\xA7\xE4\xBB\xA5\xE4\xB8\xAA\xE4\xB8\xAD\xE4\xB8\x8A\xE4\xBB\xAC";
	static constexpr const char* C8_CJK_20_02 = "\xE7\x9A\x84\xE4\xB8\x80\xE4\xB8\x8D\xE6\x98\xAF\xE4\xB8\x8D\xE4\xBA\xBA\xE5\x9C\xA8\xE6\x9C\x89\xE6\x88\x91\xE4\xBB\x96\xE8\xBF\x99\xE4\xB8\xBA\xE4\xB9\x8B\xE6\x9D\xA5\xE5\xA4\xA7\xE4\xBB\xA5\xE4\xB8\xAA\xE4\xB8\xAD\xE4\xB8\x8A\xE4\xBB\xAC";
	static constexpr const char* C8_CJK_20_03 = "\xE7\x9A\x84\xE4\xB8\x80\xE4\xB8\x8D\xE6\x98\xAFh\xE4\xBA\xBA\xE5\x9C\xA8\xE6\x9C\x89\xE6\x88\x91\xE4\xBB\x96\xE8\xBF\x99\xE4\xB8\xBA\xE4\xB9\x8B\xE6\x9D\xA5\xE5\xA4\xA7\xE4\xBB\xA5\xE4\xB8\xAA\xE4\xB8\xAD\xE4\xB8\x8A\xE4\xBB\xAC";

	memepp::string s01_01{ C8_CJK_20_01 };

	SECTION("mapping_convert")
	{
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

    auto s01_05 = s01_01.mapping_convert([](memepp::rune& r)
    {
        if (r == "\xE4\xBA\x86")
            r = memepp::rune{ (const uint8_t*)"\xE4\xB8\x8D", -1 };
        return 0;
    });
    REQUIRE(s01_05.size() == strlen(C8_CJK_20_02));
    REQUIRE(s01_05 == C8_CJK_20_02);

    auto s01_06 = s01_01.mapping_convert([](memepp::rune& r)
    {
        if (r == "\xE4\xBA\x86")
            r = memepp::rune{ 'h' };
        return 0;
    });
    REQUIRE(s01_06.size() == strlen(C8_CJK_20_03));
    REQUIRE(s01_06 == C8_CJK_20_03);
	}

	SECTION("rune iterator")
	{
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

	// Library-defined behavior (non-standard): advancing beyond the end / before
	// the begin clamps to the boundary instead of being undefined behavior.
	// std::next / std::prev with an out-of-range count is therefore well-defined
	// for const_rune_iterator; see the contract note in rune_iterator.hpp.
	auto it01_01 = s01_01.rune_begin();
	it01_01 = std::next(it01_01, 100);
    REQUIRE(it01_01 == s01_01.rune_end());

    it01_01 = std::prev(it01_01, 100);
    REQUIRE(it01_01 == s01_01.rune_begin());

    // Same clamping contract (library-defined): next(begin, 10) on a 3-rune
    // string clamps to the end, so the distance is 3.
    memepp::string s01_04{ C8_CJK_20_01, 11 };
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

	// Same clamping contract (library-defined): prev(it, 10) on a 3-rune string
	// clamps to the begin.
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
}

TEST_CASE("memepp::variable_buffer remove", "[variable_buffer]")
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

TEST_CASE("memepp::string operator+", "[string]")
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

TEST_CASE("memepp::string format", "[string]")
{
    auto str01_01 = memepp::c_format(-1, -1, "Hello %s", "World");
    REQUIRE(str01_01 == "Hello World");
	
    auto str01_02 = memepp::c_format(-1, 7, "Hello %s", "World");
    REQUIRE(str01_02 == "Hello World");
	
    auto str01_03 = memepp::c_format(7, -1, "Hello %s", "World");
    REQUIRE(str01_03 == "Hello W");

    auto str01_04 = memepp::c_format(7, 5, "Hello %s", "World");
    REQUIRE(str01_04 == "Hello W");
	
    auto str01_05 = memepp::c_format(5, 7, "Hello %s", "World");
    REQUIRE(str01_05 == "Hello");

    auto str02_01 = memepp::c_format(
		-1, -1, 
		"Hellooooooooooo %s - (%lld) - (%lld)", "Worlddddddddddd", (long long)(INT64_MIN), (long long)(INT64_MAX));
    REQUIRE(str02_01 == "Hellooooooooooo Worlddddddddddd - (-9223372036854775808) - (9223372036854775807)");
	
    auto str02_02 = memepp::c_format(
        -1, 2 * MMSTR__OBJ_SIZE, 
		"Hellooooooooooo %s - (%lld) - (%lld)", "Worlddddddddddd", (long long)(INT64_MIN), (long long)(INT64_MAX));
    REQUIRE(str02_02 == "Hellooooooooooo Worlddddddddddd - (-9223372036854775808) - (9223372036854775807)");
	
    auto str02_03 = memepp::c_format(
        48, -1, 
		"Hellooooooooooo %s - (%lld) - (%lld)", "Worlddddddddddd", (long long)(INT64_MIN), (long long)(INT64_MAX));
    REQUIRE(str02_03 == "Hellooooooooooo Worlddddddddddd - (-922337203685");

    auto str02_04 = memepp::c_format(
		48, 48,
		"Hellooooooooooo %s - (%lld) - (%lld)", "Worlddddddddddd", (long long)(INT64_MIN), (long long)(INT64_MAX));
    REQUIRE(str02_04 == "Hellooooooooooo Worlddddddddddd - (-922337203685");

    auto str02_05 = memepp::c_format(
        72, 48,
		"Hellooooooooooo %s - (%lld) - (%lld)", "Worlddddddddddd", (long long)(INT64_MIN), (long long)(INT64_MAX));
    REQUIRE(str02_05 == "Hellooooooooooo Worlddddddddddd - (-9223372036854775808) - (922337203685");

    auto str02_06 = memepp::c_format(
		48, 72,
		"Hellooooooooooo %s - (%lld) - (%lld)", "Worlddddddddddd", (long long)(INT64_MIN), (long long)(INT64_MAX));
    REQUIRE(str02_06 == "Hellooooooooooo Worlddddddddddd - (-922337203685");

	auto str02_07 = memepp::c_format(
		-1, 128,
		"Hellooooooooooo %s - (%lld) - (%lld)", "Worlddddddddddd", (long long)(INT64_MIN), (long long)(INT64_MAX));
    REQUIRE(str02_07 == "Hellooooooooooo Worlddddddddddd - (-9223372036854775808) - (9223372036854775807)");

    auto str02_08 = memepp::c_format(
        128, -1,
        "Hellooooooooooo %s - (%lld) - (%lld)", "Worlddddddddddd", (long long)(INT64_MIN), (long long)(INT64_MAX));
    REQUIRE(str02_08 == "Hellooooooooooo Worlddddddddddd - (-9223372036854775808) - (9223372036854775807)");
	
    auto str02_09 = memepp::c_format(
        128, 128,
        "Hellooooooooooo %s - (%lld) - (%lld)", "Worlddddddddddd", (long long)(INT64_MIN), (long long)(INT64_MAX));
    REQUIRE(str02_09 == "Hellooooooooooo Worlddddddddddd - (-9223372036854775808) - (9223372036854775807)");
	
	// --- Edge cases: limit=0, invalid negative limit, NULL fmt, non-ASCII, empty fmt ---

	// limit=0: C layer treats it as no limit (only _size_limit > 0 clamps)
	auto edge_01 = memepp::c_format(0, -1, "Hello %s", "World");
	REQUIRE(edge_01 == "Hello World");

	// Invalid negative limit (-2): same behavior as -1, no limit
	auto edge_02 = memepp::c_format(-2, -1, "Hello %s", "World");
	REQUIRE(edge_02 == "Hello World");

	// NULL fmt: wrapper returns empty string without invoking vsnprintf
	auto edge_03 = memepp::c_format(-1, static_cast<const char*>(nullptr));
	REQUIRE(edge_03.empty());

	auto edge_03b = memepp::c_format(-1, -1, static_cast<const char*>(nullptr));
	REQUIRE(edge_03b.empty());

	// Non-ASCII: UTF-8 multi-byte characters pass through c_format byte-exact
	auto edge_04 = memepp::c_format(-1, -1, "%s", u8"你好世界");
	REQUIRE(edge_04 == u8"你好世界");

	auto edge_05 = memepp::c_format(-1, u8"日本語テスト %d", 123);
	REQUIRE(edge_05 == u8"日本語テスト 123");

	// Mixed ASCII + non-ASCII clamped by limit at a rune boundary
	// "Hello " = 6 bytes, + U+4F60 (你) 3 bytes = 9
	// u8 literal: UTF-8 execution charset, immune to ACP conversion
	auto edge_06 = memepp::c_format(9, -1, u8"Hello 你好 World");
	REQUIRE(edge_06.size() == 9);
	// Verify byte-by-byte to avoid Catch stringification issues with UTF-8
	REQUIRE(edge_06[0] == 'H');
	REQUIRE(edge_06[5] == ' ');
	REQUIRE(static_cast<int>(static_cast<unsigned char>(edge_06[6])) == 0xe4); // First byte of 你
	REQUIRE(static_cast<int>(static_cast<unsigned char>(edge_06[7])) == 0xbd);
	REQUIRE(static_cast<int>(static_cast<unsigned char>(edge_06[8])) == 0xa0);

	// Empty format string: produces empty result
	auto edge_07 = memepp::c_format(-1, -1, "");
	REQUIRE(edge_07.empty());

	// predict=0 with the 4-arg overload forces the vsnprintf(NULL, 0, ...) path
	auto edge_08 = memepp::c_format(-1, 0, "Value: %d", 42);
	REQUIRE(edge_08 == "Value: 42");
}

TEST_CASE("memepp::string foreach", "[string]")
{
    mmint_t pos = 0;
    auto runeCount = 0;
    memepp::string s01 = "Hello World!";
    pos = s01.rune_foreach([&](const memepp::rune& _r)
    {
        if (runeCount == 5)
            REQUIRE(_r == ' ');

        ++runeCount;
    });
    REQUIRE(runeCount == 12);
    REQUIRE(pos == 12);

    memepp::string s02 = "";
    pos = s02.rune_foreach([&](const memepp::rune& _r)
    {
        REQUIRE(false);
    });
    REQUIRE(pos == 0);
    
    memepp::string s03 = u8"您好，世界！";
    runeCount = 0;
    pos = s03.rune_foreach([&](const memepp::rune& _r)
    {
        ++runeCount;
        if (runeCount == 1)
            REQUIRE(_r == u8"您");
        if (runeCount == 2)
            REQUIRE(_r == u8"好");
        if (runeCount == 3)
            REQUIRE(_r == u8"，");
        if (runeCount == 4)
            REQUIRE(_r == u8"世");
        if (runeCount == 5)
            REQUIRE(_r == u8"界");
        if (runeCount == 6)
            REQUIRE(_r == u8"！");
    });
    REQUIRE(runeCount == 6);
    REQUIRE(pos == 18);
}

TEST_CASE("memepp::string rune_foreach edge cases", "[string]")
{
    SECTION("invalid UTF-8 - lone continuation byte (0x80-0xBF) skipped") {
        // 0x80 is a continuation byte without a leading byte -> skipped
        memepp::string s("\x80" "A" "\x80" "B", 4);
        int count = 0;
        auto pos = s.rune_foreach([&](const memepp::rune& _r) {
            ++count;
            if (count == 1) REQUIRE(_r == 'A');
            if (count == 2) REQUIRE(_r == 'B');
        });
        REQUIRE(count == 2);
        REQUIRE(pos == 4);
    }

    SECTION("invalid UTF-8 - 0xFE and 0xFF bytes skipped") {
        memepp::string s("\xFE" "A" "\xFF" "B", 4);
        int count = 0;
        auto pos = s.rune_foreach([&](const memepp::rune& _r) {
            ++count;
            if (count == 1) REQUIRE(_r == 'A');
            if (count == 2) REQUIRE(_r == 'B');
        });
        REQUIRE(count == 2);
        REQUIRE(pos == 4);
    }

    SECTION("truncated multi-byte sequence at end - early break") {
        // 'A' (1 byte) + truncated 3-byte sequence (0xE4 0xBD, missing 3rd byte)
        memepp::string s("A\xE4\xBD", 3);
        int count = 0;
        auto pos = s.rune_foreach([&](const memepp::rune& _r) {
            ++count;
            REQUIRE(_r == 'A');
        });
        REQUIRE(count == 1);
        REQUIRE(pos == 1); // stopped before truncated bytes
    }

    SECTION("callback early break via mmflag_cbproc_break") {
        memepp::string s("Hello");
        int count = 0;
        auto pos = s.rune_foreach([&](const memepp::rune& _r) -> mmflag_cbproc_t {
            if (count == 2)
                return mmflag_cbproc_break;
            ++count;
            return mmflag_cbproc_continue;
        });
        REQUIRE(count == 2); // only 'H', 'e' processed
        REQUIRE(pos == 2);   // stopped at byte offset 2
    }

    SECTION("callback negative return value propagated") {
        memepp::string s("ABCDE");
        // Must return mmflag_cbproc_t exactly (int would hit the void-path
        // overload and have its value discarded by the helper).
        auto pos = s.rune_foreach([](const memepp::rune&) -> mmflag_cbproc_t {
            return static_cast<mmflag_cbproc_t>(-42);
        });
        REQUIRE(pos == -42);
    }

    SECTION("mixed valid and invalid bytes") {
        // 0x80 (invalid) + 'A' + 0xFF (invalid) + 'B' + 0xE4 0xBD 0xA0 (你, valid 3-byte)
        memepp::string s("\x80" "A" "\xFF" "B\xE4\xBD\xA0", 7);
        int count = 0;
        auto pos = s.rune_foreach([&](const memepp::rune& _r) {
            ++count;
            if (count == 1) REQUIRE(_r == 'A');
            if (count == 2) REQUIRE(_r == 'B');
            if (count == 3) REQUIRE(_r == u8"你");
        });
        REQUIRE(count == 3);
        REQUIRE(pos == 7);
    }

    SECTION("all invalid bytes - callback never invoked") {
        memepp::string s("\x80\xFF\x80\xFF", 4);
        int count = 0;
        auto pos = s.rune_foreach([&](const memepp::rune&) {
            ++count;
        });
        REQUIRE(count == 0);
        REQUIRE(pos == 4); // all bytes scanned, none valid
    }
}

#include <memepp/variant.hpp>

TEST_CASE("memepp::variant basic operations", "[variant]")
{
    SECTION("null default state") {
        memepp::variant v;
        REQUIRE(v.is_null());
        REQUIRE(v.is_type(memepp::meta::typid::null));
        REQUIRE(v.type_id() == memepp::meta::typid::null);
    }

    SECTION("scalar types - get_or success") {
        memepp::variant v;
        
        // mmbyte_t (uint8_t)
        v = mmbyte_t{200};
        REQUIRE(v.is_type(memepp::meta::typid::byte));
        REQUIRE(v.get_or<mmbyte_t>() == 200);
        
        // char
        v = char{'Z'};
        REQUIRE(v.is_type(memepp::meta::typid::char_t));
        REQUIRE(v.get_or<char>() == 'Z');
        
        // wchar_t
        v = wchar_t{0x6F22};
        REQUIRE(v.is_type(memepp::meta::typid::wchar));
        REQUIRE(v.get_or<wchar_t>() == 0x6F22);
        
        // int64_t
        v = int64_t{-9876543210LL};
        REQUIRE(v.is_type(memepp::meta::typid::int64));
        REQUIRE(v.get_or<int64_t>() == -9876543210LL);
        
        // uint64_t
        v = uint64_t{18446744073709551000ULL};
        REQUIRE(v.is_type(memepp::meta::typid::uint64));
        REQUIRE(v.get_or<uint64_t>() == 18446744073709551000ULL);
        
        // double
        v = double{2.718281828459045};
        REQUIRE(v.is_type(memepp::meta::typid::double_t));
        REQUIRE(v.get_or<double>() == 2.718281828459045);
        
        // rune (龍 = U+9F8D, UTF-8: E9 BE 99)
        v = memepp::rune{ (const uint8_t*)"\xE9\xBE\x99", 3 };
        REQUIRE(v.is_type(memepp::meta::typid::rune));
        REQUIRE(v.get_or<memepp::rune>() == memepp::rune{ (const uint8_t*)"\xE9\xBE\x99", 3 });
        
        // int32_t -> int64_t
        int32_t i32_val = -2147483647;
        v = int64_t{i32_val};
        REQUIRE(v.is_type(memepp::meta::typid::int64));
        REQUIRE(v.get_or<int64_t>() == -2147483647);
        
        // uint32_t -> uint64_t
        uint32_t u32_val = 4294967295U;
        v = uint64_t{u32_val};
        REQUIRE(v.is_type(memepp::meta::typid::uint64));
        REQUIRE(v.get_or<uint64_t>() == 4294967295U);
        
        // float -> double
        float f_val = 1.5f;
        v = double{static_cast<double>(f_val)};
        REQUIRE(v.is_type(memepp::meta::typid::double_t));
        REQUIRE(v.get_or<double>() == 1.5);
    }

    SECTION("string and buffer types - get_or success") {
        memepp::variant v;
        
        // short string (SSO)
        v = memepp::string{"hello"};
        REQUIRE(v.is_type(memepp::meta::typid::string));
        REQUIRE(v.get_or<memepp::string>() == "hello");
        
        // long string (heap)
        memepp::string long_str = "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
        v = long_str;
        REQUIRE(v.get_or<memepp::string>() == long_str);
        
        // buffer
        uint8_t buf_data[] = {0x01, 0x02, 0x03, 0x04};
        v = memepp::buffer{buf_data, 4};
        REQUIRE(v.is_type(memepp::meta::typid::buffer));
        auto buf = v.get_or<memepp::buffer>();
        REQUIRE(buf.size() == 4);
        REQUIRE(buf.at(0) == 0x01);
        REQUIRE(buf.at(3) == 0x04);
    }

    SECTION("copy semantics") {
        // scalar copy
        memepp::variant v1 = int64_t{999};
        memepp::variant v2 = v1;
        REQUIRE(v2.is_type(memepp::meta::typid::int64));
        REQUIRE(v2.get_or<int64_t>() == 999);
        
        // string copy (shared storage)
        memepp::variant v3 = memepp::string{"test"};
        memepp::variant v4 = v3;
        REQUIRE(v4.get_or<memepp::string>() == "test");
        
        // buffer copy
        memepp::variant v5 = memepp::buffer{(const uint8_t*)"abc", 3};
        memepp::variant v6 = v5;
        REQUIRE(v6.get_or<memepp::buffer>().size() == 3);
    }

    SECTION("try_get success") {
        memepp::variant v = memepp::string{"hello world"};
        
        // string_view from string
        memepp::string_view sv;
        mgec_t ec = v.try_get(sv);
        REQUIRE(ec == 0);
        REQUIRE(sv == "hello world");
        
        // string from string
        memepp::string s;
        ec = v.try_get(s);
        REQUIRE(ec == 0);
        REQUIRE(s == "hello world");
        
        // int64_t from int64_t
        memepp::variant v2 = int64_t{-42};
        int64_t i64;
        ec = v2.try_get(i64);
        REQUIRE(ec == 0);
        REQUIRE(i64 == -42);
    }

    SECTION("try_get failure - type mismatch") {
        mgec_t ec;
        
        // int64_t variant, try_get<string>
        {
            memepp::variant v = int64_t{100};
            memepp::string s;
            ec = v.try_get(s);
            REQUIRE(ec != 0);
            REQUIRE(s.empty());
        }
        
        // string variant, try_get<int64_t>
        {
            memepp::variant v = memepp::string{"not a number"};
            int64_t i64 = 999;
            ec = v.try_get(i64);
            REQUIRE(ec != 0);
        }
        
        // double variant, try_get<uint64_t>
        {
            memepp::variant v = double{3.14};
            uint64_t u64;
            ec = v.try_get(u64);
            REQUIRE(ec != 0);
        }
        
        // null variant, try_get<any>
        {
            memepp::variant v;
            int64_t i64;
            ec = v.try_get(i64);
            REQUIRE(ec != 0);
        }
        
        // byte variant, try_get<double>
        {
            memepp::variant v = mmbyte_t{255};
            double d;
            ec = v.try_get(d);
            REQUIRE(ec != 0);
        }
    }

    SECTION("get_or default - type mismatch") {
        // int64_t variant, get_or<string>
        {
            memepp::variant v = int64_t{42};
            auto result = v.get_or<memepp::string>(memepp::string{"default"});
            REQUIRE(result == "default");
        }
        
        // string variant, get_or<int64_t>
        {
            memepp::variant v = memepp::string{"text"};
            auto result = v.get_or<int64_t>(-1);
            REQUIRE(result == -1);
        }
        
        // double variant, get_or<uint64_t>
        {
            memepp::variant v = double{2.718};
            auto result = v.get_or<uint64_t>(100);
            REQUIRE(result == 100);
        }
        
        // null variant, get_or<any>
        {
            memepp::variant v;
            auto result = v.get_or<int64_t>(999);
            REQUIRE(result == 999);
        }
        
        // byte variant, get_or<double>
        {
            memepp::variant v = mmbyte_t{10};
            auto result = v.get_or<double>(0.0);
            REQUIRE(result == 0.0);
        }
        
        // get_or with default-constructed value
        {
            memepp::variant v = int64_t{1};
            auto result = v.get_or<memepp::string>();
            REQUIRE(result.empty());
        }
    }

    SECTION("bool via try_convert") {
        memepp::variant v;
        mgec_t ec;
        bool result;
        
        // int64_t(0) -> false
        v = int64_t{0};
        ec = v.try_convert(result);
        REQUIRE(ec == 0);
        REQUIRE(result == false);
        
        // int64_t(1) -> true
        v = int64_t{1};
        ec = v.try_convert(result);
        REQUIRE(ec == 0);
        REQUIRE(result == true);
        
        // int64_t(-1) -> true (non-zero is true)
        v = int64_t{-1};
        ec = v.try_convert(result);
        REQUIRE(ec == 0);
        REQUIRE(result == true);
        
        // uint64_t(42) -> true
        v = uint64_t{42};
        ec = v.try_convert(result);
        REQUIRE(ec == 0);
        REQUIRE(result == true);
        
        // double(0.0) -> false
        v = double{0.0};
        ec = v.try_convert(result);
        REQUIRE(ec == 0);
        REQUIRE(result == false);
        
        // double(3.14) -> true
        v = double{3.14};
        ec = v.try_convert(result);
        REQUIRE(ec == 0);
        REQUIRE(result == true);
        
        // null -> failure
        v.reset();
        ec = v.try_convert(result);
        REQUIRE(ec != 0);
        
        // string -> failure (not convertible)
        v = memepp::string{"text"};
        ec = v.try_convert(result);
        REQUIRE(ec != 0);
    }

    SECTION("reset") {
        memepp::variant v = int64_t{100};
        REQUIRE(v.is_type(memepp::meta::typid::int64));
        
        v.reset();
        REQUIRE(v.is_null());
        REQUIRE(v.is_type(memepp::meta::typid::null));
        REQUIRE(v.type_id() == memepp::meta::typid::null);
        
        // try_get should fail after reset
        int64_t i64;
        mgec_t ec = v.try_get(i64);
        REQUIRE(ec != 0);
    }
}


TEST_CASE("memepp::string_view keeps source data pointer for small string", "[string_view]")
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

TEST_CASE("memepp::string storage identity and takeover", "[string]")
{

	SECTION("medium string passes head/tail memory check") {
		std::string std02 = "01234567890123456789012345678901234567890123456789";

		memepp::string s02;
		auto func02 = [&](const memepp::string& _s)
		{
			s02 = _s;
		};
		func02(mm_from(std02));
		REQUIRE(s02 == std02);
		REQUIRE(s02.storage_type() == memepp::string_storage_t::medium);
		REQUIRE(MemeString_checkHeadTailMemory(
			(MemeString_Const_t)memepp::to_pointer(s02.native_handle())) == 1);
	}

	SECTION("c_format builds path text") {
		auto path = memepp::c_format(1024, "%s/%s_%d_%s",
			"Z:/path", "lt", 0, "db");
		REQUIRE(path == "Z:/path/lt_0_db");
		REQUIRE(path.size() == 15);
	}

	SECTION("moved string takes new storage") {
		// `originals` outlives the comparison loop, so the raw pointers captured into
		// `cstrs` stay valid for the whole comparison. Capturing `c_str()` of a
		// per-iteration local would leave `cstrs` holding dangling pointers.
		constexpr int count = 128;
		memepp::string originals[count];
		std::vector<memepp::string> mstrs;
		std::vector<const char*>    cstrs;

		for (auto index = 0; index < count; ++index)
		{
			originals[index] = "0";
			cstrs.emplace_back(originals[index].c_str());
			mstrs.emplace_back(std::move(originals[index]));
		}

		for (auto index = 0; index < mstrs.size(); ++index)
		{
			REQUIRE((void*)mstrs[index].c_str() != (void*)cstrs[index]);
		}
	}

	SECTION("copied string takes new storage") {
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
	}

	SECTION("large string keeps c_str identity") {
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
	}

	SECTION("c_format INT64_MAX pair") {
		int64_t v = (long long)(INT64_MAX);
		auto result = memepp::c_format(-1, "%lld - %lld", (long long)v, (long long)v);
		REQUIRE(result.size() == 41);
		REQUIRE(result == "9223372036854775807 - 9223372036854775807");
	}

	SECTION("string_view adopts temporary large storage") {
		// Binding a string_view to a temporary large-storage string must take over the
		// shared (reference-counted) storage, so the view stays valid after the temporary dies.
		memepp::string_view sv = memepp::string{}.to_large();
		REQUIRE(sv.empty());
		REQUIRE(sv.size() == 0);
		REQUIRE(sv.storage_type() == memepp::string_storage_t::large);
	}

	SECTION("takeover with free() deleter") {
		memepp::string s01_01;
		char* p01_01 = (char*)malloc(16);
		REQUIRE(p01_01 != nullptr);
		memcpy(p01_01, "123456789ABCDEF", 15);
		p01_01[15] = '\0';

		s01_01 = memepp::convert::takeover(p01_01, -1, free);
		REQUIRE(s01_01 == "123456789ABCDEF");
	}

	SECTION("takeover with delete[] deleter") {
		memepp::string s01_01;
		char* p01_01 = new char[16];
		memcpy(p01_01, "FEDCBA987654321", 15);
		p01_01[15] = '\0';
		s01_01 = memepp::convert::takeover(p01_01, -1, [](void* _p) { delete[] (reinterpret_cast<char*>(_p)); });
		REQUIRE(s01_01 == "FEDCBA987654321");
	}

	SECTION("takeover long buffer with free()") {
		const char cz01_02[] =
			"01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";

		char* p01_02 = (char*)malloc(1024);
		REQUIRE(p01_02 != nullptr);

		memcpy(p01_02, cz01_02, sizeof(cz01_02));
		auto s01_01 = memepp::convert::takeover(p01_02, -1, free);
		REQUIRE(s01_01 == cz01_02);
	}

	SECTION("takeover destructor call count - short string") {
		// Case A: short string -> small/medium storage, destructor called exactly once (synchronously)
		int dtor_count = 0;
		char* p = (char*)malloc(32);
		REQUIRE(p != nullptr);
		memcpy(p, "short_takeover_chk", sizeof("short_takeover_chk"));
		auto s = memepp::convert::takeover(p, -1,
			[&dtor_count](void* _p) { ++dtor_count; free(_p); });
		REQUIRE(s == "short_takeover_chk");
		REQUIRE(s.storage_type() != memepp::string_storage_t::user); // small or medium
		REQUIRE(dtor_count == 1); // short branch calls _fn synchronously
	}

	SECTION("takeover destructor call count - long string") {
		// Case B: long string -> user storage, destructor called exactly once after reset
		int dtor_count = 0;
		char* p = (char*)malloc(1024);
		REQUIRE(p != nullptr);
		memset(p, 'A', 1023);
		p[1023] = '\0';
		auto s = memepp::convert::takeover(p, -1,
			[&dtor_count](void* _p) { ++dtor_count; free(_p); });
		REQUIRE(s.size() == 1023);
		REQUIRE(s.storage_type() == memepp::string_storage_t::user);
		REQUIRE(dtor_count == 0); // not yet destroyed
		s.reset();
		REQUIRE(dtor_count == 1); // exactly once after reset
	}

	SECTION("takeover double-reset guard") {
		// Case C: double-reset guard - second reset is no-op, dtor still 1
		int dtor_count = 0;
		char* p = (char*)malloc(1024);
		REQUIRE(p != nullptr);
		memset(p, 'B', 1023);
		p[1023] = '\0';
		auto s = memepp::convert::takeover(p, -1,
			[&dtor_count](void* _p) { ++dtor_count; free(_p); });
		REQUIRE(s.storage_type() == memepp::string_storage_t::user);
		s.reset();
		s.reset(); // second reset must be no-op
		REQUIRE(dtor_count == 1);
	}
}
