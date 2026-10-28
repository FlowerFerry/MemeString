
#include <catch2/catch.hpp>

#include <memepp/variable_buffer.hpp>
#include <vector>

// Helpers: replay a chain of resize(size, fill) calls so each SECTION is
// self-contained, and derive the expected bytes from the same chain instead
// of hand-written literals.
//
// resize(N, v) semantics:
//   - N < current size  -> truncate to N (fill ignored)
//   - N > current size  -> keep existing bytes, append (N - size) copies of v
//
// The step list is a std::vector, NOT a std::initializer_list: copying an
// initializer_list does not extend the lifetime of its backing array before
// C++17, so naming one in a variable leaves a dangling pointer that reads as
// garbage sizes (this crashed the cxx11/cxx14 builds with bad_alloc).
using resize_step = std::pair<memepp::variable_buffer::size_type, uint8_t>;
using resize_steps = std::vector<resize_step>;

static memepp::variable_buffer build_buffer(const resize_steps& steps)
{
	memepp::variable_buffer b;
	for (auto& s : steps)
		b.resize(s.first, s.second);
	return b;
}

static std::vector<uint8_t> build_expected(const resize_steps& steps)
{
	std::vector<uint8_t> v;
	for (auto& s : steps) {
		if (s.first > v.size())
			v.insert(v.end(), s.first - v.size(), s.second);
		else
			v.resize(s.first);
	}
	return v;
}

static bool data_matches(const memepp::variable_buffer& _b, const std::vector<uint8_t>& _expected)
{
	return _b.size() == _expected.size()
		&& memcmp(_b.data(), _expected.data(), _b.size()) == 0;
}

TEST_CASE("memepp::variable_buffer::resize", "[variable_buffer]")
{
	const auto small_limit =
		static_cast<memepp::variable_buffer::size_type>(MemeStringOption_getStorageSmallLimit());

	SECTION("sizeof variable_buffer == MMSTR__OBJ_SIZE")
	{
		REQUIRE(sizeof(memepp::variable_buffer) == MMSTR__OBJ_SIZE);
	}

	SECTION("resize(0) on empty buffer")
	{
		memepp::variable_buffer b;
		b.resize(0);

		REQUIRE(b.storage_type() == memepp::buffer_storage_t::small);
		REQUIRE(b.size() == 0);
		REQUIRE(b.empty());
		REQUIRE(b.capacity() == small_limit);
		REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b.native_handle())) == 1);
	}

	SECTION("resize to small limit with fill")
	{
		resize_steps steps = {
			{ small_limit, 0x01 }
		};
		auto b = build_buffer(steps);
		auto expected = build_expected(steps);

		REQUIRE(b.storage_type() == memepp::buffer_storage_t::small);
		REQUIRE(b.size() == small_limit);
		REQUIRE(data_matches(b, expected));
		REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b.native_handle())) == 1);
	}

	SECTION("resize down within small")
	{
		resize_steps steps = {
			{ small_limit, 0x01 },
			{ 10, 0x00 }
		};
		auto b = build_buffer(steps);
		auto expected = build_expected(steps);

		REQUIRE(b.storage_type() == memepp::buffer_storage_t::small);
		REQUIRE(b.size() == 10);
		REQUIRE(data_matches(b, expected));
		REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b.native_handle())) == 1);
	}

	SECTION("resize to 20 with fill (small on x64, medium on x86)")
	{
		resize_steps steps = {
			{ small_limit, 0x01 },
			{ 10, 0x00 },
			{ 20, 0x02 }
		};
		auto b = build_buffer(steps);
		auto expected = build_expected(steps);

#if INTPTR_MAX == INT64_MAX
		REQUIRE(b.storage_type() == memepp::buffer_storage_t::small);
#else
		REQUIRE(b.storage_type() == memepp::buffer_storage_t::medium);
#endif
		REQUIRE(b.size() == 20);
		REQUIRE(data_matches(b, expected));
		REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b.native_handle())) == 1);
		REQUIRE(MemeString_checkHeadTailMemory(
			(MemeString_Const_t)memepp::to_pointer(b.native_handle())) == 1);
	}

	SECTION("resize down to 10 keeps truncation semantics")
	{
		resize_steps steps = {
			{ small_limit, 0x01 },
			{ 10, 0x00 },
			{ 20, 0x02 },
			{ 10, 0x02 }
		};
		auto b = build_buffer(steps);
		auto expected = build_expected(steps);

#if INTPTR_MAX == INT64_MAX
		REQUIRE(b.storage_type() == memepp::buffer_storage_t::small);
#else
		REQUIRE(b.storage_type() == memepp::buffer_storage_t::medium);
#endif
		REQUIRE(b.size() == 10);
		REQUIRE(data_matches(b, expected));
		REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b.native_handle())) == 1);
		REQUIRE(MemeString_checkHeadTailMemory(
			(MemeString_Const_t)memepp::to_pointer(b.native_handle())) == 1);
	}

	SECTION("resize to 23 with fill promotes to medium")
	{
		resize_steps steps = {
			{ small_limit, 0x01 },
			{ 10, 0x00 },
			{ 20, 0x02 },
			{ 10, 0x02 },
			{ 23, 0x03 }
		};
		auto b = build_buffer(steps);
		auto expected = build_expected(steps);

		REQUIRE(b.storage_type() == memepp::buffer_storage_t::medium);
		REQUIRE(b.size() == 23);
		REQUIRE(data_matches(b, expected));
		REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b.native_handle())) == 1);
		REQUIRE(MemeString_checkHeadTailMemory(
			(MemeString_Const_t)memepp::to_pointer(b.native_handle())) == 1);
	}

	SECTION("resize to 42 with fill (medium growth)")
	{
		resize_steps steps = {
			{ small_limit, 0x01 },
			{ 10, 0x00 },
			{ 20, 0x02 },
			{ 10, 0x02 },
			{ 23, 0x03 },
			{ 42, 0x04 }
		};
		auto b = build_buffer(steps);
		auto expected = build_expected(steps);

		REQUIRE(b.storage_type() == memepp::buffer_storage_t::medium);
		REQUIRE(b.size() == 42);
		REQUIRE(data_matches(b, expected));
		REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b.native_handle())) == 1);
		REQUIRE(MemeString_checkHeadTailMemory(
			(MemeString_Const_t)memepp::to_pointer(b.native_handle())) == 1);
	}

	SECTION("resize to 128 with fill (medium growth)")
	{
		resize_steps steps = {
			{ small_limit, 0x01 },
			{ 10, 0x00 },
			{ 20, 0x02 },
			{ 10, 0x02 },
			{ 23, 0x03 },
			{ 42, 0x04 },
			{ 128, 0x05 }
		};
		auto b = build_buffer(steps);
		auto expected = build_expected(steps);

		REQUIRE(b.storage_type() == memepp::buffer_storage_t::medium);
		REQUIRE(b.size() == 128);
		REQUIRE(data_matches(b, expected));
		REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b.native_handle())) == 1);
		REQUIRE(MemeString_checkHeadTailMemory(
			(MemeString_Const_t)memepp::to_pointer(b.native_handle())) == 1);
	}

	SECTION("resize down to 1 keeps medium storage")
	{
		resize_steps steps = {
			{ small_limit, 0x01 },
			{ 10, 0x00 },
			{ 20, 0x02 },
			{ 10, 0x02 },
			{ 23, 0x03 },
			{ 42, 0x04 },
			{ 128, 0x05 },
			{ 1, 0x06 }
		};
		auto b = build_buffer(steps);
		auto expected = build_expected(steps);

		REQUIRE(b.storage_type() == memepp::buffer_storage_t::medium);
		REQUIRE(b.size() == 1);
		REQUIRE(data_matches(b, expected));
		REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b.native_handle())) == 1);
		REQUIRE(MemeString_checkHeadTailMemory(
			(MemeString_Const_t)memepp::to_pointer(b.native_handle())) == 1);
	}

	SECTION("resize to 256 with fill (medium growth)")
	{
		resize_steps steps = {
			{ small_limit, 0x01 },
			{ 10, 0x00 },
			{ 20, 0x02 },
			{ 10, 0x02 },
			{ 23, 0x03 },
			{ 42, 0x04 },
			{ 128, 0x05 },
			{ 1, 0x06 },
			{ 256, 0x07 }
		};
		auto b = build_buffer(steps);
		auto expected = build_expected(steps);

		REQUIRE(b.storage_type() == memepp::buffer_storage_t::medium);
		REQUIRE(b.size() == 256);
		REQUIRE(data_matches(b, expected));
		REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b.native_handle())) == 1);
		REQUIRE(MemeString_checkHeadTailMemory(
			(MemeString_Const_t)memepp::to_pointer(b.native_handle())) == 1);
	}

	SECTION("resize to 65536 with fill (medium growth)")
	{
		resize_steps steps = {
			{ small_limit, 0x01 },
			{ 10, 0x00 },
			{ 20, 0x02 },
			{ 10, 0x02 },
			{ 23, 0x03 },
			{ 42, 0x04 },
			{ 128, 0x05 },
			{ 1, 0x06 },
			{ 256, 0x07 },
			{ 65536, 0x08 }
		};
		auto b = build_buffer(steps);

		REQUIRE(b.storage_type() == memepp::buffer_storage_t::medium);
		REQUIRE(b.size() == 65536);
		REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b.native_handle())) == 1);
		REQUIRE(MemeString_checkHeadTailMemory(
			(MemeString_Const_t)memepp::to_pointer(b.native_handle())) == 1);
	}

	SECTION("resize_and_overwrite(256)")
	{
		memepp::variable_buffer b;
		b.resize_and_overwrite(256);

		REQUIRE(b.storage_type() == memepp::buffer_storage_t::medium);
		REQUIRE(b.size() == 256);
		REQUIRE(MemeVariableBuffer_capacityCorrectness(memepp::to_pointer(b.native_handle())) == 1);
		REQUIRE(MemeString_checkHeadTailMemory(
			(MemeString_Const_t)memepp::to_pointer(b.native_handle())) == 1);
	}

	SECTION("resize_and_overwrite then memcpy prefix")
	{
		memepp::variable_buffer b;
		b.resize_and_overwrite(256);

		const uint8_t pattern[16] = {
			0x01, 0x03, 0x05, 0x07, 0x09, 0x0B, 0x0D, 0x0F,
			0x11, 0x13, 0x15, 0x17, 0x19, 0x1B, 0x1D, 0x1F
		};
		memcpy(b.data(), pattern, sizeof(pattern));

		REQUIRE(0 == memcmp(b.data(), pattern, sizeof(pattern)));
		REQUIRE(MemeString_checkHeadTailMemory(
			(MemeString_Const_t)memepp::to_pointer(b.native_handle())) == 1);
	}
}
