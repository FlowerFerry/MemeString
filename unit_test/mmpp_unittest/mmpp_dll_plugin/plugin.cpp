// A minimal MemeString plugin, compiled as its own shared library so that
// import_from_dll / export_into_dll cross a real module boundary (separate
// CRT, separate allocator, separate copy of the meme runtime).
//
// Exported entry points follow the usage pattern in AGENTS.md: only plain C
// structs (mmstrstk_t / mmbufstk_t / mmvarstk_t / mmvtsstk_t) cross the ABI.

#include <memepp/string.hpp>
#include <memepp/dll.hpp>

#include <meme/string.h>

#include <cstring>

#if defined(_WIN32)
#  define MMPP_PLUGIN_EXPORT extern "C" __declspec(dllexport)
#else
#  define MMPP_PLUGIN_EXPORT extern "C" __attribute__((visibility("default")))
#endif

namespace {

const char kPluginText[] =
    "plugin_payload_0123456789012345678901234567890123456789"
    "0123456789012345678901234567890123456789";

} // namespace

// Fills a caller-provided stack object with the plugin's own string.  A large
// payload forces heap storage so the host must not receive a pointer into this
// module's heap without copying.
MMPP_PLUGIN_EXPORT int plugin_make_string(mmstrstk_t* _out, mmint_t _struct_size)
{
    if (_out == nullptr || _struct_size <= 0)
        return -1;

    memepp::string s{ kPluginText, static_cast<memepp::string::size_type>(std::strlen(kPluginText)) };
    *_out = memepp::export_into_dll<mmstrstk_t>(s, _struct_size);
    return 0;
}

// Accepts a host-produced string, deep-copies it into the plugin, and reports
// the length the plugin sees.  Returns -1 when the payload does not match.
MMPP_PLUGIN_EXPORT int plugin_check_string(const mmstrstk_t* _s, mmint_t _struct_size)
{
    if (_s == nullptr)
        return -1;

    auto imported = memepp::import_from_dll<memepp::string>(*_s, _struct_size);
    if (imported.size() != static_cast<memepp::string::size_type>(std::strlen(kPluginText)))
        return -1;
    if (std::memcmp(imported.data(), kPluginText, std::strlen(kPluginText)) != 0)
        return -1;
    return static_cast<int>(imported.size());
}

// Generic host -> plugin echo: the plugin imports the caller's string, verifies
// it byte-for-byte against a caller-supplied expectation, and reports the length
// it saw.  Returns -1 on mismatch.
//
// Used to cover every host-side storage class (small / medium / large / user),
// since export_into_dll takes a different code path for each: small and medium
// are copied, large and user only bump a reference count.
MMPP_PLUGIN_EXPORT int plugin_echo_verify(
    const mmstrstk_t* _s, mmint_t _struct_size,
    const char* _expected, mmint_t _expected_len)
{
    if (_s == nullptr || _expected == nullptr || _expected_len < 0)
        return -1;

    auto imported = memepp::import_from_dll<memepp::string>(*_s, _struct_size);
    if (imported.size() != static_cast<memepp::string::size_type>(_expected_len))
        return -1;
    if (std::memcmp(imported.data(), _expected, static_cast<size_t>(_expected_len)) != 0)
        return -1;
    return static_cast<int>(imported.size());
}

// Round-trips a string through the plugin: import it, mutate the plugin-side
// copy, export it back into the caller's stack object.  The host's original
// must stay untouched.
MMPP_PLUGIN_EXPORT int plugin_roundtrip_string(
    const mmstrstk_t* _s, mmint_t _in_size,
    mmstrstk_t* _out, mmint_t _out_size)
{
    if (_out == nullptr || _out_size <= 0)
        return -1;

    if (_s == nullptr) {
        mmstrstk_init_v0(_out, _out_size);
        return 0;
    }

    auto imported = memepp::import_from_dll<memepp::string>(*_s, _in_size);
    auto modified = imported + memepp::string{ "_from_plugin" };
    *_out = memepp::export_into_dll<mmstrstk_t>(std::move(modified), _out_size);
    return 0;
}
