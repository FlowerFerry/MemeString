#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/dll.hpp>

#include <memepp/convert/std/string.hpp>

#include <meme/string.h>

#include <cstring>
#include <memory>
#include <string>

#include <mego/util/os/windows/windows_simplify.h>
#if defined(_WIN32)
#else
#  include <dlfcn.h>
#endif

// ---------------------------------------------------------------------------
// Real module boundary: these tests dlopen/LoadLibrary a separately compiled
// plugin.  That is the only way to exercise what import_from_dll and
// export_into_dll actually exist for: a different CRT, a different allocator
// and a different copy of the meme runtime on the other side.
// ---------------------------------------------------------------------------

namespace {

// Diagnostics for a failed load: the raw platform error plus, on Windows, the
// formatted system message.  Reported through INFO() so a missing or broken
// plugin says *why* instead of just failing on `p.ok()`.
std::string g_open_error;

#if defined(_WIN32)
using module_handle = HMODULE;
using plugin_fn      = FARPROC;

void capture_open_error()
{
    const DWORD code = ::GetLastError();

    char* msg = nullptr;
    const DWORD len = ::FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<char*>(&msg), 0, nullptr);

    g_open_error  = "LoadLibraryA(\"mmpp_dll_plugin.dll\") failed, GetLastError=";
    g_open_error += std::to_string(static_cast<unsigned long>(code));
    if (len && msg) {
        std::string text{ msg, len };
        while (!text.empty() && (text.back() == '\n' || text.back() == '\r' || text.back() == ' '))
            text.pop_back();
        g_open_error += ": " + text;
    } else {
        g_open_error += " (no system message)";
    }
    if (msg)
        ::LocalFree(msg);
}

module_handle open_plugin()
{
    auto h = ::LoadLibraryA("mmpp_dll_plugin.dll");
    if (h == nullptr)
        capture_open_error();
    return h;
}
plugin_fn lookup(module_handle _m, const char* _name)
{
    return ::GetProcAddress(_m, _name);
}
void close_plugin(module_handle _m)
{
    // Deliberately unused: see plugin_module() for why the module is never
    // unloaded.  Kept so the pair stays symmetric if that ever changes.
    (void)_m;
}
#else
using module_handle = void*;
using plugin_fn      = void*;

module_handle open_plugin()
{
    auto h = ::dlopen("mmpp_dll_plugin.so", RTLD_NOW);
    if (h == nullptr) {
        const char* err = ::dlerror();
        g_open_error  = "dlopen(\"mmpp_dll_plugin.so\") failed";
        if (err)
            g_open_error += std::string{ ": " } + err;
    }
    return h;
}
plugin_fn lookup(module_handle _m, const char* _name)
{
    return ::dlsym(_m, _name);
}
void close_plugin(module_handle _m)
{
    if (_m) ::dlclose(_m);
}
#endif

const char kExpected[] =
    "plugin_payload_0123456789012345678901234567890123456789"
    "0123456789012345678901234567890123456789";

// Loads the plugin once for the whole test binary.
//
// IMPORTANT: the plugin is intentionally never unloaded.  Objects returned by
// import_from_dll have their payload copied, but the copy's internal refcount
// block still carries free/realloc function pointers that live in the plugin's
// code segment.  Releasing such an object after FreeLibrary/dlclose calls into
// an unmapped module, so the plugin must outlive every imported object --
// including any that Catch2 itself still holds while unwinding.  Letting the
// process teardown reclaim the module sidesteps the ordering problem entirely.
module_handle plugin_module()
{
    static module_handle handle = open_plugin();
    return handle;
}

struct plugin_guard
{
    module_handle handle = plugin_module();
    bool ok() const { return handle != nullptr; }
};

// Empty when the plugin loaded; the platform error text otherwise.  Tests feed
// this to Catch's INFO() so a failed REQUIRE(p.ok()) explains itself.
const std::string& load_diagnostic()
{
    return g_open_error;
}

} // anonymous namespace

TEST_CASE("plugin DLL — loads", "[dll][plugin]")
{
    plugin_guard p;
    INFO(load_diagnostic());
    REQUIRE(p.ok());
}

TEST_CASE("plugin DLL — import_from_dll deep-copies plugin memory", "[dll][plugin]")
{
    plugin_guard p;
    INFO(load_diagnostic());
    REQUIRE(p.ok());

    using make_fn = int (*)(mmstrstk_t*, mmint_t);
    auto make = reinterpret_cast<make_fn>(lookup(p.handle, "plugin_make_string"));
    REQUIRE(make != nullptr);

    {
        // Data produced inside the plugin's heap.
        mmstrstk_t raw;
        REQUIRE(make(&raw, MMSTR__OBJ_SIZE) == 0);
        REQUIRE(MemeString_byteSize(memepp::to_pointer(raw)) == static_cast<mmint_t>(std::strlen(kExpected)));

        const void* plugin_ptr = static_cast<const void*>(MemeString_byteData(memepp::to_pointer(raw)));

        // Import into the host: must be a copy, not a pointer into the plugin.
        auto imported = memepp::import_from_dll<memepp::string>(raw, MMSTR__OBJ_SIZE);
        mmstrstk_uninit_v0(&raw, MMSTR__OBJ_SIZE);

        REQUIRE(imported.size() == static_cast<memepp::string::size_type>(std::strlen(kExpected)));
        REQUIRE(std::memcmp(imported.data(), kExpected, std::strlen(kExpected)) == 0);
        REQUIRE(static_cast<const void*>(imported.data()) != plugin_ptr);
    }   // imported destroyed while the plugin is still mapped
}

TEST_CASE("plugin DLL — export_into_dll hands the plugin usable data", "[dll][plugin]")
{
    plugin_guard p;
    INFO(load_diagnostic());
    REQUIRE(p.ok());

    using check_fn = int (*)(const mmstrstk_t*, mmint_t);
    auto check = reinterpret_cast<check_fn>(lookup(p.handle, "plugin_check_string"));
    REQUIRE(check != nullptr);

    {
        memepp::string host{ kExpected, static_cast<memepp::string::size_type>(std::strlen(kExpected)) };

        auto raw = memepp::export_into_dll<mmstrstk_t>(host, MMSTR__OBJ_SIZE);
        const int seen = check(&raw, MMSTR__OBJ_SIZE);
        mmstrstk_uninit_v0(&raw, MMSTR__OBJ_SIZE);

        REQUIRE(seen == static_cast<int>(std::strlen(kExpected)));

        // The host object itself must be unaffected by the crossing.
        REQUIRE(host.size() == static_cast<memepp::string::size_type>(std::strlen(kExpected)));
    }
}

TEST_CASE("plugin DLL — round trip leaves the host object untouched", "[dll][plugin]")
{
    plugin_guard p;
    INFO(load_diagnostic());
    REQUIRE(p.ok());

    using rt_fn = int (*)(const mmstrstk_t*, mmint_t, mmstrstk_t*, mmint_t);
    auto roundtrip = reinterpret_cast<rt_fn>(lookup(p.handle, "plugin_roundtrip_string"));
    REQUIRE(roundtrip != nullptr);

    {
        memepp::string host{ kExpected, static_cast<memepp::string::size_type>(std::strlen(kExpected)) };

        auto raw_in  = memepp::export_into_dll<mmstrstk_t>(host, MMSTR__OBJ_SIZE);
        mmstrstk_t raw_out;
        REQUIRE(roundtrip(&raw_in, MMSTR__OBJ_SIZE, &raw_out, MMSTR__OBJ_SIZE) == 0);
        mmstrstk_uninit_v0(&raw_in, MMSTR__OBJ_SIZE);

        auto replied = memepp::import_from_dll<memepp::string>(raw_out, MMSTR__OBJ_SIZE);
        mmstrstk_uninit_v0(&raw_out, MMSTR__OBJ_SIZE);

        REQUIRE(replied.size() == host.size() + std::strlen("_from_plugin"));
        REQUIRE(std::memcmp(replied.data(), kExpected, std::strlen(kExpected)) == 0);
        REQUIRE(std::memcmp(replied.data() + host.size(), "_from_plugin", std::strlen("_from_plugin")) == 0);

        // The host's own string was never mutated.
        REQUIRE(host.size() == static_cast<memepp::string::size_type>(std::strlen(kExpected)));
        REQUIRE(std::memcmp(host.data(), kExpected, std::strlen(kExpected)) == 0);
    }   // replied destroyed while the plugin is still mapped
}

// ---------------------------------------------------------------------------
// host -> plugin for every host-side storage class.
//
// export_into_dll(const string&) does not take the same path for all storage
// kinds: small and medium are copied out of the host object, while large and
// user only bump a reference count.  All four must arrive intact on the plugin
// side, and the host object must survive the crossing unchanged.
// ---------------------------------------------------------------------------

TEST_CASE("plugin DLL — host -> plugin across all storage classes", "[dll][plugin]")
{
    plugin_guard p;
    INFO(load_diagnostic());
    REQUIRE(p.ok());

    using echo_fn = int (*)(const mmstrstk_t*, mmint_t, const char*, mmint_t);
    auto echo = reinterpret_cast<echo_fn>(lookup(p.handle, "plugin_echo_verify"));
    REQUIRE(echo != nullptr);

    // Query the library's own limits rather than hard-coding byte counts: the
    // small buffer's capacity depends on struct layout, which differs between
    // 32-bit and 64-bit builds.
    const auto small_max  = static_cast<size_t>(MemeStringOption_getStorageSmallLimit());
    const auto medium_max = static_cast<size_t>(MemeStringOption_getStorageMediumLimit());

    SECTION("small storage")
    {
        REQUIRE(small_max > 0);
        std::string small_text(small_max > 4 ? small_max - 4 : 1, 's');

        memepp::string host{ small_text.data(), static_cast<memepp::string::size_type>(small_text.size()) };
        REQUIRE(host.storage_type() == memepp::string_storage_t::small);

        auto raw = memepp::export_into_dll<mmstrstk_t>(host, MMSTR__OBJ_SIZE);
        const int seen = echo(&raw, MMSTR__OBJ_SIZE, small_text.data(), static_cast<mmint_t>(small_text.size()));
        mmstrstk_uninit_v0(&raw, MMSTR__OBJ_SIZE);

        REQUIRE(seen == static_cast<int>(small_text.size()));
        REQUIRE(host.size() == static_cast<memepp::string::size_type>(small_text.size()));
        REQUIRE(std::memcmp(host.data(), small_text.data(), small_text.size()) == 0);
    }

    SECTION("medium storage")
    {
        REQUIRE(medium_max > small_max);
        std::string medium_text(medium_max > 10 ? medium_max - 10 : small_max + 5, 'm');

        memepp::string host{ medium_text.data(), static_cast<memepp::string::size_type>(medium_text.size()) };
        REQUIRE(host.storage_type() == memepp::string_storage_t::medium);

        auto raw = memepp::export_into_dll<mmstrstk_t>(host, MMSTR__OBJ_SIZE);
        const int seen = echo(&raw, MMSTR__OBJ_SIZE, medium_text.data(), static_cast<mmint_t>(medium_text.size()));
        mmstrstk_uninit_v0(&raw, MMSTR__OBJ_SIZE);

        REQUIRE(seen == static_cast<int>(medium_text.size()));
        REQUIRE(host.size() == static_cast<memepp::string::size_type>(medium_text.size()));
        REQUIRE(std::memcmp(host.data(), medium_text.data(), medium_text.size()) == 0);
    }

    SECTION("large storage (shared by refcount, not copied)")
    {
        // Large storage: anything above medium_max
        std::string large_text(medium_max + 100, 'l');

        memepp::string host{ large_text.data(), static_cast<memepp::string::size_type>(large_text.size()),
                             memepp::string_storage_t::large };
        REQUIRE(host.storage_type() == memepp::string_storage_t::large);

        auto raw = memepp::export_into_dll<mmstrstk_t>(host, MMSTR__OBJ_SIZE);

        // Shared storage: the exported stack points at the host's own bytes.
        REQUIRE(static_cast<const void*>(MemeString_byteData(memepp::to_pointer(raw)))
                == static_cast<const void*>(host.data()));

        const int seen = echo(&raw, MMSTR__OBJ_SIZE, large_text.data(), static_cast<mmint_t>(large_text.size()));

        mmstrstk_uninit_v0(&raw, MMSTR__OBJ_SIZE);

        REQUIRE(seen == static_cast<int>(large_text.size()));
        // Host object still owns its data and is untouched.
        REQUIRE(host.storage_type() == memepp::string_storage_t::large);
        REQUIRE(std::memcmp(host.data(), large_text.data(), large_text.size()) == 0);
    }

    SECTION("user storage (shared by refcount, not copied)")
    {
        // mm_from(std::string&&) only takes over the buffer as "user" storage
        // above medium_limit + MMSTR__OBJ_SIZE (224 bytes on this build) and
        // only via the rvalue overload, so the payload is moved here.
        std::string big;
        for (int i = 0; i < 30; ++i)
            big.append("0123456789");   // 300 bytes
        const std::string expected = big;   // keep a copy for comparisons

        auto host = mm_from(std::move(big));
        REQUIRE(host.storage_type() == memepp::string_storage_t::user);

        auto raw = memepp::export_into_dll<mmstrstk_t>(host, MMSTR__OBJ_SIZE);

        REQUIRE(MemeString_storageType(memepp::to_pointer(raw)) == MemeString_StorageType_user);
        REQUIRE(static_cast<const void*>(MemeString_byteData(memepp::to_pointer(raw)))
                == static_cast<const void*>(host.data()));

        const int seen = echo(&raw, MMSTR__OBJ_SIZE, expected.data(), static_cast<mmint_t>(expected.size()));

        mmstrstk_uninit_v0(&raw, MMSTR__OBJ_SIZE);

        REQUIRE(seen == static_cast<int>(expected.size()));
        REQUIRE(host.storage_type() == memepp::string_storage_t::user);
        REQUIRE(std::memcmp(host.data(), expected.data(), expected.size()) == 0);
    }
}
