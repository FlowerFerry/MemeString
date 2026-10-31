#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/string_builder.hpp>
#include <memepp/buffer.hpp>
#include <memepp/variant.hpp>
#include <memepp/varts.hpp>

#include <meme/string.h>

#include <thread>
#include <vector>
#include <atomic>
#include <cstring>

// ============================================================================
// Thread safety tests for concurrent copy operations.
//
// These tests verify the thread safety guarantees documented in AGENTS.md:
// - Concurrent copy from same source is thread-safe
// - Large/user storage uses atomic refcount
// - Source remains valid after all copies are destroyed
// - User storage destructor is called exactly once
// ============================================================================

namespace {

// Long payload to force large storage (> medium limit of 200 bytes)
const char kLongPayload[] =
    "012345678901234567890123456789012345678901234567890123456789"
    "012345678901234567890123456789012345678901234567890123456789"
    "012345678901234567890123456789012345678901234567890123456789"
    "012345678901234567890123456789012345678901234567890123456789"
    "012345678901234567890123456789012345678901234567890123456789";

const size_t kLongPayloadLen = sizeof(kLongPayload) - 1;

// User storage payload
struct UserPayload {
    char* data;
    size_t size;
};

void user_payload_destructor(void* p) {
    auto* payload = static_cast<UserPayload*>(p);
    delete[] payload->data;
    delete payload;
}

const char* user_payload_data(const void* p) {
    return static_cast<const UserPayload*>(p)->data;
}

size_t user_payload_size(const void* p) {
    return static_cast<const UserPayload*>(p)->size;
}

} // anonymous namespace

// ============================================================================
// string tests
// ============================================================================

// Global counter for tracking destructor calls in user storage tests
static std::atomic<int> g_user_destroy_count{0};

static void user_string_destructor(void* p) {
    g_user_destroy_count++;
    user_payload_destructor(p);
}

TEST_CASE("memepp::string concurrent copy — large storage", "[thread_safety][string]")
{
    // Create shared source with large storage
    memepp::string source{kLongPayload, static_cast<mmint_t>(kLongPayloadLen),
                          memepp::string_storage_t::large};
    REQUIRE(source.storage_type() == memepp::string_storage_t::large);

    constexpr int kThreads = 8;
    constexpr int kIterations = 100;
    std::vector<std::thread> threads;
    std::atomic<int> success_count{0};

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&]() {
            for (int i = 0; i < kIterations; ++i) {
                // Concurrent copy construction
                memepp::string copy1{source};
                REQUIRE(copy1 == source);
                REQUIRE(copy1.size() == source.size());

                // Concurrent copy assignment
                memepp::string copy2;
                copy2 = source;
                REQUIRE(copy2 == source);
                REQUIRE(copy2.size() == source.size());

                success_count++;
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // Verify source is still valid
    REQUIRE(source.size() == static_cast<mmint_t>(kLongPayloadLen));
    REQUIRE(source.storage_type() == memepp::string_storage_t::large);
    REQUIRE(success_count == kThreads * kIterations);
}

TEST_CASE("memepp::string concurrent copy — user storage destructor", "[thread_safety][string]")
{
    g_user_destroy_count = 0;

    {
        // Create user storage source using C API
        auto* payload = new UserPayload;
        payload->data = new char[kLongPayloadLen];
        std::memcpy(payload->data, kLongPayload, kLongPayloadLen);
        payload->size = kLongPayloadLen;

        mmstrstk_t raw;
        REQUIRE(mmstrstk_init_by_user_v0(&raw, MMSTR__OBJ_SIZE, payload,
            user_string_destructor, user_payload_data, user_payload_size) == 0);

        memepp::string source{std::move(raw)};
        REQUIRE(source.storage_type() == memepp::string_storage_t::user);

        // Concurrent copy
        constexpr int kThreads = 8;
        constexpr int kIterations = 100;
        std::vector<std::thread> threads;

        for (int t = 0; t < kThreads; ++t) {
            threads.emplace_back([&]() {
                for (int i = 0; i < kIterations; ++i) {
                    memepp::string copy{source};
                    REQUIRE(copy == source);
                    // copy destructor should not trigger user destructor
                    // (refcount > 0)
                }
            });
        }

        for (auto& t : threads) {
            t.join();
        }

        // Source should still be valid
        REQUIRE(source.storage_type() == memepp::string_storage_t::user);
        REQUIRE(source.size() == static_cast<mmint_t>(kLongPayloadLen));

        // source destructor will be called here
    }

    // Destructor should be called exactly once (when source is destroyed)
    REQUIRE(g_user_destroy_count == 1);
}

// ============================================================================
// buffer tests
// ============================================================================

TEST_CASE("memepp::buffer concurrent copy — large storage", "[thread_safety][buffer]")
{
    // Create shared source with large storage
    const uint8_t data[] = {0xDE, 0xAD, 0xBE, 0xEF, 0x01, 0x02, 0x03, 0x04};
    memepp::buffer source{data, sizeof(data), memepp::buffer_storage_t::large};
    REQUIRE(source.storage_type() == memepp::buffer_storage_t::large);

    constexpr int kThreads = 8;
    constexpr int kIterations = 100;
    std::vector<std::thread> threads;
    std::atomic<int> success_count{0};

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&]() {
            for (int i = 0; i < kIterations; ++i) {
                // Concurrent copy construction
                memepp::buffer copy1{source};
                REQUIRE(copy1 == source);
                REQUIRE(copy1.size() == source.size());

                // Concurrent copy assignment
                memepp::buffer copy2;
                copy2 = source;
                REQUIRE(copy2 == source);
                REQUIRE(copy2.size() == source.size());

                success_count++;
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // Verify source is still valid
    REQUIRE(source.size() == sizeof(data));
    REQUIRE(source.storage_type() == memepp::buffer_storage_t::large);
    REQUIRE(success_count == kThreads * kIterations);
}

// Note: memepp::buffer does not support user storage type in the current API
// Only large storage is tested for buffer

// ============================================================================
// variant tests
// ============================================================================

TEST_CASE("memepp::variant concurrent copy — string payload", "[thread_safety][variant]")
{
    // Create variant with string payload
    memepp::string str{kLongPayload, static_cast<mmint_t>(kLongPayloadLen),
                       memepp::string_storage_t::large};
    memepp::variant source{str};
    REQUIRE(source.is_type(memepp::meta::typid::string));

    constexpr int kThreads = 8;
    constexpr int kIterations = 100;
    std::vector<std::thread> threads;
    std::atomic<int> success_count{0};

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&]() {
            for (int i = 0; i < kIterations; ++i) {
                // Concurrent copy construction
                memepp::variant copy1{source};
                REQUIRE(copy1.is_type(memepp::meta::typid::string));
                REQUIRE(copy1.get_or<memepp::string>() == source.get_or<memepp::string>());

                // Concurrent copy assignment
                memepp::variant copy2;
                copy2 = source;
                REQUIRE(copy2.is_type(memepp::meta::typid::string));
                REQUIRE(copy2.get_or<memepp::string>() == source.get_or<memepp::string>());

                success_count++;
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // Verify source is still valid
    REQUIRE(source.is_type(memepp::meta::typid::string));
    REQUIRE(source.get_or<memepp::string>().size() == static_cast<mmint_t>(kLongPayloadLen));
    REQUIRE(success_count == kThreads * kIterations);
}

TEST_CASE("memepp::variant concurrent copy — buffer payload", "[thread_safety][variant]")
{
    // Create variant with buffer payload
    const uint8_t data[] = {0xDE, 0xAD, 0xBE, 0xEF, 0x01, 0x02, 0x03, 0x04};
    memepp::buffer buf{data, sizeof(data), memepp::buffer_storage_t::large};
    memepp::variant source{buf};
    REQUIRE(source.is_type(memepp::meta::typid::buffer));

    constexpr int kThreads = 8;
    constexpr int kIterations = 100;
    std::vector<std::thread> threads;
    std::atomic<int> success_count{0};

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&]() {
            for (int i = 0; i < kIterations; ++i) {
                // Concurrent copy construction
                memepp::variant copy1{source};
                REQUIRE(copy1.is_type(memepp::meta::typid::buffer));
                REQUIRE(copy1.get_or<memepp::buffer>() == source.get_or<memepp::buffer>());

                // Concurrent copy assignment
                memepp::variant copy2;
                copy2 = source;
                REQUIRE(copy2.is_type(memepp::meta::typid::buffer));
                REQUIRE(copy2.get_or<memepp::buffer>() == source.get_or<memepp::buffer>());

                success_count++;
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // Verify source is still valid
    REQUIRE(source.is_type(memepp::meta::typid::buffer));
    REQUIRE(source.get_or<memepp::buffer>().size() == sizeof(data));
    REQUIRE(success_count == kThreads * kIterations);
}

// ============================================================================
// varts tests
// ============================================================================

TEST_CASE("memepp::varts concurrent copy", "[thread_safety][varts]")
{
    // Create varts with string payload
    memepp::string str{kLongPayload, static_cast<mmint_t>(kLongPayloadLen),
                       memepp::string_storage_t::large};
    memepp::variant var{str};
    memepp::varts source{var, 1700000000LL, static_cast<int8_t>(42)};

    REQUIRE(source.timestamp() == 1700000000LL);
    REQUIRE(source.userdata() == 42);
    REQUIRE(source.var().is_type(memepp::meta::typid::string));

    constexpr int kThreads = 8;
    constexpr int kIterations = 100;
    std::vector<std::thread> threads;
    std::atomic<int> success_count{0};

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&]() {
            for (int i = 0; i < kIterations; ++i) {
                // Concurrent copy construction
                memepp::varts copy1{source};
                REQUIRE(copy1.timestamp() == source.timestamp());
                REQUIRE(copy1.userdata() == source.userdata());
                REQUIRE(copy1.var().is_type(memepp::meta::typid::string));
                REQUIRE(copy1.var().get_or<memepp::string>() ==
                        source.var().get_or<memepp::string>());

                // Concurrent copy assignment
                memepp::varts copy2;
                copy2 = source;
                REQUIRE(copy2.timestamp() == source.timestamp());
                REQUIRE(copy2.userdata() == source.userdata());
                REQUIRE(copy2.var().is_type(memepp::meta::typid::string));
                REQUIRE(copy2.var().get_or<memepp::string>() ==
                        source.var().get_or<memepp::string>());

                success_count++;
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // Verify source is still valid
    REQUIRE(source.timestamp() == 1700000000LL);
    REQUIRE(source.userdata() == 42);
    REQUIRE(source.var().is_type(memepp::meta::typid::string));
    REQUIRE(source.var().get_or<memepp::string>().size() == static_cast<mmint_t>(kLongPayloadLen));
    REQUIRE(success_count == kThreads * kIterations);
}

// ============================================================================
// string_builder tests
// ============================================================================

TEST_CASE("memepp::string_builder concurrent generate — const method",
          "[thread_safety][string_builder]")
{
    // Several parts so generate() has real work to do.
    memepp::string_builder builder;
    builder += "part_one_";
    builder += "part_two_";
    builder += "part_three_";
    builder += "part_four_";

    const std::string expected = "part_one_part_two_part_three_part_four_";
    REQUIRE(builder.generate() == memepp::string{ expected.c_str() });

    constexpr int kThreads    = 8;
    constexpr int kIterations = 100;

    std::vector<std::thread> threads;
    std::atomic<int>  success_count{ 0 };
    std::atomic<bool> mismatched{ false };

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&]() {
            for (int i = 0; i < kIterations; ++i) {
                // generate() is const: no thread writes to the builder, so this
                // exercises concurrent const reads of the shared parts vector.
                memepp::string out = builder.generate();

                if (out != memepp::string{ expected.c_str() } ||
                    out.size() != static_cast<mmint_t>(expected.size()))
                {
                    mismatched = true;
                }
                success_count++;
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    REQUIRE(mismatched == false);
    REQUIRE(success_count == kThreads * kIterations);

    // The builder must be unchanged and still usable.
    REQUIRE(builder.generate() == memepp::string{ expected.c_str() });
    REQUIRE(builder.empty() == false);
}
