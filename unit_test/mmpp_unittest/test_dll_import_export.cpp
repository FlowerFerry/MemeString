#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/buffer.hpp>
#include <memepp/buffer_view.hpp>
#include <memepp/variant.hpp>
#include <memepp/varts.hpp>

#include <megopp/util/scope_cleanup.h>
#include <memepp/convert/std/vector.hpp>
#include <memepp/convert/std/string.hpp>

#include <string>
#include <cstring>
#include <cstdlib>

// ============================================================================
// helpers
// ============================================================================

namespace {

// A long string that forces "user" storage when constructed via mm_from(std::string).
const char kLongString[] =
    "012345678901234567890123456789012345678901234567890123456789"
    "012345678901234567890123456789012345678901234567890123456789"
    "012345678901234567890123456789012345678901234567890123456789"
    "012345678901234567890123456789012345678901234567890123456789"
    "012345678901234567890123456789012345678901234567890123456789"
    "012345678901234567890123456789012345678901234567890123456789"
    "012345678901234567890123456789012345678901234567890123456789"
    "012345678901234567890123456789012345678901234567890123456789"
    "012345678901234567890123456789012345678901234567890123456789"
    "012345678901234567890123456789012345678901234567890123456789";

// Medium-length string that forces "medium" storage on 64-bit.
const char kMediumString[] = "01234567890123456789012345678901";

// Small string that stays "small".
const char kSmallString[] = "hello";

// Buffer data.
const uint8_t kBufData[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01, 0x02, 0x03 };
constexpr mmint_t kBufLen = sizeof(kBufData);

const uint8_t kBufLarge[] = {
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,
};

// Helper: compare C string pointer with a const char* (Catch2 == compares pointers).
#define REQUIRE_CSTR_EQ(cstr, expected) \
    REQUIRE(std::strcmp((cstr), (expected)) == 0)

// --- user-storage payload with an observable destructor ---------------------
// Used to prove that export_into_dll(const&) shares the payload by
// reference-counting instead of copying it.
int g_user_destroy_count = 0;

struct UserPayload {
    char* buf;
    int   len;
};

void user_payload_destruct(void* _p)
{
    ++g_user_destroy_count;
    UserPayload* pl = static_cast<UserPayload*>(_p);
    std::free(pl->buf);
    std::free(pl);
}

const char* user_payload_data(const void* _p)
{
    return static_cast<const UserPayload*>(_p)->buf;
}

size_t user_payload_size(const void* _p)
{
    return static_cast<size_t>(static_cast<const UserPayload*>(_p)->len);
}

} // anonymous namespace

// ============================================================================
// memepp::string — import_from_dll / export_into_dll
// ============================================================================

// ---------------------------------------------------------------------------
// string_view: a view does not own its bytes, so import and export(const&)
// must deep-copy.  Verified by mutating the backing store afterwards.
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_view DLL import — deep copies the backing store", "[dll][string_view]")
{
    char backing[16];
    std::strcpy(backing, kSmallString);

    memepp::string_view sv{ backing };

    // --- import (const&) ---
    auto imported = memepp::import_from_dll<memepp::string>(
        sv.native_handle(), MMSTR__OBJ_SIZE);
    REQUIRE(imported == kSmallString);

    // Mutating (and clobbering) the source must not reach the copy.
    std::strcpy(backing, "MUTATED");
    REQUIRE(imported == kSmallString);

    // --- import (&&): export a raw stack, then import as rvalue ---
    {
        auto raw = memepp::export_into_dll<mmstrstk_t>(imported, MMSTR__OBJ_SIZE);
        auto imported2 = memepp::import_from_dll<memepp::string>(
            std::move(raw), MMSTR__OBJ_SIZE);
        REQUIRE(imported2 == kSmallString);
    }
}

TEST_CASE("memepp::string_view DLL export — const& deep copies the backing store", "[dll][string_view]")
{
    char backing[16];
    std::strcpy(backing, kSmallString);

    memepp::string_view sv{ backing };

    auto stk = memepp::export_into_dll<mmstrstk_t>(sv, MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE); });

    REQUIRE_CSTR_EQ(MemeString_cStr(memepp::to_pointer(stk)), kSmallString);

    // A view cannot be shared across the boundary: the export owns a copy,
    // so clobbering the backing store leaves it intact.
    std::strcpy(backing, "MUTATED");
    REQUIRE_CSTR_EQ(MemeString_cStr(memepp::to_pointer(stk)), kSmallString);
}

TEST_CASE("memepp::string_view DLL export — && transfers the view", "[dll][string_view]")
{
    char backing[16];
    std::strcpy(backing, kSmallString);

    memepp::string_view sv{ backing };

    auto stk = memepp::export_into_dll<mmstrstk_t>(std::move(sv), MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE); });

    REQUIRE_CSTR_EQ(MemeString_cStr(memepp::to_pointer(stk)), kSmallString);
    // Swapped out of the source view.
    REQUIRE(sv.empty());
}

// ---------------------------------------------------------------------------
// TRUE small storage: an owned inline buffer, not a view.
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string DLL import/export — true small storage", "[dll][string]")
{
    memepp::string src{ kSmallString };
    REQUIRE(src.storage_type() == memepp::string_storage_t::small);

    // --- import (const&) ---
    auto imported = memepp::import_from_dll<memepp::string>(
        src.native_handle(), MMSTR__OBJ_SIZE);
    REQUIRE(imported == kSmallString);
    // Small storage is copied inline, so the copy has its own bytes.
    REQUIRE(imported.data() != src.data());

    // Mutating the source must not disturb the copy.
    src = "replaced";
    REQUIRE(imported == kSmallString);

    // --- export (const&) ---
    memepp::string exportee{ kSmallString };
    auto stk = memepp::export_into_dll<mmstrstk_t>(exportee, MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE); });
    REQUIRE_CSTR_EQ(MemeString_cStr(memepp::to_pointer(stk)), kSmallString);
    REQUIRE(MemeString_storageType(memepp::to_pointer(stk)) == MemeString_StorageType_small);

    // --- export (&&) ---
    memepp::string movable{ kSmallString };
    auto stk2 = memepp::export_into_dll<mmstrstk_t>(std::move(movable), MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { mmstrstk_uninit_v0(&stk2, MMSTR__OBJ_SIZE); });
    REQUIRE_CSTR_EQ(MemeString_cStr(memepp::to_pointer(stk2)), kSmallString);
    REQUIRE(movable.empty());
}

TEST_CASE("memepp::string DLL import/export — medium storage", "[dll][string]")
{
    memepp::string src{ kMediumString };
    REQUIRE(src.storage_type() == memepp::string_storage_t::medium);

    // --- import (const&) ---
    auto imported = memepp::import_from_dll<memepp::string>(
        src.native_handle(), MMSTR__OBJ_SIZE);
    REQUIRE(imported == src);
    REQUIRE(imported.storage_type() == memepp::string_storage_t::medium);

    // --- import (&&): export to raw stack, then import as rvalue ---
    {
        auto raw = memepp::export_into_dll<mmstrstk_t>(imported, MMSTR__OBJ_SIZE);
        auto imported2 = memepp::import_from_dll<memepp::string>(
            std::move(raw), MMSTR__OBJ_SIZE);
        REQUIRE(imported2 == kMediumString);
        // raw was uninit'd by import(&&)
    }

    // --- export (const&) ---
    auto stk = memepp::export_into_dll<mmstrstk_t>(imported, MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE); });
    REQUIRE_CSTR_EQ(MemeString_cStr(memepp::to_pointer(stk)), kMediumString);

    // --- export (&&) ---
    auto imported3 = memepp::import_from_dll<memepp::string>(
        src.native_handle(), MMSTR__OBJ_SIZE);
    auto stk2 = memepp::export_into_dll<mmstrstk_t>(std::move(imported3), MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { mmstrstk_uninit_v0(&stk2, MMSTR__OBJ_SIZE); });
    REQUIRE_CSTR_EQ(MemeString_cStr(memepp::to_pointer(stk2)), kMediumString);
}

TEST_CASE("memepp::string DLL import/export — user storage (deep copy verification)", "[dll][string]")
{
    auto src = mm_from(std::string{ kLongString });
    REQUIRE(src.storage_type() == memepp::string_storage_t::user);

    // --- import (const&): user -> deep copy -> becomes large ---
    auto imported = memepp::import_from_dll<memepp::string>(
        src.native_handle(), MMSTR__OBJ_SIZE);
    REQUIRE(imported == src);
    REQUIRE(imported.storage_type() == memepp::string_storage_t::large);

    // Deep copy means a NEW allocation: the data pointers must differ.
    REQUIRE(imported.data() != src.data());

    // Mutating the source must not disturb the copy.
    src = "replaced";
    REQUIRE(imported == kLongString);

    // --- import (&&): export to raw, import as rvalue ---
    {
        auto raw = memepp::export_into_dll<mmstrstk_t>(imported, MMSTR__OBJ_SIZE);
        auto imported2 = memepp::import_from_dll<memepp::string>(
            std::move(raw), MMSTR__OBJ_SIZE);
        REQUIRE(imported2 == kLongString);
        REQUIRE(imported2.storage_type() == memepp::string_storage_t::large);
        // Still a distinct allocation from the object it came from.
        REQUIRE(imported2.data() != imported.data());
    }

    // --- export (const&): large is shared, not copied ---
    const void* shared_ptr = static_cast<const void*>(imported.data());
    auto stk = memepp::export_into_dll<mmstrstk_t>(imported, MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE); });
    REQUIRE_CSTR_EQ(MemeString_cStr(memepp::to_pointer(stk)), kLongString);
    REQUIRE(MemeString_storageType(memepp::to_pointer(stk)) == MemeString_StorageType_large);

    // Shared storage: the exported stack points at the SAME bytes.
    REQUIRE(static_cast<const void*>(MemeString_byteData(memepp::to_pointer(stk))) == shared_ptr);

    // Dropping our reference must not invalidate the exported stack
    // (this is what the reference count is for).
    imported = "released";
    REQUIRE_CSTR_EQ(MemeString_cStr(memepp::to_pointer(stk)), kLongString);

    // --- content preserved after import round-trip ---
    auto rt = memepp::import_from_dll<memepp::string>(stk, MMSTR__OBJ_SIZE);
    REQUIRE(rt == kLongString);
}

// ---------------------------------------------------------------------------
// export_into_dll(const&) on USER storage: shared through the refcount, not
// copied.  A copy would downgrade the object to "large"; sharing keeps "user"
// and defers the user-object destructor until the last reference is gone.
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string DLL export — user storage shares via refcount", "[dll][string]")
{
    const char text[] = "user_payload_0123456789012345678901234567890123456789";
    const int len = static_cast<int>(std::strlen(text));

    UserPayload* pl = static_cast<UserPayload*>(std::malloc(sizeof(UserPayload)));
    // +1 for the NUL so MemeString_cStr() can read the payload as a C string.
    pl->buf = static_cast<char*>(std::malloc(static_cast<size_t>(len) + 1));
    std::memcpy(pl->buf, text, static_cast<size_t>(len));
    pl->buf[len] = '\0';
    pl->len = len;

    mmstrstk_t raw;
    REQUIRE(mmstrstk_init_by_user_v0(&raw, MMSTR__OBJ_SIZE, pl,
        user_payload_destruct, user_payload_data, user_payload_size) == 0);

    g_user_destroy_count = 0;

    memepp::string src{ std::move(raw) };
    REQUIRE(src.storage_type() == memepp::string_storage_t::user);

    const void* src_ptr = static_cast<const void*>(src.data());

    // --- export (const&) on USER storage ---
    auto stk = memepp::export_into_dll<mmstrstk_t>(src, MMSTR__OBJ_SIZE);

    // Sharing keeps "user"; a deep copy would have produced "large".
    REQUIRE(MemeString_storageType(memepp::to_pointer(stk)) == MemeString_StorageType_user);

    // Same bytes: only the refcount moved.
    REQUIRE(static_cast<const void*>(MemeString_byteData(memepp::to_pointer(stk))) == src_ptr);

    // Refcount incremented, so the user object is still alive.
    REQUIRE(g_user_destroy_count == 0);

    // Releasing our own reference must leave the exported stack usable.
    src = "released";
    REQUIRE(g_user_destroy_count == 0);
    REQUIRE_CSTR_EQ(MemeString_cStr(memepp::to_pointer(stk)), text);

    // The destructor fires exactly once, when the LAST reference goes away.
    mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE);
    REQUIRE(g_user_destroy_count == 1);
}

TEST_CASE("memepp::string DLL import/export — vector-user storage", "[dll][string]")
{
    // user storage from std::vector<uint8_t> (distinct from std::string user)
    // must exceed medium limit (~368 bytes on 64-bit) to force user storage
    std::vector<uint8_t> data;
    for (int i = 0; i < 50; ++i)
        data.insert(data.end(), kBufLarge, kBufLarge + sizeof(kBufLarge));
    auto src = mm_from(std::move(data));
    REQUIRE(src.storage_type() == memepp::string_storage_t::user);

    // import: must deep-copy user -> large
    auto imported = memepp::import_from_dll<memepp::string>(
        src.native_handle(), MMSTR__OBJ_SIZE);
    REQUIRE(imported == src);
    REQUIRE(imported.storage_type() == memepp::string_storage_t::large);

    // export & round-trip
    auto stk = memepp::export_into_dll<mmstrstk_t>(imported, MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE); });
    REQUIRE(MemeString_storageType(memepp::to_pointer(stk)) == MemeString_StorageType_large);
}

TEST_CASE("memepp::string DLL import/export — large storage (verify deep copy)", "[dll][string]")
{
    memepp::string src{ kLongString, (mmint_t)strlen(kLongString), memepp::string_storage_t::large };
    REQUIRE(src.storage_type() == memepp::string_storage_t::large);

    // --- import (const&) ---
    auto imported = memepp::import_from_dll<memepp::string>(
        src.native_handle(), MMSTR__OBJ_SIZE);
    REQUIRE(imported == src);
    REQUIRE(imported.storage_type() == memepp::string_storage_t::large);

    // Deep copy: a distinct allocation, independent of the source.
    REQUIRE(imported.data() != src.data());
    src = "replaced";
    REQUIRE(imported == kLongString);

    // --- import (&&): export to raw, import as rvalue ---
    {
        auto raw = memepp::export_into_dll<mmstrstk_t>(imported, MMSTR__OBJ_SIZE);
        auto imported2 = memepp::import_from_dll<memepp::string>(
            std::move(raw), MMSTR__OBJ_SIZE);
        REQUIRE(imported2 == kLongString);
        REQUIRE(imported2.data() != imported.data());
    }

    // --- export (const&): large shares (by design) ---
    const void* shared_ptr = static_cast<const void*>(imported.data());
    auto stk = memepp::export_into_dll<mmstrstk_t>(imported, MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE); });
    REQUIRE_CSTR_EQ(MemeString_cStr(memepp::to_pointer(stk)), kLongString);
    REQUIRE(MemeString_storageType(memepp::to_pointer(stk)) == MemeString_StorageType_large);

    // Shared storage: same underlying bytes, kept alive by the refcount.
    REQUIRE(static_cast<const void*>(MemeString_byteData(memepp::to_pointer(stk))) == shared_ptr);
    imported = "released";
    REQUIRE_CSTR_EQ(MemeString_cStr(memepp::to_pointer(stk)), kLongString);
}

TEST_CASE("memepp::string DLL import/export — export&& transfers ownership", "[dll][string]")
{
    memepp::string src{ kMediumString };
    REQUIRE(!src.empty());

    auto stk = memepp::export_into_dll<mmstrstk_t>(std::move(src), MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE); });

    REQUIRE_CSTR_EQ(MemeString_cStr(memepp::to_pointer(stk)), kMediumString);
    REQUIRE(src.empty());
}

// ---------------------------------------------------------------------------
// export&& per storage class: ownership moves, no copy.
// For shared storage the exported stack must own the SAME bytes the source
// used to own; for inline (small) storage the buffer is copied but the source
// is still emptied.
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string DLL export&& — large storage transfers the pointer", "[dll][string]")
{
    memepp::string src{ kLongString, (mmint_t)std::strlen(kLongString),
                        memepp::string_storage_t::large };
    REQUIRE(src.storage_type() == memepp::string_storage_t::large);

    const void* before = static_cast<const void*>(src.data());

    auto stk = memepp::export_into_dll<mmstrstk_t>(std::move(src), MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE); });

    // Ownership transfer: no allocation happened, the same bytes moved over.
    REQUIRE(static_cast<const void*>(MemeString_byteData(memepp::to_pointer(stk))) == before);
    REQUIRE(MemeString_storageType(memepp::to_pointer(stk)) == MemeString_StorageType_large);
    REQUIRE(src.empty());
}

TEST_CASE("memepp::string DLL export&& — user storage transfers the pointer", "[dll][string]")
{
    auto src = mm_from(std::string{ kLongString });
    REQUIRE(src.storage_type() == memepp::string_storage_t::user);

    const void* before = static_cast<const void*>(src.data());

    auto stk = memepp::export_into_dll<mmstrstk_t>(std::move(src), MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE); });

    REQUIRE(static_cast<const void*>(MemeString_byteData(memepp::to_pointer(stk))) == before);
    REQUIRE(MemeString_storageType(memepp::to_pointer(stk)) == MemeString_StorageType_user);
    REQUIRE(src.empty());
}

TEST_CASE("memepp::string DLL export&& — small storage empties the source", "[dll][string]")
{
    memepp::string src{ kSmallString };
    REQUIRE(src.storage_type() == memepp::string_storage_t::small);

    auto stk = memepp::export_into_dll<mmstrstk_t>(std::move(src), MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { mmstrstk_uninit_v0(&stk, MMSTR__OBJ_SIZE); });

    REQUIRE_CSTR_EQ(MemeString_cStr(memepp::to_pointer(stk)), kSmallString);
    REQUIRE(src.empty());
}

// ============================================================================
// memepp::buffer — import_from_dll / export_into_dll
// ============================================================================

TEST_CASE("memepp::buffer DLL import/export — basic", "[dll][buffer]")
{
    memepp::buffer src{ kBufData, kBufLen };
    REQUIRE(src.size() == kBufLen);

    // --- import (const&) ---
    auto imported = memepp::import_from_dll<memepp::buffer>(
        src.native_handle(), MMSTR__OBJ_SIZE);
    REQUIRE(imported == src);

    // --- import (&&) ---
    {
        auto raw = memepp::export_into_dll<mmbufstk_t>(imported, MMSTR__OBJ_SIZE);
        auto imported2 = memepp::import_from_dll<memepp::buffer>(
            std::move(raw), MMSTR__OBJ_SIZE);
        REQUIRE(imported2.size() == kBufLen);
    }

    // --- export (const&) ---
    auto stk = memepp::export_into_dll<mmbufstk_t>(imported, MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { MemeBufferStack_unInit(&stk, MMSTR__OBJ_SIZE); });
    REQUIRE(MemeBuffer_size(memepp::to_pointer(stk)) == kBufLen);

    // --- export (&&) ---
    auto imported3 = memepp::import_from_dll<memepp::buffer>(
        src.native_handle(), MMSTR__OBJ_SIZE);
    auto stk2 = memepp::export_into_dll<mmbufstk_t>(std::move(imported3), MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { MemeBufferStack_unInit(&stk2, MMSTR__OBJ_SIZE); });
    REQUIRE(MemeBuffer_size(memepp::to_pointer(stk2)) == kBufLen);
}

TEST_CASE("memepp::buffer DLL import/export — large storage (deep copy)", "[dll][buffer]")
{
    memepp::buffer src{ kBufLarge, sizeof(kBufLarge), memepp::buffer_storage_t::large };
    REQUIRE(src.storage_type() == memepp::buffer_storage_t::large);

    // --- import (const&) ---
    auto imported = memepp::import_from_dll<memepp::buffer>(
        src.native_handle(), MMSTR__OBJ_SIZE);
    REQUIRE(imported.size() == sizeof(kBufLarge));
    for (size_t i = 0; i < sizeof(kBufLarge); ++i)
        REQUIRE(imported.at(static_cast<mmint_t>(i)) == kBufLarge[i]);

    // Deep copy: a distinct allocation from the source buffer.
    REQUIRE(imported.data() != src.data());

    // --- import (&&) ---
    {
        auto raw = memepp::export_into_dll<mmbufstk_t>(imported, MMSTR__OBJ_SIZE);
        auto imported2 = memepp::import_from_dll<memepp::buffer>(
            std::move(raw), MMSTR__OBJ_SIZE);
        REQUIRE(imported2.size() == sizeof(kBufLarge));
        REQUIRE(imported2.data() != imported.data());
    }

    // --- export (const&): large shares ---
    const void* shared_ptr = static_cast<const void*>(imported.data());
    auto stk = memepp::export_into_dll<mmbufstk_t>(imported, MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { MemeBufferStack_unInit(&stk, MMSTR__OBJ_SIZE); });
    REQUIRE(MemeBuffer_size(memepp::to_pointer(stk)) == sizeof(kBufLarge));

    // Shared storage: same underlying bytes, kept alive by the refcount.
    REQUIRE(static_cast<const void*>(MemeBuffer_data(memepp::to_pointer(stk))) == shared_ptr);

    // Dropping our reference must not invalidate the exported stack.
    imported = memepp::buffer{ kBufData, kBufLen };
    REQUIRE(MemeBuffer_size(memepp::to_pointer(stk)) == sizeof(kBufLarge));
    REQUIRE(MemeBuffer_data(memepp::to_pointer(stk))[0] == kBufLarge[0]);
    REQUIRE(MemeBuffer_storageType(memepp::to_pointer(stk)) == MemeBuffer_StorageType_large);
}

TEST_CASE("memepp::buffer DLL import/export — export&& transfers ownership", "[dll][buffer]")
{
    memepp::buffer src{ kBufData, kBufLen };
    REQUIRE(!src.empty());

    auto stk = memepp::export_into_dll<mmbufstk_t>(std::move(src), MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { MemeBufferStack_unInit(&stk, MMSTR__OBJ_SIZE); });

    REQUIRE(MemeBuffer_size(memepp::to_pointer(stk)) == kBufLen);
    REQUIRE(src.empty());
}

TEST_CASE("memepp::buffer DLL export&& — large storage transfers the pointer", "[dll][buffer]")
{
    memepp::buffer src{ kBufLarge, sizeof(kBufLarge), memepp::buffer_storage_t::large };
    REQUIRE(src.storage_type() == memepp::buffer_storage_t::large);

    const void* before = static_cast<const void*>(src.data());

    auto stk = memepp::export_into_dll<mmbufstk_t>(std::move(src), MMSTR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { MemeBufferStack_unInit(&stk, MMSTR__OBJ_SIZE); });

    // Ownership transfer: no allocation happened, the same bytes moved over.
    REQUIRE(static_cast<const void*>(MemeBuffer_data(memepp::to_pointer(stk))) == before);
    REQUIRE(MemeBuffer_storageType(memepp::to_pointer(stk)) == MemeBuffer_StorageType_large);
    REQUIRE(src.empty());
}

// ============================================================================
// memepp::variant — import_from_dll / export_into_dll
// ============================================================================

TEST_CASE("memepp::variant DLL import/export — null variant", "[dll][variant]")
{
    memepp::variant src;
    REQUIRE(src.is_null());

    auto imported = memepp::import_from_dll<memepp::variant>(
        src.native_handle(), MMVAR__OBJ_SIZE);
    REQUIRE(imported.is_null());

    auto stk = memepp::export_into_dll<mmvarstk_t>(imported, MMVAR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { MemeVariantStack_unInit(&stk, MMVAR__OBJ_SIZE); });
}

TEST_CASE("memepp::variant DLL import/export — scalar types", "[dll][variant]")
{
    // int64
    {
        memepp::variant src{ static_cast<int64_t>(-42LL) };
        auto imported = memepp::import_from_dll<memepp::variant>(
            src.native_handle(), MMVAR__OBJ_SIZE);
        REQUIRE(imported.is_type(memepp::meta::typid::int64));
        REQUIRE(imported.get_or<int64_t>() == -42LL);

        auto stk = memepp::export_into_dll<mmvarstk_t>(imported, MMVAR__OBJ_SIZE);
        MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { MemeVariantStack_unInit(&stk, MMVAR__OBJ_SIZE); });
        auto rt = memepp::import_from_dll<memepp::variant>(stk, MMVAR__OBJ_SIZE);
        REQUIRE(rt.get_or<int64_t>() == -42LL);
    }

    // double
    {
        memepp::variant src{ 3.14159 };
        auto imported = memepp::import_from_dll<memepp::variant>(
            src.native_handle(), MMVAR__OBJ_SIZE);
        REQUIRE(imported.get_or<double>() == 3.14159);
    }

    // byte
    {
        memepp::variant src{ static_cast<mmbyte_t>(1) };
        auto imported = memepp::import_from_dll<memepp::variant>(
            src.native_handle(), MMVAR__OBJ_SIZE);
        REQUIRE(imported.get_or<mmbyte_t>() == 1);
    }
}

TEST_CASE("memepp::variant DLL import/export — containing string", "[dll][variant]")
{
    auto str = mm_from(std::string{ kLongString });
    memepp::variant src{ str };

    auto imported = memepp::import_from_dll<memepp::variant>(
        src.native_handle(), MMVAR__OBJ_SIZE);
    REQUIRE(imported.is_type(memepp::meta::typid::string));
    REQUIRE(imported.get_or<memepp::string>() == str);

    // export + round-trip
    auto stk = memepp::export_into_dll<mmvarstk_t>(imported, MMVAR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { MemeVariantStack_unInit(&stk, MMVAR__OBJ_SIZE); });
    auto rt = memepp::import_from_dll<memepp::variant>(stk, MMVAR__OBJ_SIZE);
    REQUIRE(rt.get_or<memepp::string>() == str);
}

TEST_CASE("memepp::variant DLL import/export — containing buffer", "[dll][variant]")
{
    memepp::buffer buf{ kBufData, kBufLen };
    memepp::variant src{ buf };

    auto imported = memepp::import_from_dll<memepp::variant>(
        src.native_handle(), MMVAR__OBJ_SIZE);
    REQUIRE(imported.is_type(memepp::meta::typid::buffer));
    auto out = imported.get_or<memepp::buffer>();
    REQUIRE(out.size() == kBufLen);
    for (mmint_t i = 0; i < static_cast<mmint_t>(kBufLen); ++i)
        REQUIRE(out.at(i) == kBufData[i]);

    // export &&
    auto stk = memepp::export_into_dll<mmvarstk_t>(std::move(imported), MMVAR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { MemeVariantStack_unInit(&stk, MMVAR__OBJ_SIZE); });
}

TEST_CASE("memepp::variant DLL import/export — export&& transfers ownership", "[dll][variant]")
{
    memepp::variant src{ static_cast<int64_t>(99LL) };
    auto stk = memepp::export_into_dll<mmvarstk_t>(std::move(src), MMVAR__OBJ_SIZE);
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([&]() { MemeVariantStack_unInit(&stk, MMVAR__OBJ_SIZE); });

    REQUIRE(src.is_null());
    auto rt = memepp::import_from_dll<memepp::variant>(stk, MMVAR__OBJ_SIZE);
    REQUIRE(rt.get_or<int64_t>() == 99LL);
}

// ============================================================================
// memepp::varts — import_from_dll / export_into_dll
// ============================================================================

TEST_CASE("memepp::varts DLL import/export — basic round-trip", "[dll][varts]")
{
    memepp::variant var{ static_cast<int64_t>(123LL) };
    memepp::varts src{ var, 1700000000LL, static_cast<int8_t>(1) };

    // --- import (const&) ---
    auto imported = memepp::import_from_dll<memepp::varts>(
        *reinterpret_cast<const mmvtsstk_t*>(&src), MMVTS__OBJ_SIZE);
    REQUIRE(imported.timestamp() == 1700000000LL);
    REQUIRE(imported.userdata() == 1);
    REQUIRE(imported.var().is_type(memepp::meta::typid::int64));
    REQUIRE(imported.var().get_or<int64_t>() == 123LL);

    // --- export (const&) ---
    auto stk = memepp::export_into_dll<mmvtsstk_t>(imported, MMVTS__OBJ_SIZE);
    REQUIRE(stk.ts == 1700000000ULL);
    REQUIRE(stk.userdata == 1);
    auto rt_var = memepp::import_from_dll<memepp::variant>(stk.var, MMVAR__OBJ_SIZE);
    REQUIRE(rt_var.get_or<int64_t>() == 123LL);
}

TEST_CASE("memepp::varts DLL import/export — nested string variant", "[dll][varts]")
{
    auto str = mm_from(std::string{ kLongString });
    memepp::variant var{ str };
    memepp::varts src{ var, 1700000000LL, static_cast<int8_t>(0) };

    // --- import (const&) ---
    auto imported = memepp::import_from_dll<memepp::varts>(
        *reinterpret_cast<const mmvtsstk_t*>(&src), MMVTS__OBJ_SIZE);
    REQUIRE(imported.timestamp() == 1700000000LL);
    REQUIRE(imported.userdata() == 0);
    REQUIRE(imported.var().get_or<memepp::string>() == str);

    // --- export (const&) ---
    auto stk = memepp::export_into_dll<mmvtsstk_t>(imported, MMVTS__OBJ_SIZE);
    REQUIRE(stk.ts == 1700000000ULL);
    auto rt_var = memepp::import_from_dll<memepp::variant>(stk.var, MMVAR__OBJ_SIZE);
    REQUIRE(rt_var.get_or<memepp::string>() == str);

    // --- export (&&) ---
    auto imported2 = memepp::import_from_dll<memepp::varts>(
        *reinterpret_cast<const mmvtsstk_t*>(&src), MMVTS__OBJ_SIZE);
    auto stk2 = memepp::export_into_dll<mmvtsstk_t>(std::move(imported2), MMVTS__OBJ_SIZE);
    auto rt_var2 = memepp::import_from_dll<memepp::variant>(stk2.var, MMVAR__OBJ_SIZE);
    REQUIRE(rt_var2.get_or<memepp::string>() == str);
}

TEST_CASE("memepp::varts DLL import/export — export&& transfers ownership", "[dll][varts]")
{
    memepp::variant var{ static_cast<double>(2.71828) };
    memepp::varts src{ var, 1700000000LL, static_cast<int8_t>(7) };

    auto stk = memepp::export_into_dll<mmvtsstk_t>(std::move(src), MMVTS__OBJ_SIZE);
    REQUIRE(stk.ts == 1700000000ULL);
    REQUIRE(stk.userdata == 7);

    auto rt_var = memepp::import_from_dll<memepp::variant>(stk.var, MMVAR__OBJ_SIZE);
    REQUIRE(rt_var.get_or<double>() == 2.71828);

    // The source must be left in a consistent empty state: variant cleared,
    // timestamp marked invalid, userdata zeroed.
    REQUIRE(src.var().is_null());
    REQUIRE(src.timestamp() == -1);
    REQUIRE(!src.is_ts_valid());
    REQUIRE(src.userdata() == 0);
}

TEST_CASE("memepp::varts DLL import/export — import rvalue", "[dll][varts]")
{
    memepp::variant var{ static_cast<int64_t>(99LL) };
    memepp::varts src{ var, 1700000000LL, static_cast<int8_t>(3) };

    // Get a mutable copy of the underlying C struct
    mmvtsstk_t raw;
    std::memcpy(&raw, &src, sizeof(mmvtsstk_t));

    auto imported = memepp::import_from_dll<memepp::varts>(
        std::move(raw), MMVTS__OBJ_SIZE);
    REQUIRE(imported.timestamp() == 1700000000LL);
    REQUIRE(imported.userdata() == 3);
    REQUIRE(imported.var().is_type(memepp::meta::typid::int64));
    REQUIRE(imported.var().get_or<int64_t>() == 99LL);
}
