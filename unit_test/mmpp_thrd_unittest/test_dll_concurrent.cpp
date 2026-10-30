#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/dll.hpp>

#include <memepp/convert/std/string.hpp>

#include <meme/string.h>

#include <algorithm>
#include <atomic>
#include <cstring>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

// ============================================================================
// Concurrent export_into_dll / import_from_dll.
//
// AGENTS.md "Thread Safety / Concurrent Copy from Same Source" claims that
// several threads may copy from the same source object without external
// synchronization, because large/user storage copies are an atomic refcount
// increment plus a read of immutable stack bytes.  Nothing exercised that claim
// until now.
//
// Both halves are stressed in parallel:
//   export_into_dll(const&)  -> atomic refcount increment, NO data copy
//   import_from_dll(const&)  -> deep copy, new allocation every time
//
// The produced objects are then released in a shuffled order so no thread can
// rely on any particular interleaving, and the shared source must still be
// intact afterwards.
// ============================================================================

namespace {

// Forces large storage: well above the medium limit (200 bytes on 64-bit).
const char kLargePayload[] =
    "concurrent_payload_0123456789012345678901234567890123456789"
    "012345678901234567890123456789012345678901234567890123456789"
    "012345678901234567890123456789012345678901234567890123456789"
    "012345678901234567890123456789012345678901234567890123456789"
    "012345678901234567890123456789012345678901234567890123456789";

constexpr int kLargeLen = static_cast<int>(sizeof(kLargePayload) - 1);

// One round of N threads hammering a shared source with alternating
// export/import, collecting the results for out-of-order release.
struct concurrent_result
{
    std::vector<mmstrstk_t>       exported;
    std::vector<memepp::string>   imported;
    std::atomic<int>              exports{ 0 };
    std::atomic<int>              imports{ 0 };
    std::atomic<bool>             mismatched{ false };
};

} // anonymous namespace

TEST_CASE("memepp::string DLL concurrent export/import — large storage", "[dll][string][concurrent]")
{
    memepp::string shared{ kLargePayload, kLargeLen, memepp::string_storage_t::large };
    REQUIRE(shared.storage_type() == memepp::string_storage_t::large);

    const void* original_data = shared.data();
    const auto  original_size = shared.size();

    constexpr int kThreads    = 4;
    constexpr int kIterations = 500;

    concurrent_result result;
    std::mutex collect_mutex;

    auto worker = [&] {
        std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<int> coin(0, 1);

        for (int i = 0; i < kIterations; ++i) {
            if (coin(gen)) {
                // Shared path: refcount only, exported bytes ARE the source bytes.
                auto stk = memepp::export_into_dll<mmstrstk_t>(shared, MMSTR__OBJ_SIZE);
                if (MemeString_byteData(memepp::to_pointer(stk)) != original_data)
                    result.mismatched = true;

                std::lock_guard<std::mutex> lock(collect_mutex);
                result.exported.push_back(stk);
                ++result.exports;
            } else {
                // Deep-copy path: new allocation, identical content.
                auto imported = memepp::import_from_dll<memepp::string>(
                    shared.native_handle(), MMSTR__OBJ_SIZE);

                if (imported.size() != original_size ||
                    std::memcmp(imported.data(), original_data, original_size) != 0 ||
                    imported.data() == original_data)
                    result.mismatched = true;

                std::lock_guard<std::mutex> lock(collect_mutex);
                result.imported.push_back(std::move(imported));
                ++result.imports;
            }
        }
    };

    std::vector<std::thread> threads;
    threads.reserve(kThreads);
    for (int i = 0; i < kThreads; ++i)
        threads.emplace_back(worker);
    for (auto& t : threads)
        t.join();

    REQUIRE(result.mismatched == false);
    REQUIRE(result.exports > 0);
    REQUIRE(result.imports > 0);
    REQUIRE(static_cast<int>(result.exported.size()) == result.exports);
    REQUIRE(static_cast<int>(result.imported.size()) == result.imports);

    // The source survived simultaneous copies of both kinds.
    REQUIRE(shared.storage_type() == memepp::string_storage_t::large);
    REQUIRE(shared.data() == original_data);
    REQUIRE(std::memcmp(shared.data(), original_data, original_size) == 0);

    // Drop the shared references in an arbitrary order; every one of them
    // decrements the same refcount, so order must not matter.
    std::shuffle(result.exported.begin(), result.exported.end(),
                 std::mt19937{ std::random_device{}() });
    for (auto& stk : result.exported)
        mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE);
    result.exported.clear();

    // Still alive: our own reference was never released.
    REQUIRE(shared.storage_type() == memepp::string_storage_t::large);
    REQUIRE(std::memcmp(shared.data(), original_data, original_size) == 0);

    // Deep copies are independent of the source and of each other.
    for (const auto& s : result.imported) {
        REQUIRE(s.size() == original_size);
        REQUIRE(s.data() != original_data);
    }
    result.imported.clear();

    REQUIRE(shared == kLargePayload);
}

TEST_CASE("memepp::string DLL concurrent export/import — user storage", "[dll][string][concurrent]")
{
    // mm_from(std::string&&) only takes over the buffer as "user" storage above
    // medium_limit + MMSTR__OBJ_SIZE (224 bytes on this build), and only via
    // the rvalue overload.
    std::string payload;
    for (int i = 0; i < 30; ++i)
        payload.append("0123456789");   // 300 bytes
    const std::string expected = payload;

    auto shared = mm_from(std::move(payload));
    REQUIRE(shared.storage_type() == memepp::string_storage_t::user);

    const void* original_data = shared.data();
    const auto  original_size = shared.size();

    constexpr int kThreads    = 4;
    constexpr int kIterations = 500;

    concurrent_result result;
    std::mutex collect_mutex;

    auto worker = [&] {
        std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<int> coin(0, 1);

        for (int i = 0; i < kIterations; ++i) {
            if (coin(gen)) {
                // Sharing keeps "user" and the same pointer; a copy would
                // have downgraded it to "large".
                auto stk = memepp::export_into_dll<mmstrstk_t>(shared, MMSTR__OBJ_SIZE);
                if (MemeString_storageType(memepp::to_pointer(stk)) != MemeString_StorageType_user ||
                    MemeString_byteData(memepp::to_pointer(stk)) != original_data)
                    result.mismatched = true;

                std::lock_guard<std::mutex> lock(collect_mutex);
                result.exported.push_back(stk);
                ++result.exports;
            } else {
                // Import always deep-copies, and a user source becomes large.
                auto imported = memepp::import_from_dll<memepp::string>(
                    shared.native_handle(), MMSTR__OBJ_SIZE);

                if (imported.size() != original_size ||
                    std::memcmp(imported.data(), original_data, original_size) != 0 ||
                    imported.storage_type() != memepp::string_storage_t::large)
                    result.mismatched = true;

                std::lock_guard<std::mutex> lock(collect_mutex);
                result.imported.push_back(std::move(imported));
                ++result.imports;
            }
        }
    };

    std::vector<std::thread> threads;
    threads.reserve(kThreads);
    for (int i = 0; i < kThreads; ++i)
        threads.emplace_back(worker);
    for (auto& t : threads)
        t.join();

    REQUIRE(result.mismatched == false);
    REQUIRE(result.exports > 0);
    REQUIRE(result.imports > 0);

    REQUIRE(shared.storage_type() == memepp::string_storage_t::user);
    REQUIRE(shared.data() == original_data);
    REQUIRE(std::memcmp(shared.data(), original_data, original_size) == 0);

    std::shuffle(result.exported.begin(), result.exported.end(),
                 std::mt19937{ std::random_device{}() });
    for (auto& stk : result.exported)
        mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE);
    result.exported.clear();

    // The user-storage object and its payload are still owned by us.
    REQUIRE(shared.storage_type() == memepp::string_storage_t::user);
    REQUIRE(std::memcmp(shared.data(), original_data, original_size) == 0);

    result.imported.clear();

    REQUIRE(shared == memepp::string{ expected.data(), static_cast<mmint_t>(expected.size()) });
}
