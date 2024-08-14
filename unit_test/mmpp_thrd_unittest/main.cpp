
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <thread>
#include <atomic>

#include <memepp/string.hpp>

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
		std::atomic_bool stop{ false };
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
			u8"堆中数组的数组越界条件"
			u8"未初始化的指针"
			u8"指向您分配但随后被释放的堆的指针。所以现在它指向一个你可以写入的位置，但现在你不能。"
			u8"本身已损坏的指针，例如被堆栈超出范围的数组覆盖的本地指针。"
			u8"堆中数组的数组越界条件";

		std::vector<std::thread> thrs_;
		std::atomic_bool stop{ false };
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
				REQUIRE((void*)dst == (void*)src);
			}));
		}

		std::this_thread::sleep_for(std::chrono::seconds(5));

		stop = true;

		for (auto& thr : thrs_) {
			thr.join();
		}

	} while (0);

}
